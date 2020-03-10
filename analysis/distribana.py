from ROOT import TH1D, TFile, TTree, TLegend
from ROOT import kRed, gStyle


file_with = TFile("../build/data_with_surround.root")
file_without = TFile("../build/data_without_surround.root")
tree_with = file_with.Get("T")
tree_without = file_without.Get("T")

hist_prim =  TH1D("prim", "prim", 1000, 0. ,1000.)
hist_all =  TH1D("all", "all", 1000, 0. ,1000.)

tree_with.Draw("hitR >> all")
tree_without.Draw("hitR >> prim")

hist_prim.SetLineColor(kRed)

hist_all.GetXaxis().SetTitle("R on Z = -2300 mm plane (mm)")


hist_all.Draw()
hist_prim.Draw("same")

gStyle.SetOptStat(0)

l = TLegend(0.6, 0.7, 0.9, 0.9)
l.AddEntry(hist_all, "With surrounding", "l")
l.AddEntry(hist_prim, "Without surrounding", "l")
l.Draw()

