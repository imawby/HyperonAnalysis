#ifndef _RecoParticle_h_
#define _RecoParticle_h_

#include <iostream>

#include "TLorentzVector.h"
#include "TVector3.h"

#ifdef __MAKE_ROOT_DICT__
#include "TObject.h"
#endif

#ifdef __MAKE_ROOT_DICT__
class RecoParticle : public TObject{
#else
class RecoParticle {
#endif

public:

RecoParticle(){}
~RecoParticle(){}

int Self;
bool InNuSlice = false;
int Key = -1;
int TrackVectorIndex = -1;

// General reco info
int PDG; // Pandora PDG code (11 or 13)
int Parentage; // 1 - neutrino daughter, 2 - neutrino granddaughter, 3 - other
int ParentIndex=-1; // -1 - neutrino candidate or no parent
double TrackShowerScore;
double X,Y,Z;
double Displacement; // Distance from PV

// Track variables
double TrackLength=0;
double TrackDirectionX=0,TrackDirectionY=0,TrackDirectionZ=0;
double TrackStartX=0,TrackStartY=0,TrackStartZ=0;
double TrackEndX=0,TrackEndY=0,TrackEndZ=0;
double TrackPID; // 3 plane PID score
double MeandEdX_Plane0,MeandEdX_Plane1,MeandEdX_Plane2,MeandEdX_ThreePlane; // Mean dE/dX scores
double Track_LLR_PID; // LLR PID
double Track_LLR_PID_Kaon; // LLR PID with Kaon hypothesis
double Track_LLR_PID_Kaon_Partial; // LLR PID with Kaon hypothesis using last 5cm of track
double Track_Bragg_PID_Kaon;
double ProtonMomentum,MuonMomentum,KaonMomentum; // Track kinematics
double TrackWiggliness;

// Truth info
bool HasTruth; // False if reco particle has no corresponding MC particle
int MCTruthIndex=-1;
int TrackTruePDG;
double TrackTrueE,TrackTruePx,TrackTruePy,TrackTruePz;
double TrackTrueEndE,TrackTrueEndPx,TrackTrueEndPy,TrackTrueEndPz;
double TrackTrueModMomentum;
double TrackTrueEndModMomentum;
double TrackTrueKE;
double TrackTrueEndKE;
double TrackTrueLength;
int TrackTrueOrigin; // 1 - primary , 2 - hyperon decay, 3 - other, 4 - kaon decay, 5 - Sigma0 decay
double TrackTruthPurity;
double TrackTruthCompleteness;
int NMatchedHits;

inline void SetVertex(TVector3 V);
inline void SetTrackPositions(TVector3 Start,TVector3 End);
inline void Print();

#ifdef __MAKE_ROOT_DICT__
ClassDef(RecoParticle,1);
#endif

};


inline void RecoParticle::SetVertex(TVector3 V){

   X = V.X();
   Y = V.Y();
   Z = V.Z();

}

inline void RecoParticle::SetTrackPositions(TVector3 Start,TVector3 End){

   TrackStartX = Start.X();
   TrackStartY = Start.Y();
   TrackStartZ = Start.Z();

   TrackEndX = End.X();
   TrackEndY = End.Y();
   TrackEndZ = End.Z();

}

inline void RecoParticle::Print(){

   std::cout << "Reco Info:" << std::endl;
   std::cout << "Self: " << Self << std::endl;
   std::cout << "InNuSlice: " << InNuSlice << std::endl;
   std::cout << "Key: " << Key << std::endl;
   std::cout << "TrackVectorIndex: " << TrackVectorIndex << std::endl;
   std::cout << "PDG Code: " << PDG << "  Track/Shower score: " << TrackShowerScore << std::endl;
   std::cout << "Parentage: " << Parentage << std::endl;
   std::cout << "X: " << X << ", Y: " << Y << ", Z: " << Z << std::endl;
   std::cout << "Displacement: " << Displacement << std::endl;
   std::cout << "Track length: " << TrackLength << "  PID score: " << TrackPID <<  std::endl;
   std::cout << "TrackDirectionX: " << TrackDirectionX << ", TrackDirectionY: " << TrackDirectionY << ", TrackDirectionZ: " << TrackDirectionZ << std::endl;
   std::cout << "MeandEdX_Plane0: " << MeandEdX_Plane0 << ", MeandEdX_Plane1: " << MeandEdX_Plane1 << ", MeandEdX_Plane2: " << MeandEdX_Plane2 << std::endl;
   std::cout << "MeandEdX_ThreePlane: " << MeandEdX_ThreePlane << std::endl;
   std::cout << "Track_LLR_PID: " << Track_LLR_PID << std::endl;
   std::cout << "Track_LLR_PID_Kaon: " << Track_LLR_PID_Kaon << std::endl;
   std::cout << "Track_LLR_PID_Kaon_Partial: " << Track_LLR_PID_Kaon_Partial << std::endl;
   std::cout << "Track_Bragg_PID_Kaon: " << Track_Bragg_PID_Kaon << std::endl;
   std::cout << "ProtonMomentum: " << ProtonMomentum << std::endl;
   std::cout << "MuonMomentum: " << MuonMomentum << std::endl;
   std::cout << "KaonMomentum: " << KaonMomentum << std::endl;
   std::cout << "TrackWiggliness: " << TrackWiggliness << std::endl;
   std::cout << "Truth Info:" << std::endl;
   std::cout << "HasTruth: " << HasTruth << std::endl;
   std::cout << "MCTruthIndex: " << MCTruthIndex << std::endl;
   std::cout << "PDG: " << TrackTruePDG << "  Origin: " << TrackTrueOrigin << std::endl;
   std::cout << "TrackTrueE: " << TrackTrueE << ", TrackTruePx: " << TrackTruePx << ", TrackTruePy: " << TrackTruePy << ", TrackTruePz: " << TrackTruePz << std::endl;
   std::cout << "TrackTrueEndE: " << TrackTrueEndE << ", TrackTrueEndPx: " << TrackTrueEndPx << ", TrackTrueEndPy: " << TrackTrueEndPy << ", TrackTrueEndPz: " << TrackTrueEndPz << std::endl;
   std::cout << "TrackTrueModMomentum: " << TrackTrueModMomentum << std::endl;
   std::cout << "TrackTrueKE: " << TrackTrueKE << ", TrackTrueEndKE: " << TrackTrueEndKE << std::endl;
   std::cout << "TrackTrueLength: " << TrackTrueLength << std::endl;
   std::cout << "TrackTruthPurity: " << TrackTruthPurity << std::endl;
   std::cout << "TrackTruthCompleteness: " << TrackTruthCompleteness << std::endl;
   std::cout << "NMatchedHits: " << NMatchedHits << std::endl;
}

#endif
