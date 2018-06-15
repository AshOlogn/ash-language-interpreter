#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "parsetoken.h"

class SymbolTable {
  
  private:
    std::vector<std::unordered_map<std::string, ParseData>*>* table;
 
  public:
    bool newScope;
    
    SymbolTable();
    void enterNewScope();
    void leaveScope();
    void put(char* var, ParseData value);
    ParseData get(char* var);
};


#endif