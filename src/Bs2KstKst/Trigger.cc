#include "TLorentzVector.h"

#include "Bs2KstKst/Trigger.h"

using namespace std;

Bs2KstKst::Trigger::Trigger(TString _name, Variables_PreSel *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::Trigger::~Trigger(){}

bool Bs2KstKst::Trigger::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //
  // L0
  if ( ! v->B_s0_L0HadronDecision_TOS                 &&
       ! v->B_s0_L0Global_TIS														 ) return false;

  // HLT1
  if ( ! v->B_s0_Hlt1DiHadronDecision_TOS             &&
       ! v->B_s0_Hlt1SingleHadronDecision_TOS         &&
       ! v->B_s0_Hlt1TrackAllL0Decision_TOS               ) return false;

  // HLT2
  if ( ! v->B_s0_Hlt2B2HHDecision_TOS            			&&
       ! v->B_s0_Hlt2Topo2BodyBBDTDecision_TOS    		&&
       ! v->B_s0_Hlt2Topo2BodySimpleDecision_TOS 			&&
       ! v->B_s0_Hlt2Topo3BodyBBDTDecision_TOS   			&&
       ! v->B_s0_Hlt2Topo3BodySimpleDecision_TOS 			&&
       ! v->B_s0_Hlt2Topo4BodyBBDTDecision_TOS   			&&
       ! v->B_s0_Hlt2Topo4BodySimpleDecision_TOS 			&&
       ! v->B_s0_Hlt2TopoMu4BodyBBDTDecision_TOS 			&&
       ! v->B_s0_Hlt2TopoOSTF4BodyDecision_TOS   					) return false;

  TLorentzVector B_s0_p4;
  B_s0_p4.SetXYZM( v->B_s0_PX, v->B_s0_PY, v->B_s0_PZ, v->B_s0_M );

  v->B_s0_ETA = B_s0_p4.Eta();

  passedEvents++;
  return true;
}
