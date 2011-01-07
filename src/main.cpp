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

using namespace CAS;

int main()
{
  Initial();
  Debug();
  Console();
  return 0;
};

void DebugSqrt()
{
  int n = 2000;
  ComplexHA a = 1 , b = 1;
  srand(time(NULL));
  for ( int i = 0 ; i < n ; i ++ )
    {
      a = a * rand();
      b = b * rand();
    }
  a = a + b * ComplexHA::I;
  cout << "start" << endl;
  for ( int j = 0 ; j < 1 ; j ++ )
    {
      ComplexHA::Sqrt(a);
    }
  cout << a.ToString() << endl;
  cout << "end" << endl;
  return;
};

void Debug()
{
};

namespace CAS
{
  string name = "m-cas";
  int version[4] = {0,0,1,3};
  vector<Expression*> In , Out;
  int countIn = 1;
  void Initial()
  {
    cout << name << " "
	 << version[0] << "."
	 << version[1] << "."
	 << version[2] << "-"
	 << version[3] << endl;
    cout << "Copyright (C) 2010, 2011 Yu Yichao, Yang Dong" << endl;
  }
  
  int Console()
  {
    Expression * expre , * tmp;
    string command;
    while ( true )
    {
      cin.clear();
      cout << endl;
      cout << "In[" << countIn << "]:= " << flush;
      getline( cin ,command );
      if ( command == "Exit" ) return 0; //退出
      try
	{
	  expre = Expression::Construct( command );
	}
      catch ( char const* str )
	{
	  cout << str << endl;
	  continue;
	}
      expre -> attach();
      tmp = expre -> unalias();
      try
	{
	  expre = Expression::Transform( expre );
	}
      catch ( char const* str )
	{
	  tmp -> detach();
	  cout << str << endl;
	  continue;
	}
      cout << "Out[" << countIn << "]= " << flush;
      cout << expre -> ToString() << endl;
      In.push_back( tmp );
      Out.push_back( expre );
      countIn++;
    }
  }
}
