#include <unordered_map>
#include "symboltable.h"

#include <string>
#include <vector>
#include <unordered_map>
#include "parsetoken.h"
#include "symboltable.h"

using namespace std;

SymbolTable::SymbolTable() {
  newScope = true;
  table = new vector<unordered_map<string, ParseData>*>();
}

void SymbolTable::enterNewScope() {
  //the next time something is added, a new symbol table is created
  newScope = true;
}

void SymbolTable::leaveScope() {
  //leave a scope, so discard "innermost" symbol table
  table->pop_back();
}

void SymbolTable::put(char* var, ParseData value) {

  //if entering a new scope, create a new hash map to represent it
  unordered_map<string, ParseData>* map = (newScope) ? new unordered_map<string, ParseData>() : table->back();
  
  //add element to the map
  (*map)[var] = value; 

  //if new scope, append to end of table and reset
  if(newScope) {
    newScope = false;
    table->push_back(map);
  }
}

ParseData SymbolTable::get(char* var) {
  
  //start at innermost scope and work your way up
  vector<unordered_map<string, ParseData>*>::reverse_iterator rit;
  for(rit = table->rbegin(); rit != table->rend(); rit++) {
  
    //map at this particular scope
    unordered_map<string, ParseData>* map = (*rit);
    
    //see if this map contains the variable
    unordered_map<string, ParseData>::const_iterator value = map->find(var);
    if(value != map->end())
      return value->second;
  }
  
  //not found, return "invalid" data
  ParseData d;
  d.type = INVALID_T;
  return d;
}

