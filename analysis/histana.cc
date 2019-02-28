{

	#define NHIST 10
	#define NBINS 30
	#define MAXCHI2 25

	std::cout << "start \n";
	TFile *file = new TFile("histos.root");
	TH1D* array = new TH1D[NHIST];

	// TH1D *hist = (TH1D*)file->Get("Hit Number1");
	// hist->Draw();

	for (Int_t i = 1; i <= NHIST; ++i){
		array[i-1] = *(TH1D*)file->Get( TString("Hit Number") + TString(std::to_string(i-1)));
	}

	TH1D* average = new TH1D("average","average", NBINS, 0 - 0.5, NBINS - 0.5);


	for (Int_t binI = 1; binI <= NBINS; ++binI) {
		Double_t av = 0.;
		for (Int_t histJ = 0; histJ < NHIST; ++histJ)
		{
			av += array[histJ].GetBinContent(binI);
		}
		av /= NHIST;
		average->SetBinContent(binI, av);
	}

	average->Draw();

	TH1D *hChi2 = new TH1D("chi2","chi2", 100, 0, MAXCHI2);


	for (Int_t histJ = 0; histJ < NHIST; ++histJ){
		Double_t chi2 = 0;

		for (Int_t binK = 1; binK <= NBINS; ++binK){
			Double_t binContentI = average->GetBinContent(binK);
			Double_t binErrorI = average->GetBinError(binK);
			Double_t binContentJ = array[histJ].GetBinContent(binK);
			Double_t binErrorJ = array[histJ].GetBinError(binK);

			if (binContentI == 0 || binContentJ == 0) continue;
			if (binErrorI * binErrorJ == 0) continue;

			chi2 += (binContentI - binContentJ)*(binContentI - binContentJ) / 
			(binErrorI*binErrorI + binErrorJ*binErrorI);

		}
		hChi2->Fill(chi2);

	} 

	hChi2->Draw();


}