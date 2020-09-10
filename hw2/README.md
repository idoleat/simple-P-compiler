# Project Assignment 2 - Syntactic Definitions

**Introduction to Compiler Design by Prof. Yi-Ping You**

### Due Date:

|group| due date |
|:-:|:-:|
|inservice-student|**23:59, May 3, 2020**|
|master-student|**23:59, May 5, 2020**|

Your assignment is to write an **LALR(1)** parser for the **`P`** language. You will have to write the grammar and create a parser using `yacc`.

Your programming assignments are based around this division and later assignments will use the parts of the system you have built in the earlier assignments; that is, in the third and fourth assignments you will implement the semantic definition based on first and second assignments, and in the last assignment you will generate RISC-V assembly code by augmenting your yacc parser.

The grammer is subject to modification as the semester progresses. You should make sure the program you write is well-structured and easily changed.

---

**Table of Contents**
- [Project Assignment 2 - Syntactic Definitions](#project-assignment-2---syntactic-definitions)
    - [Due Date:](#due-date)
  - [Assignment](#assignment)
  - [Syntactic Definitions](#syntactic-definitions)
    - [Program Units](#program-units)
    - [Data Types and Declarations](#data-types-and-declarations)
    - [Statements](#statements)
    - [Expressions](#expressions)
  - [Implementation Notes](#implementation-notes)
  - [What Should Your Parser Do?](#what-should-your-parser-do)
  - [Project Structure](#project-structure)
  - [Build and Execute](#build-and-execute)
    - [Build project](#build-project)
    - [Test your parser](#test-your-parser)
  - [Submitting the Assignment](#submitting-the-assignment)

---

## Assignment

You must create an **LALR(1)** grammar using `yacc`. You need to write the grammar following the syntactic in the following sections. Once the LALR(1) grammar is defined you can then execute `yacc` to produce a C program called `"parser.c"`, which contains the parsing function `yyparse()`. You must supply a **main** function to invoke `yyparse()`. The parsing function `yyparse()` calls `yylex()`. You will have to revise your scanner function `yylex()`.

## Syntactic Definitions

### Program Units

The two program units are the *program* and the *functions*.

 - #### Program
    <details>
    A program has the form:

        identifier;
        <zero or more variable and constant declaration>
        <zero or more function declaration and definition>
        <one compound statement>
        end

    A program has no arguments, and hence no parentheses are present in the header. There are two types of variables in a program:

    - **global variables:**   declared after the identifier but before the compound statement
    - **local variables:**   declared inside the compound statement and functions
    </details>

 - #### Function
    <details>
    A function declaration has the following form:

        identifier (<zero or more formal arguments>): scalar_type;

    A function definition has the following form:

        identifier (<zero or more formal arguments>): scalar_type
        <one compound statement>
        end

    Parentheses are required even if no arguments are declared. No functions may be declared inside a function.

    The formal arguments are declared in a formal argument section, which is a list of declaration separated by semicolons. Each declaration has the form

        identifier_list: type

    where identifier_list is a list of identifier separated by comma:

        identifier, identifier, ..., identifier

    At least one identifier must appear before each colon, which is followed by exactly one type(include array) specification. Note that if arrays are to be passed as arguments, they must be fully declared. All arguments are passed by values.

    Functions may return one value or no value at all. Consequently, the return value declaration is either a type name or is empty. If no return value is declared, there is no colon before the terminating semicolon. A function that returns no value can be called a `"procedure"`. For example, following are valid function declaration headers:

        func1(x, y: integer; z: string): boolean;
        func2(a: boolean): string;
        func3();        // procedure
        func4(b: real); // procedure
        func5(): integer;
    </details>

### Data Types and Declarations

 - #### Variable
    <details>

    The four predefined scalar data types are `integer`, `real`, `string`, and `boolean`. The only structured type is the `array`. A variable declaration has the form:

        var identifier_list: scalar_type;

    or

        var identifier_list: array integer_constant of type;

    where integer_constant should be a non-negative integer constant which represents the size of the array.
    </details>

 - #### Constant
    <details>
    A constant declaration has the form:

        var identifier_list: literal_constant;

    where literal_constant is a constant of the proper type (e.g. an integer or real literal w/ or w/o a negative sign, string literal, true, or false).

    > Note that assignments to constants are not allowed and constants can not be declared in terms of other named constants. Variables may not be initialized in declarations.

    </details>

### Statements

There are seven distinct types of statements: compound, simple, conditional, while, for, return, and procedure call.

 - #### compound
    <details>

    A compound statement consists of a block of statements delimited by the keywords `begin` and `end`, and an optional variable and constant declaration section:

        begin
        <zero or more variable and constant declaration>
        <zero or more statements>
        end

    Note that declarations inside a compound statement are local to the statements in the block and no longer exist after the block is exited.
    </details>

 - #### simple
    <details>
    The simple statement has the form:

        variable_reference := expression;

    or

        print variable_reference;

    or

        print expression;

    or

        read variable_reference;

    A *variable_reference* can be simply an *identifier* or an *array_reference* in the form of

        identifier [expression] [expression] [...]
    </details>

 - #### conditional
    <details>
    The conditional statement may appear in two forms:

        if expression then
        <one compound statement>
        else
        <one compound statement>
        end if

    or

        if exprression then
        <one compound statement>
        end if
    </details>

 - #### while
    <details>
    The while statement has the form:

        while expression do
        <one compound statement>
        end do
    </details>

 - #### for
    <details>
    The for statement has the form:

        for identifier := integer_constant to integer_constant do
        <one compound statement>
        end do

    </details>

 - #### return
    <details>
    The return statement has the form:

        return expression ;
    </details>

 - #### procedure call
    <details>
    A procedure is a function that has no return value. A procedure call is then an invocation of such a function. It has the following form:

        identifier (<expressions separated by zero or more comma>) ;
    </details>

### Expressions

Arithmetic expressions are written in infix notation, using the following operators with the precedence:

||Operator|
|:-:|:-:|
|negative|**`-`**|
|multiplication|**`*`**|
|division| **`/`**, **`mod`**|
|addition|**`+`**|
|subtraction|**`-`**|
|relational| **`<`**, **`<=`**, **`<>`**, **`>=`**, **`>`**, **`=`** |
|logical|**`and`**, **`or`**, **`not`**|

Note that:

1. The token **"-"** can be either the **binary subtraction** operator, or the **unary negation** operator. Associativity is the left. Parentheses may be used to group subexpressions to dictate a different precedence. Valid components of an expression include **literal constants**, **variable reference**, **function invocations**

2. The part of semantic checking will be handled in the 4th assignment. In this assignment, you don't need to check semantic errors like **"a := 3 + true;"**. Just take care of syntactic errors!

3. Consecutive operators are **not** legal in an expression. (e.g. `--a` or `not not a`)

 - The **function invocation** has the form:

        identifier (<expression separated by zero or more comma>)

## Implementation Notes

Since `yyparse()` doesn't do its own lexical analysis. It needs to call the `yylex()` every time it wants to obtain a token from the input. Therefore, you should modify the scanner to pass token information to `yyparse()`. For example, when the scanner recognizes an identifier, the action should be revised as

    ([A-Za-z])([A-Za-z0-9])*		{ tokenString("id", yytext); return ID; }
    /* Note that the symbol `ID` is defined by the yacc parser */

Here is a general form of context-free grammar to describe grammar rule for `zero or more something` (e.g., a function signature contains zero or more arguments):

    argseq : /* empty */
           | argseq1
           ;

    argseq1 : arg
            | argseq1 ',' arg
            ;

## What Should Your Parser Do?

The parser should list information according to **Opt_S** and **Opt_T** options (the same as Project 1).  If the input file is syntactically correct, print

    |--------------------------------|
    |  There is no syntactic error!  |
    |--------------------------------|

Once the parser encounters a syntactic error, print an error message in the form of

    |--------------------------------------------------------------------------
    | Error found in Line #[ line number where the error occurs ]: [ source code of that line ]
    |
    | Unmatched token: [ the token that is not recognized ]
    |--------------------------------------------------------------------------

## Project Structure

+ README.md
+ /src
    + Makefile
    + **`scanner.l`**
    + **`parser.y`**
    + **`error.h`**
+ /report
    + **`READMD.md`**

In this project, you have to modify `src/scanner.l`, extend `src/parser.y` and write your report in `report/README.md`.

 > **Note**: You can extend your `scanner.l` from HW1 instead of using `scanner.l` we provided. However, you need to make sure your `scanner.l` works correctly.

The report should at least describe what changes you have to make to your scanner since the previous version you turned in, the abilities of your parser.

If you want to preview your report in GitHub style markdown before pushing to GitHub, [grip](https://github.com/joeyespo/grip) might be the tool you need.

## Build and Execute

+ Get HW2 docker image: `make docker-pull`
+ Activate docker environment: `./activate_docker.sh`
+ Build: `make`
+ Execute: `./parser [input file]`
+ Test: `make test`

### Build project

TA would use `src/Makefile` to build your project by simply typing `make clean && make`. You don't need to modify this file, but if you do, it is **your responsibility** to make sure this makefile have at least the same make targets we provided to you.

### Test your parser

We provide all the test cases in the `test` folder. Simply type `make test` to test your parser. The grade you got will be shown on the terminal. You can also check `diff.txt` in `test/result` folder to know the `diff` result between the outputs of your parser and the sample solutions.

Please using `student_` as the prefix of your own tests to prevent TAs from overriding your files. For example: `student_identifier_test`.

## Submitting the Assignment

You should push all your commits to the designated repository (hw2-\<Name of your GitHub account\>) under our GitHub organization by the deadline (given in the very beginning of this assignment description). At any point, you may save your work and push the repository. You **must** commit your final version to the **master branch**, and we will grade the commit which is **last pushed** on your master branch. The **push time** of that commit will be your submission time, so you **should not** push any commits to the master branch after the deadline if you have finished your assignment; otherwise, you will get a late penalty.

Note that the penalty for late homework is **15% per day** (weekends count as 1 day). Late homework will not be accepted after sample codes have been posted.

In addition, homework assignments **must be individual work**. If I detect what I consider to be intentional plagiarism in any assignment, the assignment will receive reduced or, usually, **zero credit**.
