#ifndef HELPERS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include <ctype.h>

#define STRING_SIZE  1024
#define BUFFER_SIZE 1024
#define MAX_STRINGS 100
#define MAX_STATEMENTS 100

typedef struct {
    char *left;
    char *right;
} SplitStringPair;


void prtin_pair(SplitStringPair *result);


int count_strings(char **array);


int is_relevant_char(char c);


char** split_string_toArray(char *input);


int my_strcmp(const char *str1, const char *str2);


int count_int_array(int *array, int sentinel) ;


int count_char_array(char *array, char sentinel);


char* int_to_string(int value);


int is_number(const char *str);


int evaluate_condition(int left, int right, const char* op);


int evaluate_operation(int left, int right, const char* op);


SplitStringPair* split_string(char *input, char delimiter);


char **merge_string_arrays(char **left, char **right, int left_size, int right_size);


char *my_strdup(const char *s);


char *trim_whitespace(char *str);


void remove_unwanted_chars(char *str);


char** split_block_toStatements(char *block);


char **subtract_string_arrays(char **left_array, char **right_array, int left_size, int right_size);


char** handle_block_literal(const char* block_content, int* num_statements);


char* trim_leading_whitespace(const char* str);


char* get_first_word(const char* str);

int count_lsit(void **list);
#endif