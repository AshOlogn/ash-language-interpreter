# ash-language-interpreter
This is an interpreter written in C++ for a toy language (named Ash) that I've invented to better acquaint myself with programming language design, interpreter design, and the C++ language (especially templates and OOP).

## Features
At a high level, this language is statically-typed and supports all the basic features of a procedural language:

### Primitive Types
The primitive types supported are int32, int64, uint32, uint64, char, bool, and string. The C++ interpreter implements the numeric types using the <cstdint> header.

### Operations
The Ash language supports all basic arithmetic and bitwise operations on the numerical types, logical operations on the boolean type, and even string operations like concatenation and reversing using arithmetic operators. The arithmetic and logical operations follow C++ precedence rules, which can be found [here](http://en.cppreference.com/w/cpp/language/operator_precedence).

### Casting
Casting is done implicitly if going from a smaller to larger type (i.e. int32 to double), be it during variable assignment or in expressions. If a value is assigned to a variable of a smaller type, then the interpreter throws an error, since such a conversion could be lossy and thus must be specified explicitly. A wider variety of casts (including lossy ones) can be specified explicitly without creating errors. The syntax for casting is C-style.

### Printing
Printing functionality is provided as a built-in statement. I know that this is much less elegant than providing it in a standard library, but my goal is to keep the language as light-weight as possible while still exhibiting most of the characteristics of a full-fledged language.

The `println` and `print` keywords followed by an expression print the string representation of the expression's evaluated value (with and without trailing newline character respectively). All the primitive types are printable, as are objects (as specified by their class' `toString` method).

### Control Flow
The logical control flow constructs that are supported are also those supported in C/C++: conditionals (`if elif else ...`), `while` loops, and `for` loops. These work the same way as they do in C/C++. The only difference is that the conditions _have_ to be boolean values, not nonzero integer values. 

### Variable Assignment
Ash is a statically-typed language, meaning that type-checking is done _before_ program execution. When a variable is declared, you must provide its type. If you do assign a variable a value in its declaration, that value is implicitly casted to the variable's declared type (if the declared type is smaller, you get an error). Otherwise, the variable is assigned some garbage undefined value until you explicitly assign what you want to it. Variables can be reassigned new values as long as they're still in scope (see below).

### Scope
TODO (mostly done)

### Functions
TODO

### Classes 
TODO

### Inheritance
TODO

