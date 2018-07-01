#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "parsetoken.h"
#include "symboltable.h"

using namespace std;

SymbolTable::SymbolTable() {
  table = new vector<unordered_map<string, ParseData>*>();
  table->push_back(new unordered_map<string, ParseData>());
	depth = 0;
}

void SymbolTable::enterNewScope() {
  //the next time something is added, a new symbol table is created
  table->push_back(new unordered_map<string, ParseData>());
	depth++;
}

void SymbolTable::leaveScope() {
  //leave a scope, so discard "innermost" symbol table
  table->pop_back();
	depth--;
}

//create new key in innermost scope
void SymbolTable::declare(string var, ParseData value) {
  
  //get symbol table at innermost scope
  unordered_map<string, ParseData>* map = table->back();
  
  //add element to the map
  (*map)[var] = value;
}

//update value in innermost scope in which it is found
void SymbolTable::update(string var, ParseData value) {

  //start at innermost scope and work your way up
  vector<unordered_map<string, ParseData>*>::reverse_iterator rit;
  
  for(rit = table->rbegin(); rit != table->rend(); rit++) {
    //map at this particular scope
    unordered_map<string, ParseData>* map = (*rit);
    
    //if this map contains it, update its value
    if(map->find(var) != map->end()) {
      (*map)[var] = value;
      return;
    } 
  }
  
}

//returns if variable is already declared in current innermost scope
bool SymbolTable::isDeclaredInScope(string var) {

  unordered_map<string, ParseData>* map = *(table->rbegin());
  return map->find(var) != map->end();
}

//returns if variable is declared anywhere at all
bool SymbolTable::isDeclared(string var) {
  
  //start at innermost scope and work your way up
  vector<unordered_map<string, ParseData>*>::reverse_iterator rit;
  
  for(rit = table->rbegin(); rit != table->rend(); rit++) {
    //map at this particular scope
    unordered_map<string, ParseData>* map = (*rit);

    //if this map contains it, return true
    if(map->find(var) != map->end())
      return true; 
  }
  
  return false;
}

ParseData SymbolTable::get(string var) {
  
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

std::string SymbolTable::toString() {

	std::string str = "";

	//loop through all the hash maps
	std::vector<std::unordered_map<string, ParseData>*>::iterator it;
	for(it = table->begin(); it != table->end(); it++) {

		//loop through the keys in the hash map and print those
		std::unordered_map<string, ParseData>::iterator it2;
		for(it2 = (*it)->begin(); it2 != (*it)->end(); it2++) {
			str.append(it2->first);
			str.append(", ");
			str.append(toStringParseDataType(it2->second.type));
			str.append("\n");
		}

		str.append("----------------------------\n");
	}
	str.append("end of table");

	return str;
}

