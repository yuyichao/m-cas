// -*- C++ -*-
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

#ifndef _NUMBERTYPE_H
#define _NUMBERTYPE_H

namespace CAS
{  
  struct DataArray
  {
  public:
    static inline DataArray * make( int length = 1 ){ return new DataArray( length ); };
    inline void attach(){ RefCount++; };
    inline void detach(){ if ( --RefCount == 0 ) delete this; };
    inline DataArray * unalias()
    { 
      if ( RefCount == 1 ) return this;
      --RefCount;
      return new DataArray( *this );
    };
    inline UInt32 & operator []( int n ){ return Data[n]; };
    inline UInt32 * GetPoint() const{ return Data; };
    inline int GetLength() const{ return Length; };
    ~DataArray();
  private:
    int Length;
    UInt32 * Data;
    int RefCount;
    DataArray(int length);
    DataArray(const DataArray& value);
    DataArray & operator = (const DataArray & value);
  };//Class of data in a high accuracy number

  class HighAccuracyNumber
  {
  private :
    int Sign;
    DataArray * FullData;
    UInt32 * Data;
    int Length;
    int Point;
    static HighAccuracyNumber RecursiveDivide( const HighAccuracyNumber & left , const HighAccuracyNumber & right , int length );
    static HighAccuracyNumber NormalDivide( const HighAccuracyNumber & left , const HighAccuracyNumber & right , int length );
    void unalias();
    HighAccuracyNumber( UInt32 value );
    static HighAccuracyNumber GetInfinity();
    static HighAccuracyNumber GetZero();
    HighAccuracyNumber( int sign , int length , int point , UInt32 * data , DataArray * fulldata );
    inline UInt32 * GetData() const{ return Data; };
 public :
    static HighAccuracyNumber ReadReal( string str );
    static const HighAccuracyNumber Infinity;
    static const HighAccuracyNumber NaN;
    static const HighAccuracyNumber Zero;
    static const UInt64 A;
    static const HighAccuracyNumber One;
    static const HighAccuracyNumber OneOverTwo;
    static const HighAccuracyNumber ThreeOverFour;
    inline int GetSign() const{ if ( IsZero() ) return 0; return Sign; };
    inline int GetLength() const{ return Length; };
    inline int GetPoint() const{ return Point; };
    ~HighAccuracyNumber();
    HighAccuracyNumber();
    HighAccuracyNumber( int value );
    HighAccuracyNumber( double value );
    HighAccuracyNumber( const HighAccuracyNumber & value );
    HighAccuracyNumber( int sign , int length , int point );
    HighAccuracyNumber & operator = ( const HighAccuracyNumber & value );
    bool operator < ( const HighAccuracyNumber & value ) const;
    bool operator > ( const HighAccuracyNumber & value ) const;
    bool operator <= ( const HighAccuracyNumber & value ) const;
    bool operator >= ( const HighAccuracyNumber & value ) const;
    bool operator == ( const HighAccuracyNumber & value ) const;
    HighAccuracyNumber operator << ( int n ) const;
    inline bool IsNull() const{ if( Data == NULL or FullData == NULL or FullData -> GetPoint() == NULL ) return true; return false; };
    inline bool IsZero() const{ if(( Sign == 0 and Length != 0 ) or Data[ Length - 1 ] == 0 ) return true; return false; };
    inline bool IsSpecial() const{ return Length == 0; };
    inline bool IsInfinity() const{ return Length == 0 and Sign != 0; };
    inline bool IsNaN() const{ return Length == 0 and Sign == 0; };
    UInt32 operator [] ( int n ) const;
    void CutTo( int length );
    void AddLengthTo( int length );
    void UnCut();
    inline HighAccuracyNumber operator - () const{ return HighAccuracyNumber( -Sign , Length , Point , Data , FullData ); };
    friend HighAccuracyNumber operator + ( const HighAccuracyNumber & left , const HighAccuracyNumber & right );
    friend HighAccuracyNumber operator * ( const HighAccuracyNumber & left , const HighAccuracyNumber & right );
    friend HighAccuracyNumber operator - ( const HighAccuracyNumber & left , const HighAccuracyNumber & right );
    friend HighAccuracyNumber operator / ( const HighAccuracyNumber & left , const HighAccuracyNumber & right );
    static HighAccuracyNumber Divide( const HighAccuracyNumber & left , const HighAccuracyNumber & right , int length = 0 );
    static HighAccuracyNumber Square( const HighAccuracyNumber & value );
    HighAccuracyNumber Inverse( int length = 0 ) const;
    string ToString( int length = 0 ) const;
    static HighAccuracyNumber InverseSqrt( const HighAccuracyNumber & value , int length = 0 );
    static HighAccuracyNumber Sqrt( const HighAccuracyNumber & value , int length = 0 );
  };
  HighAccuracyNumber operator + ( const HighAccuracyNumber & left , const HighAccuracyNumber & right );
  HighAccuracyNumber operator * ( const HighAccuracyNumber & left , const HighAccuracyNumber & right );
  HighAccuracyNumber operator - ( const HighAccuracyNumber & left , const HighAccuracyNumber & right );
  HighAccuracyNumber operator / ( const HighAccuracyNumber & left , const HighAccuracyNumber & right );


  class ComplexHA
  {
  private:
    HighAccuracyNumber RePart;
    HighAccuracyNumber ImPart;
  public:
    static const ComplexHA I;
    static const ComplexHA Zero;
    static const ComplexHA RealInfinity;
    static const ComplexHA ImagInfinity;
    static const ComplexHA NaN;
    static const ComplexHA One;
    static const ComplexHA Two;
    static const ComplexHA OneOverTwo;
    ComplexHA();
    ComplexHA( double value );
    ComplexHA( complex<double> value );
    ComplexHA( int repart );
    ComplexHA( int repart , int impart );
    ComplexHA( const HighAccuracyNumber & repart );
    ComplexHA( const HighAccuracyNumber & repart , const HighAccuracyNumber & impart );
    ComplexHA( const ComplexHA & value );
    inline ComplexHA & operator = ( const ComplexHA & value )
      {
	RePart = value.RePart;
	ImPart = value.ImPart;
	return *this;
      };
    inline bool IsNull() const
    { return ( RePart.IsNull() or ImPart.IsNull() ); };
    inline bool IsNaN() const
    { return ( RePart.IsNaN() or ImPart.IsNaN() or this -> IsNull() ); };
    inline bool IsZero() const
    { return ( RePart.IsZero() and ImPart.IsZero() ); };
    inline bool IsInfinity() const
    { return ( RePart.IsInfinity() or ImPart.IsInfinity() ); };
    inline bool IsSpecial() const
    { return ( RePart.IsSpecial() or ImPart.IsSpecial() ); };
    inline int GetLength() const
    { return (((( RePart.GetLength() + RePart.GetPoint() ) 
		> ( ImPart.GetPoint() + ImPart.GetLength() )) 
	       ? ( RePart.GetLength() + RePart.GetPoint() )
	       : ( ImPart.GetPoint() + ImPart.GetLength() ))
	      - (( RePart.GetPoint() < ImPart.GetPoint() )
		 ? RePart.GetPoint() : ImPart.GetPoint() )); };
    inline HighAccuracyNumber GetRe() const{ return RePart; };
    inline HighAccuracyNumber GetIm() const{ return ImPart; };
    inline int GetSign() const{ return RePart.GetSign(); };
    void LimitLength( int length );
    inline HighAccuracyNumber SetRe( HighAccuracyNumber value )
    { return RePart = value; };
    inline HighAccuracyNumber SetIm( HighAccuracyNumber value )
    { return ImPart = value; };
    inline bool operator == ( const ComplexHA & value ) const
    { return value.RePart == RePart and value.ImPart == ImPart; };
    inline ComplexHA operator - () const
    { return ComplexHA( -RePart , -ImPart ); };
    static inline ComplexHA ComplexConjugate( const ComplexHA & value )
    { return ComplexHA( value.RePart , -( value.ImPart )); };
    string ToString( int length = 0 ) const;
    static ComplexHA Divide( const ComplexHA & left , const ComplexHA & right
			     , int length = 0 );
    ComplexHA operator + ( const ComplexHA & value ) const;
    ComplexHA operator - ( const ComplexHA & value ) const;
    ComplexHA operator * ( const ComplexHA & value ) const;
    ComplexHA operator / ( const ComplexHA & value ) const;
    static HighAccuracyNumber MSquare( const ComplexHA & value );
    static HighAccuracyNumber Abs( const ComplexHA & value , int length = 0 );
    static ComplexHA Square( const ComplexHA & value );
    ComplexHA Inverse( int length = 0 ) const;
    static ComplexHA InverseSqrt( const ComplexHA & value , int length = 0 );
    static ComplexHA Sqrt( const ComplexHA & value , int length = 0 );
  };

  inline bool isinfinity( double value )
  {
    return value > DBL_MAX or value < -DBL_MAX;
  };

  class NumberType
  {
  public :
    friend class PrimaryFunction;
    static const NumberType One;
    static const NumberType RealInfinity;
    static const NumberType ImagInfinity;
    static const NumberType Zero;
    static const NumberType NaN;
    static const NumberType I;
    typedef enum
    {
      None = 0,
      Int,
      Double,
      DoubleComplex,
      HighReal,
      HighComplex
    } TypeOfNumber;
    ~NumberType();
    NumberType();
    NumberType( int value );
    NumberType( double value );
    NumberType( const complex<double> & value );
    NumberType( const ComplexHA & value );
    NumberType( const HighAccuracyNumber & value );
    NumberType( const NumberType & value );
    inline TypeOfNumber TypeOfNum() const{ return NumType; };
    NumberType operator - () const;
    NumberType operator + ( const NumberType & value ) const;
    NumberType operator - ( const NumberType & value ) const;
    NumberType operator * ( const NumberType & value ) const;
    NumberType operator / ( const NumberType & value ) const;
    bool operator < ( const NumberType & value ) const;
    bool operator > ( const NumberType & value ) const;
    bool operator <= ( const NumberType & value ) const;
    bool operator >= ( const NumberType & value ) const;
    bool operator == ( const NumberType & value ) const;
    NumberType & operator = ( const NumberType & value );
    bool IsReal() const;
    bool IsImag() const;
    bool IsComplex() const;
    bool IsNaN() const;
    bool IsInfinity() const;
    bool IsZero() const;
    int GetSign() const;
    bool IsNull() const;
    void LimitLength( int length );
    inline bool IsSpecial() const{ return IsNaN() or IsInfinity(); };
    NumberType RePart() const;
    NumberType ImPart() const;
    string ToString() const;
    static NumberType Divide( const NumberType & left 
			      , const NumberType & right , int length );
    static NumberType Square( const NumberType & value );
    NumberType Inverse( int length = 0 ) const;
    int GetLength() const;
    NumberType GetRe() const;
    NumberType GetIm() const;
    static NumberType ComplexConjugate( const NumberType & value );
    static NumberType Abs( const NumberType & value , int length = 0 );
    static NumberType MSquare( const NumberType & value );
    static NumberType InverseSqrt( const NumberType & value , int length = 0 );
    static NumberType Sqrt( const NumberType & value , int length = 0 );
    static bool ReadReal( string str , NumberType & result );
    inline int GetInt() const{ return Number.IntNumber; };
  protected :
    
  private :
    static bool check( string & );
    union 
    {
      int IntNumber;
      double DoubleNumber;
      HighAccuracyNumber * HReal;
      complex<double> * DComplex;
      ComplexHA * HAComplex;
    } Number;
    TypeOfNumber NumType;
  };
}

#endif    //_NUMBERTYPE_H  
