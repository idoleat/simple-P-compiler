# P Language Compiler - RISC-V CourseProject
A s1mple `P` language compiler course project, compiling `P` to `RISC-V` `RV32` instructions.
* [Course website](https://compiler-s20.github.io/)
* [Discussion repo](https://github.com/compiler-s20/discussion/)
    * this course uses GitHub class room with awesome automation tool made by TAs.

## Project sturcture
The project is evoluated in 5 stages.
1. Implement lexical analyzer (scanner)
2. Implement syntax analyzer (parser) - Writing Syntactic Definitions
3. Implement syntax analyzer (parser) - Constructing Abstract Syntax Trees
4. Implement semantic analyzer
5. Implement Code Generator

### Lexical Analyzer
Write Regular expression with the tool `flex` to split codes into tokens. I follow the convention of the starter code provided by TAs creating some macros for later convenience. Then implement the regular expression rules.

#### Issues encountered
I found rules of some types like Octal, float and sciencetific can be easily messed together.

### Syntax Analyzer - Syntatic Definition
Parser takes tokens returned by scanner then parses them with grammar rules written by me with the tool `bison`, checking if there is any error. Be careful about the left/right associatin of some tokens. As teacher mentioned in the class, we are writing ambiguous grammar. With the help of TA's instruction, I can write grammars with less pain.

#### Issues encountered
* Token for keyword "begin" should not be named as BEGIN. That will direct bison to create the namespace collision. [[source]](https://bytes.com/topic/c/answers/861576-error-expected-identifier-before-token#post3456702)
* Not familiar with P languange. Need to check the syntax back and forth.
* Top down approach? Bottom up?
  * First I write grammar top down, but I found I can easily get confusing when I'm not sure what kind of base components(rules) I can get. So I tried to write grammars bottom up. Then I found I still need a top down overview of my grammar.
  * Basically I finish my grammar with both top down and bottom up approach. I spent a lot of time fooling around and write shit at the begining. After finding out a structural way to construct grammars, everything goes well.

### Syntax Analyzer - Abstract Syntax Tree
Add custom actions after grammers to construct nodes for AST. Suggested using visitor pattern to construct AST.

Mainly repeating these three steps:
* Give a non-terminal a type
* write actions for non-terminals
* Follow the AST Guild to implement nodes for actoins

#### Issues encountered
This stage stretches `c++` skills. It may make you battle-scarred, but worth it.

### Semantic Analyzer
Use the AST constructed in the last stage to detect semantic errors. Should print out detailed error messages and symbol table as well.

#### Issues encountered
* This stage stretches `c++` skills even more, especially some syntax from `c++11` or later. A great way to make me grow.
* Passing infomations between nodes could be complicated.
* Be careful about memeory management. Referencing the addresses has been released could cause segment faults that are hard to trace.

### Code Generation
Use the info stored in AST to generate `RISC-V` assembly codes. Need basic assembly code programming experience. Write generation code directly in AST could be fast, but not the best way, in terms of abstraton and maintenability.

## How to use
* Run the shell script at each HW folder root to enter container provided by TA.
* `$ make` to compile. `$ make test` to test

For more details please check the readme file in each folder. Each HW contains previous content.

## Dependencies
* `gcc`
* `flex`
* `bison`

TAs provide us docker image to work within docker. Regular Ubuntu with `flex` and `bison` installed could make it as well. I use Arch by the way.

## Notes
I only write code in `src` folder. Other are written either by TAs or provided by third party tools. For more details please check the readme file in each folder.

If you are NCTU/NYCU students and find this repo INTERESTING, please keep in mind that this is for knowledge and learning process sharing. You can ask question by opening issues. Please try your best on your own before finding something HELPFUL here.

Don't stop persuing greatness on whatever way you are heading.
