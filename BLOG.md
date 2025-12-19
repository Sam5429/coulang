# Blog

## Plan

### Interpreter

- 1. Lexer -> create the token
- 2. Parser -> convert token to abstract syntaxt tree
- 3. Sementic checker -> tcheck the life time, type
- 4. VM -> run the code (create a stack and push function on it)

## 14th~15th December

### Objetif

- Create the syntax
- Create compilation error meme
- Start the lexer and the parser

### Achievement

We want to start with a simple language with basic operators for logic, such as `+`, `-`, `if`, `else`, `while`, and functions.

To begin, we agreed on the necessary components and created some useful macros, tokens to represent our words, and a lexer that converts the code into tokens. The lexer can produce errors if it cannot convert the text into a token and provides the position of the error.

After that, we created an abstract syntax tree (AST) to link our nodes. However, we encountered a problem when the nodes were incorrectly linked together.

## todo 

add the position with the token so we can know were the error occure in the parser
