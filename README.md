# Nojs 🚀

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C-orange.svg)
![Status](https://img.shields.io/badge/status-incomplete-yellow.svg)
![Version](https://img.shields.io/badge/version-0.1.0-green.svg)

A lightweight JavaScript-like language implementation written in C. Nojs is a simplified clone of JavaScript with support for structures and dynamic method addition to structures. This project only works on Unix-like operating systems.

> ⚠️ **Note**: This project is incomplete and likely to remain so. The interpreter part is not implemented, although the lexer, parser, and AST components are functional.

## 💻 Installation

### Unix-based Systems (Linux, macOS)

```bash
# Create directory for the project
mkdir -p nojs
cd nojs

# Clone the repository
git clone https://github.com/DilemaFixer/Nojs.git .

# Build the project
gcc -c ./src/*.c -I./src -Wall -Wextra -g -O2
gcc *.o -o nojs
```

## 🔍 Language Features

Nojs is a simple JavaScript-inspired language that includes:

### Data Types
- **Numbers**: Floating-point numeric values
- **Strings**: Text enclosed in quotes
- **Booleans**: `true` and `false` values
- **Null**: Represents absence of value
- **Arrays**: Collections of values
- **Structures**: Similar to JavaScript objects

### Variables and Constants
```
let x = 5;           // Variable declaration
const PI = 3.14159;  // Constant declaration
```

### Control Flow
```
if (condition) {
    // Code to execute if condition is true
} else {
    // Code to execute if condition is false
}

loop (condition) {
    // Code to repeat while condition is true
    
    next;    // Skip to next iteration (like JavaScript's continue)
    stop;    // Exit the loop (like JavaScript's break)
}
```

### Functions
```
function add(a, b) {
    return a + b;
}
```

### Structures
One of the key features of Nojs is the ability to create structures (similar to objects in JavaScript) and dynamically add methods to them.

### Built-in Functions
- `print`: Output values to the console
- `take`: Read input from the user

## 🚧 Project Status

This project is incomplete and development has been paused. The following components are implemented:

- ✅ Lexer (tokenization of source code)
- ✅ Parser (creation of abstract syntax tree)
- ✅ AST (abstract syntax tree representation)
- ✅ Environment (variable scope handling)
- ✅ Core data structures
- ❌ Interpreter (execution engine)

Without the interpreter, the language can parse code but not execute it. The project remains as a demonstration of language implementation concepts.

## 🧠 Project Structure

The Nojs project is organized into several key components:

- **Lexer**: Converts source code into tokens
- **Parser**: Converts tokens into an Abstract Syntax Tree (AST)
- **AST**: Represents the structure of the program
- **Environment**: Manages variable scopes and values
- **Utility Libraries**: Arrays, logging, queues, etc.

Feel free to explore the codebase to understand how a simple programming language can be implemented from scratch.
