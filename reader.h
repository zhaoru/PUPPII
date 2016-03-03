
#ifndef MY_READER_H
#define MY_READER_H

#include "base.h"

class reader : public base {

 public :

 reader(TTree *tree=0);
  ~reader (){} ;

  Int_t InitEvent( Long64_t event ) ;

 private :

};

#endif
