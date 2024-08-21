#include <stdio.h>
#include <stdlib.h>
#include "statement.h"

void        initStack(StatementStack *stack) {
    if (stack == NULL) {
        fprintf(stderr, "Stack pointer is NULL\n");
        exit(EXIT_FAILURE);
    }
    
    stack->capacity = 10;  
    stack->top = -1;       
    stack->data = (Statement **)malloc(stack->capacity * sizeof(Statement *));
    
    if (stack->data == NULL) {
        fprintf(stderr, "Memory allocation failed for stack data\n");
        exit(EXIT_FAILURE);
    }
}


void        pushStatement(StatementStack *stack, Statement *stmt) {
    if (stack->top == stack->capacity - 1) {
        stack->capacity *= 2;
        stack->data = (Statement **)realloc(stack->data, stack->capacity * sizeof(Statement *));
        if (stack->data == NULL) {
            fprintf(stderr, "Memory allocation failed while resizing stack\n");
            exit(EXIT_FAILURE);
        }
    }
    stack->data[++stack->top] = stmt;
}


Statement   *popStatement(StatementStack *stack) {
    if (stack->top == -1) {
        return NULL;
    }
    return stack->data[stack->top--];
}


int         isEmpty(StatementStack *stack) {
    return stack->top == -1;
}


Statement   *createStatement(int type, char **statement) {
    Statement *stmt = (Statement *)malloc(sizeof(Statement));
    if (stmt == NULL) {
        fprintf(stderr, "Memory allocation failed for statement\n");
        return NULL;
    }
    stmt->type = type;
    stmt->statement = (void **)statement;
    return stmt;
}


int         countStatementsInStack(const StatementStack *stack) {
    return stack->top + 1;
}


Statement   **createStatementArray(int count) {
    Statement **array = (Statement **)malloc(count * sizeof(Statement *));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed for statement array\n");
        return NULL;
    }
    return array;
}


int        glimpseTopOfStack(const StatementStack *stack) {
    if (isEmpty((StatementStack *)stack)) {
        printf("Stack is empty\n");
        return -1;
    } else {
        return stack->data[stack->top]->type;
    }
}


void        freeStatement(Statement *stmt) {
    if (stmt) {
        if (stmt->statement) {
            free(stmt->statement);
        }
        free(stmt);
    }
}


void        emptyStatementStack(StatementStack *stack) {

    while (!isEmpty(stack)) {
        Statement *stmt = popStatement(stack);
    }
    stack->data = NULL;
    stack->top = -1;
    stack->capacity = 0;
}


Statement   *copyTopOfStack(const StatementStack *stack) {
    if (isEmpty(stack)) {
        return NULL;
    }

    Statement *topStatement = stack->data[stack->top];
    
    Statement *copy = (Statement *)malloc(sizeof(Statement));
    if (copy == NULL) {
        fprintf(stderr, "Memory allocation failed for statement copy\n");
        return NULL;
    }

    copy->type = topStatement->type;

    int count = 0;
    while (topStatement->statement[count] != NULL) {
        count++;
    }

    copy->statement = (void **)malloc((count + 1) * sizeof(char *));
    if (copy->statement == NULL) {
        fprintf(stderr, "Memory allocation failed for statement array\n");
        free(copy);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        copy->statement[i] = strdup((char *)topStatement->statement[i]);
        if (copy->statement[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for statement element\n");
            for (int j = 0; j < i; j++) {
                free(copy->statement[j]);
            }
            free(copy->statement);
            free(copy);
            return NULL;
        }
    }

    copy->statement[count] = NULL;

    return copy;
}


Statement **on_revers_createQStatementsFromStack(Statement **Q_if_statements, int count) {
    // Assuming `count` is the number of elements in Q_if_statements
    Statement **QQ_stack = createStatementArray(count); // Allocate memory for the new stack

    for (int i = 0; i < count; i++) {
        QQ_stack[i] = Q_if_statements[i];
    }

    return QQ_stack;
}


Statement   **createQStatementsFromStack(StatementStack *stack) {
    int n_statements = countStatementsInStack(stack);
    if (n_statements == 0) {
        fprintf(stderr, "Stack is empty, no statements to create.\n");
        return NULL;
    }

    Statement **Q_statements = createStatementArray(n_statements + 1);

    Q_statements[0] = popStatement(stack);

    for (int j = 1; j <= n_statements; j++) {
        Q_statements[j] = popStatement(stack);
    }

    return Q_statements;
}


int         if_nasted_block(Statement **QQ_statements,void *head){
    int CNT = 0;
    while(QQ_statements[CNT] != NULL){
        CNT++;
    }
    for(int i = 0; i < CNT; i++){
        if(QQ_statements[i]->type == IF_STATEMENT){
            int n_statements = QQ_statements[i]->statement[3];
            return n_statements;
        }
    }
    return 0;
}


int         if_index_inside_for(Statement **QQ_statements){

    int CNT = 0;
    while(QQ_statements[CNT] != NULL){
        CNT++;
    }
    for(int i = 0; i < CNT; i++){
        if(QQ_statements[i]->type == IF_STATEMENT){
            return i;
        }
    }
    return 0;
}


void        print_all_statements(Statement **QQ_statements){
    int CNT = 0;
    while (QQ_statements[CNT] != NULL){
        CNT++;
    }
    for (int i = 0; i < CNT; i++)
    {
        printf("[i] - > [%d]\n", i);
        printf("Statement type: %d\n", QQ_statements[i]->type);
        printf("Statement: [%s]\n", (char *)QQ_statements[i]->statement[0]);
        printf("Statement: [%s]\n", (char *)QQ_statements[i]->statement[1]);
        printf("Statement: [%s]\n", (char *)QQ_statements[i]->statement[2]);
        printf("\n");
    }
}


int         catch_if_statement(Statement **QQ_statements) {
    int if_index = -1;
    int CNT = 0;
    
    while (QQ_statements[CNT] != NULL) {
        CNT++;
    }

    for (int i = 0; i < CNT; i++) {
        if (QQ_statements[i]->type == IF_STATEMENT && if_index == -1) {
            if_index = i;
        }
    }
    
    if (if_index != -1) {
        int end_position = 3;  
        int distance = abs(end_position - if_index);

        for (int i = 0; i < distance; i++) {
            swap_index_order(QQ_statements, if_index + i + 1, if_index + i);
        }
        return distance;
    }
    
    return 0;
}


void        swap_index_order(Statement **Q_statements, int index_to, int index_from) {
    if (index_to < index_from) {
        Statement *temp = Q_statements[index_from];
        for (int i = index_from; i > index_to; i--) {
            Q_statements[i] = Q_statements[i - 1];
        }
        Q_statements[index_to] = temp;
    } else if (index_to > index_from) {
        Statement *temp = Q_statements[index_from];
        for (int i = index_from; i < index_to; i++) {
            Q_statements[i] = Q_statements[i + 1];
        }
        Q_statements[index_to] = temp;
    }
}


Statement*  create_statment_from_block(char **block, int statement_type){

    Statement *stmt = (Statement *)malloc(sizeof(Statement));
    if (stmt == NULL) {
        fprintf(stderr, "Memory allocation failed for statement\n");
        return NULL;
    }
    stmt->type = statement_type;
    stmt->statement = (void **)block;
    return stmt;
}


Statement*  create_output_statement(char **ids){
    Statement *stmt = (Statement *)malloc(sizeof(Statement));
    if (stmt == NULL) {
        fprintf(stderr, "Memory allocation failed for statement\n");
        return NULL;
    }
    stmt->type = OUTPUT_STATEMENT;
    stmt->statement = (void **)ids;
    return stmt;
}


Statement*  create_input_statement(char **ids){

    Statement *stmt = (Statement *)malloc(sizeof(Statement));
    if (stmt == NULL) {
        fprintf(stderr, "Memory allocation failed for statement\n");
        return NULL;
    }
    stmt->type = INPUT_STATEMENT;
    stmt->statement = (void **)ids;
    return stmt;
}


Statement*  create_for_statement(char **ids){
    Statement *stmt = (Statement *)malloc(sizeof(Statement));
    if (stmt == NULL) {
        fprintf(stderr, "Memory allocation failed for statement\n");
        return NULL;
    }
    stmt->type = FOR_STATEMENT;
    stmt->statement = (void **)ids;
    return stmt;
}


Statement*  create_assign_statement(char **ids){
    Statement *stmt = (Statement *)malloc(sizeof(Statement));
    if (stmt == NULL) {
        fprintf(stderr, "Memory allocation failed for statement\n");
        return NULL;
    }
    stmt->type = ASSIGNMENT_STATEMENT;
    stmt->statement = (void **)ids;
    return stmt;
}


int compare_statements(const void *a, const void *b) {
    Statement *stmtA = *(Statement **)a;
    Statement *stmtB = *(Statement **)b;

    // Define the order of types
    if (stmtA->type == 6 && stmtB->type != 6) return -1; 
    if (stmtA->type != 6 && stmtB->type == 6) return 1;

    if (stmtA->type == 0 && stmtB->type != 0) return -1; 
    if (stmtA->type != 0 && stmtB->type == 0) return 1;

    if (stmtA->type == 2 && stmtB->type != 2) return 1; 
    if (stmtA->type != 2 && stmtB->type == 2) return -1;

    return 0; 
}

void sort_statements(Statement **statements, int count) {
    qsort(statements, count, sizeof(Statement *), compare_statements);
}
