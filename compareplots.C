#include <string>
#include <vector>
#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
//#include "CMSLabels.h"
#include "TVectorD.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TLatex.h"
//#include "CMS_lumi.C"


void compareplots(){
    
    const char TGname_1[10];
    const char TGname_2[10];
   // const int bins = 6;
    const int bins = 5;
  //  const double pt_bins[7] = {0,30,50,75,100,200,250};
    const double pt_bins[6] = {0,1.3,2,2.5,3,5};
    std::string color[4] = {"kRed","kGreen","kBlue","kYellow"};
    TFile* f1[4];
    TFile* f2[4];
    TFile* f1[0] = TFile::Open("output/QCD_AK8PFPUPPI.root");
    TFile* f1[1] = TFile::Open("output/QCD_AK8PF.root");
    TFile* f1[2] = TFile::Open("output/QCD_AK8PFCHS.root");
    TFile* f1[3] = TFile::Open("output/QCD_AK8PFSK.root");
    TFile* f2[0] = TFile::Open("output80x/QCD_AK8PFPUPPI.root");
    TFile* f2[1] = TFile::Open("output80x/QCD_AK8PF.root");
    TFile* f2[2] = TFile::Open("output80x/QCD_AK8PFCHS.root");
    TFile* f2[3] = TFile::Open("output80x/QCD_AK8PFSK.root");
    
    TGraphErrors* h_pf_1[10][10];
    TGraphErrors* h_pf_2[10][10];
    
    TCanvas *c1[10];
    
    TLegend *leg[10];
    
    
    for (int j=0; j<bins; j++) {
        c1[j] = new TCanvas("c1","c1",600,600);
        c1[j]->cd();
      //  sprintf(TGname_1, "TGraph_pTres_pt/PTres_rms_eta_%.2f_%.2f",pt_bins[j],pt_bins[j+1]);
        sprintf(TGname_1, "TGraph_pTres_pt/PTres_mean_eta_%.2f_%.2f",pt_bins[j],pt_bins[j+1]);
      //  sprintf(TGname_1, "TGraph_pTres_eta/PTres_rms_pt_%.2f_%.2f",pt_bins[j],pt_bins[j+1]);
     //   sprintf(TGname_1, "TGraph_pTres_eta/PTres_mean_pt_%.2f_%.2f",pt_bins[j],pt_bins[j+1]);
        for (int i=0; i<4; i++) {
            
            h_pf_1[j][i] = (TGraphErrors *)f1[i]->Get(TGname_1);
            h_pf_2[j][i] = (TGraphErrors *)f2[i]->Get(TGname_1);
            
            
            h_pf_1[j][i]->SetMarkerColor(i+1);
            h_pf_1[j][i]->SetMarkerStyle(24);
            h_pf_1[j][i]->SetMarkerSize(1);
            h_pf_1[j][i]->SetLineColor(i+1);
            h_pf_1[j][i]->GetYaxis()->SetRangeUser(-1, 3);
            
            h_pf_2[j][i]->SetMarkerColor(i+1);
            h_pf_2[j][i]->SetMarkerStyle(25);
            h_pf_2[j][i]->SetMarkerSize(1);
            h_pf_2[j][i]->SetLineColor(i+1);
            h_pf_2[j][i]->GetYaxis()->SetRangeUser(-1, 3);
            
        }
        
        h_pf_1[j][0]->Draw("APE");
        h_pf_2[j][0]->Draw("samePE");
        
        for (int i=0; i<4; i++) {
            h_pf_1[j][i]->Draw("samePE");
            h_pf_2[j][i]->Draw("samePE");
        }
        
        leg[j]= new TLegend(0.65,0.7,0.88,0.9,NULL,"brNDC");
        leg[j]->SetFillColor(10);
        leg[j]->SetTextSize(0.025);
        leg[j]->AddEntry(h_pf_1[0][0],"76X PFPUPPI","LP");
        leg[j]->AddEntry(h_pf_1[0][1],"76X PF","LP");
        leg[j]->AddEntry(h_pf_1[0][2],"76X PFCHS","LP");
        leg[j]->AddEntry(h_pf_1[0][3],"76X PFSK","LP");
        leg[j]->AddEntry(h_pf_2[0][0],"80X PFPUPPI","LP");
        leg[j]->AddEntry(h_pf_2[0][1],"80X PF","LP");
        leg[j]->AddEntry(h_pf_2[0][2],"80X PFCHS","LP");
        leg[j]->AddEntry(h_pf_2[0][3],"80X PFSK","LP");
        leg[j]->Draw("same");
       // c1[j]->SaveAs(Form("PTres_rms_pt_%.2f_%.2f.png",pt_bins[j],pt_bins[j+1]));
       // c1[j]->SaveAs(Form("PTres_rms_eta_%.2f_%.2f.png",pt_bins[j],pt_bins[j+1]));
       // c1[j]->SaveAs(Form("PTres_mean_pt_%.2f_%.2f.png",pt_bins[j],pt_bins[j+1]));
        c1[j]->SaveAs(Form("PTres_mean_eta_%.2f_%.2f.png",pt_bins[j],pt_bins[j+1]));
        
    }
    
}