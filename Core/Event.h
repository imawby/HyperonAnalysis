#ifndef _Event2_h_
#define _Event2_h_

#include <vector>

#include "TVector3.h"

#include "SimParticle.h"
#include "RecoParticle.h"

using std::vector;
using std::string;

struct Event {

	bool IsData=false;
	UInt_t EventID;
	Int_t run;
	Int_t subrun;
	Int_t event;
	Int_t fileID;
	double Weight=1.0;

    // Flags applying to the entire event
    Bool_t EventIsSignal = false;
    Bool_t EventHasHyperon = false;
    Bool_t EventHasNeutronScatter = false;
    Bool_t EventIsSignalSigmaZero = false;

    // Flags for each MCTruth
    vector<string> Mode;
    vector<string> CCNC;
    Int_t NMCTruths;
    Int_t NMCTruthsInTPC;
    vector<bool> InActiveTPC;
    vector<bool> IsHyperon;
    vector<bool> IsLambda;
    vector<bool> IsLambdaCharged;
    vector<bool> IsSigmaZero;
    vector<bool> IsSigmaZeroCharged;
    vector<bool> IsAssociatedHyperon;
    vector<bool> IsSignal;
    vector<bool> IsSignalSigmaZero;
    vector<TVector3> TruePrimaryVertex;

	//true variables
	std::vector<SimParticle> Neutrino;
	std::vector<SimParticle> Lepton;
	std::vector<SimParticle> Hyperon;
	std::vector<SimParticle> PrimaryNucleon;
	std::vector<SimParticle> PrimaryPion;
	std::vector<SimParticle> PrimaryKaon;
	std::vector<SimParticle> Decay;
	std::vector<SimParticle> SigmaZeroDecayLambda;
	std::vector<SimParticle> SigmaZeroDecayPhoton;
	std::vector<SimParticle> KaonDecay;

    vector<TVector3> DecayVertex;

    ////////////////////////////
    //   Output for each slice
    ////////////////////////////
    int ChoosenNuSliceID;
    int TrueNuSliceID;
    int TrueMuonTrackID;
    int TrueProtonTrackID;
    int TruePionTrackID;
    int TrueGammaTrackID;

    std::vector<int> SliceID;
    std::vector<bool> GoodReco;

    std::vector<int> NPrimaryDaughters;
    std::vector<int> NPrimaryTrackDaughters;
    std::vector<int> NPrimaryShowerDaughters;
    std::vector<std::vector<RecoParticle>> TracklikePrimaryDaughters;
    std::vector<std::vector<RecoParticle>> ShowerlikePrimaryDaughters;

    std::vector<TVector3> RecoPrimaryVertex;

    std::vector<std::vector<std::vector<int>>> ConnSeedIndexes_Plane0;
    std::vector<std::vector<std::vector<int>>> ConnOutputIndexes_Plane0;
    std::vector<std::vector<std::vector<int>>> ConnOutputSizes_Plane0;
    std::vector<std::vector<std::vector<int>>> ConnSeedChannels_Plane0;
    std::vector<std::vector<std::vector<int>>> ConnSeedTicks_Plane0;

    std::vector<std::vector<std::vector<int>>> ConnSeedIndexes_Plane1;
    std::vector<std::vector<std::vector<int>>> ConnOutputIndexes_Plane1;
    std::vector<std::vector<std::vector<int>>> ConnOutputSizes_Plane1;
    std::vector<std::vector<std::vector<int>>> ConnSeedChannels_Plane1;
    std::vector<std::vector<std::vector<int>>> ConnSeedTicks_Plane1;

    std::vector<std::vector<std::vector<int>>> ConnSeedIndexes_Plane2;
    std::vector<std::vector<std::vector<int>>> ConnOutputIndexes_Plane2;
    std::vector<std::vector<std::vector<int>>> ConnOutputSizes_Plane2;
    std::vector<std::vector<std::vector<int>>> ConnSeedChannels_Plane2;
    std::vector<std::vector<std::vector<int>>> ConnSeedTicks_Plane2;

	// Selection stuff
    std::vector<RecoParticle> MuonCandidate;
    std::vector<RecoParticle> DecayProtonCandidate;
    std::vector<RecoParticle> DecayPionCandidate;

    std::vector<Float_t> SelectorBDTScore;
    std::vector<Float_t> AnalysisBDTScore;

    // Systematics    
    vector<string> SysDials;
    vector<vector<vector<double>>> SysWeights;

    void Print(){ std::cout << run << "  " << subrun << "  " << event << std::endl; }
};

#endif
