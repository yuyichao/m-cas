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
    if (expre -> P(0) -> ExpType == Number and expre -> P(1) -> ExpType == Number)
      {
	NumberType value;
	if ( expre -> GetAttach(0) == 1 )
	  if ( expre -> GetAttach(1) == 1 )
	    value = PrimaryFunction::Power(expre -> P(0) -> Value
					   , expre->P(1)->Value , length + 1);
	  else
	    value = PrimaryFunction::Power(expre -> P(0) -> Value
					   , expre->P(1)->Value.Inverse(length + 1)
					   , length + 1);
	else
	  if ( expre -> GetAttach(1) == 1 )
	    value = PrimaryFunction::Power(expre -> P(0) -> Value
					   , -expre->P(1)->Value , length + 1);
	  else
	    value = PrimaryFunction::Power(expre -> P(0) -> Value
					   , -expre->P(1)->Value.Inverse(length+1)
					   , length + 1);
	expre -> detach();
	result = make( value );
	return true;
      }
    if (expre -> P(0) -> ExpType == E and expre -> P(1) -> ExpType == Number)
      {
	NumberType value;
	if ( expre -> GetAttach(0) == 1 )
	  if ( expre -> GetAttach(1) == 1 )
	    value = PrimaryFunction::Exp( expre->P(1)->Value , length + 1);
	  else
	    value = PrimaryFunction::Exp( expre->P(1)->Value.Inverse(length + 1)
					   , length + 1);
	else
	  if ( expre -> GetAttach(1) == 1 )
	    value = PrimaryFunction::Exp( -expre->P(1)->Value , length + 1);
	  else
	    value = PrimaryFunction::Exp( -expre->P(1)->Value.Inverse(length+1)
					   , length + 1);
	expre -> detach();
	result = make( value );
	return true;
      }
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
}
