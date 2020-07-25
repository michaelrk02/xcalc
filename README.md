# xcalc
Calculator based on push-down automaton.

The idea here is to build a concrete syntax tree (a.k.a. parse tree) which is implemented as a binary tree so we can easily perform recursive binary operations for each binary expression. The recursion stops at the base case, that is, a typical numeric expression.

This is a complete rewrite and restructure from my automata exercise [pda-calc.go](https://github.com/michaelrk02/exercises/blob/master/automata/pda-calc.go). The distinction from the old one is that this calculator introduces one more arbitrary type of expression, that is, an unary expression, whereas the old one doesn't. The unary expression is capable of negating any form of expression, unlike the old version, which uses the the binary expression (implemented as 0 minus \<expr>) as a substitute for an unary expression. That was actually a hack because I didn't want to mess up with so many type assertions =D

For example, consider an expression: `A + B * (C + D) + E ^ F ^ (G - H)`. That will be translated to:
```
      ___+___
     /       \
    +         ^
   / \       / \
  A   *     E   ^
     / \       / \
    B   +     F   -
       / \       / \
      C   D     G   H

```

This program uses the help of push-down automata in order to perform lexical analysis as well as syntax analysis.
