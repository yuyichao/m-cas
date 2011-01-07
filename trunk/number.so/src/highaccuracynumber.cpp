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
  const HighAccuracyNumber HighAccuracyNumber::Infinity = HighAccuracyNumber::GetInfinity();
  
  const HighAccuracyNumber HighAccuracyNumber::NaN = HighAccuracyNumber();
  
  const HighAccuracyNumber HighAccuracyNumber::Zero = HighAccuracyNumber::GetZero();

  const UInt64 HighAccuracyNumber::A = 1ull << 32;
  
  const HighAccuracyNumber HighAccuracyNumber::One = 1;
  
  const HighAccuracyNumber HighAccuracyNumber::OneOverTwo = HighAccuracyNumber::One / 2;
  
  const HighAccuracyNumber HighAccuracyNumber::ThreeOverFour = HighAccuracyNumber( 3 ) / 4;
  
  HighAccuracyNumber HighAccuracyNumber::InverseSqrt( const HighAccuracyNumber & value , int length )
  {
    if ( length <= 0 ) length = value.Length;
    if ( value.IsInfinity() ) return Zero;
    if ( value < Zero ) return NaN;
    if ( value.IsZero() ) return Infinity;
    int n = ( value.Length + value.Point ) >> 1 , s = ( n << 1 ) - value.Point;
    long double tmp = 0;
    tmp += value[ s ];
    tmp *= A;
    tmp += value[ s - 1 ];
    tmp *= A;
    tmp += value[ s - 2 ];
    HighAccuracyNumber result = 1/(( double ) sqrt( tmp )) , a = value;
    result = result << ( 1 - n );
    HighAccuracyNumber e , tmp2;
    int i , j = 4;
    for ( i = 1 ; i <= length + 3 ; i *= 3 , j = ( i + 5 ) * 3 )
      {
	a.CutTo( j );
	result.CutTo( j );
	e = HighAccuracyNumber::Square( result );
	e.CutTo( j );
	e = a * e;
	e.CutTo( j );
	e = e - One;
	tmp2 = HighAccuracyNumber::Square( e );
	tmp2.CutTo( j );
	tmp2 = tmp2 * ThreeOverFour;
	tmp2.CutTo( j );
	tmp2 = e - tmp2;
	tmp2.CutTo( j );
	tmp2 = tmp2 * OneOverTwo;
	tmp2 = One - tmp2;
	tmp2.CutTo( j );
	result = result * tmp2;
      }
    result.CutTo( length );
    return result;
  };
  
  HighAccuracyNumber HighAccuracyNumber::Sqrt( const HighAccuracyNumber & value , int length )
  {
    if ( length <= 0 ) length = value.GetLength();
    if ( value < Zero ) return NaN;
    if ( value.IsZero() ) return Zero;
    if ( value.IsInfinity() ) return Infinity;
    HighAccuracyNumber result = InverseSqrt( value , length + 1 ) * value;
    result.CutTo( length );
    return result;
  };
  
  HighAccuracyNumber HighAccuracyNumber::GetInfinity()
  {
    HighAccuracyNumber result;
    result.Sign = 1;
    result.Length = 0;
    return result;
  };

  HighAccuracyNumber HighAccuracyNumber::GetZero()
  {
    return HighAccuracyNumber( 0u );
  };
  
  HighAccuracyNumber::HighAccuracyNumber()
    : Sign(0) , Length(0) , Point(0)
  {
    FullData = DataArray::make();
    Data = FullData -> GetPoint();
  };
  
  HighAccuracyNumber::HighAccuracyNumber( int value )
    : Length(1) , Point(0)
  {
    if ( value == 0 )
      {
	Sign = 0;
      }
    else
      {
	Sign = ( value > 0 );
	value = Sign * value;
      }
    FullData = DataArray::make( 1 );
    Data = FullData -> GetPoint();
    Data[0] = value;
  };
  
  HighAccuracyNumber::HighAccuracyNumber( UInt32 value )
    : Sign( value == 0 ? 0 : 1 ) , FullData( DataArray::make(1) ) 
    , Data( FullData -> GetPoint() ) , Length(1) , Point(0)
  {
    Data[0] = value;
  };
  
  HighAccuracyNumber::HighAccuracyNumber( double value )
  {
    if ( value != value )
      {
	Sign = 0;
	FullData = DataArray::make( 1 );
	Data = FullData -> GetPoint();
	Length = 0;
	Point = 0;
	return;
      }
    if ( value > DBL_MAX )
      {
	Sign = 1;
	FullData = DataArray::make( 1 );
	Data = FullData -> GetPoint();
	Length = 0;
	Point = 0;
	return;
      }
    if ( value < -DBL_MAX )
      {
	Sign = -1;
	FullData = DataArray::make( 1 );
	Data = FullData -> GetPoint();
	Length = 0;
	Point = 0;
	return;
      }
    if ( value == 0 )
      {
	Sign = 0;
	FullData = DataArray::make( 1 );
	Data = FullData -> GetPoint();
	Length = 1;
	Point = 0;
	return;
      }
    unsigned short * p = ( unsigned short * ) &value;
    if ( value > 0 )
      {
	Sign = 1;
      }
    else
      {
	Sign = -1;
	value = - value;
      }
    int flo = ( *( p + 3 )) >> 4 ;
    flo -= 1075;
    Point = flo >> 5;
    flo -= Point << 5;
    ( *( UInt64 * ) p ) = (((*( UInt64 * ) p ) << 12 ) >> 12 ) + ( 1ll << 52 );
    if ( flo >= 12 )
      {
	Length = 3;
	FullData = DataArray::make( 3 );
	Data = FullData -> GetPoint();
	( * ( UInt64 * )( Data + 1 )) = ( * ( UInt64 * ) p )  >> ( 32 - flo );
      }
    else 
      {
	Length = 2;
	FullData = DataArray::make( 2 );
	Data = FullData -> GetPoint();
      }
    ( * ( UInt64 * ) Data ) = ( * ( UInt64 * ) p ) << flo;
  };
  
  HighAccuracyNumber::HighAccuracyNumber( const HighAccuracyNumber& value )
    : Sign(value.Sign) , FullData( value.FullData ) , Data( value.Data ) 
    , Length(value.Length) , Point(value.Point)
  {
    FullData -> attach();
  };

  HighAccuracyNumber::HighAccuracyNumber( int sign , int length , int point , UInt32 * data , DataArray * fulldata )
    : Sign(sign) , FullData(fulldata) , Data(data) , Length(length) , Point(point)
  {
    FullData -> attach();
  };
  
  HighAccuracyNumber::HighAccuracyNumber( int sign , int length , int point )
    : Sign(sign) , Length(length) , Point(point)
  {
    FullData = DataArray::make( length );
    Data = FullData -> GetPoint();
  };
  
  HighAccuracyNumber& HighAccuracyNumber::operator = ( const HighAccuracyNumber& value )
  {
    if( &value != this )
      {
	Sign = value.Sign;
	Data = value.Data;
	Length = value.Length;
	Point = value.Point;
	if ( FullData != NULL )
	  {
	    FullData -> detach();
	    FullData = value.FullData;
	    FullData -> attach();
	  }
      }
    return *this;
  };
  
  HighAccuracyNumber::~HighAccuracyNumber()
  {
    if ( FullData != NULL )
      {
	FullData -> detach();
      }
  };
  
  void HighAccuracyNumber::unalias()
  {
    if ( FullData == NULL ) return;
    FullData = FullData -> unalias();
    Data = FullData -> GetPoint() + FullData -> GetLength() - Length;
  };
  
  UInt32 HighAccuracyNumber::operator [] ( int n) const
  {
    if ( n < 0 ) return 0;
    if ( n < Length and n < ( FullData -> GetLength() + ( FullData -> GetPoint() - Data ))) return Data[n];
    return 0;
  };
  
  HighAccuracyNumber operator + ( const HighAccuracyNumber& left , const HighAccuracyNumber& right )
  {
    if ( left.Sign * right.Sign == -1 )
      {
	HighAccuracyNumber right1( right );
	right1.Sign *= -1;
	return left - right1;
      }
    if ( left.IsSpecial() )
      {
	if ( right.IsSpecial() )
	  {
	    if ( left.Sign == 0 or right.Sign == 0 )
	      {
		return HighAccuracyNumber::NaN;
	      }
	  }
	return left;
      }
    if ( right.IsSpecial() )
      {
	return right;
      }
    if ( right.IsZero() ) return left;
    if ( left.IsZero() ) return right;
    int i = 0;
    int point = ( left.Point < right.Point ) ? left.Point : right.Point;
    int length = (( left.Length + left.Point ) > ( right.Length + right.Point )) ? ( left.Length + left.Point - point ) : ( right.Length + right.Point - point );
    UInt32 * tmpresult = new UInt32[ length + 1 ];
    tmpresult[0] = 0;
    for ( i = 0 ; i < length ; i++ )
      {
	if (( i + point ) >= right.Point and ( i + point ) < ( right.Point + right.Length ))
	  {
	    tmpresult[i] += right[i - right.Point + point];
	    if ( tmpresult[i] < right[i - right.Point + point] )
	      {
		tmpresult[i + 1] = 1;
	      }
	    else
	      {
		tmpresult[i + 1] = 0;
	      }
	  }
	else
	  {
	    tmpresult[i + 1] = 0;
	  }
	if (( i + point ) >= left.Point and ( i + point ) < ( left.Point + left.Length ))
	  {
	    tmpresult[i] += left[i - left.Point + point];
	    if ( tmpresult[i] < left[i - left.Point + point])
	      {
		tmpresult[i + 1]++;
	      }
	  }
      }
    for ( i = length ; (!tmpresult[i]) and i >= 0 ; i-- );
    length = i + 1;
    HighAccuracyNumber result(left.Sign , length , point );
    for ( i = 0 ; i < length ; i++ )
      {
	result.Data[i] = tmpresult[i];
      }
    delete []tmpresult;
    return result;
  };
  
  HighAccuracyNumber operator - ( const HighAccuracyNumber& left , const HighAccuracyNumber& right )
  {
    UInt32 tmp1 , tmp2;
    if ( left.Sign * right.Sign == -1 )
      {
	HighAccuracyNumber right1( right );
	right1.Sign *= -1;
	return left + right1;
      }
    if ( left.IsSpecial() )
      {
	if ( right.IsSpecial() )
	  {
	    return HighAccuracyNumber::NaN;
	  }
	return left;
      }
    if ( right.IsSpecial() )
      {
	return -right;
      }
    if ( right.IsZero() ) return left;
    if ( left.IsZero() ) return -right;
    int i , sign = 0;
    UInt32 j;
    int point = ( left.Point < right.Point ) ? left.Point : right.Point;
    int length = (( left.Length + left.Point ) > ( right.Length + right.Point )) ? ( left.Length + left.Point - point ) : ( right.Length + right.Point - point );
    for ( i = length - 1 ; i >= 0 ; i-- )
      {
	tmp2 = (( i + point ) >= right.Point and ( i + point ) < ( right.Point + right.Length )) ? right[i - right.Point + point] : 0;
	tmp1 = (( i + point ) >= left.Point and ( i + point ) < ( left.Point + left.Length )) ? left[i - left.Point + point] : 0;
	if ( tmp1 > tmp2 )
	  {
	    sign = 1;
	    break;
	  }
	if ( tmp2 > tmp1 )
	  {
	    sign = -1;
	    break;
	  }
      }
    if ( sign == 0 )
      {
	return HighAccuracyNumber::Zero;
      }
    UInt32 * tmpresult = new UInt32[ length ];
    j = 0;
    if ( sign == 1 )
      {
	for ( i = 0 ; i < length ; i++ )
	  {
	    if (( i + point ) >= left.Point and ( i + point ) < ( left.Point + left.Length ))
	      {
		tmpresult[i] = left[i - left.Point + point];
	      }
	    else
	      {
		tmpresult[i] = 0;
	      }
	    if ( tmpresult[i] < j )
	      {
		tmpresult[i] -= j;
		j = 1;
	      }
	    else
	      {
		tmpresult[i] -= j;
		j = 0;
	      }
	    if (( i + point ) >= right.Point and ( i + point ) < ( right.Point + right.Length ))
	      {
		if ( tmpresult[i] < right[i - right.Point + point] )
		  {
		    j++;
		  }
		tmpresult[i] -= right[i - right.Point + point];
	      }
	  }
      }
    else
      {
	for ( i = 0 ; i < length ; i++ )
	  {
	    if (( i + point ) >= right.Point and ( i + point ) < ( right.Point + right.Length ))
	      {
		tmpresult[i] = right[i - right.Point + point];
	      }
	    else
	      {
		tmpresult[i] = 0;
	      }
	    if ( tmpresult[i] < j )
	      {
		tmpresult[i] -= j;
		j = 1;
	      }
	    else
	      {
		tmpresult[i] -= j;
		j = 0;
	      }
	    if (( i + point ) >= left.Point and ( i + point ) < ( left.Point + left.Length ))
	      {
		if ( tmpresult[i] < left[i - left.Point + point] )
		  {
		    j++;
		  }
		tmpresult[i] -= left[i - left.Point + point];
	      }
	  }
      }
    for ( i = length - 1 ; (!tmpresult[i]) and i >= 0 ; i-- );
    length = i + 1;
    HighAccuracyNumber result( sign * left.Sign , length , point );
    for ( i = 0 ; i < length ; i++ )
      {
	result.Data[i] = tmpresult[i];
      }
    delete []tmpresult;
    return result;
  };
  
  HighAccuracyNumber operator * ( const HighAccuracyNumber & left , const HighAccuracyNumber & right )
  {
    if ( left.IsSpecial() )
      {
	if ( right.IsSpecial() )
	  {
	    if ( left.Sign == 0 or right.Sign == 0 )
	      {
		return HighAccuracyNumber::NaN;
	      }
	    HighAccuracyNumber result = HighAccuracyNumber::Infinity;
	    result.Sign = left.Sign * right.Sign;
	    return result;
	  }
	if ( right.IsZero() )
	  {
	    return HighAccuracyNumber::NaN;
	  }
	HighAccuracyNumber result = left;
	result.Sign *= right.Sign;
	return result;
      }
    if ( right.IsSpecial() )
      {
	if ( left.IsZero() )
	  {
	    return HighAccuracyNumber::NaN;
	  }
	HighAccuracyNumber result = right;
	result.Sign *= left.Sign;
	return result;
      }
    if ( left.IsZero() or right.IsZero() ) return HighAccuracyNumber::Zero;
    DataArray * result;
    int length = HighAccuracyBase::LongMul( left.Data , right.Data , left.Length , right.Length , result );
    HighAccuracyNumber R =  HighAccuracyNumber( left.Sign * right.Sign , length , left.Point + right.Point , result -> GetPoint() , result );
    result -> detach();
    return R;
  };
  
  HighAccuracyNumber HighAccuracyNumber::Square( const HighAccuracyNumber & value )
  {
    if ( value.IsSpecial() )
      {
	HighAccuracyNumber result = value;
	result.Sign *= result.Sign;
	return result;
      }
    if ( value.IsZero() ) return HighAccuracyNumber::Zero;
    DataArray * result;
    int length = HighAccuracyBase::Square( value.Data , value.Length , result );
    HighAccuracyNumber R =  HighAccuracyNumber( 1 , length , value.Point << 1 , result -> GetPoint() , result );
    result -> detach();
    return R;
  };
  
  HighAccuracyNumber HighAccuracyNumber::Inverse( int length ) const
  {
    if ( this -> IsSpecial() )
      {
	if ( Sign == 0 )
	  return *this;
	return Zero;
      }
    if ( this -> IsZero() )
      {
	return Infinity;
      }
    if ( length == 0 ) length = Length;
    if ( length <= 1 ) return NormalDivide( 1u , *this , 1 );
    if ( length == 2 ) return NormalDivide( 1u , *this , 2 );
    HighAccuracyNumber result = NormalDivide( 1u , *this , 3) , tmp;
    int n;
    for ( n = 30 ; !( length >> n ) ; n-- );
    n++;
    for ( int i = 0 ; i <= n ; i++ )
      {
	result = result * ( 2u - ( *this ) * result );
	result.CutTo(( 1 << i ) + 2 );
      }
    result.CutTo( length );
    return result;
  };
  
  HighAccuracyNumber HighAccuracyNumber::RecursiveDivide( const HighAccuracyNumber & left , const HighAccuracyNumber &right , int length )
  {
    HighAccuracyNumber inverse = right.Inverse( length + 1 );
    HighAccuracyNumber result = left * inverse;
    result.CutTo( length );
    return result;
  };
  
  HighAccuracyNumber HighAccuracyNumber::Divide( const HighAccuracyNumber & left1 , const HighAccuracyNumber & right1 , int length )
  {
    if ( left1.IsSpecial() )
      {
	if ( left1.Sign == 0 )
	  {
	    return left1;
	  }
	if ( right1.IsSpecial() )
	  {
	    return NaN;
	  }
	if ( right1.IsZero() )
	  {
	    return left1;
	  }
	return left1 * right1.Sign;
      }
    if ( right1.IsSpecial() )
      {
	if ( right1.Sign == 0 )
	  {
	    return right1;
	  }
	return Zero;
      }
    if ( right1.IsZero() )
      {
	return left1.Sign * Infinity;
      }
    if ( right == 0 ) return HighAccuracyNumber();
    if ( length == 0 )
      {
	length = left1.Length + 1;
      }
    HighAccuracyNumber left( left1 ) , right( right1 );
    left.CutTo( length + 1 );
    right.CutTo( length + 1 );
    if ( length >= 17 ) return RecursiveDivide( left , right , length );
    return NormalDivide( left , right , length );
  };
  
  HighAccuracyNumber HighAccuracyNumber::NormalDivide( const HighAccuracyNumber & left1 , const HighAccuracyNumber & right1 , int length )
  {
    int i , j;
    HighAccuracyNumber left , right;
    UInt32 tmp;
    UInt64 tmp2 , tmp3;
    if (( tmp = right1[right1.Length -1] ) < ( 1u << 31 ))
      {
	tmp = ( UInt32 )(( 1ll << 32 ) / ( tmp + 1 ) );
	right = right1 * tmp ;
	left = left1 * tmp ;
      }
    else
      {
	right = right1;
	left = left1;
      }
    left.unalias();
    int sign = right.Sign * left.Sign;
    left.Sign = right.Sign = 1;
    if ( left >= ( right << ( left.Point + left.Length - right.Length - right.Point )) )
      {
	j = 1;
      }
    else
      {
	j = 0;
      }
    tmp = left.Point + left.Length - right.Point - right.Length + j;
    HighAccuracyNumber result( 1 , length , tmp - length );
    right = right << ( tmp - 1 );
    int l = left.Length + left.Point ;
    for ( i = 0 ; i < length ; i++ )
      {
	result.CutTo( i + 1 );
	tmp2 = left[ l - 1 + j - i - left.Point ];
	tmp2 <<= 32;
	tmp2 += left[ l - 2 + j - i - left.Point ];
	tmp3 = tmp2 / ( right.Data[ right.Length - 1 ]);
	result.Data[0] = ( tmp3 >> 32 == 0 ) ? ( UInt32 )tmp3 + 1 : ( 0u - 1 );
	left = left - right * result.Data[0];
	while ( left < Zero )
	  {
	    left = left + right;	
	    result.Data[0]--;
	  }
	right = right << -1;
      }
    result.Sign = sign;
    return result;
  };
  
  HighAccuracyNumber HighAccuracyNumber::operator << ( int n ) const
  {
    return HighAccuracyNumber( Sign , Length , Point + n , Data , FullData );
  };
  
  bool HighAccuracyNumber::operator < ( const HighAccuracyNumber & value ) const
  {
    if ( this -> IsSpecial() )
      {
	if ( value.IsSpecial() )
	  {
	    if ( Sign == 0 or value.Sign == 0 )
	      {
		return false;
	      }
	    return Sign < value.Sign;
	  }
	return Sign < 0;
      }
    if ( value.IsSpecial() ) return value.Sign > 0;
    if ( this -> IsZero() )
      {
	if ( value.IsZero() )
	  {
	    return false;
	  }
	return value.Sign > 0;
      }
    if ( value.IsZero() ) return Sign < 0;
    if ( Sign < value.Sign ) return true;
    if ( Sign > value.Sign ) return false;
    int head1 , head2 , head;
    head1 = Length + Point;
    head2 = value.Length + value.Point;
    head = ( head1 > head2 ) ? head1 : head2;
    int point = ( Point < value.Point ) ? Point : value.Point;
    if ( Sign == 1 )
      {
	for ( int i = head - 1 ; i >= point ; i-- )
	  {
	    if ((( *this )[ i - Point ]) < value[ i - value.Point ] )
	      {
		return true;
	      }
	    if ((( *this )[ i - Point ]) > value[ i - value.Point ] )
	      {
		return false;
	      }
	  }
      }
    else
      {
	for ( int i = head - 1 ; i >= point ; i-- )
	  {
	    if ((( *this )[ i - Point ]) > value[ i - value.Point ] )
	      {
		return true;
	      }
	    if ((( *this )[ i - Point ]) < value[ i - value.Point ] )
	      {
		return false;
	      }
	  }
      }
    return false;
  };
  
  void HighAccuracyNumber::CutTo( int length )
  {
    if ( Length == 0 ) return;
    length = length > 0 ? length : 1;
    length = ( length < ( FullData -> GetLength() )) ? length : FullData -> GetLength();
    Point += Length - length;
    Data += Length - length;
    Length = length; 
  };
  
  void HighAccuracyNumber::UnCut()
  {
    if ( Length == 0 ) return;
    Point -= FullData -> GetLength() - Length;
    Length = FullData -> GetLength();
    Data = FullData -> GetPoint();
  };

  void HighAccuracyNumber::AddLengthTo( int length )
  {
    if ( Length == 0 ) return;
    if ( this -> IsNull() ) return;
    if ( length < ( FullData -> GetLength() ))
      {
	this -> CutTo( length );
	return;
      }
    DataArray * data =  DataArray::make( length );
    int i , p;
    p = length - FullData -> GetLength();
    for ( i = ( FullData -> GetLength() ) - 1 ; i >= 0 ; i-- )
      {
	( *data )[ i + p ] = ( FullData -> GetPoint())[i];
      }
    for ( i = p - 1 ; i >=0 ; i-- )
      {
	( *data )[i] = 0;
      }
    Data = data -> GetPoint();
    Point += Length - length;
    Length = length;
    FullData -> detach();
    FullData = data;
    return;
  };

  string HighAccuracyNumber::ToString( int length ) const
  {
    if ( this -> IsNull() or this -> IsNaN() ) return "NaN";
    if ( this -> IsZero() ) return "0";
    if ( this -> IsInfinity() )
      {
	if ( Sign > 0 ) return "Infinity";
	return "-Infinity";
      }
    HighAccuracyNumber tmp( *this );
    int max , min;
    max = ( int )(log10l( 1ull << 32 ) * ( Point + Length )) + (( Point +Length ) >= 0 ? 1 : 0 );
    if ( length <= 0 )
      {
	min = ( int )(log10l( 1ull << 32 ) * Point) - ( Point >= 0 ? 0 : 1 );
	length = max - min;
      }
    else
      {
	min = max - length;
      }
    int i;
    tmp.Sign = 1;
    if ( min > 0 )
      {
	HighAccuracyNumber tmp2 = 1u;
	for ( i = 0 ; i < min ; i++ )
	  {
	    tmp2 = tmp2 * 10u;
	  }
	HighAccuracyNumber * power10 = new HighAccuracyNumber[ length ];
	int * digit = new int[ length ];
	power10[0] = tmp2;
	for ( i = 1 ; i < length  ; i++ )
	  {
	    power10[i] = 10u * power10[ i - 1 ];
	  }
	for ( i = length - 1 ; i >= 0 ; i-- )
	  {
	    digit[i] = 0;
	    while( !( tmp < power10[i] ))
	      {
		digit[i]++;
		tmp = tmp - power10[i];
	      }
	  }
	delete []power10;
	for ( i = length - 1 ; !digit[i] ; i--);
	length = i;
	ostringstream stm;
	if ( Sign == -1 )
	  {
	    stm << "-";
	  }
	stm << digit[ i-- ] << ".";
	for ( ; i >= 0 ; i-- )
	  {
	    stm << digit[i];
	  }
	delete []digit;
	if ( length + min )
	  {
	    stm << "*10^" << length + min;
	  }
	return stm.str();
      }
    if ( min == 0 )
      {
	HighAccuracyNumber * power10 = new HighAccuracyNumber[ length ];
	int * digit = new int[ length ];
	power10[0] = 1u;
	for ( i = 1 ; i < length  ; i++ )
	  {
	    power10[i] = 10u * power10[ i - 1 ];
	  }
	for ( i = length - 1 ; i >= 0 ; i-- )
	  {
	    digit[i] = 0;
	    while( !( tmp < power10[i] ))
	      {
		digit[i]++;
		tmp = tmp - power10[i];
	      }
	  }
	delete []power10;
	for ( i = length - 1 ; !digit[i] ; i--);
	ostringstream stm;
	if ( Sign == -1 )
	  {
	    stm << "-";
	  }
	for ( ; i >= 0 ; i-- )
	  {
	    stm << digit[i];
	  }
	delete []digit;
	return stm.str();
      }
    if ( max > 0 )
      {
	HighAccuracyNumber * power10 = new HighAccuracyNumber[ max ];
	int * digit = new int[ length ];
	power10[0] = 1u;
	for ( i = 1 ; i < max ; i++ )
	  {
	    power10[i] = 10u * power10[ i - 1 ];
	  }
	for ( i = max - 1 ; i >= 0 ; i-- )
	  {
	    digit[ i - min ] = 0;
	    while( !( tmp < power10[i] ))
	      {
		digit[ i - min ]++;
		tmp = tmp - power10[i];
	      }
	  }
	for ( i = -min - 1 ; i >= 0 ; i-- )
	  {
	    digit[i] = 0;
	    tmp = tmp * 10u;
	    while ( !( tmp < 1u ))
	      {
		digit[i]++;
		tmp = tmp - 1u;
	      }
	  }
	delete []power10;
	for ( i = length - 1 ; !digit[i] ; i--);
	length = i;
	ostringstream stm;
	if ( Sign == -1 )
	  {
	    stm << "-";
	  }
	stm << digit[ i-- ] << ".";
	for ( ; i >= 0 ; i-- )
	  {
	    stm << digit[i];
	  }
	delete []digit;
	if ( length + min )
	  {
	    stm << "*10^" << length + min;
	  }
	return stm.str();
      }
    else
      {
	HighAccuracyNumber tmp2 = 1u;
	int * digit = new int[ length ];
	for ( i = 0 ; i < -max ; i++ )
	  {
	    tmp2 = tmp2 * 10u;
	  }
	tmp = tmp2 * tmp;
	for ( i = length - 1 ; i >= 0 ; i-- )
	  {
	    digit[i] = 0;
	    tmp = tmp * 10u;
	    while ( !( tmp < 1u ))
	      {
		digit[i]++;
		tmp = tmp - 1u;
	      }
	  }
	for ( i = length - 1 ; !digit[i] ; i--);
	length = i;
	ostringstream stm;
	if ( Sign == -1 )
	  {
	    stm << "-";
	  }
	stm << digit[ i-- ] << ".";
	for ( ; i >= 0 ; i-- )
	  {
	    stm << digit[i];
	  }
	delete []digit;
	if ( length + min )
	  {
	    stm << "*10^" << length + min;
	  }
	return stm.str();
      }
  };

  bool HighAccuracyNumber::operator == ( const HighAccuracyNumber & value ) const
  {
    if ( this -> IsSpecial() )
      {
	if ( value.IsSpecial() )
	  {
	    if ( Sign == 0 or value.Sign == 0 )
	      {
		return false;
	      }
	    return Sign == value.Sign;
	  }
	return false;
      }
    if ( value.IsSpecial() ) return false;
    if ( this -> IsZero() )
      {
	if ( value.IsZero() )
	  {
	    return true;
	  }
	return false;
      }
    if ( value.IsZero() ) return false;
    if ( Sign != value.Sign ) return false;
    if (( value.Length + value.Point ) != ( Length + Point )) return false;
    int head = Length + Point;
    int point = ( Point < value.Point ) ? Point : value.Point;
    for ( int i = point ; i < head ; i++ )
      {
	if ( (*this)[ i - Point ] != value[ i - value.Point ] ) return false;
      }
    return true;
  };

  HighAccuracyNumber operator / ( const HighAccuracyNumber & left , const HighAccuracyNumber & right )
  {
    int length = ( left.Length > right.Length ) ? left.Length : right.Length;
    return HighAccuracyNumber::Divide( left , right , length + 1 );
  };

  bool HighAccuracyNumber::operator > ( const HighAccuracyNumber & value ) const
  {
    if ( this -> IsNaN() or value.IsNaN() ) return false;
    return value < ( *this );
  };

  bool HighAccuracyNumber::operator <= ( const HighAccuracyNumber & value ) const
  {
    if ( this -> IsNaN() or value.IsNaN() ) return false;
    return !( value < ( *this ) );
  };

  bool HighAccuracyNumber::operator >= ( const HighAccuracyNumber & value ) const
  {
    return value <= ( *this );
  };

  HighAccuracyNumber HighAccuracyNumber::ReadReal( string str )
  {
    HighAccuracyNumber result = HighAccuracyNumber::Zero;
    HighAccuracyNumber tmp = HighAccuracyNumber::One;
    UInt32 i , j;
    for ( i = 0 , j = 0 ; i < str.length() ; i++ )
      {
	switch ( str[i] )
	  {
	  case '.':
	    j = 1;
	    continue;
	  case '0':
	  case '1':
	  case '2':
	  case '3':
	  case '4':
	  case '5':
	  case '6':
	  case '7':
	  case '8':
	  case '9':
	    result = result * 10 + ( str[i] - '0' );
	  if ( j == 1 )
	    tmp = tmp * 10;
	  break;
	  case '-':
	    tmp = -tmp;
	    break;
	  };
      }
    if ( j )
      return HighAccuracyNumber::Divide( result , tmp , result.GetLength() + 1 );
    else
      if ( tmp < 0 ) return -result;
      else return result;
  };
}
