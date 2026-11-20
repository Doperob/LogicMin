# LogicMin
A logic minimizer that uses Quine McCluskey.

#### Downlaod repo:
```bash
git clone git@github.com:Doperob/LogicMin.git
cd LogicMin
```

#### Compile:
```bash
cd LogicMin
clang logicMin.c -o p
```
'p' being the programm name you can call it as you like

## How to run the code
#### An example is:
```
your@Workstation LogicMin % ./p 3 abc 1 111 10_ 100 0_1
Your DMF is: aB + c
```
#### Parameters:

1. number of variables of your problem
2. names of all of these variables
3. '1' for DMF or '0' for KMF
4. terms with 0 being negation and 1 being normal and a space between the terms

if you don't specify all variables in a term use '_' to replace the missing variable so that each term contains all variables

#### Output is then:

Capital letters for negated variables and regular for normal variables.