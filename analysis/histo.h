//
// Created by vsevolod on 26/04/2021.
//

#pragma once

#include "TH1D.h"
#include "TH2D.h"

#include <map>
#include <string>


using std::map;
using std::string;

class histogramming {
public:
    histogramming() ;
    ~histogramming() ;

protected:
    map<string, TH1D*> hists1d;
	map<string, TH2D*> hists2d;

public:
    ////////////////// Histogramming ///////////////
	void book_1d(const char* name,
                    const Int_t& nbins = 100, const Double_t& xmin = 0, const Double_t& xmax = -1);
    void book_1d(const string& name,
                    const Int_t& nbins = 100, const Double_t& xmin = 0, const Double_t& xmax = -1);

	void book_2d(const char* name,
                    const Int_t& nbinsx = 100, const Double_t& xmin = 0, const Double_t& xmax = -1,
                    const Int_t& nbinsy = 100, const Double_t& ymin = 0, const Double_t& ymax = -1);
	void book_2d(const string& name,
                    const Int_t& nbinsx = 100, const Double_t& xmin = 0, const Double_t& xmax = -1,
                    const Int_t& nbinsy = 100, const Double_t& ymin = 0, const Double_t& ymax = -1);

	template<class name_type>
    void fill_1d(name_type name,   Double_t val);
	template<class name_type>
	void fill_2d(name_type name,   Double_t valx, Double_t valy);

    template<class name_type>
    void fill_1d(name_type name,   Double_t val,
                 const Int_t& nbins, const Double_t& xmin, const Double_t& xmax);

    template<class name_type>
    inline Double_t get_bin_content_1d(name_type name, Int_t i_bin) {return  hists1d[name]->GetBinContent(i_bin);}
    template<class name_type>
    inline void set_bin_content_1d(name_type name, Int_t i_bin, Double_t val) {hists1d[name]->SetBinContent(i_bin, val);}
    template<class name_type>
    inline void scale_1d(name_type name, Double_t scale) {hists1d[name]->Scale(scale);}
    template<class name_type>
    inline void stack_bin_1d(name_type name, Int_t i_bin, Double_t val)
        {hists1d[name]->SetBinContent(i_bin, hists1d[name]->GetBinContent(i_bin)+val);}
    template<class name_type>
    inline void stack_bin_error_1d(name_type name, Int_t i_bin, Double_t error)
        {hists1d[name]->SetBinError(i_bin, hists1d[name]->GetBinError(i_bin)+error);}
    template<class name_type>
    inline Double_t get_mean_1d(name_type name) {return  hists1d[name]->GetMean();}
    template<class name_type>
    inline Double_t get_mean_error_1d(name_type name) {return  hists1d[name]->GetMeanError();}
    template<class name_type>
    inline Double_t get_entries_1d(name_type name) {return  hists1d[name]->GetEntries();}
    ////////////// Histogramming END //////////////////

};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include "histo.h"



histogramming::histogramming() {};
histogramming::~histogramming() {};

void histogramming::book_1d(const char* name,
                           const Int_t& nbins, const Double_t& xmin, const Double_t& xmax){
    if ( hists1d.find(name) == hists1d.end() ) {
        hists1d[name] = new TH1D(name, name, nbins, xmin, xmax);
	}
}

void histogramming::book_1d(const string& name,
                           const Int_t& nbins, const Double_t& xmin, const Double_t& xmax){
    if ( hists1d.find(name) == hists1d.end() ) {
        hists1d[name] = new TH1D(name.data(), name.data(), nbins, xmin, xmax);
	}
}

void histogramming::book_2d(const char* name,
                           const Int_t& nbinsx, const Double_t& xmin, const Double_t& xmax,
                           const Int_t& nbinsy, const Double_t& ymin, const Double_t& ymax){
    if ( hists2d.find(name) == hists2d.end() ) {
        hists2d[name] = new TH2D(name, name,    nbinsx, xmin, xmax,
                                                nbinsy, ymin, ymax);
	}
}

void histogramming::book_2d(const string& name,
                           const Int_t& nbinsx, const Double_t& xmin, const Double_t& xmax,
                           const Int_t& nbinsy, const Double_t& ymin, const Double_t& ymax){
    if ( hists2d.find(name) == hists2d.end() ) {
        hists2d[name] = new TH2D(name.data(), name.data(),    nbinsx, xmin, xmax,
                                                nbinsy, ymin, ymax);
	}
}


template<class name_type>
void histogramming::fill_1d(name_type name, Double_t val){
    if ( hists1d.find(name) == hists1d.end() ) {
        book_1d(name);
	}
    hists1d[name]->Fill(val);
}

template<class name_type>
void histogramming::fill_2d(name_type name, Double_t valx, Double_t valy){
    if ( hists2d.find(name) == hists2d.end() ) {
        book_2d(name);
	}
    hists2d[name]->Fill(valx, valy);
}

template<class name_type>
void histogramming::fill_1d(name_type name, Double_t val,
                           const Int_t &nbins, const Double_t &xmin, const Double_t &xmax) {
    if ( hists1d.find(name) == hists1d.end() ) {
        book_1d(name, nbins, xmin, xmax);
	}
    hists1d[name]->Fill(val);
}


