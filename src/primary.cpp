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
   
   ComplexHA PrimaryFunction::AGM( const ComplexHA & , const ComplexHA & , int )
   {
     
     return ComplexHA();
   };

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

  HighAccuracyNumber PrimaryFunction::Arg( const ComplexHA & , int )
  {
    
    return HighAccuracyNumber();
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
  
  HighAccuracyNumber PrimaryFunction::Exp( const HighAccuracyNumber & , int )
  {
    
    return HighAccuracyNumber();
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

  ComplexHA PrimaryFunction::Ln( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Power( const ComplexHA & , const ComplexHA & , int )
  {
    
     return ComplexHA();
  };

  ComplexHA PrimaryFunction::Log( const ComplexHA & , const ComplexHA & , int )
  {

     return ComplexHA();
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
  
  NumberType PrimaryFunction::Exp( const NumberType & , int )
  {
    
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

