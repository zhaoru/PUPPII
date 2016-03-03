

#include "reader.h"

#include <iostream>
#include <algorithm>

Int_t reader::InitEvent( Long64_t event){

  const Int_t val = GetEntry( event ); // <= function of base.h

  // Prepare variable which are calculated from input files, if you have any.



  return val ; 
}


reader::reader(TTree *tree) : base( tree ){


};

