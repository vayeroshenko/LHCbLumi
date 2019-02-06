{
	TChain *theChain = new TChain("T");
	theChain->Add("dataNew.root");

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



	TH1D *hTime = new TH1D("Time","Time", 10000, 7, 13);
	TH1D *hMom = new TH1D("Momentum","Momentum", 10000, 0, 10000);

	TH2D *hTimeAngle = new TH2D("Corr","Corr", 	1000, 0, 0.07,
												1000, 7.22, 7.3);

	TH2D *hTimeMom = new TH2D("Corr","Corr", 	1000, 0, 100,
												1000, 7.2, 7.4);

	TH2D *hAngleMom = new TH2D("Corr","Corr", 	1000, 0, 0.07,
												1000, 0, 100);


	Long_t nEv = theChain->GetEntries();

	for (Long_t j = 0; j < nEv; ++j) {
		theChain->GetEntry(j);

		for (Int_t i = 0; i < nPart; ++i){
			hTime->Fill(Time[i]);
			// hMom->Fill(Momentum[i]/1000);
			// hTimeMom->Fill(Momentum[i]/1000,Time[i]);
			// Double_t angle = abs(atan(sqrt(X[i]*X[i]+Y[i]*Y[i])/Z[i]));
			// hAngleMom->Fill(angle,Momentum[i]/1000);
			// std::cout << angle << std::endl;
		}

	}

	hTime->Draw();
	// hTimeAngle->Draw();
	// hMom->Draw();
	// hTimeMom->Draw();
	// hAngleMom->Draw();

}