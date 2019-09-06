{
	gROOT->Reset();

	TFile* file = new TFile("dataNew_1.root");
	TTree *t = (TTree*)file->Get("T");

	Int_t nPart;
	Int_t parentID[20000];
	Int_t trackID[20000];
	Int_t grannyID[20000];
	Int_t StationID[20000];

	t->SetBranchAddress("nPart", &nPart);
	t->SetBranchAddress("ParentID", parentID);
	t->SetBranchAddress("TrackID", trackID);
	t->SetBranchAddress("grannyID", grannyID);
	t->SetBranchAddress("StationID", StationID);

	TH1D* hparent = new TH1D("parent","parent", 100, 0,100);
	TH1D* hgranny = new TH1D("granny","granny", 100,0,100);
	TH1D* hprim = new TH1D("primary","primary", 100,0,100);


	// TH1D* hp0 = new TH1D("p0","p0", 300,0,300);
	// TH1D* hp1 = new TH1D("p1","p1", 300,0,300);
	// TH1D* hs0 = new TH1D("s0","s0", 300,0,300);
	// TH1D* hs1 = new TH1D("s1","s1", 300,0,300);

	TH1D* hr0 = new TH1D("ratio0", "s0 / p0", 100,0,2);
	TH1D* hr1 = new TH1D("ratio1", "s1 / p1", 100,0,2);

	Long_t nEv = t->GetEntries();

	for (Long_t j = 0; j < nEv; ++j){
		t->GetEntry(j);
		std::map<Int_t, std::vector<Int_t> > spotPrimary;
		std::map<Int_t, std::vector<Int_t> > spotParent;
		std::map<Int_t, std::vector<Int_t> > spotGranny;

		for (Int_t i = 0; i < nPart; ++i){
			Int_t station = StationID[i];

			const Int_t granny = grannyID[i];
			const Int_t parent = parentID[i];
			const Int_t trk = trackID[i];
			
			if (parent == 0) {
				if (spotPrimary.find(trk) == spotPrimary.end()) {
					spotPrimary.insert(	std::pair<Int_t, std::vector<Int_t>>
										(trk, std::vector<Int_t>(0)) );
				}
				if (std::find(	spotPrimary.at(trk).begin(), 
								spotPrimary.at(trk).end(),
								station) == spotPrimary.at(trk).end())
				spotPrimary.at(trk).push_back(station);
			}


			if (spotParent.find(parent) == spotParent.end()) {
				spotParent.insert(	std::pair<Int_t, std::vector<Int_t>>
									(parent, std::vector<Int_t>(0)) );
			}

			if (spotGranny.find(granny) == spotGranny.end()) {
				spotGranny.insert(	std::pair<Int_t, std::vector<Int_t>>
									(granny, std::vector<Int_t>(0)) );
			}

			if (std::find(	spotParent.at(parent).begin(), 
							spotParent.at(parent).end(),
							station) == spotParent.at(parent).end())
				spotParent.at(parent).push_back(station);

			if (std::find(	spotGranny.at(granny).begin(), 
							spotGranny.at(granny).end(),
							station) == spotGranny.at(granny).end())
				spotGranny.at(granny).push_back(station);
		
		}

		Int_t p0 = 0;
		Int_t p1 = 0;
		Double_t s0 = 0;
		Double_t s1 = 0;

		std::vector<Int_t> vp0(0);
		std::vector<Int_t> vp1(0);

		for (auto pp : spotPrimary){
			std::vector<Int_t> vec = std::get<1>(pp);
			vec.erase(std::remove(vec.begin(), vec.end(), -1), vec.end());
			if (vec.size() == 0) vp0.push_back(std::get<0>(pp));
			else vp1.push_back(std::get<0>(pp));
		}	

		for (auto pp : spotParent){
			std::vector<Int_t> vec = std::get<1>(pp);
			hparent->Fill(vec.size());
		}

		for (auto pp : spotGranny){
			auto grID = std::get<0>(pp);
			auto vec = std::get<1>(pp);
			hgranny->Fill(vec.size());

			if (std::find(vp0.begin(),vp0.end(), grID) != vp0.end()) s0 ++;
			if (std::find(vp1.begin(),vp1.end(), grID) != vp1.end()) s1 ++;

		}


		hr0->Fill(s0 / vp0.size());
		hr1->Fill(s1 / vp1.size());

		// hp0->Fill(p0);
		// hp1->Fill(p1);
		// hs0->Fill(s0);
		// hs1->Fill(s1);

	}

	// hgranny->SetLineColor(kRed);

	// hparent->Draw();
	// hgranny->Draw("SAME");

	hr1->SetLineColor(kRed);

	hr1->Draw();
	hr0->Draw("same");


}