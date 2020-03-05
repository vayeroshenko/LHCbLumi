#include "TH1D.h"
#include "TH2D.h"
#include "TChain.h"
#include <iostream>
#include "TApplication.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom3.h"
#include <vector>

#define nSec 24

// TH1D* getHist(TString);


using namespace std;

// TH1D getHist(TString);

#define NHIST 1

TRandom3 *rnd = new TRandom3(0);

Int_t NMAX = 500;



Double_t eff[100] = {0.06, 0.15, 0.3, 0.4, 0.6, 0.6, 0.9, 1.2, 1.4, \
	1.8, 1.9, 2.2, 2.6, 2.9, 3.4, 3.575, 3.95, 4.5, 4.825, 5.7, 5.9, 6.35, \
	7, 7.4, 8.1, 8.4, 9.05, 10, 10.9, 11.5, 12.1, 12.75, 14, 15, 16.4, 17, \
	18.3, 20.2, 22, 24, 25.6, 26.875, 29.75, 33, 35.5, 37.2, 39.175, 42.85, \
	46, 50, 52, 53.625, 57.25, 60, 64.3, 66.2, 67.625, 70.75, 73.8, 75.6, 76.5, \
	78.35, 80.1, 82, 83.6, 84.2, 84.9, 86.3, 87.3, 88.5, 89.1, 89.35, 90.2, 91, \
	91.9, 92.3, 92.5, 93.1, 94, 94.2, 94.6, 94.875, 95.45, 96, 96.6, 96.8, 97, \
	97.4, 97.7, 98.2, 98.35, 98.5, 98.8, 99.1, 99.4, 99.5, 99.55, 99.7, 99.9, 100 };

Bool_t isFired(Int_t nPhot){

	// Double_t eff[20] ={	0.6, 1.8, 3.4, 5.7, 8.1, 11.5, 16.4, 24., \
	// 					35.5, 50, 64.5, 76, 83.6, 88.5, 91.9, 94.3, \
	// 					96.6, 98.2, 99.4, 100};

	// Double_t eff[40] ={	0.3, 0.6, 1., 1.8, 2.4, 3.4, 4.1, 5.7, 6.5, 8.1, 11.5, 13., \
	// 					16.4, 19.,  24., 30.3, 35.5, 43.5, 50, 56.5, 64.5, 69.7, 76, \
	// 					81., 83.6, 87., 88.5, 91., 91.9, 93.5, 94.3, 95.9, 96.6, 97.6, \
	// 					98.2, 99., 99.4, 99.9, 100};



	Double_t dice = rnd->Uniform() * 100.;

	// Double_t dice = 43;

	//	if (nPhot == 0) return false;
	//if (nPhot > 99) return true;

	// if (nPhot > 100) return true;
	// else return false;
	    
	
	// if (dice < eff[nPhot-1]) return true;
	// else return false;

	if (nPhot >= 35) return true;
	else return false;

	// delete rnd;

}


void getHist(TString filename, Int_t flag, TH1D* htot, TH1D* hlayer1, TH1D* hlayer2, TH2D* hcorr)
{

	cout << filename << endl;
	// TChain *theChain1 = new TChain("T");

	TFile *file = new TFile(filename);
	TTree *theChain1 = (TTree*)file->Get("T");
	// if (!theChain1) { std::
	// theChain1->Add(filename);



	// Int_t nPart1;
	Int_t *nPhot = new Int_t[nSec*2];


	theChain1->SetBranchAddress("nPhot", nPhot);

	// TH1D *h1nColl = new TH1D("nColl1", "nColl1", 10, 0, 10);

	Long_t nEv1 = theChain1->GetEntries();

	
	////////// Loop 1 //////////
	for (Long_t j = 0; j <  nEv1; ++j) {
		theChain1->GetEntry(j);


		for (Int_t i = 0; i < nSec; ++i){
			if (flag == 0) {
				htot->Fill(nPhot[i]);
				htot->Fill(nPhot[i+nSec]);
				hlayer1->Fill(nPhot[i]);
				hlayer2->Fill(nPhot[i+nSec]);
				hcorr->Fill(nPhot[i], nPhot[i+nSec]);
			}
			if (flag == 1) {
				if (isFired(nPhot[i]) && isFired(nPhot[i+nSec])) {
					htot->Fill(nPhot[i]);
					htot->Fill(nPhot[i+nSec]);
					hlayer1->Fill(nPhot[i]);
					hlayer2->Fill(nPhot[i+nSec]);
					hcorr->Fill(nPhot[i], nPhot[i+nSec]);
				}
			}
		}

	}
	
	file->Close();
	
}


int main(int argc, char** argv){
	TString filename = "file8.root";

	if (argc != 1) NMAX = atoi(argv[1]);

	if (argc != 2) {
		NMAX = atoi(argv[2]);
		filename = TString(argv[1]);
	}

	TH1D *array[6];
	TH2D *array2[2];
	
	TFile *out = new TFile("spectra.root","RECREATE");


	array[0]  = new TH1D(TString("total_raw"),TString("total_raw"), NMAX, 1, NMAX+1);
	array[1]  = new TH1D(TString("layer1_raw"),TString("layer1_raw"), NMAX, 1, NMAX+1);
	array[2]  = new TH1D(TString("layer2_raw"),TString("layer2_raw"), NMAX, 1, NMAX+1);
	array2[0] = new TH2D(TString("layers_corr_raw"),TString("layers_corr_raw"), NMAX, 1, NMAX+1,
																				NMAX, 1, NMAX+1);
 

	array[3] = new TH1D(TString("total_AND_th35"),TString("total_AND_th35"), NMAX, 1, NMAX+1);
	array[4] = new TH1D(TString("layer1_AND_th35"),TString("total_AND_th35"), NMAX, 1, NMAX+1);
	array[5] = new TH1D(TString("layer2_AND_th35"),TString("total_AND_th35"), NMAX, 1, NMAX+1);
	array2[1] = new TH2D(TString("layers_corr_AND_th35"),TString("layers_corr_AND_th35"), 	NMAX, 1, NMAX+1,
																							NMAX, 1, NMAX+1);



	getHist(filename, 0, array[0], array[1], array[2], array2[0]);
	getHist(filename, 1, array[3], array[4], array[5], array2[1]);

	array[0]->SetLineColor(kGreen);
	array[1]->SetLineColor(kRed);
	array[2]->SetLineColor(kBlue);


	array[3]->SetLineColor(kGreen);
	array[4]->SetLineColor(kRed);
	array[5]->SetLineColor(kBlue);

	// array[2]->SetLineColor(kGreen);


	// cout << "AND: \t" << array[0]->GetMean() << " \t+- \t" << array[0]->GetMeanError() << std::endl;
	// cout << "OR: \t" << array[1]->GetMean() << " \t+- \t" << array[1]->GetMeanError() << std::endl;
	// cout << "First: \t" << array[2]->GetMean() << " \t+- \t" << array[2]->GetMeanError() << std::endl;
	

	out->cd();
	array[0]->Write();
	array[1]->Write();
	array[2]->Write();
	array2[0]->Write();

	array[3]->Write();
	array[4]->Write();
	array[5]->Write();
	array2[1]->Write();

	out->Close();

	// delete htemp;

	
	return 0;
}
