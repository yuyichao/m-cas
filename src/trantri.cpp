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
	  return Transform( result , length , condition );
	}
      case ArcSin:
	( result = expre -> P(0) -> P(0) ) -> attach();
	expre -> detach();
	result = Transform( result , length , condition );
	return result;
      case ArcCos:
	result = make( Power , 2 );
	result -> P(1) = make( NumberType( 0.5 ) );
	result -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> Attach[1] = -1;
	result -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(1) -> P(1) = make( 2 );
	( result -> P(0) -> P(1) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcTan:
	result = make( Multiply , 2 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> P(1) = make( Power , 2 );
	result -> P(1) -> P(1) = make( NumberType( -0.5 ) );
	result -> P(1) -> P(0) = make( Add , 2 );
	result -> P(1) -> P(0) -> P(0) = make( NumberType::One );
	result -> P(1) -> P(0) -> P(1) = make( Power , 2 );
	result -> P(1) -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(1) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcCot:
	result = make( Power , 2 );
	result -> P(1) = make( NumberType( -0.5 ) );
	result -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcSec:
	result = make( Multiply , 2 );
	( result -> P(1) = expre -> P(0) -> P(0) ) -> attach();
	result -> Attach[1] = -1;
	result -> P(0) = make( Power , 2 );
	result -> P(0) -> P(1) = make( NumberType( 0.5 ) );
	result -> P(0) -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(0) -> Attach[0] = -1;
	result -> P(0) -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(0) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcCsc:
	result = make( Multiply , 1 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> Attach[0] = -1;
	expre -> detach();
	return Transform( result , length , condition );
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
      case ArcCos:
	( result = expre -> P(0) -> P(0) ) -> attach();
	expre -> detach();
	result = Transform( result , length , condition );
	return result;
      case ArcSin:
	result = make( Power , 2 );
	result -> P(1) = make( NumberType( 0.5 ) );
	result -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> Attach[1] = -1;
	result -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(1) -> P(1) = make( 2 );
	( result -> P(0) -> P(1) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcCot:
	result = make( Multiply , 2 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> P(1) = make( Power , 2 );
	result -> P(1) -> P(1) = make( NumberType( -0.5 ) );
	result -> P(1) -> P(0) = make( Add , 2 );
	result -> P(1) -> P(0) -> P(0) = make( NumberType::One );
	result -> P(1) -> P(0) -> P(1) = make( Power , 2 );
	result -> P(1) -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(1) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcTan:
	result = make( Power , 2 );
	result -> P(1) = make( NumberType( -0.5 ) );
	result -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcCsc:
	result = make( Multiply , 2 );
	( result -> P(1) = expre -> P(0) -> P(0) ) -> attach();
	result -> Attach[1] = -1;
	result -> P(0) = make( Power , 2 );
	result -> P(0) -> P(1) = make( NumberType( 0.5 ) );
	result -> P(0) -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(0) -> Attach[0] = -1;
	result -> P(0) -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(0) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcSec:
	result = make( Multiply , 1 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> Attach[0] = -1;
	expre -> detach();
	return Transform( result , length , condition );
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
      case ArcTan:
	( result = expre -> P(0) -> P(0) ) -> attach();
	expre -> detach();
	result = Transform( result , length , condition );
	return result;
      case ArcSec:
	result = make( Power , 2 );
	result -> P(1) = make( NumberType( 0.5 ) );
	result -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> Attach[0] = -1;
	result -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(1) -> P(1) = make( 2 );
	( result -> P(0) -> P(1) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcSin:
	result = make( Multiply , 2 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> P(1) = make( Power , 2 );
	result -> P(1) -> P(1) = make( NumberType( -0.5 ) );
	result -> P(1) -> P(0) = make( Add , 2 );
	result -> P(1) -> P(0) -> P(0) = make( NumberType::One );
	result -> P(1) -> P(0) -> P(1) = make( Power , 2 );
	result -> P(1) -> P(0) -> Attach[1] = -1;
	result -> P(1) -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(1) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcCsc:
	result = make( Power , 2 );
	result -> P(1) = make( NumberType( -0.5 ) );
	result -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> Attach[0] = -1;
	result -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcCos:
	result = make( Multiply , 2 );
	( result -> P(1) = expre -> P(0) -> P(0) ) -> attach();
	result -> Attach[1] = -1;
	result -> P(0) = make( Power , 2 );
	result -> P(0) -> P(1) = make( NumberType( 0.5 ) );
	result -> P(0) -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(0) -> Attach[1] = -1;
	result -> P(0) -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(0) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcCot:
	result = make( Multiply , 1 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> Attach[0] = -1;
	expre -> detach();
	return Transform( result , length , condition );
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
      case ArcCot:
	( result = expre -> P(0) -> P(0) ) -> attach();
	expre -> detach();
	result = Transform( result , length , condition );
	return result;
      case ArcCsc:
	result = make( Power , 2 );
	result -> P(1) = make( NumberType( 0.5 ) );
	result -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> Attach[0] = -1;
	result -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(1) -> P(1) = make( 2 );
	( result -> P(0) -> P(1) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcCos:
	result = make( Multiply , 2 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> P(1) = make( Power , 2 );
	result -> P(1) -> P(1) = make( NumberType( -0.5 ) );
	result -> P(1) -> P(0) = make( Add , 2 );
	result -> P(1) -> P(0) -> P(0) = make( NumberType::One );
	result -> P(1) -> P(0) -> P(1) = make( Power , 2 );
	result -> P(1) -> P(0) -> Attach[1] = -1;
	result -> P(1) -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(1) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcSec:
	result = make( Power , 2 );
	result -> P(1) = make( NumberType( -0.5 ) );
	result -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> Attach[0] = -1;
	result -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcSin:
	result = make( Multiply , 2 );
	( result -> P(1) = expre -> P(0) -> P(0) ) -> attach();
	result -> Attach[1] = -1;
	result -> P(0) = make( Power , 2 );
	result -> P(0) -> P(1) = make( NumberType( 0.5 ) );
	result -> P(0) -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(0) -> Attach[1] = -1;
	result -> P(0) -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(0) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcTan:
	result = make( Multiply , 1 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> Attach[0] = -1;
	expre -> detach();
	return Transform( result , length , condition );
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
      case ArcSec:
	( result = expre -> P(0) -> P(0) ) -> attach();
	expre -> detach();
	result = Transform( result , length , condition );
	return result;
      case ArcTan:
	result = make( Power , 2 );
	result -> P(1) = make( NumberType( 0.5 ) );
	result -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(1) -> P(1) = make( 2 );
	( result -> P(0) -> P(1) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcCsc:
	result = make( Multiply , 2 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> P(1) = make( Power , 2 );
	result -> P(1) -> P(1) = make( NumberType( -0.5 ) );
	result -> P(1) -> P(0) = make( Add , 2 );
	result -> P(1) -> P(0) -> P(0) = make( NumberType::One );
	result -> P(1) -> P(0) -> Attach[0] = -1;
	result -> P(1) -> P(0) -> P(1) = make( Power , 2 );
	result -> P(1) -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(1) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcSin:
	result = make( Power , 2 );
	result -> P(1) = make( NumberType( -0.5 ) );
	result -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> Attach[1] = -1;
	result -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcCot:
	result = make( Multiply , 2 );
	( result -> P(1) = expre -> P(0) -> P(0) ) -> attach();
	result -> Attach[1] = -1;
	result -> P(0) = make( Power , 2 );
	result -> P(0) -> P(1) = make( NumberType( 0.5 ) );
	result -> P(0) -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(0) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcCos:
	result = make( Multiply , 1 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> Attach[0] = -1;
	expre -> detach();
	return Transform( result , length , condition );
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
      case ArcCsc:
	( result = expre -> P(0) -> P(0) ) -> attach();
	expre -> detach();
	result = Transform( result , length , condition );
	return result;
      case ArcCot:
	result = make( Power , 2 );
	result -> P(1) = make( NumberType( 0.5 ) );
	result -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(1) -> P(1) = make( 2 );
	( result -> P(0) -> P(1) -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcSec:
	result = make( Multiply , 2 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> P(1) = make( Power , 2 );
	result -> P(1) -> P(1) = make( NumberType( -0.5 ) );
	result -> P(1) -> P(0) = make( Add , 2 );
	result -> P(1) -> P(0) -> P(0) = make( NumberType::One );
	result -> P(1) -> P(0) -> Attach[0] = -1;
	result -> P(1) -> P(0) -> P(1) = make( Power , 2 );
	result -> P(1) -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(1) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcCos:
	result = make( Power , 2 );
	result -> P(1) = make( NumberType( -0.5 ) );
	result -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> Attach[1] = -1;
	result -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcTan:
	result = make( Multiply , 2 );
	( result -> P(1) = expre -> P(0) -> P(0) ) -> attach();
	result -> Attach[1] = -1;
	result -> P(0) = make( Power , 2 );
	result -> P(0) -> P(1) = make( NumberType( 0.5 ) );
	result -> P(0) -> P(0) = make( Add , 2 );
	result -> P(0) -> P(0) -> P(0) = make( NumberType::One );
	result -> P(0) -> P(0) -> P(1) = make( Power , 2 );
	result -> P(0) -> P(0) -> P(1) -> P(1) = make( 2 );
	(result -> P(0) -> P(0) -> P(1) -> P(0)=expre -> P(0) -> P(0)) -> attach();
	expre -> detach();
	return Transform( result , length , condition );
      case ArcSin:
	result = make( Multiply , 1 );
	( result -> P(0) = expre -> P(0) -> P(0) ) -> attach();
	result -> Attach[0] = -1;
	expre -> detach();
	return Transform( result , length , condition );
      default:
	break;
      };
    return expre;
  };
}
