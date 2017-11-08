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

void run_plot(TString FileNameHead = "output_protoh",TString fileKine="10Be_4He_19MeV.txt")
{
  TString File2Name = "output_protoh.root";
  TString workdir = getenv("VMCWORKDIR");
  TString FilePath = "/home/lisa/fair_install/ATTPCROOTv2/macro/10Be/";
  TString FileNameTail = ".root";
  TString FileName     = FilePath + FileNameHead + FileNameTail;
  std::cout<<" Opening File : "<<FileName.Data()<<std::endl;
  TFile* file = new TFile(FileName.Data(),"READ");
  TFile* file2 = new TFile(File2Name.Data(),"READ");
  //TFile* file = new TFile(FileNameHead_chain.Data(),"READ");
  TTree* tree = (TTree*) file -> Get("cbmsim");
  TTree* tree2 = (TTree*) file2 -> Get("cbmsim");
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
  c6->Divide(2,1);
  TCanvas *c7 = new TCanvas("c7","c7",200,10,700,700);
  c7->Divide(3,6);
  TCanvas *c8 = new TCanvas("c8","c8",200,10,700,700);
  c8->Divide(5,4);


  //TFile *rootfile = new TFile("division.root");
  //TCanvas *c9 = (TCanvas *)rootfile->Get("c1_n2");
  //TH2 *dividecross = (TH2*)c9->FindObject("Cross Section");


  TH2D* Q02_Kine = new TH2D("Q02_Kine","Q02_Kine",180,0,180,180,0,180);
  Q02_Kine->SetMarkerColor(0);
  Q02_Kine->SetMarkerStyle(20);
  Q02_Kine->SetMarkerSize(0.7);
  TH2D* Q13_Kine = new TH2D("Q13_Kine","Q13_Kine",180,0,180,180,0,180);
  Q13_Kine->SetMarkerColor(0);
  Q13_Kine->SetMarkerStyle(20);
  Q13_Kine->SetMarkerSize(0.7);

  TH1D* Vertex = new TH1D("NewRad","NewRad",100,-0,10);
  TH1D* Vertex1 = new TH1D("OldRad","OldRad",4000,-2000,2000);
  TH1D* Vertex2 = new TH1D("new-old","new-old",4000,-2000,2000);
  TH2D* Vertex_vs_Angle = new TH2D("Vertex_vs_Angle","Vertex_vs_Angle",1000,0,1000,200,0,180);
  TH2D* Vertex_vs_Angle2 = new TH2D("Vertex_vs_Angle2","Vertex_vs_Angle2",1000,0,1000,200,0,180);

  TH2D* Diff02 = new TH2D("Diff02","Diff02",2000,-2000,2000,2000,-2000,2000);
  TH2D* Diff13 = new TH2D("Diff13","Diff13",2000,-2000,2000,2000,-2000,2000);

  TH2D* Excitation_EL = new TH2D("Elastic", "Elastic",36,0,180,80,0,8);
  TH2D* Excitation_IN = new TH2D("Inelastic", "Inelastic",36,0,180,80,0,8);
  TH2D* Excitation_ELa = new TH2D("Elastica", "Elastica",36,0,180,80,0,8);
  TH2D* Excitation_INa = new TH2D("Inelastica", "Inelastica",200,0,20,200,0,20);

  TH2D* Delta_E = new TH2D("dE","dE",1000,0,300000,1000,0,5000000);
  TH2D* Delta_E_cut = new TH2D("dE_cut","dE_cut",1000,0,300000,1000,0,5000000);
  TH1D* Max_E = new TH1D("maxE","maxE",512,0,513);

  TH1D* xVertex0 = new TH1D("xVertex0", "xVertex0",500,-250,250);
  TH1D* xVertex1 = new TH1D("xVertex1", "xVertex1",500,-250,250);

  TH2D* numHitsHist = new TH2D("numHits", "numHits",1000,0,1000,70,0,70);
  TH2D* numHitsHist2 = new TH2D("numHits2", "numHits2",1000,0,1000,70,0,70);

  TH1D* Excitation10 = new TH1D("Excitation10", "Excitation10",80,0,8);
  TH1D* Excitation20 = new TH1D("Excitation20", "Excitation20",80,0,8);
  TH1D* Excitation30 = new TH1D("Excitation30", "Excitation30",80,0,8);
  TH1D* Excitation40 = new TH1D("Excitation40", "Excitation40",80,0,8);
  TH1D* Excitation50 = new TH1D("Excitation50", "Excitation50",80,0,8);
  TH1D* Excitation60 = new TH1D("Excitation60", "Excitation60",80,0,8);
  TH1D* Excitation70 = new TH1D("Excitation70", "Excitation70",80,0,8);
  TH1D* Excitation80 = new TH1D("Excitation80", "Excitation80",80,0,8);
  TH1D* Excitation90 = new TH1D("Excitation90", "Excitation90",80,0,8);
  TH1D* Excitation100 = new TH1D("Excitation100", "Excitation100",80,0,8);
  TH1D* Excitation110 = new TH1D("Excitation110", "Excitation110",80,0,8);
  TH1D* Excitation120 = new TH1D("Excitation120", "Excitation120",80,0,8);
  TH1D* Excitation130 = new TH1D("Excitation130", "Excitation130",80,0,8);
  TH1D* Excitation140 = new TH1D("Excitation140", "Excitation140",80,0,8);
  TH1D* Excitation150 = new TH1D("Excitation150", "Excitation150",80,0,8);
  TH1D* Excitation160 = new TH1D("Excitation160", "Excitation160",80,0,8);
  TH1D* Excitation170 = new TH1D("Excitation170", "Excitation170",80,0,8);
  TH1D* Excitation180 = new TH1D("Excitation180", "Excitation180",80,0,8);

  TH1D* Energy100 = new TH1D("Energy100", "Energy600",36,0,180);
  TH1D* Energy200 = new TH1D("Energy200", "Energy700",36,0,180);
  TH1D* Energy300 = new TH1D("Energy300", "Energy800",36,0,180);
  TH1D* Energy400 = new TH1D("Energy400", "Energy900",36,0,180);
  TH1D* Energy500 = new TH1D("Energy500", "Energy1000",36,0,180);
  TH1D* Energy600 = new TH1D("Energy600", "Energy1100",36,0,180);
  TH1D* Energy700 = new TH1D("Energy700", "Energy1200",36,0,180);
  TH1D* Energy800 = new TH1D("Energy800", "Energy1300",36,0,180);
  TH1D* Energy900 = new TH1D("Energy900", "Energy1400",36,0,180);
  TH1D* Energy1000 = new TH1D("Energy1000", "Energy1500",36,0,180);
  TH1D* Energy1100 = new TH1D("Energy1100", "Energy1600",36,0,180);
  TH1D* Energy1200 = new TH1D("Energy1200", "Energy1700",36,0,180);
  TH1D* Energy1300 = new TH1D("Energy1300", "Energy1800",36,0,180);
  TH1D* Energy1400 = new TH1D("Energy1400", "Energy1900",36,0,180);
  TH1D* Energy1500 = new TH1D("Energy1500", "Energy2000",36,0,180);
  TH1D* Energy1600 = new TH1D("Energy1600", "Energy2100",36,0,180);
  TH1D* Energy1700 = new TH1D("Energy1700", "Energy2200",36,0,180);
  TH1D* Energy1800 = new TH1D("Energy1800", "Energy2300",36,0,180);
  TH1D* Energy1900 = new TH1D("Energy1900", "Energy2400",36,0,180);
  TH1D* Energy2000 = new TH1D("Energy2000", "Energy2500",36,0,180);

  Double_t *ThetaCMS = new Double_t[20000];
  Double_t *ThetaLabRec = new Double_t[20000];
  Double_t *EnerLabRec = new Double_t[20000];
  Double_t *ThetaLabSca = new Double_t[20000];
  Double_t *EnerLabSca = new Double_t[20000];
  Double_t *ThetaCMSIn = new Double_t[20000];
  Double_t *ThetaLabRecIn = new Double_t[20000];
  Double_t *EnerLabRecIn = new Double_t[20000];
  Double_t *ThetaLabScaIn = new Double_t[20000];
  Double_t *EnerLabScaIn = new Double_t[20000];
  Double_t *ThetaCMSB = new Double_t[20000];
  Double_t *ThetaLabRecB = new Double_t[20000];
  Double_t *EnerLabRecB = new Double_t[20000];
  Double_t *ThetaLabScaB = new Double_t[20000];
  Double_t *EnerLabScaB = new Double_t[20000];
  Double_t *ThetaCMS29 = new Double_t[20000];
  Double_t *ThetaLabRec29 = new Double_t[20000];
  Double_t *EnerLabRec29 = new Double_t[20000];
  Double_t *ThetaLabSca29 = new Double_t[20000];
  Double_t *EnerLabSca29 = new Double_t[20000];
  Double_t *ThetaCMS3 = new Double_t[20000];
  Double_t *ThetaLabRec3 = new Double_t[20000];
  Double_t *EnerLabRec3 = new Double_t[20000];
  Double_t *ThetaLabSca3 = new Double_t[20000];
  Double_t *EnerLabSca3 = new Double_t[20000];
  Double_t *ThetaCMS31 = new Double_t[20000];
  Double_t *ThetaLabRec31 = new Double_t[20000];
  Double_t *EnerLabRec31 = new Double_t[20000];
  Double_t *ThetaLabSca31 = new Double_t[20000];
  Double_t *EnerLabSca31 = new Double_t[20000];
  Double_t *ThetaCMS39 = new Double_t[20000];
  Double_t *ThetaLabRec39 = new Double_t[20000];
  Double_t *EnerLabRec39 = new Double_t[20000];
  Double_t *ThetaLabSca39 = new Double_t[20000];
  Double_t *EnerLabSca39 = new Double_t[20000];
  Double_t *ThetaCMS4 = new Double_t[20000];
  Double_t *ThetaLabRec4 = new Double_t[20000];
  Double_t *EnerLabRec4 = new Double_t[20000];
  Double_t *ThetaLabSca4 = new Double_t[20000];
  Double_t *EnerLabSca4 = new Double_t[20000];
  Double_t *ThetaCMS41 = new Double_t[20000];
  Double_t *ThetaLabRec41 = new Double_t[20000];
  Double_t *EnerLabRec41 = new Double_t[20000];
  Double_t *ThetaLabSca41 = new Double_t[20000];
  Double_t *EnerLabSca41 = new Double_t[20000];
  Double_t *LengthMM = new Double_t[500];
  Double_t *EnergyMM = new Double_t[500];
  Double_t *AlphaEnTab = new Double_t[100];
  Double_t *AlphaRangeTab = new Double_t[100];
  Double_t *BeEnTab = new Double_t[100];
  Double_t *BeRangeTab = new Double_t[100];
  Double_t *InelHeLab = new Double_t[100];
  Double_t *InelHeCM = new Double_t[100];
  Double_t *InelBeLab = new Double_t[100];
  Double_t *InelBeCM = new Double_t[100];


  TString kinfile="Kine195.txt";
  std::ifstream *kineStr = new std::ifstream(kinfile.Data());
  Int_t numKin=0;

  if(!kineStr->fail()){
    while(!kineStr->eof()){
      *kineStr>>ThetaCMS[numKin]>>ThetaLabRec[numKin]>>EnerLabRec[numKin]>>ThetaLabSca[numKin]>>EnerLabSca[numKin];
      numKin++;
    }
  }else if(kineStr->fail()) std::cout<<" Warning : No Kinematics file found for this reaction! Please run the macro on $SIMPATH/macro/Kinematics/Decay_kinematics/Mainrel.cxx"<<std::endl;

  TString kinfileIn = "KineInEl20.txt";
  std::ifstream *kineStrIn = new std::ifstream(kinfileIn.Data());
  numKin=0;

  if(!kineStrIn->fail()){
    while(!kineStrIn->eof()){
      *kineStrIn>>ThetaCMSIn[numKin]>>ThetaLabRecIn[numKin]>>EnerLabRecIn[numKin]>>ThetaLabScaIn[numKin]>>EnerLabScaIn[numKin];
      numKin++;
    }
  }else if(kineStrIn->fail()) std::cout<<" Warning : No Kinematics file found for this reaction! Please run the macro on $SIMPATH/macro/Kinematics/Decay_kinematics/Mainrel.cxx"<<std::endl;

  TString kinfileB = "../Kinematics/Decay_kinematics/Kine.txt";
  std::ifstream *kineStrB = new std::ifstream(kinfileB.Data());
  numKin=0;

  if(!kineStrB->fail()){
    while(!kineStrB->eof()){
      *kineStrB>>ThetaCMSB[numKin]>>ThetaLabRecB[numKin]>>EnerLabRecB[numKin]>>ThetaLabScaB[numKin]>>EnerLabScaB[numKin];
      numKin++;
    }
  }else if(kineStrB->fail()) std::cout<<" Warning : No Kinematics file found for this reaction! Please run the macro on $SIMPATH/macro/Kinematics/Decay_kinematics/Mainrel.cxx"<<std::endl;

  TString kinfile29 = "Kine29.txt";
  std::ifstream *kineStr29 = new std::ifstream(kinfile29.Data());
  numKin=0;

  if(!kineStr29->fail()){
    while(!kineStr29->eof()){
      *kineStr29>>ThetaCMS29[numKin]>>ThetaLabRec29[numKin]>>EnerLabRec29[numKin]>>ThetaLabSca29[numKin]>>EnerLabSca29[numKin];
      numKin++;
    }
  }else if(kineStr29->fail()) std::cout<<" Warning : No Kinematics file found for this reaction! Please run the macro on $SIMPATH/macro/Kinematics/Decay_kinematics/Mainrel.cxx"<<std::endl;


  TString kinfile3 = "Kine3.txt";
  std::ifstream *kineStr3 = new std::ifstream(kinfile3.Data());
  numKin=0;

  if(!kineStr3->fail()){
    while(!kineStr3->eof()){
      *kineStr3>>ThetaCMS3[numKin]>>ThetaLabRec3[numKin]>>EnerLabRec3[numKin]>>ThetaLabSca3[numKin]>>EnerLabSca3[numKin];
      numKin++;
    }
  }else if(kineStr3->fail()) std::cout<<" Warning : No Kinematics file found for this reaction! Please run the macro on $SIMPATH/macro/Kinematics/Decay_kinematics/Mainrel.cxx"<<std::endl;

  TString kinfile31 = "Kine31.txt";
  std::ifstream *kineStr31 = new std::ifstream(kinfile31.Data());
  numKin=0;

  if(!kineStr31->fail()){
    while(!kineStr31->eof()){
      *kineStr31>>ThetaCMS31[numKin]>>ThetaLabRec31[numKin]>>EnerLabRec31[numKin]>>ThetaLabSca31[numKin]>>EnerLabSca31[numKin];
      numKin++;
    }
  }else if(kineStr31->fail()) std::cout<<" Warning : No Kinematics file found for this reaction! Please run the macro on $SIMPATH/macro/Kinematics/Decay_kinematics/Mainrel.cxx"<<std::endl;

  TString kinfile39 = "Kine39.txt";
  std::ifstream *kineStr39 = new std::ifstream(kinfile39.Data());
  numKin=0;

  if(!kineStr39->fail()){
    while(!kineStr39->eof()){
      *kineStr39>>ThetaCMS39[numKin]>>ThetaLabRec39[numKin]>>EnerLabRec39[numKin]>>ThetaLabSca39[numKin]>>EnerLabSca39[numKin];
      numKin++;
    }
  }else if(kineStr39->fail()) std::cout<<" Warning : No Kinematics file found for this reaction! Please run the macro on $SIMPATH/macro/Kinematics/Decay_kinematics/Mainrel.cxx"<<std::endl;


  TString kinfile4 = "Kine4.txt";
  std::ifstream *kineStr4 = new std::ifstream(kinfile4.Data());
  numKin=0;

  if(!kineStr4->fail()){
    while(!kineStr4->eof()){
      *kineStr4>>ThetaCMS4[numKin]>>ThetaLabRec4[numKin]>>EnerLabRec4[numKin]>>ThetaLabSca4[numKin]>>EnerLabSca4[numKin];
      numKin++;
    }
  }else if(kineStr4->fail()) std::cout<<" Warning : No Kinematics file found for this reaction! Please run the macro on $SIMPATH/macro/Kinematics/Decay_kinematics/Mainrel.cxx"<<std::endl;

  TString kinfile41 = "Kine41.txt";
  std::ifstream *kineStr41 = new std::ifstream(kinfile41.Data());
  numKin=0;

  if(!kineStr41->fail()){
    while(!kineStr41->eof()){
      *kineStr41>>ThetaCMS41[numKin]>>ThetaLabRec41[numKin]>>EnerLabRec41[numKin]>>ThetaLabSca41[numKin]>>EnerLabSca41[numKin];
      numKin++;
    }
  }else if(kineStr41->fail()) std::cout<<" Warning : No Kinematics file found for this reaction! Please run the macro on $SIMPATH/macro/Kinematics/Decay_kinematics/Mainrel.cxx"<<std::endl;


  std::ifstream *energyTable = new std::ifstream("energy.dat");
  Int_t numLen=0;
  while(!energyTable->eof()){
    *energyTable>>LengthMM[numLen]>>EnergyMM[numLen];
    numLen++;
  }

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

  std::ifstream *inelhetable = new std::ifstream("EHeInEl20.dat");
  Int_t numinhe =0;
  while(numinhe <100){
    *inelhetable>>InelHeLab[numinhe]>>InelHeCM[numinhe];
    numinhe++;
  }

  std::ifstream *inelbetable = new std::ifstream("EBeInEl20.dat");
  Int_t numinbe =0;
  while(numinbe <100){
    *inelbetable>>InelBeLab[numinbe]>>InelBeCM[numinbe];
    numinbe++;
  }

  TGraph *Kine_AngRec_AngSca = new TGraph(numKin,ThetaLabRec,ThetaLabSca);
  Kine_AngRec_AngSca->SetLineColor(6);
  TGraph *Kine_AngRec_AngSca_In = new TGraph(numKin,ThetaLabRecIn,ThetaLabScaIn);
  Kine_AngRec_AngSca_In->SetLineColor(7);
  TGraph *Kine_AngRec_AngSca_B = new TGraph(numKin,ThetaLabRecB,ThetaLabScaB);
  Kine_AngRec_AngSca_B->SetLineColor(8);
  TGraph *Kine_AngRec_AngSca_vert = new TGraph(numKin,ThetaLabSca,ThetaLabRec);
  Kine_AngRec_AngSca_vert->SetLineColor(6);
  TGraph *Kine_AngRec_AngSca_In_vert = new TGraph(numKin,ThetaLabScaIn,ThetaLabRecIn);
  Kine_AngRec_AngSca_In_vert->SetLineColor(7);
  TGraph *Kine_AngRec_AngSca_B_vert = new TGraph(numKin,ThetaLabScaB,ThetaLabRecB);
  Kine_AngRec_AngSca_B_vert->SetLineColor(8);
  TGraph *Kine_AngRec_AngSca_4 = new TGraph(numKin,ThetaLabRec4,ThetaLabSca4);
  Kine_AngRec_AngSca_4->SetLineColor(8);
  TGraph *Kine_AngRec_AngSca_4_vert = new TGraph(numKin,ThetaLabSca4,ThetaLabRec4);
  Kine_AngRec_AngSca_4_vert->SetLineColor(8);
  TGraph *AlphaKine =new TGraph(numKin,ThetaLabSca29,EnerLabSca29);
  AlphaKine->SetLineColor(6);
  TGraph *AlphaKine29 =new TGraph(numKin,ThetaLabSca29,EnerLabSca29);
  AlphaKine29->SetLineColor(7);
  TGraph *AlphaKine3 =new TGraph(numKin,ThetaLabRec3,EnerLabRec3);
  AlphaKine3->SetLineColor(6);
  TGraph *AlphaKine31 =new TGraph(numKin,ThetaLabRec31,EnerLabRec31);
  AlphaKine31->SetLineColor(6);
  TGraph *AlphaKine39 =new TGraph(numKin,ThetaLabRec39,EnerLabRec39);
  AlphaKine39->SetLineColor(6);
  TGraph *AlphaKine4 =new TGraph(numKin,ThetaLabRec4,EnerLabRec4);
  AlphaKine4->SetLineColor(6);
  TGraph *AlphaKine41 =new TGraph(numKin,ThetaLabRec41,EnerLabRec41);
  AlphaKine41->SetLineColor(6);
  TGraph *BeKine3 =new TGraph(numKin,ThetaLabSca3,EnerLabSca3);
  BeKine3->SetLineColor(6);
  TGraph *BeKine4 =new TGraph(numKin,ThetaLabSca4,EnerLabSca4);
  BeKine4->SetLineColor(6);

  Double_t xa[2] = {0.0,90.0};
  Double_t ya[2] = {90.0,0.0};
  TGraph *alpha = new TGraph(2,xa,ya);

  leg = new TLegend(0.1,0.1,0.2,0.2);
  leg->AddEntry(Kine_AngRec_AngSca, "elastic","l");
  leg->AddEntry(Kine_AngRec_AngSca_In, "inelastic","l");
  leg->AddEntry(alpha, "alpha-alpha scattering","l");

  TCutG *anglecut = new TCutG("missedelastic",5);
  anglecut->SetVarX("Angle0");
  anglecut->SetVarY("Angle2");
  anglecut->SetTitle("Graph");
  anglecut->SetLineColor(2);
  anglecut->SetPoint(0,45,12);
  anglecut->SetPoint(1,57,13);
  anglecut->SetPoint(2,72,5);
  anglecut->SetPoint(3,52,6);
  anglecut->SetPoint(4,40,9);
  anglecut->SetPoint(5,45,12);

  /*anglecut->SetPoint(0,47.0,25.0);
  anglecut->SetPoint(1,83.0,5.0);
  anglecut->SetPoint(2,82.0,2.0);
  anglecut->SetPoint(3,45.0,20.0);
  anglecut->SetPoint(4,25.0,20.0);
  anglecut->SetPoint(5,25.0,25.0);
  anglecut->SetPoint(4,47.0,25.0);
  */
  /*  anglecut->SetPoint(0,55,15);
  anglecut->SetPoint(1,66,5);
  anglecut->SetPoint(2,33,8);
  anglecut->SetPoint(3,40,17);
  anglecut->SetPoint(4,55,15);
  */
  /*
  anglecut->SetPoint(0,42,42);
  anglecut->SetPoint(1,61,24);
  anglecut->SetPoint(2,77,13);
  anglecut->SetPoint(3,87,28);
  anglecut->SetPoint(4,56,56);
  anglecut->SetPoint(5,42,42);
  */
  TCutG* wideElastic = new TCutG("wideElastic",12);
  wideElastic->SetVarX("Angle0");
  wideElastic->SetVarY("Angle2");
  wideElastic->SetTitle("Graph");
  wideElastic->SetLineColor(2);
  wideElastic->SetPoint(0,0.0,0.0);
  wideElastic->SetPoint(1,1.0,0.0);
  wideElastic->SetPoint(2,15.0,15.0);
  wideElastic->SetPoint(3,25.0,20.0);
  wideElastic->SetPoint(4,45.0,20.0);
  wideElastic->SetPoint(5,82.0,2.0);
  wideElastic->SetPoint(6,83.0,5.0);
  wideElastic->SetPoint(7,47.0,25.0);
  wideElastic->SetPoint(8,25.0,25.0);
  wideElastic->SetPoint(9,18.0,22.0);
  wideElastic->SetPoint(10,9.0,12.0);
  wideElastic->SetPoint(11,0.0,1.0);
  wideElastic->SetPoint(12,0.0,0.0);
  /*
  TCutG* wideElasticup = new TCutG("wideElasticup",15);
  wideElasticup->SetVarX("Angle0");
  wideElasticup->SetVarY("Angle2");
  wideElasticup->SetTitle("Graph");
  wideElasticup->SetLineColor(2);
  wideElasticup->SetPoint(0,0.0,0.0);
  wideElasticup->SetPoint(1,1.0,0.0);
  wideElasticup->SetPoint(2,15.0,10.0);
  wideElasticup->SetPoint(3,25.0,20.0);
  wideElasticup->SetPoint(4,45.0,20.0);
  wideElasticup->SetPoint(5,82.0,2.0);
  wideElasticup->SetPoint(6,88.0,0.0);
  wideElasticup->SetPoint(7,92.0,0.0);
  wideElasticup->SetPoint(8,85.0,6.0);
  wideElasticup->SetPoint(9,47.0,24.0);
  wideElasticup->SetPoint(10,35.0,30.0);
  wideElasticup->SetPoint(11,25.0,25.0);
  wideElasticup->SetPoint(12,15.0,15.0);
  wideElasticup->SetPoint(13,8.0,5.0);
  wideElasticup->SetPoint(14,0.0,1.0);
  wideElasticup->SetPoint(15,0.0,0.0);
  */
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

  TCutG *anglecut2 = new TCutG("elasticup",6);
  anglecut2->SetVarX("Angle0");
  anglecut2->SetVarY("Angle2");
  anglecut2->SetTitle("Graph");
  anglecut2->SetLineColor(2);
  anglecut2->SetPoint(0,25.0,47.0);
  anglecut2->SetPoint(1,5.0,83.0);
  anglecut2->SetPoint(2,2.0,82.0);
  anglecut2->SetPoint(3,20.0,45.0);
  anglecut2->SetPoint(4,20.0,25.0);
  anglecut2->SetPoint(5,25.0,25.0);
  anglecut2->SetPoint(6,25.0,47.0);

  TCutG *diffcut = new TCutG("gooddiff",4);
  diffcut->SetVarX("Angle0");
  diffcut->SetVarY("Angle2");
  diffcut->SetTitle("Graph");
  diffcut->SetLineColor(2);
  diffcut->SetPoint(0,50.0,15.0);
  diffcut->SetPoint(1,30.0,15.0);
  diffcut->SetPoint(2,30.0,8.0);
  diffcut->SetPoint(3,60.0,5.0);
  diffcut->SetPoint(4,50.0,15.0);

  TCutG *cut2 = new TCutG("inelastic",5); //INELASTIC
  cut2->SetVarX("Angle0");
  cut2->SetVarY("Angle2");
  cut2->SetTitle("Graph");
  cut2->SetLineColor(3);
  cut2->SetPoint(0,40.0,20.0);
  cut2->SetPoint(1,60.0,10.0);
  cut2->SetPoint(2,65.0,0.0);
  cut2->SetPoint(3,10.0,0.0);
  cut2->SetPoint(4,30.0,20.0);
  cut2->SetPoint(5,40.0,20.0);

  TCutG *LowEIn = new TCutG("Low_inelastic",4);
  LowEIn->SetVarX("Angle0");
  LowEIn->SetVarY("Angle2");
  LowEIn->SetLineColor(3);
  LowEIn->SetPoint(0,0.0,0.0);
  LowEIn->SetPoint(1,25.0,15.0);
  LowEIn->SetPoint(2,35.0,13.0);
  LowEIn->SetPoint(3,40.0,5.0);
  LowEIn->SetPoint(4,0.0,0.0);

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

  TCutG *cross1 = new TCutG("cross1",7);
  cross1->SetVarX("angle");
  cross1->SetVarY("energy");
  cross1->SetLineColor(4);
  cross1->SetPoint(0,91,3.1);
  cross1->SetPoint(1,114,4);
  cross1->SetPoint(2,135,5.4);
  cross1->SetPoint(3,133,5.6);
  cross1->SetPoint(4,123,4.8);
  cross1->SetPoint(5,108,4.2);
  cross1->SetPoint(6,96,3.6);
  cross1->SetPoint(7,91,3.1);

  TCutG *cross = new TCutG("cross",4);
  cross->SetVarX("angle");
  cross->SetVarY("energy");
  cross->SetLineColor(4);
  cross->SetPoint(0,44.9599,2.49355);
  cross->SetPoint(1,97.9685,1.99441);
  cross->SetPoint(2,25.6189,0.80293);
  cross->SetPoint(3,10.5759,1.60799);
  cross->SetPoint(4,44.9599,2.49355);

  TCutG *alphaang = new TCutG("alphaang",4);
  alphaang->SetVarX("angle");
  alphaang->SetVarY("energy");
  alphaang->SetLineColor(4);
  alphaang->SetPoint(0,50.0,3.58);
  alphaang->SetPoint(1,57.0,3.40);
  alphaang->SetPoint(2,76.0,0.52);
  alphaang->SetPoint(3,70.6,0.41);
  alphaang->SetPoint(4,50.0,3.58);

  TCutG *disappear = new TCutG("disappear",5);
  disappear->SetVarX("angle");
  disappear->SetVarY("energy");
  disappear->SetLineColor(4);
  disappear->SetPoint(0,24.0,1.48);
  disappear->SetPoint(1,80.0,1.48);
  disappear->SetPoint(2,85.0,0.41);
  disappear->SetPoint(3,36.0,0.34);
  disappear->SetPoint(4,24.0,1.09);
  disappear->SetPoint(5,24.0,1.48);

  Double_t maxDiff = 20000.0;
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
  Bool_t isAssym[2];
  Bool_t isHit[260];
  Int_t numPadsHit[4]={0,0,0,0};
  Double_t thetalab = 0.0;
  Double_t thetacm = 0.0;
  Double_t tVertex=0.0;
  Double_t maxAllowedRad = 300.0;
  Double_t minAllowedRad = 20.0;
  Int_t FitterPicker[2]={1,1};
  Double_t calVertex[4]={0,0,0,0};
  Double_t intVertex[4]={0,0,0,0};
  Double_t intXVertex[4]={0,0,0,0};
  Double_t maxHit[4]={0,0,0,0};
  Double_t maxHitRad[4]={0,0,0,0};
  Double_t maxDer[4]={0,0,0,0};
  Double_t realtVertex[4]={0,0,0,0};
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
  Double_t BeRange=0.0;
  Double_t energycm=0.0;
  Double_t energycm1=0.0;
  Double_t energycm2=-0.0;
  Double_t sol=0.0;
  Double_t totmax=0.0;

  TRandom randGen;
  ofstream myfile;
  myfile.open ("stuff.txt");

  TTreeReader Reader1("cbmsim", file);
  TTreeReaderValue<TClonesArray> eventArray(Reader1, "ATEventH");
  TTreeReaderValue<TClonesArray> analysisArray(Reader1, "ATProtoEventAna");

  //TTreeReader Reader2("cbmsim", file2);


  Int_t nEve =0;
  while (Reader1.Next()){
    if(nEve>=sol){
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
        AngleFit->at(i)=AngleFit->at(i);
      }
      for(Int_t i=0;i<4;i++){
        if(NDF->at(i)>0) RedChi2[i]=(Chi2->at(i)/double(NDF->at(i)));
        if(RedChi2[i]>0&&RedChi2[i]<15) isGoodFit[i]=kTRUE;
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
      tVertex=0.0;
      tVertex=((max-500.0)*100.0/(3.2*40.0))+390.0;
      tot_E = 0;
      for(Int_t i=0;i<=512;i++){
        if (MeshArray[i]>thresh) tot_E+=MeshArray[i];
      }
      //max = TMath::MaxElement(512, MeshArray);

      Delta_E->Fill(start,tot_E);

      for(Int_t i=0;i<4;i++){
        realtVertex[i]=0.0;
        realtVertex[i]=((vertex->at(i)-500.0)*100.0/(3.2*40.0))+390.0;//+randGen.Uniform(-0.5,0.5);
        calVertex[i]=500.0-((double(max)-realtVertex[i])*3.2*40.0/(100.0));
        //cout<<intVertex[i]<<endl;
        //cout<<nEve<<"\t"<<max<<"\t"<<tVertex<<"\t"<<i<<"\t"<<vertex->at(i)<<"\t"<<calVertex[i]<<endl;
        //Vertex->Fill(max);
        //Vertex1->Fill(vertex->at(i));
        //Vertex2->Fill(calVertex[i]);
      }

      for(Int_t i=0;i<4;i++){
        if((Par1->at(i)-Par1->at((i+2)%4))!=0){
          intVertex[i]=(Par1->at(i)*calVertex[(i+2)%4]-calVertex[i]*Par1->at((i+2)%4))/(Par1->at(i)-Par1->at((i+2)%4));
          intXVertex[i]=(calVertex[(i+2)%4]-calVertex[i])/(Par1->at(i)-Par1->at((i+2)%4));
        }
        else{

          intVertex[i]=calVertex[i];
          intXVertex[i]=0.0;
        }
      }
      //cout<<calVertex<<endl;

      for(Int_t i=0; i<260; i++){
        isHit[i]=kFALSE;
      }
      Double_t maxRad[4] = {-1,-1,-1,-1};
      Double_t ZmaxRad[4] = {-1,-1,-1,-1};
      Double_t maxDist[4] = {-1,-1,-1,-1};
      Double_t maxHit[4] = {-1,-1,-1,-1};
      Double_t maxRadHit[4] = {-1,-1,-1,-1};
      Double_t maxHitRad[4] = {-1,-1,-1,-1};
      Double_t maxHitZ[4] = {-1,-1,-1,-1};
      Double_t maxDer[4] = {-1,-1,-1,-1};
      Double_t maxDerRad[4] = {-1,-1,-1,-1};
      Double_t maxRadStep[4] = {-1,-1,-1,-1};
      Double_t calz=0.0;
      Double_t der=0.0;
      for(Int_t i=0; i<NumHits; i++){
        ATHit *Hit = event->GetHit(i);
        Int_t PadNum = Hit->GetHitPadNum();
        isHit[PadNum]=kTRUE;
        TVector3 coords = Hit->GetPosition();
        TVector3 coordscorr = Hit->GetPositionCorr();
        Int_t tb = Hit->GetTimeStamp();
        Double_t tbcorr = Hit->GetTimeStampCorr();
        rad = TMath::Sqrt(coords.x()*coords.x()+coords.y()*coords.y());//+randGen.Gaus(0.0,1.0);
        tVertex=0.0;
        tVertex=((coords.z()-500.0)*100.0/(3.2*40.0))+390.0;//+randGen.Uniform(-0.5,0.5);
        calz = 500.0-((double(max)-tVertex)*3.2*40.0/(100.0));
        Double_t amp = Hit->GetCharge();
        if(PadNum>=1&&PadNum<=64){
          if(amp>maxHit[0]){
            maxHit[0]=amp;
            maxHitRad[0]=rad;
            maxHitZ[0]=calz;
          }
        }
        if(PadNum>=65&&PadNum<=128){
          if(amp>maxHit[1]){
            maxHit[1]=amp;
            maxHitRad[1]=rad;
            maxHitZ[1]=calz;
          }
        }
        if(PadNum>=129&&PadNum<=192){
          if(amp>maxHit[2]){
            maxHit[2]=amp;
            maxHitRad[2]=rad;
            maxHitZ[2]=calz;
          }
        }
        if(PadNum>=193&&PadNum<=256){
          if(amp>maxHit[3]){
            maxHit[3]=amp;
            maxHitRad[3]=rad;
            maxHitZ[3]=calz;
          }
        }
        for(Int_t j; j<4;j++){
          maxRadStep[j]=maxHitRad[j];

        }
        if(PadNum>=1&&PadNum<=64){
          //dist = TMath::Sqrt(coords.x()*coords.x()+coords.y()*coords.y()+(intVertex[0]-coords.z())*(intVertex[0]-coords.z()));
          dist = rad/TMath::Sin(TMath::DegToRad()*AngleFit->at(0));
          if(rad>maxRad[0]){
            maxRad[0]=rad;
            ZmaxRad[0]=calz;
          }
          if(dist>maxDist[0]) maxDist[0]=dist;
          if(rad>maxHitRad[0]){
            der = TMath::Abs(amp-maxHit[0])/(rad-maxRadStep[0]);
            maxRadHit[0]=amp;
            maxRadStep[0]=rad;
            if(der>maxDer[0]){
              maxDer[0]=der;
              maxDerRad[0]=rad;
            }
          }
        }
        if(PadNum>=65&&PadNum<=128){
          //dist = TMath::Sqrt(coords.x()*coords.x()+coords.y()*coords.y()+(intVertex[1]-coords.z())*(intVertex[1]-coords.z()));
          //dist = TMath::Sqrt((rad - intXVertex[1])*(rad - intXVertex[1])+(intVertex[1]-coords.z())*(intVertex[1]-coords.z()));
          dist = rad/TMath::Sin(TMath::DegToRad()*AngleFit->at(1));
          if(rad>maxRad[1]){
            maxRad[1]=rad;
            ZmaxRad[1]=calz;
          }
          if(dist>maxDist[1]) maxDist[1]=dist;
          if(rad>maxHitRad[1]){
            der = TMath::Abs(amp-maxHit[1])/(rad-maxRadStep[1]);
            maxRadHit[1]=amp;
            maxRadStep[1]=rad;
            if(der>maxDer[1]){
              maxDer[1]=der;
              maxDerRad[1]=rad;
            }
          }
        }
        if(PadNum>=129&&PadNum<=192){
          //dist = TMath::Sqrt(coords.x()*coords.x()+coords.y()*coords.y()+(intVertex[2]-coords.z())*(intVertex[2]-coords.z()));
          //dist = TMath::Sqrt((rad - intXVertex[2])*(rad - intXVertex[2])+(intVertex[2]-coords.z())*(intVertex[2]-coords.z()));
          dist = rad/TMath::Sin(TMath::DegToRad()*AngleFit->at(2));
          if(rad>maxRad[2]){
            maxRad[2]=rad;
            ZmaxRad[2]=calz;
          }
          if(dist>maxDist[2]) maxDist[2]=dist;
          if(rad>maxHitRad[2]){
            der = TMath::Abs(amp-maxHit[2])/(rad-maxRadStep[2]);
            maxRadHit[2]=amp;
            maxRadStep[2]=rad;
            if(der>maxDer[2]){
              maxDer[2]=der;
              maxDerRad[2]=rad;
            }
          }
        }
        if(PadNum>=193&&PadNum<=256){
          //dist = TMath::Sqrt(coords.x()*coords.x()+coords.y()*coords.y()+(intVertex[3]-coords.z())*(intVertex[3]-coords.z()));
          //dist = TMath::Sqrt((rad - intXVertex[3])*(rad - intXVertex[3])+(intVertex[3]-coords.z())*(intVertex[3]-coords.z()));
          dist = rad/TMath::Sin(TMath::DegToRad()*AngleFit->at(3));
          if(rad>maxRad[3]){
            maxRad[3]=rad;
            ZmaxRad[3]=calz;
          }
          if(dist>maxDist[3]) maxDist[3]=dist;
          if(rad>maxHitRad[3]){
            der = TMath::Abs(amp-maxHit[3])/(rad-maxRadStep[3]);
            maxRadHit[3]=amp;
            maxRadStep[3]=rad;
            if(der>maxDer[3]){
              maxDer[3]=der;
              maxDerRad[3]=rad;
            }
          }
        }
      }

      for(Int_t i=0;i<4;i++){
        //Vertex->Fill(maxDerRad[i]);
        //Vertex1->Fill(maxRad[i]);
        //Vertex2->Fill(maxDerRad[i]-maxRad[i]);
      }

      totmax = totmax + TMath::MaxElement(4,maxHit)/10000.0;

      xquad[0]=-1;
      yquad[0]=-1;
      xquad[1]=-1;
      yquad[1]=-1;
      FitterPicker[0]=1;
      FitterPicker[1]=1;

      isAssym[0]=kFALSE;
      isAssym[1]=kFALSE;

      if(1==1){//PID->IsInside(start,tot_E)){//cross->IsInside(start,tot_E)
        if(AngleFit->at(2)>AngleFit->at(0)) {
          //  if((RedChi2->at(0)>0.0&&RedChi2->at(0)<15)&&(RedChi2->at(2)>0.0&&RedChi2->at(2)<15)){
          if( (TMath::Abs(calVertex[0] - calVertex[2])<maxDiff && (calVertex[0]>0 && calVertex[2]>0) )&&(isGoodFit[2])){
            //Vertex->Fill(AngleFit->at(2));
            //if(AngleFit->at(2)<30)myfile<<nEve<<"\t2\t"<<AngleFit->at(2)<<endl;
          }
          //if( (TMath::Abs(calVertex[0] - calVertex[2])<maxDiff && (calVertex[0]>0 && calVertex[2]>0) )&&(isGoodFit[0]&&isGoodFit[2]) &&maxRad[0]>minAllowedRad&&maxRad[2]>minAllowedRad&&TMath::MaxElement(4,maxRad)<maxAllowedRad )//
          if(maxRad[0]>minAllowedRad&&maxRad[2]>minAllowedRad&&AngleFit->at(0)>6.0&&AngleFit->at(2)>6.0&&TMath::MaxElement(4,maxRad)<maxAllowedRad)//&&calVertex[0]>480.0&&calVertex[0]<500.0&&calVertex[2]>480.0&&calVertex[2]<500.0)
          {
            //cout<<nEve<<endl;

            //Delta_E->Fill(start,tot_E);
            //  Max_E->Fill(max*TMath::Cos(AngleFit->at(2)));
            x1 = AngleFit->at(2);
            y1 = AngleFit->at(0);
            xquad[0]=2;
            yquad[0]=0;
            //Q02_Kine->Fill(x1,y1);//}
            if(y1<6.0&&x1>0)isAssym[0]=kTRUE;
            //Q02_Kine->Fill(AngleFit->at(0),AngleFit->at(2));

          }
          else if( (TMath::Abs(calVertex[0] - calVertex[2])<maxDiff && (calVertex[0]>0 && calVertex[2]>0) )&&(isGoodFit[0]&&isGoodFit[2]) &&anglecut->IsInside(AngleFit->at(2),AngleFit->at(0))&&TMath::MaxElement(4,maxRad)<maxAllowedRad )
          {
            x1 = AngleFit->at(2);
            y1 = AngleFit->at(0);
            for(Int_t i=0;i<1138;i++){
              if(y1<ThetaLabSca[i]){
                x1=randGen.Gaus(ThetaLabRec[i],5.0);
                break;
              }
            }
            xquad[0]=2;
            yquad[0]=0;
            //Q02_Kine->Fill(x1,y1);
            x1=0.0;
            y1=0.0;
            xquad[0]=-1;
            yquad[0]=-1;
            FitterPicker[0]=0;
          }
          else{

            x1=0.0;
            y1=0.0;
            xquad[0]=-1;
            yquad[0]=-1;
            FitterPicker[0]=0;
          }
        }
        else {
          if(  (TMath::Abs(calVertex[0] - calVertex[2])<maxDiff && (calVertex[0]>0 && calVertex[2]>0) )&&(isGoodFit[0])&&(AngleFit->at(0)>AngleFit->at(2))){
            //Vertex->Fill(AngleFit->at(0));
            //if(AngleFit->at(0)<30)myfile<<nEve<<"\t0\t"<<AngleFit->at(0)<<endl;
          }
          //if((RedChi2->at(0)>0.0&&RedChi2->at(0)<15)&&(RedChi2->at(2)>0.0&&RedChi2->at(2)<15)){
          //if(  (TMath::Abs(calVertex[0] - calVertex[2])<maxDiff && (calVertex[0]>0 && calVertex[2]>0) )&&(isGoodFit[0]&&isGoodFit[2]) &&maxRad[0]>minAllowedRad&&maxRad[2]>minAllowedRad&&TMath::MaxElement(4,maxRad)<maxAllowedRad )//
          if(maxRad[0]>minAllowedRad&&maxRad[2]>minAllowedRad&&AngleFit->at(0)>6.0&&AngleFit->at(2)>6.0&&TMath::MaxElement(4,maxRad)<maxAllowedRad)//&&calVertex[0]>480.0&&calVertex[0]<500.0&&calVertex[2]>480.0&&calVertex[2]<500.0)
          {

            //cout<<nEve<<endl;
            x1 = AngleFit->at(0);
            y1 = AngleFit->at(2);
            //  Delta_E->Fill(start,tot_E);
            //  Max_E->Fill(max*TMath::Cos(AngleFit->at(0)));
            xquad[0]=0;
            yquad[0]=2;
            //Q02_Kine->Fill(x1,y1);//}
            //Q02_Kine->Fill(AngleFit->at(0),AngleFit->at(2));
            if(x1<6.0&&y1>0)isAssym[0]=kTRUE;

          }
          else if(  (TMath::Abs(calVertex[0] - calVertex[2])<maxDiff && (calVertex[0]>0 && calVertex[2]>0) )&&(isGoodFit[0]&&isGoodFit[2]) &&anglecut->IsInside(AngleFit->at(0),AngleFit->at(2))&&TMath::MaxElement(4,maxRad)<maxAllowedRad )

          {
            x1 = AngleFit->at(0);
            y1 = AngleFit->at(2);
            for(Int_t i=0;i<1138;i++){
              if(y1<ThetaLabSca[i]){
                x1=randGen.Gaus(ThetaLabRec[i],5.0);
                break;
              }
            }
            xquad[0]=0;
            yquad[0]=2;
            //Q02_Kine->Fill(x1,y1);
            x1=0.0;
            y1=0.0;
            xquad[0]=-1;
            yquad[0]=-1;
            FitterPicker[0]=0;
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
          if( (TMath::Abs(calVertex[1] - calVertex[3])<maxDiff && (calVertex[1]>0 && calVertex[3]>0) )&&(isGoodFit[3])){
            //
            //Vertex->Fill(AngleFit->at(3));
            //if(AngleFit->at(3)<30)myfile<<nEve<<"\t3\t"<<AngleFit->at(3)<<endl;
          }
          //  if((RedChi2->at(1)>0.0&&RedChi2->at(1)<15)&&(RedChi2->at(3)>0.0&&RedChi2->at(3)<15)){
          //if(  (TMath::Abs(calVertex[1] - calVertex[3])<maxDiff && (calVertex[1]>0 && calVertex[3]>0) )&&(isGoodFit[1]&&isGoodFit[3]) &&maxRad[1]>minAllowedRad&&maxRad[3]>minAllowedRad&&TMath::MaxElement(4,maxRad)<maxAllowedRad )//
          if(maxRad[1]>minAllowedRad&&maxRad[3]>minAllowedRad&&AngleFit->at(1)>6.0&&AngleFit->at(3)>6.0&&TMath::MaxElement(4,maxRad)<maxAllowedRad)//&&calVertex[1]>480.0&&calVertex[1]<500.0&&calVertex[3]>480.0&&calVertex[3]<500.0)
          {

            //cout<<nEve<<endl;
            x2 = AngleFit->at(3);
            y2 = AngleFit->at(1);
            //Delta_E->Fill(start,tot_E);
            //  Max_E->Fill(max*TMath::Cos(AngleFit->at(3)));
            xquad[1]=3;
            yquad[1]=1;
            //Q02_Kine->Fill(x2,y2);//}
            //Q02_Kine->Fill(AngleFit->at(1),AngleFit->at(3));
            if(y2<6.0&&x2>0)isAssym[1]=kTRUE;

          }
          else if(  (TMath::Abs(calVertex[1] - calVertex[3])<maxDiff && (calVertex[1]>0 && calVertex[3]>0) )&&(isGoodFit[1]&&isGoodFit[3]) &&anglecut->IsInside(AngleFit->at(3),AngleFit->at(1))&&TMath::MaxElement(4,maxRad)<maxAllowedRad )
          {

            x2 = AngleFit->at(3);
            y2 = AngleFit->at(1);
            for(Int_t i=0;i<1138;i++){
              if(y1<ThetaLabSca[i]){
                x2=randGen.Gaus(ThetaLabRec[i],5.0);
                break;
              }
            }
            xquad[1]=3;
            yquad[1]=1;
            x2=0.0;
            y2=0.0;
            //Q02_Kine->Fill(x2,y2);
            xquad[1]=-1;
            yquad[1]=-1;
            FitterPicker[1]=0;
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
          if( (TMath::Abs(calVertex[1] - calVertex[3])<maxDiff && (calVertex[1]>0 && calVertex[3]>0) )&&(isGoodFit[1])&&(AngleFit->at(1)>AngleFit->at(3))){
            //  Vertex->Fill(AngleFit->at(1));
            //if(AngleFit->at(1)<30)myfile<<nEve<<"\t1\t"<<AngleFit->at(1)<<endl;
          }
          //  if((RedChi2->at(1)>0.0&&RedChi2->at(1)<15)&&(RedChi2->at(3)>0.0&&RedChi2->at(3)<15)){
          //if( (TMath::Abs(calVertex[1] - calVertex[3])<maxDiff && (calVertex[1]>0 && calVertex[3]>0) )&&(isGoodFit[1]&&isGoodFit[3]) &&maxRad[1]>minAllowedRad&&maxRad[3]>minAllowedRad&&TMath::MaxElement(4,maxRad)<maxAllowedRad )//
          if(maxRad[1]>minAllowedRad&&maxRad[3]>minAllowedRad&&AngleFit->at(1)>6.0&&AngleFit->at(3)>6.0&&TMath::MaxElement(4,maxRad)<maxAllowedRad)//&&calVertex[1]>480.0&&calVertex[1]<500.0&&calVertex[3]>480.0&&calVertex[3]<500.0)
          {
            //cout<<nEve<<endl;

            x2 = AngleFit->at(1);
            y2 = AngleFit->at(3);
            //Delta_E->Fill(start,tot_E);
            //Max_E->Fill(max*TMath::Cos(AngleFit->at(1)));
            xquad[1]=1;
            yquad[1]=3;
            //Q02_Kine->Fill(x2,y2);//}
            //Q02_Kine->Fill(AngleFit->at(1),AngleFit->at(3));
            if(x2<6.0&&y2>0)isAssym[1]=kTRUE;
          }
          else if( (TMath::Abs(calVertex[1] - calVertex[3])<maxDiff && (calVertex[1]>0 && calVertex[3]>0) )&&(isGoodFit[1]&&isGoodFit[3]) &&anglecut->IsInside(AngleFit->at(1),AngleFit->at(3))&&TMath::MaxElement(4,maxRad)<maxAllowedRad )//
          {

            x2 = AngleFit->at(1);
            y2 = AngleFit->at(3);
            //Delta_E->Fill(start,tot_E);
            //Max_E->Fill(max*TMath::Cos(AngleFit->at(1)));
            for(Int_t i=0;i<1138;i++){
              if(y1<ThetaLabSca[i]){
                x2=randGen.Gaus(ThetaLabRec[i],5.0);
                break;
              }
            }
            xquad[1]=1;
            yquad[1]=3;
            //Q02_Kine->Fill(x2,y2);
            x2=0.0;
            y2=0.0;
            xquad[1]=-1;
            yquad[1]=-1;
            FitterPicker[1]=0;
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

  if(FitterPicker[0]==1&&0.5*(calVertex[0]+calVertex[2])>250.0&&0.5*(calVertex[0]+calVertex[2])<250.0){
    Q02_Kine->Fill(x1,y1);
  }
  if(FitterPicker[0]==1&&0.5*(calVertex[0]+calVertex[2])>375.0){
    //Q13_Kine->Fill(x1,y1);
  }
  if(FitterPicker[1]==1&&0.5*(calVertex[1]+calVertex[3])>250.0&&0.5*(calVertex[1]+calVertex[3])<375.0){
    Q02_Kine->Fill(x1,y1);
  }
  if(FitterPicker[1]==1&&0.5*(calVertex[1]+calVertex[3])>375.0){
    //Q13_Kine->Fill(x1,y1);
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
  Double_t energycma=0.0;
  Double_t energycmb=0.0;
  energycm1=0.0;
  energycm2=0.0;
  Double_t energycm3=0.0;
  if(wideElasticup->IsInside(x1,y1)&&FitterPicker[0]==1&&x1>0&&y1>0){
    alphaquad=xquad[0];
    bequad=yquad[0];
    /*if(AngleFit->at(xquad[0])<20.0){
      alphaquad=yquad[0];
      bequad=xquad[0];
    }*/
    //AlphaRange = TMath::Sqrt((maxRad[alphaquad]-intXVertex[alphaquad])*(maxRad[alphaquad]-intXVertex[alphaquad])+(ZmaxRad[alphaquad]-intVertex[alphaquad])*(ZmaxRad[alphaquad]-intVertex[alphaquad]));
    //BeRange = TMath::Sqrt((maxRad[bequad]-intXVertex[bequad])*(maxRad[bequad]-intXVertex[bequad])+(ZmaxRad[bequad]-intVertex[bequad])*(ZmaxRad[bequad]-intVertex[bequad]));
    AlphaRange=(maxRad[alphaquad]-intXVertex[alphaquad])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad)));
    BeRange=(maxRad[bequad]-intXVertex[alphaquad])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad)));//+randGen.Uniform(-1.0,1.0);
    if(TMath::MaxElement(4,maxRad)>118.0){
      Vertex1->Fill(maxRad[bequad]-intXVertex[alphaquad]);
    }
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
    //bestindex=bestindex-1;
    //cout<<bestindex<<"\t"<<bestrange<<endl;
    AlphaEnergy = AlphaEnTab[bestindex]+(AlphaEnTab[bestindex+1]-AlphaEnTab[bestindex])*(AlphaRange-bestrange)/(AlphaRangeTab[bestindex+1]-bestrange);//+randGen.Gaus(0,0.1);
    BeEnergy1 = BeEnTab[bestindex1]+(BeEnTab[bestindex1+1]-BeEnTab[bestindex1])*(BeRange-bestrange1)/(BeRangeTab[bestindex1+1]-bestrange1);//+randGen.Gaus(0,0.1);
    AlphaMomentum = TMath::Sqrt(double(AlphaEnergy)*2.0*4000.0);
    BeMomentum1 = TMath::Sqrt(double(BeEnergy1)*2.0*10000.0);
    BeMomentum= AlphaMomentum*TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad));
    AlphaMomentum1= BeMomentum1*TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad));
    BeEnergy = BeMomentum*BeMomentum/(2.0*10000.0);
    AlphaEnergy1 = AlphaMomentum1*AlphaMomentum1/(2.0*4000.0);
    energycm = 2.0/7.0*(BeEnergy+AlphaEnergy);
    energycm3 = TMath::Abs(calVertex[alphaquad]);
    energycm2 = 2.0/7.0*(BeEnergy1+AlphaEnergy1);


    //myfile<<AlphaEnergy<<"\t"<<AlphaMomentum<<"\t"<<AngleFit->at(alphaquad)<<"\t"<<AngleFit->at(bequad)<<"\t"<<BeMomentum<<"\t"<<BeEnergy<<"\t"<<energycm<<endl;
    Max_E->Fill(max);
    thetalab = x1;
    if (xquad[0]==0) Vertex_vs_Angle->Fill(calVertex[0],AngleFit->at(0));
    if (xquad[0]==2) Vertex_vs_Angle->Fill(calVertex[2],AngleFit->at(2));
    if (yquad[0]==0) Vertex_vs_Angle2->Fill(calVertex[0],AngleFit->at(0));
    if (yquad[0]==2) Vertex_vs_Angle2->Fill(calVertex[2],AngleFit->at(2));
    Delta_E_cut->Fill(start,tot_E);
    thetacm=180-(2*thetalab);
    if (thetacm<0) thetacm= thetacm+180.0;
    Int_t index=int(floor(intVertex[0]));
    Double_t over = intVertex[0]-index;
    if(index<=500&&index>0){
      energycm1=(2.0/7.0)*(EnergyMM[500-index]+(EnergyMM[500-index]-EnergyMM[500-index+1])*over)/1000000.0;
    }
    if(index>-500&&index<0){
      index=TMath::Abs(index);
      energycm1=(2.0/7.0)*(19.5+(EnergyMM[500-index]+(EnergyMM[500-index]-EnergyMM[500-index+1])*over)/1000000.0);
      //myfile<<intVertex[0]<<"\t"<<energycm1<<index<<endl;
    }

    if(TMath::MaxElement(4,maxRad)<=118.0)energycma=2.0/7.0*(AlphaEnergy+BeEnergy1);
    if(TMath::MaxElement(4,maxRad)>118.0)energycma=energycm2;
    if(TMath::MaxElement(4,maxRad)<=118.0)energycmb=(energycm+energycm2)/2.0;
    if(TMath::MaxElement(4,maxRad)>118.0)energycmb=energycm2;
    if(TMath::MaxElement(4,maxRad)<=118.0)myfile<<nEve<<"\t"<<energycm<<"\t"<<energycm2<<endl;
    //Excitation_EL->Fill(thetacm,energycma);
    //myfile<<nEve<<"\t0\t"<<intVertex[0]<<"\tmax\t"<<max<<"\tt0\t"<<realtVertex[0]<<"\tt2\t"<<realtVertex[2]<<endl;
    //if(ThreeD>250&&ThreeD<300&&AlphaRange>20&&AlphaRange<40)myfile<<nEve<<endl;
    //if(energycm1>2.9&&energycm1<3.0)Excitation_IN->Fill(AngleFit->at(bequad),BeEnergy1);

    //if(TMath::Abs(ThreeD-AlphaRange)<1.0&&TMath::Abs(ThreeDB-BeRange<1.0)){}
    //Excitation_ELa->Fill(thetacm,energycm1);
    //if(TMath::MaxElement(4,maxRad)<=118.0)Excitation_INa->Fill(energycm1,2.0/7.0*(AlphaEnergy+BeEnergy1));
    //Vertex->Fill(TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad)));

    if(energycma>0.0){
      if(thetacm<10.0) Excitation10->Fill(energycma);
      else if(thetacm<20.0)Excitation20->Fill(energycma);
      else if(thetacm<30.0)Excitation30->Fill(energycma);
      else if(thetacm<40.0)Excitation40->Fill(energycma);
      else if(thetacm<50.0)Excitation50->Fill(energycma);
      else if(thetacm<60.0)Excitation60->Fill(energycma);
      else if(thetacm<70.0)Excitation70->Fill(energycma);
      else if(thetacm<80.0)Excitation80->Fill(energycma);
      else if(thetacm<90.0)Excitation90->Fill(energycma);
      else if(thetacm<100.0)Excitation100->Fill(energycma);
      else if(thetacm<110.0)Excitation110->Fill(energycma);
      else if(thetacm<120.0)Excitation120->Fill(energycma);
      else if(thetacm<130.0)Excitation130->Fill(energycma);
      else if(thetacm<140.0)Excitation140->Fill(energycma);
      else if(thetacm<150.0)Excitation150->Fill(energycma);
      else if(thetacm<160.0)Excitation160->Fill(energycma);
      else if(thetacm<170.0)Excitation170->Fill(energycma);
      else if(thetacm<180.0)Excitation180->Fill(energycma);
    }
    if(energycma<2.5){
      if(energycma>2.4)Energy2000->Fill(thetacm);
      else if(energycma>2.3)Energy1900->Fill(thetacm);
      else if(energycma>2.2)Energy1800->Fill(thetacm);
      else if(energycma>2.1)Energy1700->Fill(thetacm);
      else if(energycma>2.0)Energy1600->Fill(thetacm);
      else if(energycma>1.9)Energy1500->Fill(thetacm);
      else if(energycma>1.8)Energy1400->Fill(thetacm);
      else if(energycma>1.7)Energy1300->Fill(thetacm);
      else if(energycma>1.6)Energy1200->Fill(thetacm);
      else if(energycma>1.5)Energy1100->Fill(thetacm);
      else if(energycma>1.4)Energy1000->Fill(thetacm);
      else if(energycma>1.3)Energy900->Fill(thetacm);
      else if(energycma>1.2)Energy800->Fill(thetacm);
      else if(energycma>1.1)Energy700->Fill(thetacm);
      else if(energycma>1.0)Energy600->Fill(thetacm);
      else if(energycma>0.9)Energy500->Fill(thetacm);
      else if(energycma>0.8)Energy400->Fill(thetacm);
      else if(energycma>0.7)Energy300->Fill(thetacm);
      else if(energycma>0.6)Energy200->Fill(thetacm);
      else if(energycma>0.5)Energy100->Fill(thetacm);
    }



    for(Int_t j=1; j<=64; j++){
      if(isHit[j]==kTRUE) numPadsHit[0] ++;
    }
    for(Int_t j=129; j<=192; j++){
      if(isHit[j]==kTRUE) numPadsHit[2] ++;
    }
    if(1==1){
      //myfile<<"0\t"<<nEve<<"\t"<<RedChi2[0]<<"\t"<<RedChi2[2]<<endl;

      if (xquad[0]==0)  numHitsHist->Fill(calVertex[0],numPadsHit[0]);
      if (xquad[0]==2)  numHitsHist->Fill(calVertex[2],numPadsHit[2]);
      if (yquad[0]==0) numHitsHist2->Fill(calVertex[0],numPadsHit[0]);
      if (yquad[0]==2) numHitsHist2->Fill(calVertex[2],numPadsHit[2]);
      //if(energycma<1.3)Q13_Kine->Fill(x1,y1);
    }
  }



  thetalab = 0.0;
  thetacm = 0.0;
  energycm=0.0;
  energycma=0.0;
  energycmb=0.0;
  energycm1=0.0;
  energycm2=0.0;
  energycm3=0.0;
  bestrange=0.0;
  bestrange1=0.0;
  if(wideElasticup->IsInside(x2,y2)&&FitterPicker[1]==1&&x2>0&&y2>0){
    alphaquad=xquad[1];
    bequad=yquad[1];
    /*if(AngleFit->at(xquad[1])<20.0){
      alphaquad=yquad[1];
      bequad=xquad[1];
    }*/
    //AlphaRange= TMath::Sqrt((maxRad[alphaquad]-intXVertex[alphaquad])*(maxRad[alphaquad]-intXVertex[alphaquad])+(ZmaxRad[alphaquad]-intVertex[alphaquad])*(ZmaxRad[alphaquad]-intVertex[alphaquad]));
    //BeRange = TMath::Sqrt((maxRad[bequad]-intXVertex[bequad])*(maxRad[bequad]-intXVertex[bequad])+(ZmaxRad[bequad]-intVertex[bequad])*(ZmaxRad[bequad]-intVertex[bequad]));
    AlphaRange=(maxRad[alphaquad]-intXVertex[alphaquad])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad)));
    BeRange=(maxRad[bequad]-intXVertex[alphaquad])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad)));//+randGen.Uniform(-1.0,1.0);
    if(TMath::MaxElement(4,maxRad)>118.0){
      Vertex1->Fill(maxRad[bequad]-intXVertex[alphaquad]);
    }
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
      //cout<<bestindex<<"\t"<<bestrange<<endl;
    }
    //cout<<bestindex<<"\t"<<bestrange<<endl;
    AlphaEnergy = AlphaEnTab[bestindex]+(AlphaEnTab[bestindex+1]-AlphaEnTab[bestindex])*(AlphaRange-bestrange)/(AlphaRangeTab[bestindex+1]-bestrange);//+randGen.Gaus(0,0.1);
    BeEnergy1 = BeEnTab[bestindex1]+(BeEnTab[bestindex1+1]-BeEnTab[bestindex1])*(BeRange-bestrange1)/(BeRangeTab[bestindex1+1]-bestrange1);//+randGen.Uniform(0,0.1);
    AlphaMomentum = TMath::Sqrt(double(AlphaEnergy)*2.0*4000.0);
    BeMomentum1 = TMath::Sqrt(double(BeEnergy1)*2.0*10000.0);
    BeMomentum= AlphaMomentum*TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad));
    AlphaMomentum1= BeMomentum1*TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad));
    BeEnergy = BeMomentum*BeMomentum/(2.0*10000.0);
    AlphaEnergy1 = AlphaMomentum1*AlphaMomentum1/(2.0*4000.0);
    energycm = 2.0/7.0*(BeEnergy+AlphaEnergy);
    energycm3 = TMath::Abs(calVertex[alphaquad]);
    energycm2 = 2.0/7.0*(BeEnergy1+AlphaEnergy1);
    //myfile<<AlphaEnergy<<"\t"<<AlphaMomentum<<"\t"<<AngleFit->at(alphaquad)<<"\t"<<AngleFit->at(bequad)<<"\t"<<BeMomentum<<"\t"<<BeEnergy<<"\t"<<energycm<<endl;
    if (xquad[1]==1) Vertex_vs_Angle->Fill(calVertex[1],AngleFit->at(1));
    if (xquad[1]==3) Vertex_vs_Angle->Fill(calVertex[3],AngleFit->at(3));
    if (yquad[1]==1) Vertex_vs_Angle2->Fill(calVertex[1],AngleFit->at(1));
    if (yquad[1]==3) Vertex_vs_Angle2->Fill(calVertex[3],AngleFit->at(3));
    thetalab = x2;
    Delta_E_cut->Fill(start,tot_E);
    thetacm=180-(2*thetalab);
    if (thetacm<0) thetacm= thetacm+180.0;
    Int_t index=int(floor(intVertex[1]));
    Double_t over = intVertex[1]-index;
    if(index<=500&&index>0){
      energycm1=(2.0/7.0)*(EnergyMM[500-index]+(EnergyMM[500-index]-EnergyMM[500-index+1])*over)/1000000.0;
    }
    if(index>-500&&index<0){
      index=TMath::Abs(index);
      energycm1=(2.0/7.0)*(19.5+(EnergyMM[500-index]+(EnergyMM[500-index]-EnergyMM[500-index+1])*over)/1000000.0);
      //myfile<<intVertex[1]<<"\t"<<energycm1<<endl;
    }

    if(TMath::MaxElement(4,maxRad)<=118.0)energycma=2.0/7.0*(AlphaEnergy+BeEnergy1);
    if(TMath::MaxElement(4,maxRad)>118.0)energycma=energycm2;
    if(TMath::MaxElement(4,maxRad)<=118.0)energycmb=(energycm+energycm2)/2.0;
    if(TMath::MaxElement(4,maxRad)>118.0)energycmb=energycm2;
    if(TMath::MaxElement(4,maxRad)<=118.0)myfile<<nEve<<"\t"<<energycm<<"\t"<<energycm2<<endl;
    //if(thetalab>50.0&&thetalab<55.0)Excitation_EL->Fill(AlphaRange,BeEnergy);
    //if(TMath::MaxElement(4,maxRad)<=118.0)Excitation_IN->Fill(energycm,energycm2);
    Double_t ThreeD = TMath::Sqrt((maxRad[alphaquad]-intXVertex[alphaquad])*(maxRad[alphaquad]-intXVertex[alphaquad])+(ZmaxRad[alphaquad]-intVertex[alphaquad])*(ZmaxRad[alphaquad]-intVertex[alphaquad]));
    Double_t ThreeDB = TMath::Sqrt((maxRad[bequad]-intXVertex[bequad])*(maxRad[bequad]-intXVertex[bequad])+(ZmaxRad[bequad]-intVertex[bequad])*(ZmaxRad[bequad]-intVertex[bequad]));

    //Excitation_EL->Fill(thetacm,energycma);
    //if(ThreeD>250&&ThreeD<300&&AlphaRange>20&&AlphaRange<40)myfile<<nEve<<endl;
    //if(energycm1>2.9&&energycm1<3.0)Excitation_IN->Fill(AngleFit->at(bequad),BeEnergy1);
    //myfile<<nEve<<"\t1\t"<<intVertex[1]<<"\tmax\t"<<max<<"\tt1\t"<<realtVertex[1]<<"\tt3\t"<<realtVertex[3]<<endl;
    //if(TMath::Abs(ThreeD-AlphaRange)<1.0&&TMath::Abs(ThreeDB-BeRange<1.0)){}
  //  Excitation_ELa->Fill(thetacm,energycm1);
  //  if(TMath::MaxElement(4,maxRad)<=118.0)Excitation_INa->Fill(energycm1,2.0/7.0*(AlphaEnergy+BeEnergy1));
  //  Vertex->Fill(TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad)));

    if(energycma>0.0){
      if(thetacm<10.0) Excitation10->Fill(energycma);
      else if(thetacm<20.0)Excitation20->Fill(energycma);
      else if(thetacm<30.0)Excitation30->Fill(energycma);
      else if(thetacm<40.0)Excitation40->Fill(energycma);
      else if(thetacm<50.0)Excitation50->Fill(energycma);
      else if(thetacm<60.0)Excitation60->Fill(energycma);
      else if(thetacm<70.0)Excitation70->Fill(energycma);
      else if(thetacm<80.0)Excitation80->Fill(energycma);
      else if(thetacm<90.0)Excitation90->Fill(energycma);
      else if(thetacm<100.0)Excitation100->Fill(energycma);
      else if(thetacm<110.0)Excitation110->Fill(energycma);
      else if(thetacm<120.0)Excitation120->Fill(energycma);
      else if(thetacm<130.0)Excitation130->Fill(energycma);
      else if(thetacm<140.0)Excitation140->Fill(energycma);
      else if(thetacm<150.0)Excitation150->Fill(energycma);
      else if(thetacm<160.0)Excitation160->Fill(energycma);
      else if(thetacm<170.0)Excitation170->Fill(energycma);
      else if(thetacm<180.0)Excitation180->Fill(energycma);
    }

    if(energycma<2.5){
      if(energycma>2.4)Energy2000->Fill(thetacm);
      else if(energycma>2.3)Energy1900->Fill(thetacm);
      else if(energycma>2.2)Energy1800->Fill(thetacm);
      else if(energycma>2.1)Energy1700->Fill(thetacm);
      else if(energycma>2.0)Energy1600->Fill(thetacm);
      else if(energycma>1.9)Energy1500->Fill(thetacm);
      else if(energycma>1.8)Energy1400->Fill(thetacm);
      else if(energycma>1.7)Energy1300->Fill(thetacm);
      else if(energycma>1.6)Energy1200->Fill(thetacm);
      else if(energycma>1.5)Energy1100->Fill(thetacm);
      else if(energycma>1.4)Energy1000->Fill(thetacm);
      else if(energycma>1.3)Energy900->Fill(thetacm);
      else if(energycma>1.2)Energy800->Fill(thetacm);
      else if(energycma>1.1)Energy700->Fill(thetacm);
      else if(energycma>1.0)Energy600->Fill(thetacm);
      else if(energycma>0.9)Energy500->Fill(thetacm);
      else if(energycma>0.8)Energy400->Fill(thetacm);
      else if(energycma>0.7)Energy300->Fill(thetacm);
      else if(energycma>0.6)Energy200->Fill(thetacm);
      else if(energycma>0.5)Energy100->Fill(thetacm);
    }


    for(Int_t j=65; j<=128; j++){
      if(isHit[j]==kTRUE) numPadsHit[1] ++;
    }
    for(Int_t j=193; j<=256; j++){
      if(isHit[j]==kTRUE) numPadsHit[3] ++;
    }
    if(1==1){
      //myfile<<"1\t"<<nEve<<"\t"<<RedChi2[1]<<"\t"<<RedChi2[3]<<endl;

      if (xquad[1]==1) numHitsHist->Fill(calVertex[1],numPadsHit[1]);
      if (xquad[1]==3) numHitsHist->Fill(calVertex[3],numPadsHit[3]);
      if (yquad[1]==1) numHitsHist2->Fill(calVertex[1],numPadsHit[1]);
      if (yquad[1]==3) numHitsHist2->Fill(calVertex[3],numPadsHit[3]);
      //if(energycma<1.2)Q13_Kine->Fill(x2,y2);
    }
  }

  thetalab = 0.0;
  thetacm = 0.0;
  energycm=0.0;
  energycma=0.0;
  energycmb=0.0;
  energycm1=0.0;
  energycm2=0.0;
  energycm3=0.0;
  bestrange=0.0;
  bestrange1=0.0;
  if(LowEIn->IsInside(x1,y1)&&FitterPicker[0]==1&&x1>0&&y1>0){
    alphaquad=xquad[0];
    bequad=yquad[0];
    /*if(AngleFit->at(xquad[0])<20.0){
      alphaquad=yquad[0];
      bequad=xquad[0];
    }*/
    //AlphaRange = TMath::Sqrt((maxRad[alphaquad]-intXVertex[alphaquad])*(maxRad[alphaquad]-intXVertex[alphaquad])+(ZmaxRad[alphaquad]-intVertex[alphaquad])*(ZmaxRad[alphaquad]-intVertex[alphaquad]));
    //BeRange = TMath::Sqrt((maxRad[bequad]-intXVertex[bequad])*(maxRad[bequad]-intXVertex[bequad])+(ZmaxRad[bequad]-intVertex[bequad])*(ZmaxRad[bequad]-intVertex[bequad]));
    AlphaRange=(maxRad[alphaquad]-intXVertex[alphaquad])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad)));
    BeRange=(maxRad[bequad]-intXVertex[alphaquad])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad)));//+randGen.Uniform(-1.0,1.0);
    if(TMath::MaxElement(4,maxRad)>118.0){
      Vertex1->Fill(maxRad[bequad]-intXVertex[alphaquad]);
    }
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
    //bestindex=bestindex-1;
    //cout<<bestindex<<"\t"<<bestrange<<endl;
    AlphaEnergy = AlphaEnTab[bestindex]+(AlphaEnTab[bestindex+1]-AlphaEnTab[bestindex])*(AlphaRange-bestrange)/(AlphaRangeTab[bestindex+1]-bestrange);//+randGen.Gaus(0,0.1);
    BeEnergy1 = BeEnTab[bestindex1]+(BeEnTab[bestindex1+1]-BeEnTab[bestindex1])*(BeRange-bestrange1)/(BeRangeTab[bestindex1+1]-bestrange1);//+randGen.Gaus(0,0.1);
    AlphaMomentum = TMath::Sqrt(double(AlphaEnergy)*2.0*4000.0);
    BeMomentum1 = TMath::Sqrt(double(BeEnergy1)*2.0*10000.0);
    BeMomentum= AlphaMomentum*TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad));
    AlphaMomentum1= BeMomentum1*TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad));
    BeEnergy = BeMomentum*BeMomentum/(2.0*10000.0);
    AlphaEnergy1 = AlphaMomentum1*AlphaMomentum1/(2.0*4000.0);
    energycm = 2.0/7.0*(BeEnergy+AlphaEnergy);
    energycm3 = TMath::Abs(calVertex[alphaquad]);
    energycm2 = 2.0/7.0*(BeEnergy1+AlphaEnergy1);


    //myfile<<AlphaEnergy<<"\t"<<AlphaMomentum<<"\t"<<AngleFit->at(alphaquad)<<"\t"<<AngleFit->at(bequad)<<"\t"<<BeMomentum<<"\t"<<BeEnergy<<"\t"<<energycm<<endl;
    Max_E->Fill(max);
    thetalab = x1;
    if (xquad[0]==0) Vertex_vs_Angle->Fill(calVertex[0],AngleFit->at(0));
    if (xquad[0]==2) Vertex_vs_Angle->Fill(calVertex[2],AngleFit->at(2));
    if (yquad[0]==0) Vertex_vs_Angle2->Fill(calVertex[0],AngleFit->at(0));
    if (yquad[0]==2) Vertex_vs_Angle2->Fill(calVertex[2],AngleFit->at(2));
    Delta_E_cut->Fill(start,tot_E);
    bestindex=-1;
    bestdiff = AlphaEnergy*10000;
    for(Int_t i=0; i<100;i++){
      if(TMath::Abs(AlphaEnergy - InelHeLab[i])<bestdiff){
        bestindex=i-1;
        bestdiff = TMath::Abs(AlphaEnergy - InelHeLab[i]);
        bestrange=InelHeLab[i-1];
      }
      //cout<<bestindex<<"\t"<<bestrange<<endl;
    }

    bestindex1=-1;
    bestdiff1 = BeEnergy*10000;
    for(Int_t i=0; i<100;i++){
      if(TMath::Abs(BeEnergy1 - InelBeLab[i])<bestdiff1){
        bestindex1=i-1;
        bestdiff1 = TMath::Abs(BeEnergy1 - InelBeLab[i]);
        bestrange1=InelBeLab[i-1];
      }
      //cout<<bestindex<<"\t"<<bestrange<<endl;
    }

    Q13_Kine->Fill(InelHeCM[bestindex]+(InelHeCM[bestindex + 1]-InelHeCM[bestindex])*bestdiff,InelBeCM[bestindex1]+(InelBeCM[bestindex1 + 1]-InelBeCM[bestindex1])*bestdiff1);

    thetacm=1.0/2.0*(InelHeCM[bestindex]+InelBeCM[bestindex1]);

    //thetacm=180-(2*thetalab);
    //if (thetacm<0) thetacm= thetacm+180.0;
    Int_t index=int(floor(intVertex[0]));
    Double_t over = intVertex[0]-index;
    if(index<=500&&index>0){
      energycm1=(2.0/7.0)*((EnergyMM[500-index]+(EnergyMM[500-index]-EnergyMM[500-index+1])*over)/1000000.0);
    }
    if(index>-500&&index<0){
      index=TMath::Abs(index);
      energycm1=(2.0/7.0)*(19.5+(EnergyMM[500-index]+(EnergyMM[500-index]-EnergyMM[500-index+1])*over)/1000000.0);
      //myfile<<intVertex[0]<<"\t"<<energycm1<<index<<endl;
    }

    if(TMath::MaxElement(4,maxRad)<=118.0)energycma=2.0/7.0*(AlphaEnergy+BeEnergy1+3.70);
    if(TMath::MaxElement(4,maxRad)>118.0)energycma=energycm2;
    if(TMath::MaxElement(4,maxRad)<=118.0)energycmb=(energycm+energycm2)/2.0;
    if(TMath::MaxElement(4,maxRad)>118.0)energycmb=energycm2;
    if(TMath::MaxElement(4,maxRad)<=118.0)myfile<<nEve<<"\t"<<energycm<<"\t"<<energycm2<<endl;
    Excitation_EL->Fill(thetacm,energycma);
    //myfile<<nEve<<"\t0\t"<<intVertex[0]<<"\tmax\t"<<max<<"\tt0\t"<<realtVertex[0]<<"\tt2\t"<<realtVertex[2]<<endl;
    //if(ThreeD>250&&ThreeD<300&&AlphaRange>20&&AlphaRange<40)myfile<<nEve<<endl;
    if(energycm1>2.9&&energycm1<3.0)Excitation_IN->Fill(AngleFit->at(bequad),BeEnergy1);

    //if(TMath::Abs(ThreeD-AlphaRange)<1.0&&TMath::Abs(ThreeDB-BeRange<1.0)){}
    Excitation_ELa->Fill(thetacm,energycm1);
    if(TMath::MaxElement(4,maxRad)<=118.0)Excitation_INa->Fill(energycm1,2.0/7.0*(AlphaEnergy+BeEnergy1+3.70));
    Vertex->Fill(TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad)));
  }
  thetalab = 0.0;
  thetacm = 0.0;
  energycm=0.0;
  energycma=0.0;
  energycmb=0.0;
  energycm1=0.0;
  energycm2=0.0;
  energycm3=0.0;
  bestrange=0.0;
  bestrange1=0.0;
  if(LowEIn->IsInside(x2,y2)&&FitterPicker[1]==1&&x2>0&&y2>0){
    alphaquad=xquad[1];
    bequad=yquad[1];
    /*if(AngleFit->at(xquad[1])<20.0){
      alphaquad=yquad[1];
      bequad=xquad[1];
    }*/
    //AlphaRange= TMath::Sqrt((maxRad[alphaquad]-intXVertex[alphaquad])*(maxRad[alphaquad]-intXVertex[alphaquad])+(ZmaxRad[alphaquad]-intVertex[alphaquad])*(ZmaxRad[alphaquad]-intVertex[alphaquad]));
    //BeRange = TMath::Sqrt((maxRad[bequad]-intXVertex[bequad])*(maxRad[bequad]-intXVertex[bequad])+(ZmaxRad[bequad]-intVertex[bequad])*(ZmaxRad[bequad]-intVertex[bequad]));
    AlphaRange=(maxRad[alphaquad]-intXVertex[alphaquad])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad)));
    BeRange=(maxRad[bequad]-intXVertex[alphaquad])/(TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad)));//+randGen.Uniform(-1.0,1.0);
    if(TMath::MaxElement(4,maxRad)>118.0){
      Vertex1->Fill(maxRad[bequad]-intXVertex[alphaquad]);
    }
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
      //cout<<bestindex<<"\t"<<bestrange<<endl;
    }
    //cout<<bestindex<<"\t"<<bestrange<<endl;
    AlphaEnergy = AlphaEnTab[bestindex]+(AlphaEnTab[bestindex+1]-AlphaEnTab[bestindex])*(AlphaRange-bestrange)/(AlphaRangeTab[bestindex+1]-bestrange);//+randGen.Gaus(0,0.1);
    BeEnergy1 = BeEnTab[bestindex1]+(BeEnTab[bestindex1+1]-BeEnTab[bestindex1])*(BeRange-bestrange1)/(BeRangeTab[bestindex1+1]-bestrange1);//+randGen.Uniform(0,0.1);
    AlphaMomentum = TMath::Sqrt(double(AlphaEnergy)*2.0*4000.0);
    BeMomentum1 = TMath::Sqrt(double(BeEnergy1)*2.0*10000.0);
    BeMomentum= AlphaMomentum*TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad));
    AlphaMomentum1= BeMomentum1*TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad));
    BeEnergy = BeMomentum*BeMomentum/(2.0*10000.0);
    AlphaEnergy1 = AlphaMomentum1*AlphaMomentum1/(2.0*4000.0);
    energycm = 2.0/7.0*(BeEnergy+AlphaEnergy);
    energycm3 = TMath::Abs(calVertex[alphaquad]);
    energycm2 = 2.0/7.0*(BeEnergy1+AlphaEnergy1);
    //myfile<<AlphaEnergy<<"\t"<<AlphaMomentum<<"\t"<<AngleFit->at(alphaquad)<<"\t"<<AngleFit->at(bequad)<<"\t"<<BeMomentum<<"\t"<<BeEnergy<<"\t"<<energycm<<endl;
    if (xquad[1]==1) Vertex_vs_Angle->Fill(calVertex[1],AngleFit->at(1));
    if (xquad[1]==3) Vertex_vs_Angle->Fill(calVertex[3],AngleFit->at(3));
    if (yquad[1]==1) Vertex_vs_Angle2->Fill(calVertex[1],AngleFit->at(1));
    if (yquad[1]==3) Vertex_vs_Angle2->Fill(calVertex[3],AngleFit->at(3));
    thetalab = x2;
    Delta_E_cut->Fill(start,tot_E);
    bestindex=-1;
    bestdiff = AlphaEnergy*10000;
    for(Int_t i=0; i<100;i++){
      if(TMath::Abs(AlphaEnergy - InelHeLab[i])<bestdiff){
        bestindex=i-1;
        bestdiff = TMath::Abs(AlphaEnergy - InelHeLab[i]);
        bestrange=InelHeLab[i-1];
      }
      //cout<<bestindex<<"\t"<<bestrange<<endl;
    }
    bestindex1=-1;
    bestdiff1 = BeEnergy*10000;
    for(Int_t i=0; i<100;i++){
      if(TMath::Abs(BeEnergy1 - InelBeLab[i])<bestdiff1){
        bestindex1=i-1;
        bestdiff1 = TMath::Abs(BeEnergy1 - InelBeLab[i]);
        bestrange1=InelBeLab[i-1];
      }
      //cout<<bestindex<<"\t"<<bestrange<<endl;
    }

    Q13_Kine->Fill(InelHeCM[bestindex]+(InelHeCM[bestindex + 1]-InelHeCM[bestindex])*bestdiff,InelBeCM[bestindex1]+(InelBeCM[bestindex1 + 1]-InelBeCM[bestindex1])*bestdiff1);

    thetacm=1.0/2.0*(InelHeCM[bestindex]+InelBeCM[bestindex1]);
    //thetacm=180-(2*thetalab);
    if (thetacm<0) thetacm= thetacm+180.0;
    Int_t index=int(floor(intVertex[1]));
    Double_t over = intVertex[1]-index;
    if(index<=500&&index>0){
      energycm1=(2.0/7.0)*((EnergyMM[500-index]+(EnergyMM[500-index]-EnergyMM[500-index+1])*over)/1000000.0);
    }
    if(index>-500&&index<0){
      index=TMath::Abs(index);
      energycm1=(2.0/7.0)*(19.5+(EnergyMM[500-index]+(EnergyMM[500-index]-EnergyMM[500-index+1])*over)/1000000.0);
      //myfile<<intVertex[1]<<"\t"<<energycm1<<endl;
    }

    if(TMath::MaxElement(4,maxRad)<=118.0)energycma=2.0/7.0*(AlphaEnergy+BeEnergy1+3.70);
    if(TMath::MaxElement(4,maxRad)>118.0)energycma=energycm2;
    if(TMath::MaxElement(4,maxRad)<=118.0)energycmb=(energycm+energycm2)/2.0;
    if(TMath::MaxElement(4,maxRad)>118.0)energycmb=energycm2;
    if(TMath::MaxElement(4,maxRad)<=118.0)myfile<<nEve<<"\t"<<energycm<<"\t"<<energycm2<<endl;
    //if(thetalab>50.0&&thetalab<55.0)Excitation_EL->Fill(AlphaRange,BeEnergy);
    //if(TMath::MaxElement(4,maxRad)<=118.0)Excitation_IN->Fill(energycm,energycm2);
    Double_t ThreeD = TMath::Sqrt((maxRad[alphaquad]-intXVertex[alphaquad])*(maxRad[alphaquad]-intXVertex[alphaquad])+(ZmaxRad[alphaquad]-intVertex[alphaquad])*(ZmaxRad[alphaquad]-intVertex[alphaquad]));
    Double_t ThreeDB = TMath::Sqrt((maxRad[bequad]-intXVertex[bequad])*(maxRad[bequad]-intXVertex[bequad])+(ZmaxRad[bequad]-intVertex[bequad])*(ZmaxRad[bequad]-intVertex[bequad]));

    Excitation_EL->Fill(thetacm,energycma);
    //if(ThreeD>250&&ThreeD<300&&AlphaRange>20&&AlphaRange<40)myfile<<nEve<<endl;
    if(energycm1>2.9&&energycm1<3.0)Excitation_IN->Fill(AngleFit->at(bequad),BeEnergy1);
    //myfile<<nEve<<"\t1\t"<<intVertex[1]<<"\tmax\t"<<max<<"\tt1\t"<<realtVertex[1]<<"\tt3\t"<<realtVertex[3]<<endl;
    //if(TMath::Abs(ThreeD-AlphaRange)<1.0&&TMath::Abs(ThreeDB-BeRange<1.0)){}
    Excitation_ELa->Fill(thetacm,energycm1);
    if(TMath::MaxElement(4,maxRad)<=118.0)Excitation_INa->Fill(energycm1,2.0/7.0*(AlphaEnergy+BeEnergy1+3.70));
    Vertex->Fill(TMath::Sin(TMath::DegToRad()*AngleFit->at(bequad))/TMath::Sin(TMath::DegToRad()*AngleFit->at(alphaquad)));
    }

}
//cout<<nEve<<endl;
nEve++;
if(nEve%10000==0)cout<<nEve<<endl;
//if(nEve>100000)break;
}

else{
  nEve++;
  if(nEve%10000==0)cout<<nEve<<endl;

}
//if(nEve%500==0)break;
}
//cout<<totmax<<endl;


c2->cd(1);
Vertex_vs_Angle->Draw("zcol");
c2->cd(2);
Vertex_vs_Angle2->Draw("zcol");

c3->cd(1);
gPad->SetLogz();
Q02_Kine->Draw("colz");
Kine_AngRec_AngSca->Draw("C");
Kine_AngRec_AngSca_vert->Draw("C");
//Kine_AngRec_AngSca_In->Draw("C");
//Kine_AngRec_AngSca_In_vert->Draw("C");
//Kine_AngRec_AngSca_B->Draw("C");
//Kine_AngRec_AngSca_B_vert->Draw("C");
//Kine_AngRec_AngSca_4->Draw("C");
//Kine_AngRec_AngSca_4_vert->Draw("C");
//anglecut->Draw("same");
//wideElasticup->Draw("same");
LowEIn->Draw("same");
//leg->Draw("same");
//  cut2->Draw("same");
//xVertex0->Draw();
c3->cd(2);
gPad->SetLogz();
Q13_Kine->Draw("colz");
Kine_AngRec_AngSca->Draw("C");
Kine_AngRec_AngSca_vert->Draw("C");
Kine_AngRec_AngSca_In->Draw("C");
Kine_AngRec_AngSca_In_vert->Draw("C");
//xVertex1->Draw();

c4->cd(1);
gPad->SetLogz();
//Excitation_EL->Divide(dividecross);
Excitation_EL->Draw("colz");
//BeKine4->Draw("C");
c4->cd(2);
gPad->SetLogz();
Excitation_IN->Draw("colz");
//alphaang->Draw("same");
AlphaKine->Draw("same");
//BeKine3->Draw("C");

c5->cd(1);
Vertex->Draw();
c5->cd(2);
Vertex1->Draw();
c5->cd(3);
//Vertex2->Draw();

Delta_E->Draw("colz");
PID->Draw("same");
//c5->cd(2);
//Delta_E_cut->Draw("colz");
//Max_E->Draw();
//Vertex->Draw();

c6->cd(1);
gPad->SetLogz();
//Excitation_ELa->Divide(dividecross);
Excitation_ELa->Draw("colz");
//disappear->Draw("same");
c6->cd(2);
gPad->SetLogz();
Excitation_INa->Draw("colz");

/*gPad->SetLogz();
numHitsHist->Draw("colz");
c6->cd(2);
gPad->SetLogz();
numHitsHist2->Draw("colz");
*/
c7->cd(1);
Excitation10->Draw();
c7->cd(2);
Excitation20->Draw();
c7->cd(3);
Excitation30->Draw();
c7->cd(4);
Excitation40->Draw();
c7->cd(5);
Excitation50->Draw();
c7->cd(6);
Excitation60->Draw();
c7->cd(7);
Excitation70->Draw();
c7->cd(8);
Excitation80->Draw();
c7->cd(9);
Excitation90->Draw();
c7->cd(10);
Excitation100->Draw();
c7->cd(11);
Excitation110->Draw();
c7->cd(12);
Excitation120->Draw();
c7->cd(13);
Excitation130->Draw();
c7->cd(14);
Excitation140->Draw();
c7->cd(15);
Excitation150->Draw();
c7->cd(16);
Excitation160->Draw();
c7->cd(17);
Excitation170->Draw();
c7->cd(18);
Excitation180->Draw();


c8->cd(1);
Energy100->Draw();
c8->cd(2);
Energy200->Draw();
c8->cd(3);
Energy300->Draw();
c8->cd(4);
Energy400->Draw();
c8->cd(5);
Energy500->Draw();
c8->cd(6);
Energy600->Draw();
c8->cd(7);
Energy700->Draw();
c8->cd(8);
Energy800->Draw();
c8->cd(9);
Energy900->Draw();
c8->cd(10);
Energy1000->Draw();
c8->cd(11);
Energy1100->Draw();
c8->cd(12);
Energy1200->Draw();
c8->cd(13);
Energy1300->Draw();
c8->cd(14);
Energy1400->Draw();
c8->cd(15);
Energy1500->Draw();
c8->cd(16);
Energy1600->Draw();
c8->cd(17);
Energy1700->Draw();
c8->cd(18);
Energy1800->Draw();
c8->cd(19);
Energy1900->Draw();
c8->cd(20);
Energy2000->Draw();

}
