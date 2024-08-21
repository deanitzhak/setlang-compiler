# SetLang Compiler

## Overview

The SetLang Compiler is designed to parse and interpret a custom language with support for basic data types such as collections, sets, integers, and strings. The language allows for variable declarations, assignments, operations, control structures (like loops and conditional statements), and input/output functionalities. This project leverages GNU Bison and Flex to define the grammar and lexical analysis of the language.

## Features

- **Data Types**: Supports `int`, `str`, `set`, and `collection`.
- **Variable Declarations**: Define variables and assign values.
- **Operations**: Arithmetic operations for integers, set operations (union, intersection), and string concatenations.
- **Control Structures**: Includes `for` loops, `while` loops, `if-else` statements.
- **Input/Output**: Handles input from the user and outputs results.

## Getting Started

**Note:** These instructions are tailored for a Linux environment (such as Ubuntu).

### Prerequisites

- GNU Bison (version 3.8.2 or later)
- Flex
- GCC (GNU Compiler Collection)
- Make (for automating the build process)

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/deanitzhak/setlang-compiler.git
   cd setlang-compiler

2. Install dependencies:
    sudo apt-get install bison flex gcc make

3. Build the project:
    make

####
1.  Run the compiler with a program file:
    ./setlang program_1.txt

2. Or with another program file:
    ./setlang program_2.txt

File Structure
src/:               Contains the source code for the compiler.
setlang.y:          The Bison file defining the grammar.
setlang.l:          The Flex file defining the lexical analysis.
data_holder.h:      Handles data structures for variables and operations.
statement.h:        Manages statements and control flow.
helpers.h:          Contains helper functions for processing input and managing memory.
errors_handler.h:   Handles error reporting and recovery.
bin/:               Compiled binary files.
Makefile:           Automates the build process.

### Example Programs
1. Simple variable declarations:
    int x = 5;
    str name = "John";
    set grades = {1, 2, 3};
    collection students = {"Alice", "Bob", "Charlie"};

2. Conditional statements:
    if (x > 3)
        output "x is greater than 3";
    else 
        output "x is not greater than 3";
    for (student in students) 
        output student;

3. Operations on variables:
    x = x + 2;
    grades = grades + {4, 5, 6};

#### Contributing
    If you would like to contribute to this project, please fork the repository and submit a pull request. All contributions are welcome!

##### License
    This project is licensed under the MIT License - see the LICENSE file for details.



