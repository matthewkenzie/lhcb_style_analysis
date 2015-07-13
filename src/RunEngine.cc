#include <iostream>
#include <vector>

#include "boost/filesystem.hpp"

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"

#include "Utils.h"
#include "RunEngine.h"

using namespace std;
using namespace Utils;

RunEngine::RunEngine(TString _name, int argc, char **argv):
  name(_name),
  isVariablesSet(false)
{
  // Start timer
  timer.Start();
  // Pass command line arguments
  arg.parseOptions(argc,argv);
  // Read the dat file
  cfg.readDatFile(arg.cfgFile);

  if (arg.verbose || arg.debug || arg.usage) cfg.printConfig();
  if (arg.usage) exit(0);

  // setup config
  setupConfig();

  // setup event counter
  setupEventCounter();
}

RunEngine::~RunEngine(){
  timer.Stop();
  cout << "Took: "; timer.Print();
}

void RunEngine::setupConfig()
{
  // add files etc. (needs to be sorted out)
  for ( vector<InputFileOptions>::iterator fOpt = cfg.config.begin(); fOpt != cfg.config.end(); fOpt++ ) {

    // should make sure that fOpt->filenames and fOpt->treenames have the same size
    if ( fOpt->filenames.size() != fOpt->treenames.size() ) {
      error( Form("The InputFileOption with name (%s) has a different number of files and tree",fOpt->name.Data() ) );
    }
    // now can set it up
    for (int f=0; f < fOpt->filenames.size(); f++ ) {
      itypes.push_back(  fOpt->itype );
      evnames.push_back( fOpt->name  );
      sqrtss.push_back(  fOpt->sqrts );
      years.push_back (  fOpt->year  );
      infilenames.push_back( fOpt->filenames[f] );
      intreenames.push_back( fOpt->treenames[f] );
    }
  }
}

void RunEngine::setupEventCounter() {

  for ( int f=0; f < itypes.size(); f ++ ) {
    eventCounter.addDataType( itypes[f], evnames[f] );
  }
}

void RunEngine::run() {

  // check setup
  checkSetup();

  // init event counter
  eventCounter.Init();

  // initialise analysers
  for (int a=0; a<analysers.size(); a++) {
    analysers[a]->Init();
  }

  // create new file and new tree
  TFile *outfile = new TFile(arg.outFile.c_str(),"recreate");
  TTree *outtree = new TTree("AnalysisTree","AnalysisTree");

  print("RunEngine::run()" , "Created output file: \'" + arg.outFile + "\'");
  print("RunEngine::run()" , "Placed output tree:  \'AnalysisTree\' in file");

  v->setOutputBranches( outtree );

  print("RunEngine::run()" , "Set branches of output tree");

  // loop over input files
  for ( int f=0; f<infilenames.size(); f++ ) {

    boost::filesystem::path p(infilenames[f]);
    if ( ! boost::filesystem::exists(p) ) error( "File not found found: "+infilenames[f] );

    TFile *infile = TFile::Open(infilenames[f]);
    TTree *intree = (TTree*)infile->Get(intreenames[f]);

    if ( ! intree ) error("Tree: "+intreenames[f]+" not found in file: "+infilenames[f] );

    print("RunEngine::run()" , "Reading input tree: \'" + intreenames[f] + "\'");
    print("RunEngine::run()" , "         from file: \'" + infilenames[f] + "\'");

    // set input branches of old tree
    v->setInputBranches( intree );

    // setup first and last entries
    Long64_t lEntry = arg.lastEntry  > 0 ? arg.lastEntry  : intree->GetEntries() ;
    Long64_t fEntry = arg.firstEntry >= 0 ? arg.firstEntry : 0 ;

    for ( Long64_t iEntry = fEntry; iEntry < lEntry; iEntry++ ) {
      intree->GetEntry( iEntry );

      // set the run variables
      v->itype   = itypes[f];
      v->evname  = evnames[f];
      v->sqrts   = sqrtss[f];
      v->year    = years[f];

      // print progress
      printProgressBar( iEntry, fEntry, lEntry );

      // analyse the event
      bool passed = true;

      for (int a=0; a<analysers.size(); a++) {

        if ( ! analysers[a]->AnalyseEvent() ) {
          passed=false;
          eventCounter.countEvent( v->evname, analysers[a]->name, false );
          break; // can skip on if the event fails one analysis in the chain
        }
        else {
          eventCounter.countEvent( v->evname, analysers[a]->name, true );
        }
      }

      if ( passed ) {
        outtree->Fill();
      }
    }
    infile->Close();
    //printProgressBar(lEntry, fEntry, lEntry, true);

  }

  //printProgressBar(arg.lastEntry, true);
  eventCounter.printSummary();

  //outtree->Print();
  outtree->AutoSave();
  outfile->cd();
  eventCounter.hPass->Clone()->Write();
  eventCounter.hFail->Clone()->Write();
  eventCounter.hProc->Clone()->Write();
  eventCounter.hEff->Clone()->Write();
  eventCounter.hType->Clone()->Write();
  // save datfile into output root file as well
  cfg.cfgDatFile.Write();
  //delete fChain;
  delete outfile;

  // terminate analysers
  for (int a=0; a<analysers.size(); a++) {
    analysers[a]->Term();
  }

}

void RunEngine::printProgressBar(Long64_t jentry, bool isDone) {
  printProgressBar(jentry, arg.firstEntry, arg.lastEntry, isDone);
}

void RunEngine::printProgressBar(Long64_t jentry, Long64_t fEntry, Long64_t lEntry, bool isDone) {
	double percentage = 100.*double(jentry-fEntry)/double(lEntry-fEntry);
	TString prog = "[";
	for (int i=0; i<=100; i+=2) {
		if (percentage>(double(i)-0.001)) prog += "-";
		else prog += " ";
	}
	prog += "]";

	double time = timer.RealTime();
	timer.Continue();
	double timeperevent = time/double(jentry-fEntry);
	double esttimeleft = timeperevent*double(lEntry-jentry);

	if (isDone) percentage = 100.;
	TString summary = Form("%5.1f%% -- %6d/%-6d -- %8.2f ms/ev -- %10.0f secs left",percentage,int(jentry-fEntry),int(lEntry-fEntry),timeperevent*1000.,esttimeleft);

  cout << Form("%-30s","Progress:") << prog << " " << summary;
  if (isDone ) cout << endl;
  else cout << "\r" << flush;
}

void RunEngine::checkSetup() {

  if ( !isVariablesSet )  error("RunEngine: Variables not set");

  if ( infilenames.size() == 0 )  error("RunEngine:: infilenames size is 0");
  if ( intreenames.size() == 0 )  error("RunEngine:: intreenames size is 0");
  if ( infilenames.size() != intreenames.size() ) error("RunEngine:: infilenames and intreenames have a different size");
  if ( analysers.size() == 0 )    error("RunEngine:: analysers size is 0");

  boost::filesystem::path od( arg.outFile );
  boost::filesystem::path dir =  od.parent_path();
  if ( ! boost::filesystem::exists( dir ) ) {
    print( "", "Making directory: "+TString(dir.string()) );
    boost::filesystem::create_directory( dir );
  }

}

