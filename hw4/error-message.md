# Semantic Error

**Table of Contents**

- [Semantic Error](#semantic-error)
	- [Overview](#overview)
	- [Symbol Table](#symbol-table)
	- [Variable Declaration](#variable-declaration)
	- [Variable Reference](#variable-reference)
	- [Binary/Unary Operator](#binaryunary-operator)
	- [Function Invocation](#function-invocation)
	- [Print and Read](#print-and-read)
	- [Assignment](#assignment)
	- [If and While](#if-and-while)
	- [For](#for)
	- [Return](#return)

## Overview

When your parser encounters a semantic error, it should report the error with corresponding error message defined below.

The format of a semantic error report:

```
<Error> Found in line x, column y: <error message>
    <source code>
    <notation>
```

- \<error message>: specific error message corresponding to the semantic error.
- \<source code>: source code of the semantic error. Indented for 4 spaces.
- \<notation>: character "``^``" which points to the position of the error.
	- **(x, y) should be the same as the notation position.**

## Symbol Table

- Symbol redeclaration

	**Output Format**

	```
	<Error> Found in line x, column y: symbol '{symbol_name}' is redeclared
	    <source code that the error happened at>
	    <notation that points to the position of the symbol name>
	```

	**Sample Output**

	```
	<Error> Found in line 34, column 13: symbol i is redeclared
	        var i : real;
	            ^
	```

## Variable Declaration

- Dimensions in the array declaration should be greater than 0

	**Output Format**

	```
	<Error> Found in line x, column y: '{symbol_name}' declared as an array with an index that is not greater than 0
	    <source code that the error happened at>
	    <notation that points to the position of the symbol name>
	```

	**Sample Output**

	```
	<Error> Found in line 7, column 9: 'arrError' declared as an array with an index that is not greater than 0
	    var arrError: array 10 of array 0 of integer;
	        ^
	```

## Variable Reference

- The identifier has to be in symbol tables

	**Output Format**

	```
	<Error> Found in line x, column y: use of undeclared symbol '{symbol_name}'
	    <source code that the error happened at>
	    <notation that points to the position of the symbol name>
	```

	**Sample Output**

	```
	<Error> Found in line 11, column 11: use of undeclared symbol 'arrr'
	    print arrr[30];
	          ^
	```

- The kind of symbol has to be a parameter, variable, loop_var, or constant

	**Output Format**

	```
	<Error> Found in line x, column y: use of non-variable symbol '{symbol_name}'
	    <source code that the error happened at>
	    <notation that points to the position of the symbol name>
	```

	**Sample Output**

	```
	<Error> Found in line 12, column 11: use of non-variable symbol 'func'
	    print func[30];
	          ^
	```

- Each index of an array reference must be of the integer type

	**Output Format**

	```
	<Error> Found in line x, column y: index of array reference must be an integer
	    <source code that the error happened at>
	    <notation that points to the position of the expression node that represents the wrong index>
	```

	**Sample Output**

	```
	<Error> Found in line 14, column 15: index of array reference must be an integer
	    print arr[1.0];
	              ^
	```

- An over array subscript

	**Output Format**

	```
	<Error> Found in line x, column y: there is an over array subscript on '{symbol_name}'
	    <source code that the error happened at>
	    <notation that points to the position of the symbol name>
	```

	**Sample Output**

	```
	<Error> Found in line 15, column 11: there is an over array subscript on 'a'
	    print a[40][50][60];
	          ^
	```

## Binary/Unary Operator

- Errors related to binary operator

	**Output Format**

	```
	<Error> Found in line x, column y: invalid operands to binary operator '{op_symbol}' ('{type_of_left_operand}' and '{type_of_right_operand}')
	    <source code that the error happened at>
	    <notation that points to the position of the binary operator>
	```

	**Sample Output**

	```
	<Error> Found in line 103, column 19: invalid operands to binary operator '>' ('string' and 'string')
	    print "SSLAB" > "SS";
	                  ^
	```

- Errors related to unary operator

	**Output Format**

	```
	<Error> Found in line x, column y: invalid operand to unary operator '{op_symbol}' ('{type_of_the_operand}')
	    <source code that the error happened at>
	    <notation that points to the position of the unary operator>
	```

	**Sample Output**

	```
	<Error> Found in line 10, column 11: invalid operand to unary operator 'not' ('boolean [10][100]')
	    print not arr;
	          ^
	```

## Function Invocation

- The identifier has to be in symbol tables.

	**Output Format**

	```
	<Error> Found in line x, column y: use of undeclared symbol '{symbol_name}'
	    <source code that the error happened at>
	    <notation that points to the position of symbol name>
	```

	**Sample Output**

	```
	<Error> Found in line 25, column 5: use of undeclared symbol 'null'
	    null(123, 456);
	    ^
	```

- The kind of symbol has to be function.

	**Output Format**

	```
	<Error> Found in line x, column y: call of non-function symbol '{symbol_name}'
	    <source code that the error happened at>
	    <notation that points to the position of the symbol name>
	```

	**Sample Output**

	```
	<Error> Found in line 28, column 5: call of non-function symbol 'arr'
	    arr(123, 456);
	    ^
	```

- The number of arguments must be the same as one of the parameters.

	**Output Format**

	```
	<Error> Found in line x, column y: too few/much arguments provided for function '{function_name}'
	    <source code that the error happened at>
	    <notation that points to the position of the function>
	```

	**Sample Output**

	```
	<Error> Found in line 31, column 5: too few/much arguments provided for function 'func'
	    func("overflow");
	    ^
	```

- The type of the result of the expression (argument) must be the same type of the corresponding parameter after appropriate type coercion.

	**Output Format**

	```
	<Error> Found in line x, column y: incompatible type passing '{type_of_argument}' to parameter of type '{type_of_parameter}'
	    <source code that the error happened at>
	    <notation that points to the position of the argument>
	```

	**Sample Output**

	```
	<Error> Found in line 40, column 16: incompatible type passing 'integer' to parameter of type 'string'
	    func2(123, 456);
	               ^
	```

## Print and Read

- The type of the expression (target) must be scalar type

	**Output Format**

	```
	<Error> Found in line x, column y: expression of print statement must be scalar type
	    <source code that the error happened at>
	    <notation that points to the position of the expression>
	```

	**Sample Output**

	```
	<Error> Found in line 9, column 11: expression of print statement must be scalar type
	    print arr;
	          ^
	```

- The type of the variable reference must be scalar type

	**Output Format**

	```
	<Error> Found in line x, column y: variable reference of read statement must be scalar type
	    <source code that the error happened at>
	    <notation that points to the position of the variable reference>
	```

	**Sample Output**

	```
	<Error> Found in line 7, column 10: variable reference of read statement must be scalar type
	    read arr[1];
	         ^
	```

- The kind of symbol of the variable reference cannot be constant or loop_var

	**Output Format**

	```
	<Error> Found in line x, column y: variable reference of read statement cannot be a constant or loop variable
	    <source code that the error happened at>
	    <notation that points to the position of the variable reference>
	```

	**Sample Output**

	```
	<Error> Found in line 9, column 10: variable reference of read statement cannot be a constant or loop variable
	    read constant;
	         ^
	```

## Assignment

- The type of the result of the variable reference cannot be an array type.

	**Output Format**

	```
	<Error> Found in line x, column y: array assignment is not allowed
	    <source code that the error happened at>
	    <notation that points to the position of the variable reference>
	```

	**Sample Output**

	```
	<Error> Found in line 12, column 5: array assignment is not allowed
	    arr := 1.23;
	    ^
	```

- The variable reference cannot be a reference to a constant variable

	**Output Format**

	```
	<Error> Found in line x, column y: cannot assign to variable '{symbol_name}' which is a constant
	    <source code that the error happened at>
	    <notation that points to the position of the variable reference>
	```

	**Sample Output**

	```
	<Error> Found in line 9, column 5: cannot assign to variable 'const' which is a constant
	    const := "SSLAB";
	    ^
	```

- The variable reference cannot be a reference to a loop variable when the context is within a loop body.

	**Output Format**

	```
	<Error> Found in line x, column y: the value of loop variable cannot be modified inside the loop body
	    <source code that the error happened at>
	    <notation that points to the position of the variable reference>
	```

	**Sample Output**

	```
	<Error> Found in line 23, column 9: the value of loop variable cannot be modified inside the loop body
	        i := 213;
	        ^
	```

- The type of the result of the expression cannot be an array type.

	**Output Format**

	```
	<Error> Found in line x, column y: array assignment is not allowed
	    <source code that the error happened at>
	    <notation that points to the position of the expression>
	```

	**Sample Output**

	```
	<Error> Found in line 14, column 14: array assignment is not allowed
	    float := arr[10];
	             ^
	```

- The type of the variable reference (lvalue) must be the same as the one of the expression after appropriate type coercion.

	**Output Format**

	```
	<Error> Found in line x, column y: assigning to '{type_of_variable_reference}' from incompatible type '{type_of_expression}'
	    <source code that the error happened at>
	    <notation that points to the position of the assignment>
	```

	**Sample Output**

	```
	<Error> Found in line 19, column 11: assigning to 'real' from incompatible type 'string'
	    float := "string";
	          ^
	```

## If and While

- The type of the result of the expression (condition) must be boolean type

	**Output Format**

	```
	<Error> Found in line x, column y: the expression of condition must be boolean type
	    <source code that the error happened at>
	    <notation that points to the position of the expression>
	```

	**Sample Output**

	```
	<Error> Found in line 7, column 12: the expression of condition must be boolean type
	    if int + 10 then
	           ^
	```

## For

- The initial value of the loop variable and the constant value of the condition must be in the incremental order

	**Output Format**

	```
	<Error> Found in line x, column y: the lower bound and upper bound of iteration count must be in the incremental order
	    <source code that the error happened at>
	    <notation that points to the position of the for>
	```

	**Sample Output**

	```
	<Error> Found in line 6, column 5: the lower bound and upper bound of iteration count must be in the incremental order
	    for idx := 10 to 1 do
	    ^
	```

## Return

- The current context shouldn't be in the program or a procedure since their return type is void.

	**Output Format**

	```
	<Error> Found in line x, column y: program/procedure should not return a value
	    <source code that the error happened at>
	    <notation that points to the position of the return>
	```

	**Sample Output**

	```
	<Error> Found in line 22, column 5: program/procedure should not return a value
	    return 10;
	    ^
	```

- The type of the result of the expression (return value) must be the same type as the return type of current function after appropriate type coercion.

	**Output Format**

	```
	<Error> Found in line x, column y: return '{type_of_expression}' from a function with return type '{return_type}'
	    <source code that the error happened at>
	    <notation that points to the position of the expression>
	```

	**Sample Output**

	```
	<Error> Found in line 10, column 12: return 'boolean [100]' from a function with return type 'boolean'
	    return bool[1];
	           ^
	```
