
#include "analyzer.h"

#include <unistd.h>
#include <iostream>
#include <stdlib.h>

#include <fstream>

int main(int argc,char *argv[]){

  analyzer ana;

  TH1::SetDefaultSumw2();

  //  std::string PU_reweighting_data("");
  //  std::string PU_reweighting_mc("");

  {
    int c ;
    while(( c = getopt(argc,argv,"i:o:j:p:")) != -1 ){
      
      switch( c){
	
      case 'i' :
	{
	  ana . addInputFileName( std::string(optarg) );
	  break;
	}
      case 'o' :
	{
	  ana . setOutputFileName( std::string(optarg) );
	  break ; 
	}
      case 'j' :
	{
	  ana . setJetAlgo( std::string(optarg) );
	  break ; 
	}
      case 'p' :
	{
	  ana . setPUMitigation( std::string(optarg) );
	  break ; 
	}
      }//end switch
    }//end while-Loop
  } //end scope.

  //  ana.SetPUReweightingFile( PU_reweighting_data , PU_reweighting_mc );

  ana.do_analyses ();

  return 0 ; 

}
