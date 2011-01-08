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
  bool Expression::LogSign( Expression* expre , Expression*& result, int length , ReplaceChain * condition )
  {

    return false;
  };
  
  bool Expression::LogNumber( Expression* expre , Expression*& result, int length , ReplaceChain* )
  {
    if ( expre -> NumOfPara == 1 and expre -> P(0) -> ExpType == Number )
      {
	result = make( PrimaryFunction::Ln( expre -> P(0) -> Value , length ) );
	expre -> detach();
	return true;
      }
    if (expre -> P(1) -> ExpType == Number and expre -> P(0) -> ExpType == Number)
      {
	result = make( PrimaryFunction::Log( expre -> P(0) -> Value
					     , expre -> P(1) -> Value , length ));
	expre -> detach();
	return true;
      }
    return false;
  };
  
  bool Expression::LogPow( Expression* expre , Expression*& result, int length , ReplaceChain * condition )
  {
    if ( expre -> NumOfPara == 2 and expre -> P(1) -> ExpType == E )
      {
	result = make( Log , 1 );
	( result -> P(0) = expre -> P(0) ) -> attach();
	expre -> detach();
	result = Transform( result , length , condition );
	return true;
      }
    if ( expre -> P(0) -> ExpType == Power )
      {
	if ( expre -> NumOfPara == 1 )
	  {
	    result = make( Multiply , 2 );
	    ( result -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	    result -> Attach[0] = expre -> P(0) -> GetAttach(1);
	    result -> P(1) = make( Log , 1 );
	    ( result -> P(1) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	    if ( expre -> P(0) -> GetAttach(0) == -1 )
	      {
		expre -> detach();
		expre = result;
		result = make( Add , 1 );
		result -> P(0) = expre;
		result -> Attach[0] = -1;
	      }
	    else
	      expre -> detach();
	    result = Transform( result , length , condition );
	  }
	else
	  {
	    result = make( Multiply , 2 );
	    ( result -> P(0) = expre -> P(0) -> P(1) ) -> attach();
	    result -> Attach[0] = expre -> P(0) -> GetAttach(1);
	    result -> P(1) = make( Log , 2 );
	    ( result -> P(1) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	    ( result -> P(1) -> P(1) = expre -> P(1) ) -> attach();
	    if ( expre -> P(0) -> GetAttach(0) == -1 )
	      {
		expre -> detach();
		expre = result;
		result = make( Add , 1 );
		result -> P(0) = expre;
		result -> Attach[0] = -1;
	      }
	    else
	      expre -> detach();
	    result = Transform( result , length , condition );
	  }
	return true;
      }
    return false;
  };
  
  bool Expression::LogList( Expression* expre , Expression*& result, int length , ReplaceChain * condition )
  {
    if ( expre -> NumOfPara == 1 )
      {
	if ( expre -> P(0) -> ExpType == List )
	  {
	    result = make( List , expre -> P(0) -> NumOfPara );
	    for ( int i = 0 ; i < expre -> P(0) -> NumOfPara ; i++ )
	      {
		result -> P(i) = make( Log , 1 );
		( result -> P(i) -> P(0) = expre -> P(0) -> P(i) ) -> attach();
	      }
	    expre -> detach();
	    result = Transform( result , length , condition );
	    return true;
	  }
	else
	  return false;
      }
    if ( expre -> P(0) -> ExpType == List and expre -> P(0) -> NumOfPara != 1 )
      {
	if ( expre -> P(1) -> ExpType == List and expre -> P(1) -> NumOfPara != 1 )
	  {
	    if ( expre -> P(1) -> NumOfPara != expre -> P(1) -> NumOfPara )
	      {
		expre -> detach();
		throw "Length of Lists must be the same";
	      }
	    result = make( List , expre -> P(0) -> NumOfPara );
	    for ( int i = 0 ; i < expre -> P(0) -> NumOfPara ; i++ )
	      {
		result -> P(i) = make( Log , 2 );
		( result -> P(i) -> P(0) = expre -> P(0) -> P(i) ) -> attach();
		( result -> P(i) -> P(1) = expre -> P(1) -> P(i) ) -> attach();
	      }
	    expre -> detach();
	    result = Transform( result , length , condition );
	    return true;
	  }
	else
	  {
	    result = make( List , expre -> P(0) -> NumOfPara );
	    for ( int i = 0 ; i < expre -> P(0) -> NumOfPara ; i++ )
	      {
		result -> P(i) = make( Log , 2 );
		( result -> P(i) -> P(0) = expre -> P(0) -> P(i) ) -> attach();
		( result -> P(i) -> P(1) = expre -> P(1) ) -> attach();
	      }
	    expre -> detach();
	    result = Transform( result , length , condition );
	    return true;
	  }
      }
    if ( expre -> P(1) -> ExpType == List and expre -> P(1) -> NumOfPara != 1 )
      {
	result = make( List , expre -> P(1) -> NumOfPara );
	for ( int i = 0 ; i < expre -> P(1) -> NumOfPara ; i++ )
	  {
	    result -> P(i) = make( Log , 2 );
	    ( result -> P(i) -> P(1) = expre -> P(1) -> P(i) ) -> attach();
	    ( result -> P(i) -> P(0) = expre -> P(0) ) -> attach();
	  }
	expre -> detach();
	result = Transform( result , length , condition );
	return true;
      }
    return false;
  };
  
  bool Expression::LogOne(Expression* expre,Expression*& result, int,ReplaceChain*)
  {
    if(expre->P(0)->ExpType==Number and expre->P(0)-> Value==NumberType::One)
      {
	expre -> detach();
	result = make( NumberType::Zero );
	return true;
      }
    return false;
  };
  
  bool Expression::LogSame(Expression* expre,Expression*& result,int,ReplaceChain*)
  {
    if (( expre -> NumOfPara == 1 and expre -> P(0) -> ExpType == E )
	or ( expre -> NumOfPara == 2 and *(expre -> P(0)) == *(expre -> P(1))))
      {
	expre -> detach();
	result = make( NumberType::One );
	return true;
      }
    return false;
  };
  
  bool Expression::LogDiff( Expression* expre , Expression*& result, int length , ReplaceChain * condition )
  {
    
    return false;
  };
}
