/*************************************************************************
 *                                                                       *
 *  Copyright 2010, 2011 Yu Yichao, Yang Dong                            *
 *  yyc1992@gmail.com                                                    *
 *  ydigua@gmail.com                                                     *
 *                                                                       *
 *  This file is part of m-cas.                                          *
 *                                                                       *
 *  m-cas is free software: you can redistribute it and/or modify        *
 *  it under the terms of the GNU General Public License as published by *
 *  the Free Software Foundation, either version 3 of the License, or    *
 *  (at your option) any later version.                                  *
 *                                                                       *
 *  m-cas is distributed in the hope that it will be useful,             *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with m-cas.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                       *
 *************************************************************************/

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
      {
	expre -> detach();
	throw "Invalid Use Of Function Name: " + expre -> Name;
      }
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
    if ( !CheckArgDiff( expre ) )
      {
	expre -> detach();
	throw "Invalid Input For Diff";
      }
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
      {
	expre -> detach();
	throw "Invalid Use Of Const Name: " + expre -> Name;
      }
    if ( replace.GetParaNum() != expre -> NumOfPara )
      {
	expre -> detach();
	throw "Number Of Para Of Function " + expre -> Name + " Doesn't Match.";
      }
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

  Expression * Expression::TrAdd( Expression * expre , int length
				  , ReplaceChain * condition )
  {
    if ( !CheckArgNormal( expre ) )
      {
	expre -> detach();
	throw "Invalid Input For Add";
      }
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
  
  Expression * Expression::TrMultiply( Expression * expre , int length
				       , ReplaceChain * condition )
  {
    if ( !CheckArgNormal( expre ) )
      {
	expre -> detach();
	throw "Invalid Input For Multiply";
      }
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
  
  Expression * Expression::TrPower( Expression * expre , int length
				    , ReplaceChain * condition )
  {
    if ( !CheckArgTwo( expre ) )
      {
	expre -> detach();
	throw "Invalid Input For Power";
      }
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
  
  Expression * Expression::TrLog( Expression * expre , int length
				  , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) and !CheckArgTwo( expre ))
      {
	expre -> detach();
	throw "Invalid Input Of Log";
      }
    Expression * result;
    if ( LogSign( expre , result , length , condition ) ) return result;
    if ( LogSame( expre , result , length , condition ) ) return result;
    if ( LogNumber( expre , result , length , condition ) ) return result;
    if ( LogPow( expre , result , length , condition ) ) return result;
    if ( LogList( expre , result , length , condition ) ) return result;
    if ( LogDiff( expre , result , length , condition ) ) return result;
    if ( LogLog( expre , result , length , condition ) ) return result;
    if ( LogOne( expre , result , length , condition ) ) return result;
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
    if ( !CheckArgD( expre , condition ) )
      {
	expre -> detach();
	throw "Invaid Input for D";
      }
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
	    throw " Error in D ";
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
	    throw " Error in D ";
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
	    throw " Error in D ";
	  }
	result = Transform( result , length , condition );
	condition = condition -> CutTo( tmploc );
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
    if ( !CheckArgFunction( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of Sin";
      }
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
	  result = Transform( result , length , condition );
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
    if ( !CheckArgFunction( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of Cos";
      }
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
	  result = Transform( result , length , condition );
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  
  Expression * Expression::TrTan( Expression * expre , int length , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of Tan";
      }
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
	  result = Transform( result , length , condition );
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
    if ( !CheckArgFunction( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of Cot";
      }
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
	  result = Transform( result , length , condition );
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
    if ( !CheckArgFunction( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of Sec";
      }
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
	  result = Transform( result , length , condition );
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
    if ( !CheckArgFunction( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of Csc";
      }
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
	  result = Transform( result , length , condition );
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
    if ( !CheckArgFunction( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of ArcSin";
      }
    Expression* result;
    if ( FunList( expre , result , length , condition ) ) return result;
    switch ( expre -> P(0) -> ExpType )
      {
      case Number:
	{
	  NumberType value = expre -> P(0) -> Value;
	  result = make( PrimaryFunction::ArcSin( value , length ) );
	  expre -> detach();
	  return result;
	}
      case Diff:
	{
	  result = make( Diff , 2 );
	  result -> P(0) = make( ArcSin , 1 );
	  ( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) = make( Multiply , 2 );
	  ( result -> P(1) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	  result -> P(1) -> P(1) = make( Power , 2 );
	  result -> P(1) -> P(1) -> P(1) = make( -0.5 );
	  result -> P(1) -> P(1) -> P(0) = make( Add , 2 );
	  result -> P(1) -> P(1) -> P(0) -> P(0) = make( NumberType::One );
	  result -> P(1) -> P(1) -> P(0) -> P(1) = make( Power , 2 );
	  result -> P(1) -> P(1) -> P(0) -> Attach[1] = -1;
	  result -> P(1) -> P(1) -> P(0) -> P(1) -> P(1) = make( NumberType(2) );
	  (result->P(1)->P(1)->P(0)->P(1)->P(0)=expre->P(0)->P(0))->attach();
	  expre -> detach();
	  result = Transform( result , length , condition );
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  
  Expression * Expression::TrArcCos( Expression * expre , int length
				     , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of ArcCos";
      }
    Expression* result;
    if ( FunList( expre , result , length , condition ) ) return result;
    switch ( expre -> P(0) -> ExpType )
      {
      case Number:
	{
	  NumberType value = expre -> P(0) -> Value;
	  result = make( PrimaryFunction::ArcCos( value , length ) );
	  expre -> detach();
	  return result;
	}
      case Diff:
	{
	  result = make( Diff , 2 );
	  result -> P(0) = make( ArcCos , 1 );
	  ( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) = make( Multiply , 2 );
	  ( result -> P(1) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	  result -> P(1) -> P(1) = make( Power , 2 );
	  result -> P(1) -> P(1) -> P(1) = make( -0.5 );
	  result -> P(1) -> P(1) -> P(0) = make( Add , 2 );
	  result -> P(1) -> P(1) -> P(0) -> P(0) = make( NumberType::One );
	  result -> P(1) -> P(1) -> P(0) -> P(1) = make( Power , 2 );
	  result -> P(1) -> P(1) -> P(0) -> Attach[1] = -1;
	  result -> P(1) -> P(1) -> P(0) -> P(1) -> P(1) = make( NumberType(2) );
	  (result->P(1)->P(1)->P(0)->P(1)->P(0)=expre->P(0)->P(0))->attach();
	  expre -> detach();
	  expre = result -> P(1);
	  result -> P(1) = make( Add , 1 );
	  result -> P(1) -> Attach[0] = -1;
	  result -> P(1) -> P(0) = expre;
	  result = Transform( result , length , condition );
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  
  Expression * Expression::TrArcTan( Expression * expre , int length
				     , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of ArcTan";
      }
    Expression* result;
    if ( FunList( expre , result , length , condition ) ) return result;
    switch ( expre -> P(0) -> ExpType )
      {
      case Number:
	{
	  NumberType value = expre -> P(0) -> Value;
	  result = make( PrimaryFunction::ArcTan( value , length ) );
	  expre -> detach();
	  return result;
	}
      case Diff:
	{
	  result = make( Diff , 2 );
	  result -> P(0) = make( ArcTan , 1 );
	  ( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) = make( Multiply , 2 );
	  ( result -> P(1) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	  result -> P(1) -> P(1) = make( Add , 2 );
	  result -> P(1) -> Attach[1] = -1;
	  result -> P(1) -> P(1) -> P(0) = make( NumberType::One );
	  result -> P(1) -> P(1) -> P(1) = make( Power , 2 );
	  result -> P(1) -> P(1) -> P(1) -> P(1) = make( 2 );
	  (result -> P(1) -> P(1) -> P(1) -> P(0)=expre -> P(0) -> P(0))->attach();
	  expre -> detach();
	  result = Transform( result , length , condition );
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  
  Expression * Expression::TrArcCot( Expression * expre , int length
				     , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of ArcCot";
      }
    Expression* result;
    if ( FunList( expre , result , length , condition ) ) return result;
    switch ( expre -> P(0) -> ExpType )
      {
      case Number:
	{
	  NumberType value = expre -> P(0) -> Value;
	  result = make( PrimaryFunction::ArcCot( value , length ) );
	  expre -> detach();
	  return result;
	}
      case Diff:
	{
	  result = make( Diff , 2 );
	  result -> P(0) = make( ArcCot , 1 );
	  ( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) = make( Multiply , 2 );
	  ( result -> P(1) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	  result -> P(1) -> P(1) = make( Add , 2 );
	  result -> P(1) -> Attach[1] = -1;
	  result -> P(1) -> P(1) -> P(0) = make( NumberType::One );
	  result -> P(1) -> P(1) -> P(1) = make( Power , 2 );
	  result -> P(1) -> P(1) -> P(1) -> P(1) = make( 2 );
	  (result -> P(1) -> P(1) -> P(1) -> P(0)=expre -> P(0) -> P(0))->attach();
	  expre -> detach();
	  expre = result -> P(1);
	  result -> P(1) = make( Add , 1 );
	  result -> P(1) -> Attach[0] = -1;
	  result -> P(1) -> P(0) = expre;
	  result = Transform( result , length , condition );
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  
  Expression * Expression::TrArcSec( Expression * expre , int length
				     , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of ArcSec";
      }
    Expression* result;
    if ( FunList( expre , result , length , condition ) ) return result;
    switch ( expre -> P(0) -> ExpType )
      {
      case Number:
	{
	  NumberType value = expre -> P(0) -> Value;
	  result = make( PrimaryFunction::ArcSec( value , length ) );
	  expre -> detach();
	  return result;
	}
      case Diff:
	{
	  result = make( Diff , 2 );
	  result -> P(0) = make( ArcSec , 1 );
	  ( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) = make( Multiply , 3 );
	  ( result -> P(1) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	  ( result -> P(1) -> P(1) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) -> Attach[1] = result -> P(1) -> Attach[2] = -1;
	  result -> P(1) -> P(2) = make( Power , 2 );
	  result -> P(1) -> P(2) -> P(1) = make( NumberType(0.5) );
	  result -> P(1) -> P(2) -> P(0) = make( Add , 2 );
	  result -> P(1) -> P(2) -> P(0) -> P(0) = make( -NumberType::One );
	  result -> P(1) -> P(2) -> P(0) -> P(1) = make( Power , 2 );
	  result -> P(1) -> P(2) -> P(0) -> P(1) -> P(1) = make( 2 );
	  (result->P(1)->P(2)->P(0)->P(1)->P(0)=expre->P(0)->P(0))->attach();
	  expre -> detach();
	  result = Transform( result , length , condition );
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  
  Expression * Expression::TrArcCsc( Expression * expre , int length
				     , ReplaceChain * condition )
  {
    if ( !CheckArgFunction( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of ArcCsc";
      }
    Expression* result;
    if ( FunList( expre , result , length , condition ) ) return result;
    switch ( expre -> P(0) -> ExpType )
      {
      case Number:
	{
	  NumberType value = expre -> P(0) -> Value;
	  result = make( PrimaryFunction::ArcCsc( value , length ) );
	  expre -> detach();
	  return result;
	}
      case Diff:
	{
	  result = make( Diff , 2 );
	  result -> P(0) = make( ArcCsc , 1 );
	  ( result -> P(0) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) = make( Multiply , 3 );
	  ( result -> P(1) -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	  ( result -> P(1) -> P(1) = expre -> P(0) -> P(0) ) -> attach();
	  result -> P(1) -> Attach[1] = result -> P(1) -> Attach[2] = -1;
	  result -> P(1) -> P(2) = make( Power , 2 );
	  result -> P(1) -> P(2) -> P(1) = make( NumberType(0.5) );
	  result -> P(1) -> P(2) -> P(0) = make( Add , 2 );
	  result -> P(1) -> P(2) -> P(0) -> P(0) = make( -NumberType::One );
	  result -> P(1) -> P(2) -> P(0) -> P(1) = make( Power , 2 );
	  result -> P(1) -> P(2) -> P(0) -> P(1) -> P(1) = make( 2 );
	  (result->P(1)->P(2)->P(0)->P(1)->P(0)=expre->P(0)->P(0))->attach();
	  expre -> detach();
	  expre = result -> P(1);
	  result -> P(1) = make( Add , 1 );
	  result -> P(1) -> Attach[0] = -1;
	  result -> P(1) -> P(0) = expre;
	  result = Transform( result , length , condition );
	  return result;
	}
      default:
	break;
      };
    return expre;
  };
  
  bool Expression::CheckArgN( Expression * expre )
  {
    if ( expre -> NumOfPara == 1 ) return true;
    if ( expre -> NumOfPara != 2 ) return false;
    if ( expre -> P(1) -> ExpType != Number
	 or expre -> P(1) -> Value.TypeOfNum() != NumberType::Int
	 or expre -> P(1) -> Value <= 0 ) return false;
    return true;
  };

  Expression * Expression::TrN( Expression* expre , int, ReplaceChain* condition )
  {
    if ( !CheckArgN( expre ) )
      {
	expre -> detach();
	throw "Invalid Input Of N";
      }
    int length;
    if ( expre -> NumOfPara == 1 )
      length = 1;
    else
      length = expre -> P(1) -> Value.GetInt() / 9 + 1;
    Expression* result = Transform( expre -> P(0) , length , condition );
    if ( result -> ExpType != Number )
      {
	result -> detach();
	throw "Error in N";
      }
    return result;
  };

  Expression * Expression::Transform( Expression * expre , int length
				      , ReplaceChain * condition )
  {
    expre = expre -> unalias();
    if ( expre -> ExpType == N )
      return TrN( expre , length , condition );
    if ( expre -> ExpType == Power and expre -> P(0) -> ExpType == E )
      {
	expre -> P(1) = Transform( expre -> P(1) , length , condition );
	if ( expre -> P(1) -> ExpType == Number )
	  return TrPower( expre , length , condition );
      }
    if ( expre -> ExpType == Log and
	 ( expre -> NumOfPara == 1 or expre -> P(1) -> ExpType == E ))
      {
	expre -> P(0) = Transform( expre -> P(0) , length , condition );	
	if ( expre -> P(0) -> ExpType == Number )
	  return TrLog( expre , length , condition );
      }
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      expre -> P(i) = Transform( expre -> P(i) , length , condition );
    switch( expre -> ExpType )
      {
      case None:
	expre -> detach();
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
      case Log:
	return TrLog( expre , length , condition );
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
