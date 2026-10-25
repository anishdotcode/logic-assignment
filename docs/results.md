@page results Results and Testing

# Results and Testing

This page presents representative test cases for the propositional logic program. The examples describe expected behavior and can be compared against the actual console output and generated files.

## Test 1: Infix-to-Prefix Conversion

**Input formula:**

```text
((P+Q)*(~R))
```

**Expected prefix expression:**

```text
* + P Q ~ R
```

The binary operators are represented before their operands. Negation is a unary operator and precedes its operand.

## Test 2: Parse-Tree Construction and Traversal

For the prefix expression:

```text
* + P Q ~ R
```

the parse tree has `*` as the root. Its left subtree represents `P+Q`, and its right subtree represents `~R`.

The operand of the negation node is stored in its right child; its left child is empty.

**Expected traversal sequences:**

| Traversal | Expected sequence |
|---|---|
| Inorder, recursive | `P + Q * ~ R` |
| Inorder, iterative | `P + Q * ~ R` |
| Preorder, recursive | `* + P Q ~ R` |
| Preorder, iterative | `* + P Q ~ R` |
| Postorder, recursive | `P Q + R ~ *` |
| Postorder, iterative | `P Q + R ~ *` |

The recursive and iterative versions should produce matching sequences for the same tree.

The inorder result above lists nodes in left-root-right order. Parentheses are not included by these traversal sequences, so the output is a token sequence rather than a fully parenthesized expression.

## Test 3: Tree Height

Using the same parse tree, a longest path from the root to a leaf is:

```text
* -> + -> P
```

This path contains three nodes.

**Expected result:**

```text
Recursive height: 3
Iterative height: 3
```

Both implementations should return the same height.

## Test 4: Formula Evaluation

The example formula is:

\f$ (P \lor Q) \land \neg R \f$

Supply the following truth assignment:

| Atom | Value |
|---|---|
| P | True |
| Q | False |
| R | False |

The formula evaluates to:

\f$ (T \lor F) \land \neg F = T \land T = T \f$

**Expected formula truth value:**

```text
Formula truth value: True
```

This test checks the evaluation of OR, AND, and negation in a single formula.

## Test 5: Generated Truth Table

For the same formula, the truth table contains \(2^3=8\) rows because there are three distinct atoms.

The expected contents of `truth_table.txt`, apart from any differences in spacing, are:

```text
Truth Table (T = True, F = False)

P Q R Formula
T T T F
T T F T
T F T F
T F F T
F T T F
F T F T
F F T F
F F F F
```

The truth-table generator enumerates all possible assignments and evaluates the formula for each row. The output is saved to `truth_table.txt` in the program's current working directory.

## Test 6: DIMACS CNF Input

Use the following sample file, saved as `test.cnf`:

```text
c Test CNF formula
p cnf 3 2
1 2 -3 0
-2 3 0
```

This input declares three variables and two clauses.

The expected infix formula is equivalent to:

\f$ (x_1 \lor x_2 \lor \neg x_3) \land (\neg x_2 \lor x_3) \f$

The program should translate the DIMACS literals into atom names and logical operators, then pass the resulting formula through the same conversion and tree-processing workflow used for terminal input.

## Test 7: Truth-Table Limit

The truth-table generator supports at most ten distinct atoms.

| Number of distinct atoms | Expected behavior |
|---:|---|
| 3 | Generate 8 rows |
| 10 | Generate 1024 rows |
| 11 | Do not generate the complete truth table |

When the formula has more than ten distinct atoms, the program should report that the truth table was not generated. Evaluating the formula for a supplied truth assignment should remain available.

## Summary

These test cases cover expression conversion, tree construction, all six traversals, both height implementations, formula evaluation, truth-table output, and DIMACS input conversion.

For the final submission, verify each case by running the program and comparing the actual outputs with the expected results shown here. Include any additional cases needed to demonstrate error handling and the ten-atom limit.
