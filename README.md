# LogicMin
A logic minimizer that uses Quine-McCluskey.

#### Download repo:
```bash
git clone git@github.com:Doperob/LogicMin.git
cd LogicMin
```

#### Compile:
```bash
cd LogicMin
clang logicMin.c -o p
```
'p' being the program name, you can call it as you like.

## How to run the code
#### An example is:
```
your@Workstation LogicMin % ./p 3 abc 1 111 10_ 100 0_1
Your DMF is: aB + c
```
#### Parameters:

1. Number of variables of your problem
2. Names of all of these variables
3. '1' for DMF or '0' for KMF
4. Terms with 0 being negation, 1 being normal, and a space between the terms

If you don't specify all variables in a term, use '_' to replace the missing variable so that each term contains all variables.

#### Output is then:

Capital letters for negated variables and regular letters for normal variables.
