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

#ifndef _EXPRESSION_H
#define _EXPRESSION_H

namespace CAS
{
  template< typename T >
  void SortUp( T * item[] , int );
  template< typename T , typename T2 >
  void SortUp( T * item[] , T2 attach[] , int );
  
  class Expression;

  class Replacement
  {
    friend class Expression;
    string Name;
    int NumOfPara;
    string * Parameters;
    Expression* Definition;
    Replacement();
    Expression* Replace( Expression* expre , Expression* parameters[] ) const;
  public:
    static const Replacement EmptyReplacement;
    Replacement( string name , string parameters[] , int numofpara
		 , Expression* definition );
    Replacement( const Replacement & );
    Replacement operator = ( const Replacement & );
    bool IsEmpty();
    string GetName() const;
    Expression* GetResult( Expression* parameters[] = NULL ) const;
    int GetParaNum() const;
    ~Replacement();
  };

  class ReplaceChain
  {
    const Replacement Self;
    ReplaceChain * Next;
    const int Number;
    ReplaceChain();
    ReplaceChain( const Replacement &self , ReplaceChain * const next );
    ReplaceChain( const ReplaceChain & );
    void operator = ( const ReplaceChain & );
    ReplaceChain * AddOne( const Replacement &self );
  public:
    static ReplaceChain * GlobalReplacements;
    static ReplaceChain * make( const Replacement &self
				, ReplaceChain * const next = NULL );
    int Check( string str ) const;
    static bool AddTo( ReplaceChain * & orgin , const Replacement &added );
    static bool Clear( ReplaceChain * & orgin , string str );
    int GetNumber() const;
    const Replacement& Find( int number ) const;
    const Replacement& Find( string name ) const;
    ReplaceChain * GetNext() const;
    Replacement GetCondition() const;
    ReplaceChain * CutTo( int number );
  };

  class Expression
  {
  public :
    enum TypeOfExpre
    {
      None = 0, Number, Str, Diff, List, Pi, E, UsrDefine, True, False, To,
      Add = 127, Multiply, Power, Sin, Cos, Tan, Cot, Sec, Csc, ArcSin, ArcCos, ArcTan, ArcCot, ArcSec, ArcCsc, Log, D, N, Integrate, Change, Set, Equal, UnEqual
    };//never return false when the two args of Equal or UnEqual is not a number
    string ToString();
    static Expression* Construct( string str );
    static Expression* Transform( Expression* expre , int length = 0
				   , ReplaceChain * condition
				   = ReplaceChain::GlobalReplacements );
    void attach();
    void detach();
    Expression* unalias();
    ~Expression();
    bool operator < ( const Expression & ) const;
    bool operator > ( const Expression & ) const;
    bool operator == ( const Expression & ) const;
    bool operator != ( const Expression & ) const;
    static string AddCross( string orignal );
    inline int GetAttach( int n ) const
    {
      if ( n >= NumOfPara ) return 0;
      if ( Attach == NULL ) return 1;
      return Attach[n];
    };
  private:
    friend Expression* Replacement::Replace( Expression* ,
					     Expression* parameters[] ) const;
    inline Expression*& P( int n ) const{ return Parameter[n]; }
    static bool IsEmpty( string );
    static bool IsFunctionName( string );
    static bool IsConstName( string );
    static bool CheckArgDiff( Expression* );
    static bool CheckArgNormal( Expression* );
    static bool CheckArgFunction( Expression* );
    static bool CheckArgTwo( Expression* );
    static bool CheckArgD( Expression* , ReplaceChain* );
    static bool CheckArgN( Expression* );
    static bool CheckArgIntegrate( Expression* );
    static bool CheckArgChange( Expression* );
    static bool CheckArgSet( Expression* );
    static bool CheckArgEqual( Expression* );
    static Expression* TrConst( Expression* , int , ReplaceChain* );    
    static Expression* TrC( Expression* , int , ReplaceChain* );    
    static Expression* TrStr( Expression* , int , ReplaceChain* );
    static Expression* TrDiff( Expression* , int , ReplaceChain* );
    static Expression* TrList( Expression* , int , ReplaceChain* );
    static Expression* TrUsrDefine( Expression* , int , ReplaceChain* );
    static bool OneAdd( Expression* , Expression*& , int , ReplaceChain* );
    static bool AddSim( Expression* , Expression*& , int , ReplaceChain* );
    static bool AddZero( Expression* , Expression*& , int , ReplaceChain* );
    static bool AddAdd( Expression* , Expression*& , int , ReplaceChain* );
    static bool AddNumber( Expression* , Expression*& , int , ReplaceChain* );
    static bool AddSign( Expression* , Expression*& , int , ReplaceChain* );
    static bool AddDiff( Expression* , Expression*& , int , ReplaceChain* );
    static bool AddList( Expression* , Expression*& , int , ReplaceChain* );
    static bool OneMul( Expression* , Expression*& , int , ReplaceChain* );
    static bool MulZero( Expression* , Expression*& , int , ReplaceChain* );
    static bool MulLog( Expression* , Expression*& , int , ReplaceChain* );
    static bool MulMul( Expression* , Expression*& , int , ReplaceChain* );
    static bool MulNumber( Expression* , Expression*& , int , ReplaceChain* );
    static bool MulOne( Expression* , Expression*& , int , ReplaceChain* );
    static bool MulList( Expression* , Expression*& , int , ReplaceChain* );
    static bool MulDiff( Expression* , Expression*& , int , ReplaceChain* );
    static bool MulSim( Expression* , Expression*& , int , ReplaceChain* );
    static bool MulSign( Expression* , Expression*& , int , ReplaceChain* );
    static bool MulSigna( Expression* , Expression*& , int , ReplaceChain* );
    static bool MulTri( Expression* , Expression*& , int , ReplaceChain* );
    static bool PowSign( Expression* , Expression*& , int , ReplaceChain* );
    static bool PowNumber( Expression* , Expression*& , int , ReplaceChain* );
    static bool PowPow( Expression* , Expression*& , int , ReplaceChain* );
    static bool PowList( Expression* , Expression*& , int , ReplaceChain* );
    static bool PowOne( Expression* , Expression*& , int , ReplaceChain* );
    static bool PowZero( Expression* , Expression*& , int , ReplaceChain* );
    static bool PowDiff( Expression* , Expression*& , int , ReplaceChain* );
    static bool PowLog( Expression* , Expression*& , int , ReplaceChain* );
    static bool LogSign( Expression* , Expression*& , int , ReplaceChain* );
    static bool LogNumber( Expression* , Expression*& , int , ReplaceChain* );
    static bool LogPow( Expression* , Expression*& , int , ReplaceChain* );
    static bool LogList( Expression* , Expression*& , int , ReplaceChain* );
    static bool LogOne( Expression* , Expression*& , int , ReplaceChain* );
    static bool LogSame( Expression* , Expression*& , int , ReplaceChain* );
    static bool LogDiff( Expression* , Expression*& , int , ReplaceChain* );
    static bool FunList( Expression* , Expression*& , int , ReplaceChain* );
    static Expression* TrAdd( Expression* , int , ReplaceChain* );
    static Expression* TrMultiply( Expression* , int , ReplaceChain* );
    static Expression* TrPower( Expression* , int , ReplaceChain* );
    static Expression* TrSin( Expression* , int , ReplaceChain* );
    static Expression* TrCos( Expression* , int , ReplaceChain* );
    static Expression* TrTan( Expression* , int , ReplaceChain* );
    static Expression* TrCot( Expression* , int , ReplaceChain* );
    static Expression* TrSec( Expression* , int , ReplaceChain* );
    static Expression* TrCsc( Expression* , int , ReplaceChain* );
    static Expression* TrArcSin( Expression* , int , ReplaceChain* );
    static Expression* TrArcCos( Expression* , int , ReplaceChain* );
    static Expression* TrArcTan( Expression* , int , ReplaceChain* );
    static Expression* TrArcCot( Expression* , int , ReplaceChain* );
    static Expression* TrArcSec( Expression* , int , ReplaceChain* );
    static Expression* TrArcCsc( Expression* , int , ReplaceChain* );
    static Expression* TrLog( Expression* , int , ReplaceChain* );
    static Expression* TrD( Expression* , int , ReplaceChain* );
    static Expression* TrN( Expression* , int , ReplaceChain* );
    static Expression* TrIntegrate( Expression* , int , ReplaceChain* );
    static Expression* TrChange( Expression* , int , ReplaceChain* );
    static Expression* TrSet( Expression* , int , ReplaceChain* );
    static Expression* TrEqual( Expression* , int , ReplaceChain* );
    static Expression* make();
    static Expression* make( string name );
    static Expression* make( TypeOfExpre , Expression** , int , int* );
    static Expression* make( TypeOfExpre , int );
    static Expression* make( const NumberType & value );
    static bool IsInName( char );
    static bool IsDigit( char );
    static bool ReadSet( string str , Expression*& result );
    static bool ReadChange( string str , Expression*& result );
    static bool ReadTo( string str , Expression*& result );
    static bool ReadEqual( string str , Expression*& result );
    static bool ReadAdd( string str , Expression*& result );
    static bool ReadMultiple( string str , Expression*& result );
    static bool ReadPower( string str , Expression*& result );
    static bool ReadSBraket( string str , Expression*& result );
    static bool ReadFunction( string str , Expression*& result );
    static bool ReadLBraket( string str , Expression*& result );
    static bool AcceptableName( string & str );
    static bool IfuAdd( Expression* );
    static bool IfuMul( Expression* );
    static bool IfuPow( Expression* );
    Expression();
    Expression( string name );
    Expression( TypeOfExpre exptype , Expression** parame , int paranum
		, int * attach );
    Expression( TypeOfExpre exptype , int paranum );
    Expression( const NumberType & );
    UInt64 Complexity() const;
    Expression & operator = ( const Expression & );
    Expression( const Expression & );
    TypeOfExpre ExpType;
    string Name;
    Expression** Parameter;
    int * Attach;
    int NumOfPara;
    NumberType Value;
    int refcount;
  };
}

#endif    //_EXPRESSION_H  
