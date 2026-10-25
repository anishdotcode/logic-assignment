@page algorithm_analysis Algorithm Analysis

# Algorithm Analysis

This page analyses the algorithms implemented in the propositional logic program in terms of time complexity and auxiliary memory usage.

## Notation

The following symbols are used:

- \(n\): number of tokens in an expression or number of nodes in a parse tree.
- \(k\): number of distinct propositional atoms.
- \(h\): height of the parse tree, measured as the number of nodes on the longest root-to-leaf path.
- \(w\): maximum number of nodes at any single tree level.
- \(L\): size of the DIMACS CNF input file.

The analysis describes asymptotic worst-case complexity. For traversal functions, the returned vector containing the traversal sequence is included in the space discussion.

## Task 1: Infix-to-Prefix Conversion

The expression is tokenized, processed using an operator stack, and converted into prefix notation.

Each token is processed a constant number of times, and the stacks and token vectors store at most a linear number of elements.

- **Time complexity:** \(O(n)\)
- **Auxiliary space:** \(O(n)\)

The algorithm is efficient for expressions whose size grows linearly with the number of tokens.

## Task 2: Prefix-to-Tree Conversion

The prefix tokens are processed from right to left using a stack of subtree roots.

An operand creates a leaf node. A unary operator consumes one subtree, while a binary operator consumes two subtrees and creates a new parent node. At completion, a valid expression produces exactly one root.

- **Time complexity:** \(O(n)\)
- **Auxiliary space:** \(O(n)\)
- **Parse-tree storage:** \(O(n)\) nodes

The construction stack and resulting tree both require linear space in the worst case.

## Task 3: Tree Traversals

Each traversal visits every node exactly once.

| Traversal | Order | Time | Additional working space |
|---|---|---|---|
| Inorder, recursive | Left, root, right | \(O(n)\) | \(O(h)\) call stack |
| Inorder, iterative | Left, root, right | \(O(n)\) | \(O(n)\) worst case |
| Preorder, recursive | Root, left, right | \(O(n)\) | \(O(h)\) call stack |
| Preorder, iterative | Root, left, right | \(O(n)\) | \(O(n)\) worst case |
| Postorder, recursive | Left, right, root | \(O(n)\) | \(O(h)\) call stack |
| Postorder, iterative | Left, right, root | \(O(n)\) | \(O(n)\) worst case |

The recursive implementations rely on the function call stack. The iterative implementations use explicit stacks instead. The iterative postorder method uses two stacks to obtain the required left-right-root order.

All six functions return a vector containing the visited values, which requires an additional \(O(n)\) space. Therefore, total extra space, including the returned traversal sequence, is \(O(n)\) for each implementation.

## Task 4: Tree Height

The recursive method calculates the heights of the left and right subtrees, takes the maximum, and adds one for the current node.

The iterative method performs level-order traversal using a queue and increments the height after processing each level.

- **Recursive time complexity:** \(O(n)\)
- **Recursive auxiliary space:** \(O(h)\)
- **Iterative time complexity:** \(O(n)\)
- **Iterative auxiliary space:** \(O(w)\), where \(w\) is the maximum level width; worst case \(O(n)\)

The height of an empty tree is defined as zero. A tree containing only its root has height one.

## Task 5: Formula Evaluation and Truth-Table Generation

### Formula Evaluation

Formula evaluation recursively processes the parse tree. Negation reverses the truth value of its operand, OR returns true when at least one operand is true, AND requires both operands to be true, and implication is evaluated using:

\f[
P \rightarrow Q \equiv \neg P \lor Q
\f]

The implementation uses a `std::map` to look up assigned truth values. Each map lookup costs \(O(\log k)\) in the worst case.

- **Time complexity:** \(O(n\log k)\) worst case for \(k\) distinct atoms, using the map-based assignment.
- **Auxiliary space:** \(O(h+k)\), including the evaluation call stack and assignment map.

The formula is evaluated directly from the parse tree without constructing a new tree.

### Truth-Table Generation

For \(k\) distinct atoms, there are \(2^k\) possible assignments. The implementation enumerates these assignments, evaluates the formula for each one, and writes the results directly to a text file.

Truth-table generation is restricted to \f$ k \leq 10 \f$, giving at most 1024 rows.

- **Time complexity:** \f$ O(2^k(n \log k + k \log k)) \f$ worst case for the map-based implementation.
- **Working memory:** \f$ O(n + k + h) \f$, including atom collection, the assignment map, and evaluation stack, excluding the existing parse tree.
- **Output-file size:** \f$ O(2^k k) \f$ characters up to formatting and identifier-length factors.

The truth table is written incrementally, so the entire table does not need to be held in memory. Formula evaluation for an individual assignment remains available even when the number of distinct atoms exceeds ten.

## DIMACS CNF Conversion

The CNF parser reads the DIMACS header, processes literals grouped into clauses, and converts the clauses into a fully parenthesized infix expression.

Its parsing work scales with the input file size \(L\). Memory usage depends on the literals and generated formula retained during conversion. The cost of constructing strings also depends on the string-concatenation operations used in the implementation.

## Comparative Discussion

The recursive and iterative implementations of traversal and height computation have the same asymptotic time complexity. Their principal difference is how they manage pending work: recursive methods use function calls, while iterative methods use explicit data structures.

The parse tree requires linear storage, but it provides a common representation that can be reused by all subsequent algorithms. Truth-table generation is the most expensive task as the number of atoms grows because it evaluates the formula for every possible assignment. Restricting this operation to ten distinct atoms prevents excessive output and computation.

## Conclusion

The algorithms use standard stack, tree-traversal, and queue-based techniques. Most individual tree operations run in linear time. Complete truth-table generation grows exponentially with the number of distinct atoms, which motivates the implemented limit of ten atoms.
