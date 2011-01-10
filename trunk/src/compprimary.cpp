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
   ComplexHA PrimaryFunction::AGM( const ComplexHA & a , const ComplexHA & b , int length )
   {
      if(a.IsSpecial())
	 return ComplexHA::NaN;
      ComplexHA x,y,t;
      if(a.IsZero()||b.IsZero())
	 return ComplexHA::Zero;
      if(a==b) return a;
      x=a;
      y=b;
      int l=length+2;
      static double log2 = log(2);
      if(length>2) l+=int(log(log(length)/log2)/log2);
      while (ComplexHA::MSquare(x-y)>(ComplexHA::MSquare(y)<<(-length)))
      {
	 t=x;
	 x=(x+y)*ComplexHA::OneOverTwo;
	 y=ComplexHA::Sqrt(y*t,l);
	 x.LimitLength(l);
      }
      return ComplexHA::Divide(x+y,2,length);
   };

  HighAccuracyNumber PrimaryFunction::Arg( const ComplexHA & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  ComplexHA PrimaryFunction::Sin( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Cos( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Tan( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Cot( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Sec( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Csc( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::ArcSin( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::ArcCos( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::ArcTan( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::ArcCot( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::ArcSec( const ComplexHA & , int )
  {

     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::ArcCsc( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Exp( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };

  ComplexHA PrimaryFunction::Ln( const ComplexHA & value, int length )
   {
      if ( value == ComplexHA::One )
	 return ComplexHA::Zero;
      if ( ComplexHA::MSquare(value) < HighAccuracyNumber::One )
	 return -PrimaryFunction::Ln(ComplexHA::Divide( ComplexHA::One , value , length + 1 ) , length );
      if ( ComplexHA::MSquare(value - ComplexHA::One) < HighAccuracyNumber::One << (2*(( length > 1 ) ? -(int)( length / log( length )): -1 )))
      {
	 ComplexHA result,x,y,z;
	 int i = 1;
	 result = value - ComplexHA::One;
	 y = -value;
	 x = result * y;
	 while(ComplexHA::MSquare( z = ComplexHA::Divide( x , ++i , length + 2 )) > ( ComplexHA::MSquare ( result ) << ( -length )))
	 {
	    result = result + z;
	    x=result*y;
	 }
	 return result;
      }
      int nx = (( value.GetRe().GetPoint() + value.GetRe().GetLength() + value.GetIm().GetPoint() + value.GetIm().GetLength())>>1) - 1;
      if ( 2 * nx > length )
	 return ComplexHA::Divide( PrimaryFunction::ValueOfPi( length + 2 ) , ComplexHA::Two * PrimaryFunction::AGM( ComplexHA::One , ComplexHA::Divide( 4 , value , length + 2 ) , length + 2 ) , length );
      int l = length + (int)log( length ) + 1;
      nx = (( 1 + l ) >> 1 ) - nx;
      ComplexHA temp=ComplexHA(value.GetRe()<<nx,value.GetIm()<<nx);
      ComplexHA result = ComplexHA::Divide( PrimaryFunction::ValueOfPi( l + 2 ) , ComplexHA::Two * PrimaryFunction::AGM(ComplexHA::One , ComplexHA::Divide( 4 , temp , l + 2 ) , l + 2 ) , l+1);
      result = result - nx * PrimaryFunction::LnB( l );
      result.LimitLength( length );
      return result;
   };

  ComplexHA PrimaryFunction::Power( const ComplexHA & , const ComplexHA & , int )
  {
    
     return ComplexHA();
  };

  ComplexHA PrimaryFunction::Log( const ComplexHA & , const ComplexHA & , int )
  {

     return ComplexHA();
  };
}
