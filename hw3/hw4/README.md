# Project Assignment 4 - Semantic Analyses

**Introduction to Compiler Design by Prof. Yi-Ping You**

Due Date: **23:59, December 27, 2019**

---

**Table of Contents**
- [Project Assignment 4 - Semantic Analyses](#project-assignment-4---semantic-analyses)
	- [Overview](#overview)
	- [Assignment Description](#assignment-description)
		- [0x00 Introduction to Symbol Table](#0x00-introduction-to-symbol-table)
			- [Implementation](#implementation)
			- [Operations](#operations)
			- [Scope Management](#scope-management)
			- [Scope Rules](#scope-rules)
			- [Output Format](#output-format)
			- [Pseudocomments](#pseudocomments)
	- [Semantic Definition](#semantic-definition)
	- [Implementation Hints](#implementation-hints)
		- [Symbol Table Construction (one-pass)](#symbol-table-construction-one-pass)
		- [Source Code Listing in Semantic Error](#source-code-listing-in-semantic-error)
		- [Visiting of FunctionNode](#visiting-of-functionnode)
		- [Type Information Propagation](#type-information-propagation)
	- [What Should Your Parser Do?](#what-should-your-parser-do)
	- [Project Structure](#project-structure)
	- [Assessment Rubrics (Grading)](#assessment-rubrics-grading)
	- [Build and Execute](#build-and-execute)
		- [Build Project](#build-project)
		- [Test your parser](#test-your-parser)
	- [Submitting the Assignment](#submitting-the-assignment)

## Overview

In this assignment, you will extend your parser to perform semantic analyses for a given program written in `P` language using the information recorded in the AST, which is constructed in the previous assignment.

This assignment requires you to construct a symbol table for performing semantic analyses this time and code generation in the last assignment.
You should design it using feasible data structures. **DO NOT just use string, unless you're obsessed with parsing strings for extracting information from them.**

## Assignment Description

The purpose of this assignment is to get you acquainted with semantic analysis. You'll learn what should be done to achieve it and how does it work.

### 0x00. Introduction to Symbol Table

In the previous assignment, we have constructed an AST for the given program. In a program, the most common behavior is that we declare some variables and use them later.
However, it's quite painful to walk back and forth between a declaration node and a variable reference node in the AST.

As a result, we need to maintain a data structure, symbol table, which stores some information when we encounter a declaration. After that, we can directly get information from the symbol table when a reference is analyzed instead of going back to find the declaration.

A symbol table is used for the following purposes:

- To pass information from declarations to uses
- To verify if a variable has been declared before use
- To help type checking when analyzing assignments, operations, and return statement

**Components**

A symbol table is simply a table that contains entries for each name of program, functions, or variables. Each entry consists of the following components:

| Field | Description |
| ----- | ----------- |
| Name | The name of the symbol. Each symbol have the length between 1 to 32. |
| Kind | The name type of the symbol. There are **six** kinds of symbols: program, function, parameter, variable, loop\_var, and constant. |
| Level | The scope level of the symbol. 0 represents the global scope. Local scope levels start from 1, and the scope level is incremented at the start of a scope and decremented at the end of the scope.  |
| Type | The type of the symbol. Each symbol is of types integer, real, boolean, string, or the signature of an array. (Note that this field can be used for the return type of a function ) |
| Attribute | Other attributes of the symbol, the value of a constant or list of the types of the formal parameters of a function. |

#### Implementation

A symbol table can be implemented in one of the following ways:

- Linear list
- Binary search tree
- Hash table

#### Operations

A symbol table should provide the following operations:

- insert
	- used to add information in the symbol table
- lookup
	- used to search a identifier in the symbol table to perform semantic analyses

#### Scope Management

A scope corresponds to a symbol table. `ProgramNode`, `FunctionNode`, `ForNode` and `CompoundStatementNode` are nodes that form a scope.

Following are the special rules that you should be careful of:

- A `FunctionNode` should share the same symbol table with its body (CompoundStatementNode). More specifically, parameters of a `FunctionNode` should be declared in the same symbol table with those declared in the `CompoundStatementNode`.
- A `ForNode` contains a symbol table for the loop variable. That is, there is a scope (symbol table) in a `ForNode` with **only one symbol of a loop variable**.

#### Scope Rules

- Scope rules are similar to C.
- Name must be unique within a given scope. The identifier designates the entity declared closest to it, that is, the identifier declared in the outer scope is hidden by the one declared in the inner scope.
	- Unlike the normal variable, the symbol of a loop variable **CANNOT** be redeclared whether it's in the same scope or the inner scope.
- Declarations within a compound statement, a function, or a for statement are local to the statements in the same block or the inner block, and no longer exist after exiting the block in which it's declared.

#### Output Format

> Note that your parser should dump the symbol table to **`stdout`**.

Format of each component:

- Name
	- just characters
- Kind
	- 6 kinds of symbols with respective strings: `program`, `function`, `parameter`, `variable`, `loop_var`, and `constant`
- Level
	- 0: `"0(global)"`
	- other level: `n(local)`
- Type
	- just type string, same rule as hw3
- Attribute
	- constant
		- just string of constant
	- types of formal parameters
		- type strings seperated by `", "` (e.g., `real, real [2]`)

Format of whole table:

```c
void dumpDemarcation(const char chr) {
  for (size_t i = 0; i < 110; ++i) {
    printf("%c", chr);
  }
  puts("");
}

void dumpSymbol(void) {
  dumpDemarcation('=');
  printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type",
                                        "Attribute");
  dumpDemarcation('-')
  {
    printf("%-33s", "func");
    printf("%-11s", "function");
    printf("%d%-10s", 0, "(global)");
    printf("%-17s", "boolean");
    printf("%-11s", "integer, real [2][3]");
    puts("");
  }
  dumpDemarcation('-')
}
```

#### Pseudocomments

In the first assignment, we have defined:

- `S`
	- `&S+` turns on source program listing, and `&S-` turns it off.
- `T`
	- `&T+` turns on token (which will be returned to the parser) listing, and `&T-` turns it off.

In this assignment, one more option is added:

- `D`
	- Dump the contents of the symbol table associated with a block when exiting from that block.
	- `&D+` turns on symbol table dumping, and `&D-` turns it off.
	- By default, this option is on.
	- In test cases, this option won't be turned on/off in the middle of the program, it will only be set before the `ProgramName`.

## Semantic Definition

This section describes only the semantic definitions. When your parser encounters a semantic error, the parser should report an error with relevant error messages and format, which are described in [error-message.md](./error-message.md).

Note that **once your parser has found a semantic error in a child node of an AST node, the parser doesn't check semantic errors related to the child node of the AST node.**

### Program Unit

The two program units are the *program* and the *function*.

#### Program

- The identifier after the **end** of a program declaration must be the same as the name given at the beginning of the declaration. Besides, it must be the same as the file name. For example, for the input file "test.p", the identifier at the beginning and the end of the program declaration must be "test".

- A program has no return value, thus any return statement that appeared in the main block of the program is illegal.

#### Function

- The identifier after the **end** of a function declaration must be the same as the name given at the beginning of the declaration.

- The type of the return statement inside the function must be the same as the return type of the function declaration.

### Scope Rules

- Scope rules are similar to C.

- Name must be unique within a given scope. Within the inner scope, the identifier designates the entity declared in the inner scope; the entity declared in the outer scope is hidden within the inner scope.

- A compound statement forms an inner scope. Note that declarations inside a compound statement are local to the statements in the block and no longer exist after the block is exited.

- A counting iterative control statement has a variable, called the **loop variable**, in which the count value is maintained. The loop variable is implicitly declared at the **for** statement and implicitly undeclared after loop termination. As a result, there is a scope in the loop with **only one symbol of loop var kind**.

- Unlike the normal variable, the symbol of a loop variable **can not** be redeclared whether it's in the same scope or the inner scope.

### Variable Declaration and Reference

- In an array declaration, the index of the lower bound must be smaller than that of the upper bound. Both of the indices must be greater than or equal to zero; actually, a non-conforming input was already blocked according to the syntactic definition.

- If there is an error in the array declaration, further checking is unnecessary when visiting the Variable Reference Node which is a reference to that array.

- Each index of an array reference must be of the integer type. Further checking regarding the array reference is unnecessary if an incorrect index was found within the array reference. The order of index checking is from left to right. Bound checking is not performed at compile time as in C language.

- An over array subscript is not allowed, that is, the number of indices of an array reference cannot be greater than the number of dimensions in the declaration.

### Expression

- For an arithmetic operator (+, -, *, or /), the operands must be an integer or real type, and the operation produces an integer or real value. The type of operands of operation may be different. Check out the *Type Coercion and Comparison* subsection for more details.

- For a **mod** operator, the operands must be an integer type, and it produces an integer value.

- For a Boolean operator (**and**, **or**, or **not**), the operands must be Boolean types, and the operation produces only Boolean value.

- For a relational operator (**<**, **<=**, **=**, **>=**, **>**, or **<>**), the operands must be an integer or real type, and the operation produces only Boolean value. Operands must be of the same type after appropriate type coercion.

- String operands can only appear in "+" operations (string concatenations), assignment statements, print statements and read statements. Notice that when doing string concatenation, both operands must be a string type.

### Type Coercion and Comparison

- An integer type can be implicit converted into a real type due to several situations, such as assignments, parameter passing, or arithmetic expressions.

- The result of an arithmetic operation will be real type if at least one of the operands is the real type. For example, `1.2 + 1`, `3 - 1.2`, and `1.2 * 3.4` are all result in real type.

- Two arrays are considered to be the same type if they have the same number of elements. More specifically, they have the same number of dimensions and the same size for each dimension and the type of element is the same.

### Statement

There are 7 distinct kinds of statements: compound, simple, conditional, while, for, return, and function call.

#### Simple

- Variable references of **print** or **read** statements must be scalar type.

- In assignment statements, the type of the left-hand side must be the same as that of the right-hand side unless type coercion is permitted.

- Assignment to constant and array assignment are not allowed.

#### If and While

- The conditional expression part of **if** and **while** statements must be Boolean types.

#### For

- The value of the loop variable can not be changed inside the loop.

- In a loop scope, the loop variable must be different from any other variable declaration including the nested loop variable.

- The **loop parameters** used to compute an iteration count must be in the incremental order and greater than or equal to zero; actually, a non-conforming form of the latter rule was already blocked according to the syntactic definition.

#### Return

- Check the definitions about return value in Sections [Program](#program) and [Function](#function).

#### Function Call

- A procedure is a function that has no return value.

- The number of actual parameters must be the same as the number of formal parameters in the function declaration.

- The types of actual parameters must be identical to the types of formal parameters in the function declaration.

### Identifier

- The first 32 characters are significant. That is, the additional part of an identifier will be discarded by the parser.

## Implementation Hints

### Symbol Table Construction (one-pass)

Since we need to **push** a symbol table when entering a new scope and **pop** a symbol table when exiting a scope, the property of stack - LIFO is really suitable. As a result, you may have a code snippet like this:

```cpp
class SymbolEntry {
private:
    // Variable names
    // Kind
    // Level
    // ...
};

class SymbolTable {
public:
    void addSymbol(/* attributes needed by a SymbolEntry */);
    // other methods
private:
    std::vector<SymbolEntry> entries;
};

class SymbolManager {
public:
    // Pass-by-value is slow and has unnecessary object construction,
    // but otherwise the implementation is simpler.
    // For efficientcy, you may try to learn how to use rvalue reference
    void pushScope(SymbolTable new_scope);
    void popScope();
    // other methods
private:
    std::stack <SymbolTable> tables;
};
```

### Source Code Listing in Semantic Error

To achieve this function, you need to revise your `scanner.l`.

Here are some possible approaches to implement this function:

1. Use `strdup()` to record each line of source code in an array of pointer to `char`.
2. Record the file position of head of each line in an array of `long`, then use `fseek()` + `fgets()` to get the source code when needed.

### Visiting of FunctionNode

Since parameters need to be in the same scope as the body (compound statement) of FunctionNode, you may need to skip visiting the body (compound statement node) in FunctionNode and visit the child nodes of that CompoundStatementNode directly.

### Type Information Propagation

Some semantic checks are related to type incompatibility. You need to design a mechanism to propagate the type information from a child node to its parent node. For example, you need to propagate the type of the result of an expression to a BinaryOperatorNode, so that it can check whether its operation is legal or not.

Therefore, you may need to implement a class/struct to represent the type of P language and then store it in an ExpressionNode or somewhere else to propagate the type information.

## What Should Your Parser Do?

If the input file is syntactically and semantically correct, output the following message.

```
|---------------------------------------------|
|  There is no syntactic and semantic error!  |
|---------------------------------------------|
```

Once the parser encounters a semantic error, output the related error message.

Notice that semantic errors should **not** cause the parser to stop its execution. You should let the parser keep working on finding semantic errors as much as possible.

## Project Structure

- `README.md`
- /src
	- Makefile
	- `scanner.l`
	- `parser.y`
	- /include
		- /AST
		- /semantic
		- /visitor
	- /src
		- /AST
		- /semantic
		- /visitor
	- Other modules you may add
- /report
	- `README.md`

In this assignment, you have to do the following tasks:

- Revise `scanner.l`, `parser.y`, and add some modules (e.g., `SymbolTable.[hc]`, `SemanticAnalyzer.[hc]`) to perform a semantic analysis.
- Write a report in `report/README.md`. The report should at least describe the changes you have made in `parser.y` and the abilities of your AST.

If you want to preview your report in GitHub style markdown before pushing to GitHub, [`grip`](https://github.com/joeyespo/grip) might be the tool you need.

## Assessment Rubrics (Grading)

Total of 100 points, with 15 point available bonus points.
(115 pts is the maximum grade you could get in this assignment)

+ Passing all test cases (60 pts)
+ Passing all hidden test cases (35 pts)
+ Report (5 pts)
+ Bonus
  + Preserving symbol tables for multi-pass (+10 pts)
  + No memory leak in your program (+5 pts)

**Please note down what bonus you have done in your report.**

## Build and Execute

- Build: `cd src && make clean && make`
- Execute: `./parser [input file]`
- Test: `cd test && python3 test.py`

### Build Project

TA would use `src/Makefile` to build your project by simply typing `make clean && make` on CS workstation. You have to make sure that it will generate an executable named '`parser`'. **No further grading will be made if the `make` process fails or the executable '`parser`' is not found.**

### Test your parser

We provide some basic tests in the test folder. Simply `cd` to test folder and type `python3 test.py` to test your parser. You can also type `python3 test.py --help` to know what arguments you can set. For example, you can add argument "--symbol" or "--sema" to test the cases of symbol table or semantic analyses.

The objective we provide sample test cases is making sure your parser outputs in correct format and parses program correctly. You will get at least **60 pts** if you pass all the test cases.

Please use `student_` as the prefix of your own tests to prevent TAs from overriding your files. For example: `student_identifier_test`.

If you pass all test cases, you will get:

<img src="./imgs/pass.png" width="360">

You will get the following output messages if your parser outputs a wrong format (A blue line with a prefix sample: shows a sample solution, and a green line with a prefix yours: is your output):

<img src="./imgs/error.png" width="360">

## Submitting the Assignment

You should push all your commits to the designated repository (hw4-\<Name of your GitHub account\>) under the compiler-f19 GitHub organization by the deadline (given in the very beginning of this assignment description).
At any point, you may save your work and push commits to your repository. You **must** commit your final version to the **master branch**, and we will grade the commit which is last pushed on your master branch. The **push time** of that commit will be your submission time, so you should not push any commits to the master branch after the deadline if you have finished your assignment; otherwise, you will get a late penalty.

Note that the penalty for late homework is **15% per day** (weekends count as 1 day). Late homework will not be accepted after sample codes have been posted.

In addition, homework assignments **must be individual work**. If we detect what we consider to be intentional plagiarism in any assignment, the assignment will receive reduced or, usually, **zero credit**.
