#include "Utils.h"

#include "FitEngine.h"
#include "FitterBase.h"
#include "Bs2KstKst/InvariantMassFit.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  FitEngine fitEngine("Bs2KstKstInvariantMassFit", argc, argv);

  Bs2KstKst::InvariantMassFit *fitter = new Bs2KstKst::InvariantMassFit("w","InvariantMassFit");

  fitEngine.setFitter( fitter );

  fitEngine.run();

  return 0;

}

