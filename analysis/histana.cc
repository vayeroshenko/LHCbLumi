{

	#define NHIST 20
	#define NBINS 30
	#define MAXCHI2 25

	std::cout << "start \n";
	TFile *file1 = new TFile("histos-5-25.root");
	TH1D* array1 = new TH1D[NHIST];

	TFile *file2 = new TFile("histos-5-25.root");
	TH1D* array2 = new TH1D[NHIST];

	// TH1D *hist = (TH1D*)file->Get("Hit Number1");
	// hist->Draw();

	for (Int_t i = 1; i <= NHIST; ++i){
		array1[i-1] = *(TH1D*)file1->Get( TString("Hit Number") + TString(std::to_string(i-1)));
		array2[i-1] = *(TH1D*)file2->Get( TString("Hit Number") + TString(std::to_string(i-1)));
	}

	TH1D* average = new TH1D("average","average", NBINS, 0 - 0.5, NBINS - 0.5);


	for (Int_t binI = 1; binI <= NBINS; ++binI) {
		Double_t av = 0.;
		for (Int_t histJ = 0; histJ < NHIST; ++histJ)
		{
			av += array1[histJ].GetBinContent(binI);
		}
		av /= NHIST;
		average->SetBinContent(binI, av);
	}

	//average->Draw();

	TH1D *hChi2 = new TH1D("chi2","chi2", 30, 0, 30);


	
	for (Int_t histJ = 0; histJ < NHIST; ++histJ){
	  Int_t ndf = 0;
	  Double_t chi2 = 0;

		for (Int_t binK = 1; binK <= NBINS; ++binK){
			Double_t binContentI = average->GetBinContent(binK);
			Double_t binErrorI = average->GetBinError(binK);
			Double_t binContentJ = array2[histJ].GetBinContent(binK);
			Double_t binErrorJ = array2[histJ].GetBinError(binK);

			if (binContentI == 0 && binContentJ == 0) continue;
			//if (binErrorI * binErrorJ == 0) continue;

			chi2 += (binContentI - binContentJ)*(binContentI - binContentJ) / 
			(binErrorI*binErrorI + binErrorJ*binErrorJ);

			++ndf;
		}
		hChi2->Fill(chi2);

	} 

	hChi2->Draw();


}
