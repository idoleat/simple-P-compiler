# hw1 report

|Field|Value|
|-:|:-|
|Name|簡志瑋|
|ID|0411275|

## How much time did you spend on this project

I can not count it precisely because I didn't work on the project continuously. I can only say it's roughly about 10~15hr. Actually most of the time I was fixing for advanced cases.

## Project overview

### Macro Defenition
Following the first few macros provided by TA, I define the same style macros such as "tokenInteger" and "tokenIdentifier". I created macros for almost every cases. It sounds stupid but they improves readability for me. I'm not allowed to write comments between regular expressions, so I use the macros calls to quickly find out what's the section for.

EDIT: Yes, you can write comments, but only C style comments are allowed.(/* comment */). Also, you can not write comments right at the begin of the line. Please write after a \t. Or that will cause "unrecognized rule" error on the next line.

### Variable, library and start conditions
Following some variables defined by TA, I added some variables to limit the max string length, storing string content, etc..  "string.h" is included for string operations. Also I defined some start conditions while dealing with more complex situations.

### Regular expressions
The main part of the project. The core of lexical analysis. Expressions for the same/similar cases are grouped together.

### Main function
I didn't modify main function.

## What is the hardest you think in this project

Some advanced cases are quite challenging. Octal, float and scientific can be easily messed together.

## Feedback to T.A.s

First time using GitHub Classroom. Feels good. Much better than E3. I also like test cases. I can test my program by myself and I don't need to write unit tests by myself. Quite convenient.
