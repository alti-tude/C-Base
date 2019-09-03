#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "main.h"
enum ExprType{EQUAL,LEQ,GEQ,GET,LET,AND, OR, DATA, NOOP};
enum AggregateType{MAX, MIN, SUM, AVG, NONE};

struct Expr{
    ExprType type;
    std::string data;
    Expr* left;
    Expr* right;
};

struct SelectData{
    AggregateType aggregate_type;
    std::vector<std::string> cols;
    std::vector<std::string> tables;
    Expr* where_expr;
};

TableObj select();

#endif