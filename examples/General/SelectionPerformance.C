R__LOAD_LIBRARY($HYP_TOP/lib/libHyperon.so)
R__LOAD_LIBRARY($HYP_TOP/lib/libParticleDict.so)

#include "TGraphAsymmErrors.h"

#include "SelectionManager.h"
#include "EventAssembler.h"
#include "Cut.h"
#include "SelectionParameters.h"
#include "Parameters.h"
#include "SampleSets_Example.h"

#include "BayesianPosteriorPDF.h"

bool CHEAT_SLICE_MODE = false;
bool FLASH_MATCHED_SLICE_MODE = true;
bool PANDORA_SLICE_MODE = false;
bool RECOVERY_MODE = false; // if flash matching didnt find anything.. then use the Pandora topological score
bool RUN_OVER_ALL_SLICE = false;

   // Produces graph with efficiency, purity and S/sqrt(S+B)
   // for each step of the selection and prints lots of
   // performance metrics.

void SelectionPerformance()
{
      std::cout << "\033[31m" << "CHEAT_SLICE_MODE? " << "\033[33m" << (CHEAT_SLICE_MODE ? "yeppo " : "noppo ") << "\033[0m" << std::endl;
      std::cout << "\033[31m" << "FLASH_MATCHED_SLICE_MODE? " << "\033[33m" << (FLASH_MATCHED_SLICE_MODE ? "yeppo " : "noppo ") << "\033[0m" << std::endl;
      std::cout << "\033[31m" << "PANDORA_SLICE_MODE? " << "\033[33m" << (PANDORA_SLICE_MODE ? "yeppo " : "noppo ") << "\033[0m" << std::endl;
      std::cout << "\033[31m" << "RECOVERY_MODE? " << "\033[33m" << (RECOVERY_MODE ? "yeppo " : "noppo ") << "\033[0m" << std::endl;
      std::cout << "\033[31m" << "RUN_OVER_ALL_SLICE? " << "\033[33m" << (RUN_OVER_ALL_SLICE ? "yeppo " : "noppo ") << "\033[0m" << std::endl;

      // Diables TEfficiency warnings
      gErrorIgnoreLevel = kWarning;

      double POT = 1.0e21; // POT to scale samples to

      BuildTunes();

      //SampleNames.push_back("GENIE Background");
      //SampleTypes.push_back("EXT");
      //SampleFiles.push_back("reco_stage_2_hist.root");

      SampleNames.push_back("GENIE Background");
      SampleTypes.push_back("Background");
      SampleFiles.push_back("run3b/BackgroundCV/hyperonNtuples_3b_Background_CV.root");


      SampleNames.push_back("GENIE Hyperon");
      SampleTypes.push_back("Hyperon");
      SampleFiles.push_back("run3b/HyperonCV/hyperonNtuples_3b_Hyperon_CV.root");

      SampleNames.push_back("GENIE Neutron");
      SampleTypes.push_back("Neutron");
      SampleFiles.push_back("run3b/NeutronCV/hyperonNtuples_3b_Neutron_CV.root");

      //SampleNames.push_back("GENIE Dirt");
      //SampleTypes.push_back("Dirt");
      //SampleFiles.push_back("run3b_RHC/analysisOutputRHC_Overlay_GENIE_Dirt_prodgenie_numi_uboone_overlay_dirt_rhc_mcc9_run3b_v28_sample0.root");

      //SampleNames.push_back("EXT");
      //SampleTypes.push_back("EXT");
      //SampleFiles.push_back("run3b_RHC/analysisOutputEXT_prod_extnumi_mcc9_v08_00_00_45_run3_run3b_reco2_all_reco2.root");

      SelectionParameters P = P_RHC_Tune_397_NoBDT;

      std::string label = "test";

      EventAssembler E(false);
      SelectionManager M(P);
      M.SetPOT(POT);
      /* M.ImportSelectorBDTWeights(P.p_SelectorBDT_WeightsDir); */
      //M.ImportAnalysisBDTWeights(P.p_AnalysisBDT_WeightsDir);

      TEfficiency* Eff = new TEfficiency("Eff","",9,-0.5,8.5);
      TEfficiency* Background_Acceptance = new TEfficiency("Background_Acceptance","",9,-0.5,8.5);

      // Sample Loop
      for (size_t i_s = 0; i_s < SampleNames.size(); i_s++)
      {
         E.SetFile(SampleFiles.at(i_s), SampleTypes.at(i_s));

         if(SampleTypes.at(i_s) != "EXT" && SampleTypes.at(i_s) != "Data") M.AddSample(SampleNames.at(i_s),SampleTypes.at(i_s),E.GetPOT());
         else if(SampleTypes.at(i_s) == "Data") M.AddSample(SampleNames.at(i_s),SampleTypes.at(i_s),Data_POT);
         else if(SampleTypes.at(i_s) == "EXT") M.AddSample(SampleNames.at(i_s),SampleTypes.at(i_s),EXT_POT);

         std::cout << "E.GetNEvents(): " << E.GetNEvents() << std::endl;

         // Event Loop
         for(int i = 0; i < E.GetNEvents(); i++)
         {
            if (i % 10000 == 0) std::cout << "Processing event " << i << "/" << E.GetNEvents() << std::endl;

            //std::cout << "1" << std::endl;
            Event e = E.GetEvent(i);
            //std::cout << "2" << std::endl;
            M.SetSignal(e);
            //std::cout << "3" << std::endl;
            M.AddEvent(e);
            //std::cout << "4" << std::endl;

            bool passed_FV=false,passed_Tracks=false,passed_Showers=false,passed_MuonID=false,passed_Selector=false,passed_Connectedness=false,passed_WCut=false,passed_AngleCut=false;

            for (unsigned int iSlice = 0; iSlice < e.SliceID.size(); ++iSlice)
            {
                /*
                std::cout << "iSlice: " << iSlice << std::endl;

                for (RecoParticle &recoParticle : e.TracklikePrimaryDaughters.at(iSlice))
                {
                    std::cout << "TrackParticle: " << std::endl;
                    std::cout << "------------------------------------------" << std::endl;
                    recoParticle.Print();
                    std::cout << "------------------------------------------" << std::endl;
                }


                for (RecoParticle &recoParticle : e.ShowerlikePrimaryDaughters.at(iSlice))
                {
                    std::cout << "ShowerParticle: " << std::endl;
                    std::cout << "------------------------------------------" << std::endl;
                    recoParticle.Print();
                    std::cout << "------------------------------------------" << std::endl;
                }
                */


                if (CHEAT_SLICE_MODE && (e.SliceID.at(iSlice) != e.TrueNuSliceID))
                    continue;

                if (FLASH_MATCHED_SLICE_MODE && (e.SliceID.at(iSlice) != e.FlashMatchedNuSliceID))
                    continue;

                if (PANDORA_SLICE_MODE && (e.SliceID.at(iSlice) != e.PandoraNuSliceID))
                    continue;

                passed_FV = M.FiducialVolumeCut(e, iSlice);
                if(passed_FV) passed_Tracks = M.TrackCut(e, iSlice);
                if(passed_Tracks) passed_Showers = M.ShowerCut(e, iSlice);
                if(passed_Showers) passed_MuonID = M.ChooseMuonCandidate(e, iSlice);
            /* if(passed_MuonID) passed_Selector = M.ChooseProtonPionCandidates(e); */
            /* if(passed_Selector) passed_Connectedness = M.ConnectednessTest(e); */
            /* if(passed_Connectedness) passed_WCut = M.WCut(e); */
            /* if(passed_WCut) passed_AngleCut = M.AngleCut(e); */

                
            if(e.EventIsSignalSigmaZero){
               Eff->FillWeighted(true,e.Weight,0);
               Eff->FillWeighted(passed_FV,e.Weight,1);
               //Eff->FillWeighted(passed_Tracks,e.Weight,2);
               //Eff->FillWeighted(passed_Showers,e.Weight,3);
               //Eff->FillWeighted(passed_MuonID,e.Weight,4);
               /* Eff->FillWeighted(passed_Selector,e.Weight,5); */
               /* Eff->FillWeighted(passed_Connectedness,e.Weight,6); */
               /* Eff->FillWeighted(passed_WCut,e.Weight,7); */
               /* Eff->FillWeighted(passed_AngleCut,e.Weight,8); */
            }
            else {
               Background_Acceptance->FillWeighted(true,e.Weight,0);
               Background_Acceptance->FillWeighted(passed_FV,e.Weight,1);
               //Background_Acceptance->FillWeighted(passed_Tracks,e.Weight,2);
               //Background_Acceptance->FillWeighted(passed_Showers,e.Weight,3);
               //Background_Acceptance->FillWeighted(passed_MuonID,e.Weight,4);
               /* Background_Acceptance->FillWeighted(passed_Selector,e.Weight,5); */
               /* Background_Acceptance->FillWeighted(passed_Connectedness,e.Weight,6); */
               /* Background_Acceptance->FillWeighted(passed_WCut,e.Weight,7); */
               /* Background_Acceptance->FillWeighted(passed_AngleCut,e.Weight,8); */
            }
            }
         }
         E.Close();
      }

      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      std::cout << std::endl << std::endl << "Trying New Estimators" << std::endl;

      TCanvas *c2 = new TCanvas("c","c");

      TH1D *h_PassedSignal = (TH1D*)Eff->GetPassedHistogram();
      TH1D *h_TotalSignal = (TH1D*)Eff->GetTotalHistogram();
      TH1D *h_PassedBackground = (TH1D*)Background_Acceptance->GetPassedHistogram();
      TH1D *h_TotalBackground = (TH1D*)Background_Acceptance->GetTotalHistogram();

      Eff->SetStatisticOption(TEfficiency::kBUniform);
      Background_Acceptance->SetStatisticOption(TEfficiency::kBUniform);
      Background_Acceptance->SetPosteriorMode();

      for(int i_c=1;i_c<h_PassedSignal->GetNbinsX()+1;i_c++){
         std::cout << std::endl << "Cut Number " << i_c << std::endl;
         std::cout << "Total Signal = " << h_TotalSignal->GetBinContent(i_c) << "  Signal Passed = " << h_PassedSignal->GetBinContent(i_c) << std::endl;
         std::cout << "Total Background = " << h_TotalBackground->GetBinContent(i_c) << "  Background Passed = " << h_PassedBackground->GetBinContent(i_c) << std::endl;
         std::cout << "Signal Efficiency = " << Eff->GetEfficiency(i_c) << " + " << Eff->GetEfficiencyErrorUp(i_c) << " - " << Eff->GetEfficiencyErrorLow(i_c) << std::endl;
         std::cout << "Background Acceptance = " << Background_Acceptance->GetEfficiency(i_c) << " + " << Background_Acceptance->GetEfficiencyErrorUp(i_c) << " - " << Background_Acceptance->GetEfficiencyErrorLow(i_c) << std::endl;

         std::cout << "Selected Signal = " << h_TotalSignal->GetBinContent(i_c)*Eff->GetEfficiency(i_c)  << " + " << h_TotalSignal->GetBinContent(i_c)*Eff->GetEfficiencyErrorUp(i_c) << " - " << h_TotalSignal->GetBinContent(i_c)*Eff->GetEfficiencyErrorLow(i_c) << std::endl;
         std::cout << "Selected Background = " << h_TotalBackground->GetBinContent(i_c)*Background_Acceptance->GetEfficiency(i_c) << " + " << h_TotalBackground->GetBinContent(i_c)*Background_Acceptance->GetEfficiencyErrorUp(i_c) << " - " << h_TotalBackground->GetBinContent(i_c)*Background_Acceptance->GetEfficiencyErrorLow(i_c) << std::endl;
      }
      std::cout << std::endl << std::endl;

      gSystem->Exec("mkdir -p rootfiles");
      TFile *f_out = TFile::Open(("rootfiles/Expected_Events_" + label + ".root").c_str(),"RECREATE");

      TH1D *h_ExpectedSignal = PosteriorPDF(Eff,9,"Signal");
      TH1D *h_ExpectedBackground = PosteriorPDF(Background_Acceptance,9,"Background");

      h_ExpectedSignal->Write();
      h_ExpectedBackground->Write();

      f_out->Close();

   }
