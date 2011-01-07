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
  Replacement::Replacement()
    : Name("") , NumOfPara(0) , Parameters(NULL) , Definition(NULL) {};
  
  const Replacement Replacement::EmptyReplacement = Replacement();

  bool Replacement::IsEmpty()
  {
    return Name == "";
  };

  Replacement::Replacement( string name , string parameters[] , int numofpara , Expression * definition )
    : Name( name ) , NumOfPara( numofpara )
  {
    if ( numofpara )
      Parameters = new string[ numofpara ];
    else
      Parameters = NULL;
    for ( int i = 0 ; i < numofpara ; i++ )
      Parameters[i] = parameters[i];
    if ( definition != NULL )
      ( Definition = definition ) -> attach();
    else
      Definition = NULL;
  };

  Replacement::Replacement( const Replacement & value )
    : Name( value.Name ) , NumOfPara( value.NumOfPara )
  {
    if ( NumOfPara )
      Parameters = new string[ NumOfPara ];
    else
      Parameters = NULL;
    for ( int i = 0 ; i < NumOfPara ; i++ )
      Parameters[i] = value.Parameters[i];
    if ( value.Definition != NULL )
      ( Definition = value.Definition ) -> attach();
    else
      Definition = NULL;
  };

  Replacement Replacement::operator = ( const Replacement &value )
  {
    if ( this == &value ) return *this;
    Name = value.Name;
    NumOfPara = value.NumOfPara;
    delete []Parameters;
    if ( NumOfPara )
      Parameters = new string[ NumOfPara ];
    else
      Parameters = NULL;
    if ( Definition != NULL )
      Definition -> detach();
    if ( value.Definition != NULL )
      ( Definition = value.Definition ) -> attach();
    else
      Definition = NULL;
    return *this;
  };

  string Replacement::GetName() const
  {
    return Name;
  };

  Expression * Replacement::GetResult( Expression * parameters[] ) const
  {
    if ( Definition == NULL )
      return NULL;
    if ( NumOfPara == 0 )
      {
	Definition -> attach();
	return Definition;
      }
    Definition -> attach();
    Expression *expre = Definition -> unalias();
    return Replace( expre , parameters );
  };

  Expression * Replacement::Replace( Expression *expre , Expression *parameters[] )
    const
  {
    if ( expre -> ExpType == Expression::Str )
      for ( int i = 0 ; i < NumOfPara ; i++ )
	if ( Parameters[i] == expre -> Name )
	  {
	    expre -> detach();
	    parameters[i] -> attach();
	    return parameters[i];
	  }
    if ( expre -> ExpType == Expression::Number )
      return expre;
    Expression* result = Expression::make( expre -> ExpType , expre -> NumOfPara );
    for ( int i = 0 ; i < expre -> NumOfPara ; i++ )
      result -> Parameter[i] = Replace( expre -> Parameter[i] , parameters );
    return result;
  };
  
  int Replacement::GetParaNum() const
  {
    return NumOfPara;
  };
  
  Replacement::~Replacement()
  {
    delete []Parameters;
    Parameters = NULL;
    if ( Definition != NULL )
      Definition -> detach();
  };

  ReplaceChain * ReplaceChain::GlobalReplacements = new ReplaceChain();

  ReplaceChain::ReplaceChain()
    : Self( Replacement::EmptyReplacement ) , Next( NULL ) , Number( 0 ) {};
  
  ReplaceChain::ReplaceChain( const Replacement &self , ReplaceChain * const next )
    : Self( self ) , Next( next ) , Number( next == NULL ? 0 : ( next -> Number + 1 ) ) {};

  ReplaceChain * ReplaceChain::make( const Replacement &self , ReplaceChain * const next )
  {
    return new ReplaceChain( self , next );
  };

  int ReplaceChain::GetNumber() const
  {
    return Number;
  };

  ReplaceChain * ReplaceChain::GetNext() const
  {
    return Next;
  };

  Replacement ReplaceChain::GetCondition() const
  {
    return Self;
  };

  ReplaceChain * ReplaceChain::AddOne( const Replacement &self )
  {
    return new ReplaceChain( self , this );
  };

  ReplaceChain * ReplaceChain::CutTo( int number )
  {
    if ( Number == number )
      return this;
    if ( number < 0 ) throw "Replace Condition error";
    ReplaceChain * tmp = Next -> CutTo( number );
    delete this;
    return tmp;
  };
  
  int ReplaceChain::Check( string str ) const
  {
    for ( const ReplaceChain *tmp = this ; tmp -> Number > 0 ; tmp = tmp -> Next )
      if ( tmp -> Self.GetName() == str )
	return tmp -> Number;
    return -1;
  };
  
  bool ReplaceChain::AddTo( ReplaceChain * & orgin , const Replacement &added )
  {
    if ( orgin -> Check( added.GetName() ) > 0 ) return false;
    orgin = orgin -> AddOne( added );
    return true;
  };
  
  bool ReplaceChain::Clear( ReplaceChain * & orgin , string str )
  {
    ReplaceChain ** point = NULL;
    for ( ReplaceChain * tmp = orgin ; tmp -> Number > 0 ; tmp = tmp -> Next )
      {
	if ( tmp -> Self.GetName() == str )
	  {
	    if ( point != NULL )
	      *point = tmp -> Next;
	    else
	      orgin = tmp -> Next;
	    delete tmp;
	    return true;
	  }
	point = &( tmp -> Next );
      }
    return false;
  };
  
  const Replacement& ReplaceChain::Find( int number ) const
  {
    if ( number <= 0 or number > Number ) return Replacement::EmptyReplacement;
    for ( const ReplaceChain * tmp = this ; tmp -> Number > 0 ; tmp = tmp -> Next )
      if( tmp -> Number == number )
	return tmp -> Self;
    return Replacement::EmptyReplacement;
  };
  
  const Replacement& ReplaceChain::Find( string name ) const
  {
    for ( const ReplaceChain * tmp = this ; tmp -> Number > 0 ; tmp = tmp -> Next )
      if( tmp -> Self.GetName() == name )
	return tmp -> Self;
    return Replacement::EmptyReplacement;
  };
}
