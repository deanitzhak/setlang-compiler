#include "data_holder.h"
#include "errors_hendler.h"
char*               op_result = "alfred";
int count = 0;

Data_node*  initialize_node(int type, const char *key) {
    Data_node *new_node = (Data_node *)malloc(sizeof(Data_node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    new_node->type = type;
    new_node->key = strdup(key);
    new_node->variables = NULL;
    new_node->var_count = 0;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}


void        add_node(Data_node **head, Data_node *new_node) {
    if (*head == NULL) {
        *head = new_node;
    } else {
        Data_node *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
        new_node->prev = current;
    }
}


void        add_variable(Data_node *node, void *value) {
    node->variables = realloc(node->variables, sizeof(void*) * (node->var_count + 1));
    if (!node->variables) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    node->variables[node->var_count] = value;
    node->var_count++;
}


Data_node*  search_node(Data_node *head, const char *key) {
    Data_node *current = head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


void        print_node(Data_node *node) {
    if (node == NULL) return;
    if(node->type == COLLECTION_VAR) {
        printf("{");
        for (int i = 0; i < node->var_count; i++) {
            printf(" %s,", node->variables[i]);
        }
        printf("}\n");
    } else if(node->type == SET_VAR) {
        printf("[");
        for (int i = 0; i < node->var_count; i++) {
            printf("%d", node->variables[i]);
            if (i < node->var_count - 1) {
                printf(",");
            }
        }
        printf("]\n");
    } else if(node->type == STR_VAR) {
        printf("%s\n", node->variables);
    } else if(node->type == INT_VAR) {
        printf("%d\n", node->variables);
    }else if(node->type == HELPER) {
        printf("{");
        for (int i = 0; i < node->var_count; i++) {
            printf(" %s,", node->variables[i]);
        }
        printf("}\n");

    }
}


void        print_all_nodes(Data_node *head) {
    Data_node *current = head;
    while (current != NULL) {
        print_node(current);
        current = current->next;
    }
}


void        delete_node(Data_node **head, const char *key) {
    Data_node *node_to_delete = search_node(*head, key);
    if (!node_to_delete) return;

    if (node_to_delete->prev) {
        node_to_delete->prev->next = node_to_delete->next;
    } else {
        *head = node_to_delete->next;
    }
    if (node_to_delete->next) {
        node_to_delete->next->prev = node_to_delete->prev;
    }

    free_node(node_to_delete);
}


void        free_node(Data_node *node) {
    if (node == NULL) return;
    free(node->key);
    for (int i = 0; i < node->var_count; i++) {
        free(node->variables[i]);
    }
    free(node->variables);
    free(node);
}


void        free_all_nodes(Data_node *head) {
    Data_node *current = head;
    Data_node *next;
    while (current != NULL) {
        next = current->next;
        free_node(current);
        current = next;
    }
}


void**      get_variables(Data_node *node) {
    return node->variables;
}


char*       get_key(Data_node *node) {
    return node->key;
}


int         get_type(Data_node *node) {
    return node->type;
}


Data_node*  get_next(Data_node *node) {
    return node->next;
}


Data_node*  get_prev(Data_node *node) {
    return node->prev;
}


void        update_variables_by_key(char *key, void **new_variables, int new_var_count,Data_node *head) {
    Data_node *node = search_node(head, key);
    if (node == NULL) {
        fprintf(stderr, "Node with key %s not found\n", key);
        return;
    }
    for (int i = 0; i < node->var_count; i++) {
        free(new_variables[i]);
    }
    node->variables = (void **)malloc(sizeof(void *) * new_var_count);
    node->var_count = new_var_count;
    for (int i = 0; i < new_var_count; i++) {
        node->variables[i] = new_variables[i];
    }
    
}


void        update_variables(Data_node *node, void **new_variables, int new_var_count) {
    for (int i = 0; i < node->var_count; i++) {
        free(node->variables[i]);
    }
    free(node->variables);
    node->variables = (void **)malloc(sizeof(void *) * new_var_count);
    if (!node->variables) {
        fprintf(stderr, "Memory allocation failed\n");
        node->var_count = 0;
        return;
    }
    for (int i = 0; i < new_var_count; i++) {
        if(node->type == SET_VAR || node->type == COLLECTION_VAR){
           if (node->type == COLLECTION_VAR)
           {
                node->variables[i] = new_variables[i];
           }else{
                printf("new_variables[%d] = %d\n", i, new_variables[i]);
                node->variables[i] = atoi(new_variables[i]);
           }
        }
        
    }
    node->var_count = new_var_count;
}


Data_node*  copy_node(Data_node *node){
    Data_node *new_node = initialize_node(node->type, node->key);
    new_node->variables = malloc(node->var_count * sizeof(void*));
    new_node->var_count = node->var_count;
    for (int i = 0; i < node->var_count; i++) {
        new_node->variables[i] = node->variables[i];
    }
    return new_node;
}

/* OPERAND ON NODE */

bool plus_op_nodes(char *left,char *right,void *head){
    Data_node *left_node = search_node(head, left);
    Data_node *right_node = search_node(head, right);
    Data_node *result_node = search_node(head, op_result);

    if (left_node == NULL || right_node == NULL || result_node == NULL || (left_node->type+right_node->type)%2 != 0) {
        if(right_node == NULL){
            if (left_node->type == COLLECTION_VAR) /* add */
            {

                int new_size = left_node->var_count + 1;
                void **new_variables = (void **)malloc(sizeof(void *) * new_size);
            
                for (int i = 0; i < left_node->var_count; i++)
                {
                    new_variables[i] = left_node->variables[i];
                }
            
                new_variables[left_node->var_count] = right;
                result_node->variables = new_variables;
                result_node->var_count = new_size;
                return true;
            }
            
        }
        return false;
    }else{
       if (left_node->type != right_node->type)
       {
            if (left_node->type == SET_VAR || right_node->type == SET_VAR)
            {
                int new_size = left_node->var_count + 1;
                void **new_variables = (void **)malloc(sizeof(void *) * new_size);
            
                for (int i = 0; i < left_node->var_count; i++)
                {
                    new_variables[i] = left_node->variables[i];
                }
            
                new_variables[left_node->var_count] = right_node->variables;
                result_node->variables = new_variables;
                result_node->var_count = new_size;
                return true;
            }else if(left_node->type == COLLECTION_VAR || right_node->type == COLLECTION_VAR){
                int new_size = left_node->var_count + 1;
                void **new_variables = (void **)malloc(sizeof(void *) * new_size);
            
                for (int i = 0; i < left_node->var_count; i++)
                {
                    new_variables[i] = left_node->variables[i];
                }
            
                new_variables[left_node->var_count] = right_node->variables;
                result_node->variables = new_variables;
                result_node->var_count = new_size;
                return true;

            }
            
       }
       

    }    
    
}


bool pluse_with_helper(char *main_key, char *left_key, char *stmt_helper, void *head) {
    Data_node *main_node = search_node(head, main_key);
    Data_node *left_node = search_node(head, left_key);
    Data_node *helper_node = search_node(head, stmt_helper);
    
    if (main_node == NULL || left_node == NULL || helper_node == NULL || (left_node->type + helper_node->type) % 2 != 0) {
        return false;
    }

    if (left_node->type == SET_VAR || left_node->type == COLLECTION_VAR) {
        int new_size = left_node->var_count + (helper_node->var_count-1);
        void **new_variables = (void **)malloc(sizeof(void *) * new_size);
        for (int i = 0; i < left_node->var_count; i++) {
            new_variables[i] = left_node->variables[i];
        }

        for (int i = 0; i < helper_node->var_count-1; i++) {
            new_variables[left_node->var_count + i] = helper_node->variables[i];
        }
        free
        (main_node->variables);  
        main_node->variables = new_variables;
        main_node->var_count = new_size;
        return true;
    }

    return false;
}


bool minus_op_nodes(char *left, char *right, void *head) {
    Data_node *left_node = search_node(head, left);
    Data_node *right_node = search_node(head, right);
    Data_node *result_node = search_node(head, op_result); // Assuming op_result is the name of the result node
    if (left_node == NULL || right_node == NULL || result_node == NULL) {
        return false;
    }

    if (left_node->type == SET_VAR || left_node->type == COLLECTION_VAR) {
        int new_size = 0;
        void **new_variables = (void **)malloc(sizeof(void *) * left_node->var_count);

        for (int i = 0; i < left_node->var_count; i++) {
            bool found = false;
            for (int j = 0; j < right_node->var_count; j++) {
                if (strcmp((char *)left_node->variables[i], (char *)right_node->variables[j]) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                new_variables[new_size++] = left_node->variables[i];
            }
        }
        result_node->variables = new_variables;
        result_node->var_count = new_size;
        left_node->variables = realloc(new_variables, sizeof(void *) * new_size);
        left_node->var_count = new_size;        
        return true;
    } else {
        return false;
    }
}


bool and_op_nodes(char *node1_name, char *node2_name, void *head) {
    Data_node *node1 = search_node(head, node1_name);
    Data_node *node2 = search_node(head, node2_name);
    Data_node *result_node = search_node(head, op_result);
    if (node1 == NULL || node2 == NULL || result_node == NULL) {
        return false;
    }

    if ((node1->type == SET_VAR || node1->type == COLLECTION_VAR) && 
        (node2->type == SET_VAR || node2->type == COLLECTION_VAR)) {

        int new_size = 0;
        void **new_variables = (void **)malloc(sizeof(void *) * node1->var_count);
        for (int i = 0; i < node1->var_count; i++) {
            bool found = false;
            for (int j = 0; j < node2->var_count; j++) {
                if (strcmp((char *)node1->variables[i], (char *)node2->variables[j]) == 0) {
                    found = true;
                    break;
                }
            }
            if (found) {
                new_variables[new_size++] = node1->variables[i];
            }
        }

        result_node->variables = new_variables;
        result_node->var_count = new_size;
        result_node->variables = realloc(new_variables, sizeof(void *) * new_size);
        return true;
    } else {
        return false;
    }
}



bool  assignments_varables(char *left_node, char *right_node, char *operator, void *head){
   printf("left_node = %s\n", left_node);
    if(strcmp(operator, "+") == 0){        
        return plus_op_nodes(left_node,right_node,head);
    }
}



bool deep_copy_variables(char *dest_node_key, char *source_node_key, void *head) {
    Data_node *dest_node = search_node(head, dest_node_key);
    Data_node *source_node = search_node(head, source_node_key);

    if (!source_node || !source_node->variables) {
        dest_node->variables = NULL;
        return false;        
    }

    dest_node->variables = (void **)malloc(sizeof(void *) * source_node->var_count);

    if (!dest_node->variables) {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }

    for (int i = 0; i < source_node->var_count; i++) {
        dest_node->variables[i] = source_node->variables[i];  // Deep copy if necessary
    }

    dest_node->var_count = source_node->var_count;
    return true;
}


bool are_nodes_equal(char *node1_name, char *node2_name, void *head)  {
    Data_node *node1 = search_node(head, node1_name);
    Data_node *node2 = search_node(head, node2_name);
    if (node1 == NULL || node2 == NULL) {
        return false;
    }

    if (node1->type != node2->type || node1->var_count != node2->var_count) {
        return false;
    }

    for (int i = 0; i < node1->var_count; i++) {
        bool found = false;
        for (int j = 0; j < node2->var_count; j++) {
            if (strcmp((char *)node1->variables[i], (char *)node2->variables[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false; 
        }
    }

    return true;
}


bool evaluate_node_variables_condition(char *left, char *right, char *op, void *head) {
    if (strcmp(op, "==") == 0) {
        return are_nodes_equal(left, right, head);
    } else if (strcmp(op, "!=") == 0) {
        return !are_nodes_equal(left, right, head);
    } else {
        handleError(INVALID_OPERATOR_ERROR);
    }
}


bool evaluate_node_condition(char *left, char *right, const char *op, void *head) {
    Data_node *left_node = search_node(head, left);
    int int_right_value;
    char *str_right_value;
    Data_node *right_node;
    if (is_number(right)) {
        int right_value = atoi(right);
        if (left_node->type == SET_VAR || left_node->type == COLLECTION_VAR)
        {
                return evaluate_condition(left_node->var_count, right_value, op);
        }else{
            if (left_node->type == INT_VAR)
            {
                return evaluate_condition(left_node->variables, right_value, op);
            } 
        }           
    } else {
            if (left_node->type == COLLECTION_VAR){
                return evaluate_node_variables_condition(left, right, op, head);
            }
            return evaluate_condition(left_node->variables, right, op);
    }
    
    return false;

} 