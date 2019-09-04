#ifndef MAIN_H
#define MAIN_H
#define NULL_VAL -1000000
#define MAX_RECORDS 20
#include<bits/stdc++.h>

enum ExprType{EQUAL,LEQ,GEQ,GET,LET,AND, OR, DATA, NOOP};
enum AggregateType{MAX, MIN, SUM, AVG, NONE};

struct Expr{
    ExprType type;
    std::string col;
    std::string parsed_table;
    int val;
    Expr* left;
    Expr* right;
};

struct SelectData{
    bool distinct;
    bool aggregate;
    std::vector<AggregateType> aggregate_type;
    std::vector<std::string> cols;
    std::vector<std::string> parsed_tables;
    std::vector<std::string> tables;
    Expr* where_expr;
};


/////////////////////////////////////////////////////////////////////
/////////table.cpp
/////////////////////////////////////////////////////////////////////
class TableObj{
private:
public:
//Every vector stores one column. To add a col, add a new vector, to add a record, add one to the end of every vector.
    std::map<std::string, int> col_map;
    std::vector<std::vector<int> > cols;
    int num_cols;
    int num_rows;

    TableObj():num_cols(0),num_rows(0){}
    TableObj(TableObj table1, TableObj table2);

    bool checkColumnExists(std::string col) {return this->col_map.find(col)!=this->col_map.end();}
    TableObj getColumns(std::vector<std::string> cols);
    std::vector<int> getColumnByIdx(int idx);
    void addColumn(std::string col_name, std::vector<int> col_data);
    void addRow(std::vector<int> row);
    void deleteRow(int idx);
    void deleteRowByBitset(std::bitset<MAX_RECORDS> bs);
    void distinct();
    std::vector<int> getRecord(int idx);
    std::bitset<MAX_RECORDS> applyExpr(Expr* expr);
    std::string prints();
};

class Database{
private: 
    std::vector<TableObj> tables;
    std::map<std::string, int> table_map;
    int num_tables;
public:
    Database(std::string schema, std::string data_folder);
    Database():num_tables(0){}

    TableObj getTable(std::string name);
    void addTable(std::string name, TableObj table);
};


////////////////////////////////////////////////////////////////
//////////utility.cpp
///////////////////////////////////////////////////////////////

TableObj merge(std::vector<std::string> tables, Database* db);

std::vector<std::string> validate(std::vector<std::string> cols, 
                                  std::vector<std::string>parsed_tables,
                                  std::vector<std::string> tables, 
                                  Database* db);

void validateExpr(Expr* expr, std::vector<std::string> tables, Database* db);

bool eval(ExprType type, int lhs, int rhs);
#endif