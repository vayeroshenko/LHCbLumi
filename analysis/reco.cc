#include "TH1D.h"
#include "TChain.h"
#include <iostream>
#include "TApplication.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include <vector>

#define NHIST 20
#define NBINS 30

#define TOLERANCE 1e-8
#define MAX_ITER 50

Double_t solve(Int_t, Double_t, Double_t);

int main(int argc, char *argv[])
{
	TFile *file = new TFile("histos.root");
	TH1D* array = new TH1D[NHIST];

	for (Int_t i = 1; i <= NHIST; ++i){
		array[i-1] = *(TH1D*)file->Get( TString("Hit Number") + TString(std::to_string(i-1)));
	}


	TH1D *reco = new TH1D("reco", "reco", 100, 0, 5.);
	TH1D *reco0 = new TH1D("reco0", "reco0", 100, 0, 5.);

	for (Int_t histI = 0; histI < NHIST; ++histI)
	{
		Int_t nEntries = array[histI].GetEntries();
		Double_t p0 = array[histI].GetBinContent(1) / nEntries;
		reco0->Fill(-TMath::Log(p0));

		for (Int_t binJ = 1; binJ < NBINS; ++binJ)
		{
			Double_t binContent = array[histI].GetBinContent(binJ + 1);
			if (binContent == 0) continue;

			reco->Fill(solve(binJ, binContent/nEntries, p0));
		}
		
	}

	reco->SetLineColor(kBlue);
	reco0->SetLineColor(kRed);


	TApplication *app = new TApplication("name", &argc, argv);
	TCanvas *c = new TCanvas();

	reco->Draw();
	reco0->Draw("SAME");

	c->Show();
	app->Run();
	
	return 0;
}

Double_t solve(Int_t n, Double_t p, Double_t p0)
{
	Double_t coef[50] = {1, 1.41421, 1.81712, 2.21336, 
		2.60517, 2.9938, 3.38002, 3.76435, 4.14717, 4.52873, 
		4.90924, 5.28885, 5.66769, 6.04586, 6.42342, 6.80047, 
		7.17704, 7.55318, 7.92895, 8.30436, 8.67946, 9.05426, 
		9.4288, 9.80308, 10.1771, 10.551, 10.9246, 11.2981, 
		11.6714, 12.0445, 12.4175, 12.7903, 13.163, 13.5355, 
		13.908, 14.2803, 14.6525, 15.0246, 15.3966, 15.7685, 
		16.1403, 16.5121, 16.8837, 17.2553, 17.6268, 17.9982, 
		18.3696, 18.7409, 19.1121, 19.4833};


    Double_t x = - TMath::Log(p0); 
    Double_t x_old;

    int i = 0;      
    do {
        x_old = x;
        x = coef[n - 1] * TMath::Power(p * TMath::Exp(-x), 1./n); 
        i++;
    } while (fabs(x - x_old) / x > TOLERANCE && i < MAX_ITER);

    return x;

}