#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "ext2fs.h"

#define IS_DIR(inode) ((inode->mode >> 14) & 1)
#define IS_FILE(inode) ((inode->mode >> 15) & 1)
#define BIT_CHECK(p, b) *((p) + ((int) ((b) / 8))) & (1 << ((int) ((b) % 8)))
#define BIT_SET(p, b) *((p) + ((int) ((b) / 8))) |= (1 << ((int) ((b) % 8)))
#define BIT_CLEAR(p, b) *((p) + ((int) ((b) / 8))) &= ~(1 << ((int) ((b) % 8)))

#define DEBUG(args...) {}
#define ASSERT(val) {}

#define HASH_TABLE_SIZE 1009
#define DEFAULT_TIME 0x70dbae50

typedef struct ext2_super_block ext2_super_block_t;
typedef struct ext2_block_group_descriptor ext2_block_group_descriptor_t;
typedef struct ext2_inode ext2_inode_t;
typedef struct ext2_dir_entry ext2_dir_entry_t;

typedef struct uint32_llist_node {
	uint32_t val;
	struct uint32_llist_node *next;
	struct uint32_llist_node *prev;
} uint32_llist_node_t;

typedef struct {
	uint32_llist_node_t *head;
} uint32_llist_t;

uint32_llist_t uint32_llist_new();
void uint32_llist_push(uint32_llist_t *, uint32_t);
void uint32_llist_print(uint32_llist_t *);
void uint32_llist_free(uint32_llist_t *);

typedef struct {
	int fd;
	struct stat st;
	char *p;
	uint32_t block_size;
	uint32_t indirect_block_size;
	ext2_super_block_t *super_block;
	ext2_block_group_descriptor_t *group_desc_table;
} fs_t;

fs_t fs;

typedef struct {
	ext2_dir_entry_t *prev;
	ext2_dir_entry_t *curr;
} ext2_dir_entry_pair_t;

typedef struct {
	uint32_t inode_index;
	uint32_llist_t new_block_llist;
} fs_dup_result_t;

typedef struct {
	uint32_t inode_index;
	uint32_llist_t block_llist;
} fs_rm_result_t;


uint32_llist_t uint32_llist_new() {
	uint32_llist_t entry_list;

	entry_list.head = NULL;

	return entry_list;
}

void uint32_llist_push(uint32_llist_t *entry_list, uint32_t val) {
	uint32_llist_node_t *new_entry = malloc(sizeof(uint32_llist_node_t));
	new_entry->val = val;

	if (entry_list->head == NULL) {
		new_entry->prev = NULL;
		new_entry->next = NULL;

		entry_list->head = new_entry;
	} else {
		uint32_llist_node_t *head = entry_list->head;

		bool found = false;
		while (head->next != NULL) {
			if (head->val > val) {
				new_entry->prev = head->prev;
				new_entry->next = head;

				if (head->prev != NULL) {
					head->prev->next = new_entry;
				}
				head->prev = new_entry;

				if (head == entry_list->head) {
					entry_list->head = new_entry;
				}

				found = true;
				break;
			}

			head = head->next;
		}

		if (!found) {
			if (head->val < val) {
				new_entry->prev = head;
				new_entry->next = head->next;

				if (head->next != NULL) {
					head->next->prev = new_entry;
				}
				head->next = new_entry;
			} else {
				new_entry->prev = head->prev;
				new_entry->next = head;

				if (head->prev != NULL) {
					head->prev->next = new_entry;
				}
				head->prev = new_entry;

				if (head == entry_list->head) {
					entry_list->head = new_entry;
				}
			}
		}
	}
}


void uint32_llist_print(uint32_llist_t *entry_list) {
	uint32_llist_node_t *head = entry_list->head;

	if (head == NULL) {
		printf("-1\n");
	} else {
		while (head != NULL) {
			printf("%u ", head->val);

			head = head->next;
		}

		printf("\n");
	}
}

void uint32_llist_free(uint32_llist_t *entry_list) {
	uint32_llist_node_t *head = entry_list->head;

	while (head != NULL) {
		uint32_llist_node_t *next = head->next;

		free(head);

		head = next;
	}
}


static inline uint16_t calc_ext2_dir_entry_length(uint8_t name_length) {
	uint16_t length_should_be = 8 + (name_length * sizeof(char));
	uint16_t left = length_should_be % 4;

	if (left == 0) return length_should_be;

	return length_should_be + (4 - left);
}

static inline ext2_super_block_t *fs_super_block(void) {
	return (ext2_super_block_t *) &fs.p[EXT2_SUPER_BLOCK_POSITION];
}

static inline char *fs_data_block(uint32_t data_block_index) {
	return &fs.p[fs.block_size*data_block_index];
}

static inline bool is_block_used(uint32_t block_index) {
	uint32_t block_group_index = (uint32_t) ((block_index - fs.super_block->first_data_block) / fs.super_block->blocks_per_group);
	uint32_t block_offset = (block_index - fs.super_block->first_data_block) - block_group_index * fs.super_block->blocks_per_group;
	ASSERT(block_group_index >= 0);
	ASSERT(block_offset >= 0);

	ext2_block_group_descriptor_t *group_desc = &fs.group_desc_table[block_group_index];

	char *block_bitmap = fs_data_block(group_desc->block_bitmap);

	return BIT_CHECK(block_bitmap, block_offset);
}

static inline bool set_block_used(uint32_t block_index, bool status) {
	uint32_t block_group_index = (uint32_t) ((block_index - fs.super_block->first_data_block) / fs.super_block->blocks_per_group);
	uint32_t block_offset = (block_index - fs.super_block->first_data_block) - block_group_index * fs.super_block->blocks_per_group;
	ASSERT(block_group_index >= 0);
	ASSERT(block_offset >= 0);

	ext2_block_group_descriptor_t *group_desc = &fs.group_desc_table[block_group_index];

	char *block_bitmap = fs_data_block(group_desc->block_bitmap);

	bool is_used_before = BIT_CHECK(block_bitmap, block_offset);
	if (status && !is_used_before) {
		fs.super_block->free_block_count--;
		group_desc->free_block_count--;
	} else if (!status && is_used_before) {
		fs.super_block->free_block_count++;
		group_desc->free_block_count++;
	}

	if (status) BIT_SET(block_bitmap, block_offset);
	else BIT_CLEAR(block_bitmap, block_offset);
}

static inline ext2_inode_t *fs_inode(uint32_t inode_index) {
	uint32_t block_group_index = (size_t) ((inode_index - 1) / fs.super_block->inodes_per_group);
	uint32_t inode_offset = inode_index - 1 - block_group_index * fs.super_block->inodes_per_group;
	ASSERT(block_group_index >= 0);
	ASSERT(inode_offset >= 0);

	ext2_block_group_descriptor_t *group_desc = &fs.group_desc_table[block_group_index];

	char *inode_table = fs_data_block(group_desc->inode_table);

	return (ext2_inode_t *) (inode_table + fs.super_block->inode_size*inode_offset);
}

static inline bool is_inode_used(uint32_t inode_index) {
	uint32_t block_group_index = (size_t) ((inode_index - 1) / fs.super_block->inodes_per_group);
	uint32_t inode_offset = inode_index - 1 - block_group_index * fs.super_block->inodes_per_group;
	ASSERT(block_group_index >= 0);
	ASSERT(inode_offset >= 0);

	ext2_block_group_descriptor_t *group_desc = &fs.group_desc_table[block_group_index];

	char *inode_bitmap = fs_data_block(group_desc->inode_bitmap);

	return BIT_CHECK(inode_bitmap, inode_offset);
}

static inline bool set_inode_used(uint32_t inode_index, bool status) {
	uint32_t block_group_index = (size_t) ((inode_index - 1) / fs.super_block->inodes_per_group);
	uint32_t inode_offset = inode_index - 1 - block_group_index * fs.super_block->inodes_per_group;
	ASSERT(block_group_index >= 0);
	ASSERT(inode_offset >= 0);

	ext2_block_group_descriptor_t *group_desc = &fs.group_desc_table[block_group_index];

	char *inode_bitmap = fs_data_block(group_desc->inode_bitmap);

	bool is_used_before = BIT_CHECK(inode_bitmap, inode_offset);

	if (status && !is_used_before) {
		fs.super_block->free_inode_count--;
		group_desc->free_inode_count--;
	} else if (!status && is_used_before) {
		fs.super_block->free_inode_count++;
		group_desc->free_inode_count++;
	}

	if (status) BIT_SET(inode_bitmap, inode_offset);
	else {
		ext2_inode_t *inode = fs_inode(inode_index);
		inode->deletion_time = DEFAULT_TIME;

		BIT_CLEAR(inode_bitmap, inode_offset);
	}
}

static inline uint32_t *fetch_block_refmap_value(uint32_t block_index) {
	uint32_t block_group_index = (uint32_t) ((block_index - fs.super_block->first_data_block) / fs.super_block->blocks_per_group);
	uint32_t block_offset = (block_index - fs.super_block->first_data_block) - block_group_index * fs.super_block->blocks_per_group;
	ASSERT(block_group_index >= 0);
	ASSERT(block_offset >= 0);

	ext2_block_group_descriptor_t *group_desc = &fs.group_desc_table[block_group_index];

	uint32_t *block_bitmap = (uint32_t *) fs_data_block(group_desc->block_refmap);

	return &block_bitmap[block_offset];
}

static inline int init_fs(char *filename) {
	fs.fd = open(filename, O_RDWR);
	if (fs.fd < 0) {
		printf("File not found.");
		return -1;
	}

	if (fstat(fs.fd, &fs.st) < 0) {
		printf("File not found.");
		return -1;
	}

	fs.p = mmap(0, fs.st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fs.fd, 0) ;
	if (fs.p == MAP_FAILED) {
		printf("mmap failed.");
		return -1;
	}

	fs.super_block = fs_super_block();
	fs.block_size = EXT2_UNLOG(fs.super_block->log_block_size);
	fs.indirect_block_size = fs.block_size / sizeof(uint32_t);

	fs.group_desc_table = (ext2_block_group_descriptor_t *) fs_data_block(fs.super_block->first_data_block + 1);

	return 0;
}

static inline int close_fs() {
	if (munmap(fs.p, fs.st.st_size) != 0) {
		printf("munmap failed.");
		return -1;
	}

	return 0;
}

uint32_t fs_new_block() {
	for (uint32_t i = 0; i < fs.super_block->block_count; i++) {
		if (!is_block_used(i)) {
			set_block_used(i, true);

			uint32_t *ref_value = fetch_block_refmap_value(i);
			*ref_value = 1;

			char *data_block = fs_data_block(i);
			memset(data_block, 0, fs.block_size);

			return i;
		}
	}

	return 0;
}

uint32_t fs_new_inode() {
	for (uint32_t i = fs.super_block->first_inode; i < fs.super_block->inode_count; i++) {
		if (!is_inode_used(i)) {
			set_inode_used(i, true);

			return i;
		}
	}

	return 0;
}

static inline ext2_dir_entry_t *__fs_insert_dir_entry_block_helper(char *data_block, uint32_t parent_inode_index, uint32_t new_inode_index, char *name, uint8_t new_name_length, uint16_t new_dir_entry_length) {
	char *head = data_block;
	char *end = head + fs.block_size;

	while (head < end) {
		ext2_dir_entry_t *dir_entry = (ext2_dir_entry_t *) head;

		uint16_t length_should_be = calc_ext2_dir_entry_length(dir_entry->name_length);

		if (
			dir_entry->length != length_should_be &&
			new_dir_entry_length <= dir_entry->length - length_should_be
		) {
			ext2_dir_entry_t *new_dir_entry = (ext2_dir_entry_t *) (head + length_should_be);
			new_dir_entry->length = dir_entry->length - length_should_be;
			new_dir_entry->name_length = new_name_length;
			new_dir_entry->inode = new_inode_index;
			memcpy(new_dir_entry->name, name, new_dir_entry->name_length);

			dir_entry->length = length_should_be;

			return new_dir_entry;
		} else if (
			dir_entry->inode == 0 &&
			dir_entry->length >= new_dir_entry_length
		) {
			ext2_dir_entry_t *new_dir_entry = (ext2_dir_entry_t *) head;
			new_dir_entry->name_length = new_name_length;
			new_dir_entry->inode = new_inode_index;
			memcpy(new_dir_entry->name, name, new_dir_entry->name_length);

			return new_dir_entry;
		}

		head += dir_entry->length;
	}

	return NULL;
}

ext2_dir_entry_t *__fs_insert_dir_entry_new_block_helper(uint32_t *data_block, uint32_t parent_inode_index, uint32_t new_inode_index, char *name, uint8_t new_name_length, uint16_t new_dir_entry_length, uint32_llist_t *new_block_llist) {
	uint32_t new_block = fs_new_block();

	*data_block = new_block;
	uint32_llist_push(new_block_llist, new_block);

	char *head = fs_data_block(new_block);
	ext2_dir_entry_t *new_dir_entry = (ext2_dir_entry_t *) head;
	new_dir_entry->length = fs.block_size;
	new_dir_entry->name_length = new_name_length;
	new_dir_entry->inode = new_inode_index;
	memcpy(new_dir_entry->name, name, new_dir_entry->name_length);

	return new_dir_entry;
}

ext2_dir_entry_t *fs_insert_dir_entry(uint32_t parent_inode_index, uint32_t new_inode_index, char *name, uint32_llist_t *new_block_llist) {
	ext2_inode_t *parent_inode = fs_inode(parent_inode_index);
	uint8_t new_name_length = strlen(name);
	uint16_t new_dir_entry_length = calc_ext2_dir_entry_length(new_name_length);

	for (size_t i = 0; i < EXT2_NUM_DIRECT_BLOCKS; i++) {
		if (parent_inode->direct_blocks[i] != 0) {
			ext2_dir_entry_t *dir_entry = __fs_insert_dir_entry_block_helper(
				fs_data_block(parent_inode->direct_blocks[i]),
				parent_inode_index,
				new_inode_index,
				name,
				new_name_length,
				new_dir_entry_length
			);

			if (dir_entry != NULL) return dir_entry;
		}
	}

	if (parent_inode->single_indirect != 0) {
		uint32_t *single_data_block = (uint32_t *) fs_data_block(parent_inode->single_indirect);

		for (size_t i = 0; i < fs.indirect_block_size; i++) {
			if (single_data_block[i] != 0) {
				ext2_dir_entry_t *dir_entry = __fs_insert_dir_entry_block_helper(
					fs_data_block(single_data_block[i]),
					parent_inode_index,
					new_inode_index,
					name,
					new_name_length,
					new_dir_entry_length
				);

				if (dir_entry != NULL) return dir_entry;
			}
		}
	}

	if (parent_inode->double_indirect != 0) {
		uint32_t *double_data_block = (uint32_t *) fs_data_block(parent_inode->double_indirect);

		for (size_t j = 0; j < fs.indirect_block_size; j++) {
			if (double_data_block[j] != 0) {
				uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[j]);

				for (size_t i = 0; i < fs.indirect_block_size; i++) {
					if (single_data_block[i] != 0) {
						ext2_dir_entry_t *dir_entry = __fs_insert_dir_entry_block_helper(
							fs_data_block(single_data_block[i]),
							parent_inode_index,
							new_inode_index,
							name,
							new_name_length,
							new_dir_entry_length
						);

						if (dir_entry != NULL) return dir_entry;
					}
				}
			}
		}
	}

	if (parent_inode->triple_indirect != 0) {
		uint32_t *triple_data_block = (uint32_t *) fs_data_block(parent_inode->triple_indirect);

		for (size_t k = 0; k < fs.indirect_block_size; k++) {
			if (triple_data_block[k] != 0) {
				uint32_t *double_data_block = (uint32_t *) fs_data_block(triple_data_block[k]);

				for (size_t j = 0; j < fs.indirect_block_size; j++) {
					if (double_data_block[j] != 0) {
						uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[j]);

						for (size_t i = 0; i < fs.indirect_block_size; i++) {
							if (single_data_block[i] != 0) {
								ext2_dir_entry_t *dir_entry = __fs_insert_dir_entry_block_helper(
									fs_data_block(single_data_block[i]),
									parent_inode_index,
									new_inode_index,
									name,
									new_name_length,
									new_dir_entry_length
								);

								if (dir_entry != NULL) return dir_entry;
							}
						}
					}
				}
			}
		}
	}

	for (size_t i = 0; i < EXT2_NUM_DIRECT_BLOCKS; i++) {
		if (parent_inode->direct_blocks[i] == 0) {
			ext2_dir_entry_t *new_dir_entry = __fs_insert_dir_entry_new_block_helper(
				&parent_inode->direct_blocks[i],
				parent_inode_index,
				new_inode_index,
				name,
				new_name_length,
				new_dir_entry_length,
				new_block_llist
			);

			parent_inode->size += fs.block_size;
			parent_inode->block_count_512 += fs.block_size / 512;

			return new_dir_entry;
		}
	}

	if (parent_inode->single_indirect == 0) {
		parent_inode->single_indirect = fs_new_block();
		parent_inode->size += 2 * fs.block_size;
		parent_inode->block_count_512 += 2* (fs.block_size / 512);

		uint32_llist_push(new_block_llist, parent_inode->single_indirect);

		uint32_t *single_data_block = (uint32_t *) fs_data_block(parent_inode->single_indirect);

		ext2_dir_entry_t *new_dir_entry = __fs_insert_dir_entry_new_block_helper(
			&single_data_block[0],
			parent_inode_index,
			new_inode_index,
			name,
			new_name_length,
			new_dir_entry_length,
			new_block_llist
		);

		return new_dir_entry;
	}

	if (parent_inode->double_indirect == 0) {
		parent_inode->double_indirect = fs_new_block();
		parent_inode->size += 3 * fs.block_size;
		parent_inode->block_count_512 += 3 * (fs.block_size / 512);

		uint32_llist_push(new_block_llist, parent_inode->double_indirect);

		uint32_t *double_data_block = (uint32_t *) fs_data_block(parent_inode->double_indirect);

		double_data_block[0] = fs_new_block();

		uint32_llist_push(new_block_llist, double_data_block[0]);

		uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[0]);

		ext2_dir_entry_t *new_dir_entry = __fs_insert_dir_entry_new_block_helper(
			&single_data_block[0],
			parent_inode_index,
			new_inode_index,
			name,
			new_name_length,
			new_dir_entry_length,
			new_block_llist
		);

		return new_dir_entry;
	}


	if (parent_inode->triple_indirect == 0) {
		parent_inode->triple_indirect = fs_new_block();
		parent_inode->size += 4 * fs.block_size;
		parent_inode->block_count_512 += 4 * (fs.block_size / 512);

		uint32_llist_push(new_block_llist, parent_inode->triple_indirect);

		uint32_t *triple_data_block = (uint32_t *) fs_data_block(parent_inode->triple_indirect);

		triple_data_block[0] = fs_new_block();

		uint32_llist_push(new_block_llist, triple_data_block[0]);

		uint32_t *double_data_block = (uint32_t *) fs_data_block(triple_data_block[0]);

		double_data_block[0] = fs_new_block();

		uint32_llist_push(new_block_llist, double_data_block[0]);

		uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[0]);

		ext2_dir_entry_t *new_dir_entry = __fs_insert_dir_entry_new_block_helper(
			&single_data_block[0],
			parent_inode_index,
			new_inode_index,
			name,
			new_name_length,
			new_dir_entry_length,
			new_block_llist
		);

		return new_dir_entry;
	}

	return NULL;
}

static inline ext2_dir_entry_pair_t __fs_find_dir_entry_data_block_helper(ext2_inode_t *inode, char* data_block, bool (*fn)(ext2_dir_entry_t *, ext2_dir_entry_t *, void *), void *param) {
	char *head = data_block;
	char *end = head + fs.block_size;

	ext2_dir_entry_t *dir_entry = (ext2_dir_entry_t *) head;

	if ((* fn)(NULL, dir_entry, param)) {
		ext2_dir_entry_pair_t pair = {
			.prev = NULL,
			.curr = dir_entry,
		};

		return pair;
	}

	while (head + dir_entry->length < end) {
		ext2_dir_entry_t *next_dir_entry = (ext2_dir_entry_t *) (head + dir_entry->length);

		if ((* fn)(dir_entry, next_dir_entry, param)) {
			ext2_dir_entry_pair_t pair = {
				.prev = dir_entry,
				.curr = next_dir_entry,
			};

			return pair;
		}

		head += dir_entry->length;

		dir_entry = (ext2_dir_entry_t *) head;
	}

	ext2_dir_entry_pair_t pair = {
		.prev = NULL,
		.curr = NULL,
	};

	return pair;
}

ext2_dir_entry_pair_t fs_find_dir_entry_by_parent_and_fn(ext2_inode_t *inode, bool (*fn)(ext2_dir_entry_t *, ext2_dir_entry_t *, void *), void *param) {
	for (size_t i = 0; i < EXT2_NUM_DIRECT_BLOCKS; i++) {
		if (inode->direct_blocks[i] != 0) {
			ext2_dir_entry_pair_t pair = __fs_find_dir_entry_data_block_helper(inode, fs_data_block(inode->direct_blocks[i]), fn, param);

			if (pair.curr != NULL) return pair;
		}
	}

	if (inode->single_indirect != 0) {
		uint32_t *single_data_block = (uint32_t *) fs_data_block(inode->single_indirect);

		for (size_t i = 0; i < fs.indirect_block_size; i++) {
			if (single_data_block[i] != 0) {
				ext2_dir_entry_pair_t pair = __fs_find_dir_entry_data_block_helper(inode, fs_data_block(single_data_block[i]), fn, param);

				if (pair.curr != NULL) return pair;
			}
		}
	}

	if (inode->double_indirect != 0) {
		uint32_t *double_data_block = (uint32_t *) fs_data_block(inode->double_indirect);

		for (size_t j = 0; j < fs.indirect_block_size; j++) {
			if (double_data_block[j] != 0) {
				uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[j]);

				for (size_t i = 0; i < fs.indirect_block_size; i++) {
					if (single_data_block[i] != 0) {
						ext2_dir_entry_pair_t pair = __fs_find_dir_entry_data_block_helper(inode, fs_data_block(single_data_block[i]), fn, param);

						if (pair.curr != NULL) return pair;
					}
				}
			}
		}
	}

	if (inode->triple_indirect != 0) {
		uint32_t *triple_data_block = (uint32_t *) fs_data_block(inode->triple_indirect);

		for (size_t k = 0; k < fs.indirect_block_size; k++) {
			if (triple_data_block[k] != 0) {
				uint32_t *double_data_block = (uint32_t *) fs_data_block(triple_data_block[k]);

				for (size_t j = 0; j < fs.indirect_block_size; j++) {
					if (double_data_block[j] != 0) {
						uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[j]);

						for (size_t i = 0; i < fs.indirect_block_size; i++) {
							if (single_data_block[i] != 0) {
								ext2_dir_entry_pair_t pair = __fs_find_dir_entry_data_block_helper(inode, fs_data_block(single_data_block[i]), fn, param);

								if (pair.curr != NULL) return pair;
							}
						}
					}
				}
			}
		}
	}

	ext2_dir_entry_pair_t pair = {
		.prev = NULL,
		.curr = NULL,
	};

	return pair;
}

ext2_dir_entry_pair_t fs_find_dir_entry_by_fn(bool (*fn)(ext2_dir_entry_t *, ext2_dir_entry_t *, void *), void *param) {
	for (uint32_t i = 1; i <= fs.super_block->inode_count; i++) {
		if (is_inode_used(i)) {
			ext2_inode_t *inode = fs_inode(i);

			if (IS_DIR(inode)) {
				ext2_dir_entry_pair_t pair = fs_find_dir_entry_by_parent_and_fn(inode, fn, param);

				if (pair.curr != NULL) return pair;
			}
		}
	}

	ext2_dir_entry_pair_t pair = {
		.prev = NULL,
		.curr = NULL,
	};

	return pair;
}


static bool check_inode_exists_and_name(ext2_dir_entry_t *prev, ext2_dir_entry_t *curr, void *s) {
	return curr->inode != 0 && memcmp(curr->name, (char *) s, curr->name_length) == 0;
}

static bool check_inode(ext2_dir_entry_t *prev, ext2_dir_entry_t *curr, void *val) {
	uint32_t inode_index = *((uint32_t *) val);
	return curr->inode == inode_index;
}

static bool check_dir_entry(ext2_dir_entry_t *prev, ext2_dir_entry_t *curr, void *val) {
	return curr == (ext2_dir_entry_t *) val;
}


static inline ext2_dir_entry_pair_t fs_find_dir_entry_by_parent_and_name(uint32_t parent_inode_index, char *s) {
	ext2_inode_t *parent_inode = fs_inode(parent_inode_index);

	return fs_find_dir_entry_by_parent_and_fn(parent_inode, &check_inode_exists_and_name, (void *) s);
}

static inline ext2_dir_entry_pair_t fs_find_dir_entry_by_inode(uint32_t inode_index) {
	return fs_find_dir_entry_by_fn(&check_inode, (void *) &inode_index);
}

static inline ext2_dir_entry_pair_t fs_find_dir_entry_pair_by_dir_entry(ext2_dir_entry_t *dir_entry) {
	return fs_find_dir_entry_by_fn(&check_dir_entry, (void *) &dir_entry);
}


static inline void fs_rm_dir_entry(ext2_dir_entry_t *dir_entry) {
	dir_entry->inode = 0;

	ext2_dir_entry_pair_t pair = fs_find_dir_entry_pair_by_dir_entry(dir_entry);

	if (pair.prev != NULL) {
		pair.prev->length += pair.curr->length;
	}
}

static inline void fs_rm_block_ref(uint32_t block_index, uint32_llist_t *block_llist) {
	ASSERT(block_index > 0);
	uint32_t *ref_value = fetch_block_refmap_value(block_index);

	ASSERT(*ref_value > 0);

	(*ref_value) -= 1;

	if (*ref_value == 0) {
		set_block_used(block_index, false);
		uint32_llist_push(block_llist, block_index);
	}
}

void fs_rm_inode_link(ext2_inode_t *inode, uint32_t inode_index, uint32_llist_t *block_llist) {
	ASSERT(inode->link_count > 0);


	if (--inode->link_count == 0) {
		for (size_t i = 0; i < EXT2_NUM_DIRECT_BLOCKS; i++) {
			if (inode->direct_blocks[i] != 0) {
				fs_rm_block_ref(inode->direct_blocks[i], block_llist);
			}
		}

		if (inode->single_indirect != 0) {
			uint32_t *single_data_block = (uint32_t *) fs_data_block(inode->single_indirect);

			for (size_t i = 0; i < fs.indirect_block_size; i++) {
				if (single_data_block[i] != 0) {
					fs_rm_block_ref(single_data_block[i], block_llist);
				}
			}

			fs_rm_block_ref(inode->single_indirect, block_llist);
		}

		if (inode->double_indirect != 0) {
			uint32_t *double_data_block = (uint32_t *) fs_data_block(inode->double_indirect);

			for (size_t j = 0; j < fs.indirect_block_size; j++) {
				if (double_data_block[j] != 0) {
					uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[j]);

					for (size_t i = 0; i < fs.indirect_block_size; i++) {
						if (single_data_block[i] != 0) {
							fs_rm_block_ref(single_data_block[i], block_llist);
						}
					}

					fs_rm_block_ref(double_data_block[j], block_llist);
				}
			}

			fs_rm_block_ref(inode->double_indirect, block_llist);
		}

		if (inode->triple_indirect != 0) {
			uint32_t *triple_data_block = (uint32_t *) fs_data_block(inode->triple_indirect);

			for (size_t k = 0; k < fs.indirect_block_size; k++) {
				if (triple_data_block[k] != 0) {
					uint32_t *double_data_block = (uint32_t *) fs_data_block(triple_data_block[k]);

					for (size_t j = 0; j < fs.indirect_block_size; j++) {
						if (double_data_block[j] != 0) {
							uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[j]);

							for (size_t i = 0; i < fs.indirect_block_size; i++) {
								if (single_data_block[i] != 0) {
									fs_rm_block_ref(single_data_block[i], block_llist);
								}
							}

							fs_rm_block_ref(double_data_block[j], block_llist);
						}
					}

					fs_rm_block_ref(triple_data_block[k], block_llist);
				}
			}

			fs_rm_block_ref(inode->triple_indirect, block_llist);
		}

		set_inode_used(inode_index, false);
	}
}

uint32_t fs_find_inode_by_path(char *path) {
	uint32_t inode_index = EXT2_ROOT_INODE;

	size_t cursor = 0;
	size_t string_length = strlen(path);

	for (size_t i = 1; i < string_length + 1; i++) {
		if (i == string_length || path[i] == '/') {
			char *s = calloc(i - cursor + 1, sizeof(char));
			memcpy(s, &path[cursor], i - cursor);

			ext2_dir_entry_pair_t pair = fs_find_dir_entry_by_parent_and_name(inode_index, s);

			free(s);

			if (pair.curr == NULL) return inode_index;

			inode_index = pair.curr->inode;

			cursor = i + 1;
		}
	}

	return inode_index;
}

uint32_t fs_dup(uint32_t from_inode_index, uint32_t parent_inode_index, char *name, uint32_llist_t *new_block_llist) {
	ext2_inode_t *from_inode = fs_inode(from_inode_index);

	uint32_t new_inode_index = fs_new_inode();
	uint32_t new_block = -1;

	ext2_inode_t *target_inode = fs_inode(new_inode_index);
	memcpy(target_inode, from_inode, fs.super_block->inode_size);

	target_inode->link_count = 1;

	uint32_t *ref_value;

	for (size_t i = 0; i < EXT2_NUM_DIRECT_BLOCKS; i++) {
		if (target_inode->direct_blocks[i] != 0) {
			ref_value = fetch_block_refmap_value(target_inode->direct_blocks[i]);
			(*ref_value)++;
		}
	}

	if (target_inode->single_indirect != 0) {
		uint32_t *single_data_block = (uint32_t *) fs_data_block(target_inode->single_indirect);

		ref_value = fetch_block_refmap_value(target_inode->single_indirect);
		(*ref_value)++;

		for (size_t i = 0; i < fs.indirect_block_size; i++) {
			if (single_data_block[i] != 0) {
				ref_value = fetch_block_refmap_value(single_data_block[i]);
				(*ref_value)++;
			}
		}
	}

	if (target_inode->double_indirect != 0) {
		uint32_t *double_data_block = (uint32_t *) fs_data_block(target_inode->double_indirect);

		ref_value = fetch_block_refmap_value(target_inode->double_indirect);
		(*ref_value)++;

		for (size_t j = 0; j < fs.indirect_block_size; j++) {
			if (double_data_block[j] != 0) {
				uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[j]);

				ref_value = fetch_block_refmap_value(double_data_block[j]);
				(*ref_value)++;

				for (size_t i = 0; i < fs.indirect_block_size; i++) {
					if (single_data_block[i] != 0) {
						ref_value = fetch_block_refmap_value(single_data_block[i]);
						(*ref_value)++;
					}
				}
			}
		}
	}

	if (target_inode->triple_indirect != 0) {
		uint32_t *triple_data_block = (uint32_t *) fs_data_block(target_inode->triple_indirect);

		ref_value = fetch_block_refmap_value(target_inode->triple_indirect);
		(*ref_value)++;

		for (size_t k = 0; k < fs.indirect_block_size; k++) {
			if (triple_data_block[k] != 0) {
				uint32_t *double_data_block = (uint32_t *) fs_data_block(triple_data_block[k]);

				ref_value = fetch_block_refmap_value(triple_data_block[k]);
				(*ref_value)++;

				for (size_t j = 0; j < fs.indirect_block_size; j++) {
					if (double_data_block[j] != 0) {
						uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[j]);

						ref_value = fetch_block_refmap_value(double_data_block[j]);
						(*ref_value)++;

						for (size_t i = 0; i < fs.indirect_block_size; i++) {
							if (single_data_block[i] != 0) {
								ref_value = fetch_block_refmap_value(single_data_block[i]);
								(*ref_value)++;
							}
						}
					}
				}
			}
		}
	}

	fs_insert_dir_entry(parent_inode_index, new_inode_index, name, new_block_llist);

	return new_inode_index;
}

uint8_t hash_data_block(uint32_t block_index) {
	unsigned long hash = 0;

	char *data_block = fs_data_block(block_index);

	for (size_t i = 0; i < fs.block_size; i++) {
		hash = (hash << 5) + hash + *(data_block + i);
	}

	return (uint8_t) (hash % HASH_TABLE_SIZE);
}

void fs_replace_block(uint32_llist_t *share_inode_list, uint32_t old_block_index, uint32_t new_block_index) {
	uint32_t removal_count = 0;

	uint32_llist_node_t *head = share_inode_list->head;

	while (head != NULL) {
		ext2_inode_t *inode = fs_inode(head->val);

		for (size_t i = 0; i < EXT2_NUM_DIRECT_BLOCKS; i++) {
			if (inode->direct_blocks[i] == old_block_index) {
				inode->direct_blocks[i] = new_block_index;
				removal_count++;
			}
		}

		if (inode->single_indirect != 0) {
			uint32_t *single_data_block = (uint32_t *) fs_data_block(inode->single_indirect);

			for (size_t i = 0; i < fs.indirect_block_size; i++) {
				if (single_data_block[i] == old_block_index) {
					single_data_block[i] = new_block_index;
					removal_count++;
				}
			}
		}

		if (inode->double_indirect != 0) {
			uint32_t *double_data_block = (uint32_t *) fs_data_block(inode->double_indirect);

			for (size_t j = 0; j < fs.indirect_block_size; j++) {
				if (double_data_block[j] != 0) {
					uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[j]);

					for (size_t i = 0; i < fs.indirect_block_size; i++) {
						if (single_data_block[i] == old_block_index) {
							single_data_block[i] = new_block_index;
							removal_count++;
						}
					}
				}
			}
		}

		if (inode->triple_indirect != 0) {
			uint32_t *triple_data_block = (uint32_t *) fs_data_block(inode->triple_indirect);

			for (size_t k = 0; k < fs.indirect_block_size; k++) {
				if (triple_data_block[k] != 0) {
					uint32_t *double_data_block = (uint32_t *) fs_data_block(triple_data_block[k]);

					for (size_t j = 0; j < fs.indirect_block_size; j++) {
						if (double_data_block[j] != 0) {
							uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[j]);

							for (size_t i = 0; i < fs.indirect_block_size; i++) {
								if (single_data_block[i] == old_block_index) {
									single_data_block[i] = new_block_index;
									removal_count++;
								}
							}
						}
					}
				}
			}
		}

		head = head->next;
	}

	uint32_t *ref_value;

	ref_value = fetch_block_refmap_value(old_block_index);
	(*ref_value) -= removal_count;

	if (*ref_value == 0) {
		set_block_used(old_block_index, false);
	}

	ref_value = fetch_block_refmap_value(new_block_index);
	(*ref_value) += removal_count;
}

void fs_share(uint32_llist_t *share_inode_list, uint32_llist_t *hash_table) {
	uint8_t hash_value;

	uint32_llist_node_t *head = share_inode_list->head;

	while (head != NULL) {
		ext2_inode_t *inode = fs_inode(head->val);

		for (size_t i = 0; i < EXT2_NUM_DIRECT_BLOCKS; i++) {
			if (inode->direct_blocks[i] != 0) {
				hash_value = hash_data_block(inode->direct_blocks[i]);
				uint32_llist_push(&hash_table[hash_value], inode->direct_blocks[i]);
			}
		}

		if (inode->single_indirect != 0) {
			uint32_t *single_data_block = (uint32_t *) fs_data_block(inode->single_indirect);

			for (size_t i = 0; i < fs.indirect_block_size; i++) {
				if (single_data_block[i] != 0) {
					hash_value = hash_data_block(single_data_block[i]);
					uint32_llist_push(&hash_table[hash_value], single_data_block[i]);
				}
			}
		}

		if (inode->double_indirect != 0) {
			uint32_t *double_data_block = (uint32_t *) fs_data_block(inode->double_indirect);

			for (size_t j = 0; j < fs.indirect_block_size; j++) {
				if (double_data_block[j] != 0) {
					uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[j]);

					for (size_t i = 0; i < fs.indirect_block_size; i++) {
						if (single_data_block[i] != 0) {
							hash_value = hash_data_block(single_data_block[i]);
							uint32_llist_push(&hash_table[hash_value], single_data_block[i]);
						}
					}
				}
			}
		}

		if (inode->triple_indirect != 0) {
			uint32_t *triple_data_block = (uint32_t *) fs_data_block(inode->triple_indirect);

			for (size_t k = 0; k < fs.indirect_block_size; k++) {
				if (triple_data_block[k] != 0) {
					uint32_t *double_data_block = (uint32_t *) fs_data_block(triple_data_block[k]);

					for (size_t j = 0; j < fs.indirect_block_size; j++) {
						if (double_data_block[j] != 0) {
							uint32_t *single_data_block = (uint32_t *) fs_data_block(double_data_block[j]);

							for (size_t i = 0; i < fs.indirect_block_size; i++) {
								if (single_data_block[i] != 0) {
									hash_value = hash_data_block(single_data_block[i]);
									uint32_llist_push(&hash_table[hash_value], single_data_block[i]);
								}
							}
						}
					}
				}
			}
		}

		head = head->next;
	}

	uint8_t *block_counter = calloc(fs.super_block->block_count, sizeof(uint8_t));

	for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
		uint32_llist_t *curr_block_list = &hash_table[i];

		uint32_llist_node_t *head1 = curr_block_list->head;

		while (head1 != NULL) {
			block_counter[head1->val]++;

			uint32_llist_node_t *head2 = head1->next;

			while (head2 != NULL) {
				if (head1->val == head2->val) {
					head2->prev->next = head2->next;

					if (head2->next != NULL) {
						head2->next->prev = head2->prev;
					}

					block_counter[head2->val]++;

					uint32_llist_node_t *node_will_be_deleted = head2;

					head2 = head2->next;

					free(node_will_be_deleted);
				} else {
					head2 = head2->next;
				}
			}

			head1 = head1->next;
		}
	}

	bool *hash_table_used = calloc(HASH_TABLE_SIZE, sizeof(bool));
	bool *block_used = calloc(fs.super_block->block_count, sizeof(bool));

	for (uint32_t i = 0; i < fs.super_block->block_count; i++) {
		if (block_counter[i] > 0) {
			uint8_t hash_value = hash_data_block(i);

			if (hash_table_used[hash_value]) continue;
			hash_table_used[hash_value] = true;

			uint32_llist_t *curr_block_list = &hash_table[hash_value];

			uint32_llist_node_t *head1 = curr_block_list->head;

			while (head1 != NULL) {
				uint32_llist_node_t *head2 = head1->next;

				bool found = false;
				while (head2 != NULL) {
					if (!block_used[head2->val] && memcmp(fs_data_block(head1->val), fs_data_block(head2->val), fs.block_size) == 0) {
						block_used[head2->val] = true;

						if (!found) {
							printf("%u %u:%u ", head1->val, head1->val, block_counter[head1->val]);
							found = true;
						}

						printf("%u:%u ", head2->val, block_counter[head2->val]);

						fs_replace_block(share_inode_list, head2->val, head1->val);
					}

					head2 = head2->next;
				}

				if (found) {
					printf("\n");
				}

				head1 = head1->next;
			}
		}
	}

	free(block_used);
	free(hash_table_used);
	free(block_counter);
}

fs_dup_result_t command_dup(int argc, char **argv) {
	char *source = argv[3];
	char *dest = argv[4];
	char *name;

	uint32_t from_inode_index, parent_inode_index;

	if (source[0] != '/') {
		from_inode_index = strtol(source, NULL, 10);
	} else {
		from_inode_index = fs_find_inode_by_path(source + sizeof(char));
	}

	if (dest[0] != '/') {
		for (size_t i = 0; i < strlen(dest); i++) {
			if (dest[i] == '/') {
				char *s = calloc(i + 1, sizeof(char));
				memcpy(s, dest, i);
				parent_inode_index = strtol(s, NULL, 10);
				free(s);

				name = &dest[i+1];

				break;
			}
		}
	} else {
		parent_inode_index = fs_find_inode_by_path(dest + sizeof(char));

		int cursor = 0;
		for (size_t i = strlen(dest) - 1; i >= 0; i--) {
			if (dest[i] == '/') {
				cursor = i;
				break;
			}
		}

		name = &dest[cursor + 1];
	}

	uint32_llist_t new_block_llist = uint32_llist_new();

	uint32_t new_inode_index = fs_dup(from_inode_index, parent_inode_index, name, &new_block_llist);

	fs_dup_result_t r = {
		.inode_index = new_inode_index,
		.new_block_llist = new_block_llist,
	};

	return r;
}

fs_rm_result_t command_rm(int argc, char **argv) {
	char *source = argv[3];

	ext2_dir_entry_t *dir_entry;

	if (source[0] != '/') {
		uint32_t parent_inode_index;
		char *name;

		for (size_t i = 0; i < strlen(source); i++) {
			if (source[i] == '/') {
				char *s = calloc(i + 1, sizeof(char));
				memcpy(s, source, i);
				parent_inode_index = strtol(s, NULL, 10);
				free(s);

				name = &source[i+1];

				break;
			}
		}

		ext2_dir_entry_pair_t pair = fs_find_dir_entry_by_parent_and_name(parent_inode_index, name);
		dir_entry = pair.curr;
	} else {
		uint32_t inode_index = fs_find_inode_by_path(source + sizeof(char));
		ext2_dir_entry_pair_t pair = fs_find_dir_entry_by_inode(inode_index);
		dir_entry = pair.curr;
	}

	uint32_llist_t block_llist = uint32_llist_new();

	uint32_t inode_index = dir_entry->inode;
	fs_rm_dir_entry(dir_entry);

	ext2_inode_t *inode = fs_inode(inode_index);
	fs_rm_inode_link(inode, inode_index, &block_llist);

	fs_rm_result_t result = {
		.inode_index = inode_index,
		.block_llist = block_llist,
	};

	return result;
}

void command_share(int argc, char **argv) {
	uint32_llist_t *hash_table = malloc(HASH_TABLE_SIZE * sizeof(uint32_llist_t));

	for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
		hash_table[i] = uint32_llist_new();
	}

	uint32_llist_t share_inode_list = uint32_llist_new();

	for (int i = 3; i < argc; i++) {
		uint32_t inode_index = fs_find_inode_by_path(argv[i] + sizeof(char));
		uint32_llist_push(&share_inode_list, inode_index);
	}

	fs_share(&share_inode_list, hash_table);

	uint32_llist_free(&share_inode_list);

	for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
		uint32_llist_free(&hash_table[i]);
	}

	free(hash_table);
}

int start(int argc, char **argv) {
	char *command = argv[1];
	char *filename = argv[2];

	if (init_fs(filename) != 0) return -1;

	if (strcmp(command, "dup") == 0) {
		if (argc < 5) {
			printf("Wrong number of arguments.");
			return -1;
		}

		fs_dup_result_t r = command_dup(argc, argv);
		printf("%d\n", r.inode_index);
		uint32_llist_print(&r.new_block_llist);

		uint32_llist_free(&r.new_block_llist);
	} else if (strcmp(command, "rm") == 0) {
		if (argc < 4) {
			printf("Wrong number of arguments.");
			return -1;
		}

		fs_rm_result_t r = command_rm(argc, argv);
		printf("%d\n", r.inode_index);
		uint32_llist_print(&r.block_llist);

		uint32_llist_free(&r.block_llist);
	} else if (strcmp(command, "share") == 0) {
		if (argc < 4) {
			printf("Wrong number of arguments.");
			return -1;
		}

		command_share(argc, argv);
	}

	if (close_fs() != 0) return -1;
}

int main(int argc, char *argv[]) {
	char **new_argv = calloc(argc, sizeof(char *));

	for (size_t i = 0; i < argc; i++) {
		size_t s_len = strlen(argv[i]);

		if (argv[i][0] == '"' && argv[i][s_len - 1] == '"') {
			new_argv[i] = calloc(s_len - 1, sizeof(char));

			memcpy(new_argv[i], argv[i] + sizeof(char), s_len - 1);
		} else {
			new_argv[i] = calloc(s_len + 1, sizeof(char));

			memcpy(new_argv[i], argv[i], s_len);
		}
	}

	int r = start(argc, new_argv);

	for (size_t i = 0; i < argc; i++) {
		free(new_argv[i]);
	}

	free(new_argv);

	return r;
}
