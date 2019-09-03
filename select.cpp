#include "statements.h"
#include "main.h"

// enum ExprType{EQUAL,LEQ,GEQ,GET,LET,AND, OR, DATA, NONE};
// enum AggregateType{MAX, MIN, SUM, AVG, NONE};

// struct Expr{
//     ExprType type;
//     std::string data;
//     Expr* left;
//     Expr* right;
// };

// struct SelectData{
//     AggregateType aggregate_type;
//     std::vector<std::string> cols;
//     std::vector<std::string> tables;
//     Expr* where_expr;
// };

TableObj merge(std::vector<std::string> tables, Database* db){
    if(tables.size() == 1) {
        TableObj table = db->getTable(tables[0]);

        //update the map from colName rto tableName.colName
        std::map<std::string, int> tmap;
        for(auto it:table.col_map) tmap[tables[0]+"."+it.first] = it.second;
        table.col_map = tmap;

        return table;
    } 

    std::vector<std::string> left, right;

    for(int i=0;i<tables.size()/2;i++) left.push_back(tables[i]);
    for(int i=tables.size()/2;i<tables.size();i++) right.push_back(tables[i]);

    TableObj table1 = merge(left, db);
    TableObj table2 = merge(right, db);

    TableObj table = TableObj(table1, table2);
    
    return table;
}

// TableObj select(SelectData data_struct, Database* db){
//     std::vector<TableObj> tables;
    
    

//     //get the tables and take a cross product


//     //aggregate
// }

int main(){
    Database db = Database("files/metadata.txt", "files/");

    std::vector<std::string> vec;
    vec.push_back("table1");
    vec.push_back("table2");
    
    TableObj table;
    table = merge(vec, &db);

    std::cout << table.prints();
    return 0;
}