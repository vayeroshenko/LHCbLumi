# ../build/output_coal.csv
# ../build/output_nocoal.csv

from ROOT import TGraphErrors, TCanvas, kRed, kBlue, kFullSquare, TLegend


file_coal = open("../build/output_coal.csv")
file_nocoal = open("../build/output_nocoal.csv")

graph_coal = TGraphErrors()
graph_nocoal = TGraphErrors()


counter = 0
for line in file_coal:
	items = line.split(",")
	angle, nphot, error, *_ = [float(i) for i in items]

	graph_coal.SetPoint(counter, angle, nphot)
	graph_coal.SetPointError(counter, 0, error)
	counter+=1


counter = 0
for line in file_nocoal:
	items = line.split(",")
	angle, nphot, error, *_ = [float(i) for i in items]

	graph_nocoal.SetPoint(counter, angle, nphot)
	graph_nocoal.SetPointError(counter, 0, error)
	counter+=1


graph_coal.SetLineColor(kBlue)
graph_nocoal.SetLineColor(kRed)

graph_coal.SetMarkerColor(kBlue)
graph_nocoal.SetMarkerColor(kRed)


graph_coal.SetMarkerStyle(kFullSquare)
graph_nocoal.SetMarkerStyle(kFullSquare)

graph_coal.SetMarkerSize(1)
graph_nocoal.SetMarkerSize(1)

graph_coal.SetLineWidth(2)
graph_nocoal.SetLineWidth(2)

graph_coal.GetXaxis().SetTitle("Angle, deg")
graph_coal.GetYaxis().SetTitle("Number of detected photons")


graph_coal.GetYaxis().SetRangeUser(0, 320)
graph_coal.GetXaxis().SetRangeUser(-10, 100)
graph_coal.GetXaxis().SetRange(-10, 100)
graph_coal.GetXaxis().SetNdivisions(10, 5,1)

graph_coal.Draw("APL")
graph_nocoal.Draw("Same PL")


l = TLegend(0.7,0.8,0.9,0.9)
l.AddEntry(graph_nocoal, "Without absorber", "l")
l.AddEntry(graph_coal, "With absorber", "l")
l.Draw()