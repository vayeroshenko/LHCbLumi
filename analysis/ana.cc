{
	TChain *theChain = new TChain("T");
	theChain->Add("data.root");
	TCanvas *c = new TCanvas("plots","plots", 1500,800);

	c->Divide(3,1);


	Int_t EventID;
	Int_t nPart;
	Int_t TrackID[100000];
	Int_t ParentID[100000];
	Int_t StationID[100000];
	Double_t X[100000];
	Double_t Y[100000];
	Double_t Z[100000];
	Double_t Momentum[100000];
	Double_t Px[100000];
	Double_t Py[100000];
	Double_t Pz[100000];
	Double_t Time[100000];
	Double_t PdgID[100000];

	theChain->SetBranchAddress("EventID", &EventID);
	theChain->SetBranchAddress("nPart", &nPart);
	theChain->SetBranchAddress("TrackID", TrackID);
	theChain->SetBranchAddress("ParentID", ParentID);
	theChain->SetBranchAddress("StationID", StationID);
	theChain->SetBranchAddress("X", X);
	theChain->SetBranchAddress("Y", Y);
	theChain->SetBranchAddress("Z", Z);
	theChain->SetBranchAddress("Momentum", Momentum);
	theChain->SetBranchAddress("Px", Px);
	theChain->SetBranchAddress("Py", Py);
	theChain->SetBranchAddress("Pz", Pz);
	theChain->SetBranchAddress("Time", Time);
	theChain->SetBranchAddress("PdgID", PdgID);

	nBinX = 20; nBinY = 20;
	// xMin = -500; xMax = 500;
	// yMin = -500; yMax = 500;
	xMin = -5; xMax = 5;
	yMin = -5; yMax = 5;

	TH2D StationMap[2];
	StationMap[0] =  TH2D("Farthest", "Farthest", 
		nBinX, xMin, xMax,
		nBinY, yMin, yMax);
	StationMap[1] =  TH2D("Nearest", "Nearest", 
		nBinX, xMin, xMax,
		nBinY, yMin, yMax);


	Long_t nEv = theChain->GetEntries();

	for (Long_t j = 0; j < nEv; ++j) {
		theChain->GetEntry(j);

		for (Int_t i = 0; i < nPart; ++i){
			// if (X[i] < 8. && X[i] > -8 && Y[i] < 8. && Y[i] > -8) continue;
			if (StationID[i] ==  0) StationMap[0].Fill(X[i],Y[i]);
			if (StationID[i] ==  1) StationMap[1].Fill(X[i],Y[i]);
		}
	}


	// for (Int_t xI = 0; xI < nBinX; ++xI){
	// 	for (Int_t yI = 0; yI < nBinY; ++yI){
	// 		for (Int_t i = 0; i < 2; ++i){
	// 			Double_t BinVal = StationMap[i].GetBinContent(xI,yI) / nEv;
	// 			BinVal = round(BinVal * 10.)/10.;
	// 			StationMap[i].SetBinContent(xI, yI, BinVal);
	// 		}
	// 	}
	// } 

	c->cd(1);
	theChain->Draw("Z");

	for (Int_t i = 0; i < 2; ++i){
		StationMap[i].SetOption("COL TEXT");
		c->cd(i+2);
		StationMap[i].Draw();
	}




}