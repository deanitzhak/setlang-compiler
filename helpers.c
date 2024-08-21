#include "helpers.h"
#include <ctype.h>
#include "errors_hendler.h"
#include <stdbool.h>
int count_strings(char **array) {
    int count = 0;
    while (array[count] != NULL) count++;
    return count;
}


int is_relevant_char(char c) {
    return isalnum(c); 
}


char** split_string_toArray(char *input) {

    char **array = malloc(MAX_STRINGS * sizeof(char*));
    if (!array) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    int count = 0;
    input++;
    input[strlen(input) - 1] = '\0';
    char *token = strtok(input, ", ");
    while (token != NULL) {
        if (token[0] == '\"') {
            token++;
            token[strlen(token) - 1] = '\0';
        }
        array[count] = malloc((strlen(token) + 1) * sizeof(char));
        if (!array[count]) {
            fprintf(stderr, "Memory allocation error\n");
            exit(1);
        }
        strcpy(array[count], token);
        count++;
        token = strtok(NULL, ", ");
    }
    array[count] = NULL; // Null-terminate the array
    return array;
}


int my_strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}


int count_int_array(int *array, int sentinel) {
    int count = 0;
    while (array[count] != sentinel) {
        count++;
    }
    return count;
}


int count_char_array(char *array, char sentinel) {
    int count = 0;
    while (array[count] != sentinel) {
        count++;
    }
    return count;
}


char* int_to_string(int value) {
    char buffer[1024];
    sprintf(buffer, "%d", value);
    char* result = malloc(sizeof(char) * (strlen(buffer) + 1));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    strcpy(result, buffer);
    return result;
}


int is_number(const char *str) {

    if (*str == '\0') return 0; 
    if (*str == '-' || *str == '+') str++; 
    if (*str == '\0') return 0; 
    while (*str) {
        if (!isdigit(*str)) return 0; 
        str++;
    }
    return 1;
}


int evaluate_condition(int left, int right, const char* op) {
    if (strcmp(op, "<=") == 0) return left <= right;
    if (strcmp(op, ">=") == 0) return left >= right;
    if (strcmp(op, "==") == 0) return left == right;
    if (strcmp(op, "!=") == 0) return left != right;
    if (strcmp(op, "<") == 0) return left < right;
    if (strcmp(op, ">") == 0) return left > right;
    if (strcmp(op, "TRUE") == 0) return true;
    if (strcmp(op, "FALSE") == 0) return false;
    printf("Invalid operation: %s\n", op);
    handleError(NOT_SUPPORTED_ERROR);  
    return 0;
}


int evaluate_operation(int left, int right, const char* op) {

    if (strcmp(op, "+") == 0) return left + right;
    if (strcmp(op, "-") == 0) return left - right;
    if (strcmp(op, "*") == 0) return left * right;
    if (strcmp(op, "/") == 0) return left / right;
    if (strcmp(op, "%") == 0) return left % right;
    handleError(1);  
    return 0;
}


void prtin_pair(SplitStringPair *result){
    printf("Left: %s\n", result->left);
    printf("Right: %s\n", result->right);
}


SplitStringPair* split_string(char *input, char delimiter) {
    SplitStringPair *pair = malloc(sizeof(SplitStringPair));
    if (!pair) {
        handleError(MEMORY_ALLOCATION_ERROR);
    }
    char *delimiter_position = strchr(input, delimiter);
    if (!delimiter_position) {
        fprintf(stderr, "Delimiter not found in the input string\n");
        free(pair);
        return NULL;
    }
    size_t left_length = delimiter_position - input;
    size_t right_length = strlen(input) - left_length - 1;
    pair->left = malloc((left_length + 1) * sizeof(char));
    pair->right = malloc((right_length + 1) * sizeof(char));
    if (!pair->left || !pair->right) {
        fprintf(stderr, "Memory allocation failed\n");
        free(pair->left);
        free(pair->right);
        free(pair);
        exit(EXIT_FAILURE);
    }
    strncpy(pair->left, input, left_length);
    pair->left[left_length] = '\0';
    strcpy(pair->right, delimiter_position + 1);
    return pair;
}


char **merge_string_arrays(char **left, char **right, int left_size, int right_size) {
    int total_size = left_size + right_size;
    char **merged_array = malloc(total_size * sizeof(*merged_array));
    if (merged_array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < left_size; i++) {
        merged_array[i] = my_strdup(left[i]);
        if (merged_array[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for left[%d]\n", i);
            for (int j = 0; j < i; j++) {
                free(merged_array[j]);
            }
            free(merged_array);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < right_size; i++) {
        merged_array[i + left_size] = my_strdup(right[i]);
        if (merged_array[i + left_size] == NULL) {
            fprintf(stderr, "Memory allocation failed for right[%d]\n", i);
            for (int j = 0; j < left_size + i; j++) {
                free(merged_array[j]);
            }
            free(merged_array);
            exit(EXIT_FAILURE);
        }
    }

    return merged_array;
}


char *my_strdup(const char *s) {

    if (s == NULL) {
        return NULL;
    }
    char *dup = (char *)malloc(strlen(s) + 1);
    if (dup == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(dup, s);
    return dup;
}


char *trim_whitespace(char *str) {
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)
        return str;

    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    return str;
}


void remove_unwanted_chars(char *str) {
    char *src = str;
    char *dst = str;
    while (*src) {
        if (*src != '{' && *src != '}') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}


char** split_block_toStatements(char *block) {
    remove_unwanted_chars(block);

    char **statements = malloc(MAX_STATEMENTS * sizeof(char*));
    if (!statements) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    int count = 0;
    char *token = strtok(block, ";");
    while (token != NULL) {
        token = trim_whitespace(token);

        statements[count] = malloc((strlen(token) + 1) * sizeof(char));
        if (!statements[count]) {
            fprintf(stderr, "Memory allocation error\n");
            exit(1);
        }
        strcpy(statements[count], token);
        count++;

        token = strtok(NULL, ";");
    }
    statements[count] = NULL; 

    return statements;
}


char **subtract_string_arrays(char **left_array, char **right_array, int left_size, int right_size) {
    int result_size = left_size;  
    char **result = malloc(sizeof(char *) * (result_size + 1));
    if (!result) {
        handleError(MEMORY_ALLOCATION_ERROR);
        return NULL;
    }

    int i = 0;
    for (int j = 0; j < left_size; j++) {
        int found = 0;
        for (int k = 0; k < right_size; k++) {
            if (strcmp(left_array[j], right_array[k]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            result[i] = left_array[j];
            i++;
        } else {
            result_size--;  
        }
    }
    result[i] = NULL;  

    return result;
}


char* trim_leading_whitespace(const char* str) {
    while (isspace((unsigned char)*str)) str++;
    return (char*)str;
}


char** handle_block_literal(const char* block_content, int* num_statements) {
    if (block_content == NULL) {
        fprintf(stderr, "Error: Block content is NULL.\n");
        *num_statements = 0;
        return NULL;
    }

    char* content_copy = strdup(block_content);
    if (content_copy == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for content copy.\n");
        *num_statements = 0;
        return NULL;
    }

    char* line = strtok(content_copy, "\n");
    char** statements = NULL;
    int count = 0;

    while (line != NULL) {
        // Trim leading whitespace
        char* trimmed_line = trim_leading_whitespace(line);

        if (*trimmed_line != '\0') { // Skip empty lines
            // Allocate memory for the new statement
            char** temp_statements = realloc(statements, sizeof(char*) * (count + 1));
            if (temp_statements == NULL) {
                fprintf(stderr, "Error: Unable to allocate memory for statements array.\n");
                free(content_copy);
                for (int i = 0; i < count; ++i) {
                    free(statements[i]);
                }
                free(statements);
                *num_statements = 0;
                return NULL;
            }
            statements = temp_statements;
            statements[count] = strdup(trimmed_line); // Store the trimmed statement
            if (statements[count] == NULL) {
                fprintf(stderr, "Error: Unable to allocate memory for statement.\n");
                free(content_copy);
                for (int i = 0; i < count; ++i) {
                    free(statements[i]);
                }
                free(statements);
                *num_statements = 0;
                return NULL;
            }
            count++;
        }

        // Move to the next line
        line = strtok(NULL, "\n");
    }

    free(content_copy);

    char** temp_statements = realloc(statements, sizeof(char*) * (count + 1));
    if (temp_statements == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for NULL terminator.\n");
        for (int i = 0; i < count; ++i) {
            free(statements[i]);
        }
        free(statements);
        *num_statements = 0;
        return NULL;
    }
    statements = temp_statements;
    statements[count] = NULL; 

    *num_statements = count;
    return statements;
}


char* get_first_word(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    while (isspace((unsigned char)*str)) str++;

    const char* end = str;
    while (*end && !isspace((unsigned char)*end)) end++;

    size_t length = end - str;
    char* word = (char*)malloc(length + 1);
    if (word == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for first word.\n");
        return NULL;
    }

    strncpy(word, str, length);
    word[length] = '\0'; 

    return word;
}


int count_lsit(void **list){
    int count = 0;
    while (list[count] != NULL) count++;
    return count;
}

