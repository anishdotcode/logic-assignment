/**
 * @file cnf.cpp
 * @brief Implements parsing of DIMACS CNF input files.
 *
 * Reads the CNF header and clauses, converts literals into
 * propositional atoms or their negations, and combines the
 * clauses into a fully parenthesized infix formula.
 */
#include "logic.hpp"
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// Read a DIMACS CNF file and convert it into an infix expression.
string dimacsToInfix(const string& filename) {
    ifstream file(filename);

    if (!file) {
        throw runtime_error("Could not open CNF file: " + filename);
    }

    bool headerFound = false;
    long long numVariables = 0;
    long long numClauses = 0;

    vector<vector<long long>> clauses;
    vector<long long> currentClause;

    string line;
    size_t lineNumber = 0;

    // Read the file one line at a time.
    while (getline(file, line)) {
        ++lineNumber;

        size_t first = line.find_first_not_of(" \t\r\n");

        // Skip blank lines.
        if (first == string::npos) {
            continue;
        }

        // Skip comment lines.
        if (line[first] == 'c') {
            continue;
        }

        // Process the DIMACS header.
        if (line[first] == 'p') {
            if (headerFound || !clauses.empty() ||
                !currentClause.empty()) {
                throw runtime_error(
                    "Unexpected DIMACS header at line " +
                    to_string(lineNumber)
                );
            }

            char p;
            string format;
            string extra;

            istringstream header(line);

            if (!(header >> p >> format
                         >> numVariables >> numClauses) ||
                p != 'p' || format != "cnf" ||
                numVariables < 0 || numClauses < 0 ||
                (header >> extra)) {
                throw runtime_error(
                    "Invalid DIMACS header at line " +
                    to_string(lineNumber)
                );
            }

            headerFound = true;
            continue;
        }

        if (!headerFound) {
            throw runtime_error(
                "CNF clauses encountered before the header."
            );
        }

        // Read all integers on this line.
        istringstream row(line);
        long long literal;

        while (row >> literal) {
            if (literal == 0) {
                // Zero terminates the current clause.
                if (currentClause.empty()) {
                    throw runtime_error(
                        "Empty clauses are not supported by "
                        "the current formula representation."
                    );
                }

                clauses.push_back(move(currentClause));
                currentClause.clear();
            } else {
                // Prevent overflow when taking the absolute value.
                if (literal == numeric_limits<long long>::min()) {
                    throw runtime_error("Invalid literal value.");
                }

                long long variable =
                    (literal < 0) ? -literal : literal;

                if (variable > numVariables) {
                    throw runtime_error(
                        "Literal exceeds the variable count "
                        "declared in the DIMACS header."
                    );
                }

                currentClause.push_back(literal);
            }
        }

        // Detect non-integer data on a clause line.
        if (!row.eof()) {
            throw runtime_error(
                "Invalid literal data at line " +
                to_string(lineNumber)
            );
        }
    }

    if (!headerFound) {
        throw runtime_error("DIMACS header not found.");
    }

    // Every clause must end with zero.
    if (!currentClause.empty()) {
        throw runtime_error(
            "The final clause is missing its terminating zero."
        );
    }

    // Check the clause count against the header.
    if (static_cast<long long>(clauses.size()) != numClauses) {
        throw runtime_error(
            "Number of clauses does not match the DIMACS header."
        );
    }

    // The current logic syntax has no Boolean constants.
    if (clauses.empty()) {
        throw runtime_error(
            "A CNF formula with zero clauses cannot be represented "
            "using the current operators."
        );
    }

    // Convert one DIMACS literal into infix notation.
    auto literalToInfix = [](long long value) -> string {
        long long variable = (value < 0) ? -value : value;
        string atom = "x" + to_string(variable);

        if (value < 0) {
            return "(~" + atom + ")";
        }

        return atom;
    };

    // Convert every clause into a fully parenthesized OR expression.
    vector<string> clauseExpressions;
    clauseExpressions.reserve(clauses.size());

    for (const vector<long long>& clause : clauses) {
        string expression;

        // A clause with k literals needs k-1 pairs of parentheses
        // for a fully parenthesized, left-associated expression.
        for (size_t i = 1; i < clause.size(); ++i) {
            expression.push_back('(');
        }

        expression += literalToInfix(clause.front());

        for (size_t i = 1; i < clause.size(); ++i) {
            expression += '+';
            expression += literalToInfix(clause[i]);
            expression += ')';
        }

        clauseExpressions.push_back(move(expression));
    }

    // Combine all clauses using AND (*).
    string infix;

    // Add the opening parentheses for the conjunction.
    for (size_t i = 1; i < clauseExpressions.size(); ++i) {
        infix.push_back('(');
    }

    infix += clauseExpressions.front();

    for (size_t i = 1; i < clauseExpressions.size(); ++i) {
        infix += '*';
        infix += clauseExpressions[i];
        infix += ')';
    }

    return infix;
}