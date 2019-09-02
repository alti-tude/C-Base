#include "main.h"

std::vector<int> buildRow(std::string line){
    std::vector<int> row;

    // handle "" in csv
    std::stringstream ss;
    for(int i=0;line[i];i++) 
        if(line[i]!='\"') ss << line[i];
    line = ss.str();
    ss.str("");

    // split on , (assuming no whitespaces)
    for(int i=0;line[i];i++){
        if(line[i]!=',') ss << line[i];
        else {
            row.push_back(std::stoi(ss.str(), NULL, 10));
            ss.str("");
        }

    }

    row.push_back(std::stoi(ss.str(), NULL, 10));
    ss.str("");
    return row;
}

TableObj buildTable(std::ifstream& schema, std::ifstream& data){
    std::string line;
    std::getline(schema, line, '\n');
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

    TableObj table = TableObj();
    while(line!="<end_table>"){
        std::vector<int> v;
        table.addColumn(line, v);

        std::getline(schema, line, '\n');
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    }

    while(!data.eof()){
        std::getline(data, line, '\n');
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        std::vector<int> row = buildRow(line);
        table.addRow(row);

    }

    return table;
};



// int main(){

//     Database db;
//     try{
//     db = Database("files/metadata.txt", "files/");
//     }
//     catch(std::string e){
//         std::cout << e << std::endl;
//     }

//     TableObj t;
//     try{
//         t = db.getTable("table1");
//     }
//     catch(std::string e){
//         std::cout << e << std::endl;
//     }

//     for(int i=0;i<3;i++){
//         for(auto jt:t.cols[i]){
//             std::cout << jt << " ";
//         }
//         std::cout << "\n";
//     }
//     return 0;
// }