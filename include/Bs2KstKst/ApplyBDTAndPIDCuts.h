#ifndef Bs2KstKst_ApplyBDTAndPIDCuts_h
#define Bs2KstKst_ApplyBDTAndPIDCuts_h

#include "RooWorkspace.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

#include "Analyser.h"
#include "Bs2KstKst/Variables_Analysis.h"

namespace Bs2KstKst {

  class ApplyBDTAndPIDCuts : public Analyser {

    public:

      ApplyBDTAndPIDCuts(TString _name, Variables_Analysis *_v);
      ~ApplyBDTAndPIDCuts();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif
