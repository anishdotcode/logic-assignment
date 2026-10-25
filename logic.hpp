/**
 * @file logic.hpp
 * @brief Public interface for propositional logic processing.
 *
 * Declares the parse-tree structure and functions for expression
 * conversion, tree traversal, height calculation, formula evaluation,
 * and truth-table generation.
 */
#pragma once

#include <algorithm>
#include <cctype>
#include <sstream>
#include <iostream>
#include <limits>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace std;

/**
 * @struct Node
 * @brief Represents a node in a propositional logic parse tree.
 *
 * Operators and propositional atoms are stored in value.
 * Binary operators use both children. For negation (~),
 * the operand is stored in the right child.
 */
struct Node {
    std::string value; ///< Operator or propositional atom.
    Node* left;        ///< Left child; nullptr for negation nodes.
    Node* right;       ///< Right child; operand of negation is stored here.

    /**
     * @brief Constructs a node with the given value.
     * @param v Operator or propositional atom to store.
     */
    explicit Node(const std::string& v)
        : value(v), left(nullptr), right(nullptr) {}
};

// Check whether a token is a logical operator.
bool isOperator(const string& token);

// Return the precedence of an operator.
int precedence(const string& op);

// Split an infix expression into tokens.
vector<string> tokenize(const string& expression);

// Convert a fully parenthesized infix expression to prefix.
/**
 * @brief Converts a fully parenthesized infix expression to prefix.
 * @param expression Infix propositional logic expression.
 * @return Prefix expression as a space-separated string.
 */
string infixToPrefix(const string& expression);

// Read a DIMACS CNF file and convert it into an infix expression.
/**
 * @brief Converts a DIMACS CNF file into a propositional formula.
 * @param filename Path to the input CNF file.
 * @return Fully parenthesized infix expression.
 * @throws std::runtime_error or std::invalid_argument if the file
 *         cannot be read or its contents are invalid.
 */
string dimacsToInfix(const string& filename);

/**
 * @brief Constructs a parse tree from a prefix expression.
 * @param prefix Space-separated prefix expression.
 * @return Pointer to the root of the constructed tree.
 * @throws std::invalid_argument if the expression is invalid.
 *
 * The caller is responsible for freeing the tree using deleteTree().
 */
Node* prefixToTree(const string& prefix);
/**
 * @brief Frees a parse tree and all its descendants.
 * @param root Root of the tree to delete; nullptr is permitted.
 */
void deleteTree(Node* root);

/**
 * @brief Performs recursive inorder traversal.
 * @param root Root of the parse tree.
 * @return Node values in left-root-right order.
 */
vector<string> inorderRecursive(Node* root);
/**
 * @brief Performs iterative inorder traversal using a stack.
 * @param root Root of the parse tree.
 * @return Node values in left-root-right order.
 */
vector<string> inorderIterative(Node* root);

/**
 * @brief Performs recursive preorder traversal.
 * @param root Root of the parse tree.
 * @return Node values in root-left-right order.
 */
vector<string> preorderRecursive(Node* root);
/**
 * @brief Performs iterative preorder traversal using a stack.
 * @param root Root of the parse tree.
 * @return Node values in root-left-right order.
 */
vector<string> preorderIterative(Node* root);

/**
 * @brief Performs recursive postorder traversal.
 * @param root Root of the parse tree.
 * @return Node values in left-right-root order.
 */
vector<string> postorderRecursive(Node* root);
/**
 * @brief Performs iterative postorder traversal using stacks.
 * @param root Root of the parse tree.
 * @return Node values in left-right-root order.
 */
vector<string> postorderIterative(Node* root);

// Task 4: Tree height
/**
 * @brief Computes the height of a parse tree recursively.
 * @param root Root of the parse tree.
 * @return Number of nodes on the longest root-to-leaf path.
 *
 * The height of an empty tree is zero.
 */
int heightRecursive(Node* root);
/**
 * @brief Computes the height of a parse tree iteratively.
 * @param root Root of the parse tree.
 * @return Number of nodes on the longest root-to-leaf path.
 *
 * Uses level-order traversal. The height of an empty tree is zero.
 */
int heightIterative(Node* root);

// Task 5: Formula evaluation and truth table
/**
 * @brief Collects all distinct propositional atoms in a tree.
 * @param root Root of the parse tree.
 * @return Distinct atom names in sorted order.
 */
vector<string> getAtoms(const Node* root);

/**
 * @brief Evaluates a formula for a given truth assignment.
 * @param root Root of the parse tree.
 * @param values Mapping of atom names to Boolean truth values.
 * @return True if the formula evaluates to true; otherwise false.
 * @throws std::invalid_argument if the tree is invalid or an atom
 *         has no assigned truth value.
 */
bool evaluateFormula(
    const Node* root,
    const map<string, bool>& values
);

/**
 * @brief Generates the complete truth table in a text file.
 * @param root Root of the parse tree.
 * @param filename Path of the output text file.
 * @throws std::length_error if the formula has more than 10 atoms.
 * @throws std::runtime_error if the output file cannot be written.
 *
 * Generates 2^k assignments for k distinct atoms, provided k <= 10.
 */
void writeTruthTable(
    const Node* root,
    const string& filename
);