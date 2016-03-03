
#include "analyzer.h"

#include <iostream>
#include <math.h>
#include <algorithm>

#include <TLorentzVector.h>

#include "reader.h"

#define MAX_N_NPTPLOT 100

analyzer::analyzer ()
	: output("out.root")
	  , PUMitigation("")
	  , JetAlgo("")
{

}

analyzer::~analyzer (){

}

void analyzer::do_analyses(){
	const int bins = 6;

	auto mean = new double[10][10]();
	auto rms = new double[10][10]();

	init();

	const std::string JESRecalibrationLevel = ( PUMitigation == "PFPUPPI" ) ? "Uncorrected" : "L3Absolute" ;
	std::cout << "JEC of " << JESRecalibrationLevel << " is used because the PU mitigation of the input jet is " << PUMitigation << std::endl ;

	for( std::vector<std::string>::iterator f = filelist.begin(); f != filelist.end(); f++ ){

		std::cout << "Input file = \""<<(*f)<<"\" is opend." << std::endl ;

		TFile * tf = TFile::Open( f->c_str() );
		TTree * t ;
		tf->GetObject( (JetAlgo+PUMitigation+"/t").c_str(), t );
		reader r( t );

		const long NENTRIES = t->GetEntries();

		for (Long64_t jEntry=0; jEntry < NENTRIES ; jEntry ++ )
			//for (Long64_t jEntry=0; jEntry < 100 ; jEntry ++ )
		{

			{ // Prepare event information
				Long64_t ientry = r.LoadTree(jEntry);
				if (ientry < 0) break;
				r . InitEvent (jEntry);
			}

			FillHistogram( h_nJet ,  r.p4_ , 1.0 );
			for( int iJet = 0 ; iJet < r.p4_ ; iJet ++ ){
				//for( int iJet = 0 ; iJet < 1 ; iJet ++ ){

				const double pt = r.p4_fCoordinates_fPt[iJet] * r.jec_factors->at(iJet)[ JESRecalibrationLevel ] ;
				const double eta = r.p4_fCoordinates_fEta[iJet] * r.jec_factors->at(iJet)[ JESRecalibrationLevel ] ;

				FillHistogram( h_pt , pt , 1.0 );

				if( pt > 200  ){

					FillHistogram( h_tau21   , r.tau2->at(iJet) / r.tau1 ->at(iJet) , 1.0 );
					FillHistogram( h_tau32   , r.tau3->at(iJet) / r.tau2 ->at(iJet) , 1.0 );
					FillHistogram( h_PrunedMass   , r.PrunedMass   ->at(iJet) , 1.0 );
					FillHistogram( h_FilteredMass , r.FilteredMass ->at(iJet) , 1.0 );
					FillHistogram( h_SoftDropMass , r.SoftDropMass ->at(iJet) , 1.0 );
					FillHistogram( h_TrimmedMass  , r.TrimmedMass  ->at(iJet) , 1.0 );
				}

				const double pt_gen = r.gen_p4_fCoordinates_fPt[iJet] ;
				if( pt_gen <= 0 ) continue; // = no gen_jet is matched to this jet.

				FillHistogram( h_pt_resolution , ( pt - pt_gen ) / pt_gen , 1.0 );

				//Begin pT_resolution pt&eta loop
				for (Int_t i=0; i<bins-1; i++) {
					for (Int_t j=0; j<bins-1; j++) {

						if (pt>pt_bins[i] && pt<pt_bins[i+1] &&
								fabs(eta)>eta_bins[j] && fabs(eta)<eta_bins[j+1]) {
							FillHistogram( h_pt_resolution_muti[i][j] , ( pt - pt_gen ) / pt_gen , 1.0 );


						}
					}
				}
				if (pt>pt_bins[bins-1]) {
					for (Int_t i=0; i<bins-1; i++) {
						FillHistogram( h_pt_resolution_muti[bins-1][i] , ( pt - pt_gen ) / pt_gen , 1.0 );
					}
				}
				//End pT_resolution pt&eta loop
			}

			// - - - - ANALYSES END - - - - - - - -

			} // Events-loop ends

		} // file loop ends.
		for (Int_t i=0; i<bins; i++) {
			for (Int_t j=0; j<bins-1; j++) {

				mean[i][j] = h_pt_resolution_muti[i][j]->GetMean();
				rms[i][j] = h_pt_resolution_muti[i][j]->GetRMS();


				TG_pt_mean[i]->SetPoint(j,(eta_bins[j]+eta_bins[j+1])/2, mean[i][j]);
				TG_pt_rms[i]->SetPoint(j,(eta_bins[j]+eta_bins[j+1])/2, rms[i][j]);
			}
		}

		postProcess();
	}



	void analyzer::init(){

		f_out = TFile::Open( output.c_str() ,"recreate");
		main_directory = gDirectory;

		_PreparePileupReweightingBasedonTruthPileup();
		// \_ This function check if the paths of data/mc files used for the reweighting are given,
		//    and set boolean for reweighting.
		//    It is safe to call this function even in case of no pileup reweighting.


		h_nJet =new TH1F("nJet","nJet",50,0,50);
		h_pt =new TH1F("PT","PT",100,0,1000);
		h_pt_resolution =new TH1F("PTresolution","PTresolution",200,-2,2);

		h_tau21 = new TH1F("tau21","tau21",100,0,1);
		h_tau32 = new TH1F("tau32","tau32",100,0,1);

		h_PrunedMass  = new TH1F("PrunedMass"  ,"PrunedMass",100,0,300);
		h_FilteredMass= new TH1F("FilteredMass","FilteredMass",100,0,300);
		h_SoftDropMass= new TH1F("SoftDropMass","SoftDropMass",100,0,300);
		h_TrimmedMass = new TH1F("TrimmedMass" ,"TrimmedMass",100,0,300);
		const int bins = 6;
		for (Int_t i=0; i<bins; i++) {
			for (Int_t j=0; j<bins-1; j++) {
				sprintf(histname, "PTresolution_pt_%.2f_%.2f_eta_%.2f_%.2f",pt_bins[i],pt_bins[i+1],eta_bins[j]*10,eta_bins[j+1]*10);
				h_pt_resolution_muti[i][j]=new TH1F(histname,histname,200,-2,2);
			}
		}
		for (Int_t i=0; i<bins; i++) {
			sprintf(histname_1, "PTres_mean_pt_%.2f_%.2f",pt_bins[i],pt_bins[i+1]);
			TG_pt_mean[i] = new TGraph(bins-1);
			TG_pt_mean[i]->SetName(histname_1);
			TG_pt_mean[i]->SetTitle(histname_1);
			sprintf(histname_2, "PTres_rms_pt_%.2f_%.2f",pt_bins[i],pt_bins[i+1]);
			TG_pt_rms[i] = new TGraph(bins-1);
			TG_pt_rms[i]->SetName(histname_2);
			TG_pt_rms[i]->SetTitle(histname_2);
		}
	}

	void analyzer::FillHistogram( TH1F * h , double val , double weight ){

		TAxis * axis = h->GetXaxis() ;

		const double max = axis->GetBinUpEdge ( axis->GetNbins() );
		val = val < max ? val : axis->GetBinCenter( axis -> GetNbins() )  ;

		const double min = axis->GetBinLowEdge ( 1 );
		val = val > min ? val : axis->GetBinCenter( 1 );

		h -> Fill( val , weight ) ;

	}

	void analyzer::FillHistogram2D( TH2F * h , double val_x , double val_y,  double weight ){

		{
			TAxis * axis = h->GetXaxis() ;
			const double max = axis->GetBinUpEdge ( axis->GetNbins() );
			val_x = val_x < max ? val_x : axis->GetBinCenter( axis -> GetNbins() )  ;
			const double min = axis->GetBinLowEdge ( 1 );
			val_x = val_x > min ? val_x : axis->GetBinCenter( 1 );
		}
		{
			TAxis * axis = h->GetYaxis() ;
			const double max = axis->GetBinUpEdge ( axis->GetNbins() );
			val_y = val_y < max ? val_y : axis->GetBinCenter( axis -> GetNbins() )  ;
			const double min = axis->GetBinLowEdge ( 1 );
			val_y = val_y > min ? val_y : axis->GetBinCenter( 1 );
		}
		h -> Fill( val_x ,  val_y, weight ) ;
	}

	//void analyzer::GetMeanRMS( TH1F * h , double mean , double rms ){
	//	mean = h->GetMean();
	//	rms = h->GetRMS();
	//}
	//void analyzer::FillTGraph(TGraph * g, double x[100] , double y1[100], double y2[100]){
	//	TG_pt_mean = new TGraph(6,x, y1);
	//	TG_pt_rms = new TGraph(6,x, y2);
	//}


	void analyzer::_PreparePileupReweightingBasedonTruthPileup( ){

		if( PUReweighting_DataFile == ""
				||
				PUReweighting_MCFile == ""
		  ){
			//    std::cout <<"[INFO] PUreweighting files are not set. Pileup reweighting is not applied. [INFO]" << std::endl;
			return ;
		}

		b_PerformPUReweighting = true;

		double N_data [MAX_N_NPTPLOT + 1 ];
		double N_data_total =  0 ;
		{
			TH1D * h ;
			TFile * f_data = TFile ::Open( PUReweighting_DataFile.c_str() );
			f_data -> GetObject ( "pileup" , h );
			for( int i = 0 ; i <= MAX_N_NPTPLOT ; i++ ){
				N_data[ i ]   = h->GetBinContent(i+1) ;
				N_data_total += h->GetBinContent(i+1);
			}
		}

		double N_mc [MAX_N_NPTPLOT + 1 ];
		double N_mc_total = 0 ;
		{
			TH1F * h ;
			TFile * f_mc = TFile ::Open( PUReweighting_MCFile .c_str() );
			f_mc -> GetObject ( "npT" , h );
			for( int i = 0 ; i <= MAX_N_NPTPLOT ; i++ ){
				N_mc[i]     = h->GetBinContent(i+1) ;
				N_mc_total += h->GetBinContent(i+1) ;
			}
		}


		for( int i = 0 ; i <= MAX_N_NPTPLOT ; i++ ){
			if( N_mc[i] == 0 ){
				PUScaleFactor[ i ] = 0 ;
			}else{
				PUScaleFactor[ i ] = N_data[ i ] / N_data_total  / (  N_mc[i] / N_mc_total  ) ;
			}
			std::cout <<"Weight for bin "<< i << " = " << PUScaleFactor[ i ]  << std::endl ;
		}

		std::cout <<"Weight calculated " << std::endl ;

		return ;
	}

	void analyzer::SetPUReweightingFile( std::string data_path , std::string mc_path ){
		PUReweighting_DataFile = data_path ;
		PUReweighting_MCFile = mc_path ;
	}

	void analyzer::postProcess(){
		main_directory -> mkdir("variables");
		main_directory -> mkdir("pTresolution");
		main_directory -> mkdir("TGraph_pTres");
		main_directory -> cd("variables");

		// writing histograms, etc...

		h_nJet -> Write();
		h_pt -> Write();
		h_pt_resolution -> Write();

		h_tau21->Write();
		h_tau32->Write();

		h_PrunedMass  ->Write();
		h_FilteredMass->Write();
		h_SoftDropMass->Write();
		h_TrimmedMass ->Write();
		main_directory -> cd("pTresolution");	
		const int bins = 6;
		for (Int_t i=0; i<bins; i++) {
			for (Int_t j=0; j<bins-1; j++) {
				h_pt_resolution_muti[i][j]->Write();
			}
		}

		main_directory -> cd("TGraph_pTres");  		
		for (Int_t i=0; i<bins; i++) {
			TG_pt_mean[i]->Write();
			TG_pt_rms[i]->Write();
		}

		f_out -> Close();



	}
