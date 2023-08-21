#ifndef _SelectorBDTManager_cxx_
#define _SelectorBDTManager_cxx_

#include "SelectorBDTManager.h"

///////////////////////////////////////////////////////////////////////////////////////////////

SelectorBDTManager::SelectorBDTManager(){

   fMode = "Test";
   fProtonPIDCut = 0.1;
   fPionPIDCut = -0.1;
   fSeparationCut = 4.0;
}

///////////////////////////////////////////////////////////////////////////////////////////////

SelectorBDTManager::SelectorBDTManager(std::string Mode){

   fMode = Mode;
   if(fMode == "Train") SetupTrainingTrees();    
   fProtonPIDCut = 0.1;
   fPionPIDCut = -0.1;
   fSeparationCut = 4.0;

}

///////////////////////////////////////////////////////////////////////////////////////////////

SelectorBDTManager::~SelectorBDTManager(){

   if(fMode == "Train"){
      f_Trees->Close();
      delete f_Trees;
   }
   else if(fMode == "Test"){
      if(reader != nullptr)  delete reader;
   }

}

///////////////////////////////////////////////////////////////////////////////////////////////

void SelectorBDTManager::SetCuts(double ProtonPIDCut,double PionPIDCut,double SelectionCut){

   fProtonPIDCut = ProtonPIDCut;
   fPionPIDCut =  PionPIDCut;
   fSeparationCut = SelectionCut;

}

///////////////////////////////////////////////////////////////////////////////////////////////

void SelectorBDTManager::WriteTrainingTrees(){

   std::cout << "Entries in Signal tree = " << t_Signal->GetEntries() << std::endl;
   std::cout << "Entries in Background tree = " << t_Background->GetEntries() << std::endl;

   f_Trees->cd();

   t_Signal->Write("SignalTree");
   t_Background->Write("BackgroundTree");

}   

///////////////////////////////////////////////////////////////////////////////////////////////

void SelectorBDTManager::SetupTrainingTrees(){

   system("mkdir -p SelectorMVA");

   f_Trees = TFile::Open( "SelectorMVA/Trees.root" , "RECREATE" );
   t_Signal = new TTree("t_SelectorMVA_Signal","Signal Pairings");
   t_Background = new TTree("t_SelectorMVA_Background","Background Pairings");

   t_Signal->Branch("separation",&v_separation);
   t_Signal->Branch("proton_trkscore",&v_proton_trkscore);
   t_Signal->Branch("pion_trkscore",&v_pion_trkscore);
   t_Signal->Branch("proton_dEdX",&v_proton_dEdX);
   t_Signal->Branch("pion_dEdX",&v_pion_dEdX);
   t_Signal->Branch("proton_LLR",&v_proton_LLR);
   t_Signal->Branch("pion_LLR",&v_pion_LLR);

   t_Background->Branch("separation",&v_separation);
   t_Background->Branch("proton_trkscore",&v_proton_trkscore);
   t_Background->Branch("pion_trkscore",&v_pion_trkscore);
   t_Background->Branch("proton_dEdX",&v_proton_dEdX);
   t_Background->Branch("pion_dEdX",&v_pion_dEdX);
   t_Background->Branch("proton_LLR",&v_proton_LLR);
   t_Background->Branch("pion_LLR",&v_pion_LLR);

}

///////////////////////////////////////////////////////////////////////////////////////////////

void SelectorBDTManager::FillTree(const Event &/*e*/){

   // Check in the right running mode first
   assert(fMode == "Train");

   // Only train this alg on events with real reconstructed decays
   std::cout << "ISOBEL GOOD RECO ISNT IMPLEMENTED" << std::endl;
   throw;
   //if(!e.GoodReco) return;
   /*
   if( t_Signal == nullptr || t_Background == nullptr ){ std::cout << "Trees not setup, exiting" << std::endl; return; }

   // Sort through list of candiadate tracks
   for(size_t i_tr=0;i_tr<e.TracklikePrimaryDaughters.size();i_tr++){
      for(size_t j_tr=0;j_tr<e.TracklikePrimaryDaughters.size();j_tr++){

         if(i_tr == j_tr) continue;

         // Returns false if tracks fail PID/separation cuts     
         if(!SetVariables(e.TracklikePrimaryDaughters.at(i_tr),e.TracklikePrimaryDaughters.at(j_tr))) continue;

         // If these tracks are the correct pair of decay tracks, fill signal tree 

         if(e.GoodReco && e.TracklikePrimaryDaughters.at(i_tr).TrackVectorIndex == e.TrueDecayProtonIndex && e.TracklikePrimaryDaughters.at(j_tr).TrackVectorIndex == e.TrueDecayPionIndex)
            t_Signal->Fill();         
         else t_Background->Fill();

      }
   }
*/
}

///////////////////////////////////////////////////////////////////////////////////////////////

bool SelectorBDTManager::SetVariables(const RecoParticle &thisProtonCandidate, const RecoParticle &thisPionCandidate){

   TVector3 ProtonPosition(thisProtonCandidate.TrackStartX,thisProtonCandidate.TrackStartY,thisProtonCandidate.TrackStartZ);
   TVector3 PionPosition(thisPionCandidate.TrackStartX,thisPionCandidate.TrackStartY,thisPionCandidate.TrackStartZ);

   v_separation = Limit((ProtonPosition-PionPosition).Mag(),separation_limits);
   v_proton_trkscore = Limit(thisProtonCandidate.TrackShowerScore,trkscore_limits);
   v_pion_trkscore = Limit(thisPionCandidate.TrackShowerScore,trkscore_limits);

   // Catch default dEdX fills
   if(thisProtonCandidate.MeandEdX_ThreePlane < 0 || thisPionCandidate.MeandEdX_ThreePlane < 0) return false;
   if(std::isnan(thisProtonCandidate.MeandEdX_ThreePlane) || std::isnan(thisPionCandidate.MeandEdX_ThreePlane)) return false;

   v_proton_dEdX = Limit(thisProtonCandidate.MeandEdX_ThreePlane,dEdX_limits);
   v_pion_dEdX = Limit(thisPionCandidate.MeandEdX_ThreePlane,dEdX_limits);

   v_proton_LLR = Limit(thisProtonCandidate.Track_LLR_PID,LLR_limits);
   v_pion_LLR = Limit(thisPionCandidate.Track_LLR_PID,LLR_limits);

   // Proton PID Cut
   if(v_pion_LLR < fPionPIDCut) return false;

   // Pion PID Cut
   if(v_proton_LLR > fProtonPIDCut) return false;

   // Separation
   if(v_separation > fSeparationCut) return false;

   return true;

}

///////////////////////////////////////////////////////////////////////////////////////////////

void SelectorBDTManager::SetupSelectorBDT(std::string WeightsDir,std::string alg){

   assert(fMode == "Test");

   if(WeightsDir == ""){ 
      std::cout << "No weights directory given, assuming default location" << std::endl;
      fWeightsDir = "/home/lar/cthorpe/uboone/HyperonSelection/TMVA/SelectorMVA/v1/dataset/weights";
   }
   else fWeightsDir = WeightsDir;  


   TMVA::Tools::Instance();
   reader = new TMVA::Reader( "!Color:!Silent" );

   reader->AddVariable("separation",&v_separation);
   reader->AddVariable("proton_trkscore",&v_proton_trkscore);
   reader->AddVariable("pion_trkscore",&v_pion_trkscore);
   reader->AddVariable("proton_dEdX",&v_proton_dEdX);
   reader->AddVariable("pion_dEdX",&v_pion_dEdX);
   reader->AddVariable("proton_LLR",&v_proton_LLR);
   reader->AddVariable("pion_LLR",&v_pion_LLR);

   std::map<std::string,int> Use;
   for(size_t i_a=0;i_a<Algs_str.size();i_a++)
      Use[Algs_str.at(i_a)] = 1;

   TString prefix = "TMVAClassification";

   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = TString(it->first) + TString(" method");
         TString weightfile = fWeightsDir + "/" + prefix + TString("_") + TString(it->first) + TString(".weights.xml");
         std::cout << "Opening weight file " << weightfile << std::endl; 
         reader->BookMVA( methodName, weightfile );
      }
   }

}

///////////////////////////////////////////////////////////////////////////////////////////////

void SelectorBDTManager::SetAlg(std::string alg){

   bool found_alg = false;
   for(size_t i_a=0;i_a<Algs_str.size();i_a++)
      if(alg == Algs_str.at(i_a)) found_alg = true;

   if(!found_alg) throw std::invalid_argument("SelectorBDTManager: Classifier " + alg + " not found, exiting");

   Alg = alg;

}

///////////////////////////////////////////////////////////////////////////////////////////////

std::pair<int,int> SelectorBDTManager::NominateTracks(const std::vector<RecoParticle> &tracklikePrimaryChildren,
   double &bdtScore)
{
   int i_proton_candidate=-1;
   int i_pion_candidate=-1;

   Float_t BDT_Best = -1e10;

   //sort through list of candiadate tracks
   for(size_t i_tr = 0; i_tr < tracklikePrimaryChildren.size(); i_tr++)
   {
      for(size_t j_tr=0; j_tr < tracklikePrimaryChildren.size(); j_tr++)
      {
         if(i_tr == j_tr) continue;

         // Returns false if tracks fail PID/separation cuts     
         if(!SetVariables(tracklikePrimaryChildren.at(i_tr), tracklikePrimaryChildren.at(j_tr))) 
             continue;

         //calculate BDT Score
         //double BDT_Score = reader->EvaluateMVA("BDT method");
         double thisBDTScore = reader->EvaluateMVA(Alg + " method");

         if (thisBDTScore > BDT_Best)
         {
            i_proton_candidate = i_tr;
            i_pion_candidate = j_tr;
            BDT_Best = thisBDTScore;
         }
      }
   }

   bdtScore = BDT_Best;

   return std::make_pair(i_proton_candidate,i_pion_candidate);
}

///////////////////////////////////////////////////////////////////////////////////////////////

std::pair<int,int> SelectorBDTManager::NominateTracksCheat(const std::vector<RecoParticle> &tracklikePrimaryChildren,
    double &bdtScore)
{
    std::cout << "ISOBEL THIS HASN'T BEEN IMPLEMENTED YET" << std::endl;

    throw;

    return {-1,-1};

   // Use truth information to choose the proton/pion candidates
    /*
   if(!e.GoodReco) return {-1,-1};

   int i_proton_candidate=-1;
   int i_pion_candidate=-1;

   for(size_t i_tr=0;i_tr<e.TracklikePrimaryDaughters.size();i_tr++){
      if(e.TracklikePrimaryDaughters.at(i_tr).Index == e.TrueDecayProtonIndex) i_proton_candidate = i_tr;
      if(e.TracklikePrimaryDaughters.at(i_tr).Index == e.TrueDecayPionIndex) i_pion_candidate = i_tr;
   }

   if(i_proton_candidate == -1 || i_pion_candidate == -1) return {-1,-1};

   //if(!SetVariables(e.TracklikePrimaryDaughters.at(i_proton_candidate),e.TracklikePrimaryDaughters.at(i_pion_candidate))) return {-1,-1};

   //e.SelectorBDTScore = reader->EvaluateMVA("BDT method"); 
   e.SelectorBDTScore = reader->EvaluateMVA(Alg + " method"); 

   return std::make_pair(i_proton_candidate,i_pion_candidate);
    */
}

///////////////////////////////////////////////////////////////////////////////////////////////

double SelectorBDTManager::GetScore(RecoParticle DecayProtonCandidate,RecoParticle DecayPionCandidate){

   if(!SetVariables(DecayProtonCandidate,DecayPionCandidate)) return -1000;

   return reader->EvaluateMVA(Alg+ " method");
}

///////////////////////////////////////////////////////////////////////////////////////////////

#endif
