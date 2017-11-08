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
/*
#include "../../include/ATEvent.hh"
#include "../../include/ATHit.hh"
#include "../../include/ATProtoEvent.hh"
#include "../../include/ATProtoEventAna.hh"
#include "../../include/ATAnalysis.hh"
#include "../../include/ATHoughSpaceLine.hh"
#include "../../include/ATHoughSpaceCircle.hh"*/

#include <ios>
#include <iostream>
#include <istream>
#include <limits>

void run_plot2(TString FileNameHead = "output_proto",TString fileKine="10Be_4He_19MeV.txt")
{

  TString workdir = getenv("VMCWORKDIR");
  TString FilePath = workdir + "/macro/10Be/";
  TString FileNameTail = ".root";
  TString FileName     = FilePath + FileNameHead + FileNameTail;
  std::cout<<" Opening File : "<<FileName.Data()<<std::endl;
  TFile* file = new TFile(FileName.Data(),"READ");
  //TFile* file = new TFile(FileNameHead_chain.Data(),"READ");
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
  c5->Divide(2,1);
  TCanvas *c6 = new TCanvas("c6","c6",200,10,700,700);
  c6->Divide(2,1);
  TCanvas *c7 = new TCanvas("c7","c7",200,10,700,700);
  c7->Divide(3,6);
  TCanvas *c8 = new TCanvas("c8","c8",200,10,700,700);
  c8->Divide(5,4);

  TH2D* Q02_Kine = new TH2D("Q02_Kine","Q02_Kine",1000,0,180,1000,0,180);
  Q02_Kine->SetMarkerColor(2);
  Q02_Kine->SetMarkerStyle(20);
  Q02_Kine->SetMarkerSize(0.7);
  TH2D* Q13_Kine = new TH2D("Q13_Kine","Q13_Kine",400,0,40,400,0,40);

  TH2D* Excitation_EL = new TH2D("Elastic", "Elastic",500,0,500,500,0,500);
  TH2D* Excitation_IN = new TH2D("Inelastic", "Inelastic",500,0,500,500,0,500);

  TH2D* Delta_E = new TH2D("dE","dE",1000,0,300000,1000,0,5000000);

  TH3D* Dalitz1 = new TH3D("DalitzAngle","DalitzAngle",180,0,180,180,0,180,180,0,180);
  TH3D* Dalitz2 = new TH3D("DalitzRange","DalitzRange",200,0,1,200,0,1,200,0,1);

  TCutG* wideElasticup = new TCutG("wideElasticup",20);
  wideElasticup->SetVarX("Angle0");
  wideElasticup->SetVarY("Angle2");
  wideElasticup->SetTitle("Graph");
  wideElasticup->SetLineColor(2);
  wideElasticup->SetPoint(0,90,-2);
  wideElasticup->SetPoint(1,80.0396,1.645);
  wideElasticup->SetPoint(2,70.0304,7.0978);
  wideElasticup->SetPoint(3,60.0237,12.0652);
  wideElasticup->SetPoint(4,50.0202,16.1865);
  wideElasticup->SetPoint(5,40.0204,18.9176);
  wideElasticup->SetPoint(6,30.0241,19.4013);
  wideElasticup->SetPoint(7,20.3522,16.0306);
  wideElasticup->SetPoint(8,12.4072,6.0392);
  wideElasticup->SetPoint(9,0.065432,-3.95094);
  wideElasticup->SetPoint(10,0.065432,3.95094);
  wideElasticup->SetPoint(11,12.4072,14.0392);
  wideElasticup->SetPoint(12,20.3522,24.0306);
  wideElasticup->SetPoint(13,30.0241,27.4013);
  wideElasticup->SetPoint(14,40.0204,26.9176);
  wideElasticup->SetPoint(15,50.0202,24.1865);
  wideElasticup->SetPoint(16,60.0237,20.0652);
  wideElasticup->SetPoint(17,70.0304,15.0978);
  wideElasticup->SetPoint(18,80.0396,9.645);
  wideElasticup->SetPoint(19,90,4);
  wideElasticup->SetPoint(20,90,-4);

  TCutG *PID = new TCutG("PID",9); //Particle ID Cut in dE/E plot
  PID->SetVarX("dE");
  PID->SetVarY("E");
  PID->SetTitle("Graph");
  PID->SetLineColor(3);
  PID->SetPoint(0,12175,439792);
  PID->SetPoint(1,19167,597587);
  PID->SetPoint(2,24067,1553120);
  PID->SetPoint(3,25509,2850550);
  PID->SetPoint(4,21760,4025250);
  PID->SetPoint(5,16140,3341470);
  PID->SetPoint(6,14555,1842420);
  PID->SetPoint(7,11528,790448);
  PID->SetPoint(8,11311,562588);
  PID->SetPoint(9,12175,439792);

  Double_t *AlphaEnTab = new Double_t[100];
  Double_t *AlphaRangeTab = new Double_t[100];
  Double_t *BeEnTab = new Double_t[100];
  Double_t *BeRangeTab = new Double_t[100];

  std::ifstream *alphatable = new std::ifstream("alphaenergy.dat");
  Int_t numAlpha = 0;
  while(numAlpha<100){
    *alphatable>>AlphaEnTab[numAlpha]>>AlphaRangeTab[numAlpha];
    numAlpha++;
  }

  std::ifstream *betable = new std::ifstream("beenergy.dat");
  Int_t numBe = 0;
  while(numBe<100){
    *betable>>BeEnTab[numBe]>>BeRangeTab[numBe];
    numBe++;
  }
  Double_t maxDiff = 15.0;
  Double_t x1=0.0;
  Double_t y1=0.0;
  Double_t x2=0.0;
  Double_t y2=0.0;
  Int_t xquad[2];
  Int_t yquad[2];
  Double_t start=0;
  Double_t max=512;
  Double_t thresh=2000;
  Double_t tot_E=0;
  Double_t RedChi2[4];
  Bool_t isGoodFit[4];
  Bool_t isHit[260];
  Double_t thetalab = 0.0;
  Double_t thetacm = 0.0;
  Double_t maxAllowedRad = 118.0;
  Double_t minAllowedRad = 20.0;
  Double_t tVertex=0.0;
  Double_t calVertex[4]={0,0,0,0};
  Double_t intVertex[4]={0,0,0,0};
  Double_t intXVertex[4]={0,0,0,0};
  Int_t FitterPicker[2]={1,1};
  Double_t maxHit[4]={0,0,0,0};
  Double_t maxHitRad[4]={0,0,0,0};
  Double_t maxDer[4]={0,0,0,0};
  Double_t rad =0.0;
  Double_t dist=0.0;
  Double_t AlphaEnergy=0.0;
  Double_t AlphaEnergy1=0.0;
  Double_t AlphaMomentum=0.0;
  Double_t AlphaMomentum1=0.0;
  Double_t BeEnergy=0.0;
  Double_t BeEnergy1=0.0;
  Double_t BeMomentum=0.0;
  Double_t BeMomentum1=0.0;
  Double_t AlphaRange=0.0;
  Double_t AlphaRange3D=0.0;
  Double_t BeRange=0.0;
  Double_t BeRange3D=0.0;
  Double_t energycm=0.0;
  Double_t energycm1=0.0;
  Double_t energycm2=-0.0;
  std::vector<Int_t> goodquads;
  std::vector<Int_t> goodangles;
  Int_t numGoodQuads = 0;

  TRandom randGen;

  TTreeReader Reader1("cbmsim", file);
  TTreeReaderValue<TClonesArray> analysisArray(Reader1, "ATProtoEventAna");
  TTreeReaderValue<TClonesArray> eventArray(Reader1, "ATEventH");

  Int_t nEve =0;
  while (Reader1.Next()){
    ATProtoEventAna* analysis = (ATProtoEventAna*) analysisArray->At(0);
    ATEvent* event = (ATEvent*) eventArray->At(0);
    std::vector<Double_t> *AngleFit = analysis->GetAngleFit();
    std::vector<Double_t> *vertex   = analysis->GetVertex();
    std::vector<Double_t> *Par0     = analysis->GetPar0();
    std::vector<Double_t> *Par1     = analysis->GetPar1();
    std::vector<Double_t> *Chi2     = analysis->GetChi2();
    std::vector<Int_t> *NDF         = analysis->GetNDF();
    Float_t *MeshArray              = event->GetMesh();
    Int_t NumHits                   = event->GetNumHits();

    for(Int_t i=0;i<4;i++){
      RedChi2[i] = 0.0;
      isGoodFit[i] = kFALSE;
    }
    for(Int_t i=0;i<4;i++){
      if(NDF->at(i)>0.0) RedChi2[i]=(Chi2->at(i)/double(NDF->at(i)));
      if(RedChi2[i]>0.0&&RedChi2[i]<15.0) isGoodFit[i]=kTRUE;
    }
    max=512;
    for (Int_t i=512;i>0;i--){
      if(MeshArray[i]>thresh) {
        max = i;
        break;
      }
    }


    start=0;
    if(max<511){
      for (Int_t i = max; i>=max-5;i--)start+=MeshArray[i];
    }

    tot_E = 0;
    for(Int_t i=0;i<=512;i++){
      if (MeshArray[i]>thresh) tot_E+=MeshArray[i];
    }

    for(Int_t i=0;i<4;i++){
      tVertex=0.0;
      tVertex=((vertex->at(i)-500.0)*100.0/(3.2*80.0))+360.0+randGen.Uniform(-0.5,0.5);
      calVertex[i]=500.0-((double(max)-tVertex)*3.2*80.0/(100.0));

    }

    for(Int_t i=0;i<4;i++){
      if((Par1->at(i)-Par1->at((i+2)%4))!=0){
        intVertex[i]=(Par1->at(i)*calVertex[(i+2)%4]-calVertex[i]*Par1->at((i+2)%4))/(Par1->at(i)-Par1->at((i+2)%4));
        intXVertex[i]=(calVertex[(i+2)%4]-calVertex[i])/(Par1->at(i)-Par1->at((i+2)%4));
      }
      else{

        intVertex[i]=0.0;
        intXVertex[i]=0.0;
      }
    }

    Double_t maxRad[4] = {-1,-1,-1,-1};
    Double_t zmaxRad[4] = {-1,-1,-1,-1};
    Double_t range[4]={0,0,0,0};
    for(Int_t i=0; i<NumHits; i++){
      ATHit *Hit = event->GetHit(i);
      Int_t PadNum = Hit->GetHitPadNum();
      isHit[PadNum]=kTRUE;
      TVector3 coords = Hit->GetPosition();
      tVertex=((coords.z()-500.0)*100.0/(3.2*80.0))+360.0+randGen.Uniform(-0.5,0.5);
      Double_t calz = 500.0-((double(max)-tVertex)*3.2*80.0/(100.0));
      Double_t rad = TMath::Sqrt(coords.x()*coords.x()+coords.y()*coords.y());
      if(PadNum>=1&&PadNum<=64){
        if(rad>maxRad[0]){
          maxRad[0]=rad;
          zmaxRad[0]=calz;
          range[0]=(calz-intVertex[0])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(0)));
        }
      }
      if(PadNum>=65&&PadNum<=128){
        if(rad>maxRad[1]){
          maxRad[1]=rad;
          zmaxRad[1]=calz;
          range[1]=(calz-intVertex[1])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(1)));
        }
      }
      if(PadNum>=129&&PadNum<=192){
        if(rad>maxRad[2]){
          maxRad[2]=rad;
          zmaxRad[2]=calz;
          range[2]=(calz-intVertex[2])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(2)));
        }
      }
      if(PadNum>=193&&PadNum<=256){
        if(rad>maxRad[3]){
          maxRad[3]=rad;
          zmaxRad[3]=calz;
          range[3]=(calz-intVertex[3])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(3)));
        }
      }
    }
    xquad[0]=-1;
    yquad[0]=-1;
    xquad[1]=-1;
    yquad[1]=-1;
    FitterPicker[0]=1;
    FitterPicker[1]=1;

    numGoodQuads = 0;
    goodquads.clear();
    goodangles.clear();
    for(Int_t i=0;i<3;i++){
      if (isGoodFit[i]){
        numGoodQuads++;
        goodquads.push_back(i);
        goodangles.push_back(AngleFit->at(i));
      }
    }
    if(numGoodQuads>2&&maxRad[goodquads.at(0)]>minAllowedRad&&maxRad[goodquads.at(1)]>minAllowedRad&&maxRad[goodquads.at(2)]>minAllowedRad){
      if(TMath::Abs((goodangles.at(0)-goodangles.at(1)))>3.0&&TMath::Abs((goodangles.at(0)-goodangles.at(2))%90)>3.0&&TMath::Abs((goodangles.at(2)-goodangles.at(1))%90)>3.0&&TMath::Abs((goodangles.at(0)-goodangles.at(1)))<87.0&&TMath::Abs((goodangles.at(0)-goodangles.at(2))%90)<87.0&&TMath::Abs((goodangles.at(2)-goodangles.at(1))%90)<87.0){
        cout<<nEve<<endl;
        Double_t sumrange = range[goodquads.at(0)]+range[goodquads.at(1)]+range[goodquads.at(2)];
        Dalitz1->Fill(goodangles.at(0),goodangles.at(1),goodangles.at(2));
        Dalitz2->Fill(range[goodquads.at(0)]/sumrange,range[goodquads.at(1)]/sumrange,range[goodquads.at(2)]/sumrange);
        //cout<<range[goodquads.at(0)]/sumrange<<"\t"<<range[goodquads.at(1)]/sumrange<<"\t"<<range[goodquads.at(2)]/sumrange<<endl;
      }
    }



    Delta_E->Fill(start,tot_E);



    if(1==1){
      if(AngleFit->at(2)>AngleFit->at(0)) {
        //if( (isGoodFit[0]&&isGoodFit[2]) &&maxRad[0]>minAllowedRad&&maxRad[2]>minAllowedRad&&TMath::MaxElement(4,maxRad)<maxAllowedRad )//
        if(AngleFit->at(2)>2.0&&AngleFit->at(0)>2.0&&AngleFit->at(2)<178.0&&AngleFit->at(0)<178.0)
        {
          x1 = AngleFit->at(2);
          y1 = AngleFit->at(0);
          xquad[0]=2;
          yquad[0]=0;
          Q02_Kine->Fill(x1,y1);
        }
        else{

          x1=0.0;
          y1=0.0;
          xquad[0]=-1;
          yquad[0]=-1;
          FitterPicker[0]=0;
        }
      }
      else{
        //if( (isGoodFit[0]&&isGoodFit[2]) &&maxRad[0]>minAllowedRad&&maxRad[2]>minAllowedRad&&TMath::MaxElement(4,maxRad)<maxAllowedRad )//
        if(AngleFit->at(2)>2.0&&AngleFit->at(0)>2.0&&AngleFit->at(2)<178.0&&AngleFit->at(0)<178.0)
        {
          x1 = AngleFit->at(0);
          y1 = AngleFit->at(2);
          xquad[0]=0;
          yquad[0]=2;
          Q02_Kine->Fill(x1,y1);
        }
        else{
          x1=0.0;
          y1=0.0;
          xquad[0]=-1;
          yquad[0]=-1;
          FitterPicker[0]=0;
        }
      }
      if(AngleFit->at(3)>AngleFit->at(1)){
        //if( (isGoodFit[1]&&isGoodFit[3]) &&maxRad[1]>minAllowedRad&&maxRad[3]>minAllowedRad&&TMath::MaxElement(4,maxRad)<maxAllowedRad )//
        if(AngleFit->at(1)>2.0&&AngleFit->at(3)>2.0&&AngleFit->at(1)<178.0&&AngleFit->at(3)<178.0)
        {
          x2 = AngleFit->at(3);
          y2 = AngleFit->at(1);
          xquad[1]=3;
          yquad[1]=1;
          Q02_Kine->Fill(x2,y2);
        }
        else{
          x2=0.0;
          y2=0.0;
          xquad[1]=-1;
          yquad[1]=-1;
          FitterPicker[1]=0;
        }
      }
      else{
        //if( (isGoodFit[1]&&isGoodFit[3]) &&maxRad[1]>minAllowedRad&&maxRad[3]>minAllowedRad&&TMath::MaxElement(4,maxRad)<maxAllowedRad )//
        if(AngleFit->at(1)>2.0&&AngleFit->at(3)>2.0&&AngleFit->at(1)<178.0&&AngleFit->at(3)<178.0)
        {
          x2 = AngleFit->at(1);
          y2 = AngleFit->at(3);
          xquad[1]=1;
          yquad[1]=3;
          Q02_Kine->Fill(x2,y2);
        }
        else{
          x2=0.0;
          y2=0.0;
          xquad[1]=-1;
          yquad[1]=-1;
          FitterPicker[1]=0;
        }
      }


      if(FitterPicker[0]==1&&FitterPicker[1]==1){
        if(RedChi2[0]+RedChi2[2]>RedChi2[1]+RedChi2[3]){
          FitterPicker[0]=0;
        }
        if(RedChi2[0]+RedChi2[2]<=RedChi2[1]+RedChi2[3]){
          FitterPicker[1]=0;
        }
      }

      Int_t alphaquad = -1;
      Int_t bequad = -1;
      Double_t bestdiff;
      Double_t bestrange;
      Int_t bestindex;
      Double_t bestdiff1;
      Double_t bestrange1;
      Int_t bestindex1;

      Int_t numPadsHit[4] = {0,0,0,0};
      thetalab = 0.0;
      thetacm = 0.0;
      energycm=0.0;
      energycm1=0.0;
      if(wideElasticup->IsInside(x1,y1)&&FitterPicker[0]==1){
        alphaquad=xquad[0];
        bequad=yquad[0];
        AlphaRange=(maxRad[alphaquad])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad)));
        AlphaRange3D = TMath::Sqrt(maxRad[alphaquad]*maxRad[alphaquad]+(zmaxRad[alphaquad]-vertex->at(alphaquad))*(zmaxRad[alphaquad]-vertex->at(alphaquad)));
        BeRange=(maxRad[bequad])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad)));
        BeRange3D = TMath::Sqrt(maxRad[bequad]*maxRad[bequad]+(zmaxRad[bequad]-vertex->at(bequad))*(zmaxRad[bequad]-vertex->at(bequad)));
        bestindex=-1;
        bestdiff = AlphaRange*10000;
        for(Int_t i=0; i<92;i++){
          if(TMath::Abs(AlphaRange - AlphaRangeTab[i])<bestdiff){
            bestindex=i-1;
            bestdiff = TMath::Abs(AlphaRange - AlphaRangeTab[i]);
            bestrange=AlphaRangeTab[i-1];
          }
          //cout<<bestindex<<"\t"<<bestrange<<endl;
        }
        bestindex1=-1;
        bestdiff1 = BeRange*10000;
        for(Int_t i=0; i<92;i++){
          if(TMath::Abs(BeRange - BeRangeTab[i])<bestdiff1){
            bestindex1=i-1;
            bestdiff1 = TMath::Abs(BeRange - BeRangeTab[i]);
            bestrange1=BeRangeTab[i-1];

          }
          //cout<<BeRangeTab[i]<<endl;
        }
        AlphaEnergy = AlphaEnTab[bestindex]+(AlphaEnTab[bestindex+1]-AlphaEnTab[bestindex])*(AlphaRange-bestrange)/(AlphaRangeTab[bestindex+1]-bestrange);//+randGen.Uniform(-0.5,0.5);
        BeEnergy1 = BeEnTab[bestindex1]+(BeEnTab[bestindex1+1]-BeEnTab[bestindex1])*(BeRange-bestrange1)/(BeRangeTab[bestindex1+1]-bestrange1);//+randGen.Uniform(-0.5,0.5);
        AlphaMomentum = TMath::Sqrt(double(AlphaEnergy)*2.0*4000.0);
        BeMomentum1 = TMath::Sqrt(double(BeEnergy1)*2.0*10000.0);
        BeMomentum= AlphaMomentum*TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad));
        AlphaMomentum1= BeMomentum1*TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad));
        BeEnergy = BeMomentum*BeMomentum/(2.0*10000.0);
        AlphaEnergy1 = AlphaMomentum1*AlphaMomentum1/(2.0*4000.0);
        energycm = 2.0/7.0*(BeEnergy+AlphaEnergy);
        energycm1 = 2.0/7.0*(BeEnergy1+AlphaEnergy1);

        thetacm=180-(2*thetalab);
        if (thetacm<0) thetacm= thetacm+180.0;

        Excitation_EL->Fill(AlphaRange,AlphaRange3D);
        if((AlphaRange-AlphaRange3D)<1.0&&(BeRange-BeRange3D)<1.0)Q13_Kine->Fill(energycm,energycm1);
        Excitation_IN->Fill(BeRange,BeRange3D);
      }
      thetalab = 0.0;
      thetacm = 0.0;
      energycm=0.0;
      energycm1=0.0;
      if(wideElasticup->IsInside(x2,y2)&&FitterPicker[1]==1){
        alphaquad=xquad[1];
        bequad=yquad[1];
        AlphaRange=(maxRad[alphaquad])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad)));
        AlphaRange3D = TMath::Sqrt(maxRad[alphaquad]*maxRad[alphaquad]+(zmaxRad[alphaquad]-vertex->at(alphaquad))*(zmaxRad[alphaquad]-vertex->at(alphaquad)));
        BeRange=(maxRad[bequad])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad)));
        BeRange3D = TMath::Sqrt(maxRad[bequad]*maxRad[bequad]+(zmaxRad[bequad]-vertex->at(bequad))*(zmaxRad[bequad]-vertex->at(bequad)));
        bestindex=-1;
        bestdiff = AlphaRange*10000;
        for(Int_t i=0; i<92;i++){
          if(TMath::Abs(AlphaRange - AlphaRangeTab[i])<bestdiff){
            bestindex=i-1;
            bestdiff = TMath::Abs(AlphaRange - AlphaRangeTab[i]);
            bestrange=AlphaRangeTab[i-1];
          }
          //cout<<bestindex<<"\t"<<bestrange<<endl;
        }
        bestindex1=-1;
        bestdiff1 = BeRange*10000;
        for(Int_t i=0; i<92;i++){
          if(TMath::Abs(BeRange - BeRangeTab[i])<bestdiff1){
            bestindex1=i-1;
            bestdiff1 = TMath::Abs(BeRange - BeRangeTab[i]);
            bestrange1=BeRangeTab[i-1];

          }
          //cout<<BeRangeTab[i]<<endl;
        }
        AlphaEnergy = AlphaEnTab[bestindex]+(AlphaEnTab[bestindex+1]-AlphaEnTab[bestindex])*(AlphaRange-bestrange)/(AlphaRangeTab[bestindex+1]-bestrange);//+randGen.Uniform(-0.5,0.5);
        BeEnergy1 = BeEnTab[bestindex1]+(BeEnTab[bestindex1+1]-BeEnTab[bestindex1])*(BeRange-bestrange1)/(BeRangeTab[bestindex1+1]-bestrange1);//+randGen.Uniform(-0.5,0.5);
        AlphaMomentum = TMath::Sqrt(double(AlphaEnergy)*2.0*4000.0);
        BeMomentum1 = TMath::Sqrt(double(BeEnergy1)*2.0*10000.0);
        BeMomentum= AlphaMomentum*TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad));
        AlphaMomentum1= BeMomentum1*TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad));
        BeEnergy = BeMomentum*BeMomentum/(2.0*10000.0);
        AlphaEnergy1 = AlphaMomentum1*AlphaMomentum1/(2.0*4000.0);
        energycm = 2.0/7.0*(BeEnergy+AlphaEnergy);
        energycm1 = 2.0/7.0*(BeEnergy1+AlphaEnergy1);

        thetacm=180-(2*thetalab);
        if (thetacm<0) thetacm= thetacm+180.0;

        Excitation_EL->Fill(AlphaRange,AlphaRange3D);
        if((AlphaRange-AlphaRange3D)<1.0&&(BeRange-BeRange3D)<1.0)Q13_Kine->Fill(energycm,energycm1);
        Excitation_IN->Fill(BeRange,BeRange3D);
      }

    }
    nEve++;
    //if(nEve%10000==0)cout<<nEve<<endl;
    //if(nEve>1000000)break;
  }
  c2->cd(1);
  Delta_E->Draw("colz");
  PID->Draw("same");

  c3->cd(1);
  gPad->SetLogz();
  Q02_Kine->Draw("colz");
  wideElasticup->Draw("same");
  c3->cd(2);
  gPad->SetLogz();
  Q13_Kine->Draw("colz");

  c4->cd(1);
  gPad->SetLogz();
  Excitation_EL->Draw();
  c4->cd(2);
  gPad->SetLogz();
  Excitation_IN->Draw("colz");

  c5->cd(1);
  Dalitz1->Draw();
  c5->cd(2);
  Dalitz2->Draw();
}
