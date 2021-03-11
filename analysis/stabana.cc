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

Int_t NREBIN = nSec;
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



	// Double_t dice = rnd->Uniform() * 100.;

	// Double_t dice = 43;

	//	if (nPhot == 0) return false;
	//if (nPhot > 99) return true;

	// if (nPhot > 100) return true;
	// else return false;
	    
	
	// if (dice < eff[nPhot-1]) return true;
	// else return false;

	if (nPhot >= 150) return true;
	else return false;

	// delete rnd;

}




void getHist_AND(TString filename, TH1D* h1Hnum, Int_t sec_start, Int_t sec_end)
{

	cout << filename << endl;
	// TChain *theChain1 = new TChain("T");

	TFile *file = new TFile(filename, "READ");
	TTree *theChain1 = (TTree*)file->Get("T");
	// if (!theChain1) { std::
	// theChain1->Add(filename);


	Int_t nPart1;
	Int_t *nPhot = new Int_t[nSec*2];

	Int_t n_hodo = nSec;

	theChain1->SetBranchAddress("nPhot", nPhot);

	// TH1D *h1nColl = new TH1D("nColl1", "nColl1", 10, 0, 10);

	Long_t nEv1 = theChain1->GetEntries();

	std::cout << "AND " << sec_start << " - " << sec_end << std::endl; 
	
	////////// Loop 1 //////////
	for (Long_t j = 0; j <  nEv1; ++j) {
		theChain1->GetEntry(j);


		// Bool_t isChecked[nSec] = {false};
		

		const Int_t nRebin = NREBIN;
		// const Int_t nRebin = 100;

		Double_t numOfHits = 0;			
		for (Int_t j = sec_start; j < sec_end; ++j){
			bool fired = false;
			if (isFired(nPhot[j]) && isFired(nPhot[nSec+j])) {
				fired = true;
				// break;
			}
			if (fired) {
				numOfHits += 1;	
			}
		}

		h1Hnum->Fill( numOfHits );	

	}
	
	file->Close();
	
}

void getHist_OR(TString filename, TH1D* h1Hnum, Int_t sec_start, Int_t sec_end)
{

	cout << filename << endl;
	// TChain *theChain1 = new TChain("T");

	TFile *file = new TFile(filename, "READ");
	TTree *theChain1 = (TTree*)file->Get("T");
	// if (!theChain1) { std::
	// theChain1->Add(filename);


	Int_t nPart1;
	Int_t *nPhot = new Int_t[nSec*2];

	Int_t n_hodo = nSec;

	theChain1->SetBranchAddress("nPhot", nPhot);

	// TH1D *h1nColl = new TH1D("nColl1", "nColl1", 10, 0, 10);

	Long_t nEv1 = theChain1->GetEntries();

	std::cout << "OR " << sec_start << " - " << sec_end << std::endl; 
	
	////////// Loop 1 //////////
	for (Long_t j = 0; j <  nEv1; ++j) {
		theChain1->GetEntry(j);

		// Bool_t isChecked[nSec] = {false};
		
		const Int_t nRebin = NREBIN;
		// const Int_t nRebin = 100;

		Double_t numOfHits = 0;			

		for (Int_t j = sec_start; j < sec_end; ++j){
			bool fired = false;
			if (isFired(nPhot[j]) || isFired(nPhot[nSec+j])) {
				fired = true;
				// break;
			}
			if (fired) {
				numOfHits += 1;	
			}
		}

		h1Hnum->Fill( numOfHits );	

	}
	
	file->Close();
	
}

void getHist_FIRST(TString filename, TH1D* h1Hnum, Int_t sec_start, Int_t sec_end)
{

	cout << filename << endl;
	// TChain *theChain1 = new TChain("T");

	TFile *file = new TFile(filename, "READ");
	TTree *theChain1 = (TTree*)file->Get("T");
	// if (!theChain1) { std::
	// theChain1->Add(filename);


	Int_t nPart1;
	Int_t *nPhot = new Int_t[nSec*2];

	Int_t n_hodo = nSec;

	theChain1->SetBranchAddress("nPhot", nPhot);

	// TH1D *h1nColl = new TH1D("nColl1", "nColl1", 10, 0, 10);

	Long_t nEv1 = theChain1->GetEntries();

	std::cout << "FIRST " << sec_start << " - " << sec_end << std::endl; 
	
	////////// Loop 1 //////////
	for (Long_t j = 0; j <  nEv1; ++j) {
		theChain1->GetEntry(j);


		// Bool_t isChecked[nSec] = {false};
		

		const Int_t nRebin = NREBIN;
		// const Int_t nRebin = 100;



		Double_t numOfHits = 0;			

		for (Int_t j = sec_start; j < sec_end; ++j){
			bool fired = false;
			if ( isFired(nPhot[j]) ) {
				fired = true;
				// break;
			}
			if (fired) {
				numOfHits += 1;	
			}
		}

		h1Hnum->Fill( numOfHits );	

	}
	
	// delete file;
	file->Close();
}





int main(int argc, char** argv){
	TString filename = "file8.root";


	if (argc != 1) NMAX = atoi(argv[1]);
	if (argc != 2) {
		filename = TString(argv[1]);
		NMAX = atoi(argv[2]);
	}
	if (argc != 3){
		filename = TString(argv[1]);
		NMAX = atoi(argv[2]);
		NREBIN = atoi(argv[3]);
	}

	// if (argc != 0) {
	// cout<<" PLUME simulation data analysis SW: "<<endl
	// 	<<"       [1] - input root file name               (default: \"" << filename << "\")   "<<endl
	// 	<<"       [2] - max number of photons in spectra   (default:     500     )   "<<endl
	// 	<<"       [3] - number of sectors in group         (default:      "<< nSec <<"     )"  <<endl
	// 	<<"                                                                          "  <<endl
	// 	<<" Recomended combinations: [2], [1][2], [1][2][3]"<< endl;
	// return 0 ;
	// }


	TFile *out = new TFile("histos.root","RECREATE");
	TH1D *array[15];
	
 

	Int_t configuration[5] = {4,8,4,4,4};
	Int_t i_first = 0;
	Int_t i_end = 0;

	for (Int_t i_ring = 0; i_ring < 5; ++i_ring){
		i_end += configuration[i_ring];
		array[3*i_ring+0] = new TH1D(TString("HitNumber_AND_"+std::to_string(i_ring+1)),
			TString("HitNumber_AND_"+std::to_string(i_ring+1)), nSec, 0, nSec);
		array[3*i_ring+1] = new TH1D(TString("HitNumber_OR_"+std::to_string(i_ring+1)),
			TString("HitNumber_OR_"+std::to_string(i_ring+1)), nSec, 0, nSec);
		array[3*i_ring+2] = new TH1D(TString("HitNumber_FIRST_"+std::to_string(i_ring+1)),
			TString("HitNumber_FIRST_"+std::to_string(i_ring+1)), nSec, 0, nSec);

		getHist_AND(filename, array[3*i_ring+0], i_first, i_end);
		getHist_OR(filename, array[3*i_ring+1], i_first, i_end);
		getHist_FIRST(filename, array[3*i_ring+2], i_first, i_end);

		i_first = i_end;
		array[3*i_ring+0]->SetLineColor(kBlue);
		array[3*i_ring+1]->SetLineColor(kRed);
		array[3*i_ring+2]->SetLineColor(kGreen);
	}


	cout << "AND: \t" << array[0]->GetMean() << " \t+- \t" << array[0]->GetMeanError() << std::endl;
	cout << "OR: \t" << array[1]->GetMean() << " \t+- \t" << array[1]->GetMeanError() << std::endl;
	cout << "First: \t" << array[2]->GetMean() << " \t+- \t" << array[2]->GetMeanError() << std::endl;
	

	out->cd();

	for (auto hist : array){
		hist->Write();
	} 

	// delete htemp;

	
	return 0;
}
