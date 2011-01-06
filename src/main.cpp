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
  int version[4] = {0,0,1,1};
  vector<Expression*> In , Out;
  int countIn = 1;
  void Initial()
  {
    cout << name << version[0] << "."
	 << version[1] << "."
	 << version[2] << "-"
	 << version[3] << endl;
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
