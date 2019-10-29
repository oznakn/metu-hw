#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FALSE 0
#define TRUE 1

#define MAX_BUFFER_SIZE 1000

#define KEYWORD_COUNT 3

#define TOKEN_TYPE_END 0
#define TOKEN_TYPE_X 1
#define TOKEN_TYPE_NUMBER 2
#define TOKEN_TYPE_OPERATOR 3
#define TOKEN_TYPE_KEYWORD 4
#define TOKEN_TYPE_R 5

struct Lexer;
struct Token;
struct AST;
struct ASTItem;

struct Lexer {
    char *content;
    int length;
    int cursor;
};

struct Token {
    char *content;
    int type;
};

struct AST {
    struct ASTItem *first;
};

struct ASTItem {
    struct ASTItem *left_child;
    struct ASTItem *right_child;
    char *content;
    int type;
};

typedef struct Lexer Lexer;
typedef struct Token Token;
typedef struct AST AST;
typedef struct ASTItem ASTItem;

char *keywords_with_two_length[] = {"sh", "ch", "ln"};
char *keywords_with_three_length[] = {"sin", "tan", "cos"};

int str_len(char *s) {
    int i;

    for (i = 0; ; i++) {
        if (s[i] == 0) return i;
    }
}

int str_cmp(char *s1, char *s2, int length) {
    int i;

    for (i = 0; i < length; i++) {
        if (s1[i] != s2[i]) return FALSE;
    }

    return TRUE;
}

char *str_copy(char *s) {
    char *result;
    int i, length = str_len(s);

    result = calloc(length + 1, sizeof(char));

    for (i = 0; i < length; i++) {
        result[i] = s[i];
    }

    return result;
}

int is_char_digit(char c) {
    return 48 <= c && c <= 57;
}

char *create_string_from_buffer(char *buffer, int *buffer_length) {
    int i;
    char *result;

    result = calloc(*buffer_length + 1, sizeof(char));

    for (i = 0; i < *buffer_length; i++) {
        result[i] = buffer[i];
    }

    *buffer_length = 0;

    return result;
}

char *create_string_from_char(char c) {
    char *result_string;

    result_string = calloc(2, sizeof(char));
    result_string[0] = c;

    return result_string;
}

char *slice_string(char *s, int start, int end) {
    int i, string_length = end - start;
    char *result_string;

    result_string = calloc(string_length + 1, sizeof(char));

    for (i = 0; i < string_length; i++) {
        result_string[i] = s[i + start];
    }

    return result_string;
}

char *combine_strings(char *s1, char *s2) {
    int i, length1 = str_len(s1), length2 = str_len(s2);
    char *result_string;

    result_string = calloc(length1 + length2 + 1, sizeof(char));

    for (i = 0; i < length1; i++) {
        result_string[i] = s1[i];
    }

    for (i = 0; i < length2; i++) {
        result_string[length1 + i] = s1[i];
    }

    return result_string;
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(';
}

int string_to_int(char *s) {
    int i, j, length = str_len(s), result = 0;

    for (j = 0, i = length - 1; i >= 0; i--, j++) {
        result += (s[i] - 48) * (int) pow(10, j);
    }

    return result;
}

char *int_to_string(int n) {
    char buffer[MAX_BUFFER_SIZE], *s;
    int i, c = 0;

    while (n != 0) {
        buffer[c++] = (n % 10) + 48;
        n /= 10;
    }

    s = calloc(c + 1, sizeof(char));

    for (i = 0; i < c; i++) {
        s[i] = buffer[c - i - 1];
    }

    return s;
}

int get_operator_precedence(char operator, int next) {
    switch (operator) {
        case '(':
            return 5;

        case '^':
            return next ? 3 : 4;

        case '*':
        case '/':
            return 2;

        case '+':
        case '-':
            return 1;

        default:
            return 0;
    }
}

int get_operator_precedence_from_token(Token token, int next) {
    if (token.type != TOKEN_TYPE_OPERATOR) return 0;

    return get_operator_precedence(token.content[0], next);
}

void free_string(char *s) {
    free(s);
}

void print_token(Token token) {
    printf("Token: %s %i\n", token.content, token.type);
}

void free_token(Token token) {
    if (token.content != NULL) {
        free_string(token.content);
    }
}

Lexer *new_lexer(char *content) {
    Lexer *lexer = malloc(sizeof(Lexer));

    lexer->content = content;
    lexer->cursor = 0;
    lexer->length = str_len(content);

    return lexer;
}

int lexer_has_char(Lexer *lexer) {
    return lexer->cursor < lexer->length;
}

char lexer_peek_char(Lexer *lexer) {
    return lexer->content[lexer->cursor];
}

char lexer_next_char(Lexer *lexer) {
    return lexer->content[lexer->cursor++];
}

Token lexer_next_token(Lexer *lexer) {
    char buffer[MAX_BUFFER_SIZE];
    int buffer_length = 0;

    Token token;

    token.type = TOKEN_TYPE_END;
    token.content = NULL;

    if (lexer_has_char(lexer)) {
        char c = lexer_peek_char(lexer);

        while (c == ' ') {
            lexer_next_char(lexer);

            c = lexer_peek_char(lexer);
        }

        if (c == 'X') {
            lexer_next_char(lexer);

            token.type = TOKEN_TYPE_X;
            token.content = create_string_from_char(c);

            return token;
        }
        if (c == ')') {
            lexer_next_char(lexer);

            token.type = TOKEN_TYPE_R;
            token.content = create_string_from_char(c);

            return token;
        }
        else if (is_operator(c)) {
            lexer_next_char(lexer);

            token.type = TOKEN_TYPE_OPERATOR;
            token.content = create_string_from_char(c);

            return token;
        }
        else if (is_char_digit(c)) {
            while (is_char_digit(c)) {
                lexer_next_char(lexer);

                buffer[buffer_length++] = c;

                c = lexer_peek_char(lexer);
            }

            token.type = TOKEN_TYPE_NUMBER;
            token.content = create_string_from_buffer(buffer, &buffer_length);

            return token;
        }
        else {
            int i;

            while (TRUE) {
                lexer_next_char(lexer);

                buffer[buffer_length++] = c;

                if (buffer_length == 2) {
                    for (i = 0; i < KEYWORD_COUNT; i++) {
                        if (str_cmp(keywords_with_two_length[i], buffer, 2)) {
                            token.type = TOKEN_TYPE_KEYWORD;
                            token.content = create_string_from_buffer(buffer, &buffer_length);

                            return token;
                        }
                    }
                }
                else if (buffer_length == 3) {
                    for (i = 0; i < KEYWORD_COUNT; i++) {
                        if (str_cmp(keywords_with_three_length[i], buffer, 3)) {
                            token.type = TOKEN_TYPE_KEYWORD;
                            token.content = create_string_from_buffer(buffer, &buffer_length);

                            return token;
                        }
                    }
                }

                c = lexer_peek_char(lexer);
            }
        }
    }

    return token;
}

Token lexer_peek_token(Lexer *lexer) {
    int cursor = lexer->cursor;

    Token token = lexer_next_token(lexer);

    lexer->cursor = cursor;

    return token;
}

void lexer_skip_token(Lexer *lexer, Token token) {
    int length = str_len(token.content);

    lexer->cursor += length;

    free_token(token);
}

AST *new_ast() {
    AST *ast = malloc(sizeof(AST));

    return ast;
}

ASTItem *new_ast_item(char *content, int type) {
    ASTItem *ast_item = malloc(sizeof(ASTItem));

    ast_item->type = type;
    ast_item->content = content;
    ast_item->left_child = NULL;
    ast_item->right_child = NULL;

    return ast_item;
}

ASTItem *new_ast_item_from_token(Token token) {
    return new_ast_item(token.content, token.type);
}

void free_lexer(Lexer *lexer) {
    free(lexer);
}

void free_ast_item(ASTItem *ast_item) {
    if (ast_item->left_child != NULL) free_ast_item(ast_item->left_child);
    if (ast_item->right_child != NULL) free_ast_item(ast_item->right_child);

    free_string(ast_item->content);
    free(ast_item);
}

void free_ast(AST *ast) {
    free_ast_item(ast->first);

    free(ast);
}

ASTItem *parse_expression(Lexer *lexer, int precedence);

ASTItem *parse_infix_expression(Lexer *lexer, ASTItem *left) {
    Token curr_token = lexer_next_token(lexer);

    ASTItem *item = new_ast_item_from_token(curr_token);

    item->left_child = left;
    item->right_child = parse_expression(lexer, get_operator_precedence_from_token(curr_token, TRUE));

    return item;
}

ASTItem *parse_grouped_expression(Lexer *lexer) {
    ASTItem *result;

    Token curr_token = lexer_peek_token(lexer);
    if (curr_token.type == TOKEN_TYPE_OPERATOR && curr_token.content[0] == '(') {
        lexer_skip_token(lexer, curr_token);
    }
    else {
        free_token(curr_token);
    }

    result = parse_expression(lexer, 0);

    curr_token = lexer_peek_token(lexer);
    if (curr_token.type == TOKEN_TYPE_R)  {
        lexer_skip_token(lexer, curr_token);
    }
    else {
        free_token(curr_token);
    }

    return result;
}

ASTItem *parse_expression(Lexer *lexer, int precedence) {
    Token curr_token = lexer_peek_token(lexer);
    ASTItem *left;

    if (curr_token.type == TOKEN_TYPE_END) return NULL;

    if (curr_token.type == TOKEN_TYPE_OPERATOR && curr_token.content[0] == '(') {
        left = parse_grouped_expression(lexer);
    }
    else {
        Token ast_item_token = lexer_next_token(lexer);

        left = new_ast_item_from_token(ast_item_token);
    }

    free_token(curr_token);

    curr_token = lexer_peek_token(lexer);

    if (curr_token.type == TOKEN_TYPE_END) return left;

    while (precedence < get_operator_precedence_from_token(curr_token, FALSE)) {
        if (curr_token.type == TOKEN_TYPE_END) return left;

        free_token(curr_token);

        left = parse_infix_expression(lexer, left);

        curr_token = lexer_peek_token(lexer);
    }
    free_token(curr_token);

    return left;
}

AST *parse_ast(Lexer *lexer) {
    AST *ast = new_ast();

    ast->first = parse_expression(lexer, 0);

    return ast;
}

void print_ast_item(ASTItem *ast_item, int depth) {
    int i;

    for (i = 0; i < depth; i++) printf("\t");

    printf("ASTItem: %s\n", ast_item->content);

    if (ast_item->left_child != NULL) print_ast_item(ast_item->left_child, depth + 1);
    if (ast_item->right_child != NULL) print_ast_item(ast_item->right_child, depth + 1);
}

ASTItem *copy_ast_item(ASTItem *ast_item) {
    ASTItem *result;

    if (ast_item->type != TOKEN_TYPE_OPERATOR) {
        return new_ast_item(str_copy(ast_item->content), ast_item->type);
    }

    result = new_ast_item(str_copy(ast_item->content), ast_item->type);

    result->left_child = copy_ast_item(ast_item->left_child);
    result->right_child = copy_ast_item(ast_item->right_child);

    return result;
}

void print_ast(AST *ast) {
    print_ast_item(ast->first, 0);
}

ASTItem *create_addition_ast_item(ASTItem *left, ASTItem *right) {
    ASTItem *result;

    if (left->type == TOKEN_TYPE_NUMBER && str_len(right->content) == 1 && left->content[0] == '0') {
        free_ast_item(left);

        return right;
    }

    if (right->type == TOKEN_TYPE_NUMBER && str_len(right->content) == 1 && right->content[0] == '0') {
        free_ast_item(right);

        return left;
    }

    result = new_ast_item(create_string_from_char('+'), TOKEN_TYPE_OPERATOR);

    result->left_child = left;
    result->right_child = right;

    return result;
}

ASTItem *create_subtraction_ast_item(ASTItem *left, ASTItem *right) {
    ASTItem *result;

    if (right->type == TOKEN_TYPE_NUMBER && str_len(right->content) == 1 && right->content[0] == '0') {
        free_ast_item(right);

        return left;
    }

    result = new_ast_item(create_string_from_char('-'), TOKEN_TYPE_OPERATOR);

    result->left_child = left;
    result->right_child = right;

    return result;
}

ASTItem *create_multiplication_ast_item(ASTItem *left, ASTItem *right) {
    ASTItem *result;

    if (left->type == TOKEN_TYPE_NUMBER && str_len(right->content) == 1 && left->content[0] == '0') {
        free_ast_item(right);

        return left;
    }

    if (right->type == TOKEN_TYPE_NUMBER && str_len(right->content) == 1 && right->content[0] == '0') {
        free_ast_item(left);

        return right;
    }

    if (left->type == TOKEN_TYPE_NUMBER && str_len(right->content) == 1 && left->content[0] == '1') {
        free_ast_item(left);

        return right;
    }

    if (right->type == TOKEN_TYPE_NUMBER && str_len(right->content) == 1 && right->content[0] == '1') {
        free_ast_item(right);

        return left;
    }

    result = new_ast_item(create_string_from_char('*'), TOKEN_TYPE_OPERATOR);

    result->left_child = left;
    result->right_child = right;

    return result;
}

ASTItem *create_division_ast_item(ASTItem *left, ASTItem *right) {
    ASTItem *result;

    if (left->type == TOKEN_TYPE_NUMBER && str_len(left->content) == 1 && left->content[0] == '0') {
        free_ast_item(right);

        return left;
    }

    if (right->type == TOKEN_TYPE_NUMBER && str_len(right->content) == 1 && right->content[0] == '1') {
        free_ast_item(right);

        return left;
    }

    result = new_ast_item(create_string_from_char('/'), TOKEN_TYPE_OPERATOR);

    result->left_child = left;
    result->right_child = right;

    return result;
}

ASTItem *create_power_ast_item(ASTItem *left, ASTItem *right) {
    ASTItem *result;

    if (right->type == TOKEN_TYPE_NUMBER) {
        if (str_len(right->content) == 1 && right->content[0] == '0') {
            free_ast_item(left);
            free_ast_item(right);

            return new_ast_item(create_string_from_char('1'), TOKEN_TYPE_NUMBER);
        }

        if (str_len(right->content) == 1 && right->content[0] == '1') {
            free_ast_item(right);

            return left;
        }
    }

    result = new_ast_item(create_string_from_char('^'), TOKEN_TYPE_OPERATOR);

    result->left_child = left;
    result->right_child = right;

    return result;
}

ASTItem *take_derivative_of_ast_item(ASTItem *ast_item) {
    switch (ast_item->type) {
        case TOKEN_TYPE_NUMBER:
            return new_ast_item(create_string_from_char('0'), TOKEN_TYPE_NUMBER);

        case TOKEN_TYPE_X:
            return new_ast_item(create_string_from_char('1'), TOKEN_TYPE_NUMBER);

        case TOKEN_TYPE_KEYWORD:
            if (str_cmp(ast_item->content, "sh", 2)) {
                return new_ast_item(str_copy("ch"), TOKEN_TYPE_NUMBER);
            }
            else if (str_cmp(ast_item->content, "ch", 2)) {
                return new_ast_item(str_copy("sh"), TOKEN_TYPE_NUMBER);
            }
            else if (str_cmp(ast_item->content, "ln", 2)) {
                return create_division_ast_item(
                        new_ast_item(create_string_from_char('1'), TOKEN_TYPE_NUMBER),
                        new_ast_item(create_string_from_char('X'), TOKEN_TYPE_X)
                );
            }
            else if (str_cmp(ast_item->content, "sin", 3)) {
                return new_ast_item(str_copy("cos"), TOKEN_TYPE_NUMBER);
            }
            else if (str_cmp(ast_item->content, "tan", 3)) {
                return create_addition_ast_item(
                    create_power_ast_item(
                        new_ast_item(str_copy("tan"), TOKEN_TYPE_KEYWORD),
                        new_ast_item(create_string_from_char('2'), TOKEN_TYPE_NUMBER)
                    ),
                    new_ast_item(create_string_from_char('1'), TOKEN_TYPE_NUMBER)
                );
            }
            break;

        case TOKEN_TYPE_OPERATOR:
            if (ast_item->content[0] == '+') {
                return create_addition_ast_item(
                    take_derivative_of_ast_item(ast_item->left_child),
                    take_derivative_of_ast_item(ast_item->right_child)
                );
            }
            else if (ast_item->content[0] == '-') {
                return create_subtraction_ast_item(
                    take_derivative_of_ast_item(ast_item->left_child),
                    take_derivative_of_ast_item(ast_item->right_child)
                );
            }
            else if (ast_item->content[0] == '*') {
                return create_addition_ast_item(
                    create_multiplication_ast_item(
                        take_derivative_of_ast_item(ast_item->left_child),
                        copy_ast_item(ast_item->right_child)
                    ),
                    create_multiplication_ast_item(
                        take_derivative_of_ast_item(ast_item->right_child),
                        copy_ast_item(ast_item->left_child)
                    )
                );
            }
            else if (ast_item->content[0] == '/') {
                return create_division_ast_item(
                    create_subtraction_ast_item(
                        create_multiplication_ast_item(
                            take_derivative_of_ast_item(ast_item->left_child),
                            copy_ast_item(ast_item->right_child)
                        ),
                        create_multiplication_ast_item(
                            take_derivative_of_ast_item(ast_item->right_child),
                            copy_ast_item(ast_item->left_child)
                        )
                    ),
                    create_power_ast_item(
                        copy_ast_item(ast_item->right_child),
                        new_ast_item(create_string_from_char('2'), TOKEN_TYPE_NUMBER)
                    )
                );
            }
            else if (ast_item->content[0] == '^') {
                return create_multiplication_ast_item(
                    create_multiplication_ast_item(
                        copy_ast_item(ast_item->right_child),
                        create_power_ast_item(
                            copy_ast_item(ast_item->left_child),
                            new_ast_item(int_to_string(string_to_int(ast_item->right_child->content) - 1), ast_item->right_child->type)
                        )
                    ),
                    take_derivative_of_ast_item(ast_item->left_child)
                );
            }
            break;
    }

    return NULL;
}

AST *take_derivative(AST *ast) {
    AST *result = new_ast();

    result->first = take_derivative_of_ast_item(ast->first);

    return result;
}

char *ast_item_to_string(ASTItem *ast_item, int precedence, char operator) {
    int i, k, curr_precedence, length1, length2, length = 0;
    char *s, *s1, *s2;

    if (ast_item->type != TOKEN_TYPE_OPERATOR) {
        return str_copy(ast_item->content);
    }

    curr_precedence = get_operator_precedence(ast_item->content[0], FALSE);

    s1 = ast_item_to_string(ast_item->left_child, curr_precedence, ast_item->content[0]);
    s2 = ast_item_to_string(ast_item->right_child, curr_precedence, ast_item->content[0]);

    length1 = str_len(s1);
    length2 = str_len(s2);

    if (curr_precedence < precedence || (curr_precedence == precedence && ast_item->content[0] != operator)) {
        length = length1 + length2 + 3;

        k = 1;

        s = calloc(length + 1, sizeof(char));

        s[0] = '(';
        s[length - 1] = ')';
    }
    else {
        length = length1 + length2 + 1;

        k = 0;

        s = calloc(length + 1, sizeof(char));
    }

    for (i = 0; i < length1; i++) {
        s[i + k] = s1[i];
    }

    s[i + k] = ast_item->content[0];

    for (i = 0; i < length2; i++) {
        s[length1 + i + k + 1] = s2[i];
    }

    free_string(s1);
    free_string(s2);

    return s;
}

char *ast_to_string(AST *ast) {
    return ast_item_to_string(ast->first, -1, 0);
}

int main() {
    char c, buffer[MAX_BUFFER_SIZE], *result;
    int buffer_length = 0;

    Lexer *lexer;
    AST *ast, *result_ast;

    /* (X ^ 2 - 1) * tan - ln ^ 2 / X */
    char *input_string;

    while (TRUE) {
        c = getchar();

        if (c == EOF) break;

        if (9 <= c && c <= 13) c = ' ';

        buffer[buffer_length++] = c;
    }

    input_string = create_string_from_buffer(buffer, &buffer_length);

    lexer = new_lexer(input_string);
    ast = parse_ast(lexer);
    result_ast = take_derivative(ast);

    result = ast_to_string(result_ast);
    printf("%s\n", result);

    free_string(result);
    free_ast(result_ast);
    free_ast(ast);
    free_lexer(lexer);

    free_string(input_string);

    return 0;
}