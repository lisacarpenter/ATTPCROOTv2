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
  TCanvas *c2 = new TCanvas("c2","c2",200,10,2000,500);
  //c2->Divide(1,2);
  TCanvas *c3 = new TCanvas("c3","c3",200,50,2000,500);
  c3->Divide(4,1);

  TH1D *meshSignal = new TH1D("MeshSignal","MeshSignal",512,0,512);

  TH2D *Quad0 = new TH2D("Quad0", "Quad0",300,0,300,512,0,512);
  TH2D *Quad1 = new TH2D("Quad1", "Quad1",300,0,300,512,0,512);
  TH2D *Quad2 = new TH2D("Quad2", "Quad2",300,0,300,512,0,512);
  TH2D *Quad3 = new TH2D("Quad3", "Quad3",300,0,300,512,0,512);
  Quad0->SetMarkerStyle(20);
  Quad0->SetMarkerSize(0.5);
  Quad1->SetMarkerStyle(20);
  Quad1->SetMarkerSize(0.5);
  Quad2->SetMarkerStyle(20);
  Quad2->SetMarkerSize(0.5);
  Quad3->SetMarkerStyle(20);
  Quad3->SetMarkerSize(0.5);

  TString FileName = "allpads/10Be_2013_run0021.root";
  std::cout<<" Opening File : "<<FileName.Data()<<std::endl;
  TFile* file = new TFile(FileName.Data(),"READ");

  TTree* tree = (TTree*) file -> Get("cbmsim");
  Int_t nEvents = tree -> GetEntriesFast();
  std::cout<<" Number of events : "<<nEvents<<std::endl;

  TTreeReader Reader1("cbmsim", file);
  TTreeReaderValue<TClonesArray> rawEventArray(Reader1, "ATRawEvent");

  Double_t threshold = 75.0;
  Int_t nEve=0;
  while (Reader1.Next()) {
    ATRawEvent* rawEvent = (ATRawEvent*) rawEventArray->At(0);
    for(Int_t i=0;i<meshSignal->GetSize();i++){
      //meshSignal->SetBinContent(i,0.0);
    }
    if(nEve==1943){
      std::vector<ATPad> *padVec=rawEvent->GetPads();
      cout<<padVec->size()<<endl;
      for(Int_t j=0;j<padVec->size();j++){
        for(Int_t k=0;k<512;k++){
          if(padVec->at(j).GetADC(k)>10)meshSignal->AddBinContent(meshSignal->FindBin(k),padVec->at(j).GetADC(k));
          if(padVec->at(j).GetADC(k)>10)cout<<padVec->at(j).GetPadNum()<<"\t"<<k<<"\t"<<padVec->at(j).GetADC(k)<<endl;
          //cout<<meshSignal->GetBinContent(k)<<endl;
          if(padVec->at(j).GetADC(k)>threshold){
            Double_t x = padVec->at(j).GetPadXCoord();
            Double_t y = padVec->at(j).GetPadYCoord();
            //cout<<x<<"\t"<<y<<"\t"<<k<<endl;
            if(y>0.0){
              if(x>0.0){
                Quad0->SetBinContent(Quad0->FindBin(padVec->at(j).GetPadNum(),k),padVec->at(j).GetADC(k));
              }
              else{
                Quad1->SetBinContent(Quad1->FindBin(padVec->at(j).GetPadNum(),k),padVec->at(j).GetADC(k));
              }
            }
            else{
              if(x>0.0){
                Quad3->SetBinContent(Quad3->FindBin(padVec->at(j).GetPadNum(),k),padVec->at(j).GetADC(k));
              }
              else{
                Quad2->SetBinContent(Quad2->FindBin(padVec->at(j).GetPadNum(),k),padVec->at(j).GetADC(k));
              }
            }
          }
        }
      }
     break;
    }

    nEve++;
  }
  c2->cd();
  meshSignal->Draw();

   c3->cd(1);
   Quad0->Draw("colz");
   c3->cd(2);
   Quad1->Draw("colz");
   c3->cd(3);
   Quad2->Draw("colz");
   c3->cd(4);
   Quad3->Draw("colz");

}
