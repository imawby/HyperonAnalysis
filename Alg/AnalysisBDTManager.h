#ifndef _AnalysisBDTManager_h_
#define _AnalysisBDTManager_h_

#include "Event.h"
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

#include "KinematicsCalc.h"
#include "RecoParticle.h"
#include "SecondaryVertexFitter.h"
#include "Misc.h"

// Analysis BDT class. Has two modes, training
// and testing. Training for filling the trees train
// new BDT tunes. Testing to use them in the selection.

class AnalysisBDTManager {

   public:

      AnalysisBDTManager();
      AnalysisBDTManager(std::string Mode);
      ~AnalysisBDTManager();

      void FillTree(const Event &e);
      void WriteTrainingTrees();    

      void SetupAnalysisBDT(std::string WeightsDir="");
  
      void SetPull(double Pull);

   private:

      std::string fMode;
      bool SetVariables(const Event &e);
      bool SetVariables(const RecoParticle &protonCandidate, const RecoParticle &pionCandidate,
          const TVector3 &nuVertex3D, const double bdtScore, const double eventWeight);

      // Training trees

      void SetupTrainingTrees();    

      SecondaryVertexFitter Fitter;

      TFile *f_Trees;
      TTree *t_Signal=nullptr;
      TTree *t_Background=nullptr;

      // Variables

      Float_t v_w;
      Float_t v_gap;
      Float_t v_bdt_score;
      Float_t v_lambda_angle;
      Float_t v_lambda_V_crossing_dist;
      Float_t v_weight;

      std::pair<double,double> W_limits = { 1.0 , 1.3 };
      std::pair<double,double> gap_limits = { 0.0 , 20.0 };
      std::pair<double,double> bdt_score_limits = { -1.0 , 1.0 };

      // Importing BDT and using in selection
      std::string fWeightsDir;
      TMVA::Reader *reader=nullptr;

   public:
      void SetupSelectorBDT(std::string WeightsDir="");

      double CalculateScore(const RecoParticle &protonCandidate, const RecoParticle &pionCandidate,
          const TVector3 &nuVertex3D, const double bdtScore, const double eventWeight);
};



#endif

