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

using namespace std;

namespace CAS
{
   NumberType PrimaryFunction::Arg( const NumberType & value , int length )
  {
    switch ( value.NumType )
      {
      case NumberType::None:
      case NumberType::Int:
      case NumberType::Double:
      case NumberType::HighReal:
	return 0;
      case NumberType::DoubleComplex:
	if ( length <= 2 )
	  return arg( *value.Number.DComplex );
	return Arg( ComplexHA( *value.Number.DComplex ) , length );
      case NumberType::HighComplex:
	return Arg( *value.Number.HAComplex , length );
      };
    return NumberType::NaN;
  };
  
  NumberType PrimaryFunction::Sin( const NumberType & , int )
  {
    
    return NumberType();
  };
  
  NumberType PrimaryFunction::Cos( const NumberType & , int )
  {
    
    return NumberType();
  };
  
  NumberType PrimaryFunction::Tan( const NumberType & , int )
  {
    
    return NumberType();
  };
  
  NumberType PrimaryFunction::Cot( const NumberType & , int )
  {
    
    return NumberType();
  };
  
  NumberType PrimaryFunction::Sec( const NumberType & , int )
  {
    
    return NumberType();
  };
  
  NumberType PrimaryFunction::Csc( const NumberType & , int )
  {
    
    return NumberType();
  };
  
  NumberType PrimaryFunction::ArcSin( const NumberType & , int )
  {
    
    return NumberType();
  };
  
  NumberType PrimaryFunction::ArcCos( const NumberType & , int )
  {
    
    return NumberType();
  };
  
  NumberType PrimaryFunction::ArcTan( const NumberType & , int )
  {
    
    return NumberType();
  };
  
  NumberType PrimaryFunction::ArcCot( const NumberType & , int )
  {
    
    return NumberType();
  };
  
  NumberType PrimaryFunction::ArcSec( const NumberType & , int )
  {

    return NumberType();
  };
  
  NumberType PrimaryFunction::ArcCsc( const NumberType & , int )
  {
    
    return NumberType();
  };
  
  NumberType PrimaryFunction::Exp( const NumberType &value , int length )
  {
    switch ( value.TypeOfNum() )
      {
      case NumberType::None:
	return NumberType::NaN;
      case NumberType::Int:
	if ( length <= 3 ) return NumberType( exp( value.Number.IntNumber ) );
	return Exp( HighAccuracyNumber( value.Number.IntNumber ) , length );
      case NumberType::Double:
	if ( length <= 3 ) return NumberType( exp( value.Number.DoubleNumber ) );
	return Exp( HighAccuracyNumber( value.Number.DoubleNumber ) , length );
      case NumberType::HighReal:
	return Exp( *(value.Number.HReal) , length );
      case NumberType::DoubleComplex:
	if ( length <= 3 ) return NumberType( exp( *(value.Number.DComplex) ) );
	return Exp( ComplexHA( *(value.Number.DComplex) ) , length );
      case NumberType::HighComplex:
	return Exp( *(value.Number.HAComplex) , length );
      };
    return NumberType();
  };

  NumberType PrimaryFunction::Ln( const NumberType & , int )
  {
    
    return NumberType();
  };
  
  NumberType PrimaryFunction::Power( const NumberType & , const NumberType & , int )
  {
    
    return NumberType();
  };

  NumberType PrimaryFunction::Log( const NumberType & , const NumberType & , int )
  {

    return NumberType();
  };
}

