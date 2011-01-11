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

#ifndef _ALL_H
#define _ALL_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <complex>

#ifdef LINUX_
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#endif

#define UInt128 __uint128_t
#define UInt64 __uint64_t
#define UInt32 __uint32_t
#define UInt16 __uint16_t
#define UInt8 __uint16_t
#define Int128 __uint128_t
#define Int64 __int64_t
#define Int32 __int32_t
#define Int16 __uint16_t
#define Int8 __uint16_t

using namespace std;

//{_}include_head

#include "numbertype.h"
#include "expression.h"
#include "primary.h"
#include "main.h"
//{*}include_head


#endif
