#include "ATClusterizeTask.hh"

// Fair class header
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "AtTpcPoint.h"
#include "ATVertexPropagator.h"
#include "ATSimulatedPoint.hh"

// STL class headers
#include <cmath>
#include <iostream>
#include <iomanip>

#include "TRandom.h"
#include "TMath.h"
#include "TF1.h"


ATClusterizeTask::ATClusterizeTask():FairTask("ATClusterizeTask"),
fEventID(0),
fIsPersistent(kFALSE)
{

}

ATClusterizeTask::~ATClusterizeTask()
{
  fLogger->Debug(MESSAGE_ORIGIN,"Destructor of ATClusterizeTask");
}

void
ATClusterizeTask::SetParContainers()
{
  fLogger->Debug(MESSAGE_ORIGIN,"SetParContainers of ATAvalancheTask");

  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  fPar = (ATDigiPar*) rtdb->getContainer("ATDigiPar");
}

InitStatus
ATClusterizeTask::Init()
{
  fLogger->Debug(MESSAGE_ORIGIN,"Initilization of ATClusterizeTask");

  FairRootManager* ioman = FairRootManager::Instance();

  fMCPointArray = (TClonesArray*) ioman->GetObject("AtTpcPoint");
  if (fMCPointArray == 0) {
    fLogger -> Error(MESSAGE_ORIGIN, "Cannot find fMCPointArray array!");
    return kERROR;
  }

  fElectronNumberArray = new TClonesArray("ATSimulatedPoint");
  ioman -> Register("ATSimulatedPoint", "cbmsim",fElectronNumberArray, fIsPersistent);


  fEIonize  = fPar->GetEIonize()/1000000.0; // [MeV]
  fVelDrift = fPar->GetDriftVelocity(); // [cm/us]
  fCoefT    = fPar->GetCoefDiffusionTrans()*sqrt(10.); // [cm^(-1/2)] to [mm^(-1/2)]
  fCoefL    = fPar->GetCoefDiffusionLong()*sqrt(10.);  // [cm^(-1/2)] to [mm^(-1/2)]
  return kSUCCESS;
}

void
ATClusterizeTask::Exec(Option_t* option)
{
  fLogger->Debug(MESSAGE_ORIGIN,"Exec of ATClusterizeTask");
  Int_t nMCPoints = fMCPointArray->GetEntries();
  std::cout<<"ATClusterizeTask: Number of MC Points "<<nMCPoints<<std::endl;
  if(nMCPoints<10){
    fLogger->Warning(MESSAGE_ORIGIN, "Not enough hits for digitization! (<10)");
    return;
  }

  /**
   * NOTE! that fMCPoint has unit of [cm] for length scale,
   * [GeV] for energy and [ns] for time.
   */
   fElectronNumberArray->Delete();

   Double_t  energyLoss_sca=0.0;
   Double_t  energyLoss_rec=0.0;
   Double_t  x = 0;
   Double_t  y = 0;
   Double_t  z = 0;
   Double_t  fano = 2.0;
   Int_t     nElectrons   = 0;
   Double_t     eFlux        = 0;
   Int_t     genElectrons = 0;
   //Double_t  eIonize      = 15.603/1000; //Ionization energy (MeV)
   TString   VolName;
   Double_t  tTime, entries;

   Double_t zMesh          = 500; //mm (No tilt)
   Double_t coefDiffusion  = 0.01; //from ATMCQMinimization.cc
   //Double_t driftVelocity  = 2; //cm/us for hydrogen
   //Double_t coefT          = 0.010;
   //Double_t coefL          = 0.025;
   Double_t driftLength;
   Double_t driftTime;
   Double_t propX;
   Double_t propY;
   Double_t sigmaDiffusion;
   Double_t sigstrtrans, sigstrlong, phi, r;
   Int_t    electronNumber  = 0;
   //TF1 *trans      = new TF1("trans", "x*pow(2.718,(-pow(x,2))/[0])", 0, 1);


   for(Int_t i=0; i<nMCPoints; i++) {
         fMCPoint = (AtTpcPoint*) fMCPointArray-> At(i);
         VolName=fMCPoint->GetVolName();
         Int_t trackID  = fMCPoint->GetTrackID();
         if(VolName == "drift_volume"){
           x                 = fMCPoint->GetXIn()*10; //mm
           y                 = fMCPoint->GetYIn()*10; //mm
           z                 = 0.0-(fMCPoint->GetZIn()*10); //mm
           energyLoss_rec    =(fMCPoint -> GetEnergyLoss() )*1000;//MeV
           nElectrons        = int(floor(energyLoss_rec/fEIonize)); //mean electrons generated
           eFlux             = TMath::Sqrt(fano*nElectrons);//fluctuation of generated electrons
           if(nElectrons>10){
             genElectrons      = (int)(gRandom->Gaus(nElectrons, eFlux));//generated electrons
           }
           else{
             genElectrons      = (int)(gRandom->Poisson(nElectrons));//generated electrons
           }

           //std::cout<<"gen electrons \t"<<genElectrons<<std::endl;
           driftLength       = abs(z-zMesh); //mm
           sigstrtrans       = fCoefT* sqrt(driftLength);//transverse diffusion coefficient
           sigstrlong        = fCoefL* sqrt(driftLength);//longitudal diffusion coefficient
           //trans->SetParameter(0, sigstrtrans);

               //r               = trans->GetRandom(); //non-Gaussian cloud
		           r               = gRandom -> Gaus(0,sigstrtrans); //Gaussian cloud
               phi             = gRandom->Uniform(0, TMath::TwoPi());
               propX           = x + r*TMath::Cos(phi);
               propY           = y + r*TMath::Sin(phi);
               driftLength     = driftLength + (gRandom -> Gaus(0,sigstrlong)); //mm
               driftTime       = ((driftLength/10.0)/fVelDrift); //us
               electronNumber  +=1;

               //Fill container ATSimulatedPoint
               Int_t size = fElectronNumberArray -> GetEntriesFast();
               ATSimulatedPoint* simpoint
                 = new((*fElectronNumberArray)[size]) ATSimulatedPoint(electronNumber,  //electron #
                                                                      trackID,
                                                                      genElectrons,
                                                                      propX,  //X
                                                                      propY,  //Y
                                                                      driftTime);  //Z

     }//end if drift volume
 }//end through all interaction points


  return;
}

ClassImp(ATClusterizeTask);
