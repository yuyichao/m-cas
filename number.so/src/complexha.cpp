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

#include "number.so.h"

namespace CAS
{
  ComplexHA::ComplexHA()
    : RePart( 0 ) , ImPart( 0 ){};

  ComplexHA::ComplexHA( double value )
    : RePart( value ) , ImPart( 0 ){};

  ComplexHA::ComplexHA( complex<double> value )
    : RePart( real( value ) ) , ImPart( imag( value ) ){};
  
  ComplexHA::ComplexHA( int repart )
    : RePart( repart ) , ImPart( 0 ){};
  
  ComplexHA::ComplexHA( int repart , int impart )
    : RePart( repart ) , ImPart( impart ){};

  ComplexHA::ComplexHA( const HighAccuracyNumber & repart )
    : RePart( repart ) , ImPart( 0 ){};

  ComplexHA::ComplexHA( const HighAccuracyNumber & repart , const HighAccuracyNumber & impart )
    : RePart( repart ) , ImPart ( impart ){};
  
  ComplexHA::ComplexHA( const ComplexHA & value )
    : RePart( value.RePart ) , ImPart( value.ImPart ){};

  string ComplexHA::ToString( int length ) const
  {
    if ( IsNaN() ) return "NaN";
    if ( RePart.IsZero() )
      {
	if ( ImPart.IsZero() ) return "0";
	if ( ImPart > HighAccuracyNumber::Zero )
	  return "I*" + ImPart.ToString( length );
	return "-I*" + (-ImPart).ToString( length );
      }
    else
      {
	if ( ImPart.IsZero() ) return RePart.ToString( length );
	if ( ImPart > HighAccuracyNumber::Zero )
	  return RePart.ToString( length ) + "+I*" + ImPart.ToString( length );
	return RePart.ToString( length ) + "-I*" + (-ImPart).ToString( length );
      }
  };

  ComplexHA ComplexHA::operator + ( const ComplexHA & value ) const
  {
    return ComplexHA( RePart + value.RePart , ImPart + value.ImPart );
  };

  ComplexHA ComplexHA::operator - ( const ComplexHA & value ) const
  {
    return ComplexHA( RePart - value.RePart , ImPart - value.ImPart );
  };

  ComplexHA ComplexHA::operator * ( const ComplexHA & value ) const
  {
    return ComplexHA( RePart * value.RePart - ImPart * value.ImPart , RePart * value.ImPart + ImPart * value.RePart );
  };

  ComplexHA ComplexHA::Divide( const ComplexHA & left , const ComplexHA & right , int length )
  {
    if ( right == 0 ) return ComplexHA();
    ComplexHA result = left * ComplexConjugate( right );
    HighAccuracyNumber base = MSquare( right );
    if ( length <= 0 )
      {
	length = ( left.GetLength() > right.GetLength() ) ? left.GetLength() : right.GetLength();
      }
    int head1 = result.RePart.GetPoint() + result.RePart.GetLength();
    int head2 = result.ImPart.GetPoint() + result.ImPart.GetLength();
    int tail = ( head1 + head2 )/2 - length;
    return ComplexHA( tail < head1 ? HighAccuracyNumber::Divide( result.RePart , base , head1 - tail ) : HighAccuracyNumber::Zero , tail < head2 ? HighAccuracyNumber::Divide( result.ImPart , base , head2 - tail ) : HighAccuracyNumber::Zero );
  };
  
  ComplexHA ComplexHA::operator / ( const ComplexHA & value ) const
  {
    return Divide( *this , value );
  };

  ComplexHA ComplexHA::Square( const ComplexHA & value )
  {
    return ComplexHA( HighAccuracyNumber::Square( value.RePart ) - HighAccuracyNumber::Square( value.ImPart ) , 2 * value.ImPart * value.RePart );
  };

  HighAccuracyNumber ComplexHA::Abs( const ComplexHA & value , int length )
  {
    if ( value.RePart == 0 )
      {
	if ( value.ImPart > HighAccuracyNumber::Zero )
	  {
	    return value.ImPart;
	  }
	return -value.ImPart;
      }
    if ( value.ImPart == 0 )
      {
	if ( value.RePart > HighAccuracyNumber::Zero )
	  {
	    return value.RePart;
	  }
	return -value.RePart;
      }
    if ( value.ImPart.IsNaN() or value.RePart.IsNaN() ) return HighAccuracyNumber::NaN;
    return HighAccuracyNumber::Sqrt( HighAccuracyNumber::Square( value.RePart ) + HighAccuracyNumber::Square( value.ImPart ) , length );
  };

  HighAccuracyNumber ComplexHA::MSquare( const ComplexHA & value )
  {
    if ( value.RePart == 0 ) return HighAccuracyNumber::Square( value.ImPart );
    if ( value.ImPart == 0 ) return HighAccuracyNumber::Square( value.RePart );
    if ( value.ImPart.IsNaN() or value.RePart.IsNaN() ) return HighAccuracyNumber::NaN;
    return HighAccuracyNumber::Square( value.RePart ) + HighAccuracyNumber::Square( value.ImPart );
  };

  ComplexHA ComplexHA::Inverse( int length ) const
  {
    return Divide( ComplexConjugate( *this ) , MSquare( *this ) , length );
  };

  void ComplexHA::LimitLength( int length )
  {
    if ( length <= 0 )
      {
	return;
      }
    int head1 = RePart.GetPoint() + RePart.GetLength();
    int head2 = ImPart.GetPoint() + ImPart.GetLength();
    int tail = ( head1 + head2 )/2 - length;
    if ( tail < head1 )
      {
	RePart.CutTo( head1 - tail );
      }
    else
      {
	RePart.CutTo( 1 );
      }
    if ( tail < head2 )
      {
	ImPart.CutTo( head2 - tail );
      }
    else
      {
	ImPart.CutTo( 1 );
      }
    return;
  };

  ComplexHA ComplexHA::InverseSqrt( const ComplexHA & value , int length )
  {
    if ( length <= 0 ) length = value.GetLength();
    if ( value.IsInfinity() ) return Zero;
    if ( value.IsZero() ) return RealInfinity;
    return Divide( Sqrt( value , length ) , value , length );
  };
  
  ComplexHA ComplexHA::Sqrt( const ComplexHA & value , int length )
  {
    if ( length <= 0 ) length = value.GetLength();
    if ( value.IsInfinity() ) return RealInfinity;
    ComplexHA half = value;
    half.RePart.CutTo( length + 3 );
    half.ImPart.CutTo( length + 3 );
    if ( half.IsZero() ) return Zero;
    if ( half.ImPart.IsZero() )
      {
	if ( value.RePart > HighAccuracyNumber::Zero )
	  return HighAccuracyNumber::Sqrt( value.RePart , length );
	return ComplexHA( HighAccuracyNumber::Zero , HighAccuracyNumber::Sqrt( -value.RePart , length ) );
      }
    half.RePart = HighAccuracyNumber::OneOverTwo * half.RePart;
    half.ImPart = HighAccuracyNumber::OneOverTwo * half.ImPart;
    if ( half.RePart.IsZero() )
      {
	if ( half.ImPart > 0 )
	  {
	    HighAccuracyNumber tmp = HighAccuracyNumber::Sqrt( half.ImPart , length );
	    return ComplexHA( tmp , tmp );
	  }
	HighAccuracyNumber tmp = HighAccuracyNumber::Sqrt( -half.ImPart , length );
	return ComplexHA( tmp , -tmp );	
      }
    if ( half.RePart > HighAccuracyNumber::Zero )
      {
	HighAccuracyNumber realpart = HighAccuracyNumber::Sqrt( Abs( half , length ) + half.RePart );
	return ComplexHA( realpart , HighAccuracyNumber::Divide( half.ImPart , realpart , length ) );
      }
    HighAccuracyNumber imagpart = HighAccuracyNumber::Sqrt( Abs( half , length ) - half.RePart );
    if ( half.ImPart > HighAccuracyNumber::Zero )
      {
	return ComplexHA( HighAccuracyNumber::Divide( half.ImPart , imagpart , length ) , imagpart );
      }
    return ComplexHA( -HighAccuracyNumber::Divide( half.ImPart , imagpart , length ) , -imagpart );
  };
};
