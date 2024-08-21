#ifndef DATA_HOLDER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Data_node {
    int type;
    char *key;
    void **variables;
    int var_count;
    struct Data_node *next;
    struct Data_node *prev;
} Data_node;

#define COLLECTION_VAR 0
#define SET_VAR 1
#define STR_VAR 2
#define INT_VAR 3
#define HELPER 846 



Data_node* initialize_node(int type, const char *key);


void add_node(Data_node **head, Data_node *new_node);


void add_variable(Data_node *node, void *value);


Data_node* search_node(Data_node *head, const char *key);


void print_all_nodes(Data_node *head);


void print_node(Data_node *node);


void delete_node(Data_node **head, const char *key);


void free_node(Data_node *node) ;


void free_all_nodes(Data_node *head);


void** get_variables(Data_node *node);


char* get_key(Data_node *node);


int get_type(Data_node *node);


Data_node* get_next(Data_node *node);


Data_node* get_prev(Data_node *node) ;


void update_variables(Data_node *node, void **new_variables, int new_var_count);

void update_variables_by_key(char *key, void **new_variables, int new_var_count,Data_node *head);


Data_node* copy_node(Data_node *node);

bool    and_op_nodes(char *node1_name, char *node2_name, void *head);

bool    are_nodes_equal(char *node1_name, char *node2_name, void *head);

bool    plus_op_nodes(char *left,char *right,void *head);

bool    evaluate_node_variables_condition(char *left, char *right, char *op, void *head);

bool    pluse_with_helper(char *main, char *left_key,char *stmt_helper ,void *head);


bool    minus_op_nodes(char *left, char *right, void *head) ;


bool    assignments_varables(char *left_node, char *right_node, char *operator, void *head);


bool    deep_copy_variables(char *dest_node_key, char *source_node_key, void *head);


bool    evaluate_node_condition(char *left, char *right, const char *op, void *head);
#endif