#include "Utils.h"

#include "RunEngine.h"
#include "Bs2KstKst/Variables_PreSel.h"
#include "Analyser.h"
#include "Bs2KstKst/Trigger.h"
#include "Bs2KstKst/TruthMatching.h"
#include "Bs2KstKst/PreSelection.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  RunEngine runner("RunEngine", argc, argv);

  Bs2KstKst::Variables_PreSel *v = new Bs2KstKst::Variables_PreSel() ;

  Analyser *truthMatch = new Bs2KstKst::TruthMatching( "TruthMatching" , v );
  Analyser *triggerSel = new Bs2KstKst::Trigger      ( "Trigger"       , v );
  Analyser *presel     = new Bs2KstKst::PreSelection ( "PreSelection"  , v );

  runner.setVariables( v );

  runner.addAnalyser( truthMatch );
  runner.addAnalyser( triggerSel );
  runner.addAnalyser( presel     );

  runner.run();

  delete v;
  delete truthMatch;
  delete triggerSel;
  delete presel;

  return 0;

}
