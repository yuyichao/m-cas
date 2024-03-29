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

#ifndef _PRIMARY_H
#define _PRIMARY_H

namespace CAS
{
  class PrimaryFunction
  {
  public:
    static const HighAccuracyNumber Two;
    static const HighAccuracyNumber OneOverFour;    
    static HighAccuracyNumber AGM( const HighAccuracyNumber & a , const HighAccuracyNumber & b , int length );
    static ComplexHA AGM( const ComplexHA & a , const ComplexHA & b , int length );
    static HighAccuracyNumber ValueOfPi( int length );
    static HighAccuracyNumber ValueOfE( int length );
    static HighAccuracyNumber Arg( const ComplexHA & value , int length = 0 );
    static NumberType Arg( const NumberType & value , int length = 0 );
    static HighAccuracyNumber Sin( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber Cos( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber Tan( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber Cot( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber Sec( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber Csc( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber ArcSin( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber ArcCos( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber ArcTan( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber ArcCot( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber ArcSec( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber ArcCsc( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber Exp( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber Ln( const HighAccuracyNumber & value , int length );
    static HighAccuracyNumber LnB( int length );
    static HighAccuracyNumber Power( const HighAccuracyNumber & value , const HighAccuracyNumber & n , int length );
    static HighAccuracyNumber Log( const HighAccuracyNumber & value , const HighAccuracyNumber & n , int length );
    static ComplexHA Sin( const ComplexHA & value , int length );
    static ComplexHA Cos( const ComplexHA & value , int length );
    static ComplexHA Tan( const ComplexHA & value , int length );
    static ComplexHA Cot( const ComplexHA & value , int length );
    static ComplexHA Sec( const ComplexHA & value , int length );
    static ComplexHA Csc( const ComplexHA & value , int length );
    static ComplexHA ArcSin( const ComplexHA & value , int length );
    static ComplexHA ArcCos( const ComplexHA & value , int length );
    static ComplexHA ArcTan( const ComplexHA & value , int length );
    static ComplexHA ArcCot( const ComplexHA & value , int length );
    static ComplexHA ArcSec( const ComplexHA & value , int length );
    static ComplexHA ArcCsc( const ComplexHA & value , int length );
    static ComplexHA Exp( const ComplexHA & value , int length );
    static ComplexHA Ln( const ComplexHA & value , int length );
    static ComplexHA Power( const ComplexHA & value , const ComplexHA & n , int length );
    static ComplexHA Log( const ComplexHA & value , const ComplexHA & n , int length );
    static ComplexHA RePartModTwoPi( const ComplexHA & value , int length = 0 );
    static NumberType Sin( const NumberType & value , int length );
    static NumberType Cos( const NumberType & value , int length );
    static NumberType Tan( const NumberType & value , int length );
    static NumberType Cot( const NumberType & value , int length );
    static NumberType Sec( const NumberType & value , int length );
    static NumberType Csc( const NumberType & value , int length );
    static NumberType ArcSin( const NumberType & value , int length );
    static NumberType ArcCos( const NumberType & value , int length );
    static NumberType ArcTan( const NumberType & value , int length );
    static NumberType ArcCot( const NumberType & value , int length );
    static NumberType ArcSec( const NumberType & value , int length );
    static NumberType ArcCsc( const NumberType & value , int length );
    static NumberType Exp( const NumberType & value , int length );
    static NumberType Ln( const NumberType & value , int length );
    static NumberType Power( const NumberType & value , const NumberType & n , int length );
    static NumberType Log( const NumberType & value , const NumberType & n , int length );
    template<typename T>
    static inline T log_2( T value,T base ){return log(value)/log(base);};
  };
}
#endif    //_PRIMARY_H  
