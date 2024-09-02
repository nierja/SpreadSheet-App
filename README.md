# SpreadSheet-App

[![CI Build and Test](https://github.com/nierja/SpreadSheet-App/actions/workflows/ci.yml/badge.svg)](https://github.com/nierja/SpreadSheet-App/actions/workflows/ci.yml)

## Overview

A spreadsheet command-line application written in C++. The application supports common spreadsheet operations, including:

- **Cell operations:** Cells can store values, text, or formulas (ie `1e6, 3.14, "Ahoj\r\n svete", A$6, =-F3+3+$E$E^3`). Formulas can perform basic arithmetic operations and refer to other cells (absolute and realative addressing).
- **Expression parsing:** The application includes an expression parser capable of handling arithmetic expressions and references to other cells.
- **Cell copying:** Including the resolution of relative and absolute references.
- **File operations:** saving and loading to a `std::stringstream`, with checksum validation to ensure data integrity.
- **Error handling:** The app handles cyclic dependencies and other errors, providing appropriate feedback.

## Key Features and used programming techniques

- Parsing is handled by **statically linking a parser** and then building an **abstract syntax tree** from the output (AST)
- AST relies on **runtime polymorphism** and a **virtual class hierarchy**.
- Decomposition of the code to **several `.cpp` and header `.h` files**.
- Automated build via a **Makefile** and a **CI/CD** pipeline on pushing it to GitHub.
- The build itself contains harsh **clang-tidy** checks and performs **static analysis** both before and during build.
- Functionality proven againts a comprehensive suit of **unit tests**.


### Building the app

1. Clone the repository:

    ```bash
    git clone git@github.com:nierja/SpreadSheet-App.git
    cd SpreadSheet-App
    ```

2. You may need to update the version of `libexpression_parser.a` in the `Makefile` to correctly reflect your architecture. The default is the `x86_64-linux-gnu`.

3. Use `make` to build the project.

4. Upon a succesful build, an executable `spreadsheet.out` will be created in the base directory.

### Running the app

You can run the application with a predefined suite of unit tests by launching the binary:

```bash
$ ./spreadsheet.out
PASS - Test 1: Class CPosition
PASS - Test 2: Class CExpression
PASS - Test 3: Static Linking of a Parser
PASS - Test 4: Method CCell::setCell - various datatypes
PASS - Test 5: Method CCell::getValue
PASS - Test 6: Set and evaluate expression as cell content
PASS - Test 7: Save and load spreadsheet - basic save and load
PASS - Test 8: Save and load spreadsheet - handle data corruption
PASS - Test 9: Save and load spreadsheet - handle cell references
PASS - Test 10: Copy cells - basic copying (incl. references)
PASS - Test 11: Copy cells - advanced copying (recursive references)
PASS - Test 12: Save and load spreadsheet - recursive references
PASS - Test 13: Operators - comparative and arithmetic operators
PASS - Test 14: Cyclic dependencies
```