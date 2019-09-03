#ifndef MAIN_H
#define MAIN_H
#define NULL_VAL -1000000
#include<bits/stdc++.h>


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

    bool checkColumnExists(std::string col) {return this->col_map.find(col)==this->col_map.end();}
    TableObj getColumn(std::vector<std::string> cols);
    void addColumn(std::string col_name, std::vector<int> col_data);
    void addRow(std::vector<int> row);
    std::vector<int> getRecord(int idx);
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

#endif