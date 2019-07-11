#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TH1D.h"

#include <iostream>
#include <vector>



int ana()
{
	gInterpreter->GenerateDictionary("vector<vector<double>>", "vector");

	TString filename = "data45";
	TFile *file = new TFile(filename+".root");
	TTree *t = (TTree*) file->Get("T");

	TH1D *hAverage = new TH1D("hAverage", "hAverage", 1000, 40, 90);

	TH1D *hTotal = new TH1D("hTotal", "hTotal", 1000, 40, 90);

	TH1D *hSingle = new TH1D("hSingle", "hSingle", 1000, 40, 90);

	std::vector<std::vector<double> > *incidenceAngles = 
		new std::vector<std::vector<double> > (10000, std::vector<double> (10000, 0));

	Int_t nPhot;

	t->SetBranchAddress("incidenceAngles", &incidenceAngles);
	t->SetBranchAddress("nPart", &nPhot);

	Long_t nEv = t->GetEntries();

	for (Long_t j = 0; j < nEv; ++j){
		t->GetEntry(j);


		for (Int_t i_phot = 0; i_phot < nPhot; ++i_phot) {
			


			Double_t averageAngle = 0;
			Int_t nRefl = incidenceAngles->at(i_phot).size();
			for (Int_t k_refl = 0; k_refl < nRefl; ++k_refl){
				averageAngle += incidenceAngles->at(i_phot).at(k_refl);
				hTotal->Fill(incidenceAngles->at(i_phot).at(k_refl));

				if (j == 24 && i_phot == 127) hSingle->Fill(incidenceAngles->at(i_phot).at(k_refl));

			}
			hAverage->Fill(averageAngle / nRefl);
		}
	}


	// hTotal->Scale(1./hTotal->GetEntries());
	// hTotal->Draw();

	// hAverage->Scale(1./hAverage->GetEntries());
	// hAverage->SetLineColor(kRed);
	// hAverage->Draw("Same");


	hSingle->Draw();
	

	

	return 0;
}