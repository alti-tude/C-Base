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


TableObj select(SelectData data_struct, Database* db){
    TableObj ans = TableObj();

    //get the tables and take a cross product
    TableObj table = merge(data_struct.tables, db);

    //validate selected columns.
    data_struct.cols = validate(data_struct.cols, data_struct.parsed_tables, data_struct.tables, db);
    
    //validate expr
    validateExpr(data_struct.where_expr, data_struct.tables, db);

    //apply expression
    std::bitset<MAX_RECORDS> bs = table.applyExpr(data_struct.where_expr);
    table.deleteRowByBitset(bs);

    //get the selected columns
    table = table.getColumns(data_struct.cols);
    
    //distinct
    if(data_struct.distinct) table.distinct();

    //aggregate
    
    if(!data_struct.aggregate) ans = table;
    else{
        for(int i=0;i<data_struct.aggregate_type.size();i++){
            if(data_struct.aggregate_type[i]==NONE){
                throw std::string("All columns are not aggregated.");
            }
            std::vector<int> v = table.getColumnByIdx(i);
            if(data_struct.aggregate_type[i]==MAX){
                int ma = INT_MIN;
                for(auto it:v) ma = std::max(ma,it);
                ans.addColumn("MAX("+data_struct.cols[i]+")", std::vector<int>({ma}));
            }
            if(data_struct.aggregate_type[i]==MIN){
                int ma = INT_MAX;
                for(auto it:v) ma = std::min(ma,it);
                ans.addColumn("MIN("+data_struct.cols[i]+")", std::vector<int>({ma}));
            }
            if(data_struct.aggregate_type[i]==SUM){
                int ma = 0;
                for(auto it:v) ma += it;
                ans.addColumn("SUM("+data_struct.cols[i]+")", std::vector<int>({ma}));
            }   
            if(data_struct.aggregate_type[i]==AVG){
                int ma = 0;
                for(auto it:v) ma += it;
                ma = ma/v.size();
                ans.addColumn("AVG("+data_struct.cols[i]+")", std::vector<int>({ma}));
            }
        }
    }

    return ans;
}

int main(){
    try{
        Database db = Database("files/metadata.txt", "files/");
        SelectData sd;
        sd.cols = std::vector<std::string> ({"A", "B"});
        sd.parsed_tables = std::vector<std::string> ({"table1", "NULL"});
        sd.tables = std::vector<std::string> ({"table1"});
        
        Expr expr1;
        expr1.type = GEQ;
        Expr exprl;
        exprl.col = "A";
        exprl.parsed_table = "table1";
        Expr exprr;
        exprr.col = "B";
        exprr.parsed_table = "table1";
        expr1.left = &exprl;
        expr1.right = &exprr;

        Expr expr11;
        expr11.type = EQUAL;
        Expr expr1l;
        expr1l.col = "C";
        expr1l.parsed_table = "table1";
        Expr expr1r;
        expr1r.val = 2;
        // expr1r.parsed_table = "table1";
        expr11.left = &expr1l;
        expr11.right = &expr1r;

        Expr expr2;
        expr2.type = OR;
        expr2.left = &expr1;
        expr2.right = &expr11;

        sd.aggregate = false;
        sd.aggregate_type = std::vector<AggregateType> ({MAX, MIN});
        sd.where_expr = &expr1;
        sd.distinct = true;
        std::cout << select(sd, &db).prints() << std::endl;    
    }
    catch(std::string e){
        std::cout << e << std::endl;
    }  
    return 0;
}