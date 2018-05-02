#ifndef ATSIMULATEDPOINT_H
#define ATSIMULATEDPOINT_H

#include "TROOT.h"
#include "TObject.h"
#include "TVector3.h"


class ATSimulatedPoint : public TObject {
  public:
    ATSimulatedPoint();
    ATSimulatedPoint(Int_t electronNumber, Int_t amplitude, Double_t x, Double_t y, Double_t atime);
    ~ATSimulatedPoint();

    //!< Track ID setter
    void SetElectronNumber(Int_t electronNumber);
    //!< Position setter
    void SetPoint(Int_t electronNumber, Double_t x, Double_t y, Double_t atime);
    //!< Position setter
    void SetPosition(Double_t x, Double_t y, Double_t atime);


    //!< Track ID getter
    Int_t GetElectronNumber();
    Int_t GetAmplitude();
    //!< Position getter
    TVector3 GetPosition();

  private:
    //!< Track ID having this hit
    Int_t fElectronNumber;
    Int_t fAmplitude;
    //!< Position
    TVector3 fPosition;


  ClassDef(ATSimulatedPoint, 1);
};

#endif
