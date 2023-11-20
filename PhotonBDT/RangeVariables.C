int RangeNHits3D(const int nHits3D);
int RangeNHits2D(const int nHits2D);
double RangeNuVertexSeparation(const double nuVertexSeparation);
double RangeDCA(const double dca);
double RangeNuVertexChargeDistribution(const double nuVertexChargeDist);
double RangeInitialdEdx(const double initialdEdx);
double RangeTrackScore(const double trackScore);
double RangeOpeningAngle(const double openingAngle);
double RangeShowerLength(const double showerLength);
double RangeTrackParentSeparation(const double trackParentSep);
double RangeChiPIDProton(const double chiPIDProton);
double RangeChiPIDMuon(const double chiPIDMuon);
double RangeChiPIDPion(const double chiPIDPion);
double RangeBraggPIDProton(const double braggPIDProton);
double RangeBraggPIDMuon(const double braggPIDMuon);
double RangeBraggPIDPion(const double braggPIDPion);
double RangeLLRPID(const double llrPID);

////////////////////////////////////////////////////////////////////////////////// 

int RangeNHits3D(const int nHits3D)
{
    const int nHits3DMin = -1;
    const int nHits3DMax = 300;

    return std::max(std::min(nHits3D, nHits3DMax), nHits3DMin);
}

//////////////////////////////////////////////////////////////////////////////////

int RangeNHits2D(const int nHits2D)
{
    const int nHits2DMin = -1;
    const int nHits2DMax = 300;

    return std::max(std::min(nHits2D, nHits2DMax), nHits2DMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeNuVertexSeparation(const double nuVertexSeparation)
{
    const double nuVertexSeparationMin = -1.0;
    const double nuVertexSeparationMax = 100.0;

    return std::max(std::min(nuVertexSeparation, nuVertexSeparationMax), nuVertexSeparationMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeDCA(const double dca)
{
    const double dcaMin = -1.0;
    const double dcaMax = 60.0;

    return std::max(std::min(dca, dcaMax), dcaMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeNuVertexChargeDistribution(const double nuVertexChargeDist)
{
    const double nuVertexChargeDistMin = -1.0;
    const double nuVertexChargeDistMax = 10.0;

    return std::max(std::min(nuVertexChargeDist, nuVertexChargeDistMax), nuVertexChargeDistMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeInitialdEdx(const double initialdEdx)
{
    const double initialdEdxMin = -1.0;
    const double initialdEdxMax = 10.0;

    return std::max(std::min(initialdEdx, initialdEdxMax), initialdEdxMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeTrackScore(const double trackScore)
{
    const double trackScoreMin = -1.0;
    const double trackScoreMax = 1.0;

    return std::max(std::min(trackScore, trackScoreMax), trackScoreMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeOpeningAngle(const double openingAngle)
{
    const double openingAngleMin = -1.0;
    const double openingAngleMax = 50.0;

    return std::max(std::min(openingAngle, openingAngleMax), openingAngleMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeShowerLength(const double showerLength)
{
    const double showerLengthMin = -1.0;
    const double showerLengthMax = 200.0;

    return std::max(std::min(showerLength, showerLengthMax), showerLengthMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeTrackParentSeparation(const double trackParentSep)
{
    const double trackParentSepMin = -1.0;
    const double trackParentSepMax = 2.0;

    return std::max(std::min(trackParentSep, trackParentSepMax), trackParentSepMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeChiPIDProton(const double chiPIDProton)
{
    const double chiPIDProtonMin = -20.0;
    const double chiPIDProtonMax = 250.0;

    if (TMath::IsNaN(chiPIDProton))
        return chiPIDProtonMin;

    return std::max(std::min(chiPIDProton, chiPIDProtonMax), chiPIDProtonMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeChiPIDMuon(const double chiPIDMuon)
{
    const double chiPIDMuonMin = -20.0;
    const double chiPIDMuonMax = 100.0;

    if (TMath::IsNaN(chiPIDMuon))
        return chiPIDMuonMin;

    return std::max(std::min(chiPIDMuon, chiPIDMuonMax), chiPIDMuonMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeChiPIDPion(const double chiPIDPion)
{
    const double chiPIDPionMin = -20.0;
    const double chiPIDPionMax = 100.0;

    if (TMath::IsNaN(chiPIDPion))
        return chiPIDPionMin;

    return std::max(std::min(chiPIDPion, chiPIDPionMax), chiPIDPionMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeBraggPIDProton(const double braggPIDProton)
{
    const double braggPIDProtonMin = -1.0;
    const double braggPIDProtonMax = 0.5;

    if (TMath::IsNaN(braggPIDProton))
        return braggPIDProtonMin;

    return std::max(std::min(braggPIDProton, braggPIDProtonMax), braggPIDProtonMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeBraggPIDMuon(const double braggPIDMuon)
{
    const double braggPIDMuonMin = -1.0;
    const double braggPIDMuonMax = 1.0;

    if (TMath::IsNaN(braggPIDMuon))
        return braggPIDMuonMin;

    return std::max(std::min(braggPIDMuon, braggPIDMuonMax), braggPIDMuonMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeBraggPIDPion(const double braggPIDPion)
{
    const double braggPIDPionMin = -1.0;
    const double braggPIDPionMax = 1.0;

    if (TMath::IsNaN(braggPIDPion))
        return braggPIDPionMin;

    return std::max(std::min(braggPIDPion, braggPIDPionMax), braggPIDPionMin);
}

//////////////////////////////////////////////////////////////////////////////////

double RangeLLRPID(const double llrPID)
{
    const double llrPIDMin = -2.0;
    const double llrPIDMax = 1.0;

    if (TMath::IsNaN(llrPID))
        return llrPIDMin;

    return std::max(std::min(llrPID, llrPIDMax), llrPIDMin);
}
