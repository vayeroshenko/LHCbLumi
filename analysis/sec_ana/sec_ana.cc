{
	gROOT->Reset();

	TFile* file = new TFile("dataNew_1.root");
	TTree *t = (TTree*)file->Get("T");

	Int_t nPart;
	Int_t parentID[20000];
	Int_t grannyID[20000];
	Int_t StationID[20000];

	t->SetBranchAddress("nPart", &nPart);
	t->SetBranchAddress("ParentID", parentID);
	t->SetBranchAddress("grannyID", grannyID);
	t->SetBranchAddress("StationID", StationID);

	TH1D* hparent = new TH1D("parent","parent", 100, 0,100);
	TH1D* hgranny = new TH1D("granny","granny", 100,0,100);
	TH1D* hprim = new TH1D("primary","primary", 100,0,100);

	Long_t nEv = t->GetEntries();

	for (Long_t j = 0; j < nEv; ++j){
		t->GetEntry(j);
		std::map<Int_t, std::vector<Int_t> > spotParent;
		std::map<Int_t, std::vector<Int_t> > spotGranny;

		for (Int_t i = 0; i < nPart; ++i){
			Int_t station = StationID[i];

			const Int_t granny = grannyID[i];
			const Int_t parent = parentID[i];
			
			if (parent == 0) continue;

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

		for (auto pp : spotParent){
			std::vector<Int_t> vec = std::get<1>(pp);
			hparent->Fill(vec.size());
			if (std::get<0>(pp) == 0) hparent->Fill(0);
		}

		for (auto pp : spotGranny){
			std::vector<Int_t> vec = std::get<1>(pp);
			hgranny->Fill(vec.size());
			if (std::get<0>(pp) == 0) hgranny->Fill(0);
		}

	}

	hgranny->SetLineColor(kRed);

	hparent->Draw();
	hgranny->Draw("SAME");





}