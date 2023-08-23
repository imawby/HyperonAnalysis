#include <vector>
#include "SimParticle.h"
#include "RecoParticle.h"

#ifdef __ROOTCLING__
#pragma link C++ class SimParticle+;
#pragma link C++ class RecoParticle+;
#pragma link C++ class vector<SimParticle>+;
#pragma link C++ class vector<RecoParticle>+;
#pragma link C++ class vector<vector<RecoParticle>>+;
#pragma link C++ class vector<vector<vector<int>>>+;
#endif
