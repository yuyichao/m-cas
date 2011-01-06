#include "all.h"

namespace CAS
{
  bool Expression::IsFunctionName( string name )
  {
    if ( name == "Add" ) return true;
    if ( name == "Multiply" ) return true;
    if ( name == "Power" ) return true;
    if ( name == "Sqrt" ) return true;
    if ( name == "Sin" ) return true;
    if ( name == "Cos" ) return true;
    if ( name == "Tan" ) return true;
    if ( name == "Cot" ) return true;
    if ( name == "Sec" ) return true;
    if ( name == "Csc" ) return true;
    if ( name == "ArcSin" ) return true;
    if ( name == "ArcCos" ) return true;
    if ( name == "ArcTan" ) return true;
    if ( name == "ArcCot" ) return true;
    if ( name == "ArcSec" ) return true;
    if ( name == "ArcCsc" ) return true;
    if ( name == "Exp" ) return true;
    if ( name == "Log" ) return true;
    if ( name == "D" ) return true;
    if ( name == "N" ) return true;
    if ( name == "Integrate" ) return true;
    if ( name == "Set" ) return true;
    if ( name == "Equal" ) return true;
    if ( name == "UnEqual" ) return true;
    return false;
  };

  bool Expression::IsConstName( string name )
  {
    if ( name == "Pi" ) return true;
    if ( name == "I" ) return true;
    if ( name == "E" ) return true;
    return false;
  };
  
  Expression* Expression::TrConst( Expression* expre , int length , ReplaceChain* )
  {
    Expression* result;
    if ( expre -> Name == "Pi" )
      {
	if ( length <= 0 ) 
	  {
	    result = make( Pi , 0 );
	    expre -> detach();
	    return result;
	  }
	result = make( PrimaryFunction::ValueOfPi( length ) );
	expre -> detach();
	return result;
      }
    if ( expre -> Name == "E" )
      {
	if ( length <= 0 ) 
	  {
	    result = make( E , 0 );
	    expre -> detach();
	    return result;
	  }
	result = make( PrimaryFunction::ValueOfE( length ) );
	expre -> detach();
	return result;
      }
    if ( expre -> Name == "I" )
      {
	result = make( NumberType::I );
	expre -> detach();
	return result;
      }
    return NULL;
  };
  
  Expression* Expression::TrC( Expression* expre , int length , ReplaceChain* )
  {
    Expression* result;
    if ( expre -> ExpType == Pi )
      {
	if ( length <= 0 )
	  return expre;
	result = make( PrimaryFunction::ValueOfPi( length ) );
	expre -> detach();
	return result;
      }
    if ( expre -> ExpType == E )
      {
	if ( length <= 0 )
	  return expre;
	result = make( PrimaryFunction::ValueOfE( length ) );
	expre -> detach();
	return result;
      }
    return NULL;
  };

  Expression * Expression::TrStr( Expression * expre , int length , ReplaceChain * condition )
  {
    if (IsConstName( expre -> Name )) return TrConst( expre , length , condition );
    Replacement replace = condition -> Find( expre -> Name );
    if ( replace.IsEmpty() ) return expre;
    if ( replace.GetParaNum() != 0 or IsFunctionName( expre -> Name ) )
      throw "Invalid Use Of Function Name: " + expre -> Name;
    Expression * result = replace.GetResult();
    expre -> detach();
    return result;
  };
  
  bool Expression::CheckArgDiff( Expression * expre )
  {
    if ( expre -> NumOfPara != 2 ) return false;
    for ( int i = 0 ; i < 2 ; i++ )
      switch ( expre -> P(i) -> ExpType )
	{
	case None:
	case True:
	case False:
	case To:
	case Change:
	case Set:
	case Equal:
	case UnEqual:
	  return false;
	default:
	  break;
	};
    return true;
  };

  Expression *Expression::TrDiff( Expression *expre , int length
				  , ReplaceChain* condition )
  {
    if ( !CheckArgDiff( expre ) ) throw "Invalid Input For Diff";
    if ( expre -> P(1) -> ExpType == Diff )
      {
	Expression * result = make( Diff , 2 );
	( result -> P(0) = expre -> P(0) ) -> attach();
	( result -> P(1) = expre -> P(1) -> P(0) ) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      }
    if ( expre -> P(0) -> ExpType == Diff )
      {
	Expression * result = make( Diff , 2 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> P(1) = make( Add , 2 );
	( result -> P(1) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	( result -> P(1) -> P(1) = expre -> P(1) ) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      }
    return expre;
  };
  
  Expression * Expression::TrList( Expression * expre , int length
				   , ReplaceChain * condition )
  {
    if ( expre -> NumOfPara == 1 )
      {
        Expression * result = expre -> P(0);
	result -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      }
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == Diff )
	  {
	    Expression * result = make( Diff , 2 );
	    result -> P(0) = make ( List , expre -> NumOfPara );
	    result -> P(1) = make ( List , expre -> NumOfPara );
	    for ( int j = 0 ; j < expre -> NumOfPara ; j++ )
	      {
		if ( expre -> P(j) -> ExpType == Diff )
		  {
		    ( result -> P(0) -> P(j) = expre -> P(j) -> P(0) ) -> attach();
		    ( result -> P(1) -> P(j) = expre -> P(j) -> P(1) ) -> attach();
		  }
		else
		  {
		    ( result -> P(0) -> P(j) = expre -> P(j) ) -> attach();
		    result -> P(1) -> P(j) = make( NumberType::Zero );
		  }
	      }
	    expre -> detach();
	    return Transform( result , length , condition );
	  }
      }
    return expre;
  };
  
  Expression * Expression::TrUsrDefine( Expression * expre , int length
					, ReplaceChain * condition )
  {
    Replacement replace = condition -> Find( expre -> Name );
    if ( replace.IsEmpty() ) return expre;
    if ( IsConstName( expre -> Name ) )
      throw "Invalid Use Of Const Name: " + expre -> Name;
    if ( replace.GetParaNum() != expre -> NumOfPara )
      throw "Number Of Para Of Function " + expre -> Name + " Doesn't Match.";
    Expression * tmp = replace.GetResult( expre -> Parameter );
    expre -> detach();
    return Transform( tmp , length , condition );
  };

  bool Expression::CheckArgNormal( Expression * expre )
  {
    if ( expre -> NumOfPara < 1 ) return false;
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	switch ( expre -> P(i) -> ExpType )
	  {
	  case None:
	  case True:
	  case False:
	  case To:
	  case Change:
	  case Set:
	  case Equal:
	  case UnEqual:
	    return false;
	  default:
	    break;
	  };
      }
    return true;
  };

  bool Expression::CheckArgFunction( Expression * expre )
  {
    if ( expre -> NumOfPara != 1 ) return false;
    switch ( expre -> P(0) -> ExpType )
      {
      case None:
      case True:
      case False:
      case To:
      case Change:
      case Set:
      case Equal:
      case UnEqual:
	return false;
      default:
	break;
      };
    return true;
  };
  
  bool Expression::OneAdd( Expression *expre , Expression *&result , int length
			   , ReplaceChain * condition )
  {
    if ( expre -> NumOfPara == 1 )
      {
	switch ( expre -> P(0) -> ExpType )
	  {
	  case Number:
	    if ( expre -> GetAttach(0) == 1 )
	      expre -> Value = expre -> P(0) -> Value;
	    else
	      expre -> Value = -( expre -> P(0) -> Value );
	    goto Common;
	  case Str:
	    if ( expre -> GetAttach(0) == 1 )
	      expre -> Name = expre -> P(0) -> Name;
	    else
	      return false;
	    goto Common;
	  case Pi:
	    if ( expre -> GetAttach(0) == 1 )
	      goto Common;
	    return false;
	  case None:
	  Common:
	    expre -> ExpType = expre -> P(0) -> ExpType;
	    expre -> P(0) -> detach();
	    delete []expre -> Parameter;
	    delete []expre -> Attach;
	    expre -> Parameter = NULL;
	    expre -> Attach = NULL;
	    expre -> NumOfPara = 0;
	    result = expre;
	    return true;
	  default:
	    if ( expre -> GetAttach(0) == -1 )
	      return false;
	    break;
	  };
	( result = expre -> P(0) ) -> attach();
	expre -> detach();
	result = Transform( result , length , condition );
	return true;
      }
    return false;
  };

  bool Expression::AddSim( Expression *expre , Expression *&result , int length
			   , ReplaceChain * condition )
  {
    for ( int i = 1 ; i < expre -> NumOfPara ; i++ )
      {
	for ( int j = 0 ; j < i ; j++ )
	  {
	    bool sim = false;
	    Expression ** tmp1;
	    NumberType a;
	    int * att , loftmp , att2;
	    if ( *( expre -> P(i) ) == *( expre -> P(j) ) )
	      {
		sim = true;
		tmp1 = new Expression*[1];
		tmp1[0] = expre -> P(j);
		loftmp = 1;
		if ( expre -> GetAttach( i ) == expre -> GetAttach( j ) )
		  a = 2;
		else
		  a = 0;
		att = new int[1];
		att[0] = 1;
		att2 = expre -> Attach == NULL ? 1 : expre -> Attach[i];
		goto SimTrCom;
	      }
	    if ( expre -> P(i) -> ExpType == Multiply 
		 and expre -> P(i) -> NumOfPara == 2
		 and expre -> P(i) -> P(0) -> ExpType == Number
		 and expre -> P(i) -> GetAttach(1) == 1
		 and ( *( expre -> P(i) -> P(1) ) 
		       ==  *( expre -> P(j) ) ))
	      {
		att = new int[1];
		att[0] = 1;
		sim = true;
		a = ( att2 = expre -> GetAttach( i ) ) * expre -> GetAttach( j );
		if ( expre -> P(i) -> GetAttach( 1 ) == 1 )
		  a = a + expre -> P(i) -> P(0) -> Value;
		else
		  a = a + expre -> P(i) -> P(0) 
		    -> Value.Inverse( length + 1 );
		tmp1 = new Expression*[1];
		tmp1[0] = expre -> P(j);
		loftmp = 1;
		goto SimTrCom;
	      }
	    if ( expre -> P(j) -> ExpType == Multiply 
		 and expre -> P(j) -> NumOfPara == 2
		 and expre -> P(j) -> P(0) -> ExpType == Number
		 and expre -> P(j) -> GetAttach(1) == 1
		 and ( *( expre -> P(j) -> P(1) ) 
		       ==  *( expre -> P(i) )))
	      {
		att = new int[1];
		sim = true;
		att[0] = 1;
		a = ( att2 = expre -> GetAttach( i ) ) * expre -> GetAttach( j );
		if ( expre -> P(j) -> GetAttach( 1 ) == 1 )
		  a = a + expre -> P(j) -> P(0) -> Value;
		else
		  a = a + expre -> P(j) -> P(0) -> Value.Inverse( length + 1 );
		tmp1 = new Expression*[1];
		tmp1[0] = expre -> P(i);
		loftmp = 1;
		goto SimTrCom;
	      }
	    if ( expre -> P(i) -> ExpType == Multiply 
		 and expre -> P(j) -> ExpType == Multiply )
	      {
		int k , l;
		if ( expre -> P(i) -> P(0) -> ExpType == Number )
		  k = 1;
		else
		  k = 0;
		if ( expre -> P(j) -> P(0) -> ExpType == Number )
		  l = 1;
		else
		  l = 0;
		if ( ( loftmp = expre -> P(i) -> NumOfPara - k )
		     == expre -> P(j) -> NumOfPara - l )
		  {
		    sim = true;
		    for ( int s = 0 ; s < loftmp ; s++ )
		      {
			if ( expre -> P(i) -> GetAttach( s + k )
			     != expre -> P(j) -> GetAttach( s + l )
			     or *( expre -> P(i) -> P( s + k ) ) 
			     != *( expre -> P(j) -> P( s + l ) ))
			  {
			    sim = false;
			    break;
			  }
		      }
		    if ( sim )
		      {
			att2 = 1;
			att = new int[loftmp];
			tmp1 = new Expression*[loftmp];
			for ( int s = 0 ; s < loftmp ; s++ )
			  {
			    att[s] = expre -> P(i) -> GetAttach( s + k );
			    tmp1[s] = expre -> P(i) -> P( s + k );
			  }
			if ( k )
			  {
			    if ( expre -> P(i) -> GetAttach(0) == 1 )
			      a = expre -> P(i) -> P(0) -> Value 
				* expre -> GetAttach( i );
			    else
			      a = expre -> P(i) -> P(0)
				-> Value.Inverse( length + 1 ) 
				* expre -> GetAttach( i );
			  }
			else
			  a =  expre -> GetAttach( i );
			if ( l )
			  {
			    if ( expre -> P(j) -> GetAttach(0) == 1 )
			      a = a 
				+ expre -> P(j) -> P(0) -> Value
				* expre -> GetAttach( j );
			    else
			      a = a
				+ expre -> P(j) -> P(0)
				-> Value.Inverse( length + 1 )
				* expre -> GetAttach( j );
			  }
			else
			  a = a + expre -> GetAttach( j );
			goto SimTrCom;
		      } // end of if ( sim )
		  } // end Num Equ
	      } // end of Sim Mul
	  SimTrCom:
	    if ( sim )
	      {
		result = make( Add , expre -> NumOfPara - 1 );
		for ( int s = 0 ; s < j ; s++ )
		  {
		    ( result -> P(s) = expre -> P(s) ) -> attach();
		    result -> Attach[s] = expre -> GetAttach( s );
		  }
		for ( int s = j + 1 ; s < i ; s++ )
		  {
		    ( result -> P( s - 1 ) = expre -> P(s) ) -> attach();
		    result -> Attach[ s - 1 ] = expre -> GetAttach( s );
		  }
		for ( int s = i + 1 ; s < expre -> NumOfPara ; s++ )
		  {
		    ( result -> P( s - 2 ) = expre -> P(s) ) -> attach();
		    result -> Attach[ s - 2 ] = expre -> GetAttach( s );
		  }
		result -> P(result->NumOfPara - 1) = make( Multiply , loftmp + 1 );
		result -> Attach[ result -> NumOfPara - 1 ] = att2;
		result -> P( result -> NumOfPara - 1 ) -> P(0) = make ( a );
		for ( int s = 1 ; s <= loftmp ; s++ )
		  {
		    ( result -> P( result -> NumOfPara - 1 ) -> P(s)
		      = tmp1[ s - 1 ] ) -> attach();
		    result -> P(result->NumOfPara - 1) -> Attach[s] = att[ s - 1 ];
		  }
		expre -> detach();
		delete []tmp1;
		delete []att;
		result = Transform( result , length , condition );
		return true;
	      }
	  }
      }
    return false;
  };
  
  bool Expression::AddZero( Expression *expre , Expression *&result , int length
			    , ReplaceChain * condition )
  {
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == Number
	     and expre -> P(i) -> Value.IsZero() )
	  {
	    result = make( Add , expre -> NumOfPara - 1 );
	    for ( int j = 0 ; j < i ; j++ )
	      {
		result -> P(j) = expre -> P(j);
		result -> Attach[j] = expre -> GetAttach(j);
		expre -> P(j) -> attach();
	      }
	    for ( int j = i + 1 ; j < expre -> NumOfPara ; j++ )
	      {
		result -> P( j - 1 ) = expre -> P(j);
		result -> Attach[ j - 1 ] = expre -> GetAttach(j);
		expre -> P(j) -> attach();
	      }
	    expre -> detach();
	    result = Transform( result , length , condition );
	    return true;
	  }
      }
    return false;
  };
  
  bool Expression::AddAdd( Expression *expre , Expression *&result , int length
			   , ReplaceChain * condition )
  {
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == Add )
	  {
	    result = make( Add , expre->NumOfPara + expre->P(i)->NumOfPara - 1 );
	    for ( int j = 0 ; j < i ; j++ )
	      {
		( result -> P(j) = expre -> P(j) ) -> attach();
		result -> Attach[j] = expre -> GetAttach(j);
	      }
	    for ( int j = i + 1 ; j < expre -> NumOfPara ; j++ )
	      {
		( result -> P( j - 1 ) = expre -> P(j) ) -> attach();
		result -> Attach[ j - 1 ] = expre -> GetAttach(j);
	      }
	    for ( int j = 0 ; j < expre -> P(i) -> NumOfPara ; j++ )
	      {
		(result->P(expre->NumOfPara + j - 1)=expre->P(i)->P(j))->attach();
		result -> Attach[ expre -> NumOfPara + j - 1 ] 
		  = expre -> P(i) -> GetAttach( j ) * expre -> GetAttach( i );
	      }
	    expre -> detach();
	    result = Transform( result , length , condition );
	    return true;
	  }
      }
    return false;
  };
  
  bool Expression::AddNumber( Expression *expre , Expression *&result , int length
			      , ReplaceChain * condition )
  {
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	for ( int j = 0 ; j < i ; j++ )
	  {
	    if ( expre -> P(i) -> ExpType == Number
		 and expre -> P(j) -> ExpType == Number )
	      {
		result = make( Add , expre -> NumOfPara - 1 );
		for ( int k = 0 ; k < j ; k++ )
		  {
		    ( result -> P(k) = expre -> P(k) ) -> attach();
		    result -> Attach[k] = expre -> GetAttach(k);
		  }
		NumberType tmp;
		if ( expre -> GetAttach(j) == 1 )
		  tmp = expre -> P(j) -> Value;
		else
		  tmp = - expre -> P(j) -> Value;
		if ( expre -> GetAttach(i) == 1 )
		  tmp = tmp + expre -> P(i) -> Value;
		else
		  tmp = tmp - expre -> P(i) -> Value;
		result -> P(j) = make( tmp );
		result -> Attach[j] = 1;
		for ( int k = j + 1 ; k < i ; k++ )
		  {
		    ( result -> P(k) = expre -> P(k) ) -> attach();
		    result -> Attach[k] = expre -> GetAttach(k);
		  }
		for ( int k = i + 1 ; k < expre -> NumOfPara ; k++ )
		  {
		    ( result -> P( k - 1 ) = expre -> P(k) ) -> attach();
		    result -> Attach[ k - 1 ] = expre -> GetAttach(k);
		  }
		expre -> detach();
		result = Transform( result , length , condition );
		return true;
	      }
	  }
      }
    return false;
  };
  
  bool Expression::AddSign( Expression *expre , Expression *&result , int length
			    , ReplaceChain * condition )
  {
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == Number
	     and ( expre -> P(i) -> Value.RePart() < NumberType::Zero 
		   or ( expre -> P(i) -> Value.RePart().IsZero()
			and expre -> P(i) -> Value.ImPart() < NumberType:: Zero )))
	  {
	    if ( expre -> Attach == NULL )
	      {
		expre -> Attach = new int[ expre -> NumOfPara ];
		for ( int j = 0 ; j < expre -> NumOfPara ; j++ )
		  expre -> Attach[j] = 1;
	      }
	    expre -> P(i) -> Value = - expre -> P(i) -> Value;
	    expre -> Attach[i] = - expre -> Attach[i];
	    result = Transform( expre , length , condition );
	    return true;
	  }
      }
    return false;
  };
  
  bool Expression::AddDiff( Expression *expre , Expression *&result , int length
			    , ReplaceChain * condition )
  {
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == Diff )
	  {
	    result = make( Diff , 2 );
	    result -> P(0) = make( Add , expre -> NumOfPara );
	    for ( int k = 0 ; k < i ; k++ )
	      {
		( result -> P(0) -> P(k) = expre -> P(k) ) -> attach();
		result -> P(0) -> Attach[k] = expre -> GetAttach( k );
	      }
	    for ( int k = i + 1 ; k < expre -> NumOfPara ; k++ )
	      {
		( result -> P(0) -> P(k) = expre -> P(k) ) -> attach();
		result -> P(0) -> Attach[k] = expre -> GetAttach( k );
	      }
	    ( result -> P(0) -> P(i) = expre -> P(i) -> P(0) ) -> attach();
	    result -> P(0) -> Attach[i] = expre -> GetAttach( i );
	    if ( expre -> GetAttach( i ) == 1 )
	      ( result -> P(1) = expre -> P(i) -> P(1) )
		-> attach();
	    else
	      {
		result -> P(1) = make( Add , 1 );
		result -> P(1) -> Attach[0] = -1;
		( result -> P(1) -> P(0) = expre -> P(i) -> P(1) ) -> attach();
	      }
	    expre -> detach();
	    result = Transform( result , length , condition );
	    return true;
	  }
      }
    return false;
  };

  bool Expression::AddList( Expression *expre , Expression *&result , int length1 , ReplaceChain * condition )
  {
    int length;
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == List
	     and ( length = expre -> P(i) -> NumOfPara ) >= 1 )
	  goto HaveList;
      }
    return false;
  HaveList:
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == List
	     and expre -> P(i) -> NumOfPara != 1 
	     and expre -> P(i) -> NumOfPara != length )
	  throw "Length of Lists must be the same.";
      }
    result = make( List , length );
    for ( int i = 0 ; i < length ; i++ )
      {
	result -> P(i) = make( Add , expre -> NumOfPara );
	for ( int j = 0 ; j < expre -> NumOfPara ; j++ )
	  {
	    if ( expre -> P(j) -> ExpType != List )
	      {
		( result -> P(i) -> P(j) = expre -> P(j) ) -> attach();
		result -> P(i) -> Attach[j] = expre -> GetAttach( j );
		continue;
	      }
	    if ( expre -> P(j) -> NumOfPara == 1 )
	      {
		( result -> P(i) -> P(j) = expre -> P(j) -> P(0) ) -> attach();
		result -> P(i) -> Attach[j] = expre -> GetAttach( j );
		continue;
	      }
	    ( result -> P(i) -> P(j) = expre -> P(j) -> P(i) ) -> attach();
	    result -> P(i) -> Attach[j] = expre -> GetAttach( j );
	  }
      }
    expre -> detach();
    result = Transform( result , length1 , condition );
    return true;
  };

  Expression * Expression::TrAdd( Expression * expre , int length
				  , ReplaceChain * condition )
  {
    if ( !CheckArgNormal( expre ) ) throw "Invalid Input For Add";
    Expression * result;
    if ( OneAdd( expre , result , length , condition ) ) return result;
    if ( expre -> Attach != NULL )
      SortUp( expre -> Parameter , expre -> Attach , expre -> NumOfPara );
    else
      SortUp( expre -> Parameter , expre -> NumOfPara );
    if ( AddZero( expre , result , length , condition ) ) return result;
    if ( AddAdd( expre , result , length , condition ) ) return result;
    if ( AddNumber( expre , result , length , condition ) ) return result;
    if ( AddSim( expre , result , length , condition ) ) return result;
    if ( AddSign( expre , result , length , condition ) ) return result;
    if ( AddDiff( expre , result , length , condition ) ) return result;
    if ( AddList( expre , result , length , condition ) ) return result;
    return expre;
  };
  
  bool Expression::OneMul( Expression *expre , Expression *&result , int length
			   , ReplaceChain * condition )
  {
    if ( expre -> NumOfPara == 1 )
      {
	switch ( expre -> P(0) -> ExpType )
	  {
	  case Number:
	    if ( expre -> GetAttach(0) == 1 )
	      expre -> Value = expre -> P(0) -> Value;
	    else
	      expre -> Value = expre -> P(0) -> Value.Inverse( length + 1 );
	    goto Common;
	  case Str:
	    if ( expre -> GetAttach(0) == 1 )
	      expre -> Name = expre -> P(0) -> Name;
	    else
	      return false;
	    goto Common;
	  case Pi:
	    if ( expre -> GetAttach(0) == 1 )
	      goto Common;
	    return false;
	  case None:
	  Common:
	    expre -> ExpType = expre -> P(0) -> ExpType;
	    expre -> P(0) -> detach();
	    delete []expre -> Parameter;
	    delete []expre -> Attach;
	    expre -> Parameter = NULL;
	    expre -> Attach = NULL;
	    expre -> NumOfPara = 0;
	    result = expre;
	    return true;
	  default:
	    if ( expre -> Attach != NULL and expre -> Attach[0] != 1 )
	      return false;
	    break;
	  };
	( result = expre -> P(0) ) -> attach();
	expre -> detach();
	result = Transform( result , length , condition );
	return true;
      }
    return false;
  };
  
  bool Expression::MulZero( Expression *expre , Expression *&result
			    , int, ReplaceChain* )
  {
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == Number )
	  {
	    if (( expre -> P(i) -> Value.IsZero()
		  and expre -> GetAttach(i) == 1 )
		or ( expre -> P(i) -> Value.IsInfinity()
		  and expre -> GetAttach(i) == -1 ))
	      {
		for ( int j = i + 1 ; j < expre -> NumOfPara ; j++ )
		  {
		    if (( expre -> P(i) -> Value.IsZero()
			  and expre -> GetAttach(i) == -1 )
			or ( expre -> P(i) -> Value.IsInfinity()
			     and expre -> GetAttach(i) == 1 ))
		      {
			expre -> detach();
			result = make( NumberType::NaN );
			return true;
		      }
		  }
		expre -> detach();
		result = make( NumberType::Zero );
		return true;
	      }
	    if (( expre -> P(i) -> Value.IsZero()
		  and expre -> GetAttach(i) == -1 )
		or ( expre -> P(i) -> Value.IsInfinity()
		  and expre -> GetAttach(i) == 1 ))
	      {
		for ( int j = i + 1 ; j < expre -> NumOfPara ; j++ )
		  {
		    if (( expre -> P(i) -> Value.IsZero()
			  and expre -> GetAttach(i) == 1 )
			or ( expre -> P(i) -> Value.IsInfinity()
			     and expre -> GetAttach(i) == -1 ))
		      {
			expre -> detach();
			result = make( NumberType::NaN );
			return true;
		      }
		  }
		expre -> detach();
		result = make( NumberType::RealInfinity );
		return true;
	      }
	  }
      }
    return false;
  };
  
  bool Expression::MulMul( Expression *expre , Expression *&result , int length
			   , ReplaceChain * condition )
  {
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == Multiply )
	  {
	    result = make( Multiply , expre->NumOfPara+expre->P(i)->NumOfPara-1 );
	    for ( int j = 0 ; j < i ; j++ )
	      {
		( result -> P(j) = expre -> P(j) ) -> attach();
		result -> Attach[j] = expre -> GetAttach(j);
	      }
	    for ( int j = i + 1 ; j < expre -> NumOfPara ; j++ )
	      {
		( result -> P( j - 1 ) = expre -> P(j) ) -> attach();
		result -> Attach[ j - 1 ] = expre -> GetAttach(j);
	      }
	    for ( int j = 0 ; j < expre -> P(i) -> NumOfPara ; j++ )
	      {
		( result -> P( expre -> NumOfPara + j - 1 )
		  = expre -> P(i) -> P(j) ) -> attach();
		result -> Attach[ expre -> NumOfPara + j - 1 ] 
		  = expre -> P(i) -> GetAttach(j) * expre -> GetAttach(i);
	      }
	    expre -> detach();
	    result = Transform( result , length , condition );
	    return true;
	  }
      }
    return false;
  };
  
  bool Expression::MulNumber( Expression *expre , Expression *&result , int length 
			      , ReplaceChain * condition )
  {
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	for ( int j = 0 ; j < i ; j++ )
	  {
	    if ( expre -> P(i) -> ExpType == Number
		 and expre -> P(j) -> ExpType == Number )
	      {
		result = make( Multiply , expre -> NumOfPara - 1 );
		for ( int k = 0 ; k < j ; k++ )
		  {
		    ( result -> P(k) = expre -> P(k) ) -> attach();
		    result -> Attach[k] = expre -> GetAttach(k);
		  }
		NumberType tmp;
		if ( expre -> GetAttach(j) == 1 )
		  tmp = expre -> P(j) -> Value;
		else
		  tmp = expre -> P(j) -> Value.Inverse( length + 1 );
		if ( expre -> GetAttach(i) == 1 )
		  tmp = tmp * expre -> P(i) -> Value;
		else
		  tmp = NumberType::Divide( tmp , expre->P(i)->Value , length+1 );
		result -> P(j) = make( tmp );
		result -> Attach[j] = 1;
		for ( int k = j + 1 ; k < i ; k++ )
		  {
		    ( result -> P(k) = expre -> P(k) ) -> attach();
		    result -> Attach[k] = expre -> GetAttach(k);
		  }
		for ( int k = i + 1 ; k < expre -> NumOfPara ; k++ )
		  {
		    ( result -> P( k - 1 ) = expre -> P(k) ) -> attach();
		    result -> Attach[ k - 1 ] = expre -> GetAttach(k);
		  }
		expre -> detach();
		result = Transform( result , length , condition );
		return true;
	      }
	  }
      }
    return false;
  };
  
  bool Expression::MulOne( Expression *expre , Expression *&result , int length
			   , ReplaceChain * condition )
  {
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == Number
	     and expre -> P(i) -> Value == 1 )
	  {
	    result = make( Multiply , expre -> NumOfPara - 1 );
	    for ( int j = 0 ; j < i ; j++ )
	      {
		result -> P(j) = expre -> P(j);
		result -> Attach[j] = expre -> GetAttach(j);
		expre -> P(j) -> attach();
	      }
	    for ( int j = i + 1 ; j < expre -> NumOfPara ; j++ )
	      {
		result -> P( j - 1 ) = expre -> P(j);
		result -> Attach[ j - 1 ] = expre -> GetAttach(j);
		expre -> P(j) -> attach();
	      }
	    expre -> detach();
	    result = Transform( result , length , condition );
	    return true;
	  }
      }
    return false;
  };
  
  bool Expression::MulList( Expression *expre , Expression *&result , int length1 , ReplaceChain * condition )
  {
    int length;
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == List
	     and ( length = expre -> P(i) -> NumOfPara ) >= 1 )
	  goto HaveList;
      }
    return false;
  HaveList:
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == List
	     and expre -> P(i) -> NumOfPara != 1 
	     and expre -> P(i) -> NumOfPara != length )
	  throw "Length of Lists must be the same.";
      }
    result = make( List , length );
    for ( int i = 0 ; i < length ; i++ )
      {
	result -> P(i) = make( Multiply , expre -> NumOfPara );
	for ( int j = 0 ; j < expre -> NumOfPara ; j++ )
	  {
	    if ( expre -> P(j) -> ExpType != List )
	      {
		( result -> P(i) -> P(j) = expre -> P(j) ) -> attach();
		result -> P(i) -> Attach[j] = expre -> GetAttach(j);
		continue;
	      }
	    if ( expre -> P(j) -> NumOfPara == 1 )
	      {
		( result -> P(i) -> P(j) = expre -> P(j) -> P(0) ) -> attach();
		result -> P(i) -> Attach[j] = expre -> GetAttach(j);
		continue;
	      }
	    ( result -> P(i) -> P(j) = expre -> P(j) -> P(i) ) -> attach();
	    result -> P(i) -> Attach[j] = expre -> GetAttach(j);
	  }
      }
    expre -> detach();
    result = Transform( result , length1 , condition );
    return true;
  };
  
  bool Expression::MulDiff( Expression *expre , Expression *&result , int length
			    , ReplaceChain * condition )
  {
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == Diff )
	  {
	    result = make( Diff , 2 );
	    result -> P(0) = make( Multiply , expre -> NumOfPara );
	    result -> P(1) = make( Multiply , expre -> NumOfPara );
	    for ( int j = 0 ; j < i ; j++ )
	      {
		( result -> P(0) -> P(j) = expre -> P(j) ) -> attach();
		( result -> P(1) -> P(j) = expre -> P(j) ) -> attach();
		result -> P(0) -> Attach[j]
		  = result -> P(1) -> Attach[j] = expre -> GetAttach(j);
	      }
	    if ( expre -> GetAttach(i) == 1 )
	      {
		( result -> P(0) -> P(i) = expre -> P(i) -> P(0) ) -> attach();
		( result -> P(1) -> P(i) = expre -> P(i) -> P(1) ) -> attach();
	      }
	    else
	      {
		( result -> P(0) -> P(i) = expre -> P(i) -> P(0) ) -> attach();
		result -> P(0) -> Attach[i] = -1;
		result -> P(1) -> P(i) = make( Multiply , 4 );
		result -> P(1) -> P(i) -> P(0) = make( -NumberType::One );
		( result -> P(1) -> P(i) -> P(1) = expre->P(i)->P(1) ) -> attach();
		( result -> P(1) -> P(i) -> P(2) = expre->P(i)->P(0) ) -> attach();
		result -> P(1) -> P(i) -> Attach[2] = -1;
		( result -> P(1) -> P(i) -> P(3) = expre->P(i)->P(0) ) -> attach();
		result -> P(1) -> P(i) -> Attach[3] = -1;
	      }
	    for ( int j = i+ 1 ; j < expre -> NumOfPara ; j++ )
	      {
		( result -> P(0) -> P(j) = expre -> P(j) ) -> attach();
		( result -> P(1) -> P(j) = expre -> P(j) ) -> attach();
		result -> P(0) -> Attach[j]
		  = result -> P(1) -> Attach[j] = expre -> GetAttach(j);
	      }
	    expre -> detach();
	    result = Transform( result , length , condition );
	    return true;
	  }
      }
    return false;
  };
  
  bool Expression::MulSim( Expression *expre , Expression *&result , int length
			   , ReplaceChain * condition )
  {for ( int i = 1 ; i < expre -> NumOfPara ; i++ )
      {
	for ( int j = 0 ; j < i ; j++ )
	  {
	    Expression * exp[2];
	    bool sim = false;
	    if ( *( expre -> P(i) ) == *( expre -> P(j) ) )
	      {
		sim = true;
		( exp[0] = expre -> P(i) ) -> attach();
		exp[1] = make(NumberType(expre->GetAttach(i)+expre->GetAttach(j)));
		goto SimTrCom;
	      }
	    if ( expre -> P(i) -> ExpType == Power
		 and *( expre -> P(j) ) == *( expre -> P(i) -> P(0) ) )
	      {
		sim = true;
		( exp[0] = expre -> P(j) ) -> attach();
		exp[1] = make( Add , 2 );
		exp[1] -> Attach[0] = expre -> GetAttach(i)
		  * expre -> P(i) -> GetAttach( 0 );
		exp[1] -> Attach[1] = expre -> GetAttach(j);
		if ( expre -> P(i) -> GetAttach(1) == 1 )
		  ( exp[1] -> P(0) = expre -> P(i) -> P(1) ) -> attach();
		else
		  {
		    exp[1] -> P(0) = make( Multiply , 1 );
		    exp[1] -> P(0) -> Attach[0] = -1;
		    ( exp[1] -> P(0) -> P(0) = expre -> P(i) -> P(1) ) -> attach();
		  }
		exp[1] -> P(1) = make( NumberType::One );
		goto SimTrCom;
	      }
	    if ( expre -> P(j) -> ExpType == Power
		 and *( expre -> P(i) ) == *( expre -> P(j) -> P(0) ) )
	      {
		sim = true;
		( exp[0] = expre -> P(i) ) -> attach();
		exp[1] = make( Add , 2 );
		exp[1] -> Attach[0] = expre -> GetAttach(j)
		  * expre -> P(j) -> GetAttach(0);
		exp[1] -> Attach[1] = expre -> GetAttach( i );
		if ( expre -> P(j) -> GetAttach( 1 ) == 1 )
		  ( exp[1] -> P(0) = expre -> P(j) -> P(1) ) -> attach();
		else
		  {
		    exp[1] -> P(0) = make( Multiply , 1 );
		    exp[1] -> P(0) -> Attach[0] = -1;
		    ( exp[1] -> P(0) -> P(0) = expre -> P(j) -> P(1) ) -> attach();
		  }
		exp[1] -> P(1) = make( NumberType::One );
		goto SimTrCom;
	      }
	    if ( expre -> P(i) -> ExpType == Power
		 and expre -> P(j) -> ExpType == Power
		 and *( expre -> P(i) -> P(0) ) == *( expre -> P(j) -> P(0) ))
	      {
		sim = true;
		( exp[0] = expre -> P(i) -> P(0) ) -> attach();
		exp[1] = make( Add , 2 );
		exp[1] -> Attach[0] = expre -> GetAttach(i)
		  * expre -> P(i) -> GetAttach(0);
		exp[1] -> Attach[1] = expre -> GetAttach(j)
		  * expre -> P(j) -> GetAttach(0);
		if ( expre -> P(i) -> GetAttach(1) == 1 )
		  ( exp[1] -> P(0) = expre -> P(i) -> P(1) ) -> attach();
		else
		  {
		    exp[1] -> P(0) = make( Multiply , 1 );
		    exp[1] -> P(0) -> Attach[0] = -1;
		    ( exp[1] -> P(0) -> P(0) = expre -> P(i) -> P(1) ) -> attach();
		  }
		if ( expre -> P(j) -> GetAttach(1) == 1 )
		  ( exp[1] -> P(1) = expre -> P(j) -> P(1) ) -> attach();
		else
		  {
		    exp[1] -> P(1) = make( Multiply , 1 );
		    exp[1] -> P(1) -> Attach[0] = -1;
		    ( exp[1] -> P(1) -> P(0) = expre -> P(j) -> P(1) ) -> attach();
		  }
		goto SimTrCom;
	      }
	  SimTrCom:
	    if ( sim )
	      {
		result = make( Multiply , expre -> NumOfPara - 1 );
		for ( int s = 0 ; s < j ; s++ )
		  {
		    result -> P(s) = expre -> P(s);
		    result -> Attach[s] = expre -> GetAttach( s );
		    expre -> P(s) -> attach();
		  }
		for ( int s = j + 1 ; s < i ; s++ )
		  {
		    result -> P( s - 1 ) = expre -> P(s);
		    result -> Attach[ s - 1 ] = expre -> GetAttach(s);
		    expre -> P(s) -> attach();
		  }
		for ( int s = i + 1 ; s < expre -> NumOfPara ; s++ )
		  {
		    result -> P( s - 2 ) = expre -> P(s);
		    result -> Attach[ s - 2 ] = expre -> GetAttach(s);
		    expre -> P(s) -> attach();
		  }
		result -> P( result -> NumOfPara - 1 ) = make( Power , 2 );
		result -> P( result -> NumOfPara - 1 ) -> P(0) = exp[0];
		result -> P( result -> NumOfPara - 1 ) -> P(1) = exp[1];
		expre -> detach();
		result = Transform( result , length , condition );
		return true;
	      }
	  }
      }
    return false;
  };
  
  bool Expression::MulSign( Expression *expre , Expression *&result , int length
			    , ReplaceChain * condition )
  {
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType != Power ) continue;
	if ( expre -> GetAttach(i) == -1 )
	  {
	    if ( expre -> P(i) -> Attach == NULL )
	      {
		expre -> P(i) -> Attach = new int[2];
		expre -> P(i) -> Attach[0] = 1;
		expre -> P(i) -> Attach[1] = 1;
	      }
	    expre -> Attach[i] = 1;
	    expre -> P(i) -> Attach[0] = -expre -> P(i) -> Attach[0];
	    result = Transform( expre , length , condition );
	    return true;
	  }
      }
    return false;
  };

  bool Expression::MulSigna( Expression *expre , Expression *&result , int length
			     , ReplaceChain * condition )
  {
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType == Number
	     and ( expre -> P(i) -> Value.RePart() < NumberType::Zero
		   or ( expre -> P(i) -> Value.RePart().IsZero()
			and expre -> P(i) -> Value.ImPart() < NumberType::Zero )))
	  {
	    result = make( Add , 1 );
	    result -> Attach[0] = -1;
	    expre -> P(i) -> Value = -expre -> P(i) -> Value;
	    result -> P(0) = expre;
	    result = Transform( result , length , condition );
	    return true;
	  }
	if ( expre -> P(i) -> ExpType == Add
	     and expre -> P(i) -> GetAttach(0) == -1 )
	  {
	    result = make( Add , 1 );
	    result -> Attach[0] = -1;
	    for ( int j = 0 ; j < expre -> P(i) -> NumOfPara ; j++ )
	      expre -> P(i) -> Attach[j] = -expre -> P(i) -> Attach[j];
	    result -> P(0) = expre;
	    result = Transform( result , length , condition );
	    return true;
	  }
      }
    return false;
  };
  
  bool Expression::MulTri( Expression *expre , Expression *&result , int length
			   , ReplaceChain * condition )
  {
    Expression * tmp;
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	for ( int j = 0 ; j < i ; j++ )
	  {
	    switch ( expre -> P(i) -> ExpType )
	      {
	      case Sin:
	      case Cos:
	      case Tan:
	      case Cot:
	      case Sec:
	      case Csc:
		break;
	      default:
		goto Out1;
	      };
	    switch ( expre -> P(j) -> ExpType )
	      {
	      case Sin:
	      case Cos:
	      case Tan:
	      case Cot:
	      case Sec:
	      case Csc:
		break;
	      default:
		continue;
	      };
	    if ( *( expre -> P(i) -> P(0) )
		 != *( expre -> P(j) -> P(0) )) continue;
	    switch ( expre -> P(i) -> ExpType )
	      {
	      case Sin:
		switch ( expre -> P(j) -> ExpType )
		  {
		  case Sin:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    tmp = make( NumberType::One );
		    break;
		  case Cos:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Tan , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Cot , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Tan:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Cos , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Sec , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Cot:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Cos , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Sec , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Sec:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Tan , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Cot , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Csc:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    tmp = make( NumberType::One );
		    break;
		  default:
		    continue;
		  };
		break;
	      case Cos:
		switch ( expre -> P(j) -> ExpType )
		  {
		  case Sin:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Cot , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Tan , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Cos:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    tmp = make( NumberType::One );
		    break;
		  case Tan:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Sin , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Csc , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Cot:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Sin , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Csc , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Sec:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    tmp = make( NumberType::One );
		    break;
		  case Csc:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Cot , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Tan , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  default:
		    continue;
		  };
		break;
	      case Tan:
		switch ( expre -> P(j) -> ExpType )
		  {
		  case Sin:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Sec , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Cos , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Cos:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Sin , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Csc , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Tan:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    tmp = make( NumberType::One );
		    break;
		  case Cot:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    tmp = make( NumberType::One );
		    break;
		  case Sec:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Sin , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Csc , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Csc:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Sec , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Cos , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  default:
		    continue;
		  };
		break;
	      case Cot:
		switch ( expre -> P(j) -> ExpType )
		  {
		  case Sin:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Cos , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Sec , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Cos:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Csc , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Sin , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Tan:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    tmp = make( NumberType::One );
		    break;
		  case Cot:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    tmp = make( NumberType::One );
		    break;
		  case Sec:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Csc , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Sin , 1 );
		    ( tmp -> P(0)
		      = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Csc:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Cos , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Sec , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  default:
		    continue;
		  };
		break;
	      case Sec:
		switch ( expre -> P(j) -> ExpType )
		  {
		  case Sin:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Tan , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Cot , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Cos:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    tmp = make( NumberType::One );
		    break;
		  case Tan:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Csc , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Sin , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Cot:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Csc , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Sin , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Sec:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    tmp = make( NumberType::One );
		    break;
		  case Csc:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Tan , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Cot , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  default:
		    continue;
		  };
		break;
	      case Csc:
		switch ( expre -> P(j) -> ExpType )
		  {
		  case Sin:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    tmp = make( NumberType::One );
		    break;
		  case Cos:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Cot , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Tan , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Tan:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == -1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Sec , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Cos , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Cot:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Sec , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Cos , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Sec:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    if ( expre -> GetAttach(i) == 1 )
		      {
			tmp = make( Cot , 1 );
			( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
			break;
		      }
		    tmp = make( Tan , 1 );
		    ( tmp -> P(0) = expre -> P(i) -> P(0) ) -> attach();
		    break;
		  case Csc:
		    if ( expre -> GetAttach(i) * expre -> GetAttach(j) == 1 )
		      continue;
		    tmp = make( NumberType::One );
		    break;
		  default:
		    continue;
		  };
		break;
	      default:
		break;
	      };
	    result = make( Multiply , expre -> NumOfPara - 1 );
	    for ( int s = 0 ; s < j ; s++ )
	      {
		( result -> P(s) = expre -> P(s) ) -> attach();
		result -> Attach[s] = expre -> GetAttach(s);
	      }
	    for ( int s = j + 1 ; s < i ; s++ )
	      {
		( result -> P( s - 1 ) = expre -> P(s) ) -> attach();
		result -> Attach[ s - 1 ] = expre -> GetAttach(s);
	      }
	    for ( int s = i + 1 ; s < expre -> NumOfPara ; s++ )
	      {
		( result -> P( s - 2 ) = expre -> P(s) ) -> attach();
		result -> Attach[ s - 2 ] = expre -> GetAttach(s);
	      }
	    result -> P( expre -> NumOfPara - 2 ) = tmp;
	    expre -> detach();
	    result = Transform( result , length , condition );
	    return true;
	  }
      Out1:
	;
      }
    TypeOfExpre tmp2;
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> GetAttach(i) == 1 ) continue;
	switch ( expre -> P(i) -> ExpType )
	  {
	  case Sin:
	    tmp2 = Csc;
	    break;
	  case Cos:
	    tmp2 = Sec;
	    break;
	  case Tan:
	    tmp2 = Cot;
	    break;
	  case Cot:
	    tmp2 = Tan;
	    break;
	  case Sec:
	    tmp2 = Cos;
	    break;
	  case Csc:
	    tmp2 = Sin;
	    break;
	  default:
	    continue;
	  }
	result = expre -> unalias();
	result -> P(i) = result -> P(i) -> unalias();
	result -> Attach[i] = 1;
	result -> P(i) -> ExpType = tmp2;
	result = Transform( result , length , condition );
	return true;
      }
    return false;
  };
  
  Expression * Expression::TrMultiply( Expression * expre , int length
				       , ReplaceChain * condition )
  {
    if ( !CheckArgNormal( expre ) ) throw "Invalid Input For Multiply";
    Expression * result;
    if ( OneMul( expre , result , length , condition ) ) return result;
    if ( expre -> Attach != NULL )
      SortUp( expre -> Parameter , expre -> Attach , expre -> NumOfPara );
    else
      SortUp( expre -> Parameter , expre -> NumOfPara );
    if ( MulZero( expre , result , length , condition ) ) return result;
    if ( MulOne( expre , result , length , condition ) ) return result;
    if ( MulNumber( expre , result , length , condition ) ) return result;
    if ( MulMul( expre , result , length , condition ) ) return result;
    if ( MulList( expre , result , length , condition ) ) return result;
    if ( MulDiff( expre , result , length , condition ) ) return result;
    if ( MulSim( expre , result , length , condition ) ) return result;
    if ( MulSign( expre , result , length , condition ) ) return result;
    if ( MulSigna( expre , result , length , condition ) ) return result;
    if ( MulTri( expre , result , length , condition ) ) return result;    
    return expre;
  };

  bool Expression::CheckArgTwo( Expression * expre )
  {
    if ( expre -> NumOfPara != 2 ) return false;
    for ( int i = 0 ; i < 2 ; i++ )
      {
	switch ( expre -> P(i) -> ExpType )
	  {
	  case None:
	  case True:
	  case False:
	  case To:
	  case Change:
	  case Set:
	  case Equal:
	  case UnEqual:
	    return false;
	  default:
	    break;
	  };
      }
    return true;
  };
    
  bool Expression::PowSign( Expression * expre , Expression *&result , int length
			    , ReplaceChain * condition )
  {
    if ( expre -> P(1) -> ExpType == Add and expre -> P(1) -> GetAttach(0) == -1
	 and expre -> GetAttach(0) == -1 )
      {
	result = expre -> unalias();
	result -> Attach[0] = 1;
	for ( int i = 0 ; i < result -> P(1) -> NumOfPara ; i++ )
	  result -> P(1) -> Attach[i] = -result -> P(1) -> Attach[i];
	result = Transform( expre , length , condition );
	return true;
      }
    if ( expre -> P(1) -> ExpType == Add and expre -> P(1) -> NumOfPara == 1
	 and expre -> P(1) -> GetAttach(0) == -1 )
      {
	result = expre -> unalias();
	if ( result -> Attach == NULL )
	  {
	    result -> Attach = new int[2];
	    result -> Attach[0] = result -> Attach[1] = 1;
	  }
	result -> Attach[0] = -result -> Attach[0];
	result -> P(1) -> Attach[0] = 1;
	result = Transform( expre , length , condition );
	return true;
      }
    if ( expre -> P(1) -> ExpType == Number
	 and ( expre -> P(1) -> Value.RePart() < NumberType::Zero 
	       or ( expre -> P(1) -> Value.RePart().IsZero()
		    and expre -> P(1) -> Value.ImPart() < NumberType:: Zero )))
      {
	result = expre -> unalias();
	if ( result -> Attach == NULL )
	  {
	    result -> Attach = new int[2];
	    result -> Attach[0] = result -> Attach[1] = 1;
	  }
	result -> Attach[0] = -result -> Attach[0];
	result -> P(1) -> Value = -expre -> P(1) -> Value;
	result = Transform( expre , length , condition );
	return true;
      }
    if ( expre -> P(0) -> ExpType==Multiply and expre -> P(0) -> GetAttach(0)==-1 )
      {
	result = expre -> unalias();
	if ( result -> Attach == NULL )
	  {
	    result -> Attach = new int[2];
	    result -> Attach[0] = -1;
	    result -> Attach[1] = 1;
	  }
	else
	  result -> Attach[0] = -result -> Attach[0];
	for ( int i = 0 ; i < result -> P(0) -> NumOfPara ; i++ )
	  result -> P(0) -> Attach[i] = -result -> P(0) -> Attach[i];
	result = Transform( result , length , condition );
	return true;
      }
    if ( expre -> GetAttach(1) == -1 and expre -> P(1) -> ExpType == Multiply )
      {
	result = expre -> unalias();
	expre -> Attach[1] = 1;
	if ( result -> P(1) -> Attach == NULL )
	  {
	    result -> P(1) -> Attach = new int[ expre -> P(1) -> NumOfPara ];
	  }
	else
	    for ( int i = 0 ; i < expre -> P(1) -> NumOfPara ; i++ )
	      result -> P(1) -> Attach[i] = -result -> P(1) -> Attach[i];
	result = Transform( result , length , condition );
	return true;
      }
    return false;
  };
  
  bool Expression::PowNumber( Expression * expre , Expression *&result , int length
			      , ReplaceChain * condition )
  {
    
    return false;
  };
  
  bool Expression::PowPow( Expression * expre , Expression *&result , int length
			   , ReplaceChain * condition )
  {
    
    return false;
  };
  
  bool Expression::PowList( Expression * expre , Expression *&result , int length
			    , ReplaceChain * condition )
  {
    
    return false;
  };
  
  bool Expression::PowOne( Expression * expre , Expression *&result , int length
			   , ReplaceChain * condition )
  {

    return false;
  };
  
  bool Expression::PowZero( Expression * expre , Expression *&result , int length
			    , ReplaceChain * condition )
  {

    return false;
  };
  
  bool Expression::PowDiff( Expression * expre , Expression *&result , int length
			    , ReplaceChain * condition )
  {
    if ( expre -> P(0) -> ExpType == Diff )
      {
	result = make( Diff , 2 );
	result -> P(0) = make( Power , 2 );
	( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	( result -> P(0) -> P(1) = expre -> P(1) ) -> attach();
	result -> P(0) -> Attach[0] = expre -> GetAttach(0);
	result -> P(0) -> Attach[1] = expre -> GetAttach(1);
	result -> P(1) = make( Multiply , 3 );
	( result -> P(1) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	if ( expre -> GetAttach( 0 ) == 1 )
	  ( result -> P(1) -> P(1) = expre -> P(1) ) -> attach();
	else
	  {
	    result -> P(1) -> P(1) = make( Add , 1 );
	    ( result -> P(1) -> P(1) -> P(0) = expre -> P(1) ) -> attach();
	  }
	result -> P(1) -> Attach[1] = expre -> GetAttach(1);
	result -> P(1) -> P(2) = make( Power , 2 );
	( result -> P(1) -> P(2) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> P(1) -> P(2) -> P(1) = make( Add , 2 );
	result -> P(1) -> P(2) -> P(1) -> Attach[0] = expre -> GetAttach(0);
	result -> P(1) -> P(2) -> P(1) -> Attach[1] = -1;
	result -> P(1) -> P(2) -> P(1) -> P(1) = make( NumberType::One );
	if ( expre -> GetAttach( 1 ) == 1 )
	  ( result -> P(1) -> P(2) -> P(1) -> P(0) = expre -> P(1) ) -> attach();
	else
	  {
	    result -> P(1) -> P(2) -> P(1) -> P(0) = make( Multiply , 1 );
	    (result -> P(1) -> P(2) -> P(1) -> P(0) -> P(0)=expre->P(1))->attach();
	    result -> P(1) -> P(2) -> P(1) -> P(0) -> Attach[0] = -1;
	  }
	expre -> detach();
	result = Transform( result , length , condition );
	return true;
      }
    return false;
  };
  
  bool Expression::PowLog( Expression * expre , Expression *&result , int length
			   , ReplaceChain * condition )
  {
    
    return false;
  };
  
  Expression * Expression::TrPower( Expression * expre , int length
				    , ReplaceChain * condition )
  {
    if ( !CheckArgTwo( expre ) ) throw "Invalid Input For Power";
    Expression * result;
    if ( PowSign( expre , result , length , condition ) ) return result;
    if ( PowZero( expre , result , length , condition ) ) return result;
    if ( PowNumber( expre , result , length , condition ) ) return result;
    if ( PowPow( expre , result , length , condition ) ) return result;
    if ( PowList( expre , result , length , condition ) ) return result;
    if ( PowDiff( expre , result , length , condition ) ) return result;
    if ( PowLog( expre , result , length , condition ) ) return result;
    if ( PowOne( expre , result , length , condition ) ) return result;
    return expre;
  };
  
  Expression * Expression::TrLog( Expression * expre , int length
				  , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre )
	 and !CheckArgTwo( expre )) throw "Invalid Input Of Log";
    
    return expre;
  };

  bool Expression::CheckArgD( Expression * expre , ReplaceChain * condition )
  {
    if ( expre -> NumOfPara < 1 ) return false;
    switch ( expre -> P(0) -> ExpType )
      {
      case None:
      case True:
      case False:
      case To:
      case Change:
      case Set:
      case Equal:
      case UnEqual:
	return false;
      default:
	break;
      };
    for ( int i = 1 ; i < expre -> NumOfPara ; i++ )
      {
	switch ( expre -> P(i) -> ExpType )
	  {
	  case Str:
	    if ( (!AcceptableName( expre -> P(i) -> Name ))
		 or IsFunctionName( expre -> P(i) -> Name )
		 or IsConstName( expre -> P(i) -> Name )
		 or condition -> Check( expre -> P(i) -> Name ) >= 0 )
	      return false;
	    break;
	  default:
	    return false;
	  };
      }
    return true;
  };

  Expression * Expression::TrD( Expression * expre , int length
				, ReplaceChain * condition )
  {
    if ( !CheckArgD( expre , condition ) ) throw "Invaid Input for D";
    Expression * result;
    if ( expre -> NumOfPara == 1 )
      {
	( result = expre -> P(0) ) -> attach();
	expre -> detach();
	return result;
      }
    Expression * var = make( "0diff" );
    Expression * d = make( NumberType::One );
    Expression * diff = make( Diff , 2 );
    ( diff -> P(0) = var ) -> attach();
    ( diff -> P(1) = d ) -> attach();
    for ( int i = 1 ; i < expre -> NumOfPara ; i++ )
      {
	if ( expre -> P(i) -> ExpType != Str
	     or ! AcceptableName( expre -> P(i) -> Name ) )
	  {
	    diff -> detach();
	    d -> detach();
	    var -> detach();
	    expre -> detach();
	    throw " Error in D1 ";
	  }
	Replacement rep0 = Replacement( expre -> P(i) -> Name , NULL , 0 , diff );
	Replacement rep1 = Replacement( "0diff" , NULL ,0, expre -> P(i) );
	int tmploc = condition -> GetNumber();
	if ( ! ReplaceChain::AddTo( condition , rep0 ) ) 
	  {
	    diff -> detach();
	    d -> detach();
	    var -> detach();
	    expre -> detach();
	    throw " Error in D2 ";
	  }
	expre -> P(0) = Transform( expre -> P(0) , length , condition );
	if ( expre -> P(0) -> ExpType != Diff )
	  {
	    diff -> detach();
	    d -> detach();
	    var -> detach();
	    return make( NumberType::Zero );
	  }
	condition = condition -> CutTo( tmploc );
	tmploc = condition -> GetNumber();
	( result = expre -> P(0) -> P(1) ) -> attach();
	expre -> P(0) -> detach();
	if ( ! ReplaceChain::AddTo( condition , rep1 ) )
	  {
	    diff -> detach();
	    d -> detach();
	    var -> detach();
	    expre -> detach();
	    throw " Error in D3 ";
	  }
	result = Transform( result , length , condition );
	condition -> CutTo( tmploc );
	expre -> P(0) = result;
      }
    expre -> P(0) -> attach();
    diff -> detach();
    d -> detach();
    var -> detach();
    expre -> detach();
    return result;
  };
  
  bool Expression::FunList( Expression *expre , Expression *&result , int length
			    , ReplaceChain * condition )
  {
    if ( expre -> NumOfPara != 1 ) return false;
    if ( expre -> P(0) -> ExpType == List )
      {
	result = make( List , expre -> P(0) -> NumOfPara );
	for ( int i = 0 ; i < result -> NumOfPara ; i++ )
	  {
	    result -> P(i) = make( expre -> ExpType , 1 );
	    ( result -> P(i) -> P(0) = expre -> P(0) -> P(i) ) -> attach();
	  }
	result = Transform( result , length , condition );
	return true;
      }
    return false;
  };

  Expression * Expression::TrSin( Expression * expre , int length
				  , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of Sin";
    Expression * result;
    if ( FunList( expre , result , length , condition ) ) return result;
    switch ( expre -> P(0) -> ExpType )
      {
      case Number:
	{
	  NumberType value = expre -> P(0) -> Value;
	  result = make( PrimaryFunction::Sin( value , length ) );
	  expre -> detach();
	  return result;
	}
      case Diff:
	{
	  result = make( Diff , 2 );
	  result -> P(0) = make( Sin , 1 );
	  ( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) = make( Multiply , 2 );
	  ( result -> P(1) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	  result -> P(1) -> P(1) = make( Cos , 1 );
	  ( result -> P(1) -> P(1) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  expre -> detach();
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  
  Expression * Expression::TrCos( Expression * expre , int length
				  , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of Cos";
    Expression * result;
    if ( FunList( expre , result , length , condition ) ) return result;
    switch ( expre -> P(0) -> ExpType )
      {
      case Number:
	{
	  NumberType value = expre -> P(0) -> Value;
	  result = make( PrimaryFunction::Cos( value , length ) );
	  expre -> detach();
	  return result;
	}
      case Diff:
	{
	  result = make( Diff , 2 );
	  result -> P(0) = make( Cos , 1 );
	  ( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) = make( Add , 1 );
	  result -> P(1) -> Attach[0] = -1;
	  result -> P(1) -> P(0) = make( Multiply , 2 );
	  ( result -> P(1) -> P(0) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	  result -> P(1) -> P(0) -> P(1) = make( Sin , 1 );
	  (result -> P(1) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0))->attach();
	  expre -> detach();
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  
  Expression * Expression::TrTan( Expression * expre , int length , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of Tan";
    Expression * result;
    if ( FunList( expre , result , length , condition ) ) return result;
    switch ( expre -> P(0) -> ExpType )
      {
      case Number:
	{
	  NumberType value = expre -> P(0) -> Value;
	  result = make( PrimaryFunction::Tan( value , length ) );
	  expre -> detach();
	  return result;
	}
      case Diff:
	{
	  result = make( Diff , 2 );
	  result -> P(0) = make( Tan , 1 );
	  ( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) = make( Add ,1);
	  result -> P(1) -> Attach[0] = 1;
	  result -> P(1) -> P(0) = make( Multiply , 3 );
	  ( result -> P(1) -> P(0) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	  result -> P(1) -> P(0) -> P(1) = make( Sec , 1 );
	  (result -> P(1) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0))->attach();
	  (result -> P(1) -> P(0) -> P(2)=result-> P(1) -> P(0) -> P(1))->attach();
	  expre -> detach();
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  
  Expression * Expression::TrCot( Expression * expre , int length
				  , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of Cot";
    Expression * result;
    if ( FunList( expre , result , length , condition ) ) return result;
    switch ( expre -> P(0) -> ExpType )
      {
      case Number:
	{
	  NumberType value = expre -> P(0) -> Value;
	  result = make( PrimaryFunction::Cot( value , length ) );
	  expre -> detach();
	  return result;
	}
      case Diff:
	{
	  result = make( Diff , 2 );
	  result -> P(0) = make( Cot , 1 );
	  ( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) = make( Add ,1);
	  result -> P(1) -> Attach[0] = -1;
	  result -> P(1) -> P(0) = make( Multiply , 3 );
	  ( result -> P(1) -> P(0) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	  result -> P(1) -> P(0) -> P(1) = make( Csc , 1 );
	  ( result -> P(1) -> P(0) -> P(1) -> P(0)
	    = expre -> P(0) -> P(0) ) -> attach();
	  ( result -> P(1) -> P(0) -> P(2)
	    = result -> P(1) -> P(0) -> P(1) ) -> attach();
	  expre -> detach();
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  
  Expression * Expression::TrSec( Expression * expre , int length
				  , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of Sec";
    Expression * result;
    if ( FunList( expre , result , length , condition ) ) return result;
    switch ( expre -> P(0) -> ExpType )
      {
      case Number:
	{
	  NumberType value = expre -> P(0) -> Value;
	  result = make( PrimaryFunction::Cot( value , length ));
	  expre -> detach();
	  return result;
	}
      case Diff:
	{
	  result = make( Diff , 2 );
	  result -> P(0) = make( Sec , 1 );
	  ( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) = make( Add ,1);
	  result -> P(1) -> P(0) = make( Multiply , 3 );
	  ( result -> P(1) -> P(0) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	  result -> P(1) -> P(0) -> P(1) = make( Sec , 1 );
	  ( result -> P(1) -> P(0) -> P(1) -> P(0)
	    = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) -> P(0) -> P(2) = make( Tan , 1 );
	  ( result -> P(1) -> P(0) -> P(2) -> P(0)
	    = expre -> P(0) -> P(0) ) -> attach();
	  expre -> detach();
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  Expression * Expression::TrCsc( Expression * expre , int length
				  , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of Csc";
    Expression * result;
    if ( FunList( expre , result , length , condition ) ) return result;
    switch ( expre -> P(0) -> ExpType )
      {
      case Number:
	{
	  NumberType value = expre -> P(0) -> Value;
	  result = make( PrimaryFunction::Cot( value , length ) );
	  expre -> detach();
	  return result;
	}
      case Diff:
	{
	  result = make( Diff , 2 );
	  result -> P(0) = make( Csc , 1 );
	  ( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) = make( Add ,1);
	  result -> P(1) -> Attach[0] = -1;
	  result -> P(1) -> P(0) = make( Multiply , 3 );
	  ( result -> P(1) -> P(0) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	  result -> P(1) -> P(0) -> P(1) = make( Csc , 1 );
	  ( result -> P(1) -> P(0) -> P(1) -> P(0)
	    = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) -> P(0) -> P(2) = make( Cot , 1 );
	  ( result -> P(1) -> P(0) -> P(2) -> P(0)
	    = expre -> P(0) -> P(0) ) -> attach();
	  expre -> detach();
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  
  Expression * Expression::TrArcSin( Expression * expre , int length
				     , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of ArcSin";

    return expre;
  };
  
  Expression * Expression::TrArcCos( Expression * expre , int length
				     , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of ArcCos";

    return expre;
  };
  
  Expression * Expression::TrArcTan( Expression * expre , int length
				     , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of ArcTan";

    return expre;
  };
  
  Expression * Expression::TrArcCot( Expression * expre , int length
				     , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of ArcCot";

    return expre;
  };
  
  Expression * Expression::TrArcSec( Expression * expre , int length
				     , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of ArcSec";

    return expre;
  };
  
  Expression * Expression::TrArcCsc( Expression * expre , int length
				     , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of ArcCsc";

    return expre;
  };
  
  Expression * Expression::TrN( Expression * expre , int length
				, ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) ) throw "Invalid Input Of N";

    return expre;
  };
  

  Expression * Expression::Transform( Expression * expre , int length
				      , ReplaceChain * condition )
  {
    expre = expre -> unalias();
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      expre -> P(i) = Transform( expre -> P(i) , length , condition );
    switch( expre -> ExpType )
      {
      case None:
	throw "Invalid Expression";
      case Str:
	return TrStr( expre , length , condition );
      case Number:
	return expre;
      case Diff:
	return TrDiff( expre , length , condition );
      case List:
	return TrList( expre , length , condition );
      case UsrDefine:
	return TrUsrDefine( expre , length , condition );
      case Add:
	return TrAdd( expre , length , condition );
      case Multiply:
	return TrMultiply( expre , length , condition );
      case D:
	return TrD( expre , length , condition );
      case Sin:
	return TrSin( expre , length , condition );
      case Cos:
	return TrCos( expre , length , condition );
      case Tan:
	return TrTan( expre , length , condition );
      case Cot:
	return TrCot( expre , length , condition );
      case Sec:
	return TrSec( expre , length , condition );
      case Csc:
	return TrCsc( expre , length , condition );
      case ArcSin:
	return TrArcSin( expre , length , condition );
      case ArcCos:
	return TrArcCos( expre , length , condition );
      case ArcTan:
	return TrArcTan( expre , length , condition );
      case ArcCot:
	return TrArcCot( expre , length , condition );
      case ArcSec:
	return TrArcSec( expre , length , condition );
      case ArcCsc:
	return TrArcCsc( expre , length , condition );
      case Pi:
      case E:
	return TrC( expre , length , condition );
      case Power:
	return TrPower( expre , length , condition );
      default:
	break;
      };
    return expre;
  };

  template< typename T >
  void SortUp( T * item1[] , int length )
  {
    if ( length <= 1 ) return;
    if ( length == 2 )
      {
	T * tmp;
	if ( ( *item1[0] ) < ( *item1[1] ) ) return;
	tmp = item1[0];
	item1[0] = item1[1];
	item1[1] = tmp;
	return;
      }
    T ** item = new T*[length];
    int i , j;
    for ( i = 0 ; i < length ; i++ )
      {
	item[i] = item1[i];
      }
    int half1 = length / 2 , half2 = length - half1;
    SortUp( item , half1 );
    SortUp( item + half1 , half2 );
    i = j = 0;
    while ( i < half1 or j < half2 )
      {
	if ( i == half1 )
	  {
	    item1[ i + j ] = item[ half1 + j ];
	    j++;
	    continue;
	  }
	if ( j == half2 )
	  {
	    item1[ i + j ] = item[i];
	    i++;
	    continue;
	  }
	if ( ( *item[i] ) < ( *item[ half1 + j ] ) )
	  {
	    item1[ i + j ] = item[i];
	    i++;
	    continue;
	  }
	else
	  {
	    item1[ i + j ] = item[ half1 + j ];
	    j++;
	    continue;
	  }
      }
  };
  
  template< typename T , typename T2 >
  void SortUp( T * item1[] , T2 attach[] , int length )
  {
    if ( attach == NULL ) return SortUp( item1 , length );
    if ( length <= 1 ) return;
    if ( length == 2 )
      {
	T * tmp;
	T2 tmp2;
	if ( ( *item1[0] ) < ( *item1[1] ) ) return;
	tmp = item1[0];
	item1[0] = item1[1];
	item1[1] = tmp;
	tmp2 = attach[0];
	attach[0] = attach[1];
	attach[1] = tmp2;
	return;
      }
    T ** item = new T*[length];
    T2 * attach1 = new T2[length];
    int i , j;
    for ( i = 0 ; i < length ; i++ )
      {
	item[i] = item1[i];
	attach1[i] = attach[i];
      }
    int half1 = length / 2 , half2 = length - half1;
    SortUp( item , attach1 , half1 );
    SortUp( item + half1 , attach1 + half1 , half2 );
    i = j = 0;
    while ( i < half1 or j < half2 )
      {
	if ( i == half1 )
	  {
	    item1[ i + j ] = item[ half1 + j ];
	    attach[ i + j ] = attach1[ half1 + j ];
	    j++;
	    continue;
	  }
	if ( j == half2 )
	  {
	    item1[ i + j ] = item[i];
	    attach[ i + j ] = attach1[i];
	    i++;
	    continue;
	  }
	if ( ( *item[i] ) < ( *item[ half1 + j ] ) )
	  {
	    item1[ i + j ] = item[i];
	    attach[ i + j ] = attach1[i];
	    i++;
	    continue;
	  }
	else
	  {
	    item1[ i + j ] = item[ half1 + j ];
	    attach[ i + j ] = attach1[ half1 + j ];
	    j++;
	    continue;
	  }
      }
  };
}
