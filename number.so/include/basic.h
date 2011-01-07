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

#ifndef _BASIC_H
#define _BASIC_H

namespace CAS
{
#ifndef _NUMERTYPE_H
  class DataArray;
#endif

  class HighAccuracyBase
  {
    private:
      static void FullMul( UInt64 left , UInt64 right , UInt64 result[2] );
      static UInt64 LongMod( const UInt64 left1[2] , UInt64 right );
      static void LongAdd( const UInt64 left1[2] , const UInt64 right1[2] , UInt64 result[2] );
      static void LongMin( const UInt64 left1[2] , const UInt64 right1[2] , UInt64 result[2] );
      static UInt64 ExEuclid( UInt64 a , UInt64 b , UInt64 &xa , UInt64 &yb );
      static void FiniteFFT( const UInt64 X[] , int logn , UInt64 w , UInt64 prime , UInt64 Fx[] );
      static void FiniteIFFT( const UInt64 Fx[] , int logn , UInt64 w , UInt64 prime , UInt64 X[] );
      static void ChRemain( UInt64 a1 , UInt64 a2 , UInt64 n1 , UInt64 n2 , UInt64 result[2] );
      static int FFTMul( const UInt32 left[] , const UInt32 right[] , int leftn , int rightn , DataArray * &result );
      static int FFTSquare( const UInt32 value[] , int length , DataArray * &result );
      static int NormalMul( const UInt32 left[] , const UInt32 right[] , int leftn , int rightn , DataArray * &result );
  public:
      static int LongMul( const UInt32 left[] , const UInt32 right[] , int leftn , int rightn , DataArray * &result );
      static int Square( const UInt32 value[] , int length , DataArray * &result );
  };
}

#endif    //_BASIC_H  
