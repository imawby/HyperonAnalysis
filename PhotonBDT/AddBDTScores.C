void AddBDTScores()
{
    /*
    TFile * file = new TFile("/uboone/data/users/imawby/PhotonBDT/photonBDTFilesTruncated.root", "UPDATE");
    TTree * signalTree = (TTree*)file->Get("PhotonTree");
    TTree * backgroundTree = (TTree*)file->Get("BackgroundTree");
    */

    TFile * file = new TFile("/uboone/data/users/imawby/PhotonBDT/photonBDTFiles.root", "UPDATE");

    TTree * signalTree = (TTree*)file->Get("photonBDT/PhotonTree");
    TTree * backgroundTree = (TTree*)file->Get("photonBDT/BackgroundTree");

    // Add new branch to 'selection' tree...
    float tmvaOutput;
    TBranch * photonBDTBranch_signal = signalTree->Branch("PhotonBDTScore", &tmvaOutput);
    TBranch * photonBDTBranch_background = backgroundTree->Branch("PhotonBDTScore", &tmvaOutput);

    // Got to shift the vars into the TMVA range...
    int nHits3DMin = -1, nHits2DMin = -1;
    int nHits3DMax = 300, nHits2DMax = 300;

    double nuVertexSeparationMin = -1.0, nuVertexChargeDistMin = -1.0, initialdEdxMin = -1.0, trackScoreMin = -1.0, openingAngleMin = -1.0, showerLengthMin = -1.0, trackParentSepMin = -1.0;
    double nuVertexSeparationMax = 100.0, nuVertexChargeDistMax = 10.0, initialdEdxMax = 10.0, trackScoreMax = 1.0, openingAngleMax = 50.0, showerLengthMax = 200.0, trackParentSepMax = 2.0;    // For TMVA...

    // Now hook up TMVA
    float nHits3D_tmva;
    float nHits2D_tmva;
    float nuVertexSeparation_tmva;
    float nuVertexChargeDist_tmva;
    float initialdEdx_tmva;
    float trackShowerScore_tmva;
    float openingAngle_tmva;
    float showerLength_tmva;
    float trackParentSep_tmva;

    TMVA::Reader * tmvaReader = new TMVA::Reader();
    tmvaReader->AddVariable("NHits3D", &nHits3D_tmva);
    tmvaReader->AddVariable("NHits2D", &nHits2D_tmva);
    tmvaReader->AddVariable("NuVertexSeparation", &nuVertexSeparation_tmva);
    tmvaReader->AddVariable("NuVertexChargeDistribution", &nuVertexChargeDist_tmva);
    tmvaReader->AddVariable("InitialdEdx", &initialdEdx_tmva);
    tmvaReader->AddVariable("TrackShowerScore", &trackShowerScore_tmva);
    tmvaReader->AddVariable("ShowerOpeningAngle", &openingAngle_tmva);
    tmvaReader->AddVariable("ShowerLength", &showerLength_tmva);
    tmvaReader->AddVariable("TrackParentSeparation", &trackParentSep_tmva);
    std::string weightFileName = "/uboone/app/users/imawby/HyperonAnalysis/PhotonBDT/dataset_photonBDT/weights/TMVAClassification_BDTG.weights.xml"; 
    tmvaReader->BookMVA("BDTG",  weightFileName.c_str());

    int count = 0;
    for (TTree * tree : {signalTree, backgroundTree})
    {
        // Hook up to the 'selection' tree
        int nHits3D_tree;
        int nHits2D_tree;
        double nuVertexSeparation_tree;
        double nuVertexChargeDist_tree;
        double initialdEdx_tree;
        double trackShowerScore_tree;
        double openingAngle_tree;
        double showerLength_tree;
        double trackParentSep_tree;

        tree->SetBranchAddress("NHits3D", &nHits3D_tree); 
        tree->SetBranchAddress("NHits2D", &nHits2D_tree); 
        tree->SetBranchAddress("NuVertexSeparation", &nuVertexSeparation_tree); 
        tree->SetBranchAddress("NuVertexChargeDistribution", &nuVertexChargeDist_tree); 
        tree->SetBranchAddress("InitialdEdx", &initialdEdx_tree); 
        tree->SetBranchAddress("TrackShowerScore", &trackShowerScore_tree); 
        tree->SetBranchAddress("ShowerOpeningAngle", &openingAngle_tree); 
        tree->SetBranchAddress("ShowerLength", &showerLength_tree); 
        tree->SetBranchAddress("TrackParentSeparation", &trackParentSep_tree); 

        for (unsigned int i = 0 ; i < tree->GetEntries(); ++i)
        {
            tree->GetEntry(i);

            nHits3D_tmva = std::max(std::min(nHits3D_tree, nHits3DMax), nHits3DMin);
            nHits2D_tmva = std::max(std::min(nHits2D_tree, nHits2DMax), nHits2DMin);
            nuVertexSeparation_tmva = std::max(std::min(nuVertexSeparation_tree, nuVertexSeparationMax), nuVertexSeparationMin);
            nuVertexChargeDist_tmva = std::max(std::min(nuVertexChargeDist_tree, nuVertexChargeDistMax), nuVertexChargeDistMin);
            initialdEdx_tmva = std::max(std::min(initialdEdx_tree, initialdEdxMax), initialdEdxMin);
            trackShowerScore_tmva = std::max(std::min(trackShowerScore_tree, trackScoreMax), trackScoreMin);
            openingAngle_tmva = std::max(std::min(openingAngle_tree, openingAngleMax), openingAngleMin);
            showerLength_tmva = std::max(std::min(showerLength_tree, showerLengthMax), showerLengthMin);
            trackParentSep_tmva = std::max(std::min(trackParentSep_tree, trackParentSepMax), trackParentSepMin);

            tmvaOutput = tmvaReader->EvaluateMVA("BDTG");

            count == 0 ? photonBDTBranch_signal->Fill() : photonBDTBranch_background->Fill();
        }

        ++count;
    }

    file->cd();
    file->cd("photonBDT");

    signalTree->Write("", TObject::kOverwrite);
    backgroundTree->Write("", TObject::kOverwrite);
}
