# Project Assignment 5 - Code Generation

**Introduction to Compiler Design by Prof. Yi-Ping You**

Due Date: **23:59, July 6, 2020**

Your assignment is to generate `RISC-V instructions` for the **`P`** language based on the `AST` and `symbol table` that you have built in the previous assignments. The generated code will then be executed on a RISC-V simulator, called `Spike`.

---

## Table of Contents

- [Project Assignment 5 - Code Generation](#project-assignment-5---code-generation)
  - [Assignment](#assignment)
  - [Generating RISC-V Instructions](#generating-risc-v-instructions)
    - [Initialization](#initialization)
    - [Declarations of Variables and Constants](#declarations-of-variables-and-constants)
    - [Expression](#expression)
    - [Function Declaration and Invocation](#function-declaration-and-invocation)
    - [Simple Statement](#simple-statement)
    - [Conditional Statement](#conditional-statement)
    - [For Statement and While Statement](#for-statement-and-while-statement)
    - [Combining All Examples Above](#combining-all-examples-above)
    - [Bonus](#bonus)
  - [Implementation Hints](#implementation-hints)
  - [Project Structure](#project-structure)
  - [Assessment Rubrics (Grading)](#assessment-rubrics-grading)
  - [Build and Execute](#build-and-execute)
  - [Submitting the Assignment](#submitting-the-assignment)

---

## Assignment     

In order to keep this assignment simple, only the `integer` type is needed to be implemented and the `array` type is not considered. Your assignment is to generate `RISC-V` instructions for a `P` program that contains any of the following constructs:

 - Global variable or local variable declaration.
 - Global constant or local constant declaration.
 - Function declaration.
 - Assign statement.
 - Simple statement.
 - Expressions with only `+` `-` (unary and binary) `*` `/` `mod` `function invocation` included.
 - Function invocation statement.
 - Conditional statement.
 - For statement and while statement.

The generated `RISC-V` instructions should be saved in a file with the same name as the input `P` file but with a `.S` extension. In addition, the file should be stored in a directory, which is set by the flag `--save_path [save path]`. For example, the following command translates `./test.p` into `./output_riscv_code/test/test.S`. 

```
./compiler test.p --save_path ./output_riscv_code/test
```

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

## Generating RISC-V Instructions                        

 > In the following subsections, we provide some examples of how to translate a `P` construct into RISC-V instructions. You may design your own instruction selection rules, as long as the generated code does what it should do. We recommend you read the [`RISC-V` tutorial](RISC-V-tutorial) before getting into the following subsections.

For all the examples below, we use a simple computation model called [**stack machine**](https://en.wikipedia.org/wiki/Stack_machine). 

 - When traversing to a `variable reference` node, push the **value** of the variable on the stack if it appears on the `RHS` of the `assignment` node, or push the **address** of the variable to the stack if it's on the `LHS` of the `assignment` node. 

 - When traversing to a `computation` node, (1) pop the values on the stack to some registers, and (2) then compute the result with one or more instructions, and (3) finally push the result back to the stack. 
 
 - When traversing to an `assignment` node, (1) pop the value and the address on the stack to some registers, and (2) then store the value to that address. 
 
 - For more precise steps, see [simple compilers](https://en.wikipedia.org/wiki/Stack_machine#Simple_compilers).

---

The generated `RISC-V` code will have the following structure:

```assembly
.section    .bss
    # uninitialized global variable(s)

.section    .rodata
    # global constant(s)

.section    .text
    # function

.section    .text
    # main function
```

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

### Initialization

 - An empty `P` program

    ```p
    // test1.p
    test1;
    begin
    end
    end
    ```

    will be translated into the following `RISC-V` instructions.

    ```assembly
        .file "test1.p"
        .option nopic
    .section    .text
        .align 2
        .globl main          # emit symbol 'main' to the global symbol table
        .type main, @function
    main:
        # in the function prologue
        addi sp, sp, -128    # move stack pointer to lower address to allocate a new stack
        sw ra, 124(sp)       # save return address of the caller function in the current stack
        sw s0, 120(sp)       # save frame pointer of the last stack in the current stack
        addi s0, sp, 128     # move frame pointer to the bottom of the current stack

        # in the function epilogue
        lw ra, 124(sp)       # load return address saved in the current stack
        lw s0, 120(sp)       # move frame pointer back to the bottom of the last stack
        addi sp, sp, 128     # move stack pointer back to the top of the last stack
        jr ra                # jump back to the caller function
        .size main, .-main 
    ```

A function `main` must be generated for the compound statement (program body) in the program node. 

You should allocate a local memory in the function prologue and clear the local memory in the function epilogue. In this assignment, allocate **128 bytes** of local memories is sufficient for the parameters and the local variables. However, in modern compilers, the size of the local memory depends on the demand of the function.

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

### Declarations of Variables and Constants

#### Global Variables

 - Declaring a global variable `a`

    ```p
    var a: integer;
    ```

    will be translated into the following `RISC-V` instructions.

    ```assembly
    .comm a, 4, 4    # emit object 'a' to .bss section with size = 4, align = 4
    ```

 - Assigning a value to a global variable `a`

    ```p
    a := 4;
    ```

    will be translated into the following `RISC-V` instructions.

    ```
    la t0, a         # load the address of variable 'a'
    addi sp, sp, -4
    sw t0, 0(sp)     # push the address to the stack
    li t0, 4         # load value '4' to register 't0'
    addi sp, sp, -4
    sw t0, 0(sp)     # push the value to the stack
    lw t0, 0(sp)     # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)     # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)     # save the value to the address of 'a'
    ```

#### Local Variables

 - Declaring a local variable
   + Each local integer variable occupies four bytes of the allocated local memory. For example, `fp-8` to `fp-12` for variable `b` and `fp-12` to `fp-16` for variable `c`. This information should be stored in the `symbol table`.

 - Assigning a value to a local variable `b` stored in `fp-8` to `fp-12` and a value to a local variable `c` stored in `fp-12` to `fp-16`.

    ```p
    var b, c: integer;
    b := 4;
    c := 6;
    ```

    will be translated into the following `RISC-V` instructions.

    ```assembly
    addi t0, s0, -12
    addi sp, sp, -4
    sw t0, 0(sp)     # push the address to the stack
    li t0, 4
    addi sp, sp, -4
    sw t0, 0(sp)     # push the value to the stack
    lw t0, 0(sp)     # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)     # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)     # b = 4
    addi t0, s0, -16
    addi sp, sp, -4
    sw t0, 0(sp)     # push the address to the stack   
    li t0, 6
    addi sp, sp, -4
    sw t0, 0(sp)     # push the value to the stack
    lw t0, 0(sp)     # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)     # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)     # c = 6
    ```

#### Global Constants

 - Declaring a global constant `d`

    ```p
    var d: 4;
    ```

    will be translated into the following `RISC-V` instructions.

    ```assembly
    .section    .rodata       # emit rodata section
        .align 2
        .globl d              # emit symbol 'd' to the global symbol table
        .type d, @object
    d:
        .word 4 
    ```

#### Local Constants

The same as local variables.

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

### Expression

 - Adding up local variable `b` and local variable `c`, then multiplying with global constant `d`, and finally assigning to global variable `a`

    ```p
    a := (b + c) * d;
    ```

    will be translated into the following `RISC-V` instructions.

    ```assembly
    addi sp, sp, -4
    la t0, a          # load the address of 'a'
    sw t0, 0(sp)      # push the address to the stack
    lw t0, -12(s0)    # load the value of 'b'
    addi sp, sp, -4  
    sw t0, 0(sp)      # push the value to the stack
    lw t0, -16(s0)    # load the value of 'c'
    addi sp, sp, -4
    sw t0, 0(sp)      # push the value to the stack
    lw t0, 0(sp)      # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)      # pop the value from the stack
    addi sp, sp, 4
    add t0, t1, t0    # b + c, always save the value in a certain register you choose
    addi sp, sp, -4
    sw t0, 0(sp)      # push the value to the stack
    la t0, d          # load the address of 'd'
    lw t1, 0(t0)      # load the 32-bit value of 'd'
    mv t0, t1
    addi sp, sp, -4
    sw t0, 0(sp)      # push the value to the stack
    lw t0, 0(sp)      # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)      # pop the value from the stack
    addi sp, sp, 4
    mul t0, t1, t0    # (b + c) * d, always save the value in a certain register you choose
    addi sp, sp, -4
    sw t0, 0(sp)      # push the value to the stack
    lw t0, 0(sp)      # pop the value from the stack
    addi sp, sp, 4
    la t1, 0(sp)      # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)      # save the value to 'a'
    ```

> The values on the registers may be polluted **after calling a function**, so you should take care of registers if you don't push the values on the registers to the stack every time and there's a function invocation in an expression. The simplest way is saving the registers on the stack in the function prologue and restoring them in the function epilogue.

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

### Function Declaration and Invocation

 - Declaring a function `sum`

    ```p
    sum(a,b: integer): integer
    begin
        var c: integer;
        c := a + b;
        return c;
    end
    end
    ```

    will be translated into the following `RISC-V` instructions.

    ```assembly
    .section    .text
        .align 2
        .globl sum
        .type sum, @function
    sum:
        addi sp, sp, -128
        sw ra, 124(sp)
        sw s0, 120(sp)
        addi s0, sp, 128
        sw a0, -12(s0)    # save parameter 'a' in the local stack
        sw a1, -16(s0)    # save parameter 'b' in the local stack
        addi t0, s0, -20  # load the address of 'c'
        addi sp, sp, -4
        sw t0, 0(sp)      # push the address to the stack
        lw t0, -12(s0)    # load the value of 'a'
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        lw t0, -16(s0)    # load the value of 'b'
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        lw t0, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        lw t1, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        add t0, t1, t0    # a + b, always save the value in a certain register you choose
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        lw t0, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        lw t1, 0(sp)      # pop the address from the stack
        addi sp, sp, 4
        sw t0, 0(t1)      # save the value to 'c'
        lw t0, -20(s0)
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        lw t0, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        mv a0, t0         # load the value of 'c' to the return value register 'a0'
        lw ra, 124(sp)
        lw s0, 120(sp)
        addi sp, sp, 128
        jr ra
        .size sum, .-sum
    ```

 - Call function `sum` with local variable `b` and global constant `d`, then assign the result to global variable `a`

    ```p
    a := sum(b, d);
    ```

    will be translated into the following `RISC-V` instructions.

    ```assembly
    la t0, a         # load the address of 'a'
    addi sp, sp, -4
    sw t0, 0(sp)     # push the value to the stack
    lw t0, -12(s0)   # load the value of 'b'
    addi sp, sp, -4
    sw t0, 0(sp)     # push the value to the stack
    la t0, d         # load the address of 'd'
    lw t1, 0(t0)     # load the 32-bit value of 'd'
    mv t0, t1
    addi sp, sp, -4
    sw t0, 0(sp)     # push the value to the stack
    lw a1, 0(sp)     # pop the value from the stack to the second argument register 'a1'
    addi sp, sp, 4
    lw a0, 0(sp)     # pop the value from the stack to the first argument register 'a0'
    addi sp, sp, 4
    jal ra, sum      # call function 'sum'
    mv t0, a0        # always move the return value to a certain register you choose
    addi sp, sp, -4
    sw t0, 0(sp)     # push the value to the stack
    lw t0, 0(sp)     # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)     # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)     # save the value to 'a'
    ```

> Note that the function argument number in the test case may be larger than **eight**, and there are only `a0`-`a7` registers. You should try to place the remain arguments in other places.

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

### Simple Statement

It's a little complicated to call an `IO` system call, so we provide you **print** functions and **read** functions in `io.c`. You can compile and link your generated code with the functions by:

```
riscv32-unknown-elf-gcc [generated RISC-V assembly file] io.c -o [output ELF file] 
``` 

 - Printing a global variable `a`

    ```p
    print a;
    ```

    will be translated into the following `RISC-V` instructions.

    ```assembly
    la t0, a
    lw t1, 0(t0)     # load the value of 'a'
    mv t0, t1
    addi sp, sp, -4
    sw t0, 0(sp)     # push the value to the stack
    lw a0, 0(sp)     # pop the value from the stack to the first argument register 'a0'
    addi sp, sp, 4
    jal ra, printInt # call function 'printInt'
    ```

 - Read a value and save to a global variable `a`

    ```p
    read a;
    ```

    will be translated into the following `RISC-V` instructions.

    ```assembly
    la t0, a         # load the address of 'a'
    addi sp, sp, -4 
    sw t0, 0(sp)     # push the address to the stack
    jal ra, readInt  # call function 'readInt'
    lw t0, 0(sp)     # pop the address from the stack
    addi sp, sp, 4
    sw a0, 0(t0)     # save the return value to 'a'
    ```

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

### Conditional Statement

 - Branching according to `a`'s value

    ```p
    if ( a <= 40 ) then  
    begin
        print a;
    end
    else
    begin
        print b;
    end
    end if
    ```

    will be translated into the following `RISC-V` instructions.

    ```assembly
        la t0, a
        lw t1, 0(t0)     # load the value of 'a'
        mv t0, t1
        addi sp, sp, -4
        sw t0, 0(sp)     # push the value to the stack
        li t0, 40
        addi sp, sp, -4
        sw t0, 0(sp)     # push the value to the stack
        lw t0, 0(sp)     # pop the value from the stack
        addi sp, sp, 4
        lw t1, 0(sp)     # pop the value from the stack
        addi sp, sp, 4
        ble t1, t0, L2   # if a <= 40, jump to L2
    L1:
        lw t0, -12(s0)   # load the value of 'b'
        addi sp, sp, -4
        sw t0, 0(sp)     # push the value to the stack
        lw a0, 0(sp)     # pop the value from the stack to the first argument register 'a0'
        addi sp, sp, 4
        jal ra, printInt # call function 'printInt'
        j L3             # jump to L3
    L2:
        la t0, a
        lw t1, 0(t0)     # load the value of 'a'
        mv t0, t1
        addi sp, sp, -4
        sw t0, 0(sp)     # push the value to the stack
        lw a0, 0(sp)     # pop the value from the stack to the first argument register 'a0'
        addi sp, sp, 4
        jal ra, printInt # call function 'printInt'
    L3:
    ```

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

### For Statement and While Statement

 - Looping until b >= 8

    ```p
    while b < 8 do
    begin
        print b;
        b := b + 1;
    end
    end do
    ```

    will be translated into the following `RISC-V` instructions.

    ```assembly
        j L5              # jump to L5
    L4:
        lw t0, -12(s0)    # load the value of 'b'
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        lw a0, 0(sp)      # pop the value from the stack to the first argument register 'a0'
        addi sp, sp, 4
        jal ra, printInt  # call function 'printInt'
        addi t0, s0, -12  # load the address of 'b'
        addi sp, sp, -4
        sw t0, 0(sp)      # push the address to the stack
        lw t0, -12(s0)    # load the value of 'b'
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        li t0, 1
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        lw t0, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        lw t1, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        add t0, t1, t0    # b + 1, always save the value in a certain register you choose
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack     
        lw t0, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        lw t1, 0(sp)      # pop the address from the stack
        addi sp, sp, 4
        sw t0, 0(t1)      # save the value to 'b' 
    L5: 
        lw t0, -12(s0)    # load the value of 'b' 
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        li t0, 8
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        lw t0, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        lw t1, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        blt t1, t0, L4    # if b < 8, jump to the loop content
    ```

 - Looping with loop variable `i`

    ```p
    for i := 10 to 13 do 
    begin
        print i;
    end
    end do
    ```

    will be translated into the following `RISC-V` instructions.

    ```assembly
        addi t0, s0, -20
        addi sp, sp, -4
        sw t0, 0(sp)      # push the address of the loop variable to the stack
        li t0, 10
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        lw t0, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        lw t1, 0(sp)      # pop the address from the stack
        addi sp, sp, 4
        sw t0, 0(t1)      # save the loop variable in the local stack
        j L7              # jump to L7
    L6:
        lw t0, -20(s0)    # load the value of 'i'
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        lw a0, 0(sp)      # pop the value from the stack to the first argument register 'a0'
        addi sp, sp, 4
        jal ra, printInt  # call function 'printInt'
        addi t0, s0, -20  # load the address of 'i'
        addi sp, sp, -4
        sw t0, 0(sp)      # push the address to the stack
        lw t0, -20(s0)    # load the value of 'i'
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        li t0, 1
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        lw t0, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        lw t1, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        add t0, t1, t0    # i + 1, always save the value in a certain register you choose
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack     
        lw t0, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        lw t1, 0(sp)      # pop the address from the stack
        addi sp, sp, 4
        sw t0, 0(t1)      # save the value to 'i' 
    L7:
        lw t0, -20(s0)    # load the value of 'i' 
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        li t0, 13
        addi sp, sp, -4
        sw t0, 0(sp)      # push the value to the stack
        lw t0, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        lw t1, 0(sp)      # pop the value from the stack
        addi sp, sp, 4
        blt t1, t0, L6    # if i < 13, jump to L6
    ```

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

### Combining All Examples Above

<details>
<summary>Click to expand!</summary>

```p
// test1.p
test1;

var a: integer;
var d: 4;

sum(a,b: integer): integer
begin
    var c: integer;
    c := a + b;
    return c;
end
end

begin

var b, c: integer;
b := 4;
c := 6;

read a;
print a;

a := sum(b, d);
print a;

a := (b + c) * d;
print a;

if ( a <= 40 ) then  
begin
    print a;
end
else
begin
    print b;
end
end if

while b < 8 do
begin
    print b;
    b := b + 1;
end
end do

for i := 10 to 13 do 
begin
    print i;
end
end do

end
end
```

will be translated into the following `RISC-V` instructions.

```assembly
    .file "test1.p"
    .option nopic 
.comm a, 4, 4             # emit object 'a' to .bss section with size = 4, align = 4
.section    .rodata       # emit rodata section
    .align 2
    .globl d              # emit symbol 'd' to the global symbol table
    .type d, @object
d:
    .word 4
.section    .text
    .align 2
    .globl sum
    .type sum, @function
sum:
    addi sp, sp, -128
    sw ra, 124(sp)
    sw s0, 120(sp)
    addi s0, sp, 128
    sw a0, -12(s0)        # save parameter 'a' in the local stack
    sw a1, -16(s0)        # save parameter 'b' in the local stack
    addi t0, s0, -20      # load the address of 'c'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the address to the stack
    lw t0, -12(s0)        # load the value of 'a'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, -16(s0)        # load the value of 'b'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    add t0, t1, t0        # a + b, always save the value in a certain register you choose
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)          # save the value to 'c'
    lw t0, -20(s0)
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    mv a0, t0             # load the value of 'c' to the return value register 'a0'
    lw ra, 124(sp)
    lw s0, 120(sp)
    addi sp, sp, 128
    jr ra
    .size sum, .-sum
.section    .text
    .align 2
    .globl main           # emit symbol 'main' to the global symbol table
    .type main, @function
main:
    # in the function prologue
    addi sp, sp, -128     # move stack pointer to lower address to allocate a new stack
    sw ra, 124(sp)        # save return address of the caller function in the current stack
    sw s0, 120(sp)        # save frame pointer of the last stack in the current stack
    addi s0, sp, 128      # move frame pointer to the bottom of the current stack
    # b = 4
    addi t0, s0, -12
    addi sp, sp, -4
    sw t0, 0(sp)          # push the address to the stack
    li t0, 4
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)          # b = 4
    # c = 6
    addi t0, s0, -16
    addi sp, sp, -4
    sw t0, 0(sp)          # push the address to the stack   
    li t0, 6
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)          # c = 6
    # read a
    la t0, a              # load the address of 'a'
    addi sp, sp, -4 
    sw t0, 0(sp)          # push the address to the stack
    jal ra, readInt       # call function 'readInt'
    lw t0, 0(sp)          # pop the address from the stack
    addi sp, sp, 4
    sw a0, 0(t0)          # save the return value to 'a'
    # print a
    la t0, a
    lw t1, 0(t0)          # load the value of 'a'
    mv t0, t1
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw a0, 0(sp)          # pop the value from the stack to the first argument register 'a0'
    addi sp, sp, 4
    jal ra, printInt      # call function 'printInt'
    # a = sum(b, d)
    la t0, a              # load the address of 'a'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, -12(s0)        # load the value of 'b'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    la t0, d              # load the address of 'd'
    lw t1, 0(t0)          # load the 32-bit value of 'd'
    mv t0, t1
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw a1, 0(sp)          # pop the value from the stack to the second argument register 'a1'
    addi sp, sp, 4
    lw a0, 0(sp)          # pop the value from the stack to the first argument register 'a0'
    addi sp, sp, 4
    jal ra, sum           # call function 'sum'
    mv t0, a0             # always move the return value to a certain register you choose
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)          # save the value to 'a'
    la t0, a
    lw a0, 0(t0)
    jal ra, printInt
    # a = (b + c) * d
    addi sp, sp, -4
    la t0, a              # load the address of 'a'
    sw t0, 0(sp)          # push the address to the stack
    lw t0, -12(s0)        # load the value of 'b'
    addi sp, sp, -4  
    sw t0, 0(sp)          # push the value to the stack
    lw t0, -16(s0)        # load the value of 'c'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    add t0, t1, t0        # b + c, always save the value in a certain register you choose
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    la t0, d              # load the address of 'd'
    lw t1, 0(t0)          # load the 32-bit value of 'd'
    mv t0, t1
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    mul t0, t1, t0        # (b + c) * d, always save the value in a certain register you choose
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)          # save the value to 'a'
    la t0, a
    lw a0, 0(t0)
    jal ra, printInt
    # condition example
    la t0, a    
    lw t1, 0(t0)          # load the value of 'a'
    mv t0, t1
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    li t0, 40
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    ble t1, t0, L2        # if a <= 40, jump to L2
L1:
    lw t0, -12(s0)        # load the value of 'b'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw a0, 0(sp)          # pop the value from the stack to the first argument register 'a0'
    addi sp, sp, 4
    jal ra, printInt      # call function 'printInt'
    j L3                  # jump to L3
L2:
    la t0, a
    lw t1, 0(t0)          # load the value of 'a'
    mv t0, t1
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw a0, 0(sp)          # pop the value from the stack to the first argument register 'a0'
    addi sp, sp, 4
    jal ra, printInt      # call function 'printInt'
L3:
    # while loop example
    j L5                  # jump to L5
L4:
    lw t0, -12(s0)        # load the value of 'b'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw a0, 0(sp)          # pop the value from the stack to the first argument register 'a0'
    addi sp, sp, 4
    jal ra, printInt      # call function 'printInt'
    addi t0, s0, -12      # load the address of 'b'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the address to the stack
    lw t0, -12(s0)        # load the value of 'b'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    li t0, 1
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    add t0, t1, t0        # b + 1, always save the value in a certain register you choose
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack     
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)          # save the value to 'b' 
L5: 
    lw t0, -12(s0)        # load the value of 'b' 
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    li t0, 8
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    blt t1, t0, L4        # if b < 8, jump to the loop content
    # for loop example
    addi t0, s0, -20
    addi sp, sp, -4
    sw t0, 0(sp)          # push the address of the loop variable to the stack
    li t0, 10
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4  
    lw t1, 0(sp)          # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)          # save the loop variable in the local stack
    j L7                  # jump to L7
L6:
    lw t0, -20(s0)        # load the value of 'i'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw a0, 0(sp)          # pop the value from the stack to the first argument register 'a0'
    addi sp, sp, 4
    jal ra, printInt      # call function 'printInt'
    addi t0, s0, -20      # load the address of 'i'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the address to the stack
    lw t0, -20(s0)        # load the value of 'i'
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    li t0, 1
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    add t0, t1, t0        # i + 1, always save the value in a certain register you choose
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack     
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the address from the stack
    addi sp, sp, 4
    sw t0, 0(t1)          # save the value to 'i' 
L7:
    lw t0, -20(s0)        # load the value of 'i' 
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    li t0, 13
    addi sp, sp, -4
    sw t0, 0(sp)          # push the value to the stack
    lw t0, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    lw t1, 0(sp)          # pop the value from the stack
    addi sp, sp, 4
    blt t1, t0, L6        # if i < 13, jump to L6
    # in the function epilogue
    lw ra, 124(sp)        # load return address saved in the current stack
    lw s0, 120(sp)        # move frame pointer back to the bottom of the last stack
    addi sp, sp, 128      # move stack pointer back to the top of the last stack
    jr ra                 # jump back to the caller function
    .size main, .-main
```

</details>

### Bonus

You need to generate `RISC-V` instructions for `boolean type`, `array type`, `string type`, and `real type` for the extra points. The code generation for additional types will be tested **with** declarations, statements and expressions. The following are some hints for the bonus.

#### Boolean Type
    
Just treat the `true` and `false` as `1` and `0`.

#### Array Type

For simplicity, you can pass the array variables by value.

#### String Type

There is no string concatenation in test cases, so you don't need to allocate dynamic memory for a string variable. 

 - Defining a local string variable 'st' will be translated into the following `RISC-V` instructions.

    ```
        .section	.rodata
        .align	2
    st:
        .string	"hello"
    ```

 - Referencing a local string variable 'st' will be translated into the following `RISC-V` instructions.

    ```
    lui	t0, %hi(st)
     addi a0, t0, %lo(st)
    ```

#### Real Type

You should use floating-point registers and floating-point instructions for real type code generation. Check Single-Precision Instructions in [RISC-V ISA Specification](https://riscv.org/specifications/isa-spec-pdf/).

 - Defining a local real type variable 'rv' will be translated into the following `RISC-V` instructions.

    ```
        .section	.rodata
        .align	2
    rv:
        .float	1.1
            .
            .
            .
    main:
        lui t0, %hi(rv)
        flw ft0, %lo(rv)(t0)
        fsw ft0, -24(s0)
    ```

 - Adding two real type variables will be translated into the following `RISC-V` instructions.

    ```
    flw	ft0, -24(s0)
    flw	ft1, -24(s0)
    fadd.s  ft0, ft1, ft0
    fsw	ft0, -24(s0)
    ```

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

## Implementation Hints

### Using `C` compiler that targets RISC-V

If you have no idea what instructions will be generated from a `P` program, you may write a corresponding `C` program and run the following command to generate `RISC-V` instructions using a `C` compiler. Check out `[output assembly file]` to see what instructions were generated.

```
riscv32-unknown-elf-gcc -c -S [input C file] -o [output assembly file]
```

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

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
        - /codegen
            - CodeGenerator.hpp - for code generation in visitor pattern version
    - /lib
        - /AST
        - /semantic
        - /visitor
        - /codegen
            - CodeGenerator.cpp - for code generation in visitor pattern version
    - Other modules you may add
- /report
    - `README.md`

In this assignment, you have to do the following tasks:

- Revise `CodeGenerator.hpp` and `CodeGenerator.cpp`, and add some modules to generate the `RISC-V` instructions.
- Write the report in `report/README.md`. The report should describe your feedback about `hw5` For example, is the spec and the tutorial clear? Is `hw5` too easy?

If you want to preview your report in GitHub style markdown before pushing to GitHub, [`grip`](https://github.com/joeyespo/grip) might be the tool you need.

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

## Assessment Rubrics (Grading)

Total of 119 points.

+ Passing all the basic cases (60 pts)
+ Passing all the advance cases (35 pts)
+ Report (5 pts)
+ Bonus (Total of 19 pts)
  + Code generation for boolean types (4 pts)
  + Code generation for array types (6 pts)
  + Code generation for string types (3 pts)
  + Code generation for real types (6 pts)

**Please note down what bonus you have done in your report.**

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

## Build and Execute

- Get Hw5 docker image: `make docker-pull`
- Activate docker environment: `./activate_docker.sh`
- Build: `make`
- Execute: `./compiler [input file] --save_path [save path]`
- Test: `make test`
- Test on board: `make board`

### Build Project

TA would use `src/Makefile` to build your project by simply typing `make clean && make`. You have to make sure that it will generate an executable named '`compiler`'. **No further grading will be made if the `make` process fails or the executable '`compiler`' is not found.**

### Test your compiler with the simulator

We provide all the test cases in the `test` folder. Simply type `make test` to test your compiler. The grade you got swill be shown on the terminal. You can also check `diff.txt` in `test/result` folder to know the diff result between the outputs of your compiler and the sample solutions.

Please use `student_` as the prefix of your own tests to prevent TAs from overriding your files. For example: `student_identifier_test`.

### Simulator Commands

The `RISC-V` simulator has been installed in the docker image. You may install it on your environment. The following commands show how to generate the executable and run the executable on the `RISC-V` simulator. 

 - Compile the generated `RISC-V` instructions to the `Executable and Linkable Format (ELF)` file.

```
riscv32-unknown-elf-gcc -o [output ELF file] [input RISC-V instruction file]
```

 - Run the `ELF` file on the simulator `spike`.

```
spike --isa=RV32 /risc-v/riscv32-unknown-elf/bin/pk [ELF file]
```

### Test your compiler with the RISC-V development board

> Note that the following test does not count for any points, and you can choose to implement it or not.

We provide the [`Seeed Studio Sipeed Longan Nano`](https://www.seeedstudio.com/Sipeed-Longan-Nano-RISC-V-GD32VF103CBT6-Development-Board-p-4205.html) development board for testing your compiler. Your compiler will be tested with the following tests:

1. Global variable
2. Local variable
3. Global constant 
4. Expression
5. Function definition and function invocation
6. Function invocation in an expression
7. Conditional statement
8. For statement
9. While statement

The `P` program which is going to be compiled by your compiler will call the functions written in `C` to test the correctness of your compiler, so all the above tests are based on the function declarations and the function invocations.

We use `USB DFU` download to download the compiled executable to the development board, so you should connect your board to your computer by USB TYPE-C cable first. To enter DFU mode, holding down the `BOOT` button, then press the `RESET` button to restart the development board and then release the `BOOT` button. After all, you can simply type `make board` to test your compiler with the board. The results will show on the LCD of the board.

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>

## Submitting the Assignment

You should push all your commits to the designated repository (hw5-\<Name of your GitHub account\>) under the compiler-f19 GitHub organization by the deadline (given in the very beginning of this assignment description).
At any point, you may save your work and push commits to your repository. You **must** commit your final version to the **master branch**, and we will grade the commit which is last pushed on your master branch. The **push time** of that commit will be your submission time, so you should not push any commits to the master branch after the deadline if you have finished your assignment; otherwise, you will get a late penalty.

**No late homework will be accepted!**

In addition, homework assignments **must be individual work**. If we detect what we consider to be intentional plagiarism in any assignment, the assignment will receive reduced or, usually, **zero credit**.

<div align="right">
    <b><a href="#table-of-contents">↥ back to menu</a></b>
</div>
