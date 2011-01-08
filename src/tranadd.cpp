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
		if ( expre -> GetAttach(i) == expre -> GetAttach(j) )
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
		 and ( *( expre -> P(i) -> P(1) ) ==  *( expre -> P(j) ) ))
	      {
		att = new int[1];
		att[0] = 1;
		sim = true;
		a = ( att2 = expre -> GetAttach(i) ) * expre -> GetAttach(j);
		if ( expre -> P(i) -> GetAttach(1) == 1 )
		  a = a + expre -> P(i) -> P(0) -> Value;
		else
		  a = a + expre -> P(i) -> P(0) -> Value.Inverse( length + 1 );
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
		a = ( att2 = expre -> GetAttach(j) ) * expre -> GetAttach(i);
		if ( expre -> P(j) -> GetAttach(1) == 1 )
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
				* expre -> GetAttach(i);
			    else
			      a = expre -> P(i) -> P(0)
				-> Value.Inverse( length + 1 ) 
				* expre -> GetAttach(i);
			  }
			else
			  a =  expre -> GetAttach(i);
			if ( l )
			  {
			    if ( expre -> P(j) -> GetAttach(0) == 1 )
			      a = a 
				+ expre -> P(j) -> P(0) -> Value
				* expre -> GetAttach(j);
			    else
			      a = a
				+ expre -> P(j) -> P(0)
				-> Value.Inverse( length + 1 )
				* expre -> GetAttach(j);
			  }
			else
			  a = a + expre -> GetAttach(j);
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
	  {
	    expre -> detach();
	    throw "Length of Lists must be the same.";
	  }
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
}
