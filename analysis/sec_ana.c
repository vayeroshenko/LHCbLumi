{
	auto material = TString("al");
	auto filename = TString("file_"+material+"_2.root");

	auto file = new TFile(filename);
	auto tree = (TTree*)file->Get("T");
	
	auto hist_z = new TH1D("z", "z", 50, -2100, 100);
	auto hist_dist = new TH1D("sec", "sec", 2, 0, 2);

	tree->Draw("birthZ >> z", "StationID > 0");
	tree->Draw("isPrimary >> sec", "StationID > 0");


	auto c = new TCanvas("c", "c", 1920, 1080);
	hist_z->Draw();
	c->SaveAs(TString(material + "_z_lin_2.png"));

	gPad->SetLogy();
	c->SaveAs(TString(material + "_z_log_2.png"));



	auto n_sec = hist_dist->GetBinContent(1);
	auto n_prim = hist_dist->GetBinContent(2);

	auto n_ev = tree->GetEntries();


	std::cout << "Number of primaries: \t" << n_prim / n_ev << std::endl;
	std::cout << "Number of secondaries: \t" << n_sec /n_ev << std::endl << std::endl; 
	
	std::cout << "Number of hits: \t\t" << (n_sec + n_prim) / n_ev << std::endl; 
	std::cout << "Ratio of primaries: \t\t" << n_prim / (n_sec + n_prim) << std::endl; 


	c->Close();

}