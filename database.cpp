#include "main.h"

TableObj TableObj::getByColumns(std::vector<std::string> cols){
    TableObj table = TableObj();
    
    // it is the colname
    for(auto it:cols){
        if((this->col_map).find(it)==(this->col_map).end()) 
            throw "Requested column(s) don't exis in the table.";

        int col_index = this->col_map[it];
        table.addColumn(it, this->cols[col_index]);    
    }
}
void TableObj::addColumn(std::string col_name, std::vector<int> col_data){
    if(this->col_map.find(col_name) != this->col_map.end())
        throw "Column of this name already exists";

    while(col_data.size()!=this->num_rows) col_data.push_back(NULL_VAL);

    this->col_map[col_name] = this->num_cols++;
    this->cols.push_back(col_data);
}

void TableObj::addRow(std::vector<int> row){
    while(row.size()!=this->num_cols) row.push_back(NULL_VAL);
}