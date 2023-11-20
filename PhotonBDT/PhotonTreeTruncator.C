#include "RangeVariables.C"

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
    double originalDCA_signal;
    double originalNuVertexChargeDist_signal;
    double originalInitialdEdx_signal;
    double originalTrackShowerScore_signal;
    double originalOpeningAngle_signal;
    double originalShowerLength_signal;
    double originalTrackParentSep_signal;
    double originalChiPIDProton_signal;
    double originalChiPIDMuon_signal;
    double originalChiPIDPion_signal;
    double originalBraggPIDProton_signal;
    double originalBraggPIDMuon_signal;
    double originalBraggPIDPion_signal;
    double originalLLRPID_signal;

    originalSignalTree->SetBranchAddress("Completeness", &originalCompleteness_signal);
    originalSignalTree->SetBranchAddress("Purity", &originalPurity_signal); 
    originalSignalTree->SetBranchAddress("Generation", &originalGeneration_signal); 
    originalSignalTree->SetBranchAddress("NHits3D", &originalNHits3D_signal); 
    originalSignalTree->SetBranchAddress("NHits2D", &originalNHits2D_signal); 
    originalSignalTree->SetBranchAddress("NuVertexSeparation", &originalNuVertexSeparation_signal); 
    originalSignalTree->SetBranchAddress("DCA", &originalDCA_signal);
    originalSignalTree->SetBranchAddress("NuVertexChargeDistribution", &originalNuVertexChargeDist_signal); 
    originalSignalTree->SetBranchAddress("InitialdEdx", &originalInitialdEdx_signal); 
    originalSignalTree->SetBranchAddress("TrackShowerScore", &originalTrackShowerScore_signal); 
    originalSignalTree->SetBranchAddress("ShowerOpeningAngle", &originalOpeningAngle_signal); 
    originalSignalTree->SetBranchAddress("ShowerLength", &originalShowerLength_signal); 
    originalSignalTree->SetBranchAddress("TrackParentSeparation", &originalTrackParentSep_signal); 
    originalSignalTree->SetBranchAddress("ChiPIDProton", &originalChiPIDProton_signal);
    originalSignalTree->SetBranchAddress("ChiPIDMuon", &originalChiPIDMuon_signal);
    originalSignalTree->SetBranchAddress("ChiPIDPion", &originalChiPIDPion_signal);
    originalSignalTree->SetBranchAddress("BraggPIDProton", &originalBraggPIDProton_signal);
    originalSignalTree->SetBranchAddress("BraggPIDMuon", &originalBraggPIDMuon_signal);
    originalSignalTree->SetBranchAddress("BraggPIDPion", &originalBraggPIDPion_signal);
    originalSignalTree->SetBranchAddress("LLRPIDReduced", &originalLLRPID_signal);

    // Create branches for the new tree
    double newCompleteness_signal;
    double newPurity_signal;
    int newGeneration_signal;
    int newNHits3D_signal;
    int newNHits2D_signal;
    double newNuVertexSeparation_signal;
    double newDCA_signal;
    double newNuVertexChargeDist_signal;
    double newInitialdEdx_signal;
    double newTrackShowerScore_signal;
    double newOpeningAngle_signal;
    double newShowerLength_signal;
    double newTrackParentSep_signal;
    double newChiPIDProton_signal;
    double newChiPIDMuon_signal;
    double newChiPIDPion_signal;
    double newBraggPIDProton_signal;
    double newBraggPIDMuon_signal;
    double newBraggPIDPion_signal;
    double newLLRPID_signal;

    newSignalTree->Branch("Completeness", &newCompleteness_signal);
    newSignalTree->Branch("Purity", &newPurity_signal);
    newSignalTree->Branch("Generation", &newGeneration_signal);
    newSignalTree->Branch("NHits3D", &newNHits3D_signal);
    newSignalTree->Branch("NHits2D", &newNHits2D_signal);
    newSignalTree->Branch("NuVertexSeparation", &newNuVertexSeparation_signal);
    newSignalTree->Branch("DCA", &newDCA_signal);
    newSignalTree->Branch("NuVertexChargeDistribution", &newNuVertexChargeDist_signal);
    newSignalTree->Branch("InitialdEdx", &newInitialdEdx_signal);
    newSignalTree->Branch("TrackShowerScore", &newTrackShowerScore_signal);
    newSignalTree->Branch("ShowerOpeningAngle", &newOpeningAngle_signal);
    newSignalTree->Branch("ShowerLength", &newShowerLength_signal);
    newSignalTree->Branch("TrackParentSeparation", &newTrackParentSep_signal);
    newSignalTree->Branch("ChiPIDProton", &newChiPIDProton_signal);
    newSignalTree->Branch("ChiPIDMuon", &newChiPIDMuon_signal);
    newSignalTree->Branch("ChiPIDPion", &newChiPIDPion_signal);
    newSignalTree->Branch("BraggPIDProton", &newBraggPIDProton_signal);
    newSignalTree->Branch("BraggPIDMuon", &newBraggPIDMuon_signal);
    newSignalTree->Branch("BraggPIDPion", &newBraggPIDPion_signal);
    newSignalTree->Branch("LLRPIDReduced", &newLLRPID_signal);

    int nSignal = 0;

    for (unsigned int i = 0; i < originalSignalTree->GetEntries(); ++i)
    { 
        originalSignalTree->GetEntry(i);

        if ((originalCompleteness_signal < COMPLETENESS_THRESHOLD) || (originalPurity_signal < PURITY_THRESHOLD))
            continue;

        newCompleteness_signal = originalCompleteness_signal;
        newPurity_signal = originalPurity_signal;
        newNHits3D_signal = RangeNHits3D(originalNHits3D_signal);
        newNHits2D_signal = RangeNHits2D(originalNHits2D_signal);
        newNuVertexSeparation_signal = RangeNuVertexSeparation(originalNuVertexSeparation_signal);
        newDCA_signal = RangeDCA(originalDCA_signal);
        newNuVertexChargeDist_signal = RangeNuVertexChargeDistribution(originalNuVertexChargeDist_signal);
        newInitialdEdx_signal = RangeInitialdEdx(originalInitialdEdx_signal);
        newTrackShowerScore_signal = RangeTrackScore(originalTrackShowerScore_signal);
        newOpeningAngle_signal = RangeOpeningAngle(originalOpeningAngle_signal);
        newShowerLength_signal = RangeShowerLength(originalShowerLength_signal);
        newTrackParentSep_signal = RangeTrackParentSeparation(originalTrackParentSep_signal);
        newChiPIDProton_signal = RangeChiPIDProton(originalChiPIDProton_signal);
        newChiPIDMuon_signal = RangeChiPIDMuon(originalChiPIDMuon_signal);
        newChiPIDPion_signal = RangeChiPIDPion(originalChiPIDPion_signal);
        newBraggPIDProton_signal = RangeBraggPIDProton(originalBraggPIDProton_signal);
        newBraggPIDMuon_signal = RangeBraggPIDMuon(originalBraggPIDMuon_signal);
        newBraggPIDPion_signal = RangeBraggPIDPion(originalBraggPIDPion_signal);
        newLLRPID_signal = RangeLLRPID(originalLLRPID_signal);

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
    double originalDCA_background;
    double originalNuVertexChargeDist_background;
    double originalInitialdEdx_background;
    double originalTrackShowerScore_background;
    double originalOpeningAngle_background;
    double originalShowerLength_background;
    double originalTrackParentSep_background;
    double originalChiPIDProton_background;
    double originalChiPIDMuon_background;
    double originalChiPIDPion_background;
    double originalBraggPIDProton_background;
    double originalBraggPIDMuon_background;
    double originalBraggPIDPion_background;
    double originalLLRPID_background;

    originalBackgroundTree->SetBranchAddress("Completeness", &originalCompleteness_background);
    originalBackgroundTree->SetBranchAddress("Purity", &originalPurity_background); 
    originalBackgroundTree->SetBranchAddress("Generation", &originalGeneration_background); 
    originalBackgroundTree->SetBranchAddress("NHits3D", &originalNHits3D_background); 
    originalBackgroundTree->SetBranchAddress("NHits2D", &originalNHits2D_background); 
    originalBackgroundTree->SetBranchAddress("NuVertexSeparation", &originalNuVertexSeparation_background); 
    originalBackgroundTree->SetBranchAddress("DCA", &originalDCA_background);
    originalBackgroundTree->SetBranchAddress("NuVertexChargeDistribution", &originalNuVertexChargeDist_background); 
    originalBackgroundTree->SetBranchAddress("InitialdEdx", &originalInitialdEdx_background); 
    originalBackgroundTree->SetBranchAddress("TrackShowerScore", &originalTrackShowerScore_background); 
    originalBackgroundTree->SetBranchAddress("ShowerOpeningAngle", &originalOpeningAngle_background); 
    originalBackgroundTree->SetBranchAddress("ShowerLength", &originalShowerLength_background); 
    originalBackgroundTree->SetBranchAddress("TrackParentSeparation", &originalTrackParentSep_background); 
    originalBackgroundTree->SetBranchAddress("ChiPIDProton", &originalChiPIDProton_background);
    originalBackgroundTree->SetBranchAddress("ChiPIDMuon", &originalChiPIDMuon_background);
    originalBackgroundTree->SetBranchAddress("ChiPIDPion", &originalChiPIDPion_background);
    originalBackgroundTree->SetBranchAddress("BraggPIDProton", &originalBraggPIDProton_background);
    originalBackgroundTree->SetBranchAddress("BraggPIDMuon", &originalBraggPIDMuon_background);
    originalBackgroundTree->SetBranchAddress("BraggPIDPion", &originalBraggPIDPion_background);
    originalBackgroundTree->SetBranchAddress("LLRPIDReduced", &originalLLRPID_background);

    // Create branches for the new tree
    double newCompleteness_background;
    double newPurity_background;
    int newGeneration_background;
    int newNHits3D_background;
    int newNHits2D_background;
    double newNuVertexSeparation_background;
    double newDCA_background;
    double newNuVertexChargeDist_background;
    double newInitialdEdx_background;
    double newTrackShowerScore_background;
    double newOpeningAngle_background;
    double newShowerLength_background;
    double newTrackParentSep_background;
    double newChiPIDProton_background;
    double newChiPIDMuon_background;
    double newChiPIDPion_background;
    double newBraggPIDProton_background;
    double newBraggPIDMuon_background;
    double newBraggPIDPion_background;
    double newLLRPID_background;

    newBackgroundTree->Branch("Completeness", &newCompleteness_background);
    newBackgroundTree->Branch("Purity", &newPurity_background);
    newBackgroundTree->Branch("Generation", &newGeneration_background);
    newBackgroundTree->Branch("NHits3D", &newNHits3D_background);
    newBackgroundTree->Branch("NHits2D", &newNHits2D_background);
    newBackgroundTree->Branch("NuVertexSeparation", &newNuVertexSeparation_background);
    newBackgroundTree->Branch("DCA", &newDCA_background);
    newBackgroundTree->Branch("NuVertexChargeDistribution", &newNuVertexChargeDist_background);
    newBackgroundTree->Branch("InitialdEdx", &newInitialdEdx_background);
    newBackgroundTree->Branch("TrackShowerScore", &newTrackShowerScore_background);
    newBackgroundTree->Branch("ShowerOpeningAngle", &newOpeningAngle_background);
    newBackgroundTree->Branch("ShowerLength", &newShowerLength_background);
    newBackgroundTree->Branch("TrackParentSeparation", &newTrackParentSep_background);
    newBackgroundTree->Branch("ChiPIDProton", &newChiPIDProton_background);
    newBackgroundTree->Branch("ChiPIDMuon", &newChiPIDMuon_background);
    newBackgroundTree->Branch("ChiPIDPion", &newChiPIDPion_background);
    newBackgroundTree->Branch("BraggPIDProton", &newBraggPIDProton_background);
    newBackgroundTree->Branch("BraggPIDMuon", &newBraggPIDMuon_background);
    newBackgroundTree->Branch("BraggPIDPion", &newBraggPIDPion_background);
    newBackgroundTree->Branch("LLRPIDReduced", &newLLRPID_background);

    // Truncate original tree branches, set as new tree
    int nBackground = 0;

    for (unsigned int i = 0; i < originalBackgroundTree->GetEntries(); ++i)
    { 
        originalBackgroundTree->GetEntry(i);

        if ((originalCompleteness_background < COMPLETENESS_THRESHOLD) || (originalPurity_background < PURITY_THRESHOLD))
            continue;

        newCompleteness_background = originalCompleteness_background;
        newPurity_background = originalPurity_background;
        newNHits3D_background = RangeNHits3D(originalNHits3D_background);
        newNHits2D_background = RangeNHits2D(originalNHits2D_background);
        newNuVertexSeparation_background = RangeNuVertexSeparation(originalNuVertexSeparation_background);
        newDCA_background = RangeDCA(originalDCA_background);
        newNuVertexChargeDist_background = RangeNuVertexChargeDistribution(originalNuVertexChargeDist_background);
        newInitialdEdx_background = RangeInitialdEdx(originalInitialdEdx_background);
        newTrackShowerScore_background = RangeTrackScore(originalTrackShowerScore_background);
        newOpeningAngle_background = RangeOpeningAngle(originalOpeningAngle_background);
        newShowerLength_background = RangeShowerLength(originalShowerLength_background);
        newTrackParentSep_background = RangeTrackParentSeparation(originalTrackParentSep_background);
        newChiPIDProton_background = RangeChiPIDProton(originalChiPIDProton_background);
        newChiPIDMuon_background = RangeChiPIDMuon(originalChiPIDMuon_background);
        newChiPIDPion_background = RangeChiPIDPion(originalChiPIDPion_background);
        newBraggPIDProton_background = RangeBraggPIDProton(originalBraggPIDProton_background);
        newBraggPIDMuon_background = RangeBraggPIDMuon(originalBraggPIDMuon_background);
        newBraggPIDPion_background = RangeBraggPIDPion(originalBraggPIDPion_background);
        newLLRPID_background = RangeLLRPID(originalLLRPID_background);

        newBackgroundTree->Fill();

        ++nBackground;
    }

    std::cout << "nBackground: " << nBackground << std::endl;

    // Write output trees

    newSignalTree->Write("PhotonTree");
    newBackgroundTree->Write("BackgroundTree");
}
