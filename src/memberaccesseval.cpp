#include <cstdint>
#include <cmath>
#include <cstring>
#include <string>
#include "token.h"
#include "typehandler.h"
#include "parsetoken.h"
#include "parsenode.h"
#include "memberaccesseval.h"


ParseData sliceHelper(char* arr, uint64_t startIndex, uint64_t endIndex) {
  
  ParseData d;
  int32_t len = strlen(arr);
  d.type = STRING_T;
  
  int32_t startInd = (int32_t) startIndex;
  int32_t endInd = (int32_t) endIndex;
  
  if(startInd < 0)
    startInd += len+1;
  
  if(endInd < 0)
    endInd += len+1;
  
  
  if(endInd <= startInd) {
    char* substring = new char[1];
    substring[0] = '\0';
    d.value.allocated = (void*) substring;
    return d;
  }
  
  char* substring = new char[endInd - startInd + 1];
  for(int32_t i = startInd; i < endInd; i++)
    substring[i-startInd] = arr[i];
  substring[endInd-startInd] = '\0';
  
  d.value.allocated = (void*) substring;

  return d;   
}


ParseData elementHelper(char* arr, uint64_t index) {
  
  ParseData d;
  int32_t len = strlen(arr);
  d.type = CHAR_T;
  
  int32_t ind = (int32_t) index;
  if(ind < 0)
    ind += len+1;
  
  d.value.integer = (unsigned char) arr[ind];
  return d;
}

ParseData evaluateArrayAccess(ArrayAccessNode* node) {
  
  ParseData array = node->array->evaluate();
  ParseData start = node->start->evaluate();
  ParseData end;
  
  if(node->isSlice)
    end = node->end->evaluate();
  
  if(node->isSlice) {
    return sliceHelper((char*) array.value.allocated, start.value.integer, end.value.integer);
  } else {
    return elementHelper((char*) array.value.allocated, start.value.integer); 
  }
}
