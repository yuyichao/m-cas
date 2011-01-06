#include "all.h"

namespace CAS
{
  bool Expression::IfuAdd( Expression * expre )
  {
    if ( expre -> ExpType == Add
	 and ( expre -> NumOfPara != 1
	       or expre -> GetAttach(0) == -1 )) return true;
    if ( expre -> ExpType == Number
	 and ( expre -> Value.IsComplex()
	       or expre -> Value.RePart() < NumberType::Zero
	       or expre -> Value.ImPart() < NumberType::Zero )) return true;
    return false;
  };
  
  bool Expression::IfuMul( Expression * expre )
  {
    if ( IfuAdd( expre ) ) return true;
    if ( expre -> ExpType == Multiply
	 and ( expre -> NumOfPara != 1
	       or expre -> GetAttach(0) == -1 )) return true;
    if ( expre -> ExpType == Number
	 and !( expre -> Value.IsReal()
	       and expre -> Value.RePart() > NumberType::Zero )) return true;
    return false;
  };
  
  bool Expression::IfuPow( Expression * expre )
  {
    if ( IfuMul( expre ) ) return true;
    if ( expre -> ExpType == Power
	 and expre -> Parameter[0] -> ExpType != E
	 and !( expre -> Parameter[1] -> ExpType == Number
		and expre -> GetAttach( 0 ) == 1
		and (( expre -> GetAttach(1) == -1
		       and expre -> Parameter[1] -> Value == 2 )
		     or ( expre -> GetAttach(1) == 1
			  and expre -> Parameter[1] -> Value == 0.5 ))))
      return true;
    return false;
  };
  
  string Expression::ToString()
  {
    string str;
    switch ( ExpType )
      {
      case None:
      case Str:
	return Name;
      case Number:
	return Value.ToString();
      case Pi:
	return "Pi";
      case E:
	return "E";
      case True:
	return "True";
      case False:
	return "False";
      case Add:
	if ( GetAttach(0) != 1 )
	  str += "-";
	for ( int i = 0 ; i < NumOfPara - 1 ; i++ )
	  {
	    if ( IfuAdd( Parameter[i] ) )
	      str += "(" + Parameter[i] -> ToString() + ")";
	    else
	      str += Parameter[i] -> ToString();
	    if ( GetAttach( i + 1 ) == 1 )
	      str += "+";
	    else
	      str += "-";
	  }
	if ( IfuAdd( Parameter[ NumOfPara - 1 ] ) )
	  str += "(" + Parameter[ NumOfPara - 1 ] -> ToString() + ")";
	else
	  str += Parameter[ NumOfPara - 1 ] -> ToString();
	break;
      case Multiply:
	if ( GetAttach(0) != 1 )
	  str += "1/";
	for ( int i = 0 ; i < NumOfPara - 1 ; i++ )
	  {
	    if ( IfuMul( Parameter[i] ) )
	      str += "(" + Parameter[i] -> ToString() + ")";
	    else
	      str += Parameter[i] -> ToString();
	    if ( GetAttach( i + 1 ) == 1 )
	      str += "*";
	    else
	      str += "/";
	  }
	if ( IfuMul( Parameter[ NumOfPara - 1 ] ) )
	  str += "(" + Parameter[ NumOfPara - 1 ] -> ToString() + ")";
	else
	  str += Parameter[ NumOfPara - 1 ] -> ToString();
	break;
      case Power:
	if ( GetAttach(0) == 1 and Parameter[1] -> ExpType == Number
	     and (( GetAttach(1) == -1 and Parameter[1] -> Value == 2 )
		  or ( GetAttach(1) == 1 and Parameter[1] -> Value == 0.5 )))
	  return "Sqrt[" + Parameter[0] -> ToString() + "]";
	if ( Parameter[0] -> ExpType == E )
	  {
	    str += "Exp[";
	    if ( GetAttach( 0 ) == 1 )
	      if ( GetAttach( 1 ) == 1 )
		str += Parameter[1] -> ToString();
	      else
		if ( IfuMul( Parameter[1] ) )
		  str += "1/(" + Parameter[1] -> ToString() + ")";
		else
		  str += "1/" + Parameter[1] -> ToString();
	    else
	      if ( GetAttach( 1 ) == 1 )
		if ( IfuAdd( Parameter[1] ) )
		  str += "-(" + Parameter[1] -> ToString() + ")";
		else
		  str += "-" + Parameter[1] -> ToString();
	      else
		if ( IfuMul( Parameter[1] ) )
		  str += "1/(" + Parameter[1] -> ToString() + ")";
		else
		  str += "1/" + Parameter[1] -> ToString();
	    str += "]";
	    break;
	  }
	else
	  {
	    if ( IfuPow( Parameter[0] ) )
	      str += "(" + Parameter[0] -> ToString() + ")";
	    else
	      str += Parameter[0] -> ToString();
	    str += "^";
	    if ( GetAttach(0) == -1 )
	      if ( GetAttach(1) == -1 )
		if ( IfuMul( Parameter[1] ) )
		  str += "(-1/(" + Parameter[1] -> ToString() + "))";
		else
		  str += "(-1/" + Parameter[1] -> ToString() + ")";
	      else
		if ( IfuAdd( Parameter[1] ) )
		  str += "(-(" + Parameter[1] -> ToString() + "))";
		else
		  str += "(-" + Parameter[1] -> ToString() + ")";
	    else
	      if ( GetAttach(1) == -1 )
		if ( IfuMul( Parameter[1] ) )
		  str += "(1/(" + Parameter[1] -> ToString() + "))";
		else
		  str += "(1/" + Parameter[1] -> ToString() + ")";
	      else
		if ( IfuMul( Parameter[1] ) )
		  str += "(" + Parameter[1] -> ToString() + ")";
		else
		  str += Parameter[1] -> ToString();
	  }
	break;
      case List:
	str += "{";
	for ( int i = 0 ; i < NumOfPara - 1 ; i++ )
	  {
	    str += Parameter[i] -> ToString() + ",";
	  }
	str += Parameter[ NumOfPara - 1 ] -> ToString() + "}";
	break;
      case UsrDefine:
	str += Name + "[";
	goto CommonFunction;
      case D:
	str += "D[";
	goto CommonFunction;
      case N:
	str += "N[";
	goto CommonFunction;
      case Integrate:
	str += "Integrate[";
	goto CommonFunction;
      CommonFunction:
	for ( int i = 0 ; i < NumOfPara - 1 ; i++ )
	  str += Parameter[i] -> ToString() + ",";
	str += Parameter[ NumOfPara - 1 ] -> ToString() + "]";
	break;
      case Sin:
	str += "Sin";
	goto Common;
      case Cos:
	str += "Cos";
	goto Common;
      case Tan:
	str += "Tan";
	goto Common;
      case Cot:
	str += "Cot";
	goto Common;
      case Sec:
	str += "Sec";
	goto Common;
      case Csc:
	str += "Csc";
	goto Common;
      case ArcSin:
	str += "ArcSin";
	goto Common;
      case ArcCos:
	str += "ArcCos";
	goto Common;
      case ArcTan:
	str += "ArcTan";
	goto Common;
      case ArcCot:
	str += "ArcCot";
	goto Common;
      case ArcSec:
	str += "ArcSec";
	goto Common;
      case ArcCsc:
	str += "ArcCsc";
	goto Common;
      case Log:
	if ( NumOfPara == 1 )
	  {
	    str += "Log";
	    goto Common;
	  }
	str += "Log[" + Parameter[0] -> ToString() 
	  + "," + Parameter[1] -> ToString() + "]";
	break;
      Common:
	str += "[" + Parameter[0] -> ToString() + "]";
	break;
      case To:
	return Parameter[0] -> ToString() + "->" + Parameter[1] -> ToString();
      case Diff:
	return "<" + Parameter[0] -> ToString() + "d(" + Parameter[1] -> ToString() + ")>";
      case Change:
      case Set:
	return "";
      case Equal:
	return Parameter[0] -> ToString() + "==" + Parameter[1] -> ToString();
      case UnEqual:
	return Parameter[0] -> ToString() + "!=" + Parameter[1] -> ToString();
      };
    return str;
  };
}
