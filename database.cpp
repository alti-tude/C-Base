#include "main.h"

TableObj Database::getTable(std::string name){   
    // it is the colname
    if(this->table_map.find(name)==this->table_map.end()) 
        throw "Table with name ("+name+") doesn't exist.";
    
    return this->tables[this->table_map[name]];
}
void Database::addTable(std::string table_name, TableObj table){
    if(this->table_map.find(table_name) != this->table_map.end())
        throw "Table of this name already exists";

    this->table_map[table_name] = this->num_tables++;
    this->tables.push_back(table);
}
