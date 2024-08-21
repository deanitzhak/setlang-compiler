#include <stdio.h>
#include <stdlib.h>
#include "errors_hendler.h"

void handleError(int errorCode) {
    switch (errorCode) {
        case NOT_SUPPORTED_ERROR:
            fprintf(stderr, "Error: Operation not supported.\n");
            break;
        case SYNTAX_ERROR:
            fprintf(stderr, "Error: Syntax error.\n");
            break;
        case MEMORY_ALLOCATION_ERROR:
            fprintf(stderr, "Error: Memory allocation failed.\n");
            break;
        case INVALID_INPUT_ERROR:
            fprintf(stderr, "Error: Invalid input.\n");
            break;
        case INVALID_OUTPUT_ERROR:
            fprintf(stderr, "Error: Invalid output.\n");
            break;
        case INVALID_ASSIGNMENT_ERROR:
            fprintf(stderr, "Error: Invalid assignment.\n");
            break;
        case INVALID_IF_ERROR:
            fprintf(stderr, "Error: Invalid if statement.\n");
            break;
        case INVALID_FOR_ERROR:
            fprintf(stderr, "Error: Invalid for statement.\n");
            break;
        case INVALID_STATEMENT_ERROR:
            fprintf(stderr, "Error: Invalid statement.\n");
            break;
        case INVALID_VARIABLE_ERROR:
            fprintf(stderr, "Error: Invalid variable.\n");
            break;
        case INVALID_TYPE_ERROR:
            fprintf(stderr, "Error: Invalid type.\n");
            break;
        case INVALID_EXPRESSION_ERROR:
            fprintf(stderr, "Error: Invalid expression.\n");
            break;
        case INVALID_CONDITION_ERROR:
            fprintf(stderr, "Error: Invalid condition.\n");
            break;
        case INVALID_OPERATION_ERROR:
            fprintf(stderr, "Error: Invalid operation.\n");
            break;
        case INVALID_OPERATOR_ERROR:
            fprintf(stderr, "Error: Invalid operator.\n");
            break;
        case INVALID_VARIABLE_NAME_ERROR:
            fprintf(stderr, "Error: Invalid variable name.\n");
            break;
        case INVALID_VARIABLE_TYPE_ERROR:
            fprintf(stderr, "Error: Invalid variable type.\n");
            break;
        case INVALID_VARIABLE_VALUE_ERROR:
            fprintf(stderr, "Error: Invalid variable value.\n");
            break;
        case INVALID_VARIABLE_COUNT_ERROR:
            fprintf(stderr, "Error: Invalid variable count.\n");
            break;
        case INVALID_VARIABLES_ERROR:
            fprintf(stderr, "Error: Invalid variables.\n");
            break;
        case INVALID_VARIABLES_COUNT_ERROR:
            fprintf(stderr, "Error: Invalid variables count.\n");
            break;
        case INVALID_VARIABLES_TYPE_ERROR:
            fprintf(stderr, "Error: Invalid variables type.\n");
            break;
        case DUPILCATE_VARIABLE_ERROR:
            fprintf(stderr, "Error: Duplicate variable.\n");
            break;
        case INVALID_VARIABLES_VALUE_ERROR:
            fprintf(stderr, "Error: Invalid variables value.\n");
            break;
        case INVALID_VARIABLES_NAME_ERROR:
            fprintf(stderr, "Error: Invalid variables name.\n");
            break;
        case TYPE_MISMATCH_ERROR:
            fprintf(stderr, "Error: Type mismatch.\n");
            break;
        case DIVIDE_BY_ZERO_ERROR:
            fprintf(stderr, "Error: Divide by zero.\n");
            break;
        default:
            fprintf(stderr, "Error: Unknown error.\n");
            break;
    }
    exit(EXIT_FAILURE);
}
