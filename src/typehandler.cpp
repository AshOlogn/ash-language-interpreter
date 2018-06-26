#include "parsetoken.h"
#include "typehandler.h"


////////////////////////////////////
///////       Typecheck      ///////
////////////////////////////////////

//typecheck member access
bool typecheckMemberAccessExpression(ParseDataType index) {
  return index == INT32_T || index == INT64_T || index == UINT32_T || index == UINT64_T ||
         index == CHAR_T;
}

//typecheck expression appropriately depending on type of AbstractExpressionNode
bool typecheckImplicitCastExpression(ParseDataType origType, ParseDataType finalType) {

  switch(finalType) {
    
    case INT32_T: return origType == CHAR_T || origType == INT32_T || origType == UINT32_T;
                         
    case INT64_T: return isIntParseDataType(origType);
                         
    case UINT32_T: return origType == CHAR_T || origType == INT32_T || origType == UINT32_T;
                         
    case UINT64_T: return isIntParseDataType(origType);
    case CHAR_T: return origType == CHAR_T;
    case BOOL_T: return origType == BOOL_T;
    case DOUBLE_T: return isNumberParseDataType(origType);
    case STRING_T: return true;
    default: return false;
  }
}

bool typecheckExplicitCastExpression(ParseDataType origType, ParseDataType finalType) {
  
  if(isNumberParseDataType(finalType)) {
    return isNumberParseDataType(origType) || origType == BOOL_T;
  } else if(finalType == BOOL_T) {
    return isIntParseDataType(origType) || origType == BOOL_T;  
  } else if(finalType == STRING_T) {
    return true;
  } else {
    return false; 
  }
}

bool typecheckUnaryExpression(ParseOperatorType op, ParseDataType arg) {
  
  switch(op) {
    case POSTFIX_INC_OP: return isNumberParseDataType(arg);
    case PREFIX_INC_OP: return isNumberParseDataType(arg);
    case POSTFIX_DEC_OP: return isNumberParseDataType(arg);
    case PREFIX_DEC_OP: return isNumberParseDataType(arg);
    case POSITIVE_OP: return isNumberParseDataType(arg);
    case NEGATIVE_OP: return isNumberParseDataType(arg);
    case BIT_NOT_OP: return isIntParseDataType(arg);
    case NOT_OP: return arg == BOOL_T;
    default: return false;
  }
}

bool typecheckArithmeticExpression(ParseOperatorType op, ParseDataType l, ParseDataType r) {
  
  switch(op) {
  
    case EXPONENT_OP: return isNumberParseDataType(l) && isNumberParseDataType(r);
    case DIVIDE_OP: return isNumberParseDataType(l) && isNumberParseDataType(r);
    case MULTIPLY_OP: return (isNumberParseDataType(l) && isNumberParseDataType(r)) ||
                             (isIntParseDataType(l) && r == STRING_T) ||
                             (l == STRING_T && isIntParseDataType(r));

    case MOD_OP: return isNumberParseDataType(l) && isNumberParseDataType(r);

    case ADD_OP: return l == STRING_T || r == STRING_T ||
                        (isNumberParseDataType(l) && isNumberParseDataType(r));

    case SUBTRACT_OP: return isNumberParseDataType(l) && isNumberParseDataType(r);
    default: return false;
  }
}

bool typecheckBitLogicalExpression(ParseOperatorType op, ParseDataType l, ParseDataType r) {

  if(isBitwiseParseOperatorType(op)) {
    return isIntParseDataType(l) && isIntParseDataType(r);
  } else if(isLogicalParseOperatorType(op)) {
    return l == BOOL_T && r == BOOL_T;
  } else {
    return false;
  }
}

bool typecheckComparisonExpression(ParseOperatorType op, ParseDataType l, ParseDataType r) {
  return (isNumberParseDataType(l) && isNumberParseDataType(r)) ||
         (l == STRING_T && r == STRING_T);
}


////////////////////////////////////
///////      Return Type     ///////
////////////////////////////////////

ParseDataType getTypeUnaryExpression(ParseOperatorType op, ParseDataType arg) {
  
  if(!typecheckUnaryExpression(op, arg))
    return INVALID_T;

  switch(op) {
    case POSTFIX_INC_OP: return arg;
    case POSTFIX_DEC_OP: return arg;
    case PREFIX_INC_OP: return arg;
    case PREFIX_DEC_OP: return arg;
    case POSITIVE_OP: return arg;
    case NEGATIVE_OP: return arg;
    case BIT_NOT_OP: return arg;
    case NOT_OP: return BOOL_T;
    default: return INVALID_T;
  }
}

ParseDataType getTypeArithmeticExpression(ParseOperatorType op, ParseDataType l, ParseDataType r) {
  
  if(!typecheckArithmeticExpression(op, l, r))
    return INVALID_T;

  switch(op) {
  
    case EXPONENT_OP: return DOUBLE_T;
    case DIVIDE_OP: return getLargerNumberParseDataType(l, r);    
    case MULTIPLY_OP: return (l == STRING_T || r == STRING_T) ? STRING_T : 
                            getLargerNumberParseDataType(l, r);

    case MOD_OP: return getLargerNumberParseDataType(l, r);

    case ADD_OP: return (l == STRING_T || r == STRING_T) ? STRING_T :
                       getLargerNumberParseDataType(l, r);   
    
    case SUBTRACT_OP: return getLargerNumberParseDataType(l, r);
    default: return INVALID_T;
  }
}


ParseDataType getTypeBitLogicalExpression(ParseOperatorType op, ParseDataType l, ParseDataType r) {

  if(!typecheckBitLogicalExpression(op, l, r))
    return INVALID_T;

  if(isBitwiseParseOperatorType(op)) {
  
    return getLargerNumberParseDataType(l, r);

  } else if(isLogicalParseOperatorType(op)) {
  
    return BOOL_T;

  } else {

    return INVALID_T;

  }   
}

ParseDataType getTypeComparisonExpression(ParseOperatorType op, ParseDataType l, ParseDataType r) {

  if(!typecheckComparisonExpression(op, l, r))
    return INVALID_T;

  return BOOL_T;
}

