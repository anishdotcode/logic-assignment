# Propositional Logic Parse-Tree Algorithms

## Project Overview

This project implements algorithms for processing propositional logic formulas using C++23. The main objective is to represent a logical formula as a parse tree and perform operations such as expression conversion, tree traversal, height calculation, formula evaluation, and truth-table generation.

The program accepts a fully parenthesized infix formula from the terminal or reads a formula from a DIMACS CNF file. It converts the input into prefix notation, constructs a parse tree, and executes the required algorithms.

## Objectives

The project implements the following tasks:

1. **Infix to prefix conversion:** Convert a propositional logic expression from infix notation to prefix notation.
2. **Parse-tree construction:** Construct a rooted binary parse tree from the prefix expression.
3. **Tree traversal:** Implement inorder, preorder, and postorder traversals, both recursively and iteratively.
4. **Tree height:** Calculate the height of the parse tree using recursive and iterative approaches.
5. **Formula evaluation and truth tables:** Evaluate a formula for a supplied truth assignment and generate its complete truth table when the number of distinct atoms is at most ten.

## Supported Logical Operators

| Symbol | Meaning | Example |
|---|---|---|
| `~` | Negation | `~P` |
| `+` | Logical OR | `P+Q` |
| `*` | Logical AND | `P*Q` |
| `>` | Implication | `P>Q` |

The input formula is assumed to be well-formed and fully parenthesized. Propositional atoms can be identifiers such as `P`, `Q`, or `x1`.

## Parse-Tree Representation

Each tree node is represented by a `Node` structure containing a value and pointers to its left and right children.

Binary operators use both children. For unary negation (`~`), the left child is `nullptr` and the operand is stored in the right child. This convention is used consistently by tree construction, traversal, height calculation, and formula evaluation.

## Program Workflow

1. Read an infix formula from the terminal or convert a DIMACS CNF file into an infix formula.
2. Convert the infix formula into prefix notation.
3. Construct the parse tree from the prefix expression.
4. Execute all six tree traversals.
5. Calculate tree height recursively and iteratively.
6. Request truth values for the distinct atoms and evaluate the formula.
7. Generate `truth_table.txt` if the formula contains at most ten distinct atoms.

The truth-table limit is \(2^{10}=1024\) rows. Formula evaluation for a supplied assignment is not subject to this limit.

## Source Files

| File | Responsibility |
|---|---|
| `main.cpp` | User interaction, input selection, and displaying results |
| `logic.hpp` | Node structure and public function declarations |
| `logic.cpp` | Expression conversion, parse-tree algorithms, evaluation, and truth-table generation |
| `cnf.cpp` | DIMACS CNF input parsing and conversion into an infix formula |
| `Doxyfile` | Configuration for generating the documentation website |

## Compilation and Execution

Compile the project from its root directory using:

```bash
g++ -std=c++23 main.cpp logic.cpp cnf.cpp -o logic.exe
```

Run the resulting executable from the same directory:

```bash
./logic.exe
```

The exact run command may vary depending on the terminal being used. The generated truth table is saved as `truth_table.txt` in the program's current working directory.

## Documentation Navigation

The detailed complexity analysis is available on the [Algorithm Analysis](\ref algorithm_analysis) page. Example test cases and expected outputs are provided on the [Results and Testing](\ref results) page.

The C++ source documentation provides descriptions of the functions, their parameters, and their return values.
