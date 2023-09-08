#ifndef _GenG4WeightHandler2_h_
#define _GenG4WeightHandler2_h_

#include <vector>
#include <map>

#include "Event.h"

const int GENIE_Multisim_Universes = 600;
const int G4_Multisim_Universes = 1000; 

// Various collections of dials you might want to group together
inline const std::vector<std::string> CV_Dials = {"TunedCentralValue_UBGenie","splines_general_Spline","RootinoFix_UBGenie"};
inline const std::vector<std::string> AltModel_Dials = {"VecFFCCQEshape_UBGenie","AxFFCCQEshape_UBGenie","DecayAngMEC_UBGenie","NormCCCOH_UBGenie","NormNCCOH_UBGenie","ThetaDelta2NRad_UBGenie","Theta_Delta2Npi_UBGenie","XSecShape_CCMEC_UBGenie"};
inline const std::vector<std::string> AltModel_Captions = {"CCQE Vec. FF Shape","CCQE Ax. FF Shape","MEC Decay Angle","CCCOH Norm.","NCCOH Norm","#Delta Rad. Angle","#Delta Had. Angle","CCMEC Cross Section"};
inline const std::vector<std::string> G4_Dials = {"reinteractions_proton_Geant4","reinteractions_piplus_Geant4","reinteractions_piminus_Geant4","reinteractions_Lambda_Geant4"};
inline const std::vector<std::string> Multisim_Dials = {"All_UBGenie","reinteractions_proton_Geant4","reinteractions_piplus_Geant4","reinteractions_piminus_Geant4","reinteractions_Lambda_Geant4"};

class GenG4WeightHandler {

   public: 

      GenG4WeightHandler();
      void LoadEvent(Event e);

      // Retrieve the central value weight for an event      
      double GetCVWeight();
      std::vector<double> GetWeights(std::string dial);

   private:

      bool HasWeights;
      void OrganiseWeights();
      std::vector<std::string> *theDials;
      std::vector<std::vector<double>> *theWeights;
      std::map<std::string,std::vector<double>> theWeightMap;     
      double EventTunedCentralValue = 1.0;

};

#endif
