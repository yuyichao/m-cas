#include "number.so.h"

using namespace std;

namespace CAS
{
  const NumberType NumberType::RealInfinity = ComplexHA::RealInfinity;

  const NumberType NumberType::ImagInfinity = ComplexHA::ImagInfinity;

  const NumberType NumberType::I = complex<double>( 0 , 1 );

  const NumberType NumberType::Zero = 0;

  const NumberType NumberType::NaN = NumberType();
  
  const NumberType NumberType::One = NumberType( 1 );
  
  NumberType::NumberType(double value)
    :  NumType( Double )
  {
    Number.DoubleNumber = value;
  };
  
  NumberType::NumberType(int value)
    : NumType( Int )
  {
    Number.IntNumber = value;
  };
  
  NumberType::NumberType()
    : NumType( None ) {};

  NumberType::NumberType( const complex<double> & value )
    : NumType( DoubleComplex )
  {
    Number.DComplex = new complex<double>( value );
  };

  NumberType::NumberType( const ComplexHA & value )
    : NumType( HighComplex )
  {
     Number.HAComplex = new ComplexHA( value );
  };

  NumberType::NumberType( const HighAccuracyNumber & value )
    : NumType( HighReal )
  {
     Number.HReal = new HighAccuracyNumber( value );
  };

  NumberType::NumberType( const NumberType & value )
    : NumType( value.NumType )
  {
    switch ( NumType )
      {
      case None:
      case Int:
      case Double:
	Number = value.Number;
	break;
      case HighReal:
	Number.HReal = new HighAccuracyNumber( *value.Number.HReal );
	break;
      case DoubleComplex:
	Number.DComplex = new complex<double>( *value.Number.DComplex );
	break;
      case HighComplex:
	Number.HAComplex = new ComplexHA( *value.Number.HAComplex );
	break;
      };
  };

  NumberType::~NumberType()
  {
    switch ( NumType )
      {
      case None:
      case Int:
      case Double:
	break;
      case HighReal:
	delete Number.HReal;
	break;
      case DoubleComplex:
	delete Number.DComplex;
	break;
      case HighComplex:
	delete Number.HAComplex;
	break;
      };
  };

  NumberType & NumberType::operator = ( const NumberType & value )
  {
    if ( &value == this ) return *this;
    switch ( NumType )
      {
      case None:
      case Int:
      case Double:
	break;
      case HighReal:
	delete Number.HReal;
	break;
      case DoubleComplex:
	delete Number.DComplex;
	break;
      case HighComplex:
	delete Number.HAComplex;
	break;
      };
    NumType = value.NumType;
    switch ( NumType )
      {
      case None:
      case Int:
      case Double:
	Number = value.Number;
	return *this;
      case HighReal:
	Number.HReal = new HighAccuracyNumber( * value.Number.HReal );
	return *this;
      case DoubleComplex:
	Number.DComplex = new complex<double>( * value.Number.DComplex );
	return *this;
      case HighComplex:
	Number.HAComplex = new ComplexHA( * value.Number.HAComplex );
	return *this;
      };
    return *this;
  };
  
  NumberType NumberType::operator + ( const NumberType & Right ) const
  {
    if ( NumType == None || Right.NumType == None )
      {
	return NumberType();
      }
    if ( NumType == Int && Right.NumType == Int )
      {
	if ( ( Number.IntNumber > 0 and Right.Number.IntNumber > 0 and Number.IntNumber + Right.Number.IntNumber < 0 ) or ( Number.IntNumber < 0 and Right.Number.IntNumber < 0 and Number.IntNumber + Right.Number.IntNumber > 0 ))
	  return HighAccuracyNumber( Number.IntNumber ) + Right.Number.IntNumber;
	return Number.IntNumber + Right.Number.IntNumber;
      }
    if ( NumType == Int && Right.NumType == Double )
      {
	return Number.IntNumber + Right.Number.DoubleNumber;
      }
    if ( NumType == Double && Right.NumType == Int )
      {
	return Number.DoubleNumber + Right.Number.IntNumber;
      }
    if ( NumType == Double && Right.NumType == Double )
      {
	return Number.DoubleNumber + Right.Number.DoubleNumber;
      }
    if ( NumType == Int && Right.NumType == HighReal )
      {
	return *(Right.Number.HReal) + Number.IntNumber;
      }
    if ( NumType == Int && Right.NumType == DoubleComplex )
      {
	return *Right.Number.DComplex + (complex<double>)Number.IntNumber;
      }
    if ( NumType == Int && Right.NumType == HighComplex )
      {
	return ComplexHA(Number.IntNumber) + *(Right.Number.HAComplex);
      }
    if ( NumType == Double && Right.NumType == HighReal )
      {
	return Number.DoubleNumber + *(Right.Number.HReal);
      }
    if ( NumType == Double && Right.NumType == DoubleComplex )
      {
	return Number.DoubleNumber + *(Right.Number.DComplex);
      }
    if ( NumType == Double && Right.NumType == HighComplex )
      {
	return ComplexHA(Number.DoubleNumber) + *(Right.Number.HAComplex);
      }
    if ( NumType == HighReal && Right.NumType == DoubleComplex )
      {
	return ComplexHA(*(Number.HReal)) + *(Right.Number.DComplex);
      }
    if ( NumType == HighReal && Right.NumType == HighComplex )
      {
	return ComplexHA(*(Number.HReal)) + *(Right.Number.HAComplex);
      }
    if ( NumType == DoubleComplex && Right.NumType == HighComplex )
      {
	return ComplexHA(*(Number.DComplex)) + *(Right.Number.HAComplex);
      }
    if ( NumType == DoubleComplex && Right.NumType == DoubleComplex )
      {
	return *(Number.DComplex) + *(Right.Number.DComplex);
      }
    if ( NumType == HighReal && Right.NumType == HighReal )
      {
	return *(Number.HReal) + *(Right.Number.HReal);
      }
    if ( NumType == HighComplex && Right.NumType == HighComplex )
      {
	return *(Number.HAComplex) + *(Right.Number.HAComplex);
      }
    return Right + *this;
  };
  
  NumberType NumberType::operator - ( const NumberType & Right ) const
  {
    return *this + ( -Right );
  };
  
  NumberType NumberType::operator * ( const NumberType & Right ) const
  {
    if( NumType == None || Right.NumType == None )
      {
	return NumberType();
      }
    if ( NumType == Int && Right.NumType == Int )
      {
	Int64 left = Number.IntNumber , right = Right.Number.IntNumber , result = left * right;
	result = result > 0 ? result : -result;
	if (( result >> 31 ) == 0 )
	  return Number.IntNumber * Right.Number.IntNumber;
	return HighAccuracyNumber( Number.IntNumber ) * Right.Number.IntNumber;
      }
    if ( NumType == Int && Right.NumType == Double )
      {
	return Number.IntNumber * Right.Number.DoubleNumber;
      }
    if ( NumType == Double && Right.NumType == Int )
      {
	return Number.DoubleNumber * Right.Number.IntNumber;
      }
    if ( NumType == Double && Right.NumType == Double )
      {
	return Number.DoubleNumber * Right.Number.DoubleNumber;
      }
    if ( NumType == Int && Right.NumType == HighReal )
      {
	return *(Right.Number.HReal) * Number.IntNumber;
      }
    if ( NumType == Int && Right.NumType == DoubleComplex )
      {
	return *Right.Number.DComplex * (complex<double>)Number.IntNumber;
      }
    if ( NumType == Int && Right.NumType == HighComplex )
      {
	return ComplexHA(Number.IntNumber) * *(Right.Number.HAComplex);
      }
    if ( NumType == Double && Right.NumType == HighReal )
      {
	return Number.DoubleNumber * *(Right.Number.HReal);
      }
    if ( NumType == Double && Right.NumType == DoubleComplex )
      {
	return Number.DoubleNumber * *(Right.Number.DComplex);
      }
    if ( NumType == Double && Right.NumType == HighComplex )
      {
	return ComplexHA(Number.DoubleNumber) * *(Right.Number.HAComplex);
      }
    if ( NumType == HighReal && Right.NumType == DoubleComplex )
      {
	return ComplexHA(*(Number.HReal)) * *(Right.Number.DComplex);
      }
    if ( NumType == HighReal && Right.NumType == HighComplex )
      {
	return ComplexHA(*(Number.HReal)) * *(Right.Number.HAComplex);
      }
    if ( NumType == DoubleComplex && Right.NumType == HighComplex )
      {
	return ComplexHA(*(Number.DComplex)) * *(Right.Number.HAComplex);
      }
    if ( NumType == DoubleComplex && Right.NumType == DoubleComplex )
      {
	return *(Number.DComplex) * *(Right.Number.DComplex);
      }
    if ( NumType == HighReal && Right.NumType == HighReal )
      {
	return *(Number.HReal) * *(Right.Number.HReal);
      }
    if ( NumType == HighComplex && Right.NumType == HighComplex )
      {
	return *(Number.HAComplex) * *(Right.Number.HAComplex);
      }
    return Right * *this;
  };
  
  NumberType NumberType::operator / ( const NumberType & Right ) const
  {
    if ( NumType == None or Right.NumType == None )
      return NumberType();
    if ( NumType == Int && Right.NumType == Int )
      {
	return HighAccuracyNumber( Number.IntNumber ) / Right.Number.IntNumber;
      }
    if ( NumType == Int && Right.NumType == Double )
      {
	return Number.IntNumber / Right.Number.DoubleNumber;
      }
    if ( NumType == Double && Right.NumType == Int )
      {
	return Number.DoubleNumber / Right.Number.IntNumber;
      }
    if ( NumType == Double && Right.NumType == Double )
      {
	return Number.DoubleNumber / Right.Number.DoubleNumber;
      }
    if ( NumType == Int && Right.NumType == HighReal )
      {
	return Number.IntNumber / *(Right.Number.HReal);
      }
    if ( NumType == Int && Right.NumType == DoubleComplex )
      {
	return (complex<double>)Number.IntNumber / *Right.Number.DComplex;
      }
    if ( NumType == Int && Right.NumType == HighComplex )
      {
	return ComplexHA(Number.IntNumber) / *(Right.Number.HAComplex);
      }
    if ( NumType == Double && Right.NumType == HighReal )
      {
	return Number.DoubleNumber / *(Right.Number.HReal);
      }
    if ( NumType == Double && Right.NumType == DoubleComplex )
      {
	return Number.DoubleNumber / *(Right.Number.DComplex);
      }
    if ( NumType == Double && Right.NumType == HighComplex )
      {
	return ComplexHA(Number.DoubleNumber) / *(Right.Number.HAComplex);
      }
    if ( NumType == HighReal && Right.NumType == DoubleComplex )
      {
	return ComplexHA(*(Number.HReal)) / *(Right.Number.DComplex);
      }
    if ( NumType == HighReal && Right.NumType == HighComplex )
      {
	return ComplexHA(*(Number.HReal)) / *(Right.Number.HAComplex);
      }
    if ( NumType == DoubleComplex && Right.NumType == HighComplex )
      {
	return ComplexHA(*(Number.DComplex)) / *(Right.Number.HAComplex);
      }
    if ( NumType == DoubleComplex && Right.NumType == DoubleComplex )
      {
	return *(Number.DComplex) / *(Right.Number.DComplex);
      }
    if ( NumType == HighReal && Right.NumType == HighReal )
      {
	return *(Number.HReal) / *(Right.Number.HReal);
      }
    if ( NumType == HighComplex && Right.NumType == HighComplex )
      {
	return *(Number.HAComplex) / *(Right.Number.HAComplex);
      }
    if ( Right.NumType == Int && NumType == HighReal )
      {
	return *(Number.HReal) / Right.Number.IntNumber;
      }
    if ( Right.NumType == Int && NumType == DoubleComplex )
      {
	return *Number.DComplex / (complex<double>)Right.Number.IntNumber;
      }
    if ( Right.NumType == Int && NumType == HighComplex )
      {
	return *(Number.HAComplex) / ComplexHA(Right.Number.IntNumber);
      }
    if ( Right.NumType == Double && NumType == HighReal )
      {
	return *(Number.HReal) / Right.Number.DoubleNumber;
      }
    if ( Right.NumType == Double && NumType == DoubleComplex )
      {
	return *(Number.DComplex) / Right.Number.DoubleNumber;
      }
    if ( Right.NumType == Double && NumType == HighComplex )
      {
	return *(Number.HAComplex) / ComplexHA(Right.Number.DoubleNumber);
      }
    if ( Right.NumType == HighReal && NumType == DoubleComplex )
      {
	return ComplexHA( *(Number.DComplex) ) / *(Right.Number.HReal);
      }
    if ( Right.NumType == HighReal && NumType == HighComplex )
      {
	return *(Number.HAComplex) / ComplexHA(*(Right.Number.HReal));
      }
    if ( Right.NumType == DoubleComplex && NumType == HighComplex )
      {
	return *(Number.HAComplex) / ComplexHA(*(Right.Number.DComplex));
      }
    return NaN;
  };
  
  bool NumberType::IsReal() const
  {
    switch( NumType )
      {
      case None:
      case Int:
      case Double:
      case HighReal:
	return true;
      case DoubleComplex:
	if ( imag( * Number.DComplex ) == 0 ) return true;
	return false;
      case HighComplex:
	if ( Number.HAComplex -> GetIm().IsZero() ) return true;
	return false;
      };
    return false;
  };

  bool NumberType::IsImag() const
  {
    switch( NumType )
      {
      case None:
      case Int:
      case Double:
      case HighReal:
	return false;
      case DoubleComplex:
	if ( real( * Number.DComplex ) == 0 ) return true;
	return false;
      case HighComplex:
	if ( Number.HAComplex -> GetRe().IsZero() ) return true;
	return false;
      };
    return false;
  };

  bool NumberType::IsComplex() const
  {
    switch( NumType )
      {
      case None:
      case Int:
      case Double:
      case HighReal:
	return false;
      case DoubleComplex:
	if ( imag( * Number.DComplex ) == 0 
	     or real( * Number.DComplex ) == 0 ) return false;
	return true;
      case HighComplex:
	if ( Number.HAComplex -> GetIm().IsZero() 
	     or Number.HAComplex -> GetRe().IsZero() ) return false;
	return true;
      };
    return false;
  };

  string NumberType::ToString() const
  {
    switch ( NumType )
      {
      case None:
	return "";
      case Int:
	{
	  ostringstream Stm;
	  Stm << Number.IntNumber;
	  return Stm.str();
	}
      case Double: 
	{
	  ostringstream Stm;
	  Stm << Number.DoubleNumber;
	  return Stm.str();
	}
      case DoubleComplex:
	{
	  ComplexHA tmp = *Number.DComplex;
	  return tmp.ToString();
	}
      case HighReal:
	return Number.HReal -> ToString();
      case HighComplex:
	return Number.HAComplex -> ToString();
      };
    return "";
  };  

  bool NumberType::operator < ( const NumberType & value ) const
  {
    if ( this -> IsNaN() or value.IsNaN() ) return false;
    NumberType tmp1 = this -> RePart() , tmp2 = value.RePart();
    switch ( tmp1.NumType )
      {
      case Int:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    return tmp1.Number.IntNumber < tmp2.Number.IntNumber;
	  case Double:
	    return tmp1.Number.IntNumber < tmp2.Number.DoubleNumber;
	  case HighReal:
	    return HighAccuracyNumber( tmp1.Number.IntNumber ) < *tmp2.Number.HReal;
	  default:
	    throw 0;
	  };
      case Double:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    return tmp1.Number.DoubleNumber < tmp2.Number.IntNumber;
	  case Double:
	    return tmp1.Number.DoubleNumber < tmp2.Number.DoubleNumber;
	  case HighReal:
	    return HighAccuracyNumber( tmp1.Number.DoubleNumber ) < *tmp2.Number.HReal;
	  default:
	    throw 0;
	  };
      case HighReal:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    return *tmp1.Number.HReal < tmp2.Number.IntNumber;
	  case Double:
	    return *tmp1.Number.HReal < tmp2.Number.DoubleNumber;
	  case HighReal:
	    return *tmp1.Number.HReal < *tmp2.Number.HReal;
	  default:
	    throw 0;
	  };
      default:
	throw 0;
      };
    return false;
  };

  bool NumberType::operator > ( const NumberType & value ) const
  {
    if ( this -> IsNaN() or value.IsNaN() ) return false;
    NumberType tmp1 = this -> RePart() , tmp2 = value.RePart();
    switch ( tmp1.NumType )
      {
      case Int:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    return tmp1.Number.IntNumber > tmp2.Number.IntNumber;
	  case Double:
	    return tmp1.Number.IntNumber > tmp2.Number.DoubleNumber;
	  case HighReal:
	    return HighAccuracyNumber( tmp1.Number.IntNumber ) > *tmp2.Number.HReal;
	  default:
	    throw 0;
	  };
      case Double:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    return tmp1.Number.DoubleNumber > tmp2.Number.IntNumber;
	  case Double:
	    return tmp1.Number.DoubleNumber > tmp2.Number.DoubleNumber;
	  case HighReal:
	    return HighAccuracyNumber( tmp1.Number.DoubleNumber ) > *tmp2.Number.HReal;
	  default:
	    throw 0;
	  };
      case HighReal:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    return *tmp1.Number.HReal > tmp2.Number.IntNumber;
	  case Double:
	    return *tmp1.Number.HReal > tmp2.Number.DoubleNumber;
	  case HighReal:
	    return *tmp1.Number.HReal > *tmp2.Number.HReal;
	  default:
	    throw 0;
	  };
      default:
	throw 0;
      };
    return false;
  };

  bool NumberType::operator <= ( const NumberType & value ) const
  {
    if ( this -> IsNaN() or value.IsNaN() ) return false;
    NumberType tmp1 = this -> RePart() , tmp2 = value.RePart();
    switch ( tmp1.NumType )
      {
      case Int:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    return tmp1.Number.IntNumber <= tmp2.Number.IntNumber;
	  case Double:
	    return tmp1.Number.IntNumber <= tmp2.Number.DoubleNumber;
	  case HighReal:
	    return HighAccuracyNumber( tmp1.Number.IntNumber ) <= *tmp2.Number.HReal;
	  default:
	    throw 0;
	  };
      case Double:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    return tmp1.Number.DoubleNumber <= tmp2.Number.IntNumber;
	  case Double:
	    return tmp1.Number.DoubleNumber <= tmp2.Number.DoubleNumber;
	  case HighReal:
	    return HighAccuracyNumber( tmp1.Number.DoubleNumber ) <= *tmp2.Number.HReal;
	  default:
	    throw 0;
	  };
      case HighReal:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    return *tmp1.Number.HReal <= tmp2.Number.IntNumber;
	  case Double:
	    return *tmp1.Number.HReal <= tmp2.Number.DoubleNumber;
	  case HighReal:
	    return *tmp1.Number.HReal <= *tmp2.Number.HReal;
	  default:
	    throw 0;
	  };
      default:
	throw 0;
      };
    return false;
  };

  bool NumberType::operator >= ( const NumberType & value ) const
  {
    if ( this -> IsNaN() or value.IsNaN() ) return false;
    NumberType tmp1 = this -> RePart() , tmp2 = value.RePart();
    switch ( tmp1.NumType )
      {
      case Int:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    return tmp1.Number.IntNumber >= tmp2.Number.IntNumber;
	  case Double:
	    return tmp1.Number.IntNumber >= tmp2.Number.DoubleNumber;
	  case HighReal:
	    return HighAccuracyNumber( tmp1.Number.IntNumber ) >= *tmp2.Number.HReal;
	  default:
	    throw 0;
	  };
      case Double:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    return tmp1.Number.DoubleNumber >= tmp2.Number.IntNumber;
	  case Double:
	    return tmp1.Number.DoubleNumber >= tmp2.Number.DoubleNumber;
	  case HighReal:
	    return HighAccuracyNumber( tmp1.Number.DoubleNumber ) >= *tmp2.Number.HReal;
	  default:
	    throw 0;
	  };
      case HighReal:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    return *tmp1.Number.HReal >= tmp2.Number.IntNumber;
	  case Double:
	    return *tmp1.Number.HReal >= tmp2.Number.DoubleNumber;
	  case HighReal:
	    return *tmp1.Number.HReal >= *tmp2.Number.HReal;
	  default:
	    throw 0;
	  };
      default:
	throw 0;
      };
    return false;
  };

  bool NumberType::operator == ( const NumberType & value ) const
  {
    if ( this -> IsNaN() or value.IsNaN() ) return false;
    NumberType tmp1 = this -> RePart() , tmp2 = value.RePart();
    switch ( tmp1.NumType )
      {
      case Int:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    if ( tmp1.Number.IntNumber != tmp2.Number.IntNumber )
	      return false;
	    break;
	  case Double:
	    if ( tmp1.Number.IntNumber != tmp2.Number.DoubleNumber )
	      return false;
	    break;
	  case HighReal:
	    if ( !( HighAccuracyNumber( tmp1.Number.IntNumber ) 
		    == *tmp2.Number.HReal ))
	      return false;
	    break;
	  default:
	    throw 0;
	  };
	break;
      case Double:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    if ( tmp1.Number.DoubleNumber != tmp2.Number.IntNumber )
	      return false;
	    break;
	  case Double:
	    if ( tmp1.Number.DoubleNumber != tmp2.Number.DoubleNumber )
	      return false;
	    break;
	  case HighReal:
	    if ( !( HighAccuracyNumber( tmp1.Number.DoubleNumber )
		    == *tmp2.Number.HReal ))
	      return false;
	    break;
	  default:
	    throw 0;
	  };
	break;
      case HighReal:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    if ( !( *tmp1.Number.HReal == tmp2.Number.IntNumber ))
	      return false;
	    break;
	  case Double:
	    if ( !( *tmp1.Number.HReal == tmp2.Number.DoubleNumber ))
	      return false;
	    break;
	  case HighReal:
	    if ( !( *tmp1.Number.HReal == *tmp2.Number.HReal ))
	      return false;
	    break;
	  default:
	    throw 0;
	  };
	break;
      default:
	throw 0;
      };
    tmp1 = this -> ImPart();
    tmp2 = value.ImPart();
    switch ( tmp1.NumType )
      {
      case Int:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    if ( tmp1.Number.IntNumber != tmp2.Number.IntNumber )
	      return false;
	    break;
	  case Double:
	    if ( tmp1.Number.IntNumber != tmp2.Number.DoubleNumber )
	      return false;
	    break;
	  case HighReal:
	    if ( !( HighAccuracyNumber( tmp1.Number.IntNumber ) 
		    == *tmp2.Number.HReal ))
	      return false;
	    break;
	  default:
	    throw 0;
	  };
	break;
      case Double:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    if ( tmp1.Number.DoubleNumber != tmp2.Number.IntNumber )
	      return false;
	    break;
	  case Double:
	    if ( tmp1.Number.DoubleNumber != tmp2.Number.DoubleNumber )
	      return false;
	    break;
	  case HighReal:
	    if ( !( HighAccuracyNumber( tmp1.Number.DoubleNumber )
		    == *tmp2.Number.HReal ))
	      return false;
	    break;
	  default:
	    throw 0;
	  };
	break;
      case HighReal:
	switch ( tmp2.NumType )
	  {
	  case Int:
	    if ( !( *tmp1.Number.HReal == tmp2.Number.IntNumber ))
	      return false;
	    break;
	  case Double:
	    if ( !( *tmp1.Number.HReal == tmp2.Number.DoubleNumber ))
	      return false;
	    break;
	  case HighReal:
	    if ( !( *tmp1.Number.HReal == *tmp2.Number.HReal ))
	      return false;
	    break;
	  default:
	    throw 0;
	  };
	break;
      default:
	throw 0;
      };
    return true;
  };

  NumberType NumberType::operator - () const
  {
    switch( NumType )
      {
      case None:
	return *this;
      case Int:
	return -(Number.IntNumber);
      case Double:
	return -(Number.DoubleNumber);
      case HighReal:
	return -(*(Number.HReal));
      case DoubleComplex:
	return -(*(Number.DComplex));
      case HighComplex:
	return -(*(Number.HAComplex));
      };
    return *this;
  };

  bool NumberType::IsNaN() const
  {
    switch ( NumType )
      {
      case None:
	return true;
      case Int:
	return false;
      case Double:
	return Number.DoubleNumber != Number.DoubleNumber;
      case HighReal:
	return Number.HReal -> IsNaN();
      case DoubleComplex:
	return *Number.DComplex != *Number.DComplex;
      case HighComplex:
	return Number.HAComplex -> IsNaN();
      };
    return true;
  };

  bool NumberType::IsInfinity() const
  {
    switch ( NumType )
      {
      case None:
	return false;
      case Int:
	return false;
      case Double:
	return isinfinity( Number.DoubleNumber );
      case HighReal:
	return Number.HReal -> IsInfinity();
      case DoubleComplex:
	return isinfinity( real( *Number.DComplex )) 
	  or isinfinity( imag( *Number.DComplex ));
      case HighComplex:
	return Number.HAComplex -> IsInfinity();
      };
    return false;
  };

  bool NumberType::IsZero() const
  {
    switch ( NumType )
      {
      case None:
	return false;
      case Int:
	return Number.IntNumber == 0;
      case Double:
	return Number.DoubleNumber == 0;
      case HighReal:
	return Number.HReal -> IsZero();
      case DoubleComplex:
	return real( *Number.DComplex ) == 0 and imag( *Number.DComplex ) == 0;
      case HighComplex:
	return Number.HAComplex -> IsZero();
      };
    return false;
  };

  int NumberType::GetSign() const
  {
    switch ( NumType )
      {
      case None:
	return 0;
      case Int:
	if ( Number.IntNumber == 0 ) return 0;
	return Number.IntNumber > 0;
      case Double:
	if ( Number.DoubleNumber == 0 ) return 0;
	return Number.DoubleNumber > 0;
      case HighReal:
	return Number.HReal -> GetSign();
      case DoubleComplex:
	if ( real( *Number.DComplex ) == 0 ) return 0;
	return real( *Number.DComplex ) > 0;
      case HighComplex:
	return Number.HAComplex -> GetSign();
      };
    return 0;
  };

  bool NumberType::IsNull() const
  {
    switch ( NumType )
      {
      case None:
	return true;
      case Int:
      case Double:
      case DoubleComplex:
	return false;
      case HighReal:
	return Number.HReal -> IsNull();
      case HighComplex:
	return Number.HAComplex -> IsNull();
      };
    return true;
  };

  NumberType NumberType::RePart() const
  {
    switch ( NumType )
      {
      case None:
      case Int:
      case Double:
      case HighReal:
	return *this;
      case DoubleComplex:
	return real( *Number.DComplex );
      case HighComplex:
	return Number.HAComplex -> GetRe();
      };
    return true;
  };

  NumberType NumberType::ImPart() const
  {
    switch ( NumType )
      {
      case None:
      case Int:
      case Double:
      case HighReal:
	return 0;
      case DoubleComplex:
	return imag( *Number.DComplex );
      case HighComplex:
	return Number.HAComplex -> GetIm();
      };
    return true;
  };

  NumberType NumberType::Divide( const NumberType & left , const NumberType & right , int length )
  {
    if ( left.NumType == None or right.NumType == None )
      return NumberType();
    if ( left.NumType == Int && right.NumType == Int )
      {
	if ( length <= 2 )
	  return double( left.Number.IntNumber ) / right.Number.IntNumber;
	return HighAccuracyNumber::Divide( left.Number.IntNumber , right.Number.IntNumber , length );
      }
    if ( left.NumType == Int && right.NumType == Double )
      {
	if ( length <= 2 )
	  return double( left.Number.IntNumber ) / right.Number.DoubleNumber;
	return HighAccuracyNumber::Divide( left.Number.IntNumber , right.Number.DoubleNumber , length );
      }
    if ( left.NumType == Double && right.NumType == Int )
      {
	if ( length <= 2 )
	  return left.Number.DoubleNumber / right.Number.IntNumber;
	return HighAccuracyNumber::Divide( left.Number.DoubleNumber , right.Number.IntNumber , length );
      }
    if ( left.NumType == Double && right.NumType == Double )
      {
	if ( length <= 2 )
	  return left.Number.DoubleNumber / right.Number.DoubleNumber;
	return HighAccuracyNumber::Divide( left.Number.DoubleNumber , right.Number.DoubleNumber , length );
      }
    if ( left.NumType == Int && right.NumType == HighReal )
      {
	return HighAccuracyNumber::Divide( left.Number.IntNumber , *(right.Number.HReal) , length );
      }
    if ( left.NumType == Int && right.NumType == DoubleComplex )
      {
	if ( length <= 2 )
	  return complex<double>( left.Number.IntNumber ) / *right.Number.DComplex;
	return ComplexHA::Divide( left.Number.IntNumber , *right.Number.DComplex , length );
      }
    if ( left.NumType == Int && right.NumType == HighComplex )
      {
	return ComplexHA::Divide( left.Number.IntNumber , *(right.Number.HAComplex) , length );
      }
    if ( left.NumType == Double && right.NumType == HighReal )
      {
	return HighAccuracyNumber::Divide( left.Number.DoubleNumber , *(right.Number.HReal) , length );
      }
    if ( left.NumType == Double && right.NumType == DoubleComplex )
      {
	if ( length <= 2 )
	  return left.Number.DoubleNumber / *(right.Number.DComplex);
	return ComplexHA::Divide( left.Number.DoubleNumber , *(right.Number.DComplex) , length );
      }
    if ( left.NumType == Double && right.NumType == HighComplex )
      {
	return ComplexHA::Divide( left.Number.DoubleNumber , *(right.Number.HAComplex) , length );
      }
    if ( left.NumType == HighReal && right.NumType == DoubleComplex )
      {
	return ComplexHA::Divide( *(left.Number.HReal) , *(right.Number.DComplex) , length );
      }
    if ( left.NumType == HighReal && right.NumType == HighComplex )
      {
	return ComplexHA::Divide( *(left.Number.HReal) , *(right.Number.HAComplex) , length );
      }
    if ( left.NumType == DoubleComplex && right.NumType == HighComplex )
      {
	return ComplexHA::Divide( *(left.Number.DComplex) , *(right.Number.HAComplex) , length );
      }
    if ( left.NumType == DoubleComplex && right.NumType == DoubleComplex )
      {
	if ( length <= 2 )
	  return *(left.Number.DComplex) / *(right.Number.DComplex);
	return ComplexHA::Divide( *(left.Number.DComplex) , *(right.Number.DComplex) , length );
      }
    if ( left.NumType == HighReal && right.NumType == HighReal )
      {
	return HighAccuracyNumber::Divide( *(left.Number.HReal) , *(right.Number.HReal) , length );
      }
    if ( left.NumType == HighComplex && right.NumType == HighComplex )
      {
	return ComplexHA::Divide( *(left.Number.HAComplex) , *(right.Number.HAComplex) , length );
      }
    if ( right.NumType == Int && left.NumType == HighReal )
      {
	return HighAccuracyNumber::Divide( *(left.Number.HReal) , right.Number.IntNumber , length );
      }
    if ( right.NumType == Int && left.NumType == DoubleComplex )
      {
	if ( length <= 2 )
	  return *left.Number.DComplex / (complex<double>)right.Number.IntNumber;
	return ComplexHA::Divide( *left.Number.DComplex , right.Number.IntNumber , length );
      }
    if ( right.NumType == Int && left.NumType == HighComplex )
      {
	return ComplexHA::Divide( *(left.Number.HAComplex) , ComplexHA(right.Number.IntNumber) , length );
      }
    if ( right.NumType == Double && left.NumType == HighReal )
      {
	return HighAccuracyNumber::Divide( *(left.Number.HReal) , right.Number.DoubleNumber , length );
      }
    if ( right.NumType == Double && left.NumType == DoubleComplex )
      {
	if ( length <= 2 )
	  return *(left.Number.DComplex) / right.Number.DoubleNumber;
	return ComplexHA::Divide( *(left.Number.DComplex) , right.Number.DoubleNumber , length );
      }
    if ( right.NumType == Double && left.NumType == HighComplex )
      {
	return ComplexHA::Divide( *(left.Number.HAComplex) , right.Number.DoubleNumber , length );
      }
    if ( right.NumType == HighReal && left.NumType == DoubleComplex )
      {
	return ComplexHA::Divide( *(left.Number.DComplex) , *(right.Number.HReal) , length );
      }
    if ( right.NumType == HighReal && left.NumType == HighComplex )
      {
	return ComplexHA::Divide( *(left.Number.HAComplex) , *(right.Number.HReal) , length );
      }
    if ( right.NumType == DoubleComplex && left.NumType == HighComplex )
      {
	return ComplexHA::Divide( *(left.Number.HAComplex) , *(right.Number.DComplex) , length );
      }
    return NaN;
  };

  NumberType NumberType::Square( const NumberType & value )
  {
    switch ( value.NumType )
      {
      case None:
	return value;
      case Int:
	{
	  Int64 tmp = value.Number.IntNumber , result = tmp * tmp;
	  if (( result >> 31 ) == 0 )
	    return value.Number.IntNumber * value.Number.IntNumber;
	  return HighAccuracyNumber::Square( value.Number.IntNumber );
	}
      case Double:
	return value.Number.DoubleNumber * value.Number.DoubleNumber;
      case DoubleComplex:
	return *value.Number.DComplex * *value.Number.DComplex;
      case HighReal:
	return HighAccuracyNumber::Square( *value.Number.HReal );
      case HighComplex:
	return ComplexHA::Square( *value.Number.HAComplex );
      };
    return NaN;
  };

  NumberType NumberType::Inverse( int length ) const
  {
    switch ( NumType )
      {
      case None:
	return *this;
      case Int:
	{
	  if ( length <= 2 )
	    return 1 / (double)Number.IntNumber;
	  return HighAccuracyNumber( Number.IntNumber ).Inverse( length );
	}
      case Double:
	if ( length <= 2 )
	  return 1 / Number.DoubleNumber;
	return HighAccuracyNumber( Number.DoubleNumber ).Inverse( length );
      case DoubleComplex:
	if ( length <= 2 )
	  return complex<double>(1) / *Number.DComplex;
	return ComplexHA( *Number.DComplex ).Inverse( length );
      case HighReal:
	return Number.HReal -> Inverse( length );
      case HighComplex:
	return Number.HAComplex -> Inverse( length );
      };
    return NaN;
  };

  int NumberType::GetLength() const
  {
    switch ( NumType )
      {
      case None:
	return 0;
      case Int:
	return 1;
      case Double:
	if ( isinfinity( Number.DoubleNumber ) or Number.DoubleNumber != Number.DoubleNumber )
	  return 0;
	return 2;
      case DoubleComplex:
	if ( isinfinity( real( *Number.DComplex )) or isinfinity( imag( *Number.DComplex )) or *Number.DComplex != *Number.DComplex )
	  return 0;
	return 2;
      case HighReal:
	return Number.HReal -> GetLength();
      case HighComplex:
	return Number.HAComplex -> GetLength();
      };
    return 0;
  };

  NumberType NumberType::GetRe() const
  {
    switch ( NumType )
      {
      case None:
      case Int:
      case Double:
      case HighReal:
	return *this;
      case DoubleComplex:
	return real( *Number.DComplex );
      case HighComplex:
	return Number.HAComplex -> GetRe();
      };
    return NaN;
  };

  NumberType NumberType::GetIm() const
  {
    switch ( NumType )
      {
      case None:
	return *this;
      case Int:
      case Double:
      case HighReal:
	return 0;
      case DoubleComplex:
	return imag( *Number.DComplex );
      case HighComplex:
	return Number.HAComplex -> GetIm();
      };
    return NaN;
  };

  NumberType NumberType::ComplexConjugate( const NumberType & value )
  {
    switch ( value.NumType )
      {
      case None:
      case Int:
      case Double:
      case HighReal:
	return value;
      case DoubleComplex:
	return conj( *value.Number.DComplex );
      case HighComplex:
	return ComplexHA::ComplexConjugate( *value.Number.HAComplex );
      };
    return NaN;
  };

  NumberType NumberType::Abs( const NumberType & value , int length )
  {
    switch ( value.NumType )
      {
      case None:
      case Int:
      case Double:
      case HighReal:
	return value;
      case DoubleComplex:
	if ( length <= 2 )
	  return abs( *value.Number.DComplex );
	return ComplexHA::Abs( *value.Number.DComplex , length );
      case HighComplex:
	return ComplexHA::Abs( *value.Number.HAComplex , length );
      };
    return NaN;
  };

  NumberType NumberType::MSquare( const NumberType & value )
  {
    switch ( value.NumType )
      {
      case None:
	return value;
      case Int:
      case Double:
      case HighReal:
	return Square( value );
      case DoubleComplex:
	return real( *value.Number.DComplex ) * real( *value.Number.DComplex ) + imag( *value.Number.DComplex ) * imag( *value.Number.DComplex );
      case HighComplex:
	return ComplexHA::MSquare( *value.Number.HAComplex );
      };
    return NaN;
  };

  void NumberType::LimitLength( int length )
  {
    switch ( NumType )
      {
      case HighReal:
	Number.HReal -> CutTo( length );
	return;
      case HighComplex:
	Number.HAComplex -> LimitLength( length );
	return;
      default:
	return;
      }
  };

  NumberType NumberType::InverseSqrt( const NumberType & value , int length )
  {
    switch ( value.NumType )
      {
      case None:
	return NaN;
      case Int:
	if ( length <= 2 )
	  {
	    if ( value.Number.IntNumber == 0 ) return RealInfinity;
	    if ( value.Number.IntNumber > 0 )
	      return 1 / sqrt( value.Number.IntNumber );
	    return 1.0 / sqrt( complex<double>( value.Number.IntNumber ) );
	  }
	if ( value.Number.IntNumber == 0 ) return RealInfinity;
	if ( value.Number.IntNumber > 0 )
	  return HighAccuracyNumber::InverseSqrt( value.Number.IntNumber , length );
	return ComplexHA::InverseSqrt( HighAccuracyNumber( value.Number.IntNumber ) , length );
      case Double:
	if ( length <= 2 )
	  {
	    if ( value.Number.DoubleNumber == 0 ) return RealInfinity;
	    if ( value.Number.DoubleNumber > 0 )
	      return 1 / sqrt( value.Number.DoubleNumber );
	    return 1.0 / sqrt( complex<double>( value.Number.DoubleNumber ) );
	  }
	if ( value.Number.DoubleNumber == 0 ) return RealInfinity;
	if ( value.Number.DoubleNumber > 0 )
	  return HighAccuracyNumber::InverseSqrt( value.Number.DoubleNumber , length );
	return ComplexHA::InverseSqrt( HighAccuracyNumber( value.Number.DoubleNumber ) , length );
      case DoubleComplex:
	if ( length <= 2 )
	  {
	    return 1.0 / sqrt( *value.Number.DComplex );
	  }
	return ComplexHA::InverseSqrt( *value.Number.DComplex , length );
      case HighReal:
	if ( value.Number.HReal -> IsZero() ) return RealInfinity;
	if ( *value.Number.HReal > 0 ) return HighAccuracyNumber::InverseSqrt( *value.Number.HReal , length );
	return ComplexHA::InverseSqrt( *value.Number.HReal , length );
      case HighComplex:
	return ComplexHA::InverseSqrt( *value.Number.HAComplex , length );
      };
    return NaN;
  };
  
  NumberType NumberType::Sqrt( const NumberType & value , int length )
  {
    switch ( value.NumType )
      {
      case None:
	return NaN;
      case Int:
	if ( length <= 2 )
	  {
	    if ( value.Number.IntNumber == 0 ) return Zero;
	    if ( value.Number.IntNumber > 0 )
	      return sqrt( value.Number.IntNumber );
	    return sqrt( complex<double>( value.Number.IntNumber ) );
	  }
	if ( value.Number.IntNumber == 0 ) return Zero;
	if ( value.Number.IntNumber > 0 )
	  return HighAccuracyNumber::Sqrt( value.Number.IntNumber , length );
	return ComplexHA::Sqrt( HighAccuracyNumber( value.Number.IntNumber ) , length );
      case Double:
	if ( length <= 2 )
	  {
	    if ( value.Number.DoubleNumber == 0 ) return Zero;
	    if ( value.Number.DoubleNumber > 0 )
	      return sqrt( value.Number.DoubleNumber );
	    return sqrt( complex<double>( value.Number.DoubleNumber ) );
	  }
	if ( value.Number.DoubleNumber == 0 ) return Zero;
	if ( value.Number.DoubleNumber > 0 )
	  return HighAccuracyNumber::Sqrt( value.Number.DoubleNumber , length );
	return ComplexHA::Sqrt( HighAccuracyNumber( value.Number.DoubleNumber ) , length );
      case DoubleComplex:
	if ( length <= 2 )
	  {
	    return sqrt( *value.Number.DComplex );
	  }
	return ComplexHA::Sqrt( *value.Number.DComplex , length );
      case HighReal:
	if ( value.Number.HReal -> IsZero() ) return Zero;
	if ( *value.Number.HReal > 0 ) return HighAccuracyNumber::Sqrt( *value.Number.HReal , length );
	return ComplexHA::Sqrt( *value.Number.HReal , length );
      case HighComplex:
	return ComplexHA::Sqrt( *value.Number.HAComplex , length );
      };
    return NaN;
  };
  
  bool NumberType::check( string & str )
  {
    UInt32 i = 0;
    for ( ; str[i] == ' ' ; )
      {
  	if ( ++i >= str.length() ) return false;
      }
    string result = "";
    UInt32 k = i;
    int j;
    for ( j = 0 ; i < str.length() ; i++ )
      {
	switch ( str[i] )
	  {
	  case '.':
	    if ( ++j >= 2 ) return false;
	    if ( str.length() <= i + 1 or str[ i + 1 ] == ' ' ) return false;
	    result += '.';
	    break;
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
	    result += str[i];
	  continue;
	  case '-':
	    if( i != k or str.length() == k + 1 or str[ k + 1 ] == ' ' ) return false;
	    result += '-';
	    continue;
	  case ' ':
	    for ( i++ ; i < str.length() ; i++ )
	      {
	  	if ( str[i] != ' ' ) return false;
	      }
	    str = result;
	    return true;
	  default:
	    return false;
	  };
      }
    str = result;
    return true;
  };
  
  bool NumberType::ReadReal( string str , NumberType & result1 )
  {
    if ( !check(str) ) return false;
    if ( str.length() > 11 )
      {
	result1 = HighAccuracyNumber::ReadReal( str );
	return true;
      }
    else
      {
  	istringstream istm;
  	istm.str(str);
	if ( str.length() <= 9 )
	  {
	    for ( UInt32 i = 0 ; i < str.length() ; i++ )
	      if ( str[i] == '.' ) goto Double;
	    int r;
	    istm >> r;
	    result1 = r;
	    return true;
	  }
      Double:
  	double r;
  	istm >> r;
  	result1 = r;
      }
    return true;
  };
}
