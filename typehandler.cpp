#include "parsetoken.h"
#include "typehandler.h"


////////////////////////////////////
///////       Typecheck      ///////
////////////////////////////////////

//typecheck expression appropriately depending on type of AbstractExpressionNode

bool typecheckUnaryExpression(ParseOperatorType op, ParseDataType arg) {
  
  switch(op) {
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
