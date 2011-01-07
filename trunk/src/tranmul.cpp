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
}
