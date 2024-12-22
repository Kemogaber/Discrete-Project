#include <string>
#include <utility>
#include <list>
#include <vector>
#include <cmath>
#include <iostream>

class Variable;
class VariableValue;
class And;
class Or;
class Not;
class IfThen;
class Iff;

class Expression {
public:
    // evaluate expression
    virtual bool evaluate() = 0;

    // get all binary combinations for a set of variables
    static std::vector<std::vector<VariableValue>> all_ordered_combinations(std::vector<Variable*>& variables);

    // get truth set intersection (and-ing) of two truth sets
    static std::vector<std::vector<VariableValue>> truth_set_intersection(std::vector<std::vector<VariableValue>> a, std::vector<std::vector<VariableValue>> b);

    // get truth set of an expression
    std::vector<std::vector<VariableValue>> truth_set(std::vector<Variable*>& variables);

    // compare two expressions, return collisions
    template<typename... Variables>
    static std::vector<std::vector<VariableValue>> compare(Expression *a, Expression *b, Variable *first, Variables *... rest);

    // operator overloading for all logical operations, easier front-end use experince
    friend And operator&(Expression& c1, Expression& c2);
    friend And operator&(Expression&& c1, Expression& c2);
    friend And operator&(Expression& c1, Expression&& c2);
    friend And operator&(Expression&& c1, Expression&& c2);
    friend Or operator|(Expression& c1, Expression& c2);
    friend Or operator|(Expression&& c1, Expression& c2);
    friend Or operator|(Expression& c1, Expression&& c2);
    friend Or operator|(Expression&& c1, Expression&& c2);
    friend Not operator!(Expression& c1);
    friend Not operator!(Expression&& c1);
    friend IfThen operator>>(Expression&& e1, Expression&& e2);
    friend IfThen operator>>(Expression& e1, Expression&& e2);
    friend IfThen operator>>(Expression&& e1, Expression& e2);
    friend IfThen operator>>(Expression& e1, Expression& e2);
    friend Iff operator<=>(Expression&& e1, Expression&& e2);
    friend Iff operator<=>(Expression& e1, Expression&& e2);
    friend Iff operator<=>(Expression&& e1, Expression& e2);
    friend Iff operator<=>(Expression& e1, Expression& e2);
};

class Argument {
public:
    // test if the argument is valid
    template<typename... Rest>
    static bool valid(std::vector<Variable *> variables, Expression&& conclusion, Rest&& ... rest);
    template<typename... Rest>
    static bool valid(std::vector<Variable *> variables, Expression& conclusion, Rest&& ... rest);
    static bool valid(std::vector<Variable *> variables, Expression * conclusion, std::vector<Expression*> premises);

    // test if the statements are satisfiable
    static bool satisfiable(std::vector<Variable *> variables, Expression * conclusion, std::vector<Expression*> premises);
};

// variables that have exact values, can be updated
class Variable: public Expression {
private:
    std::string name;
    bool value;

public:
    explicit Variable(std::string&& name, bool value = false): name(name), value(value) {
    }

    explicit Variable(bool value = false): name("unnamed variable"), value(value) {
    }

    explicit Variable(std::string&& name): name(name), value(false) {
    }

    void set_value(bool v) {
        this->value = v;
    }

    bool evaluate() override {
        return value;
    }
};

class UnaryExpression : public Expression {
protected:
    Expression * a;

public:
    explicit UnaryExpression(Expression& a): a(&a) {
    }

    explicit UnaryExpression(Expression&& a): a(&a) {
    }
};

class BinaryExpression : public Expression {
protected:
    Expression * a;
    Expression * b;

public:
    BinaryExpression(Expression& a, Expression& b): a(&a), b(&b) {
    }

    BinaryExpression(Expression&& a, Expression&& b): a(&a), b(&b) {
    }

    BinaryExpression(Expression&& a, Expression& b): a(&a), b(&b) {
    }

    BinaryExpression(Expression& a, Expression&& b): a(&a), b(&b) {
    }
};

class And : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;

    bool evaluate() override {
        return a->evaluate() && b->evaluate();
    };
};

class Or : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;

    bool evaluate() override {
        return a->evaluate() || b->evaluate();
    };
};

class IfThen : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;

    bool evaluate() override {
        return !a->evaluate() || b->evaluate();
    };
};

class Iff : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;

    bool evaluate() override {
        return (!a->evaluate() && !b->evaluate()) || (a->evaluate() && b->evaluate());
    };
};

class Not : public UnaryExpression {
public:
    using UnaryExpression::UnaryExpression;

    bool evaluate() override {
        return !a->evaluate();
    };
};

// saving a "snippet" of a variable value to use it later
class VariableValue {
public:
    Variable * a;
    bool value;

    VariableValue(Variable * a, bool value): a(a), value(value) {
    }

    void set_value() const {
        a->set_value(value);
    }
};

template<typename... Variables>
std::vector<std::vector<VariableValue>> Expression::compare(Expression * a, Expression * b, Variable *first, Variables *... rest) {
    std::vector<Variable *> variables = {first, rest...};
    std::vector<std::vector<VariableValue>> rows = all_ordered_combinations(variables);
    std::vector<std::vector<VariableValue>> collisions;

    for (std::vector<VariableValue>& row : rows) {
        for (VariableValue& v : row) {
            v.set_value();
        }

        // Equal
        if (a->evaluate() != b->evaluate()) {
            collisions.emplace_back(row);
        }
    }

    return collisions;
}

template<typename... Rest>
bool
Argument::valid(std::vector<Variable *> variables, Expression &&conclusion, Rest &&... rest) {

    std::vector<Expression *> premises = {&rest...};

    return valid(variables, &conclusion, premises);
}

template<typename... Rest>
bool
Argument::valid(std::vector<Variable *> variables, Expression &conclusion, Rest &&... rest) {

    std::vector<Expression *> premises = {&rest...};

    return valid(variables, &conclusion, premises);
}

class TruthRow {
public:
    static double value(std::vector<VariableValue> values) {
        double r = 0;

        for (int i = 0; i < values.size(); ++i) {
            r += pow(2, i) * values[i].value;
        }

        return r;
    }
};

And operator&(Expression& c1, Expression& c2)
{
    return {c1, c2};
}

And operator&(Expression&& c1, Expression& c2)
{
    return {c1, c2};
}

And operator&(Expression& c1, Expression&& c2)
{
    return {c1, c2};
}

And operator&(Expression&& c1, Expression&& c2)
{
    return {c1, c2};
}

Or operator|(Expression& c1, Expression& c2)
{
    return {c1, c2};
}

Or operator|(Expression&& c1, Expression& c2)
{
    return {c1, c2};
}

Or operator|(Expression& c1, Expression&& c2)
{
    return {c1, c2};
}

Or operator|(Expression&& c1, Expression&& c2)
{
    return {c1, c2};
}

Not operator!(Expression& c1)
{
    return Not(c1);
}

Not operator!(Expression&& c1)
{
    return Not(c1);
}

IfThen operator>>(Expression&& e1, Expression&& e2) {
    return {e1, e2};
}

IfThen operator>>(Expression& e1, Expression&& e2) {
    return {e1, e2};
}

IfThen operator>>(Expression&& e1, Expression& e2) {
    return {e1, e2};
}

IfThen operator>>(Expression& e1, Expression& e2) {
    return {e1, e2};
}


Iff operator<=>(Expression&& e1, Expression&& e2) {
    return {e1, e2};
}
Iff operator<=>(Expression&& e1, Expression& e2) {
    return {e1, e2};
}
Iff operator<=>(Expression& e1, Expression&& e2) {
    return {e1, e2};
}
Iff operator<=>(Expression& e1, Expression& e2) {
    return {e1, e2};
}

std::vector<std::vector<VariableValue>> Expression::all_ordered_combinations(std::vector<Variable *>& variables) {
    std::vector<std::vector<VariableValue>> rows;

    // For all possible rows in truth table
    for (int i = 0; i < pow(2, (double) variables.size()); ++i) {
        std::vector<VariableValue> row;

        // Set up each variable
        for (int j = 0; j < variables.size(); ++j) {
            row.emplace_back(variables[j], (i >> j) & 1);
        }

        rows.emplace_back(row);
    }

    return rows;
}

std::vector<std::vector<VariableValue>> Expression::truth_set(std::vector<Variable *>& variables) {
    std::vector<std::vector<VariableValue>> rows = all_ordered_combinations(variables);
    std::vector<std::vector<VariableValue>> set;

    for (std::vector<VariableValue>& row : rows) {
        for (VariableValue& v : row) {
            v.set_value();
        }

        if (this->evaluate()) {
            set.emplace_back(row);
        }
    }

    return set;
}

std::vector<std::vector<VariableValue>>
Expression::truth_set_intersection(std::vector<std::vector<VariableValue>> a, std::vector<std::vector<VariableValue>> b) {
    std::vector<std::vector<VariableValue>> set;
    int i = 0, j = 0;

    while (i < a.size() && j < b.size()) {
        if (TruthRow::value(a[i]) == TruthRow::value(b[j])) {
            set.emplace_back(a[i]);
            i++; j++;
        }
        else
        if (TruthRow::value(a[i]) < TruthRow::value(b[j]))
            i++;
        else
            j++;
    }

    return set;
}

bool Argument::valid(std::vector<Variable *> variables, Expression *conclusion, std::vector<Expression *> premises) {
    auto conclusion_ts = conclusion->truth_set(variables);
    auto premises_ts = premises[0]->truth_set(variables);

    for (int i = 1; i < premises.size(); ++i) {
        premises_ts = Expression::truth_set_intersection(premises_ts, premises[i]->truth_set(variables));
    }

    // compare and make sure
    return premises_ts.size() == Expression::truth_set_intersection(conclusion_ts, premises_ts).size();
}

bool Argument::satisfiable(std::vector<Variable *> variables, Expression *conclusion, std::vector<Expression *> premises) {
    auto conclusion_ts = conclusion->truth_set(variables);
    auto premises_ts = premises[0]->truth_set(variables);

    for (int i = 1; i < premises.size(); ++i) {
        premises_ts = Expression::truth_set_intersection(premises_ts, premises[i]->truth_set(variables));
    }

    return !Expression::truth_set_intersection(conclusion_ts, premises_ts).empty();
}

// main
int main(int argc, char** argv) {
    Variable
    f("I played football"),
    s("I played basketball"),
    b("I ate breakfast"),
    h("I am happy");

    bool valid = Argument::valid(
                    // variables
                    {&f, &b, &h},

                    // conclusion
                    h | f,

                    // premises
                    f | b,
                    b | s,
                    h <=> b
            );

    std::cout << valid << std::endl;

    return 0;
}
