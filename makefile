CC = gcc
BISON = bison
FLEX = flex

BISON_FLAGS = -d
FLEX_FLAGS = 
CFLAGS = -Wall -g

# List of source files
SOURCES = setlang.tab.c lex.yy.c data_holder.c statement.c errors_hendler.c helpers.c statement_executor.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: setlang

# Link the object files to create the final executable
setlang: $(OBJECTS)
	$(CC) $(CFLAGS) -o setlang $(OBJECTS)

# Generate the Bison output
setlang.tab.c: setlang.y
	$(BISON) $(BISON_FLAGS) setlang.y

# Generate the Flex output
lex.yy.c: setlang.l
	$(FLEX) $(FLEX_FLAGS) setlang.l

# Clean up generated files
clean:
	rm -f setlang setlang.tab.c setlang.tab.h lex.yy.c $(OBJECTS)

.PHONY: all clean
