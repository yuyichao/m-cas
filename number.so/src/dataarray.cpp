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
  DataArray::DataArray(int length)
  {
    Length = length;
    Data = new UInt32[length];
    RefCount = 1;
  };

  DataArray::DataArray( const DataArray& value )
  {
    Length = value.Length;
    Data = new UInt32[Length];
    for (int i = 0 ; i < Length ; i++)
      {
	Data[i] = value.Data[i];
      }
    RefCount = 1;
  };

  DataArray::~DataArray()
  {
    delete []Data;
    Data = NULL;
  };
};
