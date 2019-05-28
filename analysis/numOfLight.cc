{
	// TFile *file = new TFile("../../pc_instr/Lumi/results_str/dc_vc_5/results/newDataMerged.root");
	TFile *file = new TFile("fix5/newData_1.root");
	TTree *tree = (TTree*)file->Get("T");

	Int_t StationID[100000];
	Int_t nPart;

	tree->SetBranchAddress("StationID", StationID);
	tree->SetBranchAddress("nPart", &nPart);

	Long_t nEn = tree->GetEntries();

	std::vector<Int_t> N (100,0);


	std::vector<TH1D> h(100, TH1D("nPhot", "nPhot", 1000, 0, 1000));
	
	TH1D* h1 = new TH1D("nPhot", "nPhot", 1000, 0, 1000);


	for (Long_t i = 0; i < nEn; ++i) {
		tree->GetEntry(i);

		for (Int_t j = 0; j < nPart; ++j){
			N[StationID[j]-1] = N[StationID[j]-1] + 1;
		}

		// if (N[32] != 0) h1->Fill(N[32]);

		for (Int_t j = 0; j < 100; ++j){
			if (N[j] != 0) h[j].Fill(N[j]);
			// h[j]->Fill(N[j]);
		}
	}

	for (Int_t j = 1; j <= 1000; ++j) {
		Int_t binContent = 0;
		for (Int_t i = 0; i < 100; ++i)
			binContent += h[i].GetBinContent(j);
		h1->SetBinContent(j, binContent);
	}

	// h[5].Draw();
	// h[74].SetLineColor(kRed);
	// h[74].Draw("Same")
	h1->Draw();
}
