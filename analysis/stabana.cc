#include "TH1D.h"
#include "TChain.h"
#include <iostream>
#include "TApplication.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include <vector>

#define nSec 110

// TH1D* getHist(TString);


using namespace std;

// TH1D getHist(TString);

#define NHIST 20


void getHist(TString filename, TH1D* h1Hnum)
{
	cout << filename << endl;
	// TChain *theChain1 = new TChain("T");

	TFile *file = new TFile(filename);
	TTree *theChain1 = (TTree*)file->Get("T");
	// if (!theChain1) { std::
	// theChain1->Add(filename);

	Int_t EventID1;
	Int_t nPart1;
	Int_t nColl1;
	Int_t *TrackID1 = new Int_t[100000];
	Int_t *ParentID1 = new Int_t[100000];
	Int_t *StationID1 = new Int_t[100000];
	Double_t *X1 = new Double_t[100000];
	Double_t *Y1 = new Double_t[100000];
	Double_t *Z1 = new Double_t[100000];
	Double_t *Momentum1 = new Double_t[100000];
	Double_t *Px1 = new Double_t[100000];
	Double_t *Py1 = new Double_t[100000];
	Double_t *Pz1 = new Double_t[100000];
	Double_t *Time1 = new Double_t[100000];
	Double_t *PdgID1 = new Double_t[100000];

	theChain1->SetBranchAddress("EventID", &EventID1);
	theChain1->SetBranchAddress("nPart", &nPart1);
	theChain1->SetBranchAddress("nColl", &nColl1);
	theChain1->SetBranchAddress("TrackID", TrackID1);
	theChain1->SetBranchAddress("ParentID", ParentID1);
	theChain1->SetBranchAddress("StationID", StationID1);
	theChain1->SetBranchAddress("X", X1);
	theChain1->SetBranchAddress("Y", Y1);
	theChain1->SetBranchAddress("Z", Z1);
	theChain1->SetBranchAddress("Momentum", Momentum1);
	theChain1->SetBranchAddress("Px", Px1);
	theChain1->SetBranchAddress("Py", Py1);
	theChain1->SetBranchAddress("Pz", Pz1);
	theChain1->SetBranchAddress("Time", Time1);
	theChain1->SetBranchAddress("PdgID", PdgID1);



	// TH1D *h1nColl = new TH1D("nColl1", "nColl1", 10, 0, 10);

	Long_t nEv1 = theChain1->GetEntries();

	
	////////// Loop 1 //////////
	for (Long_t j = 0; j <  nEv1; ++j) {
		theChain1->GetEntry(j);
		// h1nColl->Fill(nColl1);
		// 
		Int_t nPhot[nSec] = {0};
		Bool_t isChecked[nSec] = {false};

		// 
		for (Int_t i = 0; i < nPart1; ++i){
			// (StationID1[i] < 0) continue;
			if (isChecked[StationID1[i]]) continue;

			nPhot[StationID1[i]] ++;
			if (nPhot[StationID1[i]] > 20.){
				// h1Hnum->Fill(StationID[i]);
				isChecked[StationID1[i]] = true;
			}
		}
		
		
		// 
		Int_t numOfHits = 0;
		for (int i = 0; i < nSec; ++i)
			if (isChecked[i]) numOfHits++;
		
		h1Hnum->Fill(numOfHits);
	}

	
	file->Close();

	delete TrackID1, ParentID1, StationID1, X1, Y1, Z1, Momentum1, Px1, Py1, Pz1, Time1, PdgID1; 
	// h1Hnum->Draw("E1");
	
}


int main(int argc, char** argv){


	TH1D *array[NHIST];

	TH1D *hChi2 = new TH1D ("chi2", "chi2", 1000, 0, 100);

	
	TFile *out = new TFile("histos.root","RECREATE");

	// printHello();
	// getHist(filename);


	for (Int_t i = 0; i < NHIST; ++i) {
		TString filename = "newData_";
		filename += TString (std::to_string(i+1));
		filename += TString(".root");

		array[i] = new TH1D(TString("Hit Number"+std::to_string(i)),TString("HitNumber"+std::to_string(i)), nSec, -0.5, nSec-0.5);

		
		
		getHist(filename, array[i]);

		out->cd();
		array[i]->Write();

		
		array[i]->SetLineColor(i);
		// delete htemp;
	}

	
	TApplication *app = new TApplication("name", &argc, argv);
	
	TCanvas *c = new TCanvas();

	


	Double_t binContent[NHIST];
	Double_t binError[NHIST];
	Int_t maxI;
	Int_t maxJ;

	Double_t maxChi2 = 0;

	const Int_t nBins = 20;

	Double_t average[nBins] = {0};
	Double_t rms[nBins] = {0};

	for (Int_t binK = 0; binK < nBins; ++binK){
		for (Int_t histI = 0; histI < NHIST; ++histI){
			average[binK] += array[histI]->GetBinContent(binK+1);
		}
		average[binK] /= NHIST;
		if (average[binK] == 0) continue;

		for (Int_t histI = 0; histI < NHIST; ++histI){
			rms[binK] += 
			(array[histI]->GetBinContent(binK+1) - average[binK])*
			(array[histI]->GetBinContent(binK+1) - average[binK]);
		}


		rms[binK] = sqrt(rms[binK]/(NHIST)/(NHIST-1));
		cout << "average: " <<  average[binK] << " \t rms: " << rms[binK]
		<< "\t sqrt: "<< sqrt(average[binK]) << endl;
		maxChi2 += 	(rms[binK] / sqrt(average[binK]))*
					(rms[binK] / sqrt(average[binK]));
	} 



	c->Show();
	app->Run();
	// std::cin;

	delete[] array;
	delete c;

	return 0;
}
