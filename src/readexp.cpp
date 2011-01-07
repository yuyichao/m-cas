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
  bool Expression::IsInName( char c )
  {
    return ( c >= 'a' and c <= 'z' ) or ( c >= 'A' and c <= 'Z' ) or c == '_';
  };
  
  bool Expression::IsDigit( char c )
  {
    return c >= '0' and c<= '9';
  };
  
  bool Expression::ReadSet( string str , Expression * & result )
  {
    int counts = 0 , countm = 0 , countb = 0;
    for ( UInt32 i = 0 ; i < str.length() ; i++ )
      {
	switch ( str[i] )
	  {
	  case '(':
	    counts++;
	    break;
	  case ')':
	    counts--;
	    break;
	  case '[':
	    countm++;
	    break;
	  case ']':
	    countm--;
	    break;
	  case '{':
	    countb++;
	    break;
	  case '}':
	    countb--;
	    break;
	  case ':':
	    if ( str[ i + 1 ] != '=' ) break;
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    {
	      Expression ** para = new Expression*[2];
	      para[0] = make( str.substr( 0 , i ) );
	      para[1] = Construct( str.substr( i + 2 , str.length() - i - 2 ) );
	      result = make( Set , para , 2 , NULL );
	      para[0] -> detach();
	      para[1] -> detach();
	      delete []para;
	      return true;
	    }
	  case '=':
	    if ( str[ i + 1 ] == '=' ) break;
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    {
	      Expression ** para = new Expression*[2];
	      para[0] = make( str.substr( 0 , i ) );
	      para[1] = Construct( str.substr( i + 1 , str.length() - i - 1 ) );
	      try
		{
		  para[1] = Transform( para[1] );
		}
	      catch(...)
		{
		  para[0] -> detach();
		  delete []para;
		  throw;
		}
	      result = make( Set , para , 2 , NULL );
	      para[0] -> detach();
	      para[1] -> detach();
	      delete []para;
	      return true;
	    }
	  default:
	    break;
	  };
      }
    return false;
  };
  
  bool Expression::ReadChange( string str , Expression * & result )
  {
    int counts = 0 , countm = 0 , countb = 0;
    for ( UInt32 i = 0 ; i < str.length() ; i++ )
      {
	switch ( str[i] )
	  {
	  case '(':
	    counts++;
	    break;
	  case ')':
	    counts--;
	    break;
	  case '[':
	    countm++;
	    break;
	  case ']':
	    countm--;
	    break;
	  case '{':
	    countb++;
	    break;
	  case '}':
	    countb--;
	    break;
	  case '/':
	    if ( str[ i + 1 ] != '.' ) break;
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    {
	      Expression ** para = new Expression*[2];
	      para[0] = Construct( str.substr( 0 , i ) );
	      para[1] = Construct( str.substr( i + 2 , str.length() - i - 2 ) );
	      result = make( Change , para , 2 , NULL );
	      para[0] -> detach();
	      para[1] -> detach();
	      delete []para;
	      return true;
	    }
	  default:
	    break;
	  };
      }
    return false;
  };

  bool Expression::ReadTo( string str , Expression * & result )
  {
    int counts = 0 , countm = 0 , countb = 0;
    for ( UInt32 i = 0 ; i < str.length() ; i++ )
      {
	switch ( str[i] )
	  {
	  case '(':
	    counts++;
	    break;
	  case ')':
	    counts--;
	    break;
	  case '[':
	    countm++;
	    break;
	  case ']':
	    countm--;
	    break;
	  case '{':
	    countb++;
	    break;
	  case '}':
	    countb--;
	    break;
	  case '-':
	    if ( str[ i + 1 ] != '>' ) break;
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    {
	      Expression ** para = new Expression*[2];
	      para[0] = make( str.substr( 0 , i ) );
	      para[1] = Construct( str.substr( i + 2 , str.length() - i - 2 ) );
	      result = make( To , para , 2 , NULL );
	      para[0] -> detach();
	      para[1] -> detach();
	      delete []para;
	      return true;
	    }
	  default:
	    break;
	  };
      }
    return false;
  };
  
  bool Expression::ReadEqual( string str , Expression * & result )
  {
    int counts = 0 , countm = 0 , countb = 0;
    for ( UInt32 i = 0 ; i < str.length() ; i++ )
      {
	switch ( str[i] )
	  {
	  case '(':
	    counts++;
	    break;
	  case ')':
	    counts--;
	    break;
	  case '[':
	    countm++;
	    break;
	  case ']':
	    countm--;
	    break;
	  case '{':
	    countb++;
	    break;
	  case '}':
	    countb--;
	    break;
	  case '=':
	    if ( str[ i + 1 ] != '=' ) break;
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    {
	      Expression ** para = new Expression*[2];
	      para[0] = Construct( str.substr( 0 , i ) );
	      para[1] = Construct( str.substr( i + 2 , str.length() - i - 2 ) );
	      result = make( Equal , para , 2 , NULL );
	      para[0] -> detach();
	      para[1] -> detach();
	      delete []para;
	      return true;
	    }
	  case '!':
	    if ( str[ i + 1 ] != '=' ) break;
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    {
	      Expression ** para = new Expression*[2];
	      para[0] = Construct( str.substr( 0 , i ) );
	      para[1] = Construct( str.substr( i + 2 , str.length() - i - 2 ) );
	      result = make( UnEqual , para , 2 , NULL );
	      para[0] -> detach();
	      para[1] -> detach();
	      delete []para;
	      return true;
	    }
	  default:
	    break;
	  };
      }
    return false;
  };
  
  bool Expression::ReadAdd( string str , Expression * & result )
  {
    int counts = 0 , countm = 0 , countb = 0;
    vector<string> args;
    vector<int> attachs;
    attachs.push_back( 1 );
    int location = 0;
    for ( UInt32 i = 0 ; i < str.length() ; i++ )
      {
	switch ( str[i] )
	  {
	  case '(':
	    counts++;
	    break;
	  case ')':
	    counts--;
	    break;
	  case '[':
	    countm++;
	    break;
	  case ']':
	    countm--;
	    break;
	  case '{':
	    countb++;
	    break;
	  case '}':
	    countb--;
	    break;
	  case '+':
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    if ( i == 0 )
	      {
		location = 1;
		break;
	      }
	    args.push_back( str.substr( location , i - location ) );
	    attachs.push_back( 1 );
	    location = i + 1;
	    break;
	  case '-':
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    if ( i == 0 )
	      {
		location = 1;
		attachs[0] = -1;
		break;
	      }
	    args.push_back( str.substr( location , i - location ) );
	    attachs.push_back( -1 );
	    location = i + 1;
	    break;
	  default:
	    break;
	  };
      }
    args.push_back( str.substr( location , str.length() - location ) );
    if ( args.size() != 1 or attachs[0] == -1 )
      {
	Expression **para = new Expression*[ args.size() ];
	for ( UInt32 i = 0 ; i < args.size() ; i++ )
	  {
	    para[i] = Construct( args[i] );
	  }
	result = make( Add , para , args.size() , &(attachs[0]) );
	for ( UInt32 i = 0 ; i < args.size() ; i++ )
	  {
	    para[i] -> detach();
	  }
	delete []para;
	return true;
      }
    return false;
  };
  
  string Expression::AddCross( string str )
  {
    string result;
    for ( unsigned int i = 0 ; i < str.length() ; i++ )
      {
	result += str[i];
	if ( IsDigit( str[i] ) )
	  {
	    if ( IsInName( str[ i + 1 ] ) )
	      {
		bool need = true;
		for ( int j = i - 1 ; j >= 0 ; j-- )
		  {
		    if ( IsInName( str[j] ) )
		      {
			need = false;
			break;
		      }
		    if ( IsDigit( str[j] ) ) continue;
		    break;
		  }
		if ( need == false ) continue;
		result += '*';
		continue;
	      }
	    if ( str[ i + 1 ] == '(' )
	      {
		result += '*';
		continue;
	      }
	    if ( str[ i + 1 ] == '{' )
	      {
		result += '*';
		continue;
	      }
	    if ( str[ i + 1 ] == ' ' )
	      {
		for ( i++ ; i < str.length() - 1 ; i++ )
		  {
		    if ( str[ i + 1 ] == ' ' ) continue;
		    if ( IsDigit( str[ i + 1 ] ) or IsInName( str[ i + 1 ] ) or str[ i + 1 ] == '(' or str[ i + 1 ] == '{' )
		      {
			result += '*';
			break;
		      }
		    break;
		  }
		continue;
	      }
	    continue;
	  }
	if ( IsInName( str[i] ) )
	  {
	    if ( str[ i + 1 ] == '(' )
	      {
		result += '*';
		continue;
	      }
	    if ( str[ i + 1 ] == '{' )
	      {
		result += '*';
		continue;
	      }
	    if ( str[ i + 1 ] == ' ' )
	      {
		for ( i++ ; i < str.length() - 1 ; i++ )
		  {
		    if ( str[ i + 1 ] == ' ' ) continue;
		    if ( IsDigit( str[ i + 1 ] ) or IsInName( str[ i + 1 ] ) or str[ i + 1 ] == '(' or str[ i + 1 ] == '{' )
		      {
			result += '*';
			break;
		      }
		    break;
		  }
		continue;
	      }
	    continue;
	  }
	if ( str[i] == ')' or str[i] == '}' or str[i] == ']' )
	  {
	    if ( str[ i + 1 ] == '(' or str[ i + 1 ] == '{' )
	      {
		result += '*';
		continue;
	      }
	    if ( IsDigit( str[ i + 1 ] ) or IsInName( str[ i + 1 ] ) )
	      {
		result += '*';
		continue;
	      }
	    if ( str[ i + 1 ] == ' ' )
	      {
		for ( i++ ; i < str.length() - 1 ; i++ )
		  {
		    if ( str[ i + 1 ] == ' ' ) continue;
		    if ( IsDigit( str[ i + 1 ] ) or IsInName( str[ i + 1 ] ) or str[ i + 1 ] == '(' or str[ i + 1 ] == '{' )
		      {
			result += '*';
			break;
		      }
		    break;
		  }
		continue;
	      }
	    continue;
	  }
      }
    return result;
  };

  bool Expression::ReadMultiple( string str , Expression * & result )
  {
    str = AddCross( str );
    int counts = 0 , countm = 0 , countb = 0;
    vector<string> args;
    vector<int> attachs;
    attachs.push_back( 1 );
    int location = 0;
    for ( UInt32 i = 0 ; i < str.length() ; i++ )
      {
	switch ( str[i] )
	  {
	  case '(':
	    counts++;
	    break;
	  case ')':
	    counts--;
	    break;
	  case '[':
	    countm++;
	    break;
	  case ']':
	    countm--;
	    break;
	  case '{':
	    countb++;
	    break;
	  case '}':
	    countb--;
	    break;
	  case '*':
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    if ( i == 0 )
	      {
		throw "Unexpected *";
	      }
	    args.push_back( str.substr( location , i - location ) );
	    attachs.push_back( 1 );
	    location = i + 1;
	    break;
	  case '/':
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    if ( i == 0 )
	      {
		throw "Unexpected /";
	      }
	    args.push_back( str.substr( location , i - location ) );
	    attachs.push_back( -1 );
	    location = i + 1;
	    break;
	  default:
	    break;
	  };
      }
    args.push_back( str.substr( location , str.length() - location ) );
    if ( args.size() != 1 )
      {
	Expression **para = new Expression*[ args.size() ];
	for ( UInt32 i = 0 ; i < args.size() ; i++ )
	  {
	    para[i] = Construct( args[i] );
	  }
	result = make( Multiply , para , args.size() , &(attachs[0]) );
	for ( UInt32 i = 0 ; i < args.size() ; i++ )
	  {
	    para[i] -> detach();
	  }
	delete []para;
	return true;
      }
    return false;
  };

  bool Expression::ReadPower( string str , Expression * & result )
  {
    int counts = 0 , countm = 0 , countb = 0;
    for ( int i = str.length() - 1 ; i >= 0 ; i-- )
      {
	switch ( str[i] )
	  {
	  case '(':
	    counts++;
	    break;
	  case ')':
	    counts--;
	    break;
	  case '[':
	    countm++;
	    break;
	  case ']':
	    countm--;
	    break;
	  case '{':
	    countb++;
	    break;
	  case '}':
	    countb--;
	    break;
	  case '^':
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    {
	      Expression ** para = new Expression*[2];
	      para[0] = Construct( str.substr( 0 , i ) );
	      para[1] = Construct( str.substr( i + 1 , str.length() - i - 1 ) );
	      result = make( Power , para , 2 , NULL );
	      para[0] -> detach();
	      para[1] -> detach();
	      delete []para;
	      return true;
	    }
	  default:
	    break;
	  };
      }
    return false;
  };
  
  bool Expression::ReadSBraket( string str , Expression * & result )
  {
    UInt32 i = 0 , s;
    for ( ; str[i] == ' ' ; i++ );
    if ( str[i] != '(' ) return false;
    int counts = 1;
    for ( s = ++i ; i < str.length() ; i++ )
      {
	switch ( str[i] )
	  {
	  case '(':
	    counts++;
	    break;
	  case ')':
	    counts--;
	    break;
	  default:
	    break;
	  };
	if ( counts == 0 ) break;
      }
    for ( UInt32 j = i + 1 ; j < str.length() ; j++ )
      {
	if ( str[j] != ' ' ) return false;
      }
    result = Construct( str.substr( s , i - s ) );
    return true;
  };
  
  bool Expression::ReadFunction( string str , Expression * & result )
  {
    UInt32 i = 0 , s;
    for ( ; str[i] != '[' ; )
      {
	if ( ++i >= str.length() ) return false;
      }
    string name = str.substr( 0 , i );
    if ( ! AcceptableName( name ) ) return false;
    int countm = 1;
    for ( s = ++i ; i < str.length() ; i++ )
      {
	switch ( str[i] )
	  {
	  case '[':
	    countm++;
	    break;
	  case ']':
	    countm--;
	    break;
	  default:
	    break;
	  };
	if ( countm == 0 ) break;
      }
    for ( UInt32 j = i + 1 ; j < str.length() ; j++ )
      {
	if ( str[j] != ' ' ) return false;
      }
    int counts = 0 , countb = 0;
    vector<string> args;
    int location = s;
    for ( UInt32 k = s ; k < i ; k++ )
      {
	switch ( str[k] )
	  {
	  case '(':
	    counts++;
	    break;
	  case ')':
	    counts--;
	    break;
	  case '[':
	    countm++;
	    break;
	  case ']':
	    countm--;
	    break;
	  case '{':
	    countb++;
	    break;
	  case '}':
	    countb--;
	    break;
	  case ',':
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    if ( k == 0 ) throw "Empty Arg";
	    args.push_back( str.substr( location , k - location ) );
	    location = k + 1;
	    break;
	  default:
	    break;
	  };
      }
    args.push_back( str.substr( location , i - location ) );
    Expression **para = new Expression*[ args.size() ];
    for ( UInt32 k = 0 ; k < args.size() ; k++ )
      para[k] = Construct( args[k] );
    TypeOfExpre typeofexpre = UsrDefine;
    if ( name == "Sqrt" )
      {
	if ( args.size() != 1 )
	  {
	    for ( UInt32 k = 0 ; k < args.size() ; k++ )
	      para[k] -> detach();
	    delete []para;
	    throw "To Many/Few Input for Sqrt";
	  }
	result = make( Power , 2 );
	result -> Parameter[1] = make( NumberType(2) );
	result -> Parameter[0] = para[0];
	result -> Attach[1] = -1;
	delete []para;
	return true;
      }
    if ( name == "Exp" )
      {
	if ( args.size() != 1 )
	  {
	    for ( UInt32 k = 0 ; k < args.size() ; k++ )
	      para[k] -> detach();
	    delete []para;
	    throw "To Many/Few Input for Exp";
	  }
	result = make( Power , 2 );
	result -> Parameter[0] = make( E , 0 );
	result -> Parameter[1] = para[0];
	delete []para;
	return true;
      }
    if ( name == "Add" ) { typeofexpre = Add; goto SwitchEnd; }
    if ( name == "Multiply" ) { typeofexpre = Multiply; goto SwitchEnd; }
    if ( name == "Power" ) { typeofexpre = Power; goto SwitchEnd; }
    if ( name == "Sin" ) { typeofexpre = Sin; goto SwitchEnd; }
    if ( name == "Cos" ) { typeofexpre = Cos; goto SwitchEnd; }
    if ( name == "Tan" ) { typeofexpre = Tan; goto SwitchEnd; }
    if ( name == "Cot" ) { typeofexpre = Cot; goto SwitchEnd; }
    if ( name == "Sec" ) { typeofexpre = Sec; goto SwitchEnd; }
    if ( name == "Csc" ) { typeofexpre = Csc; goto SwitchEnd; }
    if ( name == "ArcSin" ) { typeofexpre = ArcSin; goto SwitchEnd; }
    if ( name == "ArcCos" ) { typeofexpre = ArcCos; goto SwitchEnd; }
    if ( name == "ArcTan" ) { typeofexpre = ArcTan; goto SwitchEnd; }
    if ( name == "ArcCot" ) { typeofexpre = ArcCot; goto SwitchEnd; }
    if ( name == "ArcSec" ) { typeofexpre = ArcSec; goto SwitchEnd; }
    if ( name == "ArcCsc" ) { typeofexpre = ArcCsc; goto SwitchEnd; }
    if ( name == "Log" ) { typeofexpre = Log; goto SwitchEnd; }
    if ( name == "D" ) { typeofexpre = D; goto SwitchEnd; }
    if ( name == "N" ) { typeofexpre = N; goto SwitchEnd; }
    if ( name == "Integrate" ) { typeofexpre = Integrate; goto SwitchEnd; }
    if ( name == "Set" ) { typeofexpre = Set; goto SwitchEnd; }
    if ( name == "Equal" ) { typeofexpre = Equal; goto SwitchEnd; }
    if ( name == "UnEqual" ) { typeofexpre = UnEqual; goto SwitchEnd; }
  SwitchEnd:
    result = make( typeofexpre , para , args.size() , NULL );
    if ( typeofexpre == UsrDefine ) result -> Name = name;
    for ( UInt32 k = 0 ; k < args.size() ; k++ )
      {
	para[k] -> detach();
      }
    delete []para;
    return true;
  };
  
  bool Expression::ReadLBraket( string str , Expression * & result )
  {
    UInt32 i = 0 , s;
    for ( ; str[i] != '{' ; )
      {
	if ( str[i] != ' ' ) return false;
	if ( ++i >= str.length() ) return false;
      }
    int countb = 1;
    for ( s = ++i ; i < str.length() ; i++ )
      {
	switch ( str[i] )
	  {
	  case '{':
	    countb++;
	    break;
	  case '}':
	    countb--;
	    break;
	  default:
	    break;
	  };
	if ( countb == 0 ) break;
      }
    for ( UInt32 j = i + 1 ; j < str.length() ; j++ )
      {
	if ( str[j] != ' ' ) return false;
      }
    int counts = 0 , countm = 0;
    vector<string> args;
    int location = s;
    for ( UInt32 k = s ; k < i ; k++ )
      {
	switch ( str[k] )
	  {
	  case '(':
	    counts++;
	    break;
	  case ')':
	    counts--;
	    break;
	  case '[':
	    countm++;
	    break;
	  case ']':
	    countm--;
	    break;
	  case '{':
	    countb++;
	    break;
	  case '}':
	    countb--;
	    break;
	  case ',':
	    if ( counts != 0 or countm != 0 or countb != 0 ) break;
	    if ( k == 0 ) throw "Empty Arg";
	    args.push_back( str.substr( location , k - location ) );
	    location = k + 1;
	    break;
	  default:
	    break;
	  };
      }
    args.push_back( str.substr( location , i - location ) );
    if ( args.size() != 1 )
      {
	Expression **para = new Expression*[ args.size() ];
	for ( UInt32 k = 0 ; k < args.size() ; k++ )
	  {
	    para[k] = Construct( args[k] );
	  }
	result = make( List , para , args.size() , NULL );
	for ( UInt32 k = 0 ; k < args.size() ; k++ )
	  {
	    para[k] -> detach();
	  }
	delete []para;
	return true;
      }
    result = Construct( args[0] );
    return true;
  };

  bool Expression::IsEmpty( string str )
  {
    if ( str.length() <= 0 ) return true;
    for ( UInt32 i = 0 ; i < str.length() ; i++ )
      {
	if ( str[i] != ' ' ) return false;
      }
    return true;
  };
  
  Expression * Expression::Construct( string str )
  {
    Expression * result;
    if ( IsEmpty( str ) ) throw "Invalid Input";
    if ( ReadSet( str , result ) ) return result;
    if ( ReadChange( str , result ) ) return result;
    if ( ReadTo( str , result ) ) return result;
    if ( ReadEqual( str , result ) ) return result;
    if ( ReadAdd( str , result ) ) return result;
    if ( ReadMultiple( str , result ) ) return result;
    if ( ReadPower( str , result ) ) return result;
    if ( ReadSBraket( str , result ) ) return result;
    if ( ReadFunction( str , result ) ) return result;
    if ( ReadLBraket( str , result ) ) return result;
    NumberType number;
    if ( NumberType::ReadReal( str , number ) )
      {
	return make( number );
      }
    if ( AcceptableName( str ) )
      {
	return make( str );
      }
    throw "Meaningless Input";
  };

  bool Expression::AcceptableName( string & str )
  {
    UInt32 i = 0;
    while( true )
      {
	if ( str[i] != ' ' ) break;
	if ( i++ == str.length() ) return false;
      }
    if ( ! IsInName( str[i] ) ) return false;
    string result = "";
    for ( ; i < str.length() ; i++ )
      {
	if ( str[i] == ' ' ) break;
	if ( !( IsInName( str[i] ) or IsDigit( str[i] ) ) ) return false;
	result += str[i];
      }
    for ( ; i < str.length() ; i++ )
      {
	if ( str[i] != ' ' ) return false;
      }
    str = result;
    return true;
  };
}
