#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TGraphErrors.h"

void pmt_ana(int flag){
	gROOT->Reset();

	// TFile* f_al = new TFile("histos_TDR.root");
	TFile* f_al = new TFile("histos.root");

	TH1D* h_and = (TH1D*)f_al->Get("HitNumber_AND_tot");
	TH1D* h_or = (TH1D*)f_al->Get("HitNumber_OR_tot");
	TH1D* h_first = (TH1D*)f_al->Get("HitNumber_FIRST_tot");

	h_and->SetLineColor(kRed);
	h_or->SetLineColor(kBlue);
	h_first->SetLineColor(kGreen+2);

	h_and->Scale(1./h_and->GetEntries());
	h_or->Scale(1./h_or->GetEntries());
	h_first->Scale(1./h_first->GetEntries());

	Double_t occ_and = h_and->GetBinContent(2);
	Double_t occ_or = h_or->GetBinContent(2) ;
	Double_t occ_first = h_first->GetBinContent(2);

	TGraphErrors *gr_and = new TGraphErrors();
	TGraphErrors *gr_or = new TGraphErrors();
	TGraphErrors *gr_first = new TGraphErrors();

	Double_t sensitivity = 1e-6;

	for (Int_t i_bin = 1; i_bin <= h_and->GetNbinsX(); ++i_bin){
		if (h_and->GetBinContent(i_bin) > sensitivity){
			gr_and->SetPoint(i_bin-1, i_bin-1, h_and->GetBinContent(i_bin));
			gr_and->SetPointError(i_bin-1, 0, h_and->GetBinError(i_bin));
		}
		if (h_or->GetBinContent(i_bin) > sensitivity){
			gr_or->SetPoint(i_bin-1, i_bin-1, h_or->GetBinContent(i_bin));
			gr_or->SetPointError(i_bin-1, 0, h_or->GetBinError(i_bin));
		}
		if (h_first->GetBinContent(i_bin) > sensitivity){
			gr_first->SetPoint(i_bin-1, i_bin-1, h_first->GetBinContent(i_bin));
			gr_first->SetPointError(i_bin-1, 0, h_first->GetBinError(i_bin));
		}
	}	

	gr_and->SetLineColor(h_and->GetLineColor());
	gr_or->SetLineColor(h_or->GetLineColor());
	gr_first->SetLineColor(h_first->GetLineColor());

	gr_and->SetMarkerStyle(kFullSquare);
	gr_or->SetMarkerStyle(kFullSquare);
	gr_first->SetMarkerStyle(kFullSquare);

	gr_and->SetMarkerSize(1.5);
	gr_or->SetMarkerSize(1.5);
	gr_first->SetMarkerSize(1.5);

	gr_and->SetLineWidth(4);
	gr_or->SetLineWidth(4);
	gr_first->SetLineWidth(4);


	gr_and->SetMarkerColor(h_and->GetLineColor());
	gr_or->SetMarkerColor(h_or->GetLineColor());
	gr_first->SetMarkerColor(h_first->GetLineColor());
	
	gROOT->ProcessLine(".L lhcbStyle.C");

	TGraphErrors *gr_dummy = new TGraphErrors();
	gr_dummy->SetPoint(0, 1000, 0);

	TCanvas *c = new TCanvas("c","c",1920,1080);
	c->Divide(2,1);
	auto pad_lin = c->cd(1);
	

	// h_and->SetLineWidth(2);
	// h_or->SetLineWidth(2);
	// h_first->SetLineWidth(2);

	// h_and->GetXaxis()->SetRangeUser(0., 12.);
	// h_and->GetYaxis()->SetRangeUser(0., 1.);
	// h_and->GetXaxis()->SetNdivisions(13, 0, 0);

	// gr_dummy->Draw("APL");
	// h_and->Draw(	"HIST  SAME");
	// h_or->Draw(		"HIST  SAME");
	// h_first->Draw(	"HIST  SAME");
	
	// gr_dummy->GetXaxis()->SetRangeUser(0., 12.);
	// gr_dummy->GetXaxis()->SetLimits(0., 12.);
	// gr_dummy->GetYaxis()->SetRangeUser(0, 1.);

	// gr_dummy->GetXaxis()->SetNdivisions(13, 0, 0);

	gr_and->Draw("APL");
	gr_first->Draw("SAME PL");
	gr_or->Draw("SAME PL");

	gr_and->GetXaxis()->SetRangeUser(0., 12.);
	gr_and->GetXaxis()->SetLimits(0., 12.);
	gr_and->GetYaxis()->SetRangeUser(1e-7, 1.1);

	// gr_and->GetXaxis()->SetNdivisions(13, 0, 0);
	gr_and->GetXaxis()->SetNdivisions(7, 2, 0);
	gr_and->GetYaxis()->SetNdivisions(6, 2, 5);

	c->Update();

	TLegend *l = new TLegend(0.65,0.75,0.9,0.9);
	// l->SetHeader("20 sectors, 6 cm from BP", "c");

	if (flag == 0){
		l->AddEntry(h_and, TString("AND (" + std::to_string(occ_and) + ")" ) );
		l->AddEntry(h_or, TString("OR (" + std::to_string(occ_or) + ")"));
		l->AddEntry(h_first, TString("First layer only (" + std::to_string(occ_first) + ")") );
		l->Draw();
	}


	// TLegend *l = new TLegend(0.65,0.75,0.9,0.9);
	// l->SetHeader("20 sectors, 6 cm from BP", "c");
	

	// if (flag == 1){
	// 	l->AddEntry(h_and, "AND" );
	// 	l->AddEntry(h_or, "OR");
	// 	l->AddEntry(h_first, "First layer only");
	// 	l->Draw();
	// }
	if (flag == 1){
		l->AddEntry(gr_and, "AND", "lpe");
		l->AddEntry(gr_or, "OR", "lpe");
		l->AddEntry(gr_first, "First layer only", "lpe");
		l->Draw();
	}

	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);


	auto pad_log = c->cd(2);

	pad_log->SetLogy();
	gr_and->Draw("APL");
	gr_first->Draw("SAME PL");
	gr_or->Draw("SAME PL");

	l->Draw();

	c->SaveAs("comparison.png");

}


