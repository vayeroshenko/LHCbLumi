#include "TH1D.h"
#include "TH2D.h"
#include "TChain.h"
#include <iostream>
#include <string>
#include "TApplication.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom3.h"
#include <vector>

#include "histo.h"

using std::string;

#define nSec 24

// TH1D* getHist(TString);


using namespace std;

// TH1D getHist(TString);

#define NHIST 1


class stabana : public histogramming {

public:
    stabana() {
        file = new TFile(filename);
        theChain1 = (TTree *) file->Get("T");
        nEv1 = theChain1->GetEntries();
        theChain1->SetBranchAddress("nPhot", nPhot);
    }
    ~stabana() = default;

    TRandom3 *rnd = new TRandom3(0);

    Int_t NMAX = 500;

    Double_t eff[100] = {0.06, 0.15, 0.3, 0.4, 0.6, 0.6, 0.9, 1.2, 1.4, \
    1.8, 1.9, 2.2, 2.6, 2.9, 3.4, 3.575, 3.95, 4.5, 4.825, 5.7, 5.9, 6.35, \
    7, 7.4, 8.1, 8.4, 9.05, 10, 10.9, 11.5, 12.1, 12.75, 14, 15, 16.4, 17, \
    18.3, 20.2, 22, 24, 25.6, 26.875, 29.75, 33, 35.5, 37.2, 39.175, 42.85, \
    46, 50, 52, 53.625, 57.25, 60, 64.3, 66.2, 67.625, 70.75, 73.8, 75.6, 76.5, \
    78.35, 80.1, 82, 83.6, 84.2, 84.9, 86.3, 87.3, 88.5, 89.1, 89.35, 90.2, 91, \
    91.9, 92.3, 92.5, 93.1, 94, 94.2, 94.6, 94.875, 95.45, 96, 96.6, 96.8, 97, \
    97.4, 97.7, 98.2, 98.35, 98.5, 98.8, 99.1, 99.4, 99.5, 99.55, 99.7, 99.9, 100};

    Int_t configuration[5] = {4,8,4,4,4};
    string Names[5] = {"Right", "Top", "Left", "Bottom", "Diagonal"};

	TString filename = "file8.root";

    TFile *file;
    TTree *theChain1;
        // if (!theChain1) { std::
        // theChain1->Add(filename);


        // Int_t nPart1;
    Int_t *nPhot = new Int_t[nSec * 2];

    Long_t nEv1;

	void doTheJob(){

        TFile *out = new TFile("histos.root","RECREATE");

     // getHistSpectra(TString filename, Int_t flag, TH1D* htot);

        Int_t i_first = 0;
        Int_t i_end = 0;

        for (Int_t i_ring = 0; i_ring < 5; ++i_ring){
            i_end += configuration[i_ring];

            book_1d("HitNumber_AND_"+ to_string(i_ring),
                nSec, 0, nSec);
            book_1d("HitNumber_OR_"+ to_string(i_ring),
                nSec, 0, nSec);
            book_1d("HitNumber_FIRST_"+ to_string(i_ring),
                nSec, 0, nSec);

            book_1d("nPhot_"+ to_string(i_ring),
                1000, 1, 1001);
            book_1d("nPhot_AND_"+ to_string(i_ring),
                1000, 1, 1001);

            for (Int_t i_side = 0; i_side < 4; ++i_side){
                book_1d("HitNumber_AND_"+ to_string(i_ring) + "_" + Names[i_side],
                    nSec, 0, nSec);
                book_1d("HitNumber_OR_"+ to_string(i_ring) + "_" + Names[i_side],
                    nSec, 0, nSec);
                book_1d("HitNumber_FIRST_"+ to_string(i_ring) + "_" + Names[i_side],
                    nSec, 0, nSec);
                if (configuration[i_ring] == 4) {
                    getHist_AND("HitNumber_AND_" + to_string(i_ring) + "_" + Names[i_side],
                                i_first + i_side, i_first + i_side + 1);
                    getHist_OR("HitNumber_OR_" + to_string(i_ring) + "_" + Names[i_side],
                               i_first + i_side, i_first + i_side + 1);
                    getHist_FIRST("HitNumber_FIRST_" + to_string(i_ring) + "_" + Names[i_side],
                                  i_first + i_side, i_first + i_side + 1);
                } else if (configuration[i_ring] == 8){
                    getHist_AND("HitNumber_AND_" + to_string(i_ring) + "_" + Names[i_side],
                                i_first + 2*i_side, i_first + 2*i_side + 1);
                    getHist_OR("HitNumber_OR_" + to_string(i_ring) + "_" + Names[i_side],
                               i_first + 2*i_side, i_first + 2*i_side + 1);
                    getHist_FIRST("HitNumber_FIRST_" + to_string(i_ring) + "_" + Names[i_side],
                                  i_first + 2*i_side, i_first + 2*i_side + 1);

                }
            }

            getHist_AND("HitNumber_AND_" + to_string(i_ring),
                        i_first, i_end);
            getHist_OR("HitNumber_OR_" + to_string(i_ring),
                       i_first, i_end);
            getHist_FIRST("HitNumber_FIRST_" + to_string(i_ring),
                          i_first, i_end);

            getHistSpectra(0, "nPhot_"+ to_string(i_ring), i_first, i_end);
            getHistSpectra(1, "nPhot_AND_"+ to_string(i_ring), i_first, i_end);


            i_first = i_end;

            set_line_color_1d("HitNumber_AND_" + to_string(i_ring), kBlue);
            set_line_color_1d("HitNumber_OR_" + to_string(i_ring), kRed);
            set_line_color_1d("HitNumber_FIRST_" + to_string(i_ring), kGreen+2);

        }

        book_1d("HitNumber_AND_tot",
            nSec, 0, nSec);
        book_1d("HitNumber_OR_tot",
            nSec, 0, nSec);
        book_1d("HitNumber_FIRST_tot",
            nSec, 0, nSec);

        book_1d("nPhot_tot",
            1000, 1, 1001);
        book_1d("nPhot_AND_tot",
            1000, 1, 1001);

        getHistSpectra(0, "nPhot_tot", 0, 24);
        getHistSpectra(1, "nPhot_AND_tot", 0, 24);


        getHist_AND("HitNumber_AND_tot", 0, 24);
        getHist_OR("HitNumber_OR_tot", 0, 24);
        getHist_FIRST("HitNumber_FIRST_tot", 0, 24);


        cout << "AND: \t" << get_mean_1d("HitNumber_AND_tot") << " \t+- \t"
                << get_mean_error_1d("HitNumber_AND_tot") << std::endl;
        cout << "OR: \t" << get_mean_1d("HitNumber_OR_tot") << " \t+- \t"
                << get_mean_error_1d("HitNumber_OR_tot") << std::endl;
        cout << "FIRST: \t" << get_mean_1d("HitNumber_FIRST_tot") << " \t+- \t"
                << get_mean_error_1d("HitNumber_FIRST_tot") << std::endl;

        out->cd();

        for (auto hist : hists1d){
            hist.second->Write();
        }

//        for (auto hist : hists2d){
//            hist.second->Write();
//        }

        out->Close();

	// delete htemp;

    }

    Bool_t isFired(Int_t nPhot) {

        if (nPhot >= 150) return true;
        else return false;

    }


    template<class name_type>
    void getHistSpectra(Int_t flag, name_type name, Int_t sec_start, Int_t sec_end) {
	    ////////// Loop 1 //////////
        for (Long_t j = 0; j < nEv1; ++j) {
            theChain1->GetEntry(j);


            for (Int_t i = sec_start; i < sec_end; ++i) {
                if (flag == 0) {
                    fill_1d(name, nPhot[i]);
                    fill_1d(name, nPhot[i + nSec]);
                }
                if (flag == 1) {
                    if (isFired(nPhot[i]) && isFired(nPhot[i + nSec])) {
                        fill_1d(name, nPhot[i]);
                        fill_1d(name, nPhot[i + nSec]);
                    }
                }
            }

        }
    }

    template<class name_type>
    void getHist_AND(name_type name, Int_t sec_start, Int_t sec_end) {

        std::cout << "AND " << sec_start << " - " << sec_end << std::endl;

        ////////// Loop 1 //////////
        for (Long_t i_ev = 0; i_ev < nEv1; ++i_ev) {
            theChain1->GetEntry(i_ev);

            Double_t numOfHits = 0;
            for (Int_t j = sec_start; j < sec_end; ++j) {
                bool fired = false;
                if (isFired(nPhot[j]) && isFired(nPhot[nSec + j])) {
                    fired = true;
                    // break;
                }
                if (fired) {
                    numOfHits += 1;
                }
            }
            fill_1d(name, numOfHits);

        }

    }

    template<class name_type>
    void getHist_OR(name_type name, Int_t sec_start, Int_t sec_end) {

        std::cout << "OR " << sec_start << " - " << sec_end << std::endl;

        ////////// Loop 1 //////////
        for (Long_t i_ev = 0; i_ev < nEv1; ++i_ev) {
            theChain1->GetEntry(i_ev);

            Double_t numOfHits = 0;
            for (Int_t j = sec_start; j < sec_end; ++j) {
                bool fired = false;
                if (isFired(nPhot[j]) || isFired(nPhot[nSec + j])) {
                    fired = true;
                    // break;
                }
                if (fired) {
                    numOfHits += 1;
                }
            }
            fill_1d(name, numOfHits);
        }

    }

    template<class name_type>
    void getHist_FIRST(name_type name, Int_t sec_start, Int_t sec_end) {

        std::cout << "FIRST " << sec_start << " - " << sec_end << std::endl;

        ////////// Loop 1 //////////
        for (Long_t i_ev = 0; i_ev < nEv1; ++i_ev) {
            theChain1->GetEntry(i_ev);

            Double_t numOfHits = 0;
            for (Int_t j = sec_start; j < sec_end; ++j) {
                bool fired = false;
                if (isFired(nPhot[j])) {
                    fired = true;
                    // break;
                }
                if (fired) {
                    numOfHits += 1;
                }
            }
            fill_1d(name, numOfHits);

        }

    }

    template<class name_type>
    inline void set_line_color_1d(name_type name, Int_t color) {hists1d[name]->SetLineColor(color);}

};


int main(int argc, char** argv){

	stabana analyzer;

	if (argc != 1) analyzer.NMAX = atoi(argv[1]);
	if (argc != 2) {
		analyzer.filename = TString(argv[1]);
		analyzer.NMAX = atoi(argv[2]);
	}
	if (argc != 3){
		analyzer.filename = TString(argv[1]);
		analyzer.NMAX = atoi(argv[2]);
	}

	// if (argc != 0) {
	// cout<<" PLUME simulation data analysis SW: "<<endl
	// 	<<"       [1] - input root file name               (default: \"" << filename << "\")   "<<endl
	// 	<<"       [2] - max number of photons in spectra   (default:     500     )   "<<endl
	// 	<<"       [3] - number of sectors in group         (default:      "<< nSec <<"     )"  <<endl
	// 	<<"                                                                          "  <<endl
	// 	<<" Recomended combinations: [2], [1][2], [1][2][3]"<< endl;
	// return 0 ;
	// }

	analyzer.doTheJob();

	
	return 0;
}
