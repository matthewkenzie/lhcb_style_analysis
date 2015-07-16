#ifndef FitEngine_h
#define FitEngine_h

#include <vector>

#include "TString.h"
#include "TStopwatch.h"
#include "TApplication.h"

#include "FitOptParser.h"
#include "FitterBase.h"

class FitEngine {

  public:

    FitEngine(TString _name, int argc, char **argv);
    ~FitEngine();

    void setFitter( FitterBase *fit );
    void run();

  private:

    TString name;
    FitOptParser arg;
    TStopwatch timer;

    FitterBase *fitter;

    TApplication *theApp;
};

#endif
