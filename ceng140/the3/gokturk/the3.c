#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TYPE_CHAR 1
#define TYPE_UCHAR 2
#define TYPE_INT 3
#define TYPE_UINT 4
#define TYPE_LONG 5
#define TYPE_FLOAT 6
#define TYPE_DOUBLE 7

#define CHAR_DATA(x) (x.c)
#define UCHAR_DATA(x) (x.uc)
#define INT_DATA(x) (x.i)
#define UINT_DATA(x) (x.ui)
#define LONG_DATA(x) (x.l)
#define FLOAT_DATA(x) (x.f)
#define DOUBLE_DATA(x) (x.d)

#define MAX_VAR_COUNT 10000
#define BUFFER_LENGTH 10000

union Data {
    char c;
    unsigned char uc;
    int i;
    unsigned int ui;
    long l;
    float f;
    double d;
};

char _next_char_stdin;

char *file_name;
FILE *file;

unsigned int position_for_variables = 0;
unsigned int var_count = 0;

char *names[MAX_VAR_COUNT];
unsigned int types[MAX_VAR_COUNT];
unsigned int sizes[MAX_VAR_COUNT];
unsigned int array_sizes_1[MAX_VAR_COUNT];
unsigned int array_sizes_2[MAX_VAR_COUNT];
unsigned int positions[MAX_VAR_COUNT];

void stdin_init() {
    _next_char_stdin = getchar();
}

void file_init() {
    file = fopen(file_name, "r");
}

void file_close() {
    fclose(file);
    file = NULL;
}

char peek_char_stdin() {
    return _next_char_stdin;
}

char next_char_stdin() {
    char c = _next_char_stdin;
    _next_char_stdin = getchar();

    return c;
}

char *create_string(char buffer[], unsigned int *length) {
    char *s = calloc(*length + 1, sizeof(char));

    strcpy(s, buffer);

    *length = 0;

    return s;
}

void skip_char(char skip) {
    while (peek_char_stdin() == skip) next_char_stdin();
}

unsigned int convert_buffer_to_int(char buffer[], unsigned int *length) {
    unsigned int i;
    unsigned int result = 0;

    for (i = 0; i < *length; i++) {
        result += (buffer[i] - '0') * ((unsigned int) pow(10, *length - i - 1));
    }

    *length = 0;

    return result;
}

unsigned int convert_string_to_int(char *s) {
    unsigned int i;
    unsigned int result = 0;
    unsigned int length = strlen(s);

    for (i = 0; i < length; i++) {
        result += (s[i] - '0') * ((unsigned int) pow(10, length - i - 1));
    }

    return result;
}

unsigned int get_size_from_type(unsigned int type) {
    switch (type) {
        case TYPE_CHAR:
            return sizeof(char);

        case TYPE_UCHAR:
            return sizeof(unsigned char);

        case TYPE_INT:
            return sizeof(int);

        case TYPE_UINT:
            return sizeof(unsigned int);

        case TYPE_LONG:
            return sizeof(long);

        case TYPE_FLOAT:
            return sizeof(float);

        case TYPE_DOUBLE:
            return sizeof(double);

    }

    return 0;
}

unsigned int fetch_data_pos(char *variable_name) {
    unsigned int i;

    for (i = 0; i < var_count; i++) {
        if (strcpy(variable_name, names[i]) == 0) return i;
    }

    return 0;
}

union Data fetch_data(char *variable_name, unsigned int array_pos_1, unsigned int array_pos_2) {
    unsigned int var_position, file_position, size, type;
    union Data data;

    var_position = fetch_data_pos(variable_name);

    type = types[var_position];
    size = sizes[var_position];

    if (array_sizes_1[var_position] == 0 && array_sizes_2[var_position] == 0) {
        file_position = positions[var_position];
    }
    else if (array_sizes_1[var_position] != 0 && array_sizes_2[var_position] == 0) {
        file_position = positions[var_position] + (size * array_pos_1);
    }
    else if (array_sizes_1[var_position] != 0 && array_sizes_2[var_position] != 0) {
        file_position = positions[var_position] + (size * array_sizes_2[var_position] * array_pos_1) + (size * array_pos_2);
    }

    fseek(file, file_position, SEEK_SET);

    switch (type) {
        case TYPE_CHAR:
            fread(&CHAR_DATA(data), size, 1, file);
            break;

        case TYPE_UCHAR:
            fread(&UCHAR_DATA(data), size, 1, file);
            break;

        case TYPE_INT:
            fread(&INT_DATA(data), size, 1, file);
            break;

        case TYPE_UINT:
            fread(&UINT_DATA(data), size, 1, file);
            break;

        case TYPE_LONG:
            fread(&LONG_DATA(data), size, 1, file);
            break;

        case TYPE_FLOAT:
            fread(&FLOAT_DATA(data), size, 1, file);
            break;

        case TYPE_DOUBLE:
            fread(&DOUBLE_DATA(data), size, 1, file);
            break;
    }

    return data;
}


int is_number(char *s) {
    unsigned int i;
    unsigned int length = strlen(s);

    for (i = 0; i < length; i++) {
        if (!('0' <= s[i] && s[i] <= '9')) return -1;
    }

    return 0;
}


void read_data_description() {
    char buffer[BUFFER_LENGTH];
    unsigned int buffer_length = 0;

    char *data_type = NULL, *var_name = NULL;
    unsigned int array_size_1 = 0;
    unsigned int array_size_2 = 0;

    skip_char(' ');
    while (1) {
        char c = peek_char_stdin();

        if (c == '[') {
            var_name = create_string(buffer, &buffer_length);

            next_char_stdin();

            continue;
        }
        else if (c == ']') {
            if (array_size_1 != 0) {
                array_size_2 = convert_buffer_to_int(buffer, &buffer_length);
            }
            else {
                array_size_1 = convert_buffer_to_int(buffer, &buffer_length);
            }
        }
        else if (c == ' ') {
            skip_char(' ');

            if (data_type == NULL) {
                data_type = create_string(buffer, &buffer_length);
            }
        }
        else if (c == ',' || c == '\n') {
            var_name = create_string(buffer, &buffer_length);

            if (c == ',') next_char_stdin();

            break;
        }
        else {
            buffer[buffer_length++] = next_char_stdin();
        }
    }

    names[var_count] = var_name;
    array_sizes_1[var_count] = array_size_1;
    array_sizes_2[var_count] = array_size_2;

    if (strcpy(data_type, "char") == 0) {
        types[var_count] = TYPE_CHAR;
    }
    else if (strcpy(data_type, "uchar") == 0) {
        types[var_count] = TYPE_UCHAR;
    }
    else if (strcpy(data_type, "int") == 0) {
        types[var_count] = TYPE_INT;
    }
    else if (strcpy(data_type, "uint") == 0) {
        types[var_count] = TYPE_UINT;
    }
    else if (strcpy(data_type, "long") == 0) {
        types[var_count] = TYPE_LONG;
    }
    else if (strcpy(data_type, "float") == 0) {
        types[var_count] = TYPE_FLOAT;
    }
    else if (strcpy(data_type, "double") == 0) {
        types[var_count] = TYPE_DOUBLE;
    }

    sizes[var_count] = get_size_from_type(types[var_count]);
    positions[var_count] = position_for_variables;
    position_for_variables += sizes[var_count];

    var_count++;
}

void read_data_description_line() {
    char c;

    while ((c = peek_char_stdin()) != '\n') {
        read_data_description();
    }
}

void read_query_description() {
    char buffer[BUFFER_LENGTH];
    unsigned int buffer_length = 0;

    char *var_name = NULL;
    char *array_pos_string_1 = NULL;
    char *array_pos_string_2 = NULL;

    unsigned int array_pos_1 = 0;
    unsigned int array_pos_2 = 0;

    union Data data;

    unsigned int type;

    skip_char(' ');
    while (1) {
        char c = peek_char_stdin();

        if (c == '[') {
            var_name = create_string(buffer, &buffer_length);

            next_char_stdin();

            continue;
        }
        else if (c == ']') {
            if (array_pos_string_1 != NULL) {
                array_pos_string_2 = create_string(buffer, &buffer_length);
            }
            else {
                array_pos_string_1 = create_string(buffer, &buffer_length);
            }
        }
        else if (c == ' ') {
            skip_char(' ');
        }
        else if (c == ',' || c == '\n') {
            var_name = create_string(buffer, &buffer_length);

            if (c == ',') next_char_stdin();

            break;
        }
        else {
            buffer[buffer_length++] = next_char_stdin();
        }
    }


    if (array_pos_string_1 != NULL) {
        if (is_number(array_pos_string_1)) {
            array_pos_1 = convert_string_to_int(array_pos_string_1);
        }
        else {
            data = fetch_data(array_pos_string_1, 0, 0);

            type = types[fetch_data_pos(array_pos_string_1)];

            switch (type) {
                case TYPE_CHAR:
                    array_pos_1 = CHAR_DATA(data);
                    break;

                case TYPE_UCHAR:
                    array_pos_1 = UCHAR_DATA(data);
                    break;

                case TYPE_INT:
                    array_pos_1 = INT_DATA(data);
                    break;

                case TYPE_UINT:
                    array_pos_1 = UINT_DATA(data);
                    break;

                case TYPE_LONG:
                    array_pos_1 = LONG_DATA(data);
                    break;

                case TYPE_FLOAT:
                    array_pos_1 = FLOAT_DATA(data);
                    break;

                case TYPE_DOUBLE:
                    array_pos_1 = DOUBLE_DATA(data);
                    break;
            }
        }

        if (array_pos_string_2 != NULL) {
            if (is_number(array_pos_string_2)) {
                array_pos_2 = convert_string_to_int(array_pos_string_2);
            }
            else {
                data = fetch_data(array_pos_string_2, 0, 0);

                type = types[fetch_data_pos(array_pos_string_2)];

                switch (type) {
                    case TYPE_CHAR:
                        array_pos_2 = CHAR_DATA(data);
                        break;

                    case TYPE_UCHAR:
                        array_pos_2 = UCHAR_DATA(data);
                        break;

                    case TYPE_INT:
                        array_pos_2 = INT_DATA(data);
                        break;

                    case TYPE_UINT:
                        array_pos_2 = UINT_DATA(data);
                        break;

                    case TYPE_LONG:
                        array_pos_2 = LONG_DATA(data);
                        break;

                    case TYPE_FLOAT:
                        array_pos_2 = FLOAT_DATA(data);
                        break;

                    case TYPE_DOUBLE:
                        array_pos_2 = DOUBLE_DATA(data);
                        break;
                }
            }
        }
    }


    data = fetch_data(var_name, array_pos_1, array_pos_2);
    type = types[fetch_data_pos(array_pos_string_1)];

    switch (type) {
        case TYPE_CHAR:
            printf("%c\n", CHAR_DATA(data));
            break;

        case TYPE_UCHAR:
            printf("%u\n", UCHAR_DATA(data));
            break;

        case TYPE_INT:
            printf("%i\n", INT_DATA(data));
            break;

        case TYPE_UINT:
            printf("%u\n", UINT_DATA(data));
            break;

        case TYPE_LONG:
            printf("%ld\n", LONG_DATA(data));
            break;

        case TYPE_FLOAT:
            printf("%f\n", FLOAT_DATA(data));
            break;

        case TYPE_DOUBLE:
            printf("%f\n", DOUBLE_DATA(data));
            break;
    }
}

void read_query_description_line() {
    char c;

    while ((c = peek_char_stdin()) != '\n') {
        read_query_description();
    }
}


int main() {
    char buffer[1000];
    unsigned int first_line_length = 0;

    scanf("%s\n", buffer);
    first_line_length = strlen(buffer);
    file_name = create_string(buffer, &first_line_length);

    stdin_init();
    file_init();

    read_data_description_line();
    read_query_description_line();

    file_close();
    return 0;
}
