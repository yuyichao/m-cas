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
  Console();
#ifdef LINUX_
  resetterm();
#endif
  return 0;
};

namespace CAS
{
  string name = "m-cas";
  int version[4] = {0,0,2,3};
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
#ifdef LINUX_
    setterm();
    signal( SIGINT , catchsig );
    signal( SIGABRT , catchsig );
    signal( SIGQUIT , catchsig );
    signal( SIGTERM , catchsig );
    signal( SIGSEGV , catchsig );
    signal( SIGILL , catchsig );
    signal( SIGBUS , catchsig );
#endif
  }

#ifdef LINUX_
  static termios stored_settings; 
  static bool iftty = false;
  void setterm() 
  {
    if ( !isatty( STDIN_FILENO ) ) return;
    iftty = true;
    termios new_settings; 
    tcgetattr( STDIN_FILENO , &stored_settings );
    new_settings = stored_settings;
    new_settings.c_lflag &= ( ~ICANON ); 
    new_settings.c_lflag &= ( ~ECHO ); 
    new_settings.c_cc[VTIME] = 0; 
    new_settings.c_cc[VMIN] = 1; 
    tcsetattr( STDIN_FILENO , TCSANOW , &new_settings ); 
    return; 
  };
  
  void resetterm() 
  {
    if ( iftty )
      tcsetattr( STDIN_FILENO , TCSANOW , &stored_settings );
    return;
  };

  void catchsig( int num )
  {
    resetterm();
    exit(num);
  };
#endif
  
  string readline()
  {
#ifdef LINUX_
    static char *a = new char[6];
    string tmpbuf;
    unsigned int loc = 0;
    while( true )
      {
	for ( int i = 0 ; i < 6 ; i++ )
	  a[i] = 0;
	read( STDIN_FILENO , a , 6 );
	switch ( a[0] )
	  {
	  case 27:
	    switch ( a[1] )
	      {
	      case 91:
		switch ( a[2] )
		  {
		  case 65:
		    goto Up;
		  case 66:
		    goto Down;
		  case 68:
		    goto Left;
		  case 67:
		    goto Right;
		  case 51:
		    if ( a[3] == 126 )goto Delete;
		    continue;
		  case 53:
		    if ( a[3] == 126 )goto Up;
		    continue;
		  case 54:
		    if ( a[3] == 126 )goto Up;
		    continue;
		  default:
		    continue;
		  };
	      case 79:
		switch ( a[2] )
		  {
		  case 72:
		    goto Home;
		  case 70:
		    goto End;
		  default:
		    continue;
		  };
	      case 0:
		continue;
	      default:
		continue;
	      };
	  case 127:
	    goto Backspace;
	  case 1:
	    goto Home;
	  case 2:
	    goto Left;
	  case 4:
	    goto Delete;
	  case 5:
	    goto End;
	  case 6:
	    goto Right;
	  case 10:
	    goto Enter;
	  case 11:
	    continue;
	  case 14:
	    goto Down;
	  case 16:
	    goto Up;
	  case 25:
	    continue;
	  case 32 ... 126:
	    break;
	  default:
	    continue;
	  Left:
	    if ( loc > 0 )
	      {
		loc--;
		cout << "\033\133\104" << flush;
	      }
	    continue;
	  Right:
	    if ( loc < tmpbuf.length() )
	      {
		loc++;
		cout << "\033\133\103" << flush;
	      }
	    continue;
	  Up:
	    continue;
	  Down:
	    continue;
	  Home:
	    for ( ; loc > 0 ; loc-- )
	      cout << "\033\133\104" << flush;
	    continue;
	  End:
	    for ( ; loc < tmpbuf.length() ; loc++ )
	      cout << "\033\133\103"<< flush;
	    continue;
	  Delete:
	    if ( loc >= tmpbuf.length() ) continue;
	    tmpbuf = tmpbuf.substr(0,loc)+tmpbuf.substr(loc+1,tmpbuf.length()-loc);
	    cout << tmpbuf.substr(loc , tmpbuf.length() - loc) << " " << flush;
	    for ( unsigned int i = 0 ; i <= tmpbuf.length() - loc ; i++ )
	      cout << "\033\133\104";
	    cout << flush;
	    continue;
	  Enter:
	    cout << "\n" << flush;
	    return tmpbuf;
	  Backspace:
	    if ( loc == 0 ) continue;
	    tmpbuf=tmpbuf.substr(0,loc-1)+tmpbuf.substr(loc,tmpbuf.length()-loc+1);
	    loc--;
	    cout<<"\b"<<tmpbuf.substr(loc,tmpbuf.length()-loc)<<" "<<flush;
	    for ( unsigned int i = 0 ; i <= tmpbuf.length() - loc ; i++ )
	      cout << "\033\133\104";
	    cout << flush;
	    continue;
	  };
	tmpbuf = tmpbuf.substr(0,loc)+*a+tmpbuf.substr(loc,tmpbuf.length()-loc+1);
	cout << tmpbuf.substr( loc , tmpbuf.length() - loc ) << flush;
	loc++;
	for ( unsigned int i = 0 ; i < tmpbuf.length() - loc ; i++ )
	  cout << "\033\133\104";
	cout << flush;
      }
    return tmpbuf;
#else
    string command;
    getline( cin , command );
    return command;
#endif
  };
  
  int Console()
  {
    Expression * expre , * tmp;
    string command;
    while ( true )
    {
      cin.clear();
      cout << endl;
      cout << "In[" << countIn << "]:= " << flush;
      if (( command = readline() ) == "Exit" ) return 0; //退出
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
  };
}
