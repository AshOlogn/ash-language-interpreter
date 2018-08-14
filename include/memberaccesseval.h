#ifndef MEMBERACCESS_H
#define MEMBERACCESS_H

#include "parsetoken.h"
#include "parsenode.h"

//used to access members and subarrays of strings and arrays
ParseData evaluateArrayAccess(ArrayAccessNode* node);


#endif