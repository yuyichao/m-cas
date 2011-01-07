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

using namespace std;

namespace CAS
{
  void HighAccuracyBase::FullMul( UInt64 left , UInt64 right , UInt64 result[2] )
  {
    UInt64 tmp1;
    unsigned int *pre , *pri , *ple ;
    pre=(unsigned int *)result;
    pri=(unsigned int *)&right;
    ple=(unsigned int *)&left;
    result[0] = 0;
    result[1] = 0;
    *((UInt64 *)(pre)) += ((UInt64)(*pri)) * ((UInt64)(*ple));
    *((UInt64 *)(pre+1)) += ((UInt64)(*(pri+1))) * ((UInt64)(*ple));
    tmp1 = ((UInt64)(*pri)) * ((UInt64)(*(ple+1)));
    *((UInt64 *)(pre+1)) += tmp1;
    if ( *((UInt64 *)(pre+1)) < tmp1 )
    {
      *(pre+3)+=1;
    }
    *((UInt64 *)(pre+2))+=((UInt64)(*(pri+1))) * ((UInt64)(*(ple+1)));
    return;
  };

  UInt64 HighAccuracyBase::LongMod( const UInt64 left1[2] , UInt64 right )
  {
    UInt64 left[2] , tmp , tmp2;
    left[0] = left1[0];
    left[1] = left1[1];
    int i;
    if ( right == 0 ) return 0;
    left[1] %= right;
    if ( left[1] == 0 )
      {
	return left[0] % right;
      }
    for ( i = 0 ; ( tmp = ( right >> i ) >> 1 ) ; i++ )
      {
	tmp2 = right << ( 63 - i );
	if ( left[1] > tmp )
	  {
	    left[1] -= tmp;
	    if ( left[0] < tmp2 )
	      {
		left[1]--;
	      }
	    left[0] -= tmp2;
	  }
	else
	  {
	    if ( left[1] == tmp )
	      {
		tmp2 %= right;
		if ( left[0] < tmp2 ) left[0] += right;
		left[0] -= tmp2;
		return left[0] % right;
	      }
	  }
      }
    while ( left[1] != 0 )
      {
	if ( left[0] < right ) left[1]--;
	left[0] -= right;
      }
    return left[0] % right;
  };
  
  void HighAccuracyBase::LongAdd( const UInt64 left1[2] , const UInt64 right1[2] , UInt64 result[2] )
  {
    UInt64 left[2],right[2];
    left[0] = left1[0];
    left[1] = left1[1];
    right[0] = right1[0];
    right[1] = right1[1];
    result[1] = left[1] + right[1] ;
    result[0] = left[0] + right[0];
    if ( result[0] < right[0] )
    {
      result[1]++;
    }
    return;
  };
  
  void HighAccuracyBase::LongMin( const UInt64 left1[2] , const UInt64 right1[2] , UInt64 result[2] )
  {
    UInt64 left[2],right[2];
    left[0] = left1[0];
    left[1] = left1[1];
    right[0] = right1[0];
    right[1] = right1[1];
    result[1] = left[1] - right[1] ;
    if ( left[0] < right[0] )
    {
      result[1]--;
    }
    result[0] = left[0] - right[0];
    return;
    };
  
  UInt64 HighAccuracyBase::ExEuclid( UInt64 a , UInt64 b , UInt64 &xa , UInt64 &yb )
  {
    UInt64 A=1, B=0, C=0, D=1 , tmp[2] , tmp1 , aa = a , bb = b;
    while ( b != 0 )
    {
      tmp[0] = a/b;
      tmp[1] = a%b;
      a = b;
      b = tmp[1];
      tmp1 = B;
      B = A - tmp[0] * B;
      A = tmp1;
      tmp1 = D;
      D = C - tmp[0] * D;
      C = tmp1;
    }
    xa = A;
    yb = C;
    FullMul( aa , xa , tmp );
    if( LongMod( tmp , bb ) != a )
      {
	xa += bb;
      }
    else
      {
	yb += aa;
      }
    return ( a );
  };
  
  void HighAccuracyBase::FiniteFFT( const UInt64 X[] , int logn , UInt64 w , UInt64 prime , UInt64 Fx[] )
  {
    int n = 1 , i , j , l , parts , members , loc , par ;
    n <<= logn;
    UInt64 trans[( n >> 1 )] , tmp1[2] , tmp2 ; 
    trans[0] = 1;
    trans[1] = w;
   for ( i = logn ; i < 57 ; i++ )
    {
      FullMul( trans[1] , trans[1] , tmp1 );
      trans[1] = LongMod( tmp1 , prime );
    }
    for ( i = 2 ; i < ( n >> 1 ) ; i++ )
    {
      FullMul( trans[i - 1] , trans[1] , tmp1 );
      trans[i] = LongMod( tmp1 , prime );
    }
    for ( i = 0 ; i < n ; i++ )
    {
      Fx[i] = X[i];
    }
    for ( i = 0 ; i < logn ; i++ )
    {
      parts = ( 1 << i );
      members = (( n >> i ) >> 1 ) ;
      for ( j = 0 ; j < parts ; j++ )
      {
        par = j * ( members << 1 );
        for ( l = 0 ; l < members ; l++ )
        {
          loc = par + l;
          tmp2 = Fx[loc] + Fx[loc + members];
          tmp2 = ( tmp2 > prime or tmp2 < Fx[loc] ) ? ( tmp2 - prime ) : tmp2;
          Fx[loc] = ( Fx[loc] > Fx[loc + members] ) ? Fx[loc] : ( Fx[loc] + prime );
          Fx[loc + members] = Fx[loc] - Fx[loc + members];
          Fx[loc] = tmp2;
          FullMul( Fx[loc + members] , trans[l * parts] , tmp1 );
          Fx[loc + members] = LongMod( tmp1 , prime );
        }
      }
    }
    for ( i = 0 ; i < logn ; i++ )
    {
      parts = ( 1 << i );
      members = (( n >> i ) >> 1 ) ;
      for ( j = 0 ; j < parts ; j++ )
      {
        par = j * ( members << 1 );
        for ( l = 0 ; l < members ; l++ )
        {
          trans[l] = Fx[ par + ( l << 1 ) + 1];
        }
        for ( l = 1 ; l < members ; l++ )
        {
          Fx[par + l] = Fx[par + ( l << 1 )];
        }
        for ( l = 0 ; l < members ; l++ )
        {
          Fx[par + l + members] = trans[l];
        }
      }
    }
  };
  
  void HighAccuracyBase::FiniteIFFT( const UInt64 Fx[] , int logn , UInt64 w , UInt64 prime , UInt64 X[] )
  {
  	int i , n;
  	UInt64 w1 , invn , tmp[2];
  	ExEuclid( w , prime , w1 , tmp[0] );
  	n = ( 1 << logn );
  	invn = prime - (( prime - 1 ) >> logn);
  	FiniteFFT( Fx , logn , w1 , prime , X );
  	for ( i = 0 ; i < n ; i++ )
      {
        FullMul( X[i] , invn , tmp );
        X[i] = LongMod( tmp , prime );
      }
  };//can only base on primes given
  
  void HighAccuracyBase::ChRemain( UInt64 a1 , UInt64 a2 , UInt64 n1 , UInt64 n2 , UInt64 result[2] )
  {
    UInt64 b1 , b2 , tmp[2] , tmp1[2];
    if( ExEuclid( n1 , n2 , b1 , b2 ) != 1 )
      {
	cerr << "error in ChRemain()";
	return;
      }
    if ( a1 == a2 )
      {
	result[1] = 0;
	result[0] = a1;
      }
    FullMul( a2 , b1 , tmp );
    b1 = LongMod( tmp , n2 );
    FullMul( a1 , b2 , tmp );
    b2 = LongMod( tmp , n1 );
    FullMul( b2 , n2 , tmp );
    FullMul( b1 , n1 , tmp1 );
    LongAdd( tmp , tmp1 , result );
    FullMul( n1 , n2 , tmp1 );
    if (( result[1] < tmp[1] )or( result[1] == tmp[1] and result[0] < tmp[0]))
      {
	LongMin( result , tmp1 , result );
      }
    while (( result[1] > tmp1[1] )or( result[1] == tmp1[1] and result[0] > tmp1[0] ))
      {
	LongMin( result , tmp1 , result );
      }
  };

  int HighAccuracyBase::LongMul( const UInt32 left[] , const UInt32 right[] , int leftn , int rightn , DataArray * &result )
  {
    if ( leftn == 0 or rightn == 0 ) return 0;
    if ( leftn + rightn < 11400 ) return NormalMul( left , right , leftn , rightn , result );
    return FFTMul( left , right , leftn , rightn , result );
  };
  
  int HighAccuracyBase::FFTMul( const UInt32 left[] , const UInt32 right[] , int leftn , int rightn , DataArray * &result )
  {
    int length = leftn + rightn - 2 , logn ;
    int i;
    for ( i = 31 ; !( length >> i ) and i >= 0 ; i-- );
    if( i > 56 ) return 0;
    length = ( 1 << ( logn = 1 + i ));
    UInt64 (* const tmpresult) = new UInt64[length * 2]; 
    UInt64 (* const Fxy) = new UInt64[length * 2];
    UInt64 (* const fix) = (UInt64 *) tmpresult;
    UInt64 tmp[2];
    UInt64 prime1 = 29;
    prime1 <<= 57 ;
    prime1++;
    UInt64 prime2 = 71;
    prime2 <<= 57 ;
    prime2++;
    for ( i = 0 ; i < leftn ; i++ )
      {
	fix[i] = left[i];
      }
    for ( i = 0 ; i < rightn ; i++ )
      {
	fix[length + i] = right[i];
      }
    for ( i = leftn ; i < length ; i++ )
      {
	fix[i] = 0;
      }
    for ( i = rightn ; i < length ; i++ )
      {
	fix[length + i] = 0;
      }
    FiniteFFT( fix , logn , 21 , prime1 , Fxy );
    FiniteFFT( fix + length , logn , 21 , prime1 , Fxy + length );
    for ( i = 0 ; i < length ; i++ )
      {
	FullMul( Fxy[i] , Fxy[i + length] , tmp );
	Fxy[i] = LongMod( tmp , prime1 );
      }
    FiniteIFFT( Fxy , logn , 21 , prime1 , Fxy );
    FiniteFFT( fix , logn , 287 , prime2 , fix );
    FiniteFFT( fix + length , logn , 287 , prime2 , fix + length );
    for ( i = 0 ; i < length ; i++ )
      {
	FullMul( fix[i] , fix[i + length] , tmp );
	Fxy[i + length] = LongMod( tmp , prime2 );
      }
    FiniteIFFT( Fxy + length , logn , 287 , prime2 , Fxy + length );
    for ( i = 0 ; i < length ; i++ )
      {
	ChRemain( Fxy[i] , Fxy[i + length] , prime1 , prime2 , tmpresult + i * 2 );
      }
    delete []Fxy;
    UInt32 * tmpresult2 = new UInt32[length + 4];
    for ( i = 0 ; i < length + 4 ; i++ )
      {
        tmpresult2[i] = 0;
      }
    int j1 , j2 ;
    for ( i = 0 ; i < length ; i++ )
      {
        j1 = j2 = 0;
        *(UInt64 *)(tmpresult2 + i) += tmpresult[i * 2];
        if ( *(UInt64 *)(tmpresult2 + i) < tmpresult[i * 2] )
          {
            j1++;
          }
        *(UInt64 *)(tmpresult2 + i + 2) += tmpresult[i * 2 + 1];
        if ( *(UInt64 *)(tmpresult2 + i + 2) < tmpresult[i * 2 + 1] )
          {
            j2++;
          }
        *(UInt64 *)(tmpresult2 + i + 2) += j1;
        if( *(UInt64 *)(tmpresult2 + i + 2) < (UInt64)j1 )
          {
            j2++;
          }
        *(tmpresult2 + i + 4) = j2;
      }
    for ( i = length + 3 ; tmpresult2[i] == 0 and i >=0 ; i-- );
    delete []tmpresult;
    result = DataArray::make( length = i + 1 );
    for ( i = 0 ; i < length ; i++ )
      {
        ( *result )[i] = tmpresult2[i];
      }
    delete []tmpresult2;
    return length;
  };

  int HighAccuracyBase::NormalMul( const UInt32 left[] , const UInt32 right[] , int leftn , int rightn , DataArray * & result )
  {
    int length;
    UInt64 * tmpresult = new UInt64[( length = rightn + leftn - 1 ) * 2 ] , tmp1[2];
    int i , j;
    for ( i = 0 ; i < length * 2 ; i++ )
      {
	tmpresult[i] = 0;
      }
    tmp1[1] = 0;
    for ( i = 0 ; i < rightn ; i++ )
      {
	for ( j = 0 ; j < leftn ; j++ )
	  {
	    tmp1[0] = (( UInt64 ) right[i] ) * (( UInt64 ) left[j] );
	    LongAdd( tmp1 , tmpresult + ( i + j ) * 2 , tmpresult + ( i + j ) * 2 );
	  }
      }
    UInt32 * tmpresult2 = new UInt32[length + 4];
    for ( i = 0 ; i < length + 4 ; i++ )
      {
        tmpresult2[i] = 0;
      }
    int j1 , j2 ;
    for ( i = 0 ; i < length ; i++ )
      {
        j1 = j2 = 0;
        *(UInt64 *)(tmpresult2 + i) += tmpresult[i * 2];
        if ( *(UInt64 *)(tmpresult2 + i) < tmpresult[i * 2] )
          {
            j1++;
          }
        *(UInt64 *)(tmpresult2 + i + 2) += tmpresult[i * 2 + 1];
        if ( *(UInt64 *)(tmpresult2 + i + 2) < tmpresult[i * 2 + 1] )
          {
            j2++;
          }
        *(UInt64 *)(tmpresult2 + i + 2) += j1;
        if( *(UInt64 *)(tmpresult2 + i + 2) < (UInt64)j1 )
          {
            j2++;
          }
        *(tmpresult2 + i + 4) = j2;
      }
    for ( i = length + 3 ; tmpresult2[i] == 0 and i >=0 ; i-- );
    delete []tmpresult;
    result = DataArray::make( length = i + 1 );
    for ( i = 0 ; i < length ; i++ )
      {
        ( *result )[i] = tmpresult2[i];
      }
    delete []tmpresult2;
    return length;
  };
  
  int HighAccuracyBase::FFTSquare( const UInt32 value[] , int Length , DataArray * &result )
  {
    int length = ( Length << 1 ) - 2 , logn ;
    int i;
    for ( i = 31 ; !( length >> i ) and i >= 0 ; i-- );
    if( i > 56 ) return 0;
    length = ( 1 << ( logn = 1 + i ));
    UInt64 (* const tmpresult) = new UInt64[length * 2];
    UInt64 tmp[2];
    UInt64 prime1 = 29;
    prime1 <<= 57 ;
    prime1++;
    UInt64 prime2 = 71;
    prime2 <<= 57 ;
    prime2++;
    for ( i = 0 ; i < Length ; i++ )
      {
	tmpresult[i] = value[i];
      }
    for ( i = Length ; i < length ; i++ )
      {
	tmpresult[i] = 0;
      }
    FiniteFFT( tmpresult , logn , 21 , prime1 , tmpresult + length );
    for ( i = 0 ; i < length ; i++ )
      {
	FullMul( tmpresult[ i + length ] , tmpresult[ i + length ] , tmp );
	tmpresult[ i + length ] = LongMod( tmp , prime1 );
      }
    FiniteIFFT( tmpresult + length , logn , 21 , prime1 , tmpresult + length );
    FiniteFFT( tmpresult , logn , 287 , prime2 , tmpresult );
    for ( i = 0 ; i < length ; i++ )
      {
	FullMul( tmpresult[i] , tmpresult[i] , tmp );
	tmpresult[i] = LongMod( tmp , prime2 );
      }
    FiniteIFFT( tmpresult , logn , 287 , prime2 , tmpresult );
    UInt64 * tmpresult3 = new UInt64[ length * 2 ];
    for ( i = 0 ; i < length ; i++ )
      {
	ChRemain( tmpresult[i] , tmpresult[i + length] , prime2 , prime1 , tmpresult3 + i * 2 );
      }
    delete []tmpresult;
    UInt32 * tmpresult2 = new UInt32[length + 4];
    for ( i = 0 ; i < length + 4 ; i++ )
      {
        tmpresult2[i] = 0;
      }
    int j1 , j2 ;
    for ( i = 0 ; i < length ; i++ )
      {
        j1 = j2 = 0;
        *(UInt64 *)(tmpresult2 + i) += tmpresult3[i * 2];
        if ( *(UInt64 *)(tmpresult2 + i) < tmpresult3[i * 2] )
          {
            j1++;
          }
        *(UInt64 *)(tmpresult2 + i + 2) += tmpresult3[i * 2 + 1];
        if ( *(UInt64 *)(tmpresult2 + i + 2) < tmpresult3[i * 2 + 1] )
          {
            j2++;
          }
        *(UInt64 *)(tmpresult2 + i + 2) += j1;
        if( *(UInt64 *)(tmpresult2 + i + 2) < (UInt64)j1 )
          {
            j2++;
          }
        *(tmpresult2 + i + 4) = j2;
      }
    delete []tmpresult3;
    for ( i = length + 3 ; tmpresult2[i] == 0 and i >=0 ; i-- );
    result = DataArray::make( length = i + 1 );
    for ( i = 0 ; i < length ; i++ )
      {
        ( *result )[i] = tmpresult2[i];
      }
    delete []tmpresult2;
    return length;
  };

  int HighAccuracyBase::Square( const UInt32 value[] , int length , DataArray * &result )
  {
    if ( length == 0 ) return 0;
    if ( length < 4000 ) return NormalMul( value , value , length , length , result );
    return FFTSquare( value , length , result );
  };
}
