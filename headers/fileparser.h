#ifndef FILEPARSER_H
#define FILEPARSE_H
#include "main.h"
std::vector<int> buildRow(std::string line);
TableObj buildTable(std::ifstream& schema, std::ifstream& data);

#endif
