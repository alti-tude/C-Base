#include "main.h"

TableObj::TableObj(TableObj table1, TableObj table2):num_cols(0),num_rows(0){
    std::vector<int> empty;

    // copy columns from table1
    std::vector<std::string> names(table1.num_cols);
    for(auto it:table1.col_map) names[it.second] = it.first;

    for(auto it:names) this->addColumn(it, empty);

    //copy columns from table2
    std::vector<std::string> names2(table2.num_cols);
    for(auto it:table2.col_map) names2[it.second] = it.first;

    for(auto it:names2) this->addColumn(it, empty);


    //cross product
    for(int i=0;i<table1.num_rows;i++){
        std::vector<int> record1 = table1.getRecord(i);

        for(int j=0;j<table2.num_rows;j++){
            std::vector<int> record2 = table2.getRecord(i);
            std::vector<int> trecord = record1;
            trecord.insert(trecord.end(), record2.begin(), record2.end());

            this->addRow(trecord);
        }
    }
}

TableObj TableObj::getColumn(std::vector<std::string> cols){
    TableObj table = TableObj();
    
    // it is the colname
    for(auto it:cols){
        if((this->col_map).find(it)==(this->col_map).end()) 
            throw "Requested column(s) don't exis in the table.";

        int col_index = this->col_map[it];
        table.addColumn(it, this->cols[col_index]);    
    }
    return table;
}
void TableObj::addColumn(std::string col_name, std::vector<int> col_data){
    if(this->col_map.find(col_name) != this->col_map.end())
        throw "Column of this name already exists";

    while(col_data.size()!=this->num_rows) col_data.push_back(NULL_VAL);

    this->col_map[col_name] = this->num_cols++;
    this->cols.push_back(col_data);
}

void TableObj::addRow(std::vector<int> row){
    while(row.size()!=this->num_cols) 
        row.push_back(NULL_VAL);

    for(int i=0;i<this->num_cols;i++)
        this->cols[i].push_back(row[i]); 
    this->num_rows ++;
}

std::vector<int> TableObj::getRecord(int idx){
    if(idx>=this->num_rows) 
        throw "The record index is out of bounds";
    
    std::vector<int> record;

    for(int i=0;i<this->num_cols;i++){
        record.push_back(this->cols[i][idx]);
    }
    return record;
}

std::string TableObj::prints(){
    std::stringstream ss;
    ss << "Num_cols: " << this->num_cols <<"\n";
    ss << "Num_rows: " << this->num_rows <<"\n";

    std::vector<std::string> names(this->num_cols);
    for(auto it:this->col_map) names[it.second] = it.first;
    for(auto it:names) ss << it <<"\t";
    ss << std::endl;

    for(int i=0;i<this->num_rows;i++) {
        std::vector<int> record = this->getRecord(i);
        for(auto it:record) ss << it <<"\t\t";
        ss << std::endl;
    }

    return ss.str();
}