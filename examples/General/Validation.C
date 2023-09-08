R__LOAD_LIBRARY($HYP_TOP/lib/libHyperon.so)
R__LOAD_LIBRARY($HYP_TOP/lib/libParticleDict.so)

#include "SelectionManager.h"
#include "EventAssembler.h"
#include "Cut.h"
#include "SelectionParameters.h"

#include "SampleSets_Example.h"
#include "Parameters.h"

const int MUON_INDEX = 0;
const int PROTON_INDEX = 1;
const int PION_INDEX = 2;
const int GAMMA_INDEX = 3;
std::vector<int> MODE_PARTICLE_TYPE_INDICIES;

const double EPSILON = 0.0000000001;
const std::vector<int> LINE_COLOR = {kBlue, kRed, kSpring+3, kBlack};
const std::vector LEGEND_STRING = {"Muon", "Proton", "Pion", "Gamma"};

const bool CHEAT_GAMMA_RECO = false;
const bool CHEAT_EXISTING_GAMMA_RECO = true;
const bool CHEAT_SLICE_ID = false;
const bool CHEAT_GAMMA_DRAG = true;
const double CONTAMINATION_THRESHOLD = 0.5;

const bool FLASH_MATCH_MODE = false;
const bool PANDORA_MODE = false;
const bool BOTH_MODE = true;

void DrawHistogramCollection(const std::vector<TH1D*> &histogramCollection, const std::string &title,
    const std::string &xAxis, const std::string &yAxis);
void ObtainEfficiencyHistogramCollection(const std::vector<TH1D*> &weightedHistogramCollection, 
    const std::vector<TH1D*> &fullHistogramCollection);
void NormaliseHistogramCollection(const std::vector<TH1D*> &histogramCollection);
void DrawSliceEfficiency(const std::vector<TH1D*> &fullHistogramCollection, const std::vector<TH1D*> &thisSliceEffCollection,
    const std::vector<TH1D*> &otherSliceEffCollection, const int particleTypeIndex);

void Validation()
{
    gStyle->SetOptStat(0000);

    TFile * file = new TFile("/uboone/app/users/imawby/HyperonAnalysis/data/run3b/SigmaRecoAnalysis/sigmaRecoAna.root", "READ");
    TTree * tree = (TTree*)file->Get("sigmaRecoAnalyser/sigmaAna");

    int m_run;
    int m_subrun;
    int m_event;
    bool m_sigmaMode;
    bool m_lambdaMode;
    int m_flashMatchNuSliceID;
    int m_pandoraNuSliceID;
    int m_trueNuSliceID;
    std::vector<int> *m_nTrueHits(nullptr);
    std::vector<int> *m_nTrueHitsU(nullptr);
    std::vector<int> *m_nTrueHitsV(nullptr);
    std::vector<int> *m_nTrueHitsW(nullptr);
    double m_trueNuVertexX;
    double m_trueNuVertexY;
    double m_trueNuVertexZ;
    //bool m_foundCorrectNuSlice;
    std::vector<int> *m_matchFoundInTrueNuSlice(nullptr);
    std::vector<int> *m_matchFoundInTrueOtherSlice(nullptr);
    std::vector<double> *m_trueNuVertexSep(nullptr);
    std::vector<double> *m_trueBestMatchTrackScore(nullptr);
    std::vector<double> *m_trueBestMatchCompleteness(nullptr);
    std::vector<double> *m_trueBestMatchPurity(nullptr);
    std::vector<int> *m_trueBestMatchGeneration(nullptr);
    std::vector<bool> *m_trueBestMatchHasTrack(nullptr);
    std::vector<bool> *m_trueBestMatchHasShower(nullptr);
    int m_nParticlesFoundInTrueNuSlice;
    bool m_pionMergedTrueSlice;
    bool m_protonMergedTrueSlice;
    int m_pionContaminantHitsTrueSlice;
    int m_protonContaminantHitsTrueSlice;

    tree->SetBranchAddress("Run", &m_run);
    tree->SetBranchAddress("SubRun", &m_subrun);
    tree->SetBranchAddress("Event", &m_event);
    tree->SetBranchAddress("SigmaMode", &m_sigmaMode);
    tree->SetBranchAddress("LambdaMode", &m_lambdaMode);
    tree->SetBranchAddress("FlashMatchNuSliceID", &m_flashMatchNuSliceID);
    tree->SetBranchAddress("PandoraNuSliceID", &m_pandoraNuSliceID);
    tree->SetBranchAddress("TrueNuSliceID", &m_trueNuSliceID);
    tree->SetBranchAddress("NTrueHits", &m_nTrueHits);
    tree->SetBranchAddress("NTrueHitsU", &m_nTrueHitsU);
    tree->SetBranchAddress("NTrueHitsV", &m_nTrueHitsV);
    tree->SetBranchAddress("NTrueHitsW", &m_nTrueHitsW);
    tree->SetBranchAddress("TrueNuVertexX", &m_trueNuVertexX);
    tree->SetBranchAddress("TrueNuVertexY", &m_trueNuVertexY);
    tree->SetBranchAddress("TrueNuVertexZ", &m_trueNuVertexZ);
    tree->SetBranchAddress("MatchFoundInTrueNuSlice", &m_matchFoundInTrueNuSlice);
    tree->SetBranchAddress("MatchFoundInTrueOtherSlice", &m_matchFoundInTrueOtherSlice);
    tree->SetBranchAddress("TrueNuVertexSep", &m_trueNuVertexSep);
    tree->SetBranchAddress("TrueBestMatchTrackScore", &m_trueBestMatchTrackScore);
    tree->SetBranchAddress("TrueBestMatchCompleteness", &m_trueBestMatchCompleteness);
    tree->SetBranchAddress("TrueBestMatchPurity", &m_trueBestMatchPurity);
    tree->SetBranchAddress("TrueBestMatchGeneration", &m_trueBestMatchGeneration);
    tree->SetBranchAddress("TrueBestMatchHasTrack", &m_trueBestMatchHasTrack);
    tree->SetBranchAddress("TrueBestMatchHasShower", &m_trueBestMatchHasShower);
    tree->SetBranchAddress("NParticlesFoundInTrueNuSlice", &m_nParticlesFoundInTrueNuSlice);
    tree->SetBranchAddress("PionMergedTrueSlice", &m_pionMergedTrueSlice);
    tree->SetBranchAddress("ProtonMergedTrueSlice", &m_protonMergedTrueSlice);
    tree->SetBranchAddress("ProtonContaminantHitsTrueSlice", &m_protonContaminantHitsTrueSlice);
    tree->SetBranchAddress("PionContaminantHitsTrueSlice", &m_pionContaminantHitsTrueSlice);

    /////////////////////
    // Setup analyser
    /////////////////////
    tree->GetEntry(0);
    std::cout << "\033[31m" << "Analyser in: " << "\033[33m" << (m_lambdaMode ? "Lambda mode" : "Sigma mode") << "\033[0m" << std::endl;

    if (m_lambdaMode)
        MODE_PARTICLE_TYPE_INDICIES = {MUON_INDEX, PROTON_INDEX, PION_INDEX};
    else
        MODE_PARTICLE_TYPE_INDICIES = {MUON_INDEX, PROTON_INDEX, PION_INDEX, GAMMA_INDEX};

    int modeCount = 0; 

    for (bool modeBool : {FLASH_MATCH_MODE, PANDORA_MODE, BOTH_MODE})
    {
        if (modeBool)
            ++modeCount;
    }

    if (modeCount != 1)
    {
        std::cout << "Analyser mode is not set up properly :(" << std::endl;
        throw;
    }

    std::cout << "\033[31m" << "Analyser in: " << "\033[33m" << (FLASH_MATCH_MODE ? "Flash match mode" : PANDORA_MODE ? "Pandora mode" : "Both mode") << "\033[0m" << std::endl;

    SelectionParameters selectionParameters(P_RHC_Tune_397_NoBDT);
    FV::FiducialVolume a_FiducialVolume(selectionParameters.p_FV, selectionParameters.p_Padding);

    /////////////////////
    // True histograms
    /////////////////////
    // True hits
    double nTrueHitsMin(-1.5), nTrueHitsMax(199.5), nTrueHitsBin(40);
    TH1D * nTrueHitsDist_0 = new TH1D("nTrueHitsDist_0", "nTrueHitsDist_0", nTrueHitsBin, nTrueHitsMin, nTrueHitsMax);
    TH1D * nTrueHitsDist_1 = new TH1D("nTrueHitsDist_1", "nTrueHitsDist_1", nTrueHitsBin, nTrueHitsMin, nTrueHitsMax);
    TH1D * nTrueHitsDist_2 = new TH1D("nTrueHitsDist_2", "nTrueHitsDist_2", nTrueHitsBin, nTrueHitsMin, nTrueHitsMax);
    TH1D * nTrueHitsDist_3 = new TH1D("nTrueHitsDist_3", "nTrueHitsDist_3", nTrueHitsBin, nTrueHitsMin, nTrueHitsMax);
    std::vector<TH1D*> nTrueHitsDists = {nTrueHitsDist_0, nTrueHitsDist_1, nTrueHitsDist_2, nTrueHitsDist_3};

    TH1D * nTrueHitsThisSliceEff_0 = new TH1D("nTrueHitsThisSliceEff_0", "nTrueHitsThisSliceEff_0", nTrueHitsBin, nTrueHitsMin, nTrueHitsMax);
    TH1D * nTrueHitsThisSliceEff_1 = new TH1D("nTrueHitsThisSliceEff_1", "nTrueHitsThisSliceEff_1", nTrueHitsBin, nTrueHitsMin, nTrueHitsMax);
    TH1D * nTrueHitsThisSliceEff_2 = new TH1D("nTrueHitsThisSliceEff_2", "nTrueHitsThisSliceEff_2", nTrueHitsBin, nTrueHitsMin, nTrueHitsMax);
    TH1D * nTrueHitsThisSliceEff_3 = new TH1D("nTrueHitsThisSliceEff_3", "nTrueHitsThisSliceEff_3", nTrueHitsBin, nTrueHitsMin, nTrueHitsMax);
    std::vector<TH1D*> nTrueHitsThisSliceEff = {nTrueHitsThisSliceEff_0, nTrueHitsThisSliceEff_1, nTrueHitsThisSliceEff_2, nTrueHitsThisSliceEff_3};

    TH1D * nTrueHitsOtherSliceEff_0 = new TH1D("nTrueHitsOtherSliceEff_0", "nTrueHitsOtherSliceEff_0", nTrueHitsBin, nTrueHitsMin, nTrueHitsMax);
    TH1D * nTrueHitsOtherSliceEff_1 = new TH1D("nTrueHitsOtherSliceEff_1", "nTrueHitsOtherSliceEff_1", nTrueHitsBin, nTrueHitsMin, nTrueHitsMax);
    TH1D * nTrueHitsOtherSliceEff_2 = new TH1D("nTrueHitsOtherSliceEff_2", "nTrueHitsOtherSliceEff_2", nTrueHitsBin, nTrueHitsMin, nTrueHitsMax);
    TH1D * nTrueHitsOtherSliceEff_3 = new TH1D("nTrueHitsOtherSliceEff_3", "nTrueHitsOtherSliceEff_3", nTrueHitsBin, nTrueHitsMin, nTrueHitsMax);
    std::vector<TH1D*> nTrueHitsOtherSliceEff = {nTrueHitsOtherSliceEff_0, nTrueHitsOtherSliceEff_1, nTrueHitsOtherSliceEff_2, nTrueHitsOtherSliceEff_3};

    // Distance from nu vertex
    double nTrueDistanceMin(-1.5), nTrueDistanceMax(100.5), nTrueDistanceBin(50);
    TH1D * nTrueDistanceDist_0 = new TH1D("nTrueDistanceDist_0", "nTrueDistanceDist_0", nTrueDistanceBin, nTrueDistanceMin, nTrueDistanceMax);
    TH1D * nTrueDistanceDist_1 = new TH1D("nTrueDistanceDist_1", "nTrueDistanceDist_1", nTrueDistanceBin, nTrueDistanceMin, nTrueDistanceMax);
    TH1D * nTrueDistanceDist_2 = new TH1D("nTrueDistanceDist_2", "nTrueDistanceDist_2", nTrueDistanceBin, nTrueDistanceMin, nTrueDistanceMax);
    TH1D * nTrueDistanceDist_3 = new TH1D("nTrueDistanceDist_3", "nTrueDistanceDist_3", nTrueDistanceBin, nTrueDistanceMin, nTrueDistanceMax);
    std::vector<TH1D*> nTrueDistanceDists = {nTrueDistanceDist_0, nTrueDistanceDist_1, nTrueDistanceDist_2, nTrueDistanceDist_3};

    TH1D * nTrueDistanceThisSliceEff_0 = new TH1D("nTrueDistanceThisSliceEff_0", "nTrueDistanceThisSliceEff_0", nTrueDistanceBin, nTrueDistanceMin, nTrueDistanceMax);
    TH1D * nTrueDistanceThisSliceEff_1 = new TH1D("nTrueDistanceThisSliceEff_1", "nTrueDistanceThisSliceEff_1", nTrueDistanceBin, nTrueDistanceMin, nTrueDistanceMax);
    TH1D * nTrueDistanceThisSliceEff_2 = new TH1D("nTrueDistanceThisSliceEff_2", "nTrueDistanceThisSliceEff_2", nTrueDistanceBin, nTrueDistanceMin, nTrueDistanceMax);
    TH1D * nTrueDistanceThisSliceEff_3 = new TH1D("nTrueDistanceThisSliceEff_3", "nTrueDistanceThisSliceEff_3", nTrueDistanceBin, nTrueDistanceMin, nTrueDistanceMax);
    std::vector<TH1D*> nTrueDistanceThisSliceEff = {nTrueDistanceThisSliceEff_0, nTrueDistanceThisSliceEff_1, nTrueDistanceThisSliceEff_2, nTrueDistanceThisSliceEff_3};

    TH1D * nTrueDistanceOtherSliceEff_0 = new TH1D("nTrueDistanceOtherSliceEff_0", "nTrueDistanceOtherSliceEff_0", nTrueDistanceBin, nTrueDistanceMin, nTrueDistanceMax);
    TH1D * nTrueDistanceOtherSliceEff_1 = new TH1D("nTrueDistanceOtherSliceEff_1", "nTrueDistanceOtherSliceEff_1", nTrueDistanceBin, nTrueDistanceMin, nTrueDistanceMax);
    TH1D * nTrueDistanceOtherSliceEff_2 = new TH1D("nTrueDistanceOtherSliceEff_2", "nTrueDistanceOtherSliceEff_2", nTrueDistanceBin, nTrueDistanceMin, nTrueDistanceMax);
    TH1D * nTrueDistanceOtherSliceEff_3 = new TH1D("nTrueDistanceOtherSliceEff_3", "nTrueDistanceOtherSliceEff_3", nTrueDistanceBin, nTrueDistanceMin, nTrueDistanceMax);
    std::vector<TH1D*> nTrueDistanceOtherSliceEff = {nTrueDistanceOtherSliceEff_0, nTrueDistanceOtherSliceEff_1, nTrueDistanceOtherSliceEff_2, nTrueDistanceOtherSliceEff_3};

    /////////////////////
    // Reco histograms - best match if in slice...
    /////////////////////
    // Track Score
    double trackScoreMin(-1.05), trackScoreMax(1.05), trackScoreBin(22);
    TH1D * trackScoreDist_0 = new TH1D("trackScoreDist_0", "trackScoreDist_0", trackScoreBin, trackScoreMin, trackScoreMax);
    TH1D * trackScoreDist_1 = new TH1D("trackScoreDist_1", "trackScoreDist_1", trackScoreBin, trackScoreMin, trackScoreMax);
    TH1D * trackScoreDist_2 = new TH1D("trackScoreDist_2", "trackScoreDist_2", trackScoreBin, trackScoreMin, trackScoreMax);
    TH1D * trackScoreDist_3 = new TH1D("trackScoreDist_3", "trackScoreDist_3", trackScoreBin, trackScoreMin, trackScoreMax);
    std::vector<TH1D*> trackScoreDists = {trackScoreDist_0, trackScoreDist_1, trackScoreDist_2, trackScoreDist_3};

    // Completeness
    double completenessMin(-1.05), completenessMax(1.05), completenessBin(22);
    TH1D * completenessDist_0 = new TH1D("completenessDist_0", "completenessDist_0", completenessBin, completenessMin, completenessMax);
    TH1D * completenessDist_1 = new TH1D("completenessDist_1", "completenessDist_1", completenessBin, completenessMin, completenessMax);
    TH1D * completenessDist_2 = new TH1D("completenessDist_2", "completenessDist_2", completenessBin, completenessMin, completenessMax);
    TH1D * completenessDist_3 = new TH1D("completenessDist_3", "completenessDist_3", completenessBin, completenessMin, completenessMax);
    std::vector<TH1D*> completenessDists = {completenessDist_0, completenessDist_1, completenessDist_2, completenessDist_3};

    // Purity
    double purityMin(-1.05), purityMax(1.05), purityBin(22);
    TH1D * purityDist_0 = new TH1D("purityDist_0", "purityDist_0", purityBin, purityMin, purityMax);
    TH1D * purityDist_1 = new TH1D("purityDist_1", "purityDist_1", purityBin, purityMin, purityMax);
    TH1D * purityDist_2 = new TH1D("purityDist_2", "purityDist_2", purityBin, purityMin, purityMax);
    TH1D * purityDist_3 = new TH1D("purityDist_3", "purityDist_3", purityBin, purityMin, purityMax);
    std::vector<TH1D*> purityDists = {purityDist_0, purityDist_1, purityDist_2, purityDist_3};

    double nSignalEvents(0.0);
    double nEvents0Shower0Track(0.0);
    double nEvents0Shower1Track(0.0);
    double nEvents0Shower2Track(0.0);
    double nEvents0Shower3Track(0.0);
    double nEvents0Shower4PlusTrack(0.0);
    double nEvents1Shower0Track(0.0);
    double nEvents1Shower1Track(0.0);
    double nEvents1Shower2Track(0.0);
    double nEvents1Shower3Track(0.0);
    double nEvents1Shower4PlusTrack(0.0);
    double nEvents2PlusShower0Track(0.0);
    double nEvents2PlusShower1Track(0.0);
    double nEvents2PlusShower2Track(0.0);
    double nEvents2PlusShower3Track(0.0);
    double nEvents2PlusShower4PlusTrack(0.0);
    std::vector<double> foundThisSlice(4, 0.0);
    std::vector<double> foundOtherSlice(4, 0.0);
    std::vector<double> protonPionMerged(4, 0.0);
    std::vector<double> noSlice(4, 0.0);
    std::vector<double> totalParticle(4, 0.0);
    std::vector<double> thisSliceTrackID(4, 0.0);
    std::vector<double> thisSliceShowerID(4, 0.0);
    std::vector<double> thisSliceGen1(4, 0.0);
    std::vector<double> thisSliceGen2(4, 0.0);
    std::vector<double> thisSliceGen3(4, 0.0);
    std::vector<double> thisSliceGen4Plus(4, 0.0);
    std::vector<double> hasTrackObject(4, 0.0);
    std::vector<double> hasShowerObject(4, 0.0);
    double nCorrectNuSlice_FlashMatch(0.0), nCorrectNuSlice_Pandora(0.0), nCorrectNuSlice_Both(0.0), allThisSlice(0.0), allThisSlicePlusGen(0.0), allThisSlicePlusID(0.0), allThisSlicePlusGenPlusID(0.0);

    // Events where all reco particles have > 15 hits
    double nSignalEventsAboveThreshold(0.0);
    std::vector<double> foundThisSlice_15(4, 0.0);
    std::vector<double> foundOtherSlice_15(4, 0.0);
    std::vector<double> protonPionMerged_15(4, 0.0);
    std::vector<double> noSlice_15(4, 0.0);
    std::vector<double> thisSliceTrackID_15(4, 0.0);
    std::vector<double> thisSliceShowerID_15(4, 0.0);
    std::vector<double> thisSliceGen2_15(4, 0.0);
    std::vector<double> thisSliceGen3_15(4, 0.0);
    std::vector<double> thisSliceGen4Plus_15(4, 0.0);
    std::vector<double> hasTrackObject_15(4, 0.0);
    std::vector<double> hasShowerObject_15(4, 0.0);
    double nCorrectNuSlice_FlashMatch_15(0.0), nCorrectNuSlice_Pandora_15(0.0), nCorrectNuSlice_Both_15(0.0), allThisSlice_15(0.0), allThisSlicePlusGen_15(0.0), allThisSlicePlusID_15(0.0), allThisSlicePlusGenPlusID_15(0.0);

    /////////////////////
    // Loop over tree, fill counts and histograms
    /////////////////////
    std::vector<int> run, subrun, event;

    for (unsigned int treeIndex = 0; treeIndex < tree->GetEntries(); ++treeIndex)
    {
        tree->GetEntry(treeIndex);

        TVector3 trueNuVertex(m_trueNuVertexX, m_trueNuVertexY, m_trueNuVertexZ);
        const bool isInFV(a_FiducialVolume.InFiducialVolume(trueNuVertex));

        if (!isInFV)
            continue;

        ++nSignalEvents;

        bool isEventAboveThreshold = true;

        for (int particleTypeIndex : MODE_PARTICLE_TYPE_INDICIES)
        {
            if (m_nTrueHits->at(particleTypeIndex) < 15)
            {
                isEventAboveThreshold = false;
                break;
            }

            int nPlanesAboveThreshold = 0;

            if (m_nTrueHitsU->at(particleTypeIndex) >= 5) ++nPlanesAboveThreshold;
            if (m_nTrueHitsV->at(particleTypeIndex) >= 5) ++nPlanesAboveThreshold;
            if (m_nTrueHitsW->at(particleTypeIndex) >= 5) ++nPlanesAboveThreshold;

            if (nPlanesAboveThreshold < 2)
            {
                isEventAboveThreshold = false;
                break;
            }
        }

        if (isEventAboveThreshold) ++nSignalEventsAboveThreshold;

        const bool foundCorrectNuSlice_FlashMatch = (CHEAT_SLICE_ID ? true : (m_flashMatchNuSliceID == m_trueNuSliceID));
        const bool foundCorrectNuSlice_Pandora = (CHEAT_SLICE_ID ? true : (m_pandoraNuSliceID == m_trueNuSliceID));
        const int sliceID_Both = (m_flashMatchNuSliceID < 0) ? m_pandoraNuSliceID : m_flashMatchNuSliceID;
        const bool foundCorrectNuSlice_Both = (CHEAT_SLICE_ID ? true : (sliceID_Both == m_trueNuSliceID));

        if (foundCorrectNuSlice_FlashMatch)
        {
            ++nCorrectNuSlice_FlashMatch;

            if (isEventAboveThreshold) ++nCorrectNuSlice_FlashMatch_15;
        }

        if (foundCorrectNuSlice_Pandora)
        {
            ++nCorrectNuSlice_Pandora;

            if (isEventAboveThreshold) ++nCorrectNuSlice_Pandora_15;
        }

        if (foundCorrectNuSlice_Both)
        {
            ++nCorrectNuSlice_Both;

            if (isEventAboveThreshold) ++nCorrectNuSlice_Both_15;
        }

        if (FLASH_MATCH_MODE && !foundCorrectNuSlice_FlashMatch)
            continue;

        if (PANDORA_MODE && !foundCorrectNuSlice_Pandora)
            continue;

        if (BOTH_MODE && !foundCorrectNuSlice_Both)
            continue;


        int nShowers(0), nTracks(0), nParticlesFoundInNuSlice(0);

        for (int particleTypeIndex : MODE_PARTICLE_TYPE_INDICIES)
        {
            // Fill histograms
            const double boundedHits = std::min(std::max(nTrueHitsMin + EPSILON, static_cast<double>(m_nTrueHits->at(particleTypeIndex))), nTrueHitsMax - EPSILON);
            const double boundedDistance = std::min(std::max(nTrueDistanceMin + EPSILON, static_cast<double>(m_trueNuVertexSep->at(particleTypeIndex))), nTrueDistanceMax - EPSILON);
            const double boundedTrackScore = std::min(std::max(trackScoreMin + EPSILON, static_cast<double>(m_trueBestMatchTrackScore->at(particleTypeIndex))), trackScoreMax - EPSILON);

            nTrueHitsDists[particleTypeIndex]->Fill(boundedHits);
            nTrueDistanceDists[particleTypeIndex]->Fill(boundedDistance);

            if (m_matchFoundInTrueNuSlice->at(particleTypeIndex))
            {
                trackScoreDists[particleTypeIndex]->Fill(boundedTrackScore);
                completenessDists[particleTypeIndex]->Fill(m_trueBestMatchCompleteness->at(particleTypeIndex));
                purityDists[particleTypeIndex]->Fill(m_trueBestMatchPurity->at(particleTypeIndex));
            }

            nTrueHitsThisSliceEff[particleTypeIndex]->Fill(boundedHits, m_matchFoundInTrueNuSlice->at(particleTypeIndex));
            nTrueDistanceThisSliceEff[particleTypeIndex]->Fill(boundedDistance, m_matchFoundInTrueNuSlice->at(particleTypeIndex));

            nTrueHitsOtherSliceEff[particleTypeIndex]->Fill(boundedHits, m_matchFoundInTrueOtherSlice->at(particleTypeIndex));
            nTrueDistanceOtherSliceEff[particleTypeIndex]->Fill(boundedDistance, m_matchFoundInTrueOtherSlice->at(particleTypeIndex));

            // Fill counts
            ++totalParticle[particleTypeIndex];

            if ((m_matchFoundInTrueNuSlice->at(particleTypeIndex)) || (CHEAT_GAMMA_DRAG && (particleTypeIndex == GAMMA_INDEX) && m_matchFoundInTrueOtherSlice->at(particleTypeIndex)))
            {
                ++nParticlesFoundInNuSlice;

                const bool isTrackID = (!(m_trueBestMatchTrackScore->at(particleTypeIndex) < 0.0) && (m_trueBestMatchTrackScore->at(particleTypeIndex) > 0.5));
                const bool isShowerID = (!(m_trueBestMatchTrackScore->at(particleTypeIndex) < 0.0) && !(m_trueBestMatchTrackScore->at(particleTypeIndex) > 0.5));

                if (isTrackID) ++nTracks;
                if (isShowerID) ++nShowers;

                ++foundThisSlice[particleTypeIndex];
                if (isEventAboveThreshold) ++foundThisSlice_15[particleTypeIndex];

                // Track score counts
                if (isTrackID)
                {
                    ++thisSliceTrackID[particleTypeIndex];

                    if (isEventAboveThreshold) ++thisSliceTrackID_15[particleTypeIndex];
                }
                else if (isShowerID)
                {
                    ++thisSliceShowerID[particleTypeIndex];

                    if (isEventAboveThreshold) ++thisSliceShowerID_15[particleTypeIndex];
                }

                // Generation counts
                const int thisGeneration((CHEAT_GAMMA_DRAG && (particleTypeIndex == GAMMA_INDEX)) ? 2 : m_trueBestMatchGeneration->at(particleTypeIndex));

                if (thisGeneration == 2)
                {
                    ++thisSliceGen2[particleTypeIndex];

                    if (isEventAboveThreshold) ++thisSliceGen2_15[particleTypeIndex];

                }
                else if (thisGeneration == 3)
                {
                    ++thisSliceGen3[particleTypeIndex];

                    if (isEventAboveThreshold) ++thisSliceGen3_15[particleTypeIndex];
                }
                else if (thisGeneration > 3)
                {
                    ++thisSliceGen4Plus[particleTypeIndex];

                    if (isEventAboveThreshold) ++thisSliceGen4Plus_15[particleTypeIndex];
                }

                // Track/shower counts
                if (m_trueBestMatchHasTrack->at(particleTypeIndex))
                {
                    ++hasTrackObject[particleTypeIndex];

                    if (isEventAboveThreshold) ++hasTrackObject_15[particleTypeIndex];
                }
                // use if not else if because it might be fitted as both (not sure if uboone do this)
                if (m_trueBestMatchHasShower->at(particleTypeIndex))
                {
                    ++hasShowerObject[particleTypeIndex];

                    if (isEventAboveThreshold) ++hasShowerObject_15[particleTypeIndex];
                }
            }
            else if (CHEAT_GAMMA_RECO && (particleTypeIndex == GAMMA_INDEX))
            {
                ++nParticlesFoundInNuSlice;
                ++nShowers;

                ++foundThisSlice[particleTypeIndex];
                if (isEventAboveThreshold) ++foundThisSlice_15[particleTypeIndex];

                ++thisSliceShowerID[particleTypeIndex];
                if (isEventAboveThreshold) ++thisSliceShowerID_15[particleTypeIndex];

                ++thisSliceGen2[particleTypeIndex];
                if (isEventAboveThreshold) ++thisSliceGen2_15[particleTypeIndex];

                ++hasShowerObject[particleTypeIndex];
                if (isEventAboveThreshold) ++hasShowerObject_15[particleTypeIndex];
            }
            else if (((particleTypeIndex == PROTON_INDEX) && m_protonMergedTrueSlice) || ((particleTypeIndex == PION_INDEX) && m_pionMergedTrueSlice)) 
            {
                const double contaminantHits = static_cast<double>(particleTypeIndex == PROTON_INDEX ? m_protonContaminantHitsTrueSlice : m_pionContaminantHitsTrueSlice);
                const double contaminationFraction = contaminantHits / static_cast<double>(m_nTrueHits->at(particleTypeIndex));

                if (contaminationFraction > CONTAMINATION_THRESHOLD)
                {
                    ++protonPionMerged[particleTypeIndex];

                    if (isEventAboveThreshold) ++protonPionMerged_15[particleTypeIndex];
                }
                else
                {
                    ++noSlice[particleTypeIndex];

                    if (isEventAboveThreshold) ++noSlice_15[particleTypeIndex];
                }
            }
            else if (m_matchFoundInTrueOtherSlice->at(particleTypeIndex))
            {
                ++foundOtherSlice[particleTypeIndex];

                if (isEventAboveThreshold) ++foundOtherSlice_15[particleTypeIndex];
            }
            else
            {
                ++noSlice[particleTypeIndex];

                if (nCorrectNuSlice_FlashMatch_15)
                {
                    const bool fill(run.empty() || ((run.back() != m_run) && (subrun.back() != m_subrun) && (event.back() != m_event)));

                    if (fill)
                    {
                        run.push_back(m_run);
                        subrun.push_back(m_subrun);
                        event.push_back(m_event);
                    }
                }

                if (isEventAboveThreshold) ++noSlice_15[particleTypeIndex];
            }
        } //end of particle index loop

        // Track/shower multiplicity counts
        if (nShowers == 0)
        {
            if (nTracks == 0) ++nEvents0Shower0Track;
            if (nTracks == 1) ++nEvents0Shower1Track;
            if (nTracks == 2) ++nEvents0Shower2Track;
            if (nTracks == 3) ++nEvents0Shower3Track;
            if (nTracks > 3) ++nEvents0Shower4PlusTrack;
        }
        if (nShowers == 1)
        {
            if (nTracks == 0) ++nEvents1Shower0Track;
            if (nTracks == 1) ++nEvents1Shower1Track;
            if (nTracks == 2) ++nEvents1Shower2Track;
            if (nTracks == 3) ++nEvents1Shower3Track;
            if (nTracks > 3) ++nEvents1Shower4PlusTrack;
        }
        if (nShowers > 1)
        {
            if (nTracks == 0) ++nEvents2PlusShower0Track;
            if (nTracks == 1) ++nEvents2PlusShower1Track;
            if (nTracks == 2) ++nEvents2PlusShower2Track;
            if (nTracks == 3) ++nEvents2PlusShower3Track;
            if (nTracks > 3) ++nEvents2PlusShower4PlusTrack;
        }

        // Event level correctness counts
        if (nParticlesFoundInNuSlice == MODE_PARTICLE_TYPE_INDICIES.size())
        {
            allThisSlice += 1.0;

            if (isEventAboveThreshold) ++allThisSlice_15;

            bool correctGen = true;

            for (int particleTypeIndex : MODE_PARTICLE_TYPE_INDICIES)
            {
                if (!((particleTypeIndex == GAMMA_INDEX) && CHEAT_EXISTING_GAMMA_RECO) && (m_trueBestMatchGeneration->at(particleTypeIndex) != 2))
                {
                    correctGen = false;
                    break;
                }

                if (CHEAT_GAMMA_DRAG && (particleTypeIndex == GAMMA_INDEX)) correctGen = true;
            }

            if (correctGen)
            {
                ++allThisSlicePlusGen;

                if (isEventAboveThreshold) ++allThisSlicePlusGen_15;
            }

            bool correctID = true;

            for (int particleType : MODE_PARTICLE_TYPE_INDICIES)
            {
                const bool trackID = (!(m_trueBestMatchTrackScore->at(particleType) < 0.0) && (m_trueBestMatchTrackScore->at(particleType) > 0.5));
                const bool showerID = (!(m_trueBestMatchTrackScore->at(particleType) < 0.0) && !(m_trueBestMatchTrackScore->at(particleType) > 0.5));

                if ((particleType == MUON_INDEX) && showerID)
                {
                    correctID = false;
                }
                else if ((particleType == PROTON_INDEX) && showerID)
                {
                    correctID = false;
                }
                else if ((particleType == PION_INDEX) && showerID)
                {
                    correctID = false;
                }
                else if ((particleType == GAMMA_INDEX) && trackID)
                {
                    if (!((particleType == GAMMA_INDEX) && CHEAT_EXISTING_GAMMA_RECO))
                        correctID = false;
                }
            }

            if (correctID)
            {
                ++allThisSlicePlusID;

                if (isEventAboveThreshold) ++allThisSlicePlusID_15;
            }

            if (correctGen && correctID)
            {
                ++allThisSlicePlusGenPlusID;

                if (isEventAboveThreshold) ++allThisSlicePlusGenPlusID_15;
            }
        }
    } // end of tree loop
    std::cout << std::endl;

    /////////////////////
    // Normalise count information
    /////////////////////
    float nCorrectNuSlice = FLASH_MATCH_MODE ? nCorrectNuSlice_FlashMatch : PANDORA_MODE ? nCorrectNuSlice_Pandora : nCorrectNuSlice_Both;
    nEvents0Shower0Track = nEvents0Shower0Track * 100.0 / nCorrectNuSlice;
    nEvents0Shower1Track = nEvents0Shower1Track * 100.0 / nCorrectNuSlice;
    nEvents0Shower2Track = nEvents0Shower2Track * 100.0 / nCorrectNuSlice;
    nEvents0Shower3Track = nEvents0Shower3Track * 100.0 / nCorrectNuSlice;
    nEvents0Shower4PlusTrack = nEvents0Shower4PlusTrack * 100.0 / nCorrectNuSlice;
    double nEvents0Shower = nEvents0Shower0Track + nEvents0Shower1Track + nEvents0Shower2Track + nEvents0Shower3Track + nEvents0Shower4PlusTrack;

    nEvents1Shower0Track = nEvents1Shower0Track * 100.0 / nCorrectNuSlice;
    nEvents1Shower1Track = nEvents1Shower1Track * 100.0 / nCorrectNuSlice;
    nEvents1Shower2Track = nEvents1Shower2Track * 100.0 / nCorrectNuSlice;
    nEvents1Shower3Track = nEvents1Shower3Track * 100.0 / nCorrectNuSlice;
    nEvents1Shower4PlusTrack = nEvents1Shower4PlusTrack * 100.0 / nCorrectNuSlice;
    double nEvents1Shower = nEvents1Shower0Track + nEvents1Shower1Track + nEvents1Shower2Track + nEvents1Shower3Track + nEvents1Shower4PlusTrack;

    nEvents2PlusShower0Track = nEvents2PlusShower0Track * 100.0 / nCorrectNuSlice;
    nEvents2PlusShower1Track = nEvents2PlusShower1Track * 100.0 / nCorrectNuSlice;
    nEvents2PlusShower2Track = nEvents2PlusShower2Track * 100.0 / nCorrectNuSlice;
    nEvents2PlusShower3Track = nEvents2PlusShower3Track * 100.0 / nCorrectNuSlice;
    nEvents2PlusShower4PlusTrack = nEvents2PlusShower4PlusTrack * 100.0 / nCorrectNuSlice;
    double nEvents2PlusShower = nEvents2PlusShower0Track + nEvents2PlusShower1Track + nEvents2PlusShower2Track + nEvents2PlusShower3Track + nEvents2PlusShower4PlusTrack;

    double nEvents0Track = nEvents0Shower0Track + nEvents1Shower0Track + nEvents2PlusShower0Track;
    double nEvents1Track = nEvents0Shower1Track + nEvents1Shower1Track + nEvents2PlusShower1Track;
    double nEvents2Track = nEvents0Shower2Track + nEvents1Shower2Track + nEvents2PlusShower2Track;
    double nEvents3Track = nEvents0Shower3Track + nEvents1Shower3Track + nEvents2PlusShower3Track;
    double nEvents4PlusTrack = nEvents0Shower4PlusTrack + nEvents1Shower4PlusTrack + nEvents2PlusShower4PlusTrack;

    for (int particleType : MODE_PARTICLE_TYPE_INDICIES)
    {
        thisSliceTrackID[particleType] = thisSliceTrackID[particleType] * 100.0 / foundThisSlice[particleType];
        thisSliceShowerID[particleType] = thisSliceShowerID[particleType] * 100.0 / foundThisSlice[particleType];
        thisSliceGen1[particleType] = thisSliceGen1[particleType] * 100.0 / foundThisSlice[particleType];
        thisSliceGen2[particleType] = thisSliceGen2[particleType] * 100.0 / foundThisSlice[particleType];
        thisSliceGen3[particleType] = thisSliceGen3[particleType] * 100.0 / foundThisSlice[particleType];
        thisSliceGen4Plus[particleType] = thisSliceGen4Plus[particleType] * 100.0 / foundThisSlice[particleType];
        hasTrackObject[particleType] = hasTrackObject[particleType] * 100.0 / foundThisSlice[particleType];
        hasShowerObject[particleType] = hasShowerObject[particleType] * 100.0 / foundThisSlice[particleType];

        foundThisSlice[particleType] = foundThisSlice[particleType] * 100.0 / nCorrectNuSlice;
        protonPionMerged[particleType] = protonPionMerged[particleType] * 100.0 / nCorrectNuSlice;
        foundOtherSlice[particleType] = foundOtherSlice[particleType] * 100.0 / nCorrectNuSlice;
        noSlice[particleType] = noSlice[particleType] * 100.0 / nCorrectNuSlice;
    }

    nCorrectNuSlice_FlashMatch = nCorrectNuSlice_FlashMatch * 100.0 / nSignalEvents;
    nCorrectNuSlice_Pandora = nCorrectNuSlice_Pandora * 100.0 / nSignalEvents;
    nCorrectNuSlice_Both = nCorrectNuSlice_Both * 100.0 / nSignalEvents;
    allThisSlice = allThisSlice * 100.0 / nSignalEvents;
    allThisSlicePlusGen = allThisSlicePlusGen * 100.0 / nSignalEvents;
    allThisSlicePlusID = allThisSlicePlusID * 100.0 / nSignalEvents;
    allThisSlicePlusGenPlusID = allThisSlicePlusGenPlusID * 100.0 / nSignalEvents;

    float nCorrectNuSlice_15 = FLASH_MATCH_MODE ? nCorrectNuSlice_FlashMatch_15 : PANDORA_MODE ? nCorrectNuSlice_Pandora_15 : nCorrectNuSlice_Both_15;

    for (int particleType : MODE_PARTICLE_TYPE_INDICIES)
    {
        thisSliceTrackID_15[particleType] = thisSliceTrackID_15[particleType] * 100.0 / foundThisSlice_15[particleType];
        thisSliceShowerID_15[particleType] = thisSliceShowerID_15[particleType] * 100.0 / foundThisSlice_15[particleType];
        thisSliceGen2_15[particleType] = thisSliceGen2_15[particleType] * 100.0 / foundThisSlice_15[particleType];
        thisSliceGen3_15[particleType] = thisSliceGen3_15[particleType] * 100.0 / foundThisSlice_15[particleType];
        thisSliceGen4Plus_15[particleType] = thisSliceGen4Plus_15[particleType] * 100.0 / foundThisSlice_15[particleType];
        hasTrackObject_15[particleType] = hasTrackObject_15[particleType] * 100.0 / foundThisSlice_15[particleType];
        hasShowerObject_15[particleType] = hasShowerObject_15[particleType] * 100.0 / foundThisSlice_15[particleType];

        foundThisSlice_15[particleType] = foundThisSlice_15[particleType] * 100.0 / nCorrectNuSlice_15;
        protonPionMerged_15[particleType] = protonPionMerged_15[particleType] * 100.0 / nCorrectNuSlice_15;
        foundOtherSlice_15[particleType] = foundOtherSlice_15[particleType] * 100.0 / nCorrectNuSlice_15;
        noSlice_15[particleType] = noSlice_15[particleType] * 100.0 / nCorrectNuSlice_15;
    }

    nCorrectNuSlice_FlashMatch_15 = nCorrectNuSlice_FlashMatch_15 * 100.0 / nSignalEventsAboveThreshold;
    nCorrectNuSlice_Pandora_15 = nCorrectNuSlice_Pandora_15 * 100.0 / nSignalEventsAboveThreshold;
    nCorrectNuSlice_Both_15 = nCorrectNuSlice_Both_15 * 100.0 / nSignalEventsAboveThreshold;
    allThisSlice_15 = allThisSlice_15 * 100.0 / nSignalEventsAboveThreshold;
    allThisSlicePlusGen_15 = allThisSlicePlusGen_15 * 100.0 / nSignalEventsAboveThreshold;
    allThisSlicePlusID_15 = allThisSlicePlusID_15 * 100.0 / nSignalEventsAboveThreshold;
    allThisSlicePlusGenPlusID_15 = allThisSlicePlusGenPlusID_15 * 100.0 / nSignalEventsAboveThreshold;

    std::cout << std::endl;
    std::cout << "\033[31m" << "Is the slicing ID cheated? " << "\033[33m" << (CHEAT_SLICE_ID ? "YES " : "NO") << "\033[0m" << std::endl;
    std::cout << std::endl;
    std::cout << "Existence information..." << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "| Particle | Nu slice | Other slice |  Merged  |    None    |" << std::endl;
    std::cout << "|  Muon    | " << foundThisSlice[MUON_INDEX] << "% | " << foundOtherSlice[MUON_INDEX] << "% | " <<
        "  N/A   | " << noSlice[MUON_INDEX] << "% |" << std::endl;
    std::cout << "|  Proton  | " << foundThisSlice[PROTON_INDEX] << "% | " << foundOtherSlice[PROTON_INDEX] << "% | " << 
        protonPionMerged[PROTON_INDEX] << "% | " << noSlice[PROTON_INDEX] << "% |" << std::endl;
    std::cout << "|  Pion    | " << foundThisSlice[PION_INDEX] << "% | " << foundOtherSlice[PION_INDEX] << "% | " << 
        protonPionMerged[PION_INDEX] << "% | " << noSlice[PION_INDEX] << "% |" << std::endl;
    if (m_sigmaMode)
    {
        std::cout << "|  Gamma   | " << foundThisSlice[GAMMA_INDEX] << "% | " << foundOtherSlice[GAMMA_INDEX] << "% | " << 
            "  N/A   | " << noSlice[GAMMA_INDEX] << "% |" << std::endl;
    }
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "Track/shower ID for nu slice reco... " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "| Particle | Track | Shower |" << std::endl;
    std::cout << "|  Muon    | " << thisSliceTrackID[MUON_INDEX] << "% | " << thisSliceShowerID[MUON_INDEX] << "% | " << std::endl;
    std::cout << "|  Proton  | " << thisSliceTrackID[PROTON_INDEX] << "% | " << thisSliceShowerID[PROTON_INDEX] << "% | " << std::endl;
    std::cout << "|  Pion    | " << thisSliceTrackID[PION_INDEX] << "% | " << thisSliceShowerID[PION_INDEX] << "% | " << std::endl;
    if (m_sigmaMode)
         std::cout << "|  Gamma   | " << thisSliceTrackID[GAMMA_INDEX] << "% | " << thisSliceShowerID[GAMMA_INDEX] << "% | " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "Generation for nu slice reco... " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "| Particle |  Gen 2  |  Gen 3  |  Gen 4+  |" << std::endl;
    std::cout << "|  Muon    | " << thisSliceGen2[MUON_INDEX] << "% | " << thisSliceGen3[MUON_INDEX] << "% | " << 
        thisSliceGen4Plus[MUON_INDEX] << "% | " << std::endl;
    std::cout << "|  Proton  | " << thisSliceGen2[PROTON_INDEX] << "% | " << thisSliceGen3[PROTON_INDEX] << "% | " << 
        thisSliceGen4Plus[PROTON_INDEX] << "% | " << std::endl;
    std::cout << "|  Pion    | " << thisSliceGen2[PION_INDEX] << "% | " << thisSliceGen3[PION_INDEX] << "% | " << 
        thisSliceGen4Plus[PION_INDEX] << "% | " << std::endl;
    if (m_sigmaMode)
    {
        std::cout << "|  Gamma   | " << thisSliceGen2[GAMMA_INDEX] << "% | " << thisSliceGen3[GAMMA_INDEX] << "% | " << 
            thisSliceGen4Plus[GAMMA_INDEX] << "% | " << std::endl;
    }
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "Associated track/shower object for nu slice reco... " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "| Particle |  Track  |  Shower  |" << std::endl;
    std::cout << "|  Muon    | " << hasTrackObject[MUON_INDEX] << "% | " << hasShowerObject[MUON_INDEX] << "% | " << std::endl;
    std::cout << "|  Proton  | " << hasTrackObject[PROTON_INDEX] << "% | " << hasShowerObject[PROTON_INDEX] << "% | " << std::endl;
    std::cout << "|  Pion    | " << hasTrackObject[PION_INDEX] << "% | " << hasShowerObject[PION_INDEX] << "% | " << std::endl;
    if (m_sigmaMode)
        std::cout << "|  Gamma   | " << hasTrackObject[GAMMA_INDEX] << "% | " << hasShowerObject[GAMMA_INDEX] << "% | " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "Reco particle matrix for nu slice..." << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "|         | 0 Shower | 1 Shower | 2 Shower |           | " << std::endl;
    std::cout << "| 0 Track | " << nEvents0Shower0Track << "% | " << nEvents1Shower0Track << "% |" << nEvents2PlusShower0Track << "% |" << nEvents0Track << "% | " << std::endl; 
    std::cout << "| 1 Track | " << nEvents0Shower1Track << "% | " << nEvents1Shower1Track << "% |" << nEvents2PlusShower1Track << "% |" << nEvents1Track << "% | " << std::endl; 
    std::cout << "| 2 Track | " << nEvents0Shower2Track << "% | " << nEvents1Shower2Track << "% |" << nEvents2PlusShower2Track << "% |" << nEvents2Track << "% | " << std::endl; 
    std::cout << "| 3 Track | " << nEvents0Shower3Track << "% | " << nEvents1Shower3Track << "% |" << nEvents2PlusShower3Track << "% |" << nEvents3Track << "% | " << std::endl; 
    std::cout << "| 4 Track | " << nEvents0Shower4PlusTrack << "% | " << nEvents1Shower4PlusTrack << "% |" << nEvents2PlusShower4PlusTrack << "% | " << nEvents4PlusTrack << "% |" << std::endl; 
    std::cout << "|         | " << nEvents0Shower << "% | " << nEvents1Shower << "% | " << nEvents2PlusShower << "% |         | " << std::endl; 
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "How did we do?" << std::endl;
    std::cout << nCorrectNuSlice_FlashMatch << "% of events have correct Nu slice (flash match)..." << std::endl;
    std::cout << nCorrectNuSlice_Pandora << "% of events have correct Nu slice (pandora)..." << std::endl;
    std::cout << nCorrectNuSlice_Both << "% of events have correct Nu slice (both)..." << std::endl;
    std::cout << allThisSlice << "% of events had all particles found in nu slice, where" << std::endl;
    std::cout << allThisSlicePlusGen << "% also had correct hierarchy tiers" << std::endl;   
    std::cout << allThisSlicePlusID << "% also had correct track/shower IDs" << std::endl;   
    std::cout << allThisSlicePlusGenPlusID << "% also had correct hierachy tier & track/shower IDs" << std::endl;   

    std::cout << std::endl;
    std::cout << "Okay, but how do we do when above reco thresholds (each particle has >= 15 hits" << std::endl;
    std::cout << nSignalEventsAboveThreshold * 100.0 / nSignalEvents << "% of events above threshold" << std::endl;
    std::cout << "Existence information..." << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "| Particle | Nu slice | Other slice |  Merged  |    None    |" << std::endl;
    std::cout << "|  Muon    | " << foundThisSlice_15[MUON_INDEX] << "% | " << foundOtherSlice_15[MUON_INDEX] << "% | " <<
        "  N/A   | " << noSlice_15[MUON_INDEX] << "% |" << std::endl;
    std::cout << "|  Proton  | " << foundThisSlice_15[PROTON_INDEX] << "% | " << foundOtherSlice_15[PROTON_INDEX] << "% | " << 
        protonPionMerged_15[PROTON_INDEX] << "% | " << noSlice_15[PROTON_INDEX] << "% |" << std::endl;
    std::cout << "|  Pion    | " << foundThisSlice_15[PION_INDEX] << "% | " << foundOtherSlice_15[PION_INDEX] << "% | " << 
        protonPionMerged_15[PION_INDEX] << "% | " << noSlice_15[PION_INDEX] << "% |" << std::endl;
    if (m_sigmaMode)
    {
        std::cout << "|  Gamma   | " << foundThisSlice_15[GAMMA_INDEX] << "% | " << foundOtherSlice_15[GAMMA_INDEX] << "% | " << 
            "  N/A   | " << noSlice_15[GAMMA_INDEX] << "% |" << std::endl;
    }
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "Track/shower ID for nu slice reco... " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "| Particle | Track | Shower |" << std::endl;
    std::cout << "|  Muon    | " << thisSliceTrackID_15[MUON_INDEX] << "% | " << thisSliceShowerID_15[MUON_INDEX] << "% | " << std::endl;
    std::cout << "|  Proton  | " << thisSliceTrackID_15[PROTON_INDEX] << "% | " << thisSliceShowerID_15[PROTON_INDEX] << "% | " << std::endl;
    std::cout << "|  Pion    | " << thisSliceTrackID_15[PION_INDEX] << "% | " << thisSliceShowerID_15[PION_INDEX] << "% | " << std::endl;
    if (m_sigmaMode)
        std::cout << "|  Gamma   | " << thisSliceTrackID_15[GAMMA_INDEX] << "% | " << thisSliceShowerID_15[GAMMA_INDEX] << "% | " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "Track/shower ID for nu slice reco... " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "| Particle |  Gen 2  |  Gen 3  |  Gen 4+  |" << std::endl;
    std::cout << "|  Muon    | " << thisSliceGen2_15[MUON_INDEX] << "% | " << thisSliceGen3_15[MUON_INDEX] << "% | " << 
        thisSliceGen4Plus_15[MUON_INDEX] << "% | " << std::endl;
    std::cout << "|  Proton  | " << thisSliceGen2_15[PROTON_INDEX] << "% | " << thisSliceGen3_15[PROTON_INDEX] << "% | " << 
        thisSliceGen4Plus_15[PROTON_INDEX] << "% | " << std::endl;
    std::cout << "|  Pion    | " << thisSliceGen2[PION_INDEX] << "% | " << thisSliceGen3[PION_INDEX] << "% | " << 
        thisSliceGen4Plus_15[PION_INDEX] << "% | " << std::endl;
    if (m_sigmaMode)
    {
        std::cout << "|  Gamma   | " << thisSliceGen2_15[GAMMA_INDEX] << "% | " << thisSliceGen3_15[GAMMA_INDEX] << "% | " << 
            thisSliceGen4Plus_15[GAMMA_INDEX] << "% | " << std::endl;
    }
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "How did we do?" << std::endl;
    std::cout << nCorrectNuSlice_FlashMatch_15 << "% of events have correct Nu slice (flash match)..." << std::endl;
    std::cout << nCorrectNuSlice_Pandora_15 << "% of events have correct Nu slice (pandora)..." << std::endl;
    std::cout << nCorrectNuSlice_Both_15 << "% of events have correct Nu slice (both)..." << std::endl;
    std::cout << allThisSlice_15 << "% of events had all particles found in nu slice, where" << std::endl;
    std::cout << allThisSlicePlusGen_15 << "% also had correct hierarchy tiers" << std::endl;   
    std::cout << allThisSlicePlusID_15 << "% also had correct track/shower IDs" << std::endl;   
    std::cout << allThisSlicePlusGenPlusID_15 << "% also had correct hierachy tier & track/shower IDs" << std::endl;

    /////////////////////
    // Divide efficiency histograms
    /////////////////////
    
    // True hits
    //ObtainEfficiencyHistogramCollection(nTrueHitsThisSliceEff, nTrueHitsDists);
    //ObtainEfficiencyHistogramCollection(nTrueHitsOtherSliceEff, nTrueHitsDists);

    // True distance
    //ObtainEfficiencyHistogramCollection(nTrueDistanceThisSliceEff, nTrueDistanceDists);
    //ObtainEfficiencyHistogramCollection(nTrueDistanceOtherSliceEff, nTrueDistanceDists);    
    

    /////////////////////
    // Normalise histograms
    /////////////////////
    //NormaliseHistogramCollection(nTrueHitsDists);
    //NormaliseHistogramCollection(nTrueDistanceDists);
    //NormaliseHistogramCollection(trackScoreDists);
    //NormaliseHistogramCollection(completenessDists);
    //NormaliseHistogramCollection(purityDists);

    /////////////////////
    // Draw histograms
    /////////////////////
    TCanvas * c1 = new TCanvas("c1", "c1");

    // Distributions
    //DrawHistogramCollection(nTrueHitsDists, "", "nTrueHits", "Proportion of Events");
    //DrawHistogramCollection(nTrueDistanceDists, "", "TrueNuVertexSep", "Proportion of Events");
    //DrawHistogramCollection(trackScoreDists, "", "Track Score", "Proportion of Events");
    //DrawHistogramCollection(completenessDists, "", "Completeness", "Proportion of Events");
    //DrawHistogramCollection(purityDists, "", "Purity", "Proportion of Events");

    // Each particle type efficiency
    //TCanvas * cSliceEfficiency_MUON = new TCanvas("cSliceEfficiency_MUON", "cSliceEfficiency_MUON");
    //DrawSliceEfficiency(nTrueHitsDists, nTrueHitsThisSliceEff, nTrueHitsOtherSliceEff, MUON_INDEX);
    //DrawSliceEfficiency(nTrueDistanceDists, nTrueDistanceThisSliceEff, nTrueDistanceOtherSliceEff, MUON_INDEX);

    //TCanvas * cSliceEfficiency_PROTON = new TCanvas("cSliceEfficiency_PROTON", "cSliceEfficiency_PROTON");
    //DrawSliceEfficiency(nTrueHitsDists, nTrueHitsThisSliceEff, nTrueHitsOtherSliceEff, PROTON_INDEX);
    //DrawSliceEfficiency(nTrueDistanceDists, nTrueDistanceThisSliceEff, nTrueDistanceOtherSliceEff, PROTON_INDEX);

    //TCanvas * cSliceEfficiency_PION = new TCanvas("cSliceEfficiency_PION", "cSliceEfficiency_PION");
    //DrawSliceEfficiency(nTrueHitsDists, nTrueHitsThisSliceEff, nTrueHitsOtherSliceEff, PION_INDEX);
    //DrawSliceEfficiency(nTrueDistanceDists, nTrueDistanceThisSliceEff, nTrueDistanceOtherSliceEff, PION_INDEX);

    //TCanvas * cSliceEfficiency_GAMMA = new TCanvas("cSliceEfficiency_GAMMA", "cSliceEfficiency_GAMMA");
    //DrawSliceEfficiency(nTrueHitsDists, nTrueHitsThisSliceEff, nTrueHitsOtherSliceEff, GAMMA_INDEX);
    //DrawSliceEfficiency(nTrueDistanceDists, nTrueDistanceThisSliceEff, nTrueDistanceOtherSliceEff, GAMMA_INDEX);

    /*
    ofstream myfile;
    myfile.open ("runsubrunevent.txt");

    for (unsigned int i = 0; i < run.size(); ++i)
        myfile << run.at(i) << " " << subrun.at(i) << " " << event.at(i) << std::endl;
    */
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void ObtainEfficiencyHistogramCollection(const std::vector<TH1D*> &weightedHistogramCollection, 
    const std::vector<TH1D*> &fullHistogramCollection)
{
    for (unsigned int particleTypeIndex : MODE_PARTICLE_TYPE_INDICIES)
    {
        for (unsigned int bin = 1; bin <= weightedHistogramCollection[particleTypeIndex]->GetNbinsX(); ++bin)
        {
            const double weighted(weightedHistogramCollection[particleTypeIndex]->GetBinContent(bin));
            const double full(fullHistogramCollection[particleTypeIndex]->GetBinContent(bin));
            const double newBinContent(full < std::numeric_limits<double>::epsilon() ? 0.0 : weighted / full);

            weightedHistogramCollection[particleTypeIndex]->SetBinContent(bin, newBinContent);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void NormaliseHistogramCollection(const std::vector<TH1D*> &histogramCollection)
{
    for (int particleTypeIndex : MODE_PARTICLE_TYPE_INDICIES)
    {
        if (histogramCollection[particleTypeIndex]->Integral() > std::numeric_limits<float>::epsilon())
            histogramCollection[particleTypeIndex]->Scale(1.f / histogramCollection[particleTypeIndex]->Integral());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawHistogramCollection(const std::vector<TH1D*> &histogramCollection, const std::string &title,
    const std::string &xAxis, const std::string &yAxis)
{
    for (int particleTypeIndex : MODE_PARTICLE_TYPE_INDICIES)
    {
        std::string jam = title + ";" + xAxis + ";" + yAxis;
        histogramCollection[particleTypeIndex]->SetTitle(jam.c_str());
        histogramCollection[particleTypeIndex]->GetYaxis()->SetRangeUser(0.0, 1.0);
        histogramCollection[particleTypeIndex]->SetLineColor(LINE_COLOR[particleTypeIndex]);
        histogramCollection[particleTypeIndex]->SetLineWidth(2);
    }

    for (int particleTypeIndex : MODE_PARTICLE_TYPE_INDICIES)
        histogramCollection[particleTypeIndex]->Draw("hist same");

    TLegend * legend = new TLegend(0.1,0.7,0.48,0.9);

    for (int particleTypeIndex : MODE_PARTICLE_TYPE_INDICIES)
        legend->AddEntry(histogramCollection[particleTypeIndex], LEGEND_STRING[particleTypeIndex], "l");

    legend->Draw("same");

    //delete legend;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawSliceEfficiency(const std::vector<TH1D*> &fullHistogramCollection, const std::vector<TH1D*> &thisSliceEffCollection,
    const std::vector<TH1D*> &otherSliceEffCollection, const int particleTypeIndex)
{
    TH1D * totalEff = (TH1D*)thisSliceEffCollection[particleTypeIndex]->Clone();
    totalEff->Add(otherSliceEffCollection[particleTypeIndex]);

    thisSliceEffCollection[particleTypeIndex]->SetLineStyle(9);
    otherSliceEffCollection[particleTypeIndex]->SetLineStyle(2);
    totalEff->SetLineStyle(1);

    for (TH1D * hist : {thisSliceEffCollection[particleTypeIndex], otherSliceEffCollection[particleTypeIndex], totalEff})
    {
        hist->GetYaxis()->SetRangeUser(0.0, 1.0);
        hist->SetTitle(";nTrueHits;Efficiency");
        hist->SetLineWidth(2);
        hist->SetLineColor(LINE_COLOR[particleTypeIndex]);
    }

    //fullHistogramCollection[particleTypeIndex]->SetFillStyle(1001);
    fullHistogramCollection[particleTypeIndex]->SetFillColorAlpha(LINE_COLOR[particleTypeIndex], 0.2);
    fullHistogramCollection[particleTypeIndex]->GetYaxis()->SetRangeUser(0.0, 1.0);
    fullHistogramCollection[particleTypeIndex]->SetTitle(";nTrueHits;Efficiency");

    if (fullHistogramCollection[particleTypeIndex]->Integral() > std::numeric_limits<float>::epsilon())
        fullHistogramCollection[particleTypeIndex]->Scale(1.f / fullHistogramCollection[particleTypeIndex]->Integral());


    for (TH1D * hist : {thisSliceEffCollection[particleTypeIndex], otherSliceEffCollection[particleTypeIndex], fullHistogramCollection[particleTypeIndex]})
        hist->Draw("hist same");

    TLegend * legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(thisSliceEffCollection[particleTypeIndex], "Neutrino Slice", "l");
    legend->AddEntry(otherSliceEffCollection[particleTypeIndex], "Other Slice", "l");
    //legend->AddEntry(totalEff, "Total", "l");
    legend->AddEntry(fullHistogramCollection[particleTypeIndex], "Normalised True Distribution", "f");
    legend->Draw("same");

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
