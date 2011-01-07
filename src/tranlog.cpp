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

namespace CAS
{
  bool Expression::LogSign( Expression* expre , Expression*& result, int length , ReplaceChain * condition )
  {

    return false;
  };
  
  bool Expression::LogNumber( Expression* expre , Expression*& result, int length , ReplaceChain* )
  {

    return false;
  };
  
  bool Expression::LogPow( Expression* expre , Expression*& result, int length , ReplaceChain * condition )
  {

    return false;
  };
  
  bool Expression::LogList( Expression* expre , Expression*& result, int length , ReplaceChain * condition )
  {

    return false;
  };
  
  bool Expression::LogOne( Expression* expre , Expression*& result, int length , ReplaceChain * condition )
  {

    return false;
  };
  
  bool Expression::LogSame( Expression* expre , Expression*& result, int length , ReplaceChain * condition )
  {
    if (( expre -> NumOfPara == 1 and expre -> P(0) -> ExpType == E )
	or ( expre -> NumOfPara == 2 and *(expre -> P(0)) == *(expre -> P(1))))
      {
	result = make( NumberType::One );
	expre -> detach();
	return true;
      }
    return false;
  };
  
  bool Expression::LogDiff( Expression* expre , Expression*& result, int length , ReplaceChain * condition )
  {

    return false;
  };
  
  bool Expression::LogLog( Expression* expre , Expression*& result, int length , ReplaceChain * condition )
  {

    return false;
  };
}
