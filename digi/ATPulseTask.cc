#include "ATPulseTask.hh"
#include "ATHit.hh"

// Fair class header
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "ATVertexPropagator.h"
#include "ATPad.hh"
#include "ATSimulatedPoint.hh"

// STL class headers
#include <cmath>
#include <iostream>
#include <iomanip>

#include "TRandom.h"
#include "TMath.h"
#include "TF1.h"

#define cRED "\033[1;31m"
#define cYELLOW "\033[1;33m"
#define cNORMAL "\033[0m"
#define cGREEN "\033[1;32m"

void ATPulseTask::SetGeo(TString geofile)				                                               { fGeoFile = geofile; }
void ATPulseTask::SetProtoMap(TString mapfile)	                                               { fProtoMapFile = mapfile;}
void ATPulseTask::SetMapOpt(Int_t value)                                                       { fOpt = value; }
Bool_t ATPulseTask::SetMap(TString map)                                                        { fMap = map; }



ATPulseTask::ATPulseTask():FairTask("ATPulseTask"),
fEventID(0)
{

}

ATPulseTask::~ATPulseTask()
{
  fLogger->Debug(MESSAGE_ORIGIN,"Destructor of ATPulseTask");
}

void
ATPulseTask::SetParContainers()
{
  fLogger->Debug(MESSAGE_ORIGIN,"SetParContainers of ATAvalancheTask");

  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb = ana->GetRuntimeDb();
  fPar = (ATDigiPar*) rtdb->getContainer("ATDigiPar");
}

InitStatus
ATPulseTask::Init()
{
  fLogger->Debug(MESSAGE_ORIGIN,"Initilization of ATPulseTask");

  FairRootManager* ioman = FairRootManager::Instance();

fDriftedElectronArray = (TClonesArray *) ioman -> GetObject("ATSimulatedPoint");
  if (fDriftedElectronArray == 0) {
    fLogger -> Error(MESSAGE_ORIGIN, "Cannot find fDriftedElectronArray array!");
    return kERROR;
  }
  fRawEventArray  = new TClonesArray("ATRawEvent", 100);        //!< Raw Event array(only one)
  ioman -> Register("ATRawEvent", "cbmsim", fRawEventArray, fIsPersistent);

  fGain = fPar->GetGain();
  //std::cout<<"Gain: "<<fGain<<std::endl;

  // ***************Create ATTPC Pad Plane***************************
  // TString scriptfile = "LookupProto10Be.xml";
  // TString dir = getenv("VMCWORKDIR");
  // TString scriptdir = dir + "/scripts/"+ scriptfile;

      // fAtMapPtr = new AtTpcMap();
      // fAtMapPtr->GenerateATTPC();
      //Bool_t MapIn = fAtMapPtr->ParseXMLMap(scriptdir);
      fDetmap  =  new AtTpcProtoMap();
      fDetmap -> SetProtoMap(fProtoMapFile);
      fDetmap -> SetGeoFile(fGeoFile);
      fDetmap -> SetName("ATTPC_Proto");
      gROOT->GetListOfSpecials()->Add(fDetmap);
      fPadPlane = fDetmap->GetATTPCPlane("ATTPC_Proto");

  fEventID = 0;
  fRawEvent = NULL;

  return kSUCCESS;
}

struct vPad{
  Double_t RawADC[512];
  Int_t padnumb;
  Int_t trackID;
};

void
ATPulseTask::Exec(Option_t* option)
{
  fLogger->Debug(MESSAGE_ORIGIN,"Exec of ATPulseTask");
  gRandom->SetSeed(1.0);

  Int_t nMCPoints = fDriftedElectronArray->GetEntries();
  std::cout<<" ATPulseTask: Number of Points "<<nMCPoints<<std::endl;
  if(nMCPoints<10){
    fLogger->Warning(MESSAGE_ORIGIN, "Not enough hits for digitization! (<10)");
    return;
  }

  fRawEventArray -> Delete();
  fRawEvent = NULL;
  fRawEvent = (ATRawEvent*)fRawEventArray->ConstructedAt(0);

  Int_t size = fRawEventArray -> GetEntriesFast();
   Double_t e                       = 2.718;
   Double_t tau                     = 1; //shaping time (us)
   Double_t samplingtime            = 60;
   Double_t samplingrate            = 0.080; //us
   Double_t delay                   = 1.0; //us
   Double_t timeBucket[512]         = {0};
   Int_t counter                    = 0;
   Double_t output                  = 0;
   std::vector<vPad> padarray;
   Double_t output_sum              = 0;
   Int_t    tbcounter               = 0;
   Double_t c                       = 10;
   Int_t  vsize;
   Int_t cc                         = 0;
   Int_t amplitude;
   Double_t pBin, g, xElectron, yElectron, eTime, clusterNum, eventID;
   Int_t padNumber;
   TVector3 coord;
   Int_t pointnum;
   ATSimulatedPoint* dElectron;
   std::vector<Float_t> PadCenterCoord;
   TF1 *gain                        =  new TF1("gain", "4*(x/[0])*exp(-2*(x/[0]))", 0.1, 5000);//Polya distribution of gain
   gain->SetParameter(0, fGain);


 // ***************Create Time Buckets*******************************
   for(Int_t d = 0; d<512; d++){
     timeBucket[d] = d*samplingrate+delay;
   }
 // ***************Create Pulse for Each Electron********************
         //std::cout<<"Total number of entries: "<<cGREEN<<nEvents<<cNORMAL<<std::endl;
         //#pragma omp parallel for ordered schedule(dynamic,1)
       for(Int_t iEvents = 1; iEvents<nMCPoints; iEvents++){//for every electron
         dElectron                     = (ATSimulatedPoint*) fDriftedElectronArray -> At(iEvents);
         coord                         = dElectron->GetPosition();
         amplitude                     = dElectron->GetAmplitude();
         pointnum                      = dElectron->GetTrackID();
         xElectron                     = coord (0); //mm
         yElectron                     = coord (1); //mm
         eTime                         = coord (2); //us
         counter                       = 0;
         fPadPlane->Fill(xElectron,yElectron,c);
         pBin                          = fPadPlane->FindBin(xElectron,yElectron);
         padNumber                     = fDetmap->BinToPad(pBin);
         Double_t pointmem[1000][3]    = {0};
         Double_t digital[512]         = {0};

         //*******Create new element in padarray if there's a new pad******//
         TString check = kTRUE;
         vsize  = padarray.size();
         for(Int_t r = 0; r<vsize; r++){
           if(padNumber == padarray[r].padnumb) check = kFALSE;
         }

         if(check == kTRUE){
           padarray.push_back(vPad());
           padarray[vsize].padnumb = padNumber;
         }

         //if(iEvents % 1000 == 0)   std::cout<<"Number of Electrons Processed: "<<cRED<<iEvents<<cNORMAL<<std::endl;

         // *********Pulse Generation for each electron************
         for(Double_t j = eTime; j<eTime+10; j+=samplingrate/5.0){
           output                = exp(-3*((j-eTime)/tau))*sin((j-eTime)/tau)*pow((j-eTime)/tau,3)*amplitude;
           pointmem[counter][0]  = j;
           pointmem[counter][1]  = output;
           counter++;

           // **************Once a point is assigned a height in time, it assigns time to a time bucket********************
           for(Int_t k = 0; k<512; k++){//go through all time buckets
             if(j>=timeBucket[k] && j<timeBucket[k+1]){//if point on pulse is in this time bucket, assign it that time bucket
               pointmem[counter][2]  = k;
               break;
             }//end if for time buckets
           }//end assigning pulse to time buckets
         }//end plotting pulse function

         //*********Once pulse is generated, it adds points to the running average********
         Int_t pTimebucket  = pointmem[0][2];
         Int_t A            = 0;
         Int_t nOPoints     = 0;
         Double_t acum[512] = {0};
         while (A<1000){
           if(pTimebucket == pointmem[A][2]){
             acum[pTimebucket]+= pointmem[A][1];
             nOPoints++;
             A++;
           }
           else{
             digital[pTimebucket] = acum[pTimebucket]/nOPoints++;
             pTimebucket          = pointmem[A][2];
             nOPoints             = 0;
           }
         }

         // ********Adds pulse to output array**************
         vsize  = padarray.size();
         for(Int_t y = 0; y<vsize; y++){
           if(padarray[y].padnumb == padNumber){
             padarray[y].trackID=pointnum;
             for(Int_t del = 0; del<512; del++){//go through every time bucket
               if(digital[del] != 0)  padarray[y].RawADC[del] += digital[del];
             }
             break;
           }
         }
       }// end through all electrons

// ***************Set Pad and add to event**************
       vsize = padarray.size();
       Int_t thepad;
       for(Int_t q = 0; q<vsize; q++){
         g = gain->GetRandom();
         ATPad *pad = new ATPad();
         thepad = padarray[q].padnumb;
         if(thepad<256 && thepad>0){
           pad->SetPad(thepad);
           PadCenterCoord = fDetmap->CalcPadCenter(thepad);
           pad->SetValidPad(kTRUE);
           pad->SetPadXCoord(PadCenterCoord[0]);
           pad->SetPadYCoord(PadCenterCoord[1]);
           pad->SetPedestalSubtracted(kTRUE);
           pad->SetTrackID(padarray[q].trackID);
           for(Int_t p = 0; p<512; p++){
             pad->SetADC(p, padarray[q].RawADC[p]*g);
           }
           fRawEvent->SetPad(pad);
           fRawEvent->SetEventID(fEventID);
         }
       }
           fEventID++;
           padarray.clear();
  //return;
}

ClassImp(ATPulseTask);
