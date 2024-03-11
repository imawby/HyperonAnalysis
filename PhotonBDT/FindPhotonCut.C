void FindPhotonCut()
{
    TFile * file = new TFile("/uboone/data/users/imawby/PhotonBDT/photonBDTFiles.root", "READ");

    TTree * signalTree = (TTree*)file->Get("photonBDT/PhotonTree");
    TTree * backgroundTree = (TTree*)file->Get("photonBDT/BackgroundTree");

    float photonScore_signal;
    int isTrueSigmaPhoton_signal;

    float photonScore_background;
    int isTrueSigmaPhoton_background;

    signalTree->SetBranchAddress("PhotonBDTScore", &photonScore_signal);
    signalTree->SetBranchAddress("IsTrueSigmaPhoton", &isTrueSigmaPhoton_signal);

    backgroundTree->SetBranchAddress("PhotonBDTScore", &photonScore_background);
    backgroundTree->SetBranchAddress("IsTrueSigmaPhoton", &isTrueSigmaPhoton_background);

    // Set up histograms, and fill them
    int nBins = 24;
    float min = -1.2, max = 1.2;

    TH1F * scores_sigmaSignal = new TH1F("scores_sigmaSignal", "scores_sigmaSignal", nBins, min, max);
    TH1F * scores_signal = new TH1F("scores_signal", "scores_signal", nBins, min, max);
    TH1F * scores_background = new TH1F("scores_background", "scores_background", nBins, min, max);

    float totalSignal = 0.f;
    float totalSigmaSignal = 0.f;

    for (unsigned int i = 0; i < signalTree->GetEntries(); ++i)
    {
        signalTree->GetEntry(i);

        ++totalSignal;
        scores_signal->Fill(photonScore_signal);

        if (isTrueSigmaPhoton_signal == 1)
        {
            ++totalSigmaSignal;
            scores_sigmaSignal->Fill(photonScore_signal);
        }
    }

    for (unsigned int i = 0; i < backgroundTree->GetEntries(); ++i)
    {
        backgroundTree->GetEntry(i);

        scores_background->Fill(photonScore_background);
    }

    double efficiency_signal[nBins];
    double efficiency_sigmaSignal[nBins];
    double purity_signal[nBins];
    double purity_sigmaSignal[nBins];
    double eXp_signal[nBins];
    double eXp_sigmaSignal[nBins];
    double cutValues[nBins];

    // Find cuts...
    float bestEXP_signal = -1.0;
    float bestEXP_sigmaSignal = -1.0;

    float cut_signal = -1.0;
    float cut_sigmaSignal = -1.0;

    for (unsigned int i = 1; i <= nBins; ++i)
    {
        float selectedSignal = 0;
        float selectedSigmaSignal = 0;
        float selectedBackground = 0;

        cutValues[(i - 1)] = scores_signal->GetBinLowEdge(i);

        // include over/underflow
        for (unsigned int j = 0; j <= (nBins+1); ++j)
        {
            if (j >= i)
            {
                selectedSignal += scores_signal->GetBinContent(j);    
                selectedSigmaSignal += scores_sigmaSignal->GetBinContent(j);
                selectedBackground += scores_background->GetBinContent(j);
            }
        }

        const float selected = selectedSignal + selectedBackground;
        const float selected_sigma = selectedSigmaSignal + selectedBackground;
        const float signalEfficiency = totalSignal < std::numeric_limits<float>::epsilon() ? 0.f : selectedSignal / totalSignal;
        const float sigmaSignalEfficiency = totalSigmaSignal < std::numeric_limits<float>::epsilon() ? 0.f : selectedSigmaSignal / totalSigmaSignal;
        const float signalPurity = selected < std::numeric_limits<float>::epsilon() ? 0.f : selectedSignal / selected;
        const float sigmaSignalPurity = selected_sigma < std::numeric_limits<float>::epsilon() ? 0.f : selectedSigmaSignal / selected_sigma;

        std::cout << "-----------" << std::endl;
        std::cout << "selectedSigmaSignal: " <<selectedSigmaSignal << std::endl;
        std::cout << "selectedBackground: " << selectedBackground << std::endl;
        std::cout << "sigmaSignalPurity: " << sigmaSignalPurity << std::endl;
        std::cout << "-----------" << std::endl;

        efficiency_signal[(i - 1)] = signalEfficiency;
        efficiency_sigmaSignal[(i - 1)] = sigmaSignalEfficiency;
        purity_signal[(i - 1)] = signalPurity;
        purity_sigmaSignal[(i - 1)] = sigmaSignalPurity;

        eXp_signal[(i - 1)] = signalEfficiency * signalPurity;
        eXp_sigmaSignal[(i - 1)] = sigmaSignalEfficiency * sigmaSignalPurity;

        if (eXp_signal[(i - 1)] > bestEXP_signal)
        {
            bestEXP_signal = eXp_signal[(i - 1)];
            cut_signal = cutValues[(i - 1)];
        }

        if (eXp_sigmaSignal[(i - 1)] > bestEXP_sigmaSignal)
        {
            bestEXP_sigmaSignal = eXp_sigmaSignal[(i - 1)];
            cut_sigmaSignal = cutValues[(i - 1)];
        }
    }


    TGraph * efficiencyGraph_signal = new TGraph(nBins, cutValues, efficiency_signal);
    TGraph * efficiencyGraph_sigmaSignal = new TGraph(nBins, cutValues, efficiency_sigmaSignal);
    TGraph * purityGraph_signal = new TGraph(nBins, cutValues, purity_signal);
    TGraph * purityGraph_sigmaSignal = new TGraph(nBins, cutValues, purity_sigmaSignal);
    TGraph * eXpGraph_signal = new TGraph(nBins, cutValues, eXp_signal);
    TGraph * eXpGraph_sigmaSignal = new TGraph(nBins, cutValues, eXp_sigmaSignal);

    TCanvas * c1 = new TCanvas("signal", "signal");
    efficiencyGraph_signal->SetLineColor(kBlue);
    efficiencyGraph_signal->SetTitle("PhotonSignal;Photon Cut;Arbitrary Units");
    efficiencyGraph_sigmaSignal->GetYaxis()->SetRangeUser(0.0, 1.0);
    efficiencyGraph_signal->Draw("AC");
    purityGraph_signal->SetLineColor(kRed);
    purityGraph_signal->Draw("C SAME");
    eXpGraph_signal->SetLineColor(kBlack);
    eXpGraph_signal->Draw("C SAME");

    TCanvas * c2 = new TCanvas("sigmaSignal", "sigmaSignal");
    efficiencyGraph_sigmaSignal->SetLineColor(kBlue);
    efficiencyGraph_sigmaSignal->SetTitle("SigmaPhotonSignal;Photon Cut;Arbitrary Units");
    efficiencyGraph_sigmaSignal->GetYaxis()->SetRangeUser(0.0, 1.0);
    efficiencyGraph_sigmaSignal->Draw("AC");
    purityGraph_sigmaSignal->SetLineColor(kRed);
    purityGraph_sigmaSignal->Draw("C SAME");
    eXpGraph_sigmaSignal->SetLineColor(kBlack);
    eXpGraph_sigmaSignal->Draw("C SAME");

    std::cout << "bestEXP_signal: " << bestEXP_signal << std::endl;
    std::cout << "cut_signal: " << cut_signal << std::endl;
    std::cout << "bestEXP_sigmaSignal: " << bestEXP_sigmaSignal << std::endl;
    std::cout << "cut_sigmaSignal: " << cut_sigmaSignal << std::endl;
}
