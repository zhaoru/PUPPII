#ifndef MY_ANALYZR
#define MY_ANALYZR


#include <vector>
#include <string>
#include <map>

#include <TH1F.h>
#include <TH2F.h>
#include <TString.h>
#include <TGraphErrors.h>

#include "reader.h"

class analyzer {

	private : 

		std::vector<std::string> filelist  ;
		std::string output ; 
		std::string PUMitigation;
		std::string JetAlgo;

	public : 

		analyzer ();
		~analyzer ();

		inline void addInputFileName ( std::string name ){ filelist.push_back(name); };
		inline void setOutputFileName( std::string name ){ output.assign(name); };
		inline void setPUMitigation( std::string name ){ PUMitigation.assign(name); };
		inline void setJetAlgo( std::string name ){ JetAlgo.assign(name); };


		void do_analyses();

		void SetPUReweightingFile( std::string data_path , std::string mc_path );

	private :

		void init();
		void postProcess();
		void FillHistogram( TH1F * h , double val , double weight );
		void FillHistogram2D( TH2F * h , double val_x , double val_y,  double weight );
		void _PreparePileupReweightingBasedonTruthPileup( );

		bool b_PerformPUReweighting;
		double PUScaleFactor [100];
		std::string PUReweighting_DataFile ;
		std::string PUReweighting_MCFile;

		TFile * f_out ;
		TDirectory * main_directory ;


		TH1F * h_nJet ; 
		TH1F * h_pt ; 
		TH1F * h_pt_resolution ;

		TH1F * h_tau21 ; 
		TH1F * h_tau32 ; 

		TH1F * h_PrunedMass;
		TH1F * h_FilteredMass;
		TH1F * h_SoftDropMass;
		TH1F * h_TrimmedMass;
		TH1F *h_pt_resolution_muti[10][10];
		TGraphErrors *TG_pt_mean[10];
		TGraphErrors *TG_pt_rms[10];
 TGraphErrors *TG_eta_mean[10];
                TGraphErrors *TG_eta_rms[10];
		char *histname = new char[100];
		char *histname_1 = new char[100];
		char *histname_2 = new char[100];
		const double pt_bins[7] = {0, 30, 50, 75, 100, 200,250};
		const double eta_bins[6] = {0, 1.3, 2, 2.5, 3, 5};
};

#endif
