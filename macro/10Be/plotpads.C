#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TH1.h"
#include "TH3.h"
#include "TClonesArray.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TClonesArray.h"
#include "TStyle.h"
#include "TTreeReader.h"
#include "TChain.h"
#include "TFileCollection.h"
#include "TError.h"
#include "TMinuit.h"

#include <ios>
#include <iostream>
#include <istream>
#include <limits>

void plotpads()
{
  TCanvas *c2 = new TCanvas("c2","c2",200,10,700,700);
  //c2->Divide(1,2);

  TH1D *meshSignal = new TH1D("MeshSignal","MeshSignal",512,0,512);

  TString FileName = "output_proto6.root";
  std::cout<<" Opening File : "<<FileName.Data()<<std::endl;
  TFile* file = new TFile(FileName.Data(),"READ");

  TTree* tree = (TTree*) file -> Get("cbmsim");
  Int_t nEvents = tree -> GetEntriesFast();
  std::cout<<" Number of events : "<<nEvents<<std::endl;

  TTreeReader Reader1("cbmsim", file);
  TTreeReaderValue<TClonesArray> rawEventArray(Reader1, "ATRawEvent");
  Int_t nEve=0;
  while (Reader1.Next()) {
    ATRawEvent* rawEvent = (ATRawEvent*) rawEventArray->At(0);
    for(Int_t i=0;i<meshSignal->GetSize();i++){
      //meshSignal->SetBinContent(i,0.0);
    }
    if(nEve==848){
      std::vector<ATPad> *padVec=rawEvent->GetPads();
      cout<<padVec->size()<<endl;
      for(Int_t j=0;j<padVec->size();j++){
        for(Int_t k=0;k<512;k++){
          meshSignal->AddBinContent(meshSignal->FindBin(k),padVec->at(j).GetRawADC(k));
          //cout<<padVec->at(j).GetADC(k)<<endl;
          //cout<<meshSignal->GetBinContent(k)<<endl;
        }
      }
      // for(Int_t tb=1;tb<511;tb++){
      //   Double_t a = meshSignal->GetBinContent(tb-1);
      //   Double_t b = meshSignal->GetBinContent(tb);
      //   Double_t c = meshSignal->GetBinContent(tb+1);
      //   meshSignal->SetBinContent(tb, (c-a)/2);
      // }
    }

    nEve++;
  }
  c2->cd(1);
  meshSignal->Draw();

}
