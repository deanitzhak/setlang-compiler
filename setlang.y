%{
#include <stdio.h>
#include <stdlib.h>
#include "data_holder.h"
#include "statement.h"
#include "helpers.h"
#include  "errors_hendler.h"
/**/
extern                  int yylex(void);
extern                  int yyparse(void);
extern                  FILE *yyin;  /*FOR READING A FILE*/
extern int              yylineno;

void                    yyerror(const char *s);
extern void             yy_scan_string(const char *str);
extern void             yy_delete_buffer(void *buffer);
void                    yyerror(const char *s);
/**/    
StatementStack          statement_stack ;
Data_node               *head = NULL; 
void                    pop_regulation();
void                    pop_loop_statement();
void                    release_stck();
void                    pop_all_if_stack();  
void                    pop_loop_no_brace_statement();

char*                   my_helper = "alfred";
int                     if_block = 0;
/**/
void                    read_file_and_process(const char *filename) ;
%}
%define                 parse.error verbose
%define                 parse.lac full

%union {
    char    *sval;
    int     ival;
    char    **vars;
    struct u_number_list {
        int *vars;
        int count;
    } u_number_list;

    struct  u_var_list {
        char    **vars;
        int     count;
       }        u_var_list;
}   

%token <ival>           NUMBER
%token <sval>           IDENTIFIER STRING_LITERAL 
%token                  COLLECTION SET INT STR
%token                  LBRACE RBRACE SEMICOLON LBRACKET RBRACKET COMMA
%token <sval>           PLUS MINUS MULTIPLY DIVIDE AND PIPE GREATER_THAN LESS_THAN
%token                  GREATER_EQUAL LESS_EQUAL EQUAL NOT ASSIGN
%token                  FOR  WHILE IF ELSE 
%token                  COLON OUTPUT LPAREN RPAREN OR
%token <sval>           INPUT
%token                  YYEOF

%type                   declarations declaration 
%type <u_var_list>      u_var_list ;
%type <u_var_list>      string_list;
%type <u_number_list>   number_list;
%type                   continue 
%type <u_var_list>      list 
%type <sval>            type operators conditions
%type <sval>            operation program
%type <sval>            input_statement assignment if_statement for_loop output_statement operations loop_no_brace if_else
%start                  program

%%
/* PROGRAM */
program: {}
    | declarations operations  {if(!isEmpty(&statement_stack)){release_stck();}}
    | operations   
    ;
/* DECLARATION */
declarations:
    | declarations declaration
    | declaration
    ;
declaration :
                    IDENTIFIER ASSIGN  NUMBER SEMICOLON {
                        Data_node *current_node = search_node(head, $1);
                        current_node->var_count = 0;
                        current_node->variables = $3;
                        free($1);
                    }
                    | IDENTIFIER ASSIGN STRING_LITERAL SEMICOLON {
                        Data_node *current_node = search_node(head, $1);
                        current_node->var_count = 1;
                        current_node->variables = $3;
                    }
                    | IDENTIFIER ASSIGN LBRACE string_list RBRACE SEMICOLON    {
                        Data_node *current_node = search_node(head, $1);
                        update_variables(current_node, $4.vars, $4.count);
                        //free($4.vars);
                    }
                    | IDENTIFIER ASSIGN LBRACKET number_list RBRACKET SEMICOLON { 
                        if($4.vars[0] != 0){
                            Data_node *current_node = search_node(head, $1);
                            current_node->var_count = $4.count;
                            free(current_node->variables);
                            current_node->variables = (int *)malloc(sizeof(int) * ($4.count+1));
                            for (int i = 0; i < $4.count; i++) {
                            current_node->variables[i] = $4.vars[i];
                        }
                        }else{/* EMPTY INIT */
                            Data_node *current_node = search_node(head, $1);
                            current_node->var_count = $4.count;
                            current_node->variables = (int *)malloc(sizeof(int) * ($4.count+1));
                            current_node->variables[0] = NULL;
                        }
                        free($4.vars);
                    }
                    | COLLECTION    u_var_list SEMICOLON  {
                        for (int i = 0; i < $2.count; i++) {
                            Data_node *new_node = initialize_node(COLLECTION_VAR, $2.vars[i]);
                            add_node(&head, new_node); 
                        }
                        free($2.vars);
                        }
                    | SET           u_var_list SEMICOLON {
                        for (int i = 0; i < $2.count; i++) {
                            Data_node *new_node = initialize_node(SET_VAR, $2.vars[i]);
                            add_node(&head, new_node); 
                        }
                        }
                    | INT           u_var_list SEMICOLON {
                        for (int i = 0; i < $2.count; i++) {
                            Data_node *new_node = initialize_node(INT_VAR, $2.vars[i]);
                            add_node(&head, new_node); 
                        }
                        }
                    | STR           u_var_list SEMICOLON {
                        for (int i = 0; i < $2.count; i++) {
                            Data_node *new_node = initialize_node(STR_VAR, $2.vars[i]);
                            add_node(&head, new_node); 
                        }

                        }
                    ;
/* ASSIGNMENT */
/* OPERATION */
operations:{} 
    | operation 
    | operations operation {if(!isEmpty(&statement_stack)){release_stck();}} 
    ;

operation  : 
    |   loop_no_brace
    |   for_loop {release_stck();}
    |   input_statement
    |   assignment 
    |   if_statement 
    |   output_statement 
    |   if_else 
    |   declaration

    ;

loop_no_brace:
        FOR LPAREN IDENTIFIER COLON IDENTIFIER RPAREN operation  {
            char **ids =  malloc(3 * sizeof(char *));
            ids[0] = $3;
            ids[1] = $5;
            ids[2] = NULL;
            Statement *new_statement = create_for_statement(ids);
            new_statement->type = FOR_STATEMENT_NO_BRACE;
            pushStatement(&statement_stack, new_statement);    
        } 

for_loop:
        | FOR LPAREN IDENTIFIER COLON IDENTIFIER RPAREN LBRACE operation RBRACE  {
            char **ids =  malloc(3 * sizeof(char *));
            ids[0] = $3;
            ids[1] = $5;
            ids[2] = NULL;
            Statement *new_statement = create_for_statement(ids);
            pushStatement(&statement_stack, new_statement);
        } 
        | WHILE LPAREN IDENTIFIER GREATER_THAN NUMBER RPAREN LBRACE operation RBRACE continue {printf("While loop\n");}
        

        ;

if_statement:
        | IF LPAREN IDENTIFIER conditions type RPAREN LBRACE operation RBRACE  {
                char **ids = (char **)malloc(4 * sizeof(char *));
                if($5 < 100 && $5 > 0){
                    ids[2] = int_to_string($5); //int
                }else{
                    ids[2] = $5; //string
                }
                ids[0] = $3; //identifier
                ids[1] = $4; //condition
                ids[3] = NULL; //padding
                Statement *stmt = createStatement(IF_STATEMENT,ids);
                pushStatement(&statement_stack,stmt);
        }
        | IF LPAREN operators conditions list  RPAREN continue {
            char **ids = (char **)malloc(4 * sizeof(char *));
                if($5.vars < 100 && $5.vars > 0){
                    ids[2] = int_to_string($5.vars); //int
                }else{
                    ids[2] = $5.vars; //string
                }
                ids[0] = $3; //identifier
                ids[1] = $4; //condition
                ids[3] = NULL; //padding
                Statement *stmt = createStatement(IF_STATEMENT,ids);
                pushStatement(&statement_stack,stmt);
        }
        ;

if_else :
        | IF LPAREN IDENTIFIER conditions type RPAREN operation ELSE operation {
            char **ids = (char **)malloc(4 * sizeof(char *));
            if($5 < 100 && $5 > 0){
                ids[2] = int_to_string($5); //int
            }else{
                ids[2] = $5; //string
            }
            ids[0] = $3; //identifier
            ids[1] = $4; //condition
            ids[3] = NULL; //padding
            Statement *stmt = createStatement(IF_ELSE_STATEMENT,ids);
            pushStatement(&statement_stack,stmt);
        } 
output_statement:
        | OUTPUT STRING_LITERAL IDENTIFIER operators type operators LPAREN operators RPAREN SEMICOLON  operation{
            char **ids =  malloc(4 * sizeof(char *));
            if($3 < 100 && $3 > 0){
                ids[1] = int_to_string($3); //int
            }else{
                ids[1] = $3; //string
            }
            ids[0] = $2;
            ids[2] = $8;
            ids[3] = NULL;
            Statement *new_statement = create_output_statement(ids);
            pushStatement(&statement_stack, new_statement);
            if_block++;
        }
        | OUTPUT STRING_LITERAL operators type operators SEMICOLON operation {
            char **ids =  malloc(4 * sizeof(char *));
            ids[0] = $2;
            ids[1] = $3;
            ids[2] = NULL;
            Statement *new_statement = create_output_statement(ids);
        }
        | OUTPUT STRING_LITERAL operators  SEMICOLON operation {         
            char **ids =  malloc(4 * sizeof(char *));
            if($3 < 100 && $3 > 0){
                ids[1] = int_to_string($3); //int
            }else{
                ids[1] = $3; //string
            }
            ids[0] = $2;
            ids[2] = NULL;
            Statement *new_statement = create_output_statement(ids);
            pushStatement(&statement_stack, new_statement);
            if_block++;
        }
        ;


input_statement:
        | INPUT STRING_LITERAL IDENTIFIER SEMICOLON operation {
            char **ids =  malloc(4 * sizeof(char *));
            ids[0] = $1;
            ids[1] = $2;
            ids[2] = $3;
            ids[4] = NULL;
            Statement *new_statement = create_input_statement(ids);
            pushStatement(&statement_stack, new_statement);
        }
        ;

assignment:
        | IDENTIFIER ASSIGN IDENTIFIER operators IDENTIFIER SEMICOLON continue {
            char **ids =  malloc(5 * sizeof(char *));
            ids[0] = $1; //MAIN
            ids[1] = $3; //LEFT
            ids[2] = $4; //OPERATOR
            ids[3] = $5; //RIGHT
            ids[4] = NULL;
            Statement *new_statement = create_assign_statement(ids);
            pushStatement(&statement_stack, new_statement);
            if_block++;
        }
        | IDENTIFIER ASSIGN IDENTIFIER operators IDENTIFIER operators IDENTIFIER SEMICOLON operation {
                /*MAIN = LEFT OP RIGHT OP RIGHTER*/
                /*HENDELING LEF AND RIGHT*/
                char **ids =  malloc(5 * sizeof(char *));
                ids[0] = $1;
                ids[1] = $3;
                ids[2] = $4;
                ids[3] = $5;
                ids[4] = NULL;
                Statement *new_statement = create_assign_statement(ids);
                execute_statement(new_statement, head);
                Data_node *current_node = search_node(head, $1);
                /*HENDELING MAIN OP RIGHTER*/
                char **mian_ids =  malloc(5 * sizeof(char *));
                mian_ids[0] = $1;
                mian_ids[1] = $1;
                mian_ids[2] = $6;
                mian_ids[3] = $7;
                mian_ids[4] = NULL;
                Statement *main_new_statement = create_assign_statement(mian_ids);
                pushStatement(&statement_stack, main_new_statement);
                if_block++;
        }
        | IDENTIFIER ASSIGN IDENTIFIER operators list SEMICOLON continue {
            update_variables_by_key(my_helper, $5.vars,$5.count,head);
            char **ids =  malloc(5 * sizeof(char *));
            ids[0] = $1;
            ids[1] = $3;
            ids[2] = $4;
            ids[3] = my_helper;
            ids[4] = NULL;
            Statement *new_statement = create_assign_statement(ids);
            pushStatement(&statement_stack, new_statement);
            if_block++;
        }
        | IDENTIFIER ASSIGN IDENTIFIER operators type SEMICOLON continue {
            char **ids =  malloc(5 * sizeof(char *));
            ids[0] = $1; //MAIN
            ids[1] = $3; //LEFT
            ids[2] = $4; //OPERATOR
            ids[3] = $5; //RIGHT
            ids[4] = NULL;
            Statement *new_statement = create_assign_statement(ids);
            pushStatement(&statement_stack, new_statement);
            if_block++;
        }
        ;

/* LIST OF VARS */
u_var_list:
        IDENTIFIER {
            $$ = (struct u_var_list){ malloc(sizeof(char *)), 1 };
            $$ .vars[0] = $1;  // Set $$ to the value of IDENTIFIER
    
        }
        | u_var_list COMMA IDENTIFIER {                                   //padding for memory allocate
            $$ = (struct u_var_list){ realloc($1.vars, sizeof(char *) * ($1.count + 2)), $1.count + 1 };
            if ($$ .vars == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(EXIT_FAILURE);
            }
            $$ .vars[$$ .count - 1] = $3;
        }
        ;



string_list:    {} 
    | STRING_LITERAL { 
        $$ = (struct u_var_list) { malloc(sizeof(char *)), 1 };
        $$ .vars[0] = strdup($1);
     }
    | string_list COMMA STRING_LITERAL { 
        $$ = (struct u_var_list) { realloc($1.vars, sizeof(char *) * ($1.count + 1)), $1.count + 1 };
        if ($$ .vars == NULL) {
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
        }
        $$ .vars[$$ .count - 1] = strdup($3);
     }
    ;


number_list     : {$$ = (struct u_number_list) { malloc(sizeof(int)), 1 };$$ .vars[0] = 0;$$ .count = 0;} /*/* EMPTY INIT */
                | NUMBER{
                    $$ = (struct u_number_list) { malloc(sizeof(int)), 1 };
                    $$ .vars[0] = $1;
                 }
                | number_list COMMA NUMBER{
                    $$ = (struct u_number_list) { realloc($1.vars, sizeof(int) * ($1.count + 1)), $1.count + 1 };
                    if ($$ .vars == NULL) {
                        fprintf(stderr, "Out of memory\n");
                        exit(EXIT_FAILURE);
                    }
                    $$ .vars[$$ .count - 1] = $3;
                  }
                ;


continue    :
            | /* Nothing */
            | declaration
            | operation
            | operators
            | type 
            | SEMICOLON 
            ;

conditions  :   {$$ = "TRUE";}
            |   GREATER_THAN { $$ = ">"; }
            |   LESS_THAN { $$ = "<"; } 
            |   GREATER_EQUAL { $$ = ">="; }
            |   LESS_EQUAL { $$ = "<="; }
            |   EQUAL { $$ = "=="; }
            |   NOT { $$ = "!="; }
            |   AND{ $$ = "&&"; }

            ;

operators   :
            | PLUS { $$ = "+";}
            | MINUS { $$ = "-";}
            | MULTIPLY { $$ = "*";}
            | DIVIDE { $$ = "/";}
            | AND { $$ = "&";}
            | PIPE { $$ = "|";}
            | GREATER_THAN { $$ = ">"; }
            | LESS_THAN  { $$ = "<"; }
            | GREATER_EQUAL { $$ = ">="; }
            | LESS_EQUAL { $$ = "<="; }
            | EQUAL { $$ = "==";}
            | NOT { $$ = "!=";}
            | OR { $$ = "||";}
            | IDENTIFIER { $$ = $1;}
            | IDENTIFIER AND IDENTIFIER {
                char **ids =  malloc(5 * sizeof(char *));
                ids[0] = my_helper;
                ids[1] = $1;
                ids[2] ="&";
                ids[3] = $3;
                ids[4] = NULL;
                Statement *new_statement = create_assign_statement(ids);
                execute_statement(new_statement, head);
                $$ = my_helper;
                }
            | IDENTIFIER PLUS IDENTIFIER {
                char **ids =  malloc(5 * sizeof(char *));
                ids[0] = my_helper;
                ids[1] = $1;
                ids[2] ="+";
                ids[3] = $3;
                ids[4] = NULL;                
                Statement *new_statement = create_assign_statement(ids);
                execute_statement(new_statement, head);
                $$ = my_helper;
            } 
            | PIPE IDENTIFIER PIPE{
                Data_node *current_node = search_node(head, $2);
                $$ = current_node->var_count;
            }

            ;


type: {$$ = "TRUE";}
    | NUMBER { $$ = $1; }
    | STRING_LITERAL { $$ = $1; }
    | IDENTIFIER { $$ = $1; }
    | list
    | operators
    ;
list:
    | LBRACE RBRACE { $$.vars = "FALSE"; } /* EMPTY INIT */
    | LBRACE string_list RBRACE {
        int count = $2.count;
        $$.count = count;
        $$ = (struct u_var_list) { malloc(sizeof(char *)), 1 };
        $$.vars = (char **)malloc(sizeof(char *) * (count + 1));
        for (int i = 0; i < count; i++) {
            $$.vars[i] = strdup($2.vars[i]);  // Use strdup to duplicate the string
        }
        $$.count = count;
        $$.vars[count] = NULL;
    }
    | LBRACKET number_list RBRACKET
    ;



%%

int main(int argc, char **argv) {     
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    initStack(&statement_stack);
    Data_node *helper = initialize_node(HELPER,my_helper);
    add_node(&head, helper);
    read_file_and_process(argv[1]);    
    return EXIT_SUCCESS;
}    


void yyerror(const char *s) {
    fprintf(stderr, "Error at line %d: %s\n", yylineno, s);
}


void read_file_and_process(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    yyin = file;  
    printf("***************** START PARSING ******************\n");

    int parse_result = yyparse();

    fclose(file);

    if (parse_result != 0) {
        fprintf(stderr, "Parsing failed.\n");
        exit(EXIT_FAILURE);
    }
}


void pop_regulation(){
    Statement *current_statement = popStatement(&statement_stack);
    execute_statement(current_statement, head);
}



void pop_loop_statement(){
    Statement **QQ_stack = createQStatementsFromStack(&statement_stack);
    Data_node *left_node = search_node(head, QQ_stack[0]->statement[0]);
    Data_node *right_node = search_node(head, QQ_stack[0]->statement[1]);
    if(left_node == NULL || right_node == NULL){
        handleError(NOT_SUPPORTED_ERROR);
    }
    int statments_count = count_lsit(QQ_stack);
    for(int i = 0; i < right_node->var_count; i++){
        left_node->variables = right_node->variables[i];
        for(int j = 0 ; j < statments_count ; j++){
            if(QQ_stack[j]->type == IF_STATEMENT){
                bool condition = execute_statement(QQ_stack[j], head);
                if(condition == 0){
                    j += if_block;
                }
            }else{
                execute_statement(QQ_stack[j], head);
            }
        } 
    }
}


void pop_all_if_stack() {
    int total_elements = if_block + 1;
    Statement **Q_if_statements = createStatementArray(total_elements);
    Statement **temp_stack = createStatementArray(total_elements); 
    
    int temp_count = 0;
    while (!isEmpty(&statement_stack)) { 
        temp_stack[temp_count] = popStatement(&statement_stack);
        temp_count++;
    }

    int processed_count = 0;
    for (int i = temp_count - 1; i >= 0; i--) {
        if (processed_count == 0) {
            Q_if_statements[processed_count++] = temp_stack[i];
            continue;
        }
        if (processed_count <= if_block) {
            if (temp_stack[i]->type != 1) {
                Q_if_statements[processed_count++] = temp_stack[i];
            } else {
                pushStatement(&statement_stack, temp_stack[i]);
            }
        } else {
            pushStatement(&statement_stack, temp_stack[i]);
        }
    }
    bool suc = execute_statement(Q_if_statements[if_block], head);
    if(suc == true){
        for(int i = if_block-1; i >= 0; i--){
            execute_statement(Q_if_statements[i], head);
        }
    }
    free(temp_stack);
}


void pop_loop_no_brace_statement() {
    Statement **Q_if_statements = createStatementArray(2);
    Q_if_statements[0] = popStatement(&statement_stack);
    Data_node *left_node = search_node(head, Q_if_statements[0]->statement[0]);
    Data_node *right_node = search_node(head, Q_if_statements[0]->statement[1]);
    
    if (left_node == NULL || right_node == NULL) {
        handleError(NOT_SUPPORTED_ERROR);
        return;
    }else{
        Q_if_statements[1] = popStatement(&statement_stack);
    }
    for (int i = 0; i < right_node->var_count; i++) {
        left_node->variables = right_node->variables[i];
        execute_statement(Q_if_statements[1], head);
    }
}


void pop_if_else_stack(){
    int stack_count = countStatementsInStack(&statement_stack);
    Statement **Q_if_statements = createStatementArray(stack_count);
    Statement *temp_stack = statement_stack.data[2];
    if(Q_if_statements == NULL || temp_stack == NULL || stack_count == 0){
        handleError(MEMORY_ALLOCATION_ERROR);
    }
    for(int i = 0; i < stack_count; i++){
        Q_if_statements[i] = popStatement(&statement_stack);
    }
    sort_statements(Q_if_statements, stack_count);
    bool condition = execute_statement(Q_if_statements[0], head);
    if(condition == true){
        execute_statement(Q_if_statements[8], head);
    }else{
        execute_statement(Q_if_statements[2], head);
    }
    execute_statement(Q_if_statements[10], head);

    condition = execute_statement(Q_if_statements[1], head);
    if(condition == true){
        execute_statement(Q_if_statements[7], head);
    }else{
        execute_statement(Q_if_statements[3], head);
    }

    execute_statement(Q_if_statements[4], head);

    condition = execute_statement(Q_if_statements[9], head);
    if(condition == false){
        execute_statement(Q_if_statements[5], head);
    }
    execute_statement(temp_stack, head);
    if_block = 1;
}


void release_stck() {
    while (!isEmpty(&statement_stack)) { 
        int type = glimpseTopOfStack(&statement_stack);
        if (type == IF_STATEMENT) {
            pop_all_if_stack();  
            if_block = 0;
        } else if (type == FOR_STATEMENT) {
            pop_loop_statement();
        }else if(type == FOR_STATEMENT_NO_BRACE){
            pop_loop_no_brace_statement();
        } else if(type == IF_ELSE_STATEMENT) {
            pop_if_else_stack();
        } 
        else {
            pop_regulation();
        }
    }
    if_block = 0;
}


