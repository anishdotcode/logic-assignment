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
#include <map>
#include <stdexcept>

using namespace std;

void printTraversal(
    const string& label,
    const vector<string>& result
) {
    cout << label << ": ";

    for (const string& token : result) {
        cout << token << ' ';
    }
    cout << '\n';
}

bool readTruthValue(const string& atom) {
    string input;

    while (true) {
        cout << "Enter truth value for "
                  << atom << " (T/F or 1/0): ";

        if (!(cin >> input)) {
            throw runtime_error(
                "Failed to read truth value."
            );
        }

        if (input == "T" || input == "t" ||
            input == "1" || input == "true" ||
            input == "TRUE") {
            return true;
        }

        if (input == "F" || input == "f" ||
            input == "0" || input == "false" ||
            input == "FALSE") {
            return false;
        }

        cout << "Invalid input. Please enter T or F.\n";
    }
}

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
                // Task 3: Perform all six traversals.
                printTraversal("Inorder (recursive)",inorderRecursive(root));
                printTraversal("Inorder (iterative)",inorderIterative(root));
                printTraversal("Preorder (recursive)",preorderRecursive(root));
                printTraversal("Preorder (iterative)",preorderIterative(root));
                printTraversal("Postorder (recursive)",postorderRecursive(root));
                printTraversal("Postorder (iterative)",postorderIterative(root));
                // Task 4: Calculate tree height.
                cout << "\nTask 4: Tree Height\n";
                cout << "Recursive height: "<< heightRecursive(root) << '\n';
                cout << "Iterative height: "<< heightIterative(root) << '\n';
                // ==================== TASK 5 ====================
                cout << "\nTask 5: Formula Evaluation\n";
                vector<string> atoms = getAtoms(root);
                cout << "Distinct atoms: ";
                for (const string& atom : atoms) {
                    cout << atom << ' ';
                }
                cout << "\n";
                // Ask the user for one truth assignment.
                map<string, bool> assignment;

                for (const string& atom : atoms) {
                    assignment[atom] = readTruthValue(atom);
                }
                // Evaluate the formula for the supplied assignment.
                bool result = evaluateFormula(root, assignment);

                cout << "Formula truth value: "<< (result ? "True" : "False")<< '\n';
                // Generate the complete truth table only for <= 10 atoms.
                if (atoms.size() <= 10) {
                    try {
                        writeTruthTable(root, "truth_table.txt");
                        cout << "Truth table saved to truth_table.txt\n";
                    }
                    catch (const exception& e) {
                        cerr << "Truth-table error: "<< e.what() << '\n';
                    }
                }
                else {
                    cout << "Truth table not generated: "<< atoms.size()<< " distinct atoms found; maximum allowed is 10.\n";
                }
                // Free the tree only after all traversals are complete.
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
                    // Task 3: Perform all six traversals.
                    printTraversal("Inorder (recursive)",inorderRecursive(root));
                    printTraversal("Inorder (iterative)",inorderIterative(root));
                    printTraversal("Preorder (recursive)",preorderRecursive(root));
                    printTraversal("Preorder (iterative)",preorderIterative(root));
                    printTraversal("Postorder (recursive)",postorderRecursive(root));
                    printTraversal("Postorder (iterative)",postorderIterative(root));
                    // Task 4: Calculate tree height.
                    cout << "\nTask 4: Tree Height\n";
                    cout << "Recursive height: "<< heightRecursive(root) << '\n';
                    cout << "Iterative height: "<< heightIterative(root) << '\n';         
                    // ==================== TASK 5 ====================
                    cout << "\nTask 5: Formula Evaluation\n";
                    vector<string> atoms = getAtoms(root);
                    cout << "Distinct atoms: ";
                    for (const string& atom : atoms) {
                        cout << atom << ' ';
                    }
                    cout << "\n";
                    // Ask the user for one truth assignment.
                    map<string, bool> assignment;

                    for (const string& atom : atoms) {
                        assignment[atom] = readTruthValue(atom);
                    }

                    // Evaluate the formula for the supplied assignment.
                    bool result = evaluateFormula(root, assignment);

                    cout << "Formula truth value: "<< (result ? "True" : "False")<< '\n';
                    // Generate the complete truth table only for <= 10 atoms.
                    if (atoms.size() <= 10) {
                        try {
                            writeTruthTable(root, "truth_table.txt");
                            cout << "Truth table saved to truth_table.txt\n";
                        }
                        catch (const exception& e) {
                            cerr << "Truth-table error: "<< e.what() << '\n';
                        }
                    }
                    else {
                        cout << "Truth table not generated: "<< atoms.size()<< " distinct atoms found; maximum allowed is 10.\n";
                    }
                    // Free the tree only after all traversals are complete.
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