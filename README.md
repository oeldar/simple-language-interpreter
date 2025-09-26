# Simple Language Interpreter

A simple programming language interpreter built with Flex and Bison, featuring program evaluation using Abstract Syntax Trees (AST).

## Features

The interpreter supports the following language features:

- **Data Types**: Integer as the only arithmetic data type
- **Operations**: Arithmetic, relational, and logical operations on integers
- **Variables**: Integer variable definitions with C-style naming
- **Code Blocks**: Block structure using `{` and `}`
- **Control Flow**: `if-then-else` statements
- **Loops**: `while-do` and `for` loops
- **Functions**: Function definition and calling

## Language Syntax Examples

### Function Definition and Call
```c
fn foo() {
    print(1);
    print(2);
    print(3);
}

foo(); // Function call
```

### Variables and While Loop
```c
a = 5;
while a do {
    a = a - 1;
    print(a);
}
```

### Function with Parameters
```c
fn add(x, y) {
    return x + y;
}

result = add(10, 20);
```

## Architecture

The interpreter implements three main node types for function handling:

### FunctionNode
- Represents the function itself and its body
- Evaluation executes the function code
- Stored in Environment's function map

### FunctionDefinitionNode
- Represents function definition in source code
- Creates FunctionNode and registers it in Environment
- Makes function available for calls

### FunctionCallNode
- Represents function invocation
- Retrieves function from Environment map
- Evaluates parameter expressions
- Maps parameters to function arguments
- Executes function body

## Implementation Details

- **Lexical Analysis**: Flex (`lang.l`) for tokenization
- **Parsing**: Bison (`lang.y`) for syntax analysis  
- **Evaluation**: AST-based program execution via expression tree
- **Environment**: Variable and function scope management
- **Testing**: Comprehensive test suite with 24 test cases
- **Build System**: Makefile with C++14 standard compliance

## Building and Running

```bash
# Build the interpreter
make

# Run with input file
./lang < input_file.txt

# Run tests
make test

# Update test suite
make update-test

# Clean build files
make clean
```

## Technology Stack

- **Flex**: Lexical analyzer generator
- **Bison**: Parser generator
- **C/C++**: Implementation language
- **AST**: Abstract Syntax Tree evaluation

## Project Structure

```
├── lang.l              # Flex lexical analyzer
├── lang.y              # Bison grammar rules
├── environment.hpp     # Environment for variables/functions
├── expression_tree.hpp # AST node implementations
├── Makefile           # Build configuration
├── README.md          # Project documentation
└── test/              # Test suite with input/output files
    ├── start.sh       # Test runner script
    ├── test01.in/out  # Test case 01
    ├── test02.in/out  # Test case 02
    └── ...            # Additional test cases (24 total)
```
