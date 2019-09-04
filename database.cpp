#include "main.h"
#include "fileparser.h"

Database::Database(std::string schema, std::string data_folder):num_tables(0){
    std::ifstream schema_fil(schema);

    if(!schema_fil) throw "Schema file doesn't exist.";
    
    while(!schema_fil.eof()){
        std::string line;
        std::getline(schema_fil, line, '\n');
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        if(line == "<begin_table>"){
            std::string table_name;
            std::getline(schema_fil, table_name, '\n');
            table_name.erase(std::remove(table_name.begin(), table_name.end(), '\r'), table_name.end());
            
            std::ifstream data(data_folder+table_name+".csv");

            if(!data) throw "Data file for table ("+table_name+") doesn't exist.";

            TableObj table = buildTable(schema_fil, data);
            this->addTable(table_name, table);
        }
    }
}

TableObj Database::getTable(std::string name){   
    if(this->table_map.find(name)==this->table_map.end()) 
        throw "Table with name ("+name+") doesn't exist.";
    
    return this->tables[this->table_map[name]];
}
void Database::addTable(std::string table_name, TableObj table){
    if(this->table_map.find(table_name) != this->table_map.end())
        throw "Table of this name already exists";

    std::cout << "Adding table with name " << table_name << "\n";
    this->table_map[table_name] = this->num_tables++;
    this->tables.push_back(table);
}
