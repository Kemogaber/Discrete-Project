/****************************************************/
/**********      Yousif Hazim Nazar       ***********/
/***************     2001741       ******************/
/****   Digital Logic Circuit Simplification     ****/
/****************************************************/

#include <iostream>
#include <string>
#include <stack>
#include <cctype>

using namespace std;

// All possible input
bool truthTable[8][3] = {
    {0, 0, 0},
    {0, 0, 1},
    {0, 1, 0},
    {0, 1, 1},
    {1, 0, 0},
    {1, 0, 1},
    {1, 1, 0},
    {1, 1, 1}
};

// Substitute variables in the expression
void substituteVariables(string &expression, bool A, bool B, bool C) {
    for (char &ch : expression) {
        if (ch == 'A') {
            ch = A ? '1' : '0';
        } else if (ch == 'B') {
            ch = B ? '1' : '0';
        } else if (ch == 'C') {
            ch = C ? '1' : '0';
        }
    }
}

char evaluateOperation(char operand1, char operand2, char operation) {
    if (operation == '&') {
        return (operand1 == '1' && operand2 == '1') ? '1' : '0';
    } else if (operation == '|') {
        return (operand1 == '1' || operand2 == '1') ? '1' : '0';
    }
    return '0';
}

bool charToBool(char ch) {
    return ch == '1';
}


char boolToChar(bool b) {
    return b ? '1' : '0';
}

char NOT(char operand) {
    return operand == '1' ? '0' : '1';
}

bool evaluateBooleanExpression(const string &expression) {
    stack<char> operators;
    stack<char> operands;

    for (char ch : expression) {
        if (isspace(ch)) continue;

        if (ch == '0' || ch == '1') {
            operands.push(ch);
        } else if (ch == '(') {
            operators.push(ch);
        } else if (ch == ')') {

            while (!operators.empty() && operators.top() != '(') {
                char operation = operators.top();
                operators.pop();

                if (operation == '!') {
                    char operand = operands.top();
                    operands.pop();
                    operands.push(NOT(operand));
                } else {
                    char operand2 = operands.top();
                    operands.pop();
                    char operand1 = operands.top();
                    operands.pop();
                    operands.push(evaluateOperation(operand1, operand2, operation));
                }
            }
            operators.pop();
        } else if (ch == '&' || ch == '|' || ch == '!') {

            while (!operators.empty() && (operators.top() == '!' ||
                   (operators.top() == '&' && (ch == '&' || ch == '|')) ||
                   (operators.top() == '|' && ch == '|'))) {
                char operation = operators.top();
                operators.pop();

                if (operation == '!') {
                    char operand = operands.top();
                    operands.pop();
                    operands.push(NOT(operand));
                } else {
                    char operand2 = operands.top();
                    operands.pop();
                    char operand1 = operands.top();
                    operands.pop();
                    operands.push(evaluateOperation(operand1, operand2, operation));
                }
            }
            operators.push(ch);
        }
    }


    while (!operators.empty()) {
        char operation = operators.top();
        operators.pop();

        if (operation == '!') {
            char operand = operands.top();
            operands.pop();
            operands.push(NOT(operand));
        } else {
            char operand2 = operands.top();
            operands.pop();
            char operand1 = operands.top();
            operands.pop();
            operands.push(evaluateOperation(operand1, operand2, operation));
        }
    }


    return operands.top() == '1';
}

// Print the truth table for both
void printTruthTable(bool circuitOutput[8], string circuitName) {
    cout << "Truth Table for " << circuitName << ":\n";
    for (int i = 0; i < 8; ++i) {
        cout << truthTable[i][0] << " " << truthTable[i][1] << " " << truthTable[i][2]
             << " : " << circuitOutput[i] << "\n";
    }
}

// Check if the expressions are equivalent
void checkEquivalence(string originalExpr, string simplifiedExpr) {
    bool A, B, C;
    bool originalOutput[8], simplifiedOutput[8];
    bool areEquivalent = true;

    for (int i = 0; i < 8; ++i) {
        A = truthTable[i][0];
        B = truthTable[i][1];
        C = truthTable[i][2];
        string temp1 = originalExpr, temp2 = simplifiedExpr;
        substituteVariables(temp1, A, B, C);
        substituteVariables(temp2, A, B, C);

        originalOutput[i] = evaluateBooleanExpression(temp1);
        simplifiedOutput[i] = evaluateBooleanExpression(temp2);

        if (originalOutput[i] != simplifiedOutput[i]) {
            areEquivalent = false;
            break;
        }
    }

    if (areEquivalent) {
        cout << "Expressions are equivalent.\n";
    } else {
        cout << "Expressions are not equivalent.\n";
    }
}

// Check if the expressions are satisfiable
bool checkSatisfiability(string originalExpr, string simplifiedExpr) {
    bool A, B, C;
    bool originalOutput[8], simplifiedOutput[8];
    bool satisfiable = false;


    cout << "\nPrinting truth tables for both expressions:\n";
    // Truth table for the original expression
    cout << "Original Expression:\n";
    for (int i = 0; i < 8; ++i) {
        A = truthTable[i][0];
        B = truthTable[i][1];
        C = truthTable[i][2];
        string temp1 = originalExpr;
        substituteVariables(temp1, A, B, C);
        originalOutput[i] = evaluateBooleanExpression(temp1);
    }
    printTruthTable(originalOutput, "Original Expression");

    // Truth table for the simplified expression
    cout << "\nSimplified Expression:\n";
    for (int i = 0; i < 8; ++i) {
        A = truthTable[i][0];
        B = truthTable[i][1];
        C = truthTable[i][2];
        string temp2 = simplifiedExpr;
        substituteVariables(temp2, A, B, C);
        simplifiedOutput[i] = evaluateBooleanExpression(temp2);
    }
    printTruthTable(simplifiedOutput, "Simplified Expression");

    // Check equivalence before satisfiability
    checkEquivalence(originalExpr, simplifiedExpr);

    // check for satisfiable combinations
    cout << "\nSatisfiable input combinations (A B C):\n";
    for (int i = 0; i < 8; ++i) {
        A = truthTable[i][0];
        B = truthTable[i][1];
        C = truthTable[i][2];
        string temp1 = originalExpr, temp2 = simplifiedExpr;
        substituteVariables(temp1, A, B, C);
        substituteVariables(temp2, A, B, C);
        originalOutput[i] = evaluateBooleanExpression(temp1);
        simplifiedOutput[i] = evaluateBooleanExpression(temp2);

        // If the outputs from both expressions match, print the values of A, B, C
        if (originalOutput[i] == simplifiedOutput[i]) {
            cout << A << " " << B << " " << C << "\n";
            satisfiable = true;
        }
    }

    if (!satisfiable) {
        cout << "No satisfiable input combinations found.\n";
    }

    return satisfiable;
}

// Function to change a gate
void modifyGate(string &expression) {
    for (size_t i = 0; i < expression.size(); ++i) {
        if (expression[i] == '&') {
            expression[i] = '|';
            return;
        } else if (expression[i] == '|') {
            expression[i] = '&';
            return;
        }
    }
}

int main() {
    string originalExpr, simplifiedExpr;

    cout << "Enter the Original circuit expression (e.g. ((A|!C)&(B|!C)]&[(C|B)&(C|A)): ";
    getline(cin, originalExpr);

    cout << "Enter the Simplified circuit expression (e.g. A&B): ";
    getline(cin, simplifiedExpr);


    if (checkSatisfiability(originalExpr, simplifiedExpr)) {
        cout << "Expressions are satisfiable.\n";
    } else {
        cout << "Expressions are not satisfiable. Modifying the circuit...\n";

        modifyGate(simplifiedExpr);
        cout << "Modified expression: " << simplifiedExpr << "\n";

        if (checkSatisfiability(originalExpr, simplifiedExpr)) {
            cout << "Expressions are now satisfiable after modification.\n";
        } else {
            cout << "Expressions are still not satisfiable after modification.\n";
        }
    }

    return 0;
}
