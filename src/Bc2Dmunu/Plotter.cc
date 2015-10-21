#include "Bc2Dmunu/Plotter.h"
#include "TMath.h"

using namespace std;
using namespace TMath;

Bc2Dmunu::Plotter::Plotter(TString _name, const Variables_Analysis *_v):
  PlotterBase(_name,_v),
  v(_v),
  split(false)
{
  normalise = true;
  normalisation = 1.;
  outfilename = Form("root/%sOut.root",_name.Data());
}

Bc2Dmunu::Plotter::~Plotter(){}

void Bc2Dmunu::Plotter::defineHistograms(){

  // add these histograms
  //

  addHist("Bplus_M"    , "m(B^{+}) [MeV]", 100, 2000,8000, "R");
  addHist("Bplus_Mcorr", "m_{corr}(B^{+}) [MeV]", 100,2000,8000, "R");
  addHist("D0_M"       , "m(D^{0}) [MeV]", 100, 1800, 1950, "R");
  addHist("D0_LOGMINIPCHI2", "Log Min IP #chi^{2} (D^{0})", 100,-5,15,"L");

  addHist("Bplus_LOGMINIPCHI2", "Log Min IP #chi^{2} (B^{+})", 100,-5,15,"L");
  addHist("Bplus_DIRA_OWNPV",   "DIRA (B^{+})", 100, 0.999,1.0,"L");

  addHist("Bplus_LOGENDVERTEX_CHI2", "Log vtx #chi^{2} (B^{+})", 100, -5, 15, "L");
  addHist("D0_LOGENDVERTEX_CHI2", "Log vtx #chi^{2} (D^{0})", 100, -5, 15, "L");

  addHist("Bplus_ENDVERTEX_CHI2", "Vtx #chi^{2} (B^{+})", 100, 0, 16, "L");
  addHist("D0_ENDVERTEX_CHI2", "Vtx #chi^{2} (D^{0})", 100, 0, 16, "L");

  addHist("Kminus_PIDK", "PIDK (K^{-})",   100, 0, 200, "R");
  addHist("piplus_PIDK", "PIDK (#pi^{+})", 100, -200, 0, "L");
}

void Bc2Dmunu::Plotter::defineDrawingConfig(){

  // add these drawing options
  // -------------------------------------------- //
  // -------------------------------------------- //
  TColor *blueFill = gROOT->GetColor(kBlue-7);
  blueFill->SetAlpha(0.4);
  TColor *redFill = gROOT->GetColor(kRed-7);
  redFill->SetAlpha(0.4);
  TColor *greenFill = gROOT->GetColor(kGreen-3);
  greenFill->SetAlpha(0.4);

  addDrawOpt("mc_Bu",   "MC B^{+}#rightarrow D^{0}#mu#nu", -82);
  setDrawOptDefaultFill(blueFill->GetNumber());

  addDrawOpt("mc_Bc",   "MC B^{+}_{c}#rightarrow D^{0}#mu#nu", -80);
  setDrawOptDefaultFill(redFill->GetNumber());

  addDrawOpt("mc_Bc_Dst", "MC B^{+}_{c}#rightarrow D^{*}#mu#nu", -81);
  setDrawOptDefaultFill(greenFill->GetNumber());

  addDrawOpt("data",   "Data", 80);
  setDrawOptDefaultPoint(kBlack);

  addResidOpt(make_pair(3,0));
  setResidType(1);
  // -------------------------------------------- //

}

bool Bc2Dmunu::Plotter::fillHistograms(){

  // fill hists now
  fillHist("Bplus_M"     , v->Bplus_M      );
  fillHist("Bplus_Mcorr" , v->Bplus_Mcorr  );
  fillHist("D0_M"        , v->D0_M         );
  fillHist("D0_LOGMINIPCHI2", TMath::Log(v->D0_MINIPCHI2) );
  fillHist("Bplus_LOGMINIPCHI2", TMath::Log(v->Bplus_MINIPCHI2) );
  fillHist("Bplus_DIRA_OWNPV", v->Bplus_DIRA_OWNPV );
  fillHist("Bplus_LOGENDVERTEX_CHI2", TMath::Log( v->Bplus_ENDVERTEX_CHI2 ) );
  fillHist("D0_LOGENDVERTEX_CHI2", TMath::Log( v->D0_ENDVERTEX_CHI2 ) );
  fillHist("Bplus_ENDVERTEX_CHI2",  v->Bplus_ENDVERTEX_CHI2 ) ;
  fillHist("D0_ENDVERTEX_CHI2",  v->D0_ENDVERTEX_CHI2 ) ;
  fillHist("Kminus_PIDK", v->Kminus_PIDK );
  fillHist("piplus_PIDK", v->piplus_PIDK );
  return true;
}
