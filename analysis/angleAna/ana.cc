#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TH1D.h"

#include <iostream>
#include <vector>



int ana()
{
	gInterpreter->GenerateDictionary("vector<vector<double>>", "vector");

	TString filename = "data2";
	TFile *file = new TFile(filename+".root");
	TTree *t = (TTree*) file->Get("T");

	TH1D *hAverage = new TH1D("hAverage", "hAverage", 1000, 40, 90);

	TH1D *hTotal_tir = new TH1D("hTotal_tir", "hTotal_tir", 100, 0, 90);
	TH1D *hTotal_fr = new TH1D("hTotal_fr", "hTotal_fr", 100, 0, 90);
	TH1D *hTotal_refr = new TH1D("hTotal_refr", "hTotal_refr", 100, 0, 90);

	TH1D *hTotal = new TH1D("hTotal", "hTotal", 100, 0, 90);

	TH1D *hSingle = new TH1D("hSingle", "hSingle", 1000, 40, 90);

	std::vector<std::vector<double> > *angle_tir = 
		new std::vector<std::vector<double> > (10000, std::vector<double> (10000, 0));
	std::vector<std::vector<double> > *angle_fr = 
		new std::vector<std::vector<double> > (10000, std::vector<double> (10000, 0));
	std::vector<std::vector<double> > *angle_refr = 
		new std::vector<std::vector<double> > (10000, std::vector<double> (10000, 0));

	Int_t nPhot;

	t->SetBranchAddress("angle_tir", &angle_tir);
	t->SetBranchAddress("angle_fr", &angle_fr);
	t->SetBranchAddress("angle_refr", &angle_refr);
	t->SetBranchAddress("nPart", &nPhot);

	Long_t nEv = t->GetEntries();

	Int_t n_tir = 0;
	Int_t n_fr = 0;
	Int_t n_refr = 0;

	for (Long_t j = 0; j < nEv; ++j){
		t->GetEntry(j);


		for (Int_t i_phot = 0; i_phot < nPhot; ++i_phot) {
			Double_t averageAngle = 0;
			// Int_t nRefl = incidenceAngles->at(i_phot).size();

			for (auto k : angle_tir->at(i_phot)){
				hTotal_tir->Fill(k);
				hTotal->Fill(k);
				n_tir++;
			}
			for (auto k : angle_fr->at(i_phot)){
				hTotal_fr->Fill(k);
				hTotal->Fill(k);
				n_fr++;
			}
			for (auto k : angle_refr->at(i_phot)){
				hTotal_refr->Fill(k);
				hTotal->Fill(k);
				n_refr++;
			}
				// if (j == 24 && i_phot == 127) hSingle->Fill(incidenceAngles->at(i_phot).at(k_refl));
			// hAverage->Fill(averageAngle / nRefl);
		}
	}


	// hTotal->Scale(1./hTotal->GetEntries());
	// hTotal->Draw();

	// hAverage->Scale(1./hAverage->GetEntries());
	// hAverage->SetLineColor(kRed);
	// hAverage->Draw("Same");


	// hSingle->Draw();

	// for (Int_t i = 1; i < 101; ++i){
	// 	hTotal_tir->SetBinContent(i, hTotal_tir->GetBinContent(i) / hTotal->GetBinContent(i));
	// 	hTotal_fr->SetBinContent(i, hTotal_fr->GetBinContent(i) / hTotal->GetBinContent(i));
	// 	hTotal_refr->SetBinContent(i, hTotal_refr->GetBinContent(i) / hTotal->GetBinContent(i));
	// }

	hTotal->SetLineColor(kBlack);
	hTotal->SetLineWidth(2);
	
	hTotal_refr->SetLineColor(kBlue);
	hTotal_fr->SetLineColor(kGreen);
	hTotal_tir->SetLineColor(kRed);

	hTotal->Draw();

	hTotal_tir->Draw("same");
	hTotal_fr->Draw("same");
	hTotal_refr->Draw("same");
	

	TLegend *l = new TLegend();
	l->AddEntry(hTotal_tir,"Total internal reflection", "l");
	l->AddEntry(hTotal_fr,"Fresnel reflection", "l");
	l->AddEntry(hTotal_refr,"Fresnel refraction", "l");
	l->AddEntry(hTotal,"Total", "l");
	l->Draw();


	return 0;
}