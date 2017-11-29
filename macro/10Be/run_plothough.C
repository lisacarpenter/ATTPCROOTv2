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
#include "TRandom.h"

#include <ios>
#include <iostream>
#include <istream>
#include <limits>

double avg(std::vector<Double_t> *v) {
  return 1.0 * std::accumulate(v->begin(), v->end(), 0LL) / v->size();
}


void run_plothough(TString FileNameHead = "output_protoh"){
  TString FilePath = "/home/lisa/fair_install/ATTPCROOTv2/macro/10Be/";
  TString FileNameTail = ".root";
  TString FileName     = FilePath + FileNameHead + FileNameTail;
  std::cout<<" Opening File : "<<FileName.Data()<<std::endl;
  TFile* file = new TFile(FileName.Data(),"READ");
  TTree* tree = (TTree*) file -> Get("cbmsim");
  Int_t nEvents = tree -> GetEntriesFast();
  std::cout<<" Number of events : "<<nEvents<<std::endl;


  TCanvas *c2 = new TCanvas("c2","c2",200,10,700,700);
  c2->Divide(2,1);
  TCanvas *c3 = new TCanvas("c3","c3",200,10,700,700);
  c3->Divide(2,1);
  TCanvas *c4 = new TCanvas("c4","c4",200,10,700,700);
  c4->Divide(2,1);
  TCanvas *c5 = new TCanvas("c5","c5",200,10,700,700);
  c5->Divide(3,1);
  TCanvas *c6 = new TCanvas("c6","c6",200,10,700,700);
  c6->Divide(2,2);

  TH2D* Q02_Kine = new TH2D("Q02_Kine","Q02_Kine",180,0,180,180,0,180);
  Q02_Kine->SetMarkerColor(0);
  Q02_Kine->SetMarkerStyle(20);
  Q02_Kine->SetMarkerSize(0.7);

  TH2D* Excitation_EL = new TH2D("Elastic", "Elastic",36,0,180,500,-1000,1000);
  TH2D* Excitation_IN = new TH2D("Inelastic", "Inelastic",36,0,180,250,0,500);

  TH2D* PhiCompare = new TH2D("PhiCompare","PhiCompare",90,0,90,90,0,90);
  TH1D* PhiCompare1 = new TH1D("PhiCompare1","PhiCompare1",180,-90,90);


  TString kinfile="Kine195.txt";
  std::ifstream *kineStr = new std::ifstream(kinfile.Data());
  Int_t numKin=0;

  Double_t *ThetaCMS = new Double_t[20000];
  Double_t *ThetaLabRec = new Double_t[20000];
  Double_t *EnerLabRec = new Double_t[20000];
  Double_t *ThetaLabSca = new Double_t[20000];
  Double_t *EnerLabSca = new Double_t[20000];


  if(!kineStr->fail()){
    while(!kineStr->eof()){
      *kineStr>>ThetaCMS[numKin]>>ThetaLabRec[numKin]>>EnerLabRec[numKin]>>ThetaLabSca[numKin]>>EnerLabSca[numKin];
      numKin++;
    }
  }else if(kineStr->fail()) std::cout<<" Warning : No Kinematics file found for this reaction! Please run the macro on $SIMPATH/macro/Kinematics/Decay_kinematics/Mainrel.cxx"<<std::endl;

  TGraph *Kine_AngRec_AngSca = new TGraph(numKin,ThetaLabRec,ThetaLabSca);
  Kine_AngRec_AngSca->SetLineColor(6);
  TGraph *Kine_AngRec_AngSca_vert = new TGraph(numKin,ThetaLabSca,ThetaLabRec);
  Kine_AngRec_AngSca_vert->SetLineColor(6);

  TCutG* pidcut = new TCutG("pidcut",4);
  pidcut->SetVarX("dE");
  pidcut->SetVarY("E");
  pidcut->SetTitle("Good PID");
  pidcut->SetPoint(0,1026.36,284564.0);
  pidcut->SetPoint(1,1026.36,128353.0);
  pidcut->SetPoint(2,2697.63,192181.0);
  pidcut->SetPoint(3,2697.63,247611.0);
  pidcut->SetPoint(4,1026.36,284564.0);

  TCutG* wideElasticup = new TCutG("wideElasticup",20);
  wideElasticup->SetVarX("Angle0");
  wideElasticup->SetVarY("Angle2");
  wideElasticup->SetTitle("Graph");
  wideElasticup->SetLineColor(2);
  wideElasticup->SetPoint(0,90,-2);
  wideElasticup->SetPoint(1,80.0396,3.645);
  wideElasticup->SetPoint(2,70.0304,9.0978);
  wideElasticup->SetPoint(3,60.0237,14.0652);
  wideElasticup->SetPoint(4,50.0202,18.1865);
  wideElasticup->SetPoint(5,40.0204,20.9176);
  wideElasticup->SetPoint(6,30.0241,21.4013);
  wideElasticup->SetPoint(7,20.3522,18.0306);
  wideElasticup->SetPoint(8,12.4072,8.0392);
  wideElasticup->SetPoint(9,0.065432,-1.95094);
  wideElasticup->SetPoint(10,0.065432,1.95094);
  wideElasticup->SetPoint(11,12.4072,12.0392);
  wideElasticup->SetPoint(12,20.3522,22.0306);
  wideElasticup->SetPoint(13,30.0241,25.4013);
  wideElasticup->SetPoint(14,40.0204,24.9176);
  wideElasticup->SetPoint(15,50.0202,22.1865);
  wideElasticup->SetPoint(16,60.0237,18.0652);
  wideElasticup->SetPoint(17,70.0304,13.0978);
  wideElasticup->SetPoint(18,80.0396,7.645);
  wideElasticup->SetPoint(19,90,2);
  wideElasticup->SetPoint(20,90,-2);

  Int_t goodnums[]={368, 546, 918, 1943, 2844, 3081, 4457, 7081, 7637, 7835, 11231, 11308, 13623, 13896, 14140, 14602, 14649, 15256, 17781, 18092, 18659, 20274, 20678, 21208, 22827, 26088, 26902, 31502, 32550, 36911, 38328,38454, 38991, 40353, 41674, 42665, 43057, 47482, 50423, 50772, 50957, 52889, 52994, 54162, 59071, 59640, 60900, 61144, 61310, 62143, 63112, 65759, 65993, 66440, 66548, 66729, 69168, 69269, 70682, 71436, 73422, 73424, 73854, 73979, 74732, 75218, 76747, 78723, 79641, 80995, 82782, 83837, 84318, 86693, 86726, 87468, 88570, 89956, 91337, 91637, 92300, 92650};
  std::vector<int> v(goodnums,goodnums+83);

  Double_t x1;
  Double_t y1;
  Double_t x2;
  Double_t y2;

  TTreeReader Reader1("cbmsim", file);
  TTreeReaderValue<TClonesArray> protoeventArray(Reader1, "ATProtoEvent");
  TTreeReaderValue<TClonesArray> analysisArray(Reader1, "ATProtoEventAna");
  TTreeReaderValue<TClonesArray> eventArray(Reader1, "ATEventH");

  Int_t nEve =0;
  while (Reader1.Next()){
    x1=0;
    y1=0;
    x2=0;
    y2=0;
    ATProtoEventAna* analysis = (ATProtoEventAna*) analysisArray->At(0);
    ATProtoEvent* protoevent = (ATProtoEvent*) protoeventArray->At(0);
    ATEvent* event = (ATEvent*) eventArray->At(0);
    std::vector<Double_t> *AngleFit = analysis->GetAngleFit();
    std::vector<Double_t> *vertex   = analysis->GetVertex();
    Float_t *MeshArray              = event->GetMesh();
    std::vector<ATProtoQuadrant> *quadvec   = protoevent->GetQuadrantArray();
    if(AngleFit->at(2)>AngleFit->at(0)) {
      x1 = AngleFit->at(2);
      y1 = AngleFit->at(0);
    }
    if(AngleFit->at(0)>AngleFit->at(2)) {
      y1 = AngleFit->at(0);
      x1 = AngleFit->at(2);
    }
    if(AngleFit->at(3)>AngleFit->at(1)){
      x2 = AngleFit->at(3);
      y2 = AngleFit->at(1);
    }
    if(AngleFit->at(1)>AngleFit->at(3)){
      y2 = AngleFit->at(1);
      x2 = AngleFit->at(3);
    }

    Double_t totalE = 0;
    Double_t deltaE = 0;
    for(Int_t i=0;i<410;i++){
      totalE = totalE+MeshArray[i];
      if(i>400){
        deltaE=deltaE+MeshArray[i];
      }
    }

    TH1D* PhiDistr[5];
    Char_t phihistname[256];
    for(Int_t i=0;i<5;i++){
      sprintf(phihistname,"PhiDistr_%d",i);
      PhiDistr[i] = new TH1D(phihistname,phihistname,9,0,90);
      PhiDistr[i]->SetName(phihistname);
      PhiDistr[i]->SetTitle(phihistname);
      if(i==0) {
        PhiDistr[i]->SetLineColor(kRed);
        PhiDistr[i]->SetMarkerColor(kRed);
      }
      else if(i==1) {
        PhiDistr[i]->SetLineColor(kBlue);
        PhiDistr[i]->SetMarkerColor(kBlue);
      }
      else if(i==2) {
        PhiDistr[i]->SetLineColor(kGreen);
        PhiDistr[i]->SetMarkerColor(kGreen);
      }
      else if(i==3) {
        PhiDistr[i]->SetLineColor(kCyan);
        PhiDistr[i]->SetMarkerColor(kCyan);
      }
      //PhiDistr[i]->SetLineWidth(0);
      PhiDistr[i]->SetMarkerStyle(7);
      PhiDistr[i]->GetXaxis()->SetRangeUser(0., 90.);
    }

    TGraph* PhiGr[5];
    Char_t phigraname[256];
    for(Int_t i=0;i<5;i++){
      sprintf(phigraname,"PhiGr_%d",i);
      PhiGr[i] = new TGraph(0);
      PhiGr[i]->SetName(phigraname);
      PhiGr[i]->SetTitle(phigraname);
      if(i==0) {
        PhiGr[i]->SetLineColor(kRed);
        PhiGr[i]->SetMarkerColor(kRed);
      }
      else if(i==1) {
        PhiGr[i]->SetLineColor(kBlue);
        PhiGr[i]->SetMarkerColor(kBlue);
      }
      else if(i==2) {
        PhiGr[i]->SetLineColor(kGreen);
        PhiGr[i]->SetMarkerColor(kGreen);
      }
      else if(i==3) {
        PhiGr[i]->SetLineColor(kCyan);
        PhiGr[i]->SetMarkerColor(kCyan);
      }
      PhiGr[i]->SetLineWidth(0);
      PhiGr[i]->SetMarkerStyle(7);
      PhiGr[i]->GetXaxis()->SetRangeUser(0., 90.);
    }

    if(x1>5.0&&y1>5.0){
      //cout<<nEve<<" , "<<endl;
      if(!pidcut->IsInside(deltaE,totalE)){
      Q02_Kine->Fill(x1,y1);

      if(wideElasticup->IsInside(x1,y1)){
        //Q02_Kine->Fill(x1,y1);
//cout<<"good "<<nEve<<endl;
        Excitation_EL->Fill(180-2*x1,(vertex->at(0)+vertex->at(2))/2.0);
        //cout<<nEve<<"\t"<<180-2*x2<<"\t"<<(vertex->at(1)+vertex->at(3))/2.0<<endl;
      //   std::vector<Double_t> *Phi0Array =quadvec->at(0).GetPhiArray();
      //   std::vector<Double_t> *Phi0RArray =quadvec->at(0).GetPhiRArray();
      //   std::vector<Double_t> *Phi2Array =quadvec->at(2).GetPhiArray();
      //   std::vector<Double_t> *Phi2RArray =quadvec->at(2).GetPhiRArray();
      //   if(Phi0RArray->size()>5){
      //     for(Int_t del=0;del<4;del++){
      //       Phi0Array->pop_back();
      //       Phi0RArray->pop_back();
      //     }
      //     for(Int_t pval=0;pval<Phi0RArray->size();pval++){
      //       PhiDistr[0]->Fill(Phi0Array->at(pval));
      //     }
      //   }
      //   if(Phi2Array->size()>5){
      //     for(Int_t del=0;del<4;del++){
      //       Phi2Array->pop_back();
      //       Phi2RArray->pop_back();
      //     }
      //     for(Int_t pval=0;pval<Phi2Array->size();pval++){
      //       PhiDistr[2]->Fill(Phi2Array->at(pval));
      //     }
      //   }
      //   Int_t del0 = 0;
      //   Int_t del2 = 0;
      //   while(del0<Phi0Array->size()){
      //     if(Phi0Array->at(del0)<PhiDistr[0]->GetBinLowEdge(PhiDistr[0]->GetMaximumBin()-1)){
      //       Phi0Array->erase(Phi0Array->begin()+del0);
      //       Phi0RArray->erase(Phi0RArray->begin()+del0);
      //     }
      //     else if(Phi0Array->at(del0)>PhiDistr[0]->GetBinLowEdge(PhiDistr[0]->GetMaximumBin()+2)){
      //       Phi0Array->erase(Phi0Array->begin()+del0);
      //       Phi0RArray->erase(Phi0RArray->begin()+del0);
      //     }
      //     else if(!(Phi0RArray->at(del0)>0.0)){
      //       Phi0Array->erase(Phi0Array->begin()+del0);
      //       Phi0RArray->erase(Phi0RArray->begin()+del0);
      //     }
      //     else{
      //       del0++;
      //     }
      //   }
      //   while(del2<Phi2Array->size()){
      //     if(Phi2Array->at(del2)<PhiDistr[2]->GetBinLowEdge(PhiDistr[2]->GetMaximumBin()-1)){
      //       Phi2Array->erase(Phi2Array->begin()+del2);
      //       Phi2RArray->erase(Phi2RArray->begin()+del2);
      //     }
      //     else if(Phi2Array->at(del2)>PhiDistr[2]->GetBinLowEdge(PhiDistr[2]->GetMaximumBin()+2)){
      //       Phi2Array->erase(Phi2Array->begin()+del2);
      //       Phi2RArray->erase(Phi2RArray->begin()+del2);
      //     }
      //     else if(!(Phi2RArray->at(del2)>0.0)){
      //       Phi2Array->erase(Phi2Array->begin()+del2);
      //       Phi2RArray->erase(Phi2RArray->begin()+del2);
      //     }
      //     else{
      //       del2++;
      //     }
      //   }
      //   Double_t phi0 = avg(Phi0Array);
      //   Double_t phi2 = avg(Phi2Array);
      //   if(Phi0Array->size()>0&&Phi2Array->size()>0){
      //   for(Int_t pval=0;pval<Phi0Array->size();pval++){
      //     PhiGr[0]->Set(PhiGr[0]->GetN()+1);
      //     PhiGr[0]->SetPoint(PhiGr[0]->GetN()-1, Phi0RArray->at(pval),Phi0Array->at(pval));
      //   }
      //   for(Int_t pval=0;pval<Phi2Array->size();pval++){
      //     PhiGr[2]->Set(PhiGr[2]->GetN()+1);
      //     PhiGr[2]->SetPoint(PhiGr[2]->GetN()-1, Phi2RArray->at(pval),Phi2Array->at(pval));
      //   }
      //   PhiGr[0]->Fit("pol1","q");
      //   PhiGr[2]->Fit("pol1","q");
      //   TF1 *fit0 = PhiGr[0]->GetFunction("pol1");
      //   TF1 *fit2 = PhiGr[2]->GetFunction("pol1");
      //   PhiCompare1->Fill(TMath::RadToDeg()*TMath::Tan(fit0->GetParameter(1)));
      //   PhiCompare1->Fill(TMath::RadToDeg()*TMath::Tan(fit2->GetParameter(1)));
      // }
      //   if(quadvec->at(0).GetNumHits()<quadvec->at(2).GetNumHits())PhiCompare->Fill(quadvec->at(0).GetNumHits(),TMath::Abs(phi0-phi2));
      //   if(quadvec->at(0).GetNumHits()>quadvec->at(2).GetNumHits())PhiCompare->Fill(quadvec->at(2).GetNumHits(),TMath::Abs(phi0-phi2));
         //if(quadvec->at(0).GetNumHits()>25&&quadvec->at(2).GetNumHits()>25)PhiCompare1->Fill(phi0-phi2);
      }
    }
    }

    if(x2>5.0&&y2>5.0){
      //cout<<nEve<<" , "<<endl;
      if(!pidcut->IsInside(deltaE,totalE)){
      Q02_Kine->Fill(x2,y2);

      if(wideElasticup->IsInside(x2,y2)){
        //Q02_Kine->Fill(x2,y2);
	//cout<<"good "<<nEve<<endl;
        Excitation_EL->Fill(180-2*x2,(vertex->at(1)+vertex->at(3))/2.0);
        //cout<<nEve<<"\t"<<180-2*x2<<"\t"<<(vertex->at(1)+vertex->at(3))/2.0<<endl;
      //   std::vector<Double_t> *Phi0Array =quadvec->at(1).GetPhiArray();
      //   std::vector<Double_t> *Phi0RArray =quadvec->at(1).GetPhiRArray();
      //   std::vector<Double_t> *Phi2Array =quadvec->at(3).GetPhiArray();
      //   std::vector<Double_t> *Phi2RArray =quadvec->at(3).GetPhiRArray();
      //   if(Phi0RArray->size()>5){
      //     for(Int_t del=0;del<4;del++){
      //       Phi0Array->pop_back();
      //       Phi0RArray->pop_back();
      //     }
      //     for(Int_t pval=0;pval<Phi0RArray->size();pval++){
      //       PhiDistr[1]->Fill(Phi0Array->at(pval));
      //     }
      //   }
      //   if(Phi2Array->size()>5){
      //     for(Int_t del=0;del<4;del++){
      //       Phi2Array->pop_back();
      //       Phi2RArray->pop_back();
      //     }
      //     for(Int_t pval=0;pval<Phi2Array->size();pval++){
      //       PhiDistr[3]->Fill(Phi2Array->at(pval));
      //     }
      //   }
      //   Int_t del0 = 0;
      //   Int_t del2 = 0;
      //   while(del0<Phi0Array->size()){
      //     if(Phi0Array->at(del0)<PhiDistr[1]->GetBinLowEdge(PhiDistr[1]->GetMaximumBin()-1)){
      //       Phi0Array->erase(Phi0Array->begin()+del0);
      //       Phi0RArray->erase(Phi0RArray->begin()+del0);
      //     }
      //     else if(Phi0Array->at(del0)>PhiDistr[1]->GetBinLowEdge(PhiDistr[1]->GetMaximumBin()+2)){
      //       Phi0Array->erase(Phi0Array->begin()+del0);
      //       Phi0RArray->erase(Phi0RArray->begin()+del0);
      //     }
      //     else if(!(Phi0RArray->at(del0)>0.0)){
      //       Phi0Array->erase(Phi0Array->begin()+del0);
      //       Phi0RArray->erase(Phi0RArray->begin()+del0);
      //     }
      //     else{
      //       del0++;
      //     }
      //   }
      //   while(del2<Phi2Array->size()){
      //     if(Phi2Array->at(del2)<PhiDistr[3]->GetBinLowEdge(PhiDistr[3]->GetMaximumBin()-1)){
      //       Phi2Array->erase(Phi2Array->begin()+del2);
      //       Phi2RArray->erase(Phi2RArray->begin()+del2);
      //     }
      //     else if(Phi2Array->at(del2)>PhiDistr[3]->GetBinLowEdge(PhiDistr[3]->GetMaximumBin()+2)){
      //       Phi2Array->erase(Phi2Array->begin()+del2);
      //       Phi2RArray->erase(Phi2RArray->begin()+del2);
      //     }
      //     else if(!(Phi2RArray->at(del2)>0.0)){
      //       Phi2Array->erase(Phi2Array->begin()+del2);
      //       Phi2RArray->erase(Phi2RArray->begin()+del2);
      //     }
      //     else{
      //       del2++;
      //     }
      //   }
      //   Double_t phi1 = avg(Phi0Array);
      //   Double_t phi3 = avg(Phi2Array);
      //   if(Phi0Array->size()>0&&Phi2Array->size()>0){
      //   for(Int_t pval=0;pval<Phi0Array->size();pval++){
      //     PhiGr[1]->Set(PhiGr[0]->GetN()+1);
      //     PhiGr[1]->SetPoint(PhiGr[1]->GetN()-1, Phi0RArray->at(pval),Phi0Array->at(pval));
      //   }
      //   for(Int_t pval=0;pval<Phi2Array->size();pval++){
      //     PhiGr[3]->Set(PhiGr[3]->GetN()+1);
      //     PhiGr[3]->SetPoint(PhiGr[3]->GetN()-1, Phi2RArray->at(pval),Phi2Array->at(pval));
      //   }
      //   PhiGr[1]->Fit("pol1","q");
      //   PhiGr[3]->Fit("pol1","q");
      //   TF1 *fit1 = PhiGr[1]->GetFunction("pol1");
      //   TF1 *fit3 = PhiGr[3]->GetFunction("pol1");
      //   PhiCompare1->Fill(TMath::RadToDeg()*TMath::Tan(fit1->GetParameter(1)));
      //   PhiCompare1->Fill(TMath::RadToDeg()*TMath::Tan(fit3->GetParameter(1)));
      // }
      //   if(quadvec->at(1).GetNumHits()<quadvec->at(3).GetNumHits())PhiCompare->Fill(quadvec->at(1).GetNumHits(),TMath::Abs(phi1-phi3));
      //   if(quadvec->at(1).GetNumHits()>quadvec->at(3).GetNumHits())PhiCompare->Fill(quadvec->at(3).GetNumHits(),TMath::Abs(phi1-phi3));
         //if(quadvec->at(1).GetNumHits()>25&&quadvec->at(3).GetNumHits()>25)PhiCompare1->Fill(phi1-phi3);
       }
     }
    }
    if(nEve==800){
      for(Int_t iQ=0; iQ<4; iQ++){
        cout<<"Quadrant "<<iQ<<endl;
        std::vector<Double_t> *PhiArray =quadvec->at(iQ).GetPhiArray();
        std::vector<Double_t> *PhiRArray =quadvec->at(iQ).GetPhiRArray();
        Int_t del = 0;
        while(del<PhiArray->size()){
          cout<<PhiArray->at(del)<<endl;
          if(PhiArray->at(del)<PhiDistr[iQ]->GetBinLowEdge(PhiDistr[iQ]->GetMaximumBin()-1)){
            PhiArray->erase(PhiArray->begin()+del);
            PhiRArray->erase(PhiRArray->begin()+del);
          }
          else if(PhiArray->at(del)>PhiDistr[iQ]->GetBinLowEdge(PhiDistr[iQ]->GetMaximumBin()+2)){
            PhiArray->erase(PhiArray->begin()+del);
            PhiRArray->erase(PhiRArray->begin()+del);
          }
          else{
            del++;
          }
        }
        if(PhiArray->size()>0){
          for(Int_t pval=0;pval<PhiArray->size();pval++){
            if(PhiRArray->at(pval)>0.0&&PhiArray->at(pval)>0.0){
            PhiGr[iQ]->Set(PhiGr[iQ]->GetN()+1);
            PhiGr[iQ]->SetPoint(PhiGr[iQ]->GetN()-1, PhiRArray->at(pval),PhiArray->at(pval));
          }
          }
        }
        PhiArray->clear();
        PhiRArray->clear();
      }
      c6->Draw();
      c6->cd(1);
      PhiGr[0]->Draw();
      c6->cd(2);
      PhiGr[1]->Draw();
      c6->cd(3);
      PhiGr[2]->Draw();
      c6->cd(4);
      PhiGr[3]->Draw();
    }
    nEve++;
    if(nEve%1000==0)cout<<nEve<<endl;
    /*for(Int_t iQ=0;iQ<5;iQ++){
      PhiDistr[iQ]->Delete();
      PhiGr[iQ]->Delete();
    }*/
    //if(nEve==10000)break;
  }
  c2->cd(1);
  PhiCompare->Draw("colz");
  c2->cd(2);
  PhiCompare1->Draw();

  c3->cd(1);
  Q02_Kine->Draw("colz");
  Kine_AngRec_AngSca->Draw("same");
  Kine_AngRec_AngSca_vert->Draw("same");
  c4->cd(1);
  Excitation_EL->Draw("colz");


}
