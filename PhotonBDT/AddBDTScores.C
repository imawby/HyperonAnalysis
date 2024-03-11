#include "RangeVariables.C"

void AddBDTScores()
{
    /*
    TFile * file = new TFile("/uboone/data/users/imawby/PhotonBDT/photonBDTFilesTruncated.root", "UPDATE");
    TTree * signalTree = (TTree*)file->Get("PhotonTree");
    TTree * backgroundTree = (TTree*)file->Get("BackgroundTree");
    */

    TFile * file = new TFile("/uboone/data/users/imawby/PhotonBDT/photonBDTFiles.root", "UPDATE");
    //TFile * file = new TFile("/uboone/app/users/imawby/larsoft_olderVersion/junk/reco_stage_2_hist.root", "UPDATE");

    TTree * signalTree = (TTree*)file->Get("photonBDT/PhotonTree");
    TTree * backgroundTree = (TTree*)file->Get("photonBDT/BackgroundTree");
    //TTree * signalTree = (TTree*)file->Get("visualiseSlice/VisualisationTree");
    //TTree * backgroundTree = (TTree*)file->Get("visualiseSlice/VisualisationTree");

    // Add new branch to 'selection' tree...
    float tmvaOutput;
    TBranch * photonBDTBranch_signal = signalTree->Branch("PhotonBDTScore", &tmvaOutput);
    TBranch * photonBDTBranch_background = backgroundTree->Branch("PhotonBDTScore", &tmvaOutput);

    // Now hook up TMVA
    float nuVertexSeparation_tmva;
    float dca_tmva;
    float nuVertexChargeDist_tmva;
    float initialdEdx_tmva;
    float trackShowerScore_tmva;
    float openingAngle_tmva;
    float showerLength_tmva;
    float trackParentSep_tmva;
    float chiPIDProton_tmva;
    float braggPIDMuon_tmva;
    float LLRPIDReduced_tmva;

    TMVA::Reader * tmvaReader = new TMVA::Reader();
    tmvaReader->AddVariable("NuVertexSeparation", &nuVertexSeparation_tmva);
    tmvaReader->AddVariable("DCA", &dca_tmva);
    tmvaReader->AddVariable("NuVertexChargeDistribution", &nuVertexChargeDist_tmva);
    tmvaReader->AddVariable("InitialdEdx", &initialdEdx_tmva);
    tmvaReader->AddVariable("TrackShowerScore", &trackShowerScore_tmva);
    tmvaReader->AddVariable("ShowerOpeningAngle", &openingAngle_tmva);
    tmvaReader->AddVariable("ShowerLength", &showerLength_tmva);
    tmvaReader->AddVariable("TrackParentSeparation", &trackParentSep_tmva);
    tmvaReader->AddVariable("ChiPIDProton", &chiPIDProton_tmva);
    tmvaReader->AddVariable("BraggPIDMuon", &braggPIDMuon_tmva);
    tmvaReader->AddVariable("LLRPIDReduced", &LLRPIDReduced_tmva);

    std::string weightFileName = "/uboone/app/users/imawby/HyperonAnalysis/PhotonBDT/training/dataset_photonBDT/weights/TMVAClassification_BDTG.weights.xml"; 
    tmvaReader->BookMVA("BDTG",  weightFileName.c_str());

    int count = 0;
    for (TTree * tree : {signalTree, backgroundTree})
    {
        // Hook up to the 'selection' tree
        double nuVertexSeparation_tree;
        double dca_tree;
        double nuVertexChargeDist_tree;
        double initialdEdx_tree;
        double trackShowerScore_tree;
        double openingAngle_tree;
        double showerLength_tree;
        double trackParentSep_tree;
        double chiPIDProton_tree;
        double braggPIDMuon_tree;
        double LLRPIDReduced_tree;

        tree->SetBranchAddress("NuVertexSeparation", &nuVertexSeparation_tree); 
        tree->SetBranchAddress("DCA", &dca_tree);
        tree->SetBranchAddress("NuVertexChargeDistribution", &nuVertexChargeDist_tree); 
        tree->SetBranchAddress("InitialdEdx", &initialdEdx_tree); 
        tree->SetBranchAddress("TrackShowerScore", &trackShowerScore_tree); 
        tree->SetBranchAddress("ShowerOpeningAngle", &openingAngle_tree); 
        tree->SetBranchAddress("ShowerLength", &showerLength_tree); 
        tree->SetBranchAddress("TrackParentSeparation", &trackParentSep_tree); 
        tree->SetBranchAddress("ChiPIDProton", &chiPIDProton_tree);
        tree->SetBranchAddress("BraggPIDMuon", &braggPIDMuon_tree);
        tree->SetBranchAddress("LLRPIDReduced", &LLRPIDReduced_tree);

        for (unsigned int i = 0 ; i < tree->GetEntries(); ++i)
        {
            tree->GetEntry(i);

            nuVertexSeparation_tmva = RangeNuVertexSeparation(nuVertexSeparation_tree);
            dca_tmva = RangeDCA(dca_tree);
            nuVertexChargeDist_tmva = RangeNuVertexChargeDistribution(nuVertexChargeDist_tree);
            initialdEdx_tmva = RangeInitialdEdx(initialdEdx_tree);
            trackShowerScore_tmva = RangeTrackScore(trackShowerScore_tree);
            openingAngle_tmva = RangeOpeningAngle(openingAngle_tree);
            showerLength_tmva = RangeShowerLength(showerLength_tree);
            trackParentSep_tmva = RangeTrackParentSeparation(trackParentSep_tree);
            chiPIDProton_tmva = RangeChiPIDProton(chiPIDProton_tree);
            braggPIDMuon_tmva = RangeBraggPIDMuon(braggPIDMuon_tree);
            LLRPIDReduced_tmva = RangeLLRPID(LLRPIDReduced_tree);

            tmvaOutput = tmvaReader->EvaluateMVA("BDTG");

            count == 0 ? photonBDTBranch_signal->Fill() : photonBDTBranch_background->Fill();
        }

        ++count;
    }

    file->cd();
    file->cd("photonBDT");
    //file->cd("visualiseSlice");

    signalTree->Write("", TObject::kOverwrite);
    backgroundTree->Write("", TObject::kOverwrite);
}
