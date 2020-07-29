# xcalc
Calculator based on push-down automaton.

The idea here is to build a concrete syntax tree (a.k.a. parse tree) which is implemented as a binary tree so we can easily perform recursive binary operations for each binary expression. The recursion stops at the base case, that is, a typical numeric expression.

This is a complete rewrite and restructure from my automata exercise [pda-calc.go](https://github.com/michaelrk02/exercises/blob/master/automata/pda-calc.go). The distinction from the old one is that this calculator introduces one more arbitrary type of expression, that is, an unary expression, whereas the old one doesn't (though it uses the same "unary" term, its role remains a typical numeric expression). The unary expression is capable of negating any form of expression, unlike the old version, which uses the the binary expression (implemented as 0 minus \<expr>) as a substitute for an unary expression. That was actually a hack because I didn't want to mess up with so many type assertions =D

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

This program uses the help of push-down automata in order to perform syntax analysis.

Example program output:
```
[michael@archlinux build]$ ./xcalc
Enter mathematical expression (without spaces):
 * 15+5*3+7*(4+2)+(3-1)^2
 -> 76
Enter mathematical expression (without spaces):
 * 2*3^2+10*3+15
 -> 63
Enter mathematical expression (without spaces):
 * 10^2-5^2     
 -> 75
Enter mathematical expression (without spaces):
 * (10+5)*(10-5)
 -> 75
Enter mathematical expression (without spaces):
 * 10/(1+1)
 -> 5
Enter mathematical expression (without spaces):
 * 225^(1/2)
 -> 15
Enter mathematical expression (without spaces):
 * ^C
```

