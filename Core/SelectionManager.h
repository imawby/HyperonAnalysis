#ifndef _SelectionManager_h_
#define _SelectionManager_h_

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TH1D.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TMVA/Reader.h"
#include "TLegend.h"
#include "TPad.h"
#include "TGraph.h"
#include <TROOT.h>
#include <TStyle.h>
#include "TMatrix.h"

// Local includes
#include "FluxWeight2.h"
#include "GenG4WeightHandler2.h"
#include "FiducialVolume.h"
#include "Muon_ID.h"
#include "TrackLengthCutManager.h"
#include "SelectionParameters.h"
#include "Event.h"
#include "RecoParticle.h"
#include "SimParticle.h"
#include "Cut.h"
#include "Misc.h"
#include "SelectorBDTManager.h"
#include "AnalysisBDTManager.h"
#include "PlottingFunctions.h"
#include "EventListFilter.h"
#include "CTTest.h"
#include "SystematicsHeader.h"
#include "EventType.h"

enum Generators {kGENIE,kNuWro};

class SelectionManager {

   // General variables/functions //

   public:

      // Constructors/Destructors
      SelectionManager();
      ~SelectionManager();
      SelectionManager(SelectionParameters p);

      void Close();

      // Start processing a sample
      void AddSample(std::string Name,std::string Type,double SamplePOT,std::string EventList="");

      // Increment metadata
      void AddEvent(Event &e);  
      void SetSignal(Event &e);

      // Set POT
      void SetPOT(double pot){ POT = pot; }

      // Beam Mode
      int BeamMode = kFHC;
      int Run = 1;
      void SetBeamMode(int mode);
      void SetRun(int run){ Run = run; }

      void ImportSelectorBDTWeights(std::string WeightDir="");
      void ImportAnalysisBDTWeights(std::string WeightDir="");

      SelectionParameters GetParams();

   private: 

      // Stores the parameters controlling the selection
      SelectionParameters TheParams;

      // POT to scale samples to
      double POT;

      // List of allowed sample types
      const std::vector<std::string> SampleTypes = { "Hyperon" , "Background" , "Dirt" , "EXT" , "Data" };

      // Parameters of current sample being processed
      double thisSampleWeight;
      std::string thisSampleName;
      std::string thisSampleType;
      int thisSampleGenerator;

      // Flux and Generator/G4 weight calculators
      FluxWeighter a_FluxWeightCalc;
      GenG4WeightHandler a_GenG4WeightCalc;

      // Selection Algs
      FV::FiducialVolume a_FiducialVolume;
      MuonID a_MuonID;
      TrackLengthCutManager a_TrackLengthCutManager;
      SelectorBDTManager a_SelectorBDTManager;
      AnalysisBDTManager a_AnalysisBDTManager;
      EventListFilter a_EventListFilter;
      SecondaryVertexFitter a_SecondaryVertexFitter;
      CTTest a_CTTest_Plane0;
      CTTest a_CTTest_Plane1;
      CTTest a_CTTest_Plane2;

      // Misc settings/methods //
   private:

      bool fUseFluxWeight = true;
      bool fUseGenWeight = true;

   public:

      void UseFluxWeight(bool usefluxweight);
      void UseGenWeight(bool usegenweight); 

   private:
      //
      // Cut Data Management //
      const std::vector<std::string> CutTypes = { "FV" , "Tracks" , "Showers" , "MuonID" , "SubleadingTracks" , "DecaySelector" , "DecayAnalysis" , "Connectedness" , "InvariantMass" , "AlphaAngle" };
      void DeclareCuts();
      std::vector<Cut> Cuts;
      void UpdateCut(const Event &e,bool Passed,std::string CutName);

   public:

      Cut GetCut(std::string CutName);
      void Reset();

      // Cut Functions //

   public:

      bool FiducialVolumeCut(const Event &e, const int sliceIndex);
      bool TrackCut(const Event &e, const int sliceIndex);
      bool ShowerCut(const Event &e, const int sliceIndex);
      bool ChooseMuonCandidate(Event &e, const int sliceIndex);
      bool TrackLengthCut(const Event &e, const int sliceIndex);
      bool ChooseProtonPionCandidates(Event &e, const int sliceIndex, bool cheat=false);
      bool AnalysisBDTCut(Event &e, const int sliceIndex);
      bool EventListCut(const Event &e);
      bool ConnectednessTest(const Event &e, const int sliceIndex, int nplanes=1);
      bool WCut(const Event &e, const int sliceIndex);
      bool AngleCut(const Event &e, const int sliceIndex);

      // Histogram Functions //

   private:

      std::string fTitle;
      int fHistNBins;
      double fHistLow;
      double fHistHigh;
      std::vector<double> fHistBoundaries;

      bool fHasData=false;
      bool fUseText=false;

      TH1D* Hist_Data=nullptr;
      TH1D* Hist_All=nullptr;
      std::map<std::string,TH1D*> Hists_ByProc;
      std::map<std::string,TH1D*> Hists_ByType;
      std::map<std::string,TH1D*> Hists_ByType2;

      std::map<std::string,std::vector<TH1D*>> Multisim_Sys_Hists_All;
      std::map<std::string,std::vector<TH1D*>> SingleUnisim_Sys_Hists_All;
      std::map<std::string,std::vector<TH1D*>> DualUnisim_Sys_Hists_All;
      std::map<std::string,std::map<std::string,std::vector<TH1D*>>> Multisim_Sys_Hists; // histogram in different multisim universes
      std::map<std::string,std::map<std::string,std::vector<TH1D*>>> SingleUnisim_Sys_Hists; // histogram in different single univese unisims
      std::map<std::string,std::map<std::string,std::vector<TH1D*>>> DualUnisim_Sys_Hists;  // histogram in different two universe multisims

      TFile *f_Hists = nullptr;
      void OpenHistFile(std::string label="Hists");

      std::vector<std::string> BinLabels;

   public:

      void AddSystematic(int systype,int universes,std::string name);
      void FillHistogramsSys(const Event &e,double variable,std::string name,std::vector<double> weights);
      void FillHistogramsSys(const Event &e,double variable,std::string name,int universe,double weight);
      void DrawHistogramsSys(std::string label,std::string name,std::string type="All");
      TMatrixD GetCovarianceMatrix(std::string label,std::string name,std::string type="All");

      void SetupHistograms(int n,double low,double high,std::string title="");
      void SetupHistograms(std::vector<double> boundaries,std::string title="");
      void FillHistograms(const Event &e,double variable,double weight=1.0);
      void DrawHistograms(std::string label="Hists",double Scale=1.0,double SignalScale=1.0);

      double GetPrediction(int bin,std::string type="");

      std::string PlotDir="Plots/";
      std::string RootfileDir="rootfiles/";

      void SetBinLabels(std::vector<std::string> binlabels);
      std::vector<double> GetBinBoundaries();

      void WidthScaleHistograms();

      void UseText(){ fUseText = true; }
};

#endif
