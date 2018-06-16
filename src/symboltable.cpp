#include <string>
#include <vector>
#include <unordered_map>
#include "parsetoken.h"
#include "symboltable.h"

using namespace std;

SymbolTable::SymbolTable() {
  table = new vector<unordered_map<char*, ParseData>*>();
  table->push_back(new unordered_map<char*, ParseData>());
}

void SymbolTable::enterNewScope() {
  //the next time something is added, a new symbol table is created
  table->push_back(new unordered_map<char*, ParseData>());
}

void SymbolTable::leaveScope() {
  //leave a scope, so discard "innermost" symbol table
  table->pop_back();
}

void SymbolTable::put(char* var, ParseData value) {
  
  //get symbol table at innermost scope
  unordered_map<char*, ParseData>* map = table->back();
  
  //add element to the map
  (*map)[var] = value;
}

ParseData SymbolTable::get(char* var) {
  
  //start at innermost scope and work your way up
  vector<unordered_map<char*, ParseData>*>::reverse_iterator rit;
  for(rit = table->rbegin(); rit != table->rend(); rit++) {
  
    //map at this particular scope
    unordered_map<char*, ParseData>* map = (*rit);
    
    //see if this map contains the variable
    unordered_map<char*, ParseData>::const_iterator value = map->find(var);
    if(value != map->end())
      return value->second;
  }
  
  //not found, return "invalid" data
  ParseData d;
  d.type = INVALID_T;
  return d;
}

