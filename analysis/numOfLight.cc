{
	TFile *file = new TFile("../../pc_instr/Lumi/results_str/dc_vc_5/results/newDataMerged.root");
	TTree *tree = (TTree*)file->Get("T");

	Int_t StationID[100000];
	Int_t nPart;

	tree->SetBranchAddress("StationID", StationID);
	tree->SetBranchAddress("nPart", &nPart);

	Long_t nEn = tree->GetEntries();

	Int_t N = 0;


	TH1D *h = new TH1D("nPhot", "nPhot", 1000, 0, 1000);

	for (Long_t i = 0; i < nEn; ++i) {
		tree->GetEntry(i);

		for (Int_t j = 0; j < nPart; ++j){
			if (StationID[j] == 1) ++N;
		}

		if (N != 0) h->Fill(N);
	}

	h->Draw();
}
