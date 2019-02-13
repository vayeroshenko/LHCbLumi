{
	TChain *theChain = new TChain("T");
	theChain->Add("dataNew.root");

	TCanvas *c = new TCanvas();
	c->Divide(4,2);

	Int_t EventID;
	Int_t nPart;
	Int_t nColl;
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
	theChain->SetBranchAddress("nColl", &nColl);
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


	#define nSec 110


	// TH1D *hHnum = new TH1D("Hit Number","HitNumber", nSec, -0.5, nSec-0.5);

	TH1D *hHnum1 = new TH1D("Hit Number","HitNumber", 10, -0.5, 10-0.5);
	TH1D *hHnum2 = new TH1D("Hit Number","HitNumber", 10, -0.5, 10-0.5);
	TH1D *hHnum3 = new TH1D("Hit Number","HitNumber", 10, -0.5, 10-0.5);
	TH1D *hHnum4 = new TH1D("Hit Number","HitNumber", 10, -0.5, 10-0.5);
	TH1D *hHnum5 = new TH1D("Hit Number","HitNumber", 10, -0.5, 10-0.5);
	TH1D *hHnum6 = new TH1D("Hit Number","HitNumber", 10, -0.5, 10-0.5);
	TH1D *hHnum7 = new TH1D("Hit Number","HitNumber", 10, -0.5, 10-0.5);
	TH1D *hHnum8 = new TH1D("Hit Number","HitNumber", 10, -0.5, 10-0.5);


	Long_t nEv = theChain->GetEntries();

	for (Long_t j = 0; j < nEv; ++j) {
		theChain->GetEntry(j);

		// if (nColl != 2) continue;

		Int_t nPhot[nSec] = {0};
		Bool_t isChecked[nSec] = {false};

		for (Int_t i = 0; i < nPart; ++i){
			if (StationID[i] < 0) continue;
			if (isChecked[StationID[i]]) continue;

			nPhot[StationID[i]] ++;
			if (nPhot[StationID[i]] > 20.){
				// hHnum->Fill(StationID[i]);
				isChecked[StationID[i]] = true;
			}
		}

		Int_t numOfHits = 0;
		for (int i = 0; i < nSec; ++i)
			if (isChecked[i]) numOfHits++;
				// hHnum->Fill(numOfHits);


		if (nColl == 1) hHnum1->Fill(numOfHits);
		else if (nColl == 1) hHnum1->Fill(numOfHits);
		else if (nColl == 2) hHnum2->Fill(numOfHits);
		else if (nColl == 3) hHnum3->Fill(numOfHits);
		else if (nColl == 4) hHnum4->Fill(numOfHits);
		else if (nColl == 5) hHnum5->Fill(numOfHits);
		else if (nColl == 6) hHnum6->Fill(numOfHits);
		else if (nColl == 7) hHnum7->Fill(numOfHits);
		else if (nColl == 8) hHnum8->Fill(numOfHits);


	}

	c->cd(1);
	hHnum1->Draw();
	c->cd(2);
	hHnum2->Draw();
	c->cd(3);
	hHnum3->Draw();
	c->cd(4);
	hHnum4->Draw();
	c->cd(5);
	hHnum5->Draw();
	c->cd(6);
	hHnum6->Draw();
	c->cd(7);
	hHnum7->Draw();
	c->cd(8);
	hHnum8->Draw();

	// hTime->Draw();
	// hTimeAngle->Draw();
	// hMom->Draw();
	// hTimeMom->Draw();
	// hAngleMom->Draw();

}