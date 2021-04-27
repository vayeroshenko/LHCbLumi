// all users - please change the name of this file to lhcbStyle.C
// Commits to lhcbdocs svn of .C files are not allowed
{
  
// Global scope variables
TStyle* lhcbStyle; // general lhcb style
TPaveText* lhcbName; // standard lhcb text for plot
TText* lhcbLabel; // style for Ttext
TLatex *lhcbLatex; //style for TLatex;

// define names for colours
Int_t black=1;
Int_t red=2;
Int_t green=3;
Int_t blue=4;
Int_t yellow=5; 
Int_t magenta=6;
Int_t cyan=7;
Int_t purple=9;

////////////////////////////////////////////////////////////////////
// PURPOSE:
//
// This macro defines a standard style for (black-and-white) 
// "publication quality" LHCb ROOT plots. 
//
// USAGE:
//
// Include the lines
//   gROOT->ProcessLine(".L lhcbstyle.C");
//   lhcbStyle();
// at the beginning of your root macro.
//
// Example usage is given in myPlot.C
//
// COMMENTS:
//
// Font:
// 
// The font is chosen to be 62, this is helvetica-bold-r-normal with
// precision 2.
//
// "Landscape histograms":
//
// The style here is designed for more or less square plots.
// For longer histograms, or canvas with many pads, adjustements are needed. 
// For instance, for a canvas with 1x5 histograms:
//  TCanvas* c1 = new TCanvas("c1", "L0 muons", 600, 800);
//  c1->Divide(1,5);
//  Adaptions like the following will be needed:
//  lhcbStyle->SetTickLength(0.05,"x");
//  lhcbStyle->SetTickLength(0.01,"y");
//  lhcbStyle->SetLabelSize(0.15,"x");
//  lhcbStyle->SetLabelSize(0.1,"y");
//  lhcbStyle->SetStatW(0.15);
//  lhcbStyle->SetStatH(0.5);
//
// Authors: Thomas Schietinger, Andrew Powell, Chris Parkes
// Maintained by Editorial board member (currently Chris)
///////////////////////////////////////////////////////////////////

lhcbStyle= new TStyle("lhcbStyle","Standard LHCb plots style");

// use helvetica-bold-r-normal, precision 2 (rotatable)
//Int_t lhcbFont = 62;
Int_t lhcbFont = 132;
// line thickness
//Double_t lhcbWidth = 3.00;
Double_t lhcbWidth = 2.50;

// Make the ends of the error bars more visible
lhcbStyle->SetEndErrorSize(3);

// use plain black on white colors
lhcbStyle->SetFrameBorderMode(0);
lhcbStyle->SetCanvasBorderMode(0);
lhcbStyle->SetPadBorderMode(0);
lhcbStyle->SetPadColor(0);
lhcbStyle->SetCanvasColor(0);
lhcbStyle->SetStatColor(0);
//lhcbStyle->SetPalette(1);

// set the paper & margin sizes
lhcbStyle->SetPaperSize(20,26);
lhcbStyle->SetPadTopMargin(0.07); //0.05
lhcbStyle->SetPadRightMargin(0.06); // increase for colz plots
lhcbStyle->SetPadBottomMargin(0.17);
lhcbStyle->SetPadLeftMargin(0.15);

// use large fonts
lhcbStyle->SetTextFont(lhcbFont);
lhcbStyle->SetTextSize(0.07);
lhcbStyle->SetLabelFont(lhcbFont,"x");
lhcbStyle->SetLabelFont(lhcbFont,"y");
lhcbStyle->SetLabelFont(lhcbFont,"z");
lhcbStyle->SetLabelSize(0.06,"x");
lhcbStyle->SetLabelSize(0.06,"y");
lhcbStyle->SetLabelSize(0.06,"z");
lhcbStyle->SetTitleFont(lhcbFont);
lhcbStyle->SetTitleFont(lhcbFont,"xyz");
lhcbStyle->SetTitleSize(0.065,"x");
lhcbStyle->SetTitleSize(0.065,"y");
lhcbStyle->SetTitleSize(0.065,"z");

lhcbStyle->SetTitleOffset(1.15,"y");

 
// use bold lines and markers
lhcbStyle->SetLineWidth(lhcbWidth);
lhcbStyle->SetFrameLineWidth(lhcbWidth);
lhcbStyle->SetHistLineWidth(lhcbWidth);
lhcbStyle->SetFuncWidth(lhcbWidth);
lhcbStyle->SetGridWidth(lhcbWidth);
lhcbStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
lhcbStyle->SetMarkerStyle(20);
lhcbStyle->SetMarkerSize(1.2);

// label offsets
lhcbStyle->SetLabelOffset(0.015);


// by default, do not display histogram decorations:
lhcbStyle->SetOptStat(0);  
//lhcbStyle->SetOptStat("emr");  // show only nent -e , mean - m , rms -r
// full opts at http://root.cern.ch/root/html/TStyle.html#TStyle:SetOptStat
lhcbStyle->SetStatFormat("6.3g"); // specified as c printf options
lhcbStyle->SetOptTitle(0);
lhcbStyle->SetOptFit(0);
//lhcbStyle->SetOptFit(1011); // order is probability, Chi2, errors, parameters

// look of the statistics box:
lhcbStyle->SetStatBorderSize(0);
lhcbStyle->SetStatFont(lhcbFont);
lhcbStyle->SetStatFontSize(0.05);
lhcbStyle->SetStatX(0.9);
lhcbStyle->SetStatY(0.9);
lhcbStyle->SetStatW(0.25);
lhcbStyle->SetStatH(0.15);
// put tick marks on top and RHS of plots
lhcbStyle->SetPadTickX(1);
lhcbStyle->SetPadTickY(1);

// histogram divisions: only 5 in x to avoid label overlaps
lhcbStyle->SetNdivisions(505,"x");
lhcbStyle->SetNdivisions(510,"y");


//define style for text
lhcbLabel = new TText();
lhcbLabel->SetTextFont(lhcbFont);
lhcbLabel->SetTextColor(1);
lhcbLabel->SetTextSize(0.04);
lhcbLabel->SetTextAlign(12);

// define style of latex text
lhcbLatex = new TLatex();
lhcbLatex->SetTextFont(lhcbFont);
lhcbLatex->SetTextColor(1);
lhcbLatex->SetTextSize(0.04);
lhcbLatex->SetTextAlign(12);

// set this style
gROOT->SetStyle("lhcbStyle");
gROOT->ForceStyle();



}
