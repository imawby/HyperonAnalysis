double COMPLETENESS_THRESHOLD = 0.0;
double PURITY_THRESHOLD = 0.0;

void PhotonTreeTruncator()
{
    TFile * originalFile = new TFile("/uboone/data/users/imawby/PhotonBDT/photonBDTFiles.root", "READ");
    TTree * originalSignalTree = (TTree*)originalFile->Get("photonBDT/PhotonTree");
    TTree * originalBackgroundTree = (TTree*)originalFile->Get("photonBDT/BackgroundTree");
    //originalSignalTree->SetDirectory(0);
    //originalBackgroundTree->SetDirectory(0);
    //originalFile->Close();

    TFile * newFile = new TFile("/uboone/data/users/imawby/PhotonBDT/photonBDTFilesTruncated.root", "CREATE");
    TTree * newSignalTree = new TTree("PhotonTree", "PhotonTree");
    TTree * newBackgroundTree = new TTree("BackgroundTree", "BackgroundTree");

    // Read branches from the original tree
    double originalCompleteness_signal;
    double originalPurity_signal;
    int originalGeneration_signal;
    int originalNHits3D_signal;
    int originalNHits2D_signal;
    double originalNuVertexSeparation_signal;
    double originalNuVertexChargeDist_signal;
    double originalInitialdEdx_signal;
    double originalTrackShowerScore_signal;
    double originalOpeningAngle_signal;
    double originalShowerLength_signal;
    double originalTrackParentSep_signal;

    originalSignalTree->SetBranchAddress("Completeness", &originalCompleteness_signal);
    originalSignalTree->SetBranchAddress("Purity", &originalPurity_signal); 
    originalSignalTree->SetBranchAddress("Generation", &originalGeneration_signal); 
    originalSignalTree->SetBranchAddress("NHits3D", &originalNHits3D_signal); 
    originalSignalTree->SetBranchAddress("NHits2D", &originalNHits2D_signal); 
    originalSignalTree->SetBranchAddress("NuVertexSeparation", &originalNuVertexSeparation_signal); 
    originalSignalTree->SetBranchAddress("NuVertexChargeDistribution", &originalNuVertexChargeDist_signal); 
    originalSignalTree->SetBranchAddress("InitialdEdx", &originalInitialdEdx_signal); 
    originalSignalTree->SetBranchAddress("TrackShowerScore", &originalTrackShowerScore_signal); 
    originalSignalTree->SetBranchAddress("ShowerOpeningAngle", &originalOpeningAngle_signal); 
    originalSignalTree->SetBranchAddress("ShowerLength", &originalShowerLength_signal); 
    originalSignalTree->SetBranchAddress("TrackParentSeparation", &originalTrackParentSep_signal); 

    // Create branches for the new tree
    double newCompleteness_signal;
    double newPurity_signal;
    int newGeneration_signal;
    int newNHits3D_signal;
    int newNHits2D_signal;
    double newNuVertexSeparation_signal;
    double newNuVertexChargeDist_signal;
    double newInitialdEdx_signal;
    double newTrackShowerScore_signal;
    double newOpeningAngle_signal;
    double newShowerLength_signal;
    double newTrackParentSep_signal;

    newSignalTree->Branch("Completeness", &newCompleteness_signal);
    newSignalTree->Branch("Purity", &newPurity_signal);
    newSignalTree->Branch("Generation", &newGeneration_signal);
    newSignalTree->Branch("NHits3D", &newNHits3D_signal);
    newSignalTree->Branch("NHits2D", &newNHits2D_signal);
    newSignalTree->Branch("NuVertexSeparation", &newNuVertexSeparation_signal);
    newSignalTree->Branch("NuVertexChargeDistribution", &newNuVertexChargeDist_signal);
    newSignalTree->Branch("InitialdEdx", &newInitialdEdx_signal);
    newSignalTree->Branch("TrackShowerScore", &newTrackShowerScore_signal);
    newSignalTree->Branch("ShowerOpeningAngle", &newOpeningAngle_signal);
    newSignalTree->Branch("ShowerLength", &newShowerLength_signal);
    newSignalTree->Branch("TrackParentSeparation", &newTrackParentSep_signal);

    // Truncate original tree branches, set as new tree
    int nHits3DMin = -1, nHits2DMin = -1;
    int nHits3DMax = 300, nHits2DMax = 300;

    double nuVertexSeparationMin = -1.0, nuVertexChargeDistMin = -1.0, initialdEdxMin = -1.0, trackScoreMin = -1.0, openingAngleMin = -1.0, showerLengthMin = -1.0, trackParentSepMin = -1.0;
    double nuVertexSeparationMax = 100.0, nuVertexChargeDistMax = 10.0, initialdEdxMax = 10.0, trackScoreMax = 1.0, openingAngleMax = 50.0, showerLengthMax = 200.0, trackParentSepMax = 2.0;

    int nSignal = 0;

    for (unsigned int i = 0; i < originalSignalTree->GetEntries(); ++i)
    { 
        originalSignalTree->GetEntry(i);

        if ((originalCompleteness_signal < COMPLETENESS_THRESHOLD) || (originalPurity_signal < PURITY_THRESHOLD))
            continue;

        newCompleteness_signal = originalCompleteness_signal;
        newPurity_signal = originalPurity_signal;
        newNHits3D_signal = std::max(std::min(originalNHits3D_signal, nHits3DMax), nHits3DMin);
        newNHits2D_signal = std::max(std::min(originalNHits2D_signal, nHits2DMax), nHits2DMin);
        newNuVertexSeparation_signal = std::max(std::min(originalNuVertexSeparation_signal, nuVertexSeparationMax), nuVertexSeparationMin);
        newNuVertexChargeDist_signal = std::max(std::min(originalNuVertexChargeDist_signal, nuVertexChargeDistMax), nuVertexChargeDistMin);
        newInitialdEdx_signal = std::max(std::min(originalInitialdEdx_signal, initialdEdxMax), initialdEdxMin);
        newTrackShowerScore_signal = std::max(std::min(originalTrackShowerScore_signal, trackScoreMax), trackScoreMin);
        newOpeningAngle_signal = std::max(std::min(originalOpeningAngle_signal, openingAngleMax), openingAngleMin);
        newShowerLength_signal = std::max(std::min(originalShowerLength_signal, showerLengthMax), showerLengthMin);
        newTrackParentSep_signal = std::max(std::min(originalTrackParentSep_signal, trackParentSepMax), trackParentSepMin);

        newSignalTree->Fill();

        ++nSignal;
    }

    std::cout << "nSignal: " << nSignal << std::endl;

    // Read branches from the original tree
    double originalCompleteness_background;
    double originalPurity_background;
    int originalGeneration_background;
    int originalNHits3D_background;
    int originalNHits2D_background;
    double originalNuVertexSeparation_background;
    double originalNuVertexChargeDist_background;
    double originalInitialdEdx_background;
    double originalTrackShowerScore_background;
    double originalOpeningAngle_background;
    double originalShowerLength_background;
    double originalTrackParentSep_background;

    originalBackgroundTree->SetBranchAddress("Completeness", &originalCompleteness_background);
    originalBackgroundTree->SetBranchAddress("Purity", &originalPurity_background); 
    originalBackgroundTree->SetBranchAddress("Generation", &originalGeneration_background); 
    originalBackgroundTree->SetBranchAddress("NHits3D", &originalNHits3D_background); 
    originalBackgroundTree->SetBranchAddress("NHits2D", &originalNHits2D_background); 
    originalBackgroundTree->SetBranchAddress("NuVertexSeparation", &originalNuVertexSeparation_background); 
    originalBackgroundTree->SetBranchAddress("NuVertexChargeDistribution", &originalNuVertexChargeDist_background); 
    originalBackgroundTree->SetBranchAddress("InitialdEdx", &originalInitialdEdx_background); 
    originalBackgroundTree->SetBranchAddress("TrackShowerScore", &originalTrackShowerScore_background); 
    originalBackgroundTree->SetBranchAddress("ShowerOpeningAngle", &originalOpeningAngle_background); 
    originalBackgroundTree->SetBranchAddress("ShowerLength", &originalShowerLength_background); 
    originalBackgroundTree->SetBranchAddress("TrackParentSeparation", &originalTrackParentSep_background); 

    // Create branches for the new tree
    double newCompleteness_background;
    double newPurity_background;
    int newGeneration_background;
    int newNHits3D_background;
    int newNHits2D_background;
    double newNuVertexSeparation_background;
    double newNuVertexChargeDist_background;
    double newInitialdEdx_background;
    double newTrackShowerScore_background;
    double newOpeningAngle_background;
    double newShowerLength_background;
    double newTrackParentSep_background;

    newBackgroundTree->Branch("Completeness", &newCompleteness_background);
    newBackgroundTree->Branch("Purity", &newPurity_background);
    newBackgroundTree->Branch("Generation", &newGeneration_background);
    newBackgroundTree->Branch("NHits3D", &newNHits3D_background);
    newBackgroundTree->Branch("NHits2D", &newNHits2D_background);
    newBackgroundTree->Branch("NuVertexSeparation", &newNuVertexSeparation_background);
    newBackgroundTree->Branch("NuVertexChargeDistribution", &newNuVertexChargeDist_background);
    newBackgroundTree->Branch("InitialdEdx", &newInitialdEdx_background);
    newBackgroundTree->Branch("TrackShowerScore", &newTrackShowerScore_background);
    newBackgroundTree->Branch("ShowerOpeningAngle", &newOpeningAngle_background);
    newBackgroundTree->Branch("ShowerLength", &newShowerLength_background);
    newBackgroundTree->Branch("TrackParentSeparation", &newTrackParentSep_background);

    // Truncate original tree branches, set as new tree
    int nBackground = 0;

    for (unsigned int i = 0; i < originalBackgroundTree->GetEntries(); ++i)
    { 
        originalBackgroundTree->GetEntry(i);

        if ((originalCompleteness_signal < COMPLETENESS_THRESHOLD) || (originalPurity_signal < PURITY_THRESHOLD))
            continue;

        newCompleteness_background = originalCompleteness_background;
        newPurity_background = originalPurity_background;
        newNHits3D_background = std::max(std::min(originalNHits3D_background, nHits3DMax), nHits3DMin);
        newNHits2D_background = std::max(std::min(originalNHits2D_background, nHits2DMax), nHits2DMin);
        newNuVertexSeparation_background = std::max(std::min(originalNuVertexSeparation_background, nuVertexSeparationMax), nuVertexSeparationMin);
        newNuVertexChargeDist_background = std::max(std::min(originalNuVertexChargeDist_background, nuVertexChargeDistMax), nuVertexChargeDistMin);
        newInitialdEdx_background = std::max(std::min(originalInitialdEdx_background, initialdEdxMax), initialdEdxMin);
        newTrackShowerScore_background = std::max(std::min(originalTrackShowerScore_background, trackScoreMax), trackScoreMin);
        newOpeningAngle_background = std::max(std::min(originalOpeningAngle_background, openingAngleMax), openingAngleMin);
        newShowerLength_background = std::max(std::min(originalShowerLength_background, showerLengthMax), showerLengthMin);
        newTrackParentSep_background = std::max(std::min(originalTrackParentSep_background, trackParentSepMax), trackParentSepMin);

        newBackgroundTree->Fill();

        ++nBackground;
    }

    std::cout << "nBackground: " << nBackground << std::endl;

    // Write output trees

    newSignalTree->Write("PhotonTree");
    newBackgroundTree->Write("BackgroundTree");
}
