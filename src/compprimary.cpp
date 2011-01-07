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

using namespace std;

namespace CAS
{
   ComplexHA PrimaryFunction::AGM( const ComplexHA & , const ComplexHA & , int )
   {
     
     return ComplexHA();
   };
   
  HighAccuracyNumber PrimaryFunction::Arg( const ComplexHA & , int )
  {
    
    return HighAccuracyNumber();
  };
  
  ComplexHA PrimaryFunction::Sin( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Cos( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Tan( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Cot( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Sec( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Csc( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::ArcSin( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::ArcCos( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::ArcTan( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::ArcCot( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::ArcSec( const ComplexHA & , int )
  {

     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::ArcCsc( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Exp( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };

  ComplexHA PrimaryFunction::Ln( const ComplexHA & , int )
  {
    
     return ComplexHA();
  };
  
  ComplexHA PrimaryFunction::Power( const ComplexHA & , const ComplexHA & , int )
  {
    
     return ComplexHA();
  };

  ComplexHA PrimaryFunction::Log( const ComplexHA & , const ComplexHA & , int )
  {

     return ComplexHA();
  };
}
