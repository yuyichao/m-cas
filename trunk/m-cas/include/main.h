// -*- C++ -*-
#ifndef _MAIN_H
#define _MAIN_H

int main();
void Debug();

namespace CAS
{
  extern string name;
  extern int version[4];
  extern vector<Expression*> In,Out;
  extern int countIn;
  void F( int * &a );
  void Initial();
  int Console();
}

#endif    //_MAIN_H
