#include <algorithm>
#include <cctype>
#include <sstream>
#include <iostream>
#include <limits>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
#include <stack>
#include "logic.hpp"

using namespace std;

// Check whether a token is a logical operator.
bool isOperator(const string& token) {
    return token == "~" || token == "+" ||
           token == "*" || token == ">";
}

// Return the precedence of an operator.
int precedence(const string& op) {
    if (op == "~") return 4;
    if (op == "*") return 3;
    if (op == "+") return 2;
    if (op == ">") return 1;
    throw invalid_argument("Unknown operator: " + op);
}

// Split an infix expression into tokens.
vector<string> tokenize(const string& expression) {
    vector<string> tokens;
    size_t i = 0;
    while (i < expression.size()) {
        //typecast into unsigned char
        unsigned char ch = static_cast<unsigned char>(expression[i]);
        // Ignore whitespace.
        if (isspace(ch)) {
            ++i;
            continue;
        }
        // Recognize operators and parentheses.
        if (string("()+*~>").find(expression[i])!= string::npos) {
            tokens.push_back(string(1, expression[i]));
            ++i;
            continue;
        }
        // Recognize propositional atoms such as P, Q, x1, x12.
        if (isalpha(ch) || ch == '_') {
            size_t start = i;
            while (i < expression.size()) {
                unsigned char current =
                    static_cast<unsigned char>(expression[i]);

                if (isalnum(current) || current == '_') {
                    ++i;
                } else {
                    break;
                }
            }
            tokens.push_back(
                expression.substr(start, i - start)
            );
            continue;
        }
        throw invalid_argument(
            "Invalid character in expression."
        );
    }
    if (tokens.empty()) {
        throw invalid_argument("The expression is empty.");
    }
    return tokens;
}

// Convert a fully parenthesized infix expression to prefix.
string infixToPrefix(const string& expression) {
    vector<string> tokens = tokenize(expression);
    // Step 1: Reverse the tokens.
    reverse(tokens.begin(), tokens.end());
    // Step 2: Swap opening and closing parentheses.
    for (string& token : tokens) {
        if (token == "(") {
            token = ")";
        } else if (token == ")") {
            token = "(";
        }
    }
    // Step 3: Convert the modified expression to postfix.
    vector<string> postfix;
    stack<string> operators;
    for (const string& token : tokens) {

        // An operand goes directly to the output.
        if (!isOperator(token) && token != "(" && token != ")") {
            postfix.push_back(token);
        }
        // Opening parenthesis goes onto the stack.
        else if (token == "(") {
            operators.push(token);
        }
        // Pop operators until the matching opening parenthesis.
        else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                postfix.push_back(operators.top());
                operators.pop();
            }
            if (operators.empty()) {
                throw invalid_argument(
                    "Mismatched parentheses."
                );
            }
            operators.pop();
        }
        // Process logical operators.
        else {
            // Use strict greater-than because the original
            // expression has been reversed.
            while (!operators.empty() && operators.top() != "(" && precedence(operators.top()) > precedence(token)) {
                postfix.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
    }
    // Pop any remaining operators.
    while (!operators.empty()) {
        if (operators.top() == "(" ||
            operators.top() == ")") {
            throw invalid_argument(
                "Mismatched parentheses."
            );
        }
        postfix.push_back(operators.top());
        operators.pop();
    }
    // Step 4: Reverse the postfix tokens to obtain prefix.
    reverse(postfix.begin(), postfix.end());
    // Step 5: Join the tokens with spaces.
    string prefix;
    for (const string& token : postfix) {
        if (!prefix.empty()) {
            prefix += ' ';
        }
        prefix += token;
    }
    return prefix;
}
//task 2
namespace {
    bool isValidAtomToken(const string& token) {
        if (token.empty()) {
            return false;
        }
        return all_of(
            token.begin(), token.end(),
            [](unsigned char ch) {
                return isalnum(ch) || ch == '_';
            }
        );
    }
}
//prefix to rooted binary parse tree
Node* prefixToTree(const string& prefix) {
    // Split the prefix expression into tokens.
    istringstream input(prefix);
    vector<string> tokens;
    string token;

    while (input >> token) {
        tokens.push_back(token);
    }
    if (tokens.empty()) {
        throw invalid_argument("Prefix expression is empty.");
    }
    // Stack stores roots of subtrees.
    vector<Node*> st;
    try {
        // Process tokens from right to left.
        for (auto it = tokens.rbegin(); it != tokens.rend(); ++it) {
            token = *it;
            if (token == "~") {
                // Unary operator: requires one operand.
                if (st.empty()) {
                    throw invalid_argument(
                        "Invalid prefix expression: missing operand for ~."
                    );
                }
                Node* child = st.back();
                st.pop_back();
                Node* node = new Node(token);
                node->right = child;

                st.push_back(node);
            }
            else if (token == "+" || token == "*" || token == ">") {
                // Binary operator: requires two operands/subtrees.
                if (st.size() < 2) {
                    throw invalid_argument(
                        "Invalid prefix expression: missing operands."
                    );
                }
                // IMPORTANT: first popped becomes LEFT child.
                Node* leftChild = st.back();
                st.pop_back();
                Node* rightChild = st.back();
                st.pop_back();

                Node* node = new Node(token);
                node->left = leftChild;
                node->right = rightChild;

                st.push_back(node);
            }
            else {
                // Operand: create a leaf node.
                if (!isValidAtomToken(token)) {
                    throw invalid_argument(
                        "Invalid token in prefix expression: " + token
                    );
                }
                st.push_back(new Node(token));
            }
        }
        // A valid expression must produce exactly one root.
        if (st.size() != 1) {
            throw invalid_argument(
                "Invalid prefix expression: multiple roots remain."
            );
        }
        return st.back();
    }
    catch (...) {
        // Clean up subtrees if construction fails.
        for (Node* node : st) {
            deleteTree(node);
        }
        throw;
    }
}

void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
//task 3
namespace {

void inorderRec(const Node* root, vector<string>& result) {
    if (root == nullptr) {
        return;
    }

    inorderRec(root->left, result);
    result.push_back(root->value);
    inorderRec(root->right, result);
}

void preorderRec(const Node* root, vector<string>& result) {
    if (root == nullptr) {
        return;
    }

    result.push_back(root->value);
    preorderRec(root->left, result);
    preorderRec(root->right, result);
}

void postorderRec(const Node* root, vector<string>& result) {
    if (root == nullptr) {
        return;
    }

    postorderRec(root->left, result);
    postorderRec(root->right, result);
    result.push_back(root->value);
}

} // anonymous namespace


vector<string> inorderRecursive(Node* root) {
    vector<string> result;
    inorderRec(root, result);
    return result;
}

vector<string> preorderRecursive(Node* root) {
    vector<string> result;
    preorderRec(root, result);
    return result;
}

vector<string> postorderRecursive(Node* root) {
    vector<string> result;
    postorderRec(root, result);
    return result;
}


// ---------- Iterative traversals ----------

vector<string> inorderIterative(Node* root) {
    vector<string> result;
    stack<Node*> st;
    Node* current = root;

    while (current != nullptr || !st.empty()) {
        // Go as far left as possible.
        while (current != nullptr) {
            st.push(current);
            current = current->left;
        }

        // Visit the top node.
        current = st.top();
        st.pop();

        result.push_back(current->value);

        // Process its right subtree.
        current = current->right;
    }

    return result;
}


vector<string> preorderIterative(Node* root) {
    vector<string> result;

    if (root == nullptr) {
        return result;
    }

    stack<Node*> st;
    st.push(root);

    while (!st.empty()) {
        Node* current = st.top();
        st.pop();

        result.push_back(current->value);

        // Push right first so that left is processed first.
        if (current->right != nullptr) {
            st.push(current->right);
        }

        if (current->left != nullptr) {
            st.push(current->left);
        }
    }

    return result;
}


vector<string> postorderIterative(Node* root) {

    vector<string> result;

    if (root == nullptr) {
        return result;
    }

    stack<Node*> first;
    stack<Node*> second;

    first.push(root);

    while (!first.empty()) {
        Node* current = first.top();
        first.pop();

        second.push(current);

        if (current->left != nullptr) {
            first.push(current->left);
        }

        if (current->right != nullptr) {
            first.push(current->right);
        }
    }

    // Reversing the processing order gives left-right-root.
    while (!second.empty()) {
        result.push_back(second.top()->value);
        second.pop();
    }

    return result;
}

