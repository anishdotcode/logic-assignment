#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
#include "logic.hpp"
#include <exception>

using namespace std;

int main() {
    cout << "=== Propositional Logic Program ===\n";
    cout << "1. Input from CNF file\n";
    cout << "2. Input infix expression from terminal\n";
    cout << "Enter your choice: ";

    int choice;

    if (!(cin >> choice)) {
        cerr << "Invalid menu choice.\n";
        return 1;
    }
    // Remove the newline left behind by reading the choice.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (choice) {
        case 1: {
            string filename;

            cout << "Enter path to DIMACS CNF file: ";
            getline(cin, filename);

            try {
            // Convert the CNF file into an infix expression.
            string infix = dimacsToInfix(filename);

            // Reuse the Task 1 function.
            string prefix = infixToPrefix(infix);
            
            try {
                Node* root = prefixToTree(prefix);

                cout << "Parse tree constructed successfully.\n";
                cout << "Root node: " << root->value << '\n';

                deleteTree(root);
            }
            catch (const exception& e) {
                cerr << "Tree construction failed: "<< e.what() << '\n';
            }

            cout << "Prefix expression:\n";
            cout << prefix << '\n';
            }
            catch (const exception& e) {
            cerr << "Error: " << e.what() << '\n';
            return 1;
            }
    break;
}
        case 2: {
            string infix;
            cout << "Enter a fully parenthesized infix expression:\n";
            getline(cin, infix);
            try {
                string prefix = infixToPrefix(infix);
                cout << "Prefix expression: "<< prefix << '\n';
                try {
                    Node* root = prefixToTree(prefix);

                    cout << "Parse tree constructed successfully.\n";
                    cout << "Root node: " << root->value << '\n';

                    deleteTree(root);
                }
                catch (const exception& e) {
                    cerr << "Tree construction failed: "<< e.what() << '\n';
                }
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << '\n';
                return 1;
            }
            break;
        }
        default:
            cout << "Please choose option 1 or 2.\n";
    }

    return 0;
}