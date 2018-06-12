# ash-language-interpreter
This is an interpreter written in C++ for a toy language (named Ash) that I've invented to better acquaint myself with programming language design, interpreter design, and the C++ language (especially templates and OOP).

## Features
At a high level, this language is statically-typed and supports all the basic features of a procedural language:
* Primitive types: signed and unsigned 32-bit and 64-bit integers, double, char, boolean, and string
* Arithmetic on these primitives (including bitwise operations, comparisons, and logical operations) consistent with C precedence rules
* Explicit and implicit casting (i.e. both "abc" + 123 = "abc123" and explicit (int64) 5.5 = 5) 
* Printing
* Control flow
* Variable assignment
* Scope

Additionally, string operations are built into the primitive arithmetic operations, so addition can be used to concatenate strings and multiplication by a signed integer can be used to repeat and reverse them.

## Syntax
The syntax is inspired by C but has a few notable differences. For example, semicolons are NOT required to end statements, and functions are first-class, meaning that they can be passed around just like other data types. In fact, this language is really flexible in that functions can be declared both in the traditional sense (think C style) and assigned to variables.

Additionally, the "slice" syntax is borrowed from Python to get extract substrings from strings, and negative indexing is also supported.

## Todo
So far, I have implemented arithmetic, implicit casting, and basic conditionals. Variable assignment is a simple next step, and function support (especially closure) will be the next big challenge. Stay tuned!
