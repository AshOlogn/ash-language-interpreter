# ash-language-interpreter

## Table of Contents
[About](#about)  
[Features](#features)
- [Primitive Types](#primitive-types)
- [Array Type](#array-type)
- [Operations](#operations)
    - [Arithmetic Operations](#arithmetic-operations) 
    - [Logical Operations](#logical-operations)
    - [String Arithmetic](#string-arithmetic)
    - [Array Arithmetic](#array-arithmetic)
    - [String/Array Indexing](#string-and-array-indexing)
    - [Casting](#casting)
    - [Printing](#printing)
- [Control Flow](#control-flow)
- [Variable Assignment](#variable-assignment)
- [Scope](#scope)
- [Functions](#functions)
- [Error Handling](#error-handling)

[Using the Interpreter](#using-the-interpreter)

## About
This is an interpreter written in C++ for a toy language (named Ash) that I've invented to better acquaint myself with programming language design, interpreter design, and the C++ language (especially templates and OOP).

## Features
At a high level, this language is statically-typed, statically-scoped, and supports all the basic features of a procedural language:

### Primitive Types
The primitive types supported are int32, int64, uint32, uint64, char, double, bool, and string. The C++ interpreter implements the numeric types using the <cstdint> header.

### Array Type
This language supports single-dimensional arrays of the primitive types described above. There are 2 ways of declaring them. One is to just allocate memory for an array of a certain length and the other is to provide an initializer list.
```
int[] x = new int[5]
int[] y = [1,2,3,4]
```
The language does not currently support arrays of arbitrary types, so multi-dimensional arrays are not possible.  

If indices outside the bounds of the array are assigned to or accessed, an `OutOfBoundsException` is thrown at runtime. Also keep in mind that when an array is allocated with the `new` keyword, it has no meaningful initial values.

### Operations
The Ash language supports all basic arithmetic and bitwise operations on the numerical types, logical operations on the boolean type, and even arithmetic operations on strings and arrays. The arithmetic and logical operations follow C++ precedence rules, which can be found [here](http://en.cppreference.com/w/cpp/language/operator_precedence).

#### Arithmetic Operations
The arithmetic operations that are supported are (in order of precedence):
- **Unary Positive**: `+num1`, returns the original number
- **Unary Negation**: `-num1`, returns the original number but negated
- **Exponentiation**: `num1 ** num1`, where `num1` and `num2` are any numbers
- **Multiplication**, **Division**, and **Modulo**
    - Multiplication: `num1 * num2`, where `num1` and `num2` are any numbers
    - Division: `num1 / num2`, where `num1` and `num2` are any numbers
        - If they are both integers, the result is that of integer division (i.e. `5/2 == 2`)
    - Modulo: `num1 % num2`, where `num1` and `num2` are any numbers
- **Addition** and **Subtraction**
    - Addition: `num1 + num2` where `num1` and `num2` are any numbers
    - Subtraction: `num1 - num2` where `num1` and `num2` are any numbers
- **Bitwise AND**: `num1 & num2`, where `num1` and `num2` are both integers
- **Bitwise XOR**: `num1 ^ num2`, where `num1` and `num2` are both integers
- **Bitwise OR**: `num1 | num2`, where `num1` and `num2` are both integers

#### Logical Operations
- **Logical NOT**: `!b1`, where `b1` is a boolean
- **Logical AND**: `b1 && b2`, where `b1` and `b2` are booleans
- **Logical XOR**: `b1 ^^ b2`, where `b1` and `b2` are booleans
- **Logical OR**: `b1 || b2`, where `b1` and `b2` are booleans

#### String Arithmetic
- **Unary Negation**: `-str1`, returns a deep copy of the reversed string
- **Unary Positive**: `+str1`, returns an identical deep copy of the string
- **Addition**: `str1 + str2`, string concatenation
- **Multiplication**: `val1 * val2`, where one of the values is a string and the other is an integer
    - Returns the string concatenated to itself the integer number of times (i.e. `"abc" * 2` yields `"abcabc"`)
    - If the integer is signed and negative, the above is done with the integer's absolute value and the result is reversed (i.e. `"abc" * -2` yields `"cbacba"`)

#### Array Arithmetic
These are the same as the associated string operations, but what is done to the characters is instead done to array elements. Think of strings as character arrays, even though they aren't implemented the same way in the Ash language. 

The following examples show how arithmetic can be done with arrays:
- `[1,2,3] + [7,8,9]` yields `[1,2,3,7,8,9]`
- `[1,2,3] * 2` yields `[1,2,3,1,2,3]`
- `[1,2,3] * -2` yields `[3,2,1,3,2,1]`
- `-[1,2,3]` yields `[3,2,1]` 


#### String and Array Indexing
Indexing of strings and arrays is done Python style, meaning that you can access both individual elements as well as slices. Positive indices are zero-indexed and negative indices count from the back, with -1 denoting the last element (different from Python, in which -1 denotes the second to last element). I'd rather not belabor a system already described by the Python language, so here are some examples of how indexing works:
- `"abcd"[0]` returns `'a'`
- `[1,2,3][1]` returns `2`
- `"abcd"[-2]` returns `'c'`
- `[1,2,3,4][2:-1]` returns `[3,4]`
- `"abcd"[2:1]` returns `""`

#### Casting
Casting is done implicitly if going from a smaller to larger type (i.e. int32 to double) or to a string in all cases, be it during variable assignment or in expressions. If a value is assigned to a variable of a smaller type, then the interpreter throws an error, since such a conversion could be lossy and thus must be specified explicitly. A wider variety of casts (including lossy ones) can be specified explicitly without creating errors. The syntax for casting is C-style.  

The following are examples of valid implicit casts:
- `int32 x = 'A' //cast from 1-byte char to 4-byte integer`
- `double y = 2+3 //cast from integer to double`
- `string z = 3.5 //cast from double to string`

The following are examples of valid explicit casts:
- `int32 x = (int32) 5.5 //x = 5`
- `uint64 y = (uint64) 10.5 //y = 10`

The following are examples of invalid casts:
- `uint32 x = 5.5 //this is lossy and must be explicit`
- `int x = (int) "1234" //this is not allowed even as an explicit cast` 


#### Printing
Printing functionality is provided as a built-in statement. I know that this is much less elegant than providing it in a standard library, but my goal is to keep the language as light-weight as possible while still exhibiting most of the characteristics of a full-fledged language.

The `println` and `print` keywords followed by an expression print the string representation of the expression's evaluated value (with and without trailing newline character respectively). All the primitive types are printable, as are arrays.

### Control Flow
The logical control flow constructs that are supported are also those supported in C/C++: conditionals (`if elif else ...`), `while` loops, and `for` loops. These work the same way as they do in C/C++. The only difference is that the conditions _have_ to be boolean values, not nonzero integer values. 

### Variable Assignment
Ash is a statically-typed language, meaning that type-checking is done _before_ program execution. When a variable is declared, you must provide its type. If you do assign a variable a value in its declaration, that value is implicitly casted to the variable's declared type (if the declared type is smaller, you get an error). Otherwise, the variable is assigned some garbage undefined value until you explicitly assign what you want to it. Variables can be reassigned new values as long as they're still in scope (see below).

### Scope
The concept of scope is a simplified version of that found in existing programming languages. The outermost scope is considered _global_ and functions and variables declared here are accessible anywhere else in the program. New scopes are created inside a function body and inside braces.  

If a variable or function is declared when one of the same name exists in an outer scope, the innermost version is used:
```
int x = 5
{
    double x = 5.5
    println x //5.5 is the output
}
```

It is an error to declare a functions or variables of the same name multiple times in the same scope:
```
{
    int x = 5

    fun x(int x, string y) -> string {
        return x * y
    }

    //the second declaration is illegal
}
```

### Functions
Functions have the following syntax:
```
fun name(parameters...) -> returnType {
    body with return statements if necessary
}
```

Here are some examples:
```
fun foo(int32 x) -> string {
    return x + "abc"
}

fun fib(int n) -> int {
    if(n == 0)
        return 0
    elif(n == 1)
        return 1
    else
        return fib(n-1) + fib(n-2)
}
```

As the last example suggests, recursion _is_ supported by the Ash language.  

Additionally, functions can access variables in their _environment_, that is, variables in enclosing scopes, within their body:
```
int x = 4
fun foo(int y) -> int {
    return x + y
}

println foo(5) //prints 9
```
Relying on variables in enclosing scopes is risky as those variables can be changed by other entities, potentially leading to unexpected behavior.  

### Error Handling
An important feature of the Ash programming language is its robust static handling of syntax, type, and scope errors, as well as array access errors at runtime.  

If an error is detected, a descriptive error is printed, along with the line number(s) involved and the problematic section of code. The following are examples of bad code and associated error messages:

Type Error
```
int x = 5.5
```

```
StaticCastError on line 1
	int x = 5.5
Cannot implicitly cast from type double to type int32
```

Scope Error
```
int x = 5
double x = 5.0
```
```
StaticVariableScopeError on line 2
	double x = 5.0
The variable x is already declared in this scope. It cannot be re-declared in the same scope.
```

Note that since type, scope, and syntax errors are statically detected, error messages are output _before_ any code is actually executed. For example, the string "abcabc" is never printed in the following code:

```
int x = 2
string y = "abc" * x

println y
int z = (int) y
println z
```
```
StaticCastError on line 5
	int z = (int) y
Cannot cast at all from type string to type int32
```

Additionally, errors in which arrays and strings are indexed out-of-bounds are caught at runtime and appropriately handled:

```
string x = "abc"
x += "defg"       // x = "abcdefg"
println x[:100]
```
```
OutOfBoundsException on line 3:
		println x[:100]
The index 100 is out of bounds in string of length 7
```

## Using the Interpreter
To run the Ash language interpreter, first clone this repository using the following command:
```
git clone https://github.com/AshOlogn/ash-language-interpreter.git
```
Then run `make` in the root of the project directory to produce the executable. To interpret Ash source code, run the executable with the name of the source file as the sole argument (i.e. `bin/ash test.ash`). If you want to interpret source code from any directory conveniently, add the path to executable's bin to the `PATH` environment variable and just use the `ash` command to execute Ash code (i.e. `ash test.ash`). The conventional file extension for Ash source code is `.ash`.
