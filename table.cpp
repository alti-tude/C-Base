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

TableObj TableObj::getColumns(std::vector<std::string> cols){
    TableObj table = TableObj();
    
    // it is the colname
    for(auto it:cols){
        if((this->col_map).find(it)==(this->col_map).end()) 
            throw std::string("Requested column(s) don't exis in the table.");

        int col_index = this->col_map[it];
        table.addColumn(it, this->cols[col_index]);    
    }
    return table;
}

std::vector<int> TableObj::getColumnByIdx(int idx){
    if(idx>=this->num_cols or idx<0)
        throw std::string("column index out of bounds");

    std::cout << this->cols[0].size() << std::endl;
    for(auto it:this->cols[idx]) std::cout << it << std::endl;
    return this->cols[idx];
}

void TableObj::addColumn(std::string col_name, std::vector<int> col_data){
    if(this->col_map.find(col_name) != this->col_map.end())
        throw "Column of this name already exists";

    while(col_data.size()<this->num_rows) col_data.push_back(NULL_VAL);
    while(this->num_rows < col_data.size()){
        for(int i=0;i<this->num_cols;i++) {
            this->cols[i].push_back(NULL_VAL);
        }
        this->num_rows +=1;
    }

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

void TableObj::deleteRow(int idx){
    if(idx<0 or idx>=this->num_rows)
        throw std::string("Row index out of range");

    for(int i=0;i<this->num_cols;i++)
        this->cols[i].erase(this->cols[i].begin()+idx); 
    this->num_rows --;
}

void TableObj::deleteRowByBitset(std::bitset<MAX_RECORDS> bs){
    std::vector<std::vector<int> > tcols;
    std::vector<int> empty;

    for(int i=0;i<this->num_cols;i++) {
        tcols.push_back(empty);
        for(int j=0;j<this->num_rows;j++) 
            if(bs.test(j))
                tcols[i].push_back(this->cols[i][j]); 
    }

    this->cols = tcols;
    this->num_rows = tcols[0].size();
}

void TableObj::distinct(){
    std::bitset<MAX_RECORDS> bs;
    bs.set();
    for(int i=0;i<this->num_rows;i++){
        for(int j=i+1;j<this->num_rows;j++){
            bool match =1;
            for(int k=0;k<this->num_cols;k++){
                if(this->cols[k][i]!=this->cols[k][j]){
                    match =0;
                    break;
                }
            }

            if(match) bs.reset(j);
        }
    }

    this->deleteRowByBitset(bs);
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

std::bitset<MAX_RECORDS> TableObj::applyExpr(Expr* expr){
    if(expr->type == AND or expr->type == OR){
        std::bitset<MAX_RECORDS> bs1, bs2;
        bs1 = this->applyExpr(expr->left);
        bs2 = this->applyExpr(expr->right);

        if(expr->type==AND) return bs1 & bs2;
        else return bs1 | bs2;
    }

    Expr* lhs = expr->left;
    Expr* rhs = expr->right;
    std::bitset<MAX_RECORDS> bs;
    bs.reset();
    
    if(lhs->col.size()!=0 and rhs->col.size()!=0) {
        int lhs_idx = this->col_map[lhs->col];
        int rhs_idx = this->col_map[rhs->col];
            
        for(int i=0;i<this->num_rows;i++){
            if(eval(expr->type, this->cols[lhs_idx][i], this->cols[rhs_idx][i]))
                bs.set(i);   
        }

        return bs;
    }
    if(lhs->col.size()!=0) {
        int lhs_idx = this->col_map[lhs->col];
            
        for(int i=0;i<this->num_rows;i++){
            if(eval(expr->type, this->cols[lhs_idx][i], rhs->val))
                bs.set(i);   
        }

        return bs;
    }
    if(rhs->col.size()!=0) {
        int rhs_idx = this->col_map[rhs->col];
            
        for(int i=0;i<this->num_rows;i++){
            if(eval(expr->type, lhs->val, this->cols[rhs_idx][i]))
                bs.set(i);   
        }

        return bs;
    }

    for(int i=0;i<this->num_rows;i++){
        if(eval(expr->type, lhs->val, rhs->val))
            bs.set(i);
    }

    return bs;
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