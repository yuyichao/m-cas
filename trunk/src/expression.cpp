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
  Expression::Expression()
    : ExpType( None ) , Name( "" ) , Parameter( NULL ) , Attach( NULL ) , NumOfPara( 0 ) , Value() , refcount( 1 ) {};
  
  Expression::Expression( string name )
    : ExpType( Str ) , Name( name ) , Parameter( NULL ) , Attach( NULL ) , NumOfPara( 0 ) , Value() , refcount( 1 ) {};
  
  Expression::Expression( Expression::TypeOfExpre exptype , Expression ** parame , int paranum , int * attach )
    : ExpType( exptype ) , Name( "" ) , NumOfPara( paranum ) , Value() , refcount( 1 )
  {
    Parameter = new Expression*[ paranum ];
    Attach = new int[ paranum ];
    for ( int i = 0 ; i < paranum ; i++ )
      {
	P(i) = parame[i];
	P(i) -> attach();
	if ( attach != NULL )
	  Attach[i] = attach[i];
	else
	  Attach[i] = 1;
      }
  };

  Expression::Expression( TypeOfExpre exptype , int paranum )
    : ExpType( exptype ) , Name( "" ) , NumOfPara( paranum ) , Value() , refcount( 1 )
  {
    Parameter = new Expression*[ NumOfPara ];
    Attach = new int[ NumOfPara ];
    for ( int i = 0 ; i < NumOfPara ; i++ )
      Attach[i] = 1;
  };
  
  Expression::Expression( const NumberType & value )
    : ExpType( Number ) , Name( "" ) , Parameter( NULL ) , Attach( NULL ) , NumOfPara( 0 ) , Value( value ) , refcount( 1 ) {};

  Expression::Expression( const Expression & value )
    : ExpType( value.ExpType ) , Name( value.Name ) , NumOfPara( value.NumOfPara ) , Value( value.Value ) , refcount( 1 )
  {
    Parameter = new Expression*[ NumOfPara ];
    Attach = new int[ NumOfPara ];
    for ( int i = 0 ; i < NumOfPara ; i++ )
      {
	P(i) = value.P(i);
	P(i) -> attach();
	if ( value.Attach != NULL )
	  Attach[i] = value.Attach[i];
	else
	  Attach[i] = 1;
      }
  };

  Expression::~Expression()
  {
    delete []Attach;
    for ( int i = 0 ; i < NumOfPara ; i++ )
      {
	P(i) -> detach();
      }
    delete []Parameter;
  };

  Expression * Expression::make()
  {
    return new Expression();
  };

  Expression * Expression::make( string name  )
  {
    return new Expression( name );
  };

  Expression * Expression::make( TypeOfExpre exptype , Expression ** parame , int paranum , int * attach )
  {
    return new Expression( exptype , parame , paranum , attach );
  };

  Expression * Expression::make( TypeOfExpre exptype , int paranum )
  {
    return new Expression( exptype , paranum );
  };

  Expression * Expression::make( const NumberType & value )
  {
    return new Expression( value );
  };
    
  void Expression::attach()
  {
    refcount++;
  };

  void Expression::detach()
  {
    if( --refcount == 0 ) delete this;
  };

  Expression * Expression::unalias()
  {
    if( refcount == 1 ) return this;
    refcount--;
    return new Expression( *this );
  };

  bool Expression::operator < ( const Expression & value ) const
  {
    if ( ExpType != value.ExpType ) return ExpType < value.ExpType;
    if ( NumOfPara != value.NumOfPara )return NumOfPara < value.NumOfPara;
    switch( ExpType )
      {
      case None:
	return false;
      case Number:
	return Value < value.Value;
      case Str:
	return Name < value.Name;
      case Pi:
      case E:
      case True:
      case False:
	return false;
      case Sin:
      case Cos:
      case Tan:
      case Cot:
      case Sec:
      case Csc:
      case ArcSin:
      case ArcCos:
      case ArcTan:
      case ArcCot:
      case ArcSec:
      case ArcCsc:
	return *P(0) < *( value.P(0) );
      case Add:
      case Multiply:
	for ( int i = 0 ; i < NumOfPara ; i++ )
	  {
	    if ( GetAttach( i ) != value.GetAttach( i ) )
	      return GetAttach( i ) < value.GetAttach( i );
	    if ( *P(i) < *( value.P(i) )) return true;
	    if ( *P(i) > *( value.P(i) )) return false;
	  }
	return false;
      case UsrDefine:
	if ( Name != value.Name ) return Name < value.Name;
      case Power:
      case Log:
      case Diff:
      case D:
      case N:
      case Integrate:
      case Change:
      case Set:
      case Equal:
      case UnEqual:
      case To:
      case List:
	for ( int i = 0 ; i < NumOfPara ; i++ )
	  {
	    if ( *P(i) < *( value.P(i) )) return true;
	    if ( *P(i) > *( value.P(i) )) return false;
	  }
	return false;
      };
    return false;
  };

  bool Expression::operator > ( const Expression & value ) const
  {
    return value < *this;
  };

  bool Expression::operator == ( const Expression & value ) const
  {
    if ( this == &value ) return true;
    if ( ExpType != value.ExpType ) return false;
    if ( NumOfPara != value.NumOfPara )return false;
    switch( ExpType )
      {
      case None:
	return false;
      case Number:
	return Value == value.Value;
      case Str:
	return Name == value.Name;
      case Pi:
      case E:
      case True:
      case False:
	return true;
      case Sin:
      case Cos:
      case Tan:
      case Cot:
      case Sec:
      case Csc:
      case ArcSin:
      case ArcCos:
      case ArcTan:
      case ArcCot:
      case ArcSec:
      case ArcCsc:
	return *P(0) == *( value.P(0) );
      case Power:
      case Log:
      case Diff:
      case List:
      case UsrDefine:
      case To:
      case D:
      case N:
      case Integrate:
      case Change:
      case Set:
      case Equal:
      case UnEqual:
	for ( int i = 0 ; i < NumOfPara ; i++ )
	  if ( *P(i) != *( value.P(i) )) return false;
	return true;
      case Add:
      case Multiply:
	for ( int i = 0 ; i < NumOfPara ; i++ )
	  {
	    if ( GetAttach( i ) != value.GetAttach( i ) ) return false;
	    if ( *P(i) != *( value.P(i) )) return false;
	  }
	return true;
      };
    return false;
  };

  bool Expression::operator != ( const Expression & value ) const
  {
    return !( *this == value );
  };
}
