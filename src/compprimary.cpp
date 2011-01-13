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

  HighAccuracyNumber PrimaryFunction::Arg( const ComplexHA & value, int length)
  {
    return PrimaryFunction::Ln(value,length).GetIm();
  };
  
  ComplexHA PrimaryFunction::Sin( const ComplexHA & value , int length )
  {
     return ComplexHA::Divide(PrimaryFunction::Exp(ComplexHA(-value.GetIm(),value.GetRe()),length+1)-PrimaryFunction::Exp(ComplexHA(value.GetIm(),-value.GetRe()),length+1),ComplexHA::TwoI,length);
  };
  
  ComplexHA PrimaryFunction::Cos( const ComplexHA & value , int length )
  {
     return ComplexHA::Divide(PrimaryFunction::Exp(ComplexHA(-value.GetIm(),value.GetRe()),length+1)+PrimaryFunction::Exp(ComplexHA(value.GetIm(),-value.GetRe()),length+1),ComplexHA::Two,length);
  };
  
  ComplexHA PrimaryFunction::Tan( const ComplexHA & value, int length )
  {
     ComplexHA temp=PrimaryFunction::Cos(value,length+1);
     if (temp.IsZero())
	return ComplexHA::NaN; 
     return ComplexHA::Divide(PrimaryFunction::Sin(value,length+1),temp,length);
  };
  
  ComplexHA PrimaryFunction::Cot( const ComplexHA & value , int length)
  {
     ComplexHA temp=PrimaryFunction::Sin(value,length+1);
     if (temp.IsZero())
	return ComplexHA::NaN; 
     return ComplexHA::Divide(PrimaryFunction::Cos(value,length),temp,length);
  };
  
  ComplexHA PrimaryFunction::Sec( const ComplexHA & value, int length)
  {
     ComplexHA temp=PrimaryFunction::Cos(value,length+1);
     if (temp.IsZero())
	return ComplexHA::NaN; 
     return ComplexHA::Divide(ComplexHA::One,temp,length);
  };
  
  ComplexHA PrimaryFunction::Csc( const ComplexHA & value, int length)
  {
     ComplexHA temp=PrimaryFunction::Sin(value,length+1);
     if (temp.IsZero())
	return ComplexHA::NaN; 
     return ComplexHA::Divide(ComplexHA::One,temp,length);
  };
  
  ComplexHA PrimaryFunction::ArcSin( const ComplexHA & value, int length)
  {
     ComplexHA temp=PrimaryFunction::Ln(ComplexHA(-value.GetIm(),value.GetRe())+ComplexHA::Sqrt(ComplexHA::One-ComplexHA::Square(value),length+1),length);
     return ComplexHA(temp.GetIm(),-temp.GetRe());
  };
  
  ComplexHA PrimaryFunction::ArcCos( const ComplexHA & value, int length)
  {
     ComplexHA temp=ComplexHA::Sqrt(ComplexHA::One-ComplexHA::Square(value),length+1);
     temp=PrimaryFunction::Ln(value+ComplexHA(-temp.GetIm(),temp.GetRe()),length);
     return ComplexHA(temp.GetIm(),-temp.GetRe());
  };
  
  ComplexHA PrimaryFunction::ArcTan( const ComplexHA & value, int length)
  {
     return ComplexHA::Divide(PrimaryFunction::Ln(ComplexHA::Divide(ComplexHA::One+ComplexHA(-value.GetIm(),value.GetRe()),ComplexHA::One+ComplexHA(value.GetIm(),-value.GetRe()),length+2),length+1),ComplexHA::TwoI,length);
  };
  
  ComplexHA PrimaryFunction::ArcCot( const ComplexHA & value, int length)
  {
     if (value.IsZero())
	return ComplexHA(HighAccuracyNumber::Divide(PrimaryFunction::ValueOfPi(length),HighAccuracyNumber::Two,length));
     return PrimaryFunction::ArcTan(ComplexHA::Divide(ComplexHA::One,value,length+1),length);
  };
  
  ComplexHA PrimaryFunction::ArcSec( const ComplexHA & value, int length)
  {
     if (value.IsZero())
	return ComplexHA::NaN;
     return PrimaryFunction::ArcCos(ComplexHA::Divide(ComplexHA::One,value,length+1),length);
  };
  
  ComplexHA PrimaryFunction::ArcCsc( const ComplexHA & value, int length)
  {
     if (value.IsZero())
	return ComplexHA::NaN;
     return PrimaryFunction::ArcSin(ComplexHA::Divide(ComplexHA::One,value,length+1),length);
  };
  
  ComplexHA PrimaryFunction::Exp( const ComplexHA & value , int length)
  {
      if (value.GetRe().GetSign()>0)
      {
	 if(value.GetRe().GetPoint()+value.GetRe().GetLength()>1)
	    throw "OverFlow";
      }
      else if (value.IsNaN())
	 return ComplexHA::NaN;
      else if (value.IsZero())
	 return ComplexHA::One;
      if(length<=0)
	 length=value.GetLength();
      ComplexHA result=ComplexHA(PrimaryFunction::Exp(value.GetRe(),2));
      ComplexHA temp;
      while (ComplexHA::MSquare(temp=PrimaryFunction::RePartModTwoPi(value-PrimaryFunction::Ln(result,2),2))>HighAccuracyNumber::One)
      {
	 ComplexHA a=temp,b=ComplexHA::One,c=ComplexHA::One,d=ComplexHA::One;
	 while(ComplexHA::MSquare(a)>=ComplexHA::MSquare(b))
	 {
	    d=d+ComplexHA::Divide(a,b,2);
	    a=a*temp;
	    b=b*(c=c+1);
	 }
	 result=result*d;
	 result.LimitLength(2);
      }
      int l=length+1;
      while ((((temp=PrimaryFunction::RePartModTwoPi(value-PrimaryFunction::Ln(result,l+1),l+1)).GetRe().GetLength()+temp.GetRe().GetPoint()+length>0)&&(!temp.GetRe().IsZero()))||((temp.GetIm().GetLength()+temp.GetIm().GetPoint()+length>0)&&(!temp.GetIm().IsZero())))
      {
	 result=result*(ComplexHA::One+temp);
	 result.LimitLength(l+1);
      }
      result.LimitLength(length);
      return result;
  };

  ComplexHA PrimaryFunction::Ln( const ComplexHA & value, int length )
   {
      if (value.IsSpecial())
	 return ComplexHA::NaN;
      if ( value == ComplexHA::One )
	 return ComplexHA::Zero;
      if ( value == ComplexHA::Zero )
	 throw "ERROR Log 0";
      if ( value.GetRe()<0)
      {
	 ComplexHA temp=PrimaryFunction::Ln(-value,length+1);
	 if (temp.GetIm()>0)
	    temp.SetIm(temp.GetIm()-PrimaryFunction::ValueOfPi(1+length-temp.GetFP()));
	 else if (temp.GetIm()<0)
	    temp.SetIm(temp.GetIm()+PrimaryFunction::ValueOfPi(1+length-temp.GetFP()));
	 else
	    temp.SetIm(PrimaryFunction::ValueOfPi(1+length-temp.GetRe().GetPoint()-temp.GetRe().GetLength()));
	 temp.LimitLength(length);
	 return temp;
      }
      if ( ComplexHA::MSquare(value) < HighAccuracyNumber::One )
	 return -PrimaryFunction::Ln(ComplexHA::Divide( ComplexHA::One , value , length + 1 ) , length );
      if ( ComplexHA::MSquare(value - ComplexHA::One) < HighAccuracyNumber::One << ((( length > 1 ) ? -(int)((length-1)/log(length)): -1 )))
      {
	 ComplexHA result,x,y,z;
	 int i = 1,l;
	 result = value - ComplexHA::One;
	 l=length+1-(result.GetRe().GetLength()+result.GetRe().GetPoint()<result.GetIm().GetLength()+result.GetIm().GetPoint()?result.GetRe().GetLength()+result.GetRe().GetPoint():result.GetIm().GetLength()+result.GetIm().GetPoint());
	 y = -result;
	 x= result * y;
	 while(ComplexHA::MSquare( z = ComplexHA::Divide( x , ComplexHA(++i) , l )) > ( ComplexHA::MSquare ( result ) << ( -length )))
	 {
	    result = result + z;
	    x=x*y;
	 }
	 return result;
      }
      int nx = (( value.GetRe().GetPoint() + value.GetRe().GetLength() + value.GetIm().GetPoint() + value.GetIm().GetLength())>>1) - 1;
      if ( 2 * nx > length )
      {
	 ComplexHA result= ComplexHA::Divide( PrimaryFunction::ValueOfPi( length + 2 ) , ComplexHA::Two * PrimaryFunction::AGM( ComplexHA::One , ComplexHA::Divide( 4 , value , length + 2 ) , length + 2 ) , length );
         return result;
      }
      int l = length + (int)log( length ) + 1;
      nx = (( 1 + l ) >> 1 ) - nx+2;
      ComplexHA temp=ComplexHA(value.GetRe()<<nx,value.GetIm()<<nx);
      ComplexHA result = ComplexHA::Divide( PrimaryFunction::ValueOfPi( l + 2 ) , ComplexHA::Two * PrimaryFunction::AGM(ComplexHA::One , ComplexHA::Divide( 4 , temp , l + 2 ) , l + 2 ) , l+1);
      result = result - nx * PrimaryFunction::LnB( l );
      result.LimitLength( length );
      return result;
   };

  ComplexHA PrimaryFunction::Power( const ComplexHA & a, const ComplexHA & b, int length)
  {
      return PrimaryFunction::Exp(b*PrimaryFunction::Ln(a,length+(b.GetRe().GetPoint()+b.GetRe().GetLength()>b.GetIm().GetLength()+b.GetIm().GetPoint()?b.GetRe().GetPoint()+b.GetRe().GetLength():b.GetIm().GetPoint()+b.GetIm().GetLength())),length);
  };

  ComplexHA PrimaryFunction::Log( const ComplexHA & a, const ComplexHA & b, int length)
  {
    return ComplexHA::Divide(PrimaryFunction::Ln(b,length+1),PrimaryFunction::Ln(a,length+1),length);
  }

  ComplexHA PrimaryFunction::RePartModTwoPi( const ComplexHA & value , int length )
  {
     if ((value.GetIm()<PrimaryFunction::ValueOfPi(length+1))&&((-value.GetIm())<PrimaryFunction::ValueOfPi(length+1)))
	return value;
     HighAccuracyNumber temp;
     temp=HighAccuracyNumber::Divide(value.GetIm(),HighAccuracyNumber::Two*PrimaryFunction::ValueOfPi(1+value.GetIm().GetPoint()+value.GetIm().GetLength()),1+value.GetIm().GetPoint()+value.GetIm().GetLength());;
     if (temp.GetLength()+temp.GetPoint()>0)
     {
	temp.CutTo(temp.GetLength()+temp.GetPoint());
	temp=value.GetIm()-temp*HighAccuracyNumber::Two*PrimaryFunction::ValueOfPi(length+temp.GetLength());
     }
     else
	temp=value.GetIm();
     if (temp>PrimaryFunction::ValueOfPi(length+1))
	temp=temp-HighAccuracyNumber::Two*PrimaryFunction::ValueOfPi(length+1);
     else if ((-temp)>PrimaryFunction::ValueOfPi(length+1))
	temp=temp+HighAccuracyNumber::Two*PrimaryFunction::ValueOfPi(length+1);
     temp.CutTo(length+temp.GetPoint()+temp.GetLength());
     return ComplexHA(value.GetRe(),temp);
  }
}
