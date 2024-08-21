#ifndef STATEMENT_H
#define STATEMENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define OUTPUT_STATEMENT 0
#define INPUT_STATEMENT 1
#define ASSIGNMENT_STATEMENT  2
#define IF_STATEMENT 3
#define FOR_STATEMENT 4
#define FOR_STATEMENT_NO_BRACE 5
#define IF_ELSE_STATEMENT 6



typedef struct Statement {
    int type;
    void **statement;
} Statement;

typedef struct {
    Statement **data;
    int top;
    int capacity;
} StatementStack;


/* STATEMENTS MANAGEMENT */
/* statement.c */

void        initStack(StatementStack *stack);


void        pushStatement(StatementStack *stack, Statement *stmt);


Statement   *popStatement(StatementStack *stack) ;


int         isEmpty(StatementStack *stack) ;


Statement   *createStatement(int type, char **statement);


int         countStatementsInStack(const StatementStack *stack);  


Statement   **createStatementArray(int count);


int        glimpseTopOfStack(const StatementStack *stack);


void        emptyStatementStack(StatementStack *stack);


void        freeStatement(Statement *stmt);


Statement   *copyTopOfStack(const StatementStack *stack);


Statement   **createQStatementsFromStack(StatementStack *stack);


int         if_nasted_block(Statement **QQ_statements,void *head);


int         if_index_inside_for(Statement **QQ_statements);


void        print_all_statements(Statement **QQ_statements);


void        swap_index_order(Statement **Q_statements, int index_to, int idex_from);


int         catch_if_statement(Statement **QQ_statements);

Statement*  create_statment_from_block(char **block, int statement_type);

Statement*  create_output_statement(char **ids);

Statement*  create_input_statement(char **ids);

Statement*  create_for_statement(char **ids);

Statement*  create_assign_statement(char **ids);

Statement **on_revers_createQStatementsFromStack(Statement **Q_if_statements, int count);

int compare_statements(const void *a, const void *b) ;


void sort_statements(Statement **statements, int count) ;
/* STATEMENTS EXECUTE */

/* statement_executor.c */
bool execute_statement(Statement *statement,void *head);


void execute_output_statement(char **ids, void *head);


void input_statement_execute(char **ids, void *head);


void execute_assignment_statement(Statement *st, void *head );


bool execute_if_statement(char **ids, void *head);
#endif 