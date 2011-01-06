// -*- C++ -*-
#include "all.h"

#ifndef _EXPRESSION_H
#define _EXPRESSION_H

namespace CAS
{
  template< typename T >
  void SortUp( T * item[] , int length );
  template< typename T , typename T2 >
  void SortUp( T * item[] , T2 attach[] , int length );
  
  class Expression;

  class Replacement
  {
    friend class Expression;
    string Name;
    int NumOfPara;
    string * Parameters;
    Expression * Definition;
    Replacement();
    Expression * Replace( Expression * expre , Expression *parameters[] ) const;
  public:
    static const Replacement EmptyReplacement;
    Replacement( string name , string parameters[] , int numofpara
		 , Expression * definition );
    Replacement( const Replacement & );
    Replacement operator = ( const Replacement &value );
    bool IsEmpty();
    string GetName() const;
    Expression * GetResult( Expression * parameters[] = NULL ) const;
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
    static Expression * Construct( string str );
    static Expression * Transform( Expression * expre , int length = 0
				   , ReplaceChain * condition
				   = ReplaceChain::GlobalReplacements );
    void attach();
    void detach();
    Expression * unalias();
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
    friend Expression * Replacement::Replace( Expression * expre ,
					      Expression *parameters[] ) const;
    inline Expression *& P( int n ) const{ return Parameter[n]; }
    static bool IsEmpty( string );
    static bool IsFunctionName( string );
    static bool IsConstName( string );
    static bool CheckArgDiff( Expression * expre );
    static bool CheckArgNormal( Expression * expre );
    static bool CheckArgFunction( Expression * expre );
    static bool CheckArgTwo( Expression * expre );
    static bool CheckArgD( Expression * expre , ReplaceChain * condition );
    static bool CheckArgN( Expression * expre );
    static bool CheckArgIntegrate( Expression * expre );
    static bool CheckArgChange( Expression * expre );
    static bool CheckArgSet( Expression * expre );
    static bool CheckArgEqual( Expression * expre );
    static Expression * TrConst( Expression * expre , int length , ReplaceChain * condition );    
    static Expression * TrC( Expression * expre , int length , ReplaceChain * condition );    
    static Expression * TrStr( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrDiff( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrList( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrUsrDefine( Expression * expre , int length , ReplaceChain * condition );
    static bool OneAdd( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool AddSim( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool AddZero( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool AddAdd( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool AddNumber( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool AddSign( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool AddDiff( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool AddList( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool OneMul( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool MulZero( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool MulMul( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool MulNumber( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool MulOne( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool MulList( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool MulDiff( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool MulSim( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool MulSign( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool MulSigna( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool MulTri( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool PowSign( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool PowNumber( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool PowPow( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool PowList( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool PowOne( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool PowZero( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool PowDiff( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool PowLog( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static bool FunList( Expression * expre , Expression * & , int length , ReplaceChain * condition );
    static Expression * TrAdd( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrMultiply( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrPower( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrSin( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrCos( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrTan( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrCot( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrSec( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrCsc( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrArcSin( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrArcCos( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrArcTan( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrArcCot( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrArcSec( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrArcCsc( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrLog( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrD( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrN( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrIntegrate( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrChange( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrSet( Expression * expre , int length , ReplaceChain * condition );
    static Expression * TrEqual( Expression * expre , int length , ReplaceChain * condition );
    static Expression * make();
    static Expression * make( string name );
    static Expression * make( TypeOfExpre exptype , Expression ** parame
			      , int paranum , int * attach );
    static Expression * make( TypeOfExpre exptype , int paranum );
    static Expression * make( const NumberType & value );
    static bool IsInName( char c );
    static bool IsDigit( char c );
    static bool ReadSet( string str , Expression * & result );
    static bool ReadChange( string str , Expression * & result );
    static bool ReadTo( string str , Expression * & result );
    static bool ReadEqual( string str , Expression * & result );
    static bool ReadAdd( string str , Expression * & result );
    static bool ReadMultiple( string str , Expression * & result );
    static bool ReadPower( string str , Expression * & result );
    static bool ReadSBraket( string str , Expression * & result );
    static bool ReadFunction( string str , Expression * & result );
    static bool ReadLBraket( string str , Expression * & result );
    static bool AcceptableName( string & str );
    static bool IfuAdd( Expression * expre );
    static bool IfuMul( Expression * expre );
    static bool IfuPow( Expression * expre );
    Expression();
    Expression( string name );
    Expression( TypeOfExpre exptype , Expression ** parame , int paranum
		, int * attach );
    Expression( TypeOfExpre exptype , int paranum );
    Expression( const NumberType & value );
    UInt64 Complexity() const;
    Expression & operator = ( const Expression & );
    Expression( const Expression & value );
    TypeOfExpre ExpType;
    string Name;
    Expression ** Parameter;
    int * Attach;
    int NumOfPara;
    NumberType Value;
    int refcount;
  };
}

#endif    //_EXPRESSION_H  
