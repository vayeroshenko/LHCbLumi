from ROOT import TFile, TH1D, TTree, TCanvas
import os

angle_list = range(0, 100, 10)
# angle_list = [10]
# filenames = []

mode = "coal"

def scan_angles(angles):
	nphot = {}
	for angle in angles:
		filename = "angular_test_{}_{}.root".format(mode, angle)
		os.system('./LHCbLumi vis_file.mac {} {}'.format(filename, angle))
		nphot[angle] = get_nPhot(filename)
	return nphot



def get_nPhot(name):
	file = TFile(name, "READ")
	tree = file.Get("T")		
	hist = TH1D("h_temp", "h_temp", 500, 1, 501)

	c = TCanvas("c","c", 1920,1080)
	tree.Draw("nPhot >> h_temp")
	c.SaveAs("photons/{}.png".format(name))

	return (hist.GetMean(), hist.GetRMS())


if __name__ == '__main__':
	nphot = scan_angles(angle_list)
	file_out = open("output_{}.csv".format(mode), "w")
	for angle in nphot:
		file_out.write("{},{},{}\n".format(angle,nphot[angle][0], nphot[angle][1]))
	file_out.close()