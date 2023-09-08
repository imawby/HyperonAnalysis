#ifndef _EventAssembler2_h_
#define _EventAssembler2_h_

#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "SimParticle.h"
#include "RecoParticle.h"
#include "Event.h"

using std::vector;
using std::string;

///////////////////////////////////////////////////////////////////////////////////////////////

class EventAssembler {

   private:

      // Input file and event tree
      TFile * f_in=nullptr;
      TTree * t_in;

      TTree *t_meta;
      Double_t POT;

      // Number of events (entries in event tree)
      int nEvents;

      TString DataDir;

   public:

      EventAssembler();
      EventAssembler(bool loadweights);

      // Setters and getters
      void SetFile(string infilename,string sampletype);
      void Close();

      Event GetEvent(int i);
      Long64_t GetNEvents();
      double GetPOT();		

   private:

      std::string SampleType;
      const bool LoadWeights;

      // General Info
      Bool_t          IsData;
      UInt_t          EventID;
      Int_t           run;
      Int_t           subrun;
      Int_t           event;
      Double_t        Weight;

      // Event flags
      vector<string>* Mode=0;
      vector<string>* CCNC=0;
      Int_t NMCTruths;
      Int_t NMCTruthsInTPC;
      vector<bool>* InActiveTPC=0;
      vector<bool>* IsHyperon=0;
      vector<bool>* IsLambda=0;
      vector<bool>* IsLambdaCharged=0;
      vector<bool>* IsSigmaZero=0;
      vector<bool>* IsSigmaZeroCharged=0;
      vector<bool>* IsAssociatedHyperon=0;
      vector<bool>* IsSignal=0;
      vector<bool>* IsSignalSigmaZero=0;
      Bool_t EventHasNeutronScatter;
      Bool_t EventHasHyperon;

      // Truth variables
      std::vector<SimParticle>* Neutrino=0;
      std::vector<SimParticle>* Lepton=0;
      std::vector<SimParticle>* Hyperon=0;
      std::vector<SimParticle>* PrimaryNucleon=0;
      std::vector<SimParticle>* PrimaryPion=0;
      std::vector<SimParticle>* PrimaryKaon=0;
      std::vector<SimParticle>* Decay=0;
      std::vector<SimParticle>* SigmaZeroDecayLambda=0;
      std::vector<SimParticle>* SigmaZeroDecayPhoton=0;
      std::vector<SimParticle>* KaonDecay=0;

      vector<double>  *TruePrimaryVertex_X=0;
      vector<double>  *TruePrimaryVertex_Y=0;
      vector<double>  *TruePrimaryVertex_Z=0;
      vector<double>  *DecayVertex_X=0;
      vector<double>  *DecayVertex_Y=0;
      vector<double>  *DecayVertex_Z=0;

      ////////////////////////////
      //   Output for each slice
      ////////////////////////////
      int FlashMatchedNuSliceID;
      int PandoraNuSliceID;
      int TrueNuSliceID;
      int TrueMuonTrackID;
      int TrueProtonTrackID;
      int TruePionTrackID;
      int TrueGammaTrackID;

      std::vector<int> *SliceID = nullptr;
      std::vector<bool> *GoodReco = nullptr;

      std::vector<int> *NPrimaryDaughters = nullptr;
      std::vector<int> *NPrimaryTrackDaughters = nullptr;
      std::vector<int> *NPrimaryShowerDaughters = nullptr;
      std::vector<std::vector<RecoParticle>> *TracklikePrimaryDaughters = nullptr;
      std::vector<std::vector<RecoParticle>> *ShowerlikePrimaryDaughters = nullptr;   

      std::vector<double> *RecoPrimaryVertexX = nullptr;
      std::vector<double> *RecoPrimaryVertexY = nullptr;
      std::vector<double> *RecoPrimaryVertexZ = nullptr;

      std::vector<std::vector<std::vector<int>>> *ConnSeedIndexes_Plane0 = nullptr;
      std::vector<std::vector<std::vector<int>>> *ConnOutputIndexes_Plane0 = nullptr;
      std::vector<std::vector<std::vector<int>>> *ConnOutputSizes_Plane0 = nullptr;
      std::vector<std::vector<std::vector<int>>> *ConnSeedChannels_Plane0 = nullptr;
      std::vector<std::vector<std::vector<int>>> *ConnSeedTicks_Plane0 = nullptr;

      std::vector<std::vector<std::vector<int>>> *ConnSeedIndexes_Plane1 = nullptr;
      std::vector<std::vector<std::vector<int>>> *ConnOutputIndexes_Plane1 = nullptr;
      std::vector<std::vector<std::vector<int>>> *ConnOutputSizes_Plane1 = nullptr;
      std::vector<std::vector<std::vector<int>>> *ConnSeedChannels_Plane1 = nullptr;
      std::vector<std::vector<std::vector<int>>> *ConnSeedTicks_Plane1 = nullptr;

      std::vector<std::vector<std::vector<int>>> *ConnSeedIndexes_Plane2 = nullptr;
      std::vector<std::vector<std::vector<int>>> *ConnOutputIndexes_Plane2 = nullptr;
      std::vector<std::vector<std::vector<int>>> *ConnOutputSizes_Plane2 = nullptr;
      std::vector<std::vector<std::vector<int>>> *ConnSeedChannels_Plane2 = nullptr;
      std::vector<std::vector<std::vector<int>>> *ConnSeedTicks_Plane2 = nullptr;

      // Systematics
      vector<string>* SysDials=0;
      vector<vector<double>>* SysWeights=0;
};

///////////////////////////////////////////////////////////////////////////////////////////////

#endif
