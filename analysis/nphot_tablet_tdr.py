from ROOT import(TFile, TTree, TH1D, TCanvas,
	gStyle, TLatex)



fin = TFile("../build/test_tablet.root")
tree = fin.Get("T")
hist = TH1D("hist", "hist", 300, 0, 600)
tree.Draw("nPhot[0] >> hist")



c = TCanvas("c", "c", 1920, 1080)
hist.Fit("gaus")
hist.GetXaxis().SetTitle("Number of photons")
hist.GetYaxis().SetTitle("Entries")
# hist.GetYaxis().SetTitleOffset(0.5)
gStyle.SetOptStat(0)


latex = TLatex()
latex.SetTextSize(0.045)
# latex.SetTextAlign(13)
latex.DrawLatex(350, 3000., "Fitted mean: 274.1 photons")




# hist = TH1D("nPhot",)

