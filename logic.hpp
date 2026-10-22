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

using namespace std;

struct Node {
    string value;
    Node* left;
    Node* right;

    explicit Node(const string& v)
        : value(v), left(nullptr), right(nullptr) {}
};

// Check whether a token is a logical operator.
bool isOperator(const string& token);

// Return the precedence of an operator.
int precedence(const string& op);

// Split an infix expression into tokens.
vector<string> tokenize(const string& expression);

// Convert a fully parenthesized infix expression to prefix.
string infixToPrefix(const string& expression);

// Read a DIMACS CNF file and convert it into an infix expression.
string dimacsToInfix(const string& filename);

Node* prefixToTree(const string& prefix);
void deleteTree(Node* root);

vector<string> inorderRecursive(Node* root);
vector<string> inorderIterative(Node* root);

vector<string> preorderRecursive(Node* root);
vector<string> preorderIterative(Node* root);

vector<string> postorderRecursive(Node* root);
vector<string> postorderIterative(Node* root);