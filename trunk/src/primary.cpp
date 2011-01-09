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
  }
  
  NumberType PrimaryFunction::Sin( const NumberType& value , int length )
  {
    switch ( value.TypeOfNum() )
      {
      case NumberType::None:
	return NumberType::NaN;
      case NumberType::Int:
	if ( length <= 2 ) return NumberType( sin( value.Number.IntNumber ) );
	return Sin( HighAccuracyNumber( value.Number.IntNumber ) , length );
      case NumberType::Double:
	if ( length <= 2 ) return NumberType( sin( value.Number.DoubleNumber ) );
	return Sin( HighAccuracyNumber( value.Number.DoubleNumber ) , length );
      case NumberType::HighReal:
	return Sin( *(value.Number.HReal) , length );
      case NumberType::DoubleComplex:
	if ( length <= 2 ) return NumberType( sin( *(value.Number.DComplex) ) );
	return Sin( ComplexHA( *(value.Number.DComplex) ) , length );
      case NumberType::HighComplex:
	return Sin( *(value.Number.HAComplex) , length );
      };
    return NumberType();
  };
  
  NumberType PrimaryFunction::Cos( const NumberType& value , int length )
  {
    switch ( value.TypeOfNum() )
      {
      case NumberType::None:
	return NumberType::NaN;
      case NumberType::Int:
	if ( length <= 2 ) return NumberType( cos( value.Number.IntNumber ) );
	return Cos( HighAccuracyNumber( value.Number.IntNumber ) , length );
      case NumberType::Double:
	if ( length <= 2 ) return NumberType( cos( value.Number.DoubleNumber ) );
	return Cos( HighAccuracyNumber( value.Number.DoubleNumber ) , length );
      case NumberType::HighReal:
	return Cos( *(value.Number.HReal) , length );
      case NumberType::DoubleComplex:
	if ( length <= 2 ) return NumberType( cos( *(value.Number.DComplex) ) );
	return Cos( ComplexHA( *(value.Number.DComplex) ) , length );
      case NumberType::HighComplex:
	return Cos( *(value.Number.HAComplex) , length );
      };
    return NumberType();
  };
  
  NumberType PrimaryFunction::Tan( const NumberType& value, int length )
  {
    switch ( value.TypeOfNum() )
      {
      case NumberType::None:
	return NumberType::NaN;
      case NumberType::Int:
	if ( length <= 2 ) return NumberType( tan( value.Number.IntNumber ) );
	return Tan( HighAccuracyNumber( value.Number.IntNumber ) , length );
      case NumberType::Double:
	if ( length <= 2 ) return NumberType( tan( value.Number.DoubleNumber ) );
	return Tan( HighAccuracyNumber( value.Number.DoubleNumber ) , length );
      case NumberType::HighReal:
	return Tan( *(value.Number.HReal) , length );
      case NumberType::DoubleComplex:
	if ( length <= 2 ) return NumberType( tan( *(value.Number.DComplex) ) );
	return Tan( ComplexHA( *(value.Number.DComplex) ) , length );
      case NumberType::HighComplex:
	return Tan( *(value.Number.HAComplex) , length );
      };
    return NumberType();
  };
  
  NumberType PrimaryFunction::Cot( const NumberType& value, int length )
  {
    switch ( value.TypeOfNum() )
      {
      case NumberType::None:
	return NumberType::NaN;
      case NumberType::Int:
	if ( length <= 2 ) return NumberType( 1/tan( value.Number.IntNumber ) );
	return Cot( HighAccuracyNumber( value.Number.IntNumber ) , length );
      case NumberType::Double:
	if ( length <= 2 ) return NumberType( 1/tan( value.Number.DoubleNumber ) );
	return Cot( HighAccuracyNumber( value.Number.DoubleNumber ) , length );
      case NumberType::HighReal:
	return Cot( *(value.Number.HReal) , length );
      case NumberType::DoubleComplex:
	if(length <= 2) return NumberType((complex<double>)
					  1/tan(*(value.Number.DComplex)));
	return Cot( ComplexHA( *(value.Number.DComplex) ) , length );
      case NumberType::HighComplex:
	return Cot( *(value.Number.HAComplex) , length );
      };
    return NumberType();
  };
  
  NumberType PrimaryFunction::Sec( const NumberType& value, int length )
  {
    switch ( value.TypeOfNum() )
      {
      case NumberType::None:
	return NumberType::NaN;
      case NumberType::Int:
	if ( length <= 2 ) return NumberType( 1/cos( value.Number.IntNumber ) );
	return Sec( HighAccuracyNumber( value.Number.IntNumber ) , length );
      case NumberType::Double:
	if ( length <= 2 ) return NumberType( 1/cos( value.Number.DoubleNumber ) );
	return Sec( HighAccuracyNumber( value.Number.DoubleNumber ) , length );
      case NumberType::HighReal:
	return Sec( *(value.Number.HReal) , length );
      case NumberType::DoubleComplex:
	if ( length <= 2 ) return NumberType( (complex<double>)
					      1/cos( *(value.Number.DComplex) ) );
	return Sec( ComplexHA( *(value.Number.DComplex) ) , length );
      case NumberType::HighComplex:
	return Sec( *(value.Number.HAComplex) , length );
      };
    return NumberType();
  };
  
  NumberType PrimaryFunction::Csc( const NumberType& value, int length )
  {
    switch ( value.TypeOfNum() )
      {
      case NumberType::None:
	return NumberType::NaN;
      case NumberType::Int:
	if ( length <= 2 ) return NumberType( 1/sin( value.Number.IntNumber ) );
	return Csc( HighAccuracyNumber( value.Number.IntNumber ) , length );
      case NumberType::Double:
	if ( length <= 2 ) return NumberType( 1/sin( value.Number.DoubleNumber ) );
	return Csc( HighAccuracyNumber( value.Number.DoubleNumber ) , length );
      case NumberType::HighReal:
	return Csc( *(value.Number.HReal) , length );
      case NumberType::DoubleComplex:
	if ( length <= 2 ) return NumberType( (complex<double>)
					      1/sin( *(value.Number.DComplex) ) );
	return Csc( ComplexHA( *(value.Number.DComplex) ) , length );
      case NumberType::HighComplex:
	return Csc( *(value.Number.HAComplex) , length );
      };
    return NumberType();
  };
  
  NumberType PrimaryFunction::ArcSin( const NumberType& value, int length )
  {
    if ( !value.IsReal() or ( value.RePart() >= -1 and value.RePart() <= 1 ) )
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    if ( length <= 2 ) return NumberType( asin( value.Number.IntNumber ) );
	    return ArcSin( HighAccuracyNumber( value.Number.IntNumber ) , length );
	  case NumberType::Double:
	    if (length <= 2) return NumberType( asin( value.Number.DoubleNumber ));
	    return ArcSin( HighAccuracyNumber(value.Number.DoubleNumber),length );
	  case NumberType::HighReal:
	  return ArcSin( *(value.Number.HReal) , length );
	  case NumberType::DoubleComplex:
	    return ArcSin( ComplexHA( *(value.Number.DComplex) ) , length );
	  case NumberType::HighComplex:
	    return ArcSin( *(value.Number.HAComplex) , length );
	  };
      }
    else
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    return ArcSin( ComplexHA( value.Number.IntNumber ) , length );
	  case NumberType::Double:
	    return ArcSin( ComplexHA( value.Number.DoubleNumber ) , length );
	  case NumberType::HighReal:
	    return ArcSin( ComplexHA(*(value.Number.HReal)) , length );
	  default:
	    break;
	  };
      }
    return NumberType();
  };
  
  NumberType PrimaryFunction::ArcCos( const NumberType& value , int length )
  {
    if ( !value.IsReal() or ( value.RePart() >= -1 and value.RePart() <= 1 ) )
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    if ( length <= 2 ) return NumberType( acos( value.Number.IntNumber ) );
	    return ArcCos( HighAccuracyNumber( value.Number.IntNumber ) , length );
	  case NumberType::Double:
	    if (length <= 2) return NumberType( acos( value.Number.DoubleNumber ));
	    return ArcCos( HighAccuracyNumber(value.Number.DoubleNumber),length );
	  case NumberType::HighReal:
	  return ArcCos( *(value.Number.HReal) , length );
	  case NumberType::DoubleComplex:
	    return ArcCos( ComplexHA( *(value.Number.DComplex) ) , length );
	  case NumberType::HighComplex:
	    return ArcCos( *(value.Number.HAComplex) , length );
	  };
      }
    else
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    return ArcCos( ComplexHA( value.Number.IntNumber ) , length );
	  case NumberType::Double:
	    return ArcCos( ComplexHA( value.Number.DoubleNumber ) , length );
	  case NumberType::HighReal:
	    return ArcCos( ComplexHA(*(value.Number.HReal)) , length );
	  default:
	    break;
	  };
      }
    return NumberType();
  };
  
  NumberType PrimaryFunction::ArcTan( const NumberType& value , int length )
  {
    switch ( value.TypeOfNum() )
      {
      case NumberType::None:
	return NumberType::NaN;
      case NumberType::Int:
	if ( length <= 2 ) return NumberType( atan( value.Number.IntNumber ) );
	return ArcTan( HighAccuracyNumber( value.Number.IntNumber ) , length );
      case NumberType::Double:
	if (length <= 2) return NumberType( atan( value.Number.DoubleNumber ));
	return ArcTan( HighAccuracyNumber(value.Number.DoubleNumber),length );
      case NumberType::HighReal:
	return ArcTan( *(value.Number.HReal) , length );
      case NumberType::DoubleComplex:
	return ArcTan( ComplexHA( *(value.Number.DComplex) ) , length );
      case NumberType::HighComplex:
	return ArcTan( *(value.Number.HAComplex) , length );
      };
    return NumberType();
  };
  
  NumberType PrimaryFunction::ArcCot( const NumberType& value , int length )
  {
    switch ( value.TypeOfNum() )
      {
      case NumberType::None:
	return NumberType::NaN;
      case NumberType::Int:
	if(length<=2) return NumberType( atan( 1/(double)value.Number.IntNumber ));
	return ArcCot( HighAccuracyNumber( value.Number.IntNumber ) , length );
      case NumberType::Double:
	if(length<=2) return NumberType( atan( 1/value.Number.DoubleNumber ));
	return ArcCot( HighAccuracyNumber(value.Number.DoubleNumber),length );
      case NumberType::HighReal:
	return ArcCot( *(value.Number.HReal) , length );
      case NumberType::DoubleComplex:
	return ArcCot( ComplexHA( *(value.Number.DComplex) ) , length );
      case NumberType::HighComplex:
	return ArcCot( *(value.Number.HAComplex) , length );
      };
    return NumberType();
  };
  
  NumberType PrimaryFunction::ArcSec( const NumberType& value , int length )
  {
    if ( !value.IsReal() or value.RePart() >= -1 or value.RePart() <= 1 )
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    if(length<=2)return NumberType(acos(1/(double)value.Number.IntNumber));
	    return ArcSec( HighAccuracyNumber( value.Number.IntNumber ) , length );
	  case NumberType::Double:
	    if(length<=2) return NumberType( acos( 1/value.Number.DoubleNumber ));
	    return ArcSec( HighAccuracyNumber(value.Number.DoubleNumber),length );
	  case NumberType::HighReal:
	  return ArcSec( *(value.Number.HReal) , length );
	  case NumberType::DoubleComplex:
	    return ArcSec( ComplexHA( *(value.Number.DComplex) ) , length );
	  case NumberType::HighComplex:
	    return ArcSec( *(value.Number.HAComplex) , length );
	  };
      }
    else
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    return ArcSec( ComplexHA( value.Number.IntNumber ) , length );
	  case NumberType::Double:
	    return ArcSec( ComplexHA( value.Number.DoubleNumber ) , length );
	  case NumberType::HighReal:
	    return ArcSec( ComplexHA(*(value.Number.HReal)) , length );
	  default:
	    break;
	  };
      }
    return NumberType();
  };
  
  NumberType PrimaryFunction::ArcCsc( const NumberType& value , int length )
  {
    if ( !value.IsReal() or value.RePart() >= -1 or value.RePart() <= 1 )
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    if(length<=2)return NumberType(asin(1/(double)value.Number.IntNumber));
	    return AecCsc( HighAccuracyNumber( value.Number.IntNumber ) , length );
	  case NumberType::Double:
	    if(length<=2) return NumberType( asin( 1/value.Number.DoubleNumber ));
	    return AecCsc( HighAccuracyNumber(value.Number.DoubleNumber),length );
	  case NumberType::HighReal:
	  return AecCsc( *(value.Number.HReal) , length );
	  case NumberType::DoubleComplex:
	    return AecCsc( ComplexHA( *(value.Number.DComplex) ) , length );
	  case NumberType::HighComplex:
	    return AecCsc( *(value.Number.HAComplex) , length );
	  };
      }
    else
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    return AecCsc( ComplexHA( value.Number.IntNumber ) , length );
	  case NumberType::Double:
	    return AecCsc( ComplexHA( value.Number.DoubleNumber ) , length );
	  case NumberType::HighReal:
	    return AecCsc( ComplexHA(*(value.Number.HReal)) , length );
	  default:
	    break;
	  };
      }
    return NumberType();
  };
  
  NumberType PrimaryFunction::Exp( const NumberType &value , int length )
  {
    switch ( value.TypeOfNum() )
      {
      case NumberType::None:
	return NumberType::NaN;
      case NumberType::Int:
	if ( length <= 2 ) return NumberType( exp( value.Number.IntNumber ) );
	return Exp( HighAccuracyNumber( value.Number.IntNumber ) , length );
      case NumberType::Double:
	if ( length <= 2 ) return NumberType( exp( value.Number.DoubleNumber ) );
	return Exp( HighAccuracyNumber( value.Number.DoubleNumber ) , length );
      case NumberType::HighReal:
	return Exp( *(value.Number.HReal) , length );
      case NumberType::DoubleComplex:
	if ( length <= 2 ) return NumberType( exp( *(value.Number.DComplex) ) );
	return Exp( ComplexHA( *(value.Number.DComplex) ) , length );
      case NumberType::HighComplex:
	return Exp( *(value.Number.HAComplex) , length );
      };
    return NumberType();
  }

  NumberType PrimaryFunction::Ln( const NumberType& value , int length )
  {
    if ( value.RePart() > 0 or !value.IsReal() )
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    if ( length <= 2 ) return NumberType( log( value.Number.IntNumber ) );
	    return Ln( HighAccuracyNumber( value.Number.IntNumber ) , length );
	  case NumberType::Double:
	    if (length <= 2) return NumberType( log( value.Number.DoubleNumber ) );
	    return Ln( HighAccuracyNumber( value.Number.DoubleNumber ) , length );
	  case NumberType::HighReal:
	  return Ln( *(value.Number.HReal) , length );
	  case NumberType::DoubleComplex:
	    if ( length <= 2 ) return NumberType( log( *(value.Number.DComplex)));
	    return Ln( ComplexHA( *(value.Number.DComplex) ) , length );
	  case NumberType::HighComplex:
	    return Ln( *(value.Number.HAComplex) , length );
	  };
      }
    else
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    if (length<=2)
	      return NumberType(log((complex<double>)value.Number.IntNumber));
	    return Ln( ComplexHA( value.Number.IntNumber ) , length );
	  case NumberType::Double:
	    if ( length <= 2 )
	      return NumberType(log((complex<double>)value.Number.DoubleNumber ) );
	    return Ln( ComplexHA( value.Number.DoubleNumber ) , length );
	  case NumberType::HighReal:
	    return Ln( ComplexHA(*(value.Number.HReal)) , length );
	  default:
	    break;
	  };
      }
    return NumberType();
  };
  
  NumberType PrimaryFunction::Power(const NumberType& value
				    , const NumberType& n , int length )
  {
    if ( value.RePart() > 0 or !value.IsReal() or !n.IsReal() )
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		if(length<=2) return NumberType(pow(value.Number.IntNumber
						    ,n.Number.IntNumber));
		return Power(HighAccuracyNumber(value.Number.IntNumber),
			     HighAccuracyNumber(n.Number.IntNumber), length );
	      case NumberType::Double:
		if(length<=2) return NumberType(pow(value.Number.IntNumber
						    ,n.Number.DoubleNumber));
		return Power(HighAccuracyNumber(value.Number.IntNumber),
			     HighAccuracyNumber(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Power( HighAccuracyNumber(value.Number.IntNumber)
			      , *(n.Number.HReal) , length );
	      case NumberType::DoubleComplex:
		if(length<=2) return NumberType(pow((double)value.Number.IntNumber
						    ,*(n.Number.DComplex)));
		return Power(HighAccuracyNumber(value.Number.IntNumber),
			     ComplexHA(*(n.Number.DComplex)), length );
	      case NumberType::HighComplex:
		return Power(HighAccuracyNumber(value.Number.IntNumber),
			     *(n.Number.HAComplex), length );
	      };
	  case NumberType::Double:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		if(length<=2) return NumberType(pow(value.Number.DoubleNumber
						    ,n.Number.IntNumber));
		return Power(HighAccuracyNumber(value.Number.DoubleNumber),
			     HighAccuracyNumber(n.Number.IntNumber), length );
	      case NumberType::Double:
		if(length<=2) return NumberType(pow(value.Number.DoubleNumber
						,n.Number.DoubleNumber));
		return Power(HighAccuracyNumber(value.Number.DoubleNumber),
			     HighAccuracyNumber(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Power( HighAccuracyNumber(value.Number.DoubleNumber)
			      , *(n.Number.HReal) , length );
	      case NumberType::DoubleComplex:
		if(length<=2) return NumberType(pow(value.Number.DoubleNumber
						    ,*(n.Number.DComplex)));
		return Power(HighAccuracyNumber(value.Number.DoubleNumber),
			     ComplexHA(*(n.Number.DComplex)), length );
	      case NumberType::HighComplex:
		return Power(HighAccuracyNumber(value.Number.DoubleNumber),
			     *(n.Number.HAComplex), length );
	      };
	  case NumberType::HighReal:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		return Power(*(value.Number.HReal),
			     HighAccuracyNumber(n.Number.IntNumber), length );
	      case NumberType::Double:
		return Power(*(value.Number.HReal),
			     HighAccuracyNumber(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Power( *(value.Number.HReal) , *(n.Number.HReal) , length );
	      case NumberType::DoubleComplex:
		return Power(*(value.Number.HReal)
			     ,ComplexHA(*(n.Number.DComplex)),length);
	      case NumberType::HighComplex:
		return Power(*(value.Number.HReal),*(n.Number.HAComplex), length );
	      };
	  case NumberType::DoubleComplex:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		if(length<=2) return NumberType(pow(*(value.Number.DComplex)
						    ,n.Number.IntNumber));
		return Power(ComplexHA(*(value.Number.DComplex)),
			     HighAccuracyNumber(n.Number.IntNumber), length );
	      case NumberType::Double:
		if(length<=2) return NumberType(pow(*(value.Number.DComplex)
						    ,n.Number.DoubleNumber));
		return Power(ComplexHA(*(value.Number.DComplex)),
			     HighAccuracyNumber(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Power(ComplexHA(*(value.Number.DComplex))
			     ,*(n.Number.HReal),length);
	      case NumberType::DoubleComplex:
		if(length<=2) return NumberType(pow(*(value.Number.DComplex)
						    ,*(n.Number.DComplex)));
		return Power(ComplexHA((*value.Number.DComplex)),
			     ComplexHA(*(n.Number.DComplex)), length );
	      case NumberType::HighComplex:
		return Power(*(value.Number.DComplex)
			     ,*(n.Number.HAComplex),length);
	      };
	  case NumberType::HighComplex:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		return Power(*(value.Number.HAComplex),
			     HighAccuracyNumber(n.Number.IntNumber), length );
	      case NumberType::Double:
		return Power(*(value.Number.HAComplex),
			     HighAccuracyNumber(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Power(*(value.Number.HAComplex),*(n.Number.HReal),length);
	      case NumberType::DoubleComplex:
		return Power(*(value.Number.HAComplex)
			     ,*(n.Number.DComplex),length);
	      case NumberType::HighComplex:
		return Power(*(value.Number.HAComplex)
			     ,*(n.Number.HAComplex),length);
	      };
	  };
      }
    else
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		if(length<=2) return NumberType(pow((complex<double>)
						    value.Number.IntNumber
						    ,n.Number.IntNumber));
		return Power(ComplexHA(value.Number.IntNumber),
			     HighAccuracyNumber(n.Number.IntNumber), length );
	      case NumberType::Double:
		if(length<=2) return NumberType(pow((complex<double>)
						    value.Number.IntNumber
						    ,n.Number.DoubleNumber));
		return Power(ComplexHA(value.Number.IntNumber),
			     HighAccuracyNumber(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Power( ComplexHA(value.Number.IntNumber)
			      , *(n.Number.HReal) , length );
	      default:
		break;
	      };
	  case NumberType::Double:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		if(length<=2) return NumberType(pow((complex<double>)
						    value.Number.DoubleNumber
						    ,n.Number.IntNumber));
		return Power(ComplexHA(value.Number.DoubleNumber),
			     ComplexHA(n.Number.IntNumber), length );
	      case NumberType::Double:
		if(length<=2) return NumberType(pow((complex<double>)
						    value.Number.DoubleNumber
						    ,n.Number.DoubleNumber));
		return Power(ComplexHA(value.Number.DoubleNumber),
			     ComplexHA(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Power( ComplexHA(value.Number.DoubleNumber)
			      , *(n.Number.HReal) , length );
	      default:
		break;
	      };
	  case NumberType::HighReal:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		return Power(*(value.Number.HReal),
			     ComplexHA(n.Number.IntNumber), length );
	      case NumberType::Double:
		return Power(*(value.Number.HReal),
			     ComplexHA(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Power(ComplexHA(*(value.Number.HReal))
			     ,*(n.Number.HReal),length);
	      default:
		break;
	      };
	  default:
	    break;
	  };
      }
    return NumberType();
  };

  NumberType PrimaryFunction::Log( const NumberType& value
				   , const NumberType& n , int length )
  {
    if ( value.RePart() > 0 or !value.IsReal() or !n.IsReal() )
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		if(length<=2) return NumberType(log_2(value.Number.IntNumber
						    ,n.Number.IntNumber));
		return Log(HighAccuracyNumber(value.Number.IntNumber),
			   HighAccuracyNumber(n.Number.IntNumber), length );
	      case NumberType::Double:
		if(length<=2) return NumberType(log_2<double>
						(value.Number.IntNumber
						 ,n.Number.DoubleNumber));
		return Log(HighAccuracyNumber(value.Number.IntNumber),
			   HighAccuracyNumber(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Log( HighAccuracyNumber(value.Number.IntNumber)
			    , *(n.Number.HReal) , length );
	      case NumberType::DoubleComplex:
		if(length<=2) return NumberType(log_2< complex<double> >
						(value.Number.IntNumber
						 ,*(n.Number.DComplex)));
		return Log(HighAccuracyNumber(value.Number.IntNumber),
			   ComplexHA(*(n.Number.DComplex)), length );
	      case NumberType::HighComplex:
		return Log(HighAccuracyNumber(value.Number.IntNumber),
			   *(n.Number.HAComplex), length );
	      };
	  case NumberType::Double:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		if(length<=2) return NumberType(log_2<double>
						(value.Number.DoubleNumber
						 ,n.Number.IntNumber));
		return Log(HighAccuracyNumber(value.Number.DoubleNumber),
			   HighAccuracyNumber(n.Number.IntNumber), length );
	      case NumberType::Double:
		if(length<=2) return NumberType(log_2(value.Number.DoubleNumber
						      ,n.Number.DoubleNumber));
		return Log(HighAccuracyNumber(value.Number.DoubleNumber),
			   HighAccuracyNumber(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Log( HighAccuracyNumber(value.Number.DoubleNumber)
			    , *(n.Number.HReal) , length );
	      case NumberType::DoubleComplex:
		if(length<=2) return NumberType(log_2< complex<double> >
						(value.Number.DoubleNumber
						 ,*(n.Number.DComplex)));
		return Log(HighAccuracyNumber(value.Number.DoubleNumber),
			   ComplexHA(*(n.Number.DComplex)), length );
	      case NumberType::HighComplex:
		return Log(HighAccuracyNumber(value.Number.DoubleNumber),
			   *(n.Number.HAComplex), length );
	      };
	  case NumberType::HighReal:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		return Log(*(value.Number.HReal),
			   HighAccuracyNumber(n.Number.IntNumber), length );
	      case NumberType::Double:
		return Log(*(value.Number.HReal),
			   HighAccuracyNumber(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Log( *(value.Number.HReal) , *(n.Number.HReal) , length );
	      case NumberType::DoubleComplex:
		return Log(*(value.Number.HReal)
			   ,ComplexHA(*(n.Number.DComplex)),length);
	      case NumberType::HighComplex:
		return Log(*(value.Number.HReal),*(n.Number.HAComplex), length );
	      };
	  case NumberType::DoubleComplex:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		if(length<=2) return NumberType(log_2< complex<double> >
						(*(value.Number.DComplex)
						 ,n.Number.IntNumber));
		return Log(ComplexHA(*(value.Number.DComplex)),
			   HighAccuracyNumber(n.Number.IntNumber), length );
	      case NumberType::Double:
		if(length<=2) return NumberType(log_2< complex<double> >
						(*(value.Number.DComplex)
						 ,n.Number.DoubleNumber));
		return Log(ComplexHA(*(value.Number.DComplex)),
			   HighAccuracyNumber(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Log(ComplexHA(*(value.Number.DComplex))
			   ,*(n.Number.HReal),length);
	      case NumberType::DoubleComplex:
		if(length<=2) return NumberType(log_2(*(value.Number.DComplex)
						      ,*(n.Number.DComplex)));
		return Log(ComplexHA((*value.Number.DComplex)),
			   ComplexHA(*(n.Number.DComplex)), length );
	      case NumberType::HighComplex:
		return Log(*(value.Number.DComplex) ,*(n.Number.HAComplex),length);
	      };
	  case NumberType::HighComplex:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		return Log(*(value.Number.HAComplex),
			   HighAccuracyNumber(n.Number.IntNumber), length );
	      case NumberType::Double:
		return Log(*(value.Number.HAComplex),
			   HighAccuracyNumber(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Log(*(value.Number.HAComplex),*(n.Number.HReal),length);
	      case NumberType::DoubleComplex:
		return Log(*(value.Number.HAComplex)
			   ,*(n.Number.DComplex),length);
	      case NumberType::HighComplex:
		return Log(*(value.Number.HAComplex)
			   ,*(n.Number.HAComplex),length);
	      };
	  };
      }
    else
      {
	switch ( value.TypeOfNum() )
	  {
	  case NumberType::None:
	    return NumberType::NaN;
	  case NumberType::Int:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		if(length<=2) return NumberType(log_2< complex<double> >
						(value.Number.IntNumber
						 ,n.Number.IntNumber));
		return Log(ComplexHA(value.Number.IntNumber),
			   HighAccuracyNumber(n.Number.IntNumber), length );
	      case NumberType::Double:
		if(length<=2) return NumberType(log_2< complex<double> >
						      (value.Number.IntNumber
						      ,n.Number.DoubleNumber));
		return Log(ComplexHA(value.Number.IntNumber),
			   HighAccuracyNumber(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Log( ComplexHA(value.Number.IntNumber)
			    , *(n.Number.HReal) , length );
	      default:
		break;
	      };
	  case NumberType::Double:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		if(length<=2) return NumberType(log_2< complex<double> >
						(value.Number.DoubleNumber
						 ,n.Number.IntNumber));
		return Log(ComplexHA(value.Number.DoubleNumber),
			   ComplexHA(n.Number.IntNumber), length );
	      case NumberType::Double:
		if(length<=2) return NumberType(log_2< complex<double> >
						(value.Number.DoubleNumber
						 ,n.Number.DoubleNumber));
		return Log(ComplexHA(value.Number.DoubleNumber),
			   ComplexHA(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Log( ComplexHA(value.Number.DoubleNumber)
			    , *(n.Number.HReal) , length );
	      default:
		break;
	      };
	  case NumberType::HighReal:
	    switch ( n.TypeOfNum() )
	      {
	      case NumberType::None:
		return NumberType::NaN;
	      case NumberType::Int:
		return Log(*(value.Number.HReal),
			   ComplexHA(n.Number.IntNumber), length );
	      case NumberType::Double:
		return Log(*(value.Number.HReal),
			   ComplexHA(n.Number.DoubleNumber), length );
	      case NumberType::HighReal:
		return Log(ComplexHA(*(value.Number.HReal))
			   ,*(n.Number.HReal),length);
	      default:
		break;
	      };
	  default:
	    break;
	  };
      }
    return NumberType();
  };
}

