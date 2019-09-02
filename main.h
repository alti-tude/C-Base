#ifndef MAIN_H
#define MAIN_H
#define NULL_VAL -1000000
#include<bits/stdc++.h>


class TableObj{
private:
    std::map<std::string, int> col_map;
    int num_cols;
    int num_rows;
public:
//Every vector stores one column. To add a col, add a new vector, to add a record, add one to the end of every vector.
    std::vector<std::vector<int> > cols;
    TableObj():num_cols(0),num_rows(0){}
    ~TableObj();

    TableObj getByColumns(std::vector<std::string> cols);
    void addColumn(std::string col_name, std::vector<int> col_data);
    void addRow(std::vector<int> row);
};

class Database{
private: 
    std::vector<TableObj> tables;
    std::map<std::string, int> table_map;
    int num_tables;
public:
    Database():num_tables(0){}
    ~Database();

    TableObj getTable(std::string name);
    void addTable(std::string name, TableObj table);
};

#endif