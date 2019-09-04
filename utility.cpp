#include "statements.h"
#include "main.h"


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

std::vector<std::string> validate(std::vector<std::string> cols, std::vector<std::string>parsed_tables, std::vector<std::string> tables, Database* db){
    for(int i=0;i<cols.size();i++){
        if(parsed_tables[i]!="NULL"){
            if(!(db->getTable(parsed_tables[i])).checkColumnExists(cols[i]))
                throw "The column: " + parsed_tables[i] + "." + cols[i] +" doesn't exist\n";
            
            //check if the parsed table is in the source list.
            bool check = 0;
            for(int j=0;j<tables.size();j++)
                if(tables[j]==parsed_tables[i]) {check=1; break;}
            
            if(!check) throw "Referenced table: " + parsed_tables[i] +" not part of source list\n";
        }
        else{
            for(int j=0;j<tables.size();j++){
                TableObj table = db->getTable(tables[j]);
                
                if(table.checkColumnExists(cols[i]) && parsed_tables[i]!="NULL")
                    throw std::string("Ambiguous/duplicate column. \n");
                else if(table.checkColumnExists(cols[i]))
                    parsed_tables[i] = tables[j];
            }

            if(parsed_tables[i]=="NULL")
                throw "Column: " + cols[i] + " doesn't exist";
        }
    }

    std::vector<std::string> ret;
    for(int i=0;i<cols.size();i++) ret.push_back(parsed_tables[i]+"."+cols[i]);

    return ret;
}

void validateExpr(Expr* expr, std::vector<std::string> tables, Database* db){
    if(expr->type==AND or expr->type==OR) {
        validateExpr(expr->left, tables, db);
        validateExpr(expr->right, tables, db);
        return;
    }

    Expr* lhs = expr->left;
    Expr* rhs = expr->right;

    if(lhs->col.size()!=0 and rhs->col.size()!=0){
        std::vector<std::string> validated_cols;
        validated_cols = validate(std::vector<std::string>({lhs->col, rhs->col}),
                                 std::vector<std::string>({lhs->parsed_table, rhs->parsed_table}), 
                                 tables, db);
        
        lhs->col = validated_cols[0];
        rhs->col = validated_cols[1];
    }
    else if(lhs->col.size()!=0){
        std::vector<std::string> validated_cols;
        validated_cols = validate(std::vector<std::string>({lhs->col}),
                                 std::vector<std::string>({lhs->parsed_table}), 
                                 tables, db);
        
        lhs->col = validated_cols[0];
    }
    else if(rhs->col.size()!=0){
        std::vector<std::string> validated_cols;
        validated_cols = validate(std::vector<std::string>({rhs->col}),
                                 std::vector<std::string>({rhs->parsed_table}), 
                                 tables, db);
        
        rhs->col = validated_cols[0];
    }
}

bool eval(ExprType type, int lhs, int rhs){
    if(type==EQUAL) return lhs==rhs;
    if(type==LEQ) return lhs<=rhs;
    if(type==GEQ) return lhs>=rhs;
    if(type==LET) return lhs<rhs;
    if(type==GET) return lhs>rhs;

    return false;
}