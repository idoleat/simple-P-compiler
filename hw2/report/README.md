# hw2 report

|item|content|
|:-:|:-:|
|Name|簡志瑋|
|ID|0411275|

## How much time did you spend on this project

About 17 hr in 4 days. Maybe more. Because I got confusing writing grammars at first.

## Project overview

### Token return in Lex
In last project, scanner only scans and prints out the result in terminal. In this project, I added the return value of tokens in rule actions, thanks to TA's starter code, parser can easily get tokens.

### Token declarations in Yacc
Tokens in yacc are mapped in this section. Note that the higher precedence level tokens should be placed lower. It can help you bring down the use of %prec in grammar (if you don't like %prec). Also left associative tokes should use %left.

### Grammar rules
As teacher mentioned in the class, we are writing ambiguous grammar. With the help of TA's instruction, I can write grammars with less pain.


## Issues encountered

* Token for keyword "begin" should not be named as BEGIN. That will direct bison to create the namespace collision. [[source]](https://bytes.com/topic/c/answers/861576-error-expected-identifier-before-token#post3456702)
* Not familiar with P languange. Need to check the syntax back and forth.
* Top down approach? Bottom up?
    * First I write grammar top down, but I found I can easily get confusing when I'm not sure what kind of base components(rules) I can get. So I tried to write grammars bottom up. Then I found I still need a top down overview of my grammar.
    * Basically I finish my grammar with both top down and bottom up approach. I spent a lot of time fooling around and write shit at the begining. After finding out a structural way to construct grammars, everything goes well.
* TA has provided a pattern of writing <zero or more>. I found that is quite useful. Not only that, I found something repeats over time, so I took one way as a pattern and applied to all. That makes debuggin painless.

## Feedback to T.A.s

Since I have to check P syntax frequently, I found README hard to navigate. So I changed it a little bit in [this commit](https://github.com/compiler-s20/hw2-idoleat/commit/013e774c1c146cd28e66e7430a7c704317d00db5) for better experience. Collapsing those details makes me feel better.
