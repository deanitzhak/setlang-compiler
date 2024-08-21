#include <stdio.h>
#include <stdlib.h>
#include "statement.h"
#include "data_holder.h"
#include "helpers.h"
#include "errors_hendler.h"
char *stmt_helper = "alfred";

bool execute_statement(Statement *statement,void *head) {
    if(head == NULL){
        handleError(NOT_SUPPORTED_ERROR);
        return;
    }
    switch (statement->type) {
        case OUTPUT_STATEMENT:
            execute_output_statement((char **)statement->statement,head);
            return true;
        case INPUT_STATEMENT:
            input_statement_execute((char **)statement->statement,head);
            return true;
        case ASSIGNMENT_STATEMENT:
            execute_assignment_statement(statement,head);
            return true;
        case IF_STATEMENT:
            return execute_if_statement((char **)statement->statement,head);
        case FOR_STATEMENT:
            return true;
        case FOR_STATEMENT_NO_BRACE:
            return true;
        case IF_ELSE_STATEMENT:
            return execute_if_statement((char **)statement->statement,head);
        default:
            fprintf(stderr, "Invalid statement type\n");
            break;
    }
}


void execute_output_statement(char **id, void *head) {
    Data_node *node = search_node((Data_node *)head, id[1]);
    if(node == NULL){
        printf("%s ", id[0]);
        printf("%d\n", atoi(id[1]));
        return;
    }
    printf("%s ", id[0]);
    print_node(node);
}


void input_statement_execute(char **statement, void *head) {
    Data_node *current_node = search_node(head, statement[2]);
    if (current_node == NULL) {
        handleError(NOT_SUPPORTED_ERROR);
        return;
    }

    char buffer[STRING_SIZE];
    printf("%s ", statement[1]);
    fflush(stdout);

    if (fgets(buffer, STRING_SIZE, stdin) == NULL) {
        handleError(MEMORY_ALLOCATION_ERROR);
        return;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    if (current_node->type == INT_VAR) {
        if (atoi(buffer) == 0 && buffer[0] != '0') {
            handleError(INVALID_INPUT_ERROR);
            return;
        }
        current_node->variables = atoi(buffer);
        return;
    } else if (current_node->type == STR_VAR) {
    size_t buffer_len = strlen(buffer);
    char *str_value = malloc(buffer_len + 3); 

    if (str_value == NULL) {
        handleError(MEMORY_ALLOCATION_ERROR);
        return;
    }

    str_value[0] = '"';
    strcpy(str_value + 1, buffer);
    str_value[buffer_len + 1] = '"';
    str_value[buffer_len + 2] = '\0';

    if (current_node->variables != NULL) {
        free(current_node->variables);
    }
    current_node->variables = (void *)str_value;

    printf("Quoted string assigned to current_node->variables: %s\n", str_value);
}
 else if (current_node->type == COLLECTION_VAR || current_node->type == SET_VAR) {
    char **current_array = split_string_toArray(buffer);
    if (current_array == NULL) {
        handleError(MEMORY_ALLOCATION_ERROR);
        return;
    }

    int CNT = 0;
    while (current_array[CNT] != NULL) {
        CNT++;
    }

    if (current_node->variables != NULL) {
        for (int i = 0; i < current_node->var_count; i++) {
            free(current_node->variables[i]);
        }
        free(current_node->variables);
    }

    current_node->variables = (void **)malloc(sizeof(void *) * (CNT + 1));
    if (current_node->variables == NULL) {
        handleError(MEMORY_ALLOCATION_ERROR);
        return;
    }

    for (int i = 0; i < CNT; i++) {
        size_t str_len = strlen(current_array[i]);
        char *quoted_str = malloc(str_len + 3);  
        if (quoted_str == NULL) {
            handleError(MEMORY_ALLOCATION_ERROR);
            return;
        }

        quoted_str[0] = '"';
        strcpy(quoted_str + 1, current_array[i]);
        quoted_str[str_len + 1] = '"';
        quoted_str[str_len + 2] = '\0';

        current_node->variables[i] = quoted_str;
    }
    current_node->variables[CNT] = NULL;
    current_node->var_count = CNT;

    free(current_array);
    } else {
        handleError(NOT_SUPPORTED_ERROR);
    }
}

    /** [main] = [left] op [right] <- other **/
void execute_assignment_statement(Statement *st, void *head) {
    bool success = false;
    char *op = st->statement[2];
    if (strcmp(op, "+") == 0){
        if (strcmp(st->statement[3], stmt_helper) == 0)
        {
            // [main] = [left] + [helper]
            success = plus_op_nodes( st->statement[3], st->statement[1], head);
            success = pluse_with_helper(st->statement[0], st->statement[1], stmt_helper ,head);
            return success;
        }
        success = plus_op_nodes(st->statement[1], st->statement[3], head);
        if (success == true) {
            return deep_copy_variables(st->statement[0], stmt_helper, head);
        }
    }else if (strcmp(op, "-") == 0){
        success = minus_op_nodes(st->statement[1], st->statement[3], head);
        if (success == true) {
            return  deep_copy_variables(st->statement[0], stmt_helper, head);
        }
    }else if (strcmp(op,"&") == 0)
    {
        success = and_op_nodes(st->statement[1], st->statement[3], head);
        if (success == true) { // if the operation was successful
            if (strcmp(st->statement[0], stmt_helper) != 0) { // if there is assaignment to a variable
                return  deep_copy_variables(st->statement[0], stmt_helper, head);
            }else{
                return  success; // if there is no assignment to a variable and opertaion was successful
            }
        }
    }
    
    
}


bool execute_if_statement(char **ids, void *head){
    return evaluate_node_condition(ids[0],ids[2],ids[1] ,head);
}
