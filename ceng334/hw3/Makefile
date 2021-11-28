all: compile

compile: ext2fs.h ext2fs.c
	$(CC) ext2fs.h ext2fs.c -o ext2sutils

valgrind_memcheck: compile
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./ext2sutils

gdb: compile
	gdb ./ext2sutils

clean:
	rm -f ext2sutils

pack:
	rm -f hw3.tar.gz
	tar czf hw3.tar.gz Makefile ext2fs.h ext2fs.c

create:
	dd if=/dev/zero of=initial.img bs=1024 count=512
	mke2fs -t ext2 -b 2048 -N 64 initial.img
	cp initial.img initial-converted.img
	./robin convert initial-converted.img

mount:
	fuseext2 -o rw+ active.img /fs-root

umount:
	fusermount -u /fs-root

reset:
	cp initial-converted.img active.img

test1: compile
	cp base-test1.img active.img
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./ext2sutils dup active.img /hello.txt /hello2.txt

test2: compile
	cp base-test2.img active.img
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./ext2sutils rm active.img /hello.txt

test2_2: compile
	cp base-test2-2.img active.img
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./ext2sutils rm active.img /hello2.txt

test_3: compile
	cd io-bonus/share/small_mixed_2048 && \
		cp input.img.bk input.img && \
		valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes $$(cat cmd)

test_share: compile
	cd io-bonus/share/small_mixed_2048 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)
	cd io-bonus/share/constant_4096 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)
	cd io-bonus/share/many_copy_1024 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)

test_dup: compile
	cd io-basic/dup/abspath_1024 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)
	cd io-basic/dup/big_1024 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)
	cd io-basic/dup/empty_2048 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)
	cd io-basic/dup/full_2048 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)
	cd io-basic/dup/simple_4096 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)

test_rm: compile
	cd io-basic/rm/big_1024 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)
	cd io-basic/rm/dups_2048 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)
	cd io-basic/rm/first_2048 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)
	cd io-basic/rm/hard_link_4096 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)
	cd io-basic/rm/lots_2048 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)
	cd io-basic/rm/simple_1024 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)
	cd io-basic/rm/simple_2048 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)

test_bonus_dup: compile
	cd io-bonus/big-dup/simple_triple_1024 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img

	cd io-bonus/big-dup/double_trouble_2048 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)


test_bonus_rm: compile
	cd io-bonus/big-rm/big_file_2048 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)

	cd io-bonus/big-rm/big_dir_2048 && \
		cp input.img.bk input.img && \
		((($$(cat cmd) | diff -Z output.txt -) && (echo 'PASS')) || (echo 'FAIL')) && \
		../../../robin check input.img && \
		(e2fsck -fnv input.img || true)

test_bench: test_dup test_rm test_bonus_dup test_bonus_rm
