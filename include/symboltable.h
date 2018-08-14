#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <cstring>
#include <vector>
#include <unordered_map>
#include "parsetoken.h"

//used to keep track of variables and functions in appropriate scopes
class SymbolTable {
  
  private:
    std::vector<std::unordered_map<std::string, ParseData>*>* table;
		uint32_t depth;
 
  public:
    SymbolTable();
    void enterNewScope();
    void leaveScope();
    void declare(std::string var, ParseData value);
    void update(std::string var, ParseData value);
    bool isDeclaredInScope(std::string var);
    bool isDeclared(std::string var);
    ParseData get(std::string var);
		std::string toString();
};


#endif
