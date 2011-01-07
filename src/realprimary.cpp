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
   const HighAccuracyNumber PrimaryFunction::Two = 2;

   const HighAccuracyNumber PrimaryFunction::OneOverFour = 0.25;

   HighAccuracyNumber PrimaryFunction::AGM( const HighAccuracyNumber & a , const HighAccuracyNumber & b , int length )
   {
      if(a.IsNaN()||(a.GetSign()<0)||b.IsSpecial()||(b.GetSign()<0))
	 return HighAccuracyNumber::NaN;
      if(a.IsInfinity()||b.IsInfinity())
	 return HighAccuracyNumber::Infinity;
      HighAccuracyNumber x,y,t;
      if(a.IsZero()||b.IsZero())
	 return HighAccuracyNumber::Zero;
      if(a>b){x=a;y=b;}
      else if(a<b){x=b;y=a;}
      else if(a==b) return a;
      else
	 return HighAccuracyNumber::NaN;
      int l=length+1;
      static double log2 = log(2);
      if(length>2) l+=int(log(log(length)/log2)/log2);
      while (x-y>(y<<(-length)))
      {
	 t=x;
	 x=(x+y)*HighAccuracyNumber::OneOverTwo;
	 y=HighAccuracyNumber::Sqrt(y*t,l);
	 x.CutTo(l);
      }
      return HighAccuracyNumber::Divide(x+y,2,length);
   };
  HighAccuracyNumber PrimaryFunction::ValueOfE( int )
  {

    return HighAccuracyNumber();
  };

  HighAccuracyNumber PrimaryFunction::ValueOfPi( int length )
  {
    if ( length <= 0 ) return 3;
     ++length;
     static HighAccuracyNumber result;
     if ( result.GetLength() >= length )
     {
	HighAccuracyNumber temp = result;
	temp.CutTo( length );
	return temp;
     }
     int l;
     static double log2 = log( 2 );
     switch ( length )
       {
       case 1:
       case 2:
	 l = length + 1;
       default:
	 l = length + int( log( log( length ) / log2 ) / log2 ) + 1;
       };
     HighAccuracyNumber a = 1;
     HighAccuracyNumber b = HighAccuracyNumber::InverseSqrt( Two , l );
     HighAccuracyNumber t = OneOverFour , x = 1 , y
       , c = HighAccuracyNumber::One << ( 1 - length );
     while ( a - b > c )
     {
	y = a;
	a = ( a + b ) * HighAccuracyNumber::OneOverTwo;
	b = HighAccuracyNumber::Sqrt( y * b , l );
	t = t - x * HighAccuracyNumber::Square( a - y );
	x = Two * x;
	a.CutTo( l );
	t.CutTo( l );
     }
     result = HighAccuracyNumber::Divide( HighAccuracyNumber::Square( a + b )
					  , 4 * t , length );
     return result;
  };
  
  HighAccuracyNumber PrimaryFunction::Sin( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::Cos( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::Tan( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::Cot( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::Sec( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::Csc( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::ArcSin( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::ArcCos( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::ArcTan( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::ArcCot( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::ArcSec( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::ArcCsc( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::Exp( const HighAccuracyNumber &value , int length )
  {
      HighAccuracyNumber result=HighAccuracyNumber::Divide(value,22,1),temp;
      if (result.GetSign()>0)
      {
	 if (result.IsInfinity()||(result.GetPoint()>0)||((result.GetPoint()==0)&&(result[0]>>31)))
	    throw "OverFlow";
      }
      else if (result.GetSign()<0)
      {
	 if (result.IsInfinity()||(result.GetPoint()>0)||((result.GetPoint()==0)&&(result[0]>>31)))
	    return HighAccuracyNumber::Zero;
      }
      else if (result.IsNaN())
	 return HighAccuracyNumber::NaN;
      else return HighAccuracyNumber::One;
      if (result.GetPoint()==0)
	 result=HighAccuracyNumber::One<<(result.GetSign()*result[0]);
      else
	 result=HighAccuracyNumber::One;
      while(((temp=value-PrimaryFunction::Ln(result,length+1))>HighAccuracyNumber::One<<(-length))||(-temp>HighAccuracyNumber::One<<(-length)))
      {
	 result=result*(HighAccuracyNumber::One+temp);
	 result.CutTo(length+1);
      }
      return result;
  };

   HighAccuracyNumber PrimaryFunction::Ln( const HighAccuracyNumber & value , int length )
   {
      if ( value.GetSign() <= 0)
	 return HighAccuracyNumber::NaN;
      if ( value.IsInfinity() )
	 return HighAccuracyNumber::Infinity;
      if ( value == HighAccuracyNumber::One )
	 return HighAccuracyNumber::Zero;
      if ( value < HighAccuracyNumber::One )
	 return -PrimaryFunction::Ln(HighAccuracyNumber::Divide( HighAccuracyNumber::One , value , length + 1 ) , length );
      if ( value - HighAccuracyNumber::One < HighAccuracyNumber::One << (( length > 1 ) ? -(int)( length / log( length )): -1 ))
      {
	 HighAccuracyNumber result,x,y,z;
	 int i = 1;
	 result = value - HighAccuracyNumber::One;
	 y = -value;
	 x = result * y;
	 if (( z = HighAccuracyNumber::Divide( x , ++i , length + 1 )) > ( result << ( -length )))
	    result = result + z;
	 return result;
      }
      int nx = value.GetPoint() + value.GetLength() - 1;
      if ( 2 * nx > length )
	 return HighAccuracyNumber::Divide( PrimaryFunction::ValueOfPi( length + 1 ) , PrimaryFunction::Two * PrimaryFunction::AGM( HighAccuracyNumber::One , HighAccuracyNumber::Divide( 4 , value , length + 1 ) , length + 1 ) , length );
      int l = length + log( length ) + 1;
      nx = (( 1 + l ) >> 1 ) - nx;
      HighAccuracyNumber result = HighAccuracyNumber::Divide( PrimaryFunction::ValueOfPi( l + 1 ) , PrimaryFunction::Two * PrimaryFunction::AGM( HighAccuracyNumber::One , HighAccuracyNumber::Divide( 4 , value << nx , l + 1 ) , l + 1 ) , l );
      result = result - nx * PrimaryFunction::LnB( l );
      result.CutTo( length );
      return result;
   };

   HighAccuracyNumber PrimaryFunction::LnB( int length )
   {
      static HighAccuracyNumber result;
      static int l = 0;
      if( l >= length )
      {
	 HighAccuracyNumber temp = result;
	 temp.CutTo( length );
	 return temp;
      }
      l = ( length + 1 ) >> 1;
      result = HighAccuracyNumber::Divide( PrimaryFunction::ValueOfPi( length + 1 ) , ( l << 1 ) * PrimaryFunction::AGM( HighAccuracyNumber::One , HighAccuracyNumber::Divide( 4 , HighAccuracyNumber::One << l , length + 1 ), length + 1 ) , length );
      l = length;
      return result;
   }

  HighAccuracyNumber PrimaryFunction::Power( const HighAccuracyNumber & , const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  HighAccuracyNumber PrimaryFunction::Log( const HighAccuracyNumber & , const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
  };
}
