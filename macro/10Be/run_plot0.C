//General Plotting Macro for Feb 2013 10Be + alpha experiment


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

//#include "../../include/ATEvent.hh"
//#include "../../include/ATHit.hh"
//#include "../../include/ATProtoEvent.hh"
//#include "../../include/ATProtoEventAna.hh"
//#include "../../include/ATAnalysis.hh"
//#include "../../include/ATHoughSpaceLine.hh"
//#include "../../include/ATHoughSpaceCircle.hh"

#include <ios>
#include <iostream>
#include <istream>
#include <limits>

int mode1(const std::vector<int>& values)
//returns the mode of the input vector
{
  int old_mode = 0;
  int old_count = 0;
  for(size_t n=0; n < values.size(); ++n)
  {
    int mode = values[n];
    int count = std::count(values.begin()+n+1, values.end(), mode);

    if(count > old_count)
    {
      old_mode = mode;
      old_count = count;
    }
  }
  return old_mode;
}

int mode2(const std::vector<int>& values)
//outputs the number of times the most common entry of the vector occurs
{
  int old_mode = 0;
  int old_count = 0;
  if(values.size()==0){
    return 0;
  }
  for(size_t n=0; n < values.size(); ++n)
  {
    int mode = values[n];
    int count = std::count(values.begin()+n+1, values.end(), mode);

    if(count > old_count)
    {
      old_mode = mode;
      old_count = count;
    }
  }
  return old_count;
}

double avg(std::vector<Double_t> *v) {
  //calcluates the mean of a vector
  return 1.0 * std::accumulate(v->begin(), v->end(), 0LL) / v->size();
}

TGraph2D *line3d(std::vector<Double_t> parFit ){
  //input the paramenters of the fit, returns the TGraph of the fit line
  TGraph2D *graph = new TGraph2D(1000);
  for (int j = 0; j<1000;++j){
    double t = j;
    double x,y,z,r;
    x = parFit[0] + parFit[1]*t;
    y = parFit[2] + parFit[3]*t;
    r = TMath::Sqrt(x*x+y*y);
    z = t;
    //myfile<<x<<"\t"<<y<<endl;
    graph->SetPoint(j,x,y,z);
  }
  return graph;
}

void run_plot0(Int_t runnum=21,Int_t viewEvent = -1)
{

  //filing in theoretical kinematic lines...
  TString FileNameHead = "output_proto";
  TString fileKine="../Kinematics/Decay_kinematics/10Be_4He_19MeV.txt";
  Double_t *ThetaCMS = new Double_t[20000];
  Double_t *ThetaLabRec = new Double_t[20000];
  Double_t *EnerLabRec = new Double_t[20000];
  Double_t *ThetaLabSca = new Double_t[20000];
  Double_t *EnerLabSca = new Double_t[20000];
  Double_t *ThetaCMSIn = new Double_t[1800];
  Double_t *ThetaLabRecIn = new Double_t[1800];
  Double_t *EnerLabRecIn = new Double_t[1800];
  Double_t *ThetaLabScaIn = new Double_t[1800];
  Double_t *EnerLabScaIn = new Double_t[1800];


  std::ifstream *kineStr = new std::ifstream(fileKine.Data());
  Int_t numKin=0;

  if(!kineStr->fail()){
    while(!kineStr->eof()){
      *kineStr>>ThetaCMS[numKin]>>ThetaLabRec[numKin]>>EnerLabRec[numKin]>>ThetaLabSca[numKin]>>EnerLabSca[numKin];
      numKin++;
    }
  }else if(kineStr->fail()) std::cout<<" Warning : No Kinematics file found for this reaction! Please run the macro on $SIMPATH/macro/Kinematics/Decay_kinematics/Mainrel.cxx"<<std::endl;
  TGraph *Kine_AngRec_AngSca = new TGraph(numKin,ThetaLabRec,ThetaLabSca);
  TGraph *Kine_AngRec_AngSca_vert = new TGraph(numKin,ThetaLabSca,ThetaLabRec);


  TString kinfileIn = "KineInEl.txt";
  std::ifstream *kineStrIn = new std::ifstream(kinfileIn.Data());
  numKin=0;

  if(!kineStrIn->fail()){
    while(!kineStrIn->eof()){
      *kineStrIn>>ThetaCMSIn[numKin]>>ThetaLabRecIn[numKin]>>EnerLabRecIn[numKin]>>ThetaLabScaIn[numKin]>>EnerLabScaIn[numKin];
      numKin++;
    }
  }else if(kineStrIn->fail()) std::cout<<" Warning : No Kinematics file found for this reaction! Please run the macro on $SIMPATH/macro/Kinematics/Decay_kinematics/Mainrel.cxx"<<std::endl;
  TGraph *Kine_AngRec_AngSca_In = new TGraph(numKin,ThetaLabRecIn,ThetaLabScaIn);
  TGraph *Kine_AngRec_AngSca_In_vert = new TGraph(numKin,ThetaLabScaIn,ThetaLabRecIn);

  TCanvas *c2 = new TCanvas("c2","c2",200,10,700,700);
  c2->Divide(2,1);
  TCanvas *c3 = new TCanvas("c3","c3",200,10,700,700);
  c3->Divide(2,1);
  TCanvas *c4 = new TCanvas("c4","c4",200,10,700,700);
  c4->Divide(2,1);
  TCanvas *c5 = new TCanvas("c5","c5",200,10,700,700);
  TCanvas *c6 = new TCanvas("c6","c6",1000,500);
  c6->Divide(2,1);

  TH2D* Q02_Kine = new TH2D("Q02_Kine","Q02_Kine",180,0,180,180,0,180);
  Q02_Kine->SetMarkerColor(2);
  Q02_Kine->SetMarkerStyle(20);
  Q02_Kine->SetMarkerSize(0.7);
  TH2D* Q13_Kine = new TH2D("Q13_Kine","Q13_Kine",75,0,15,75,0,15);
  Q13_Kine->SetMarkerColor(2);
  Q13_Kine->SetMarkerStyle(20);
  Q13_Kine->SetMarkerSize(0.7);

  TH1D* Vertex = new TH1D("Vertex","Vertex",80,-5,15);
  TH1D* Vertex1 = new TH1D("Vertex1","Vertex1",150,0,1500);
  TH2D* Vertex_vs_Angle = new TH2D("Vertex_vs_Angle","Vertex_vs_Angle",2000,-1000,1000,600,0,600);

  TH2D* PID = new TH2D("PID","PID",100,0,10000,100,0,500000);

  TH2D* PhiCompare = new TH2D("PhiCompare","PhiCompare",180,0,90,180,0,90);
  TH1D* PhiCompare1 = new TH1D("PhiCompare1","PhiCompare1",500,-500,500);

  TH3D* trackpic = new TH3D("trackpic","trackpic",300,-150,150,300,-150,150,1000,0,1000);
  trackpic->SetMarkerColor(2);
  trackpic->SetMarkerStyle(20);
  trackpic->SetMarkerSize(0.7);

  TH3D* dalitz = new TH3D("Dalitz","Dalitz",90,0,90,90,0,90,90,0,90);
  dalitz->SetMarkerColor(2);
  dalitz->SetMarkerStyle(20);
  dalitz->SetMarkerSize(0.7);

  TH2D* dalitz2d = new TH2D("Dalitz2D","Dalitz2D",200,-1,1,200,-1,1);

  Double_t slope = 1.0/TMath::Sqrt(3);
  TF1 *a = new TF1("a","[0]*x",-1,1);
  TF1 *b = new TF1("b","[0]*x",-1,1);
  a->SetParameter(0,slope);
  b->SetParameter(0,-1*slope);
  TLine *l = new TLine(0,-1,0,1);
  l->SetLineColor(kRed);
  l->SetLineWidth(2);

  TH2D* Excitation_EL = new TH2D("Elastic", "Elastic",60,0,180,75,0,15);
  TH2D* Excitation_IN = new TH2D("Inelastic", "Inelastic",60,0,180,75,0,15);

  TCutG* wideElasticup = new TCutG("wideElasticup",20);
  //elastic cut for "folded" kinematics
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

  TCutG* inelasticcut = new TCutG("inelastic",numKin,ThetaLabRecIn,ThetaLabScaIn);


  TCutG* pidcut = new TCutG("pidcut",5);
  pidcut->SetVarX("dE");
  pidcut->SetVarY("E");
  pidcut->SetTitle("Good PID");
  pidcut->SetPoint(0,97.94,332823.0);
  pidcut->SetPoint(1,784.462,329883.0);
  pidcut->SetPoint(2,1753.34,275657.0);
  pidcut->SetPoint(3,1437.76,240704.0);
  pidcut->SetPoint(4,97.94,294276.0);
  pidcut->SetPoint(5,97.94,332823.0);

  TCutG* cut1 = new TCutG("cut1",4);
  cut1->SetVarX("Evertex");
  cut1->SetVarY("Etracks");
  cut1->SetTitle("small");
  cut1->SetPoint(0,11.21,10.1);
  cut1->SetPoint(1,4.59,4.09);
  cut1->SetPoint(2,5.0,2.6);
  cut1->SetPoint(3,11.39,7.73);
  cut1->SetPoint(4,11.21,10.1);

  TCutG* cut2 = new TCutG("cut2",4);
  cut2->SetVarX("Evertex");
  cut2->SetVarY("Etracks");
  cut2->SetTitle("large");
  cut2->SetPoint(0,12.0,11.7);
  cut2->SetPoint(1,7.8,7.3);
  cut2->SetPoint(2,8.3,6.8);
  cut2->SetPoint(3,12.0,9.9);
  cut2->SetPoint(4,12.0,11.7);

  Double_t corra[2] = {0.0,0.0};
  Double_t corrb[2] = {15.0,15.0};
  TGraph *corr = new TGraph(2,corra,corrb);

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

  Double_t *enlist15 = new Double_t[100];
  Double_t *enlist18 = new Double_t[100];
  Double_t *enlist20 = new Double_t[100];
  Double_t *enlist23 = new Double_t[100];
  Double_t *enlist25 = new Double_t[100];
  Double_t *enlist28 = new Double_t[100];
  Double_t *enlist30 = new Double_t[100];
  Double_t *enlist33 = new Double_t[100];
  Double_t *enlist35 = new Double_t[100];
  Double_t *enlist38 = new Double_t[100];
  Double_t *enlist40 = new Double_t[100];
  Double_t *enlist43 = new Double_t[100];
  Double_t *enlist45 = new Double_t[100];
  Double_t *anglist15 = new Double_t[100];
  Double_t *anglist18 = new Double_t[100];
  Double_t *anglist20 = new Double_t[100];
  Double_t *anglist23 = new Double_t[100];
  Double_t *anglist25 = new Double_t[100];
  Double_t *anglist28 = new Double_t[100];
  Double_t *anglist30 = new Double_t[100];
  Double_t *anglist33 = new Double_t[100];
  Double_t *anglist35 = new Double_t[100];
  Double_t *anglist38 = new Double_t[100];
  Double_t *anglist40 = new Double_t[100];
  Double_t *anglist43 = new Double_t[100];
  Double_t *anglist45 = new Double_t[100];

  std::ifstream *table15 = new std::ifstream("inelE15.txt");
  Int_t num15 = 0;
  while(num15<100){
    *table15>>enlist15[num15]>>anglist15[num15];
    num15++;
  }

  std::ifstream *table18 = new std::ifstream("inelE18.txt");
  Int_t num18 = 0;
  while(num18<100){
    *table18>>enlist18[num18]>>anglist18[num18];
    num18++;
  }

  std::ifstream *table20 = new std::ifstream("inelE20.txt");
  Int_t num20 = 0;
  while(num20<100){
    *table20>>enlist20[num20]>>anglist20[num20];
    num20++;
  }

  std::ifstream *table23 = new std::ifstream("inelE23.txt");
  Int_t num23 = 0;
  while(num23<100){
    *table23>>enlist23[num23]>>anglist23[num23];
    num23++;
  }

  std::ifstream *table25 = new std::ifstream("inelE25.txt");
  Int_t num25 = 0;
  while(num25<100){
    *table25>>enlist25[num25]>>anglist25[num25];
    num25++;
  }

  std::ifstream *table28 = new std::ifstream("inelE28.txt");
  Int_t num28 = 0;
  while(num28<100){
    *table28>>enlist28[num28]>>anglist28[num28];
    num28++;
  }

  std::ifstream *table30 = new std::ifstream("inelE30.txt");
  Int_t num30 = 0;
  while(num30<100){
    *table30>>enlist30[num30]>>anglist30[num30];
    num30++;
  }

  std::ifstream *table33 = new std::ifstream("inelE33.txt");
  Int_t num33 = 0;
  while(num33<100){
    *table33>>enlist33[num33]>>anglist33[num33];
    num33++;
  }

  std::ifstream *table35 = new std::ifstream("inelE35.txt");
  Int_t num35 = 0;
  while(num35<100){
    *table35>>enlist35[num35]>>anglist35[num35];
    num35++;
  }

  std::ifstream *table38 = new std::ifstream("inelE38.txt");
  Int_t num38 = 0;
  while(num38<100){
    *table38>>enlist38[num38]>>anglist38[num38];
    num38++;
  }

  std::ifstream *table40 = new std::ifstream("inelE40.txt");
  Int_t num40 = 0;
  while(num40<100){
    *table40>>enlist40[num40]>>anglist40[num40];
    num40++;
  }

  std::ifstream *table43 = new std::ifstream("inelE43.txt");
  Int_t num43 = 0;
  while(num43<100){
    *table43>>enlist43[num43]>>anglist43[num43];
    num43++;
  }

  std::ifstream *table45 = new std::ifstream("inelE45.txt");
  Int_t num45 = 0;
  while(num45<100){
    *table45>>enlist45[num45]>>anglist45[num45];
    num45++;
  }


  TVector3 zero;
  zero.SetX(0.0);
  zero.SetY(0.0);
  zero.SetZ(0.0);
  Double_t *LengthMM = new Double_t[500];
  Double_t *EnergyMM = new Double_t[500];
  std::ifstream *energyTable = new std::ifstream("energyhigh.dat");
  Int_t numLen=0;
  while(!energyTable->eof()){
    *energyTable>>LengthMM[numLen]>>EnergyMM[numLen];
    numLen++;
  }
  Double_t energycm;
  Double_t thetacm;
  Int_t offset = 460;//"true" detector entrance position.
  Double_t maxchi2=50.0;
  std::vector<TPolyLine3D*> tracksGraph;
  std::vector<Int_t> trackquads;
  std::vector<Int_t> hitquads;

  std::vector<double> RedChi2;
  std::vector<double> maxrad;
  std::vector<double> minrad;
  std::vector<TVector3> maxradvec;
  std::vector<TVector3> extrapoints;
  Int_t bestindex;
  Double_t bestrange;
  Double_t bestdiff;
  Int_t bestindex1;
  Double_t bestrange1;
  Double_t bestdiff1;
  Double_t x;
  Double_t y;
  Double_t anglea;
  Double_t angleb;
  Double_t lengtha;
  Double_t lengthb;
  TVector3 vertexi;
  TVector3 vertexj;
  TVector3 vertexmean;
  std::vector<int> zlist;
  std::vector<int> numspark;
  Double_t totalE = 0;
  Double_t deltaE = 0;
  Double_t maxmesh=0;
  Int_t    maxmeshpos=0;
  Double_t meshthresh=200;
  Bool_t is2B=kFALSE;

  TRandom randGen;
  ofstream myfile;
  myfile.open ("stuff.txt");

  TString workdir = getenv("VMCWORKDIR");
  TChain *chain = new TChain("cbmsim");
  TFileCollection *filecol = new TFileCollection();
  TString FileNameHead_num;
  TString FileNameHead_chain;
  TString FilePath = workdir + "/macro/10Be/";
  TString FileNameTail = ".root";
  TString FileName     = FilePath + FileNameHead + FileNameTail;
  Int_t file_ini=runnum;
  Int_t file_end=runnum;
  Int_t evnt = 0;
  for(Int_t i=file_ini;i<=file_end;i++){
    if(i<10) FileNameHead_num.Form("000%i",i);
    else if(i<100) FileNameHead_num.Form("00%i",i);
    else if(i<1000) FileNameHead_num.Form("0%i",i);
    FileNameHead_chain = "10Be_2013_run"+FileNameHead_num;
    std::cout<<" File : "<<FileNameHead_chain<<" added"<<std::endl;
    TString FileName     = FilePath + FileNameHead_chain + FileNameTail;

    std::cout<<" Opening File : "<<FileName.Data()<<std::endl;
    TFile* file = new TFile(FileName.Data(),"READ");
    //TFile* file = new TFile(FileNameHead_chain.Data(),"READ");
    TTree* tree = (TTree*) file -> Get("cbmsim");
    Int_t nEvents = tree -> GetEntriesFast();
    std::cout<<" Number of events : "<<nEvents<<std::endl;

    TTreeReader Reader1("cbmsim", file);
    TTreeReaderValue<TClonesArray> ransacArray(Reader1, "ATRansac");
    TTreeReaderValue<TClonesArray> eventArray(Reader1, "ATEventH");
    TTreeReaderValue<TClonesArray> protoeventArray(Reader1, "ATProtoEvent");


    while (Reader1.Next()) {


      ATRANSACN::ATRansac* ransac = (ATRANSACN::ATRansac*) ransacArray->At(0);
      ATEvent* event = (ATEvent*) eventArray->At(0);
      ATProtoEvent* protoevent = (ATProtoEvent*) protoeventArray->At(0);
      std::vector<ATTrack> trackVector = ransac->GetTrackCand();
      Float_t *MeshArray              = event->GetMesh();
      Int_t NumHits                   = event->GetNumHits();
      std::vector<ATProtoQuadrant> *quadvec   = protoevent->GetQuadrantArray();
      deltaE=0;
      for(Int_t i=512;i>0;i--){
        if(MeshArray[i]>meshthresh){
          //Vertex->Fill(i);
          deltaE=i;
          break;
        }
      }

      for(Int_t i=512;i>0;i--){
        if(MeshArray[i]>5*meshthresh){
          //Vertex1->Fill(i);
          //deltaE=i;
          break;
        }
      }


      //if(evnt==viewEvent)cout<<trackVector.size()<<endl;
      RedChi2.clear();
      trackquads.clear();
      maxrad.clear();
      minrad.clear();
      maxradvec.clear();
      extrapoints.clear();
      //Vertex->Fill(trackVector.size());
      if(trackVector.size()>0){
        for(Int_t i=0;i<trackVector.size();i++){
          RedChi2.push_back(trackVector.at(i).GetMinimum()/double(trackVector.at(i).GetHitArray()->size())); //calculate Reduced Chi square
          trackVector.at(i).SetAngleZAxis(trackVector.at(i).GetAngleZAxis()-2.5*TMath::DegToRad());
          maxrad.push_back(0.0);
          maxradvec.push_back(zero);
          minrad.push_back(125.0);
          //Vertex->Fill(RedChi2.at(i));
          //Vertex->Fill(trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg());
          //myfile<<trackVector.at(i).GetMinimum()<<"\t"<<trackVector.at(i).GetHitArray()->size()<<"\t"<<RedChi2.at(i)<<endl;
          hitquads.clear();
          for(Int_t j=0;j<trackVector.at(i).GetHitArray()->size();j++){//calculate what quadrant each RANSAC track is in
            ATHit Hit = trackVector.at(i).GetHitArray()->at(j);
            TVector3 coords= Hit.GetPosition();
            if(coords.x()>0){
              if(coords.y()>0) hitquads.push_back(0);
              else hitquads.push_back(3);
            }
            else{
              if(coords.y()>0)hitquads.push_back(1);
              else hitquads.push_back(2);
            }
          }
          trackquads.push_back(mode1(hitquads));
        }
      }
      if(trackVector.size()!=RedChi2.size())cout<<"HALT "<<trackVector.size()<<" "<<RedChi2.size()<<endl;
      std::vector<std::vector<int>> numgood(5); //number of hits that also correspond to a different track
      numspark.clear();
      if(trackVector.size()>0){
        Int_t i=0;
        while(i<trackVector.size()){
          std::vector<Double_t> parFit = trackVector.at(i).GetFitPar();
          std::vector<ATHit>* trackhits = trackVector.at(i).GetHitArray();
          TVector3 vertexcoords = trackVector.at(i).GetTrackVertex();
          Double_t vertexrad = TMath::Sqrt(vertexcoords.x()*vertexcoords.x()+vertexcoords.y()*vertexcoords.y());
          zlist.clear();
          for(Int_t hit1=0; hit1<trackhits->size(); hit1++){
            TVector3 coords1=trackhits->at(hit1).GetPosition();
            Double_t hitrad=TMath::Sqrt(coords1.x()*coords1.x()+coords1.y()*coords1.y());
            zlist.push_back(coords1.z());
            if(hitrad>maxrad.at(i)){
              maxrad[i]=hitrad;
              maxradvec[i]=coords1;
            }
            if(hitrad<minrad.at(i)){
              minrad[i]=hitrad;
            }
          }
          numspark.push_back(mode2(zlist));//calclulate the maximum number of points that are at the same z value
          TGraph2D *dummy3d = line3d(parFit);
          Int_t j=i+1;
          while(j<trackVector.size()){
            std::vector<ATHit>* trackhits2 = trackVector.at(j).GetHitArray();//checking which points also fit another track
            for(Int_t k=0; k<trackhits2->size(); k++){
              TVector3 coords=trackhits2->at(k).GetPosition();
              if(TMath::Sqrt(pow(coords.x()-dummy3d->GetX()[int(floor(coords.z()))],2)+pow(coords.y()-dummy3d->GetY()[int(floor(coords.z()))],2))<3.5){//points that are closer than 3.5 mm in xy distance are considered "close enough"
              numgood.at(j).push_back(k);
            }
          }
          std::sort(numgood.at(j).begin(),numgood.at(j).end()); //remove duplicates from the list of points
          auto last = std::unique(numgood.at(j).begin(),numgood.at(j).end());
          numgood.at(j).erase(last, numgood.at(j).end());
          j++;

        }
        if(trackVector.at(i).GetAngleZAxis()<5.0*TMath::DegToRad()||trackVector.at(i).GetAngleZAxis()>85.0*TMath::DegToRad()){//}||RedChi2.at(i)>maxchi2){
          //removes beam-like tracks
          trackVector.erase(trackVector.begin()+i);
          RedChi2.erase(RedChi2.begin()+i);
          trackquads.erase(trackquads.begin()+i);
          numgood.erase(numgood.begin()+i);
          numspark.erase(numspark.begin()+i);
          maxrad.erase(maxrad.begin()+i);
          maxradvec.erase(maxradvec.begin()+i);
          minrad.erase(minrad.begin()+i);
        }
        else if((trackhits->size()/2)<=numgood.at(i).size()){
          //if more than half the points in a track also fit with a different track, remove it
          for(Int_t j=0;j<numgood.at(i).size();j++){
            //store the removed points in order to make range calculation
            extrapoints.push_back(trackVector.at(i).GetHitArray()->at(numgood.at(i).at(j)).GetPosition());
          }
          trackVector.erase(trackVector.begin()+i);
          RedChi2.erase(RedChi2.begin()+i);
          trackquads.erase(trackquads.begin()+i);
          numgood.erase(numgood.begin()+i);
          numspark.erase(numspark.begin()+i);
          maxrad.erase(maxrad.begin()+i);
          maxradvec.erase(maxradvec.begin()+i);
          minrad.erase(minrad.begin()+i);
        }
        else if(numspark.at(i)>trackhits->size()/2){
          //if most of the track is at the same z-value, it is a spark, remove it
          trackVector.erase(trackVector.begin()+i);
          RedChi2.erase(RedChi2.begin()+i);
          trackquads.erase(trackquads.begin()+i);
          numgood.erase(numgood.begin()+i);
          numspark.erase(numspark.begin()+i);
          maxrad.erase(maxrad.begin()+i);
          maxradvec.erase(maxradvec.begin()+i);
          minrad.erase(minrad.begin()+i);
        }
        else if(minrad.at(i)>10.0){
          //remove tracks with non-physical vertex
          trackVector.erase(trackVector.begin()+i);
          RedChi2.erase(RedChi2.begin()+i);
          trackquads.erase(trackquads.begin()+i);
          numgood.erase(numgood.begin()+i);
          numspark.erase(numspark.begin()+i);
          maxrad.erase(maxrad.begin()+i);
          maxradvec.erase(maxradvec.begin()+i);
          minrad.erase(minrad.begin()+i);
        }
        else{
          i++;
        }
        //i++;
      }

    }

    for(Int_t t=0; t<trackVector.size();t++){
      std::vector<Double_t> parFit = trackVector.at(t).GetFitPar();
      TGraph2D *dummy3d = line3d(parFit);
      for(Int_t p=0; p<extrapoints.size();p++){
        if(TMath::Sqrt(pow(extrapoints.at(p).x()-dummy3d->GetX()[int(floor(extrapoints.at(p).z()))],2)+pow(extrapoints.at(p).y()-dummy3d->GetY()[int(floor(extrapoints.at(p).z()))],2))<3.5){
          if(TMath::Sqrt(extrapoints.at(p).x()*extrapoints.at(p).x()+extrapoints.at(p).y()*extrapoints.at(p).y())>maxrad[t]){
            maxrad[t] = TMath::Sqrt(extrapoints.at(p).x()*extrapoints.at(p).x()+extrapoints.at(p).y()*extrapoints.at(p).y());
            maxradvec[t]=extrapoints.at(p);
          }
        }
      }
    }

    // if(evnt==viewEvent){
    //   //view points and 3-d lines for the specified event
    //   for(Int_t i=0; i<NumHits; i++){
    //     ATHit *Hit = event->GetHit(i);
    //     TVector3 coords= Hit->GetPosition();
    //     //trackpic->Fill(coords.y(),coords.z());
    //     trackpic->Fill(coords.x(),coords.y(),coords.z());
    //     //cout<<coords.x()<<"\t"<<coords.y()<<"\t"<<coords.z()<<endl;
    //   }
    //
    //   //uncomment break statement below if the only point is viewing
    //   //break;
    // }
    is2B=kFALSE;
    std::vector<Double_t> *Phi0Array;
    std::vector<Double_t> *Phi0RArray;
    std::vector<Double_t> *Phi2Array;
    std::vector<Double_t> *Phi2RArray;
    if(trackVector.size()>1){//})&&deltaE>370&&deltaE<400){
      for(Int_t i=0;i<trackVector.size();i++){
        for(Int_t j=i+1;j<trackVector.size();j++){
          vertexi = trackVector.at(i).GetTrackVertex();
          vertexj = trackVector.at(j).GetTrackVertex();
          vertexmean = 1.0/2.0*(vertexi+vertexj);
          Double_t vertexmeanrad = TMath::Sqrt(vertexmean.x()*vertexmean.x()+vertexmean.y()*vertexmean.y());
          if(trackVector.at(i).GetAngleZAxis()>trackVector.at(j).GetAngleZAxis()){
            //assigning alpha and beryllium angles
            anglea=trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg();
            angleb=trackVector.at(j).GetAngleZAxis()*TMath::RadToDeg();
            //calculating alpha and beryllium track lengths
            lengtha=(maxradvec[i]-vertexmean).Mag();
            lengthb=(maxradvec[j]-vertexmean).Mag();
            //lengtha=(maxrad[i]-TMath::Sqrt(vertexmean.x()*vertexmean.x()+vertexmean.y()*vertexmean.y()))/TMath::Sin(trackVector.at(i).GetAngleZAxis());
            //lengthb=(maxrad[j]-TMath::Sqrt(vertexmean.x()*vertexmean.x()+vertexmean.y()*vertexmean.y()))/TMath::Sin(trackVector.at(j).GetAngleZAxis());
          }
          else{
            angleb=trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg();
            anglea=trackVector.at(j).GetAngleZAxis()*TMath::RadToDeg();
            //lengthb=(maxrad[i]-TMath::Sqrt(vertexmean.x()*vertexmean.x()+vertexmean.y()*vertexmean.y()))/TMath::Sin(trackVector.at(i).GetAngleZAxis());
            //lengtha=(maxrad[j]-TMath::Sqrt(vertexmean.x()*vertexmean.x()+vertexmean.y()*vertexmean.y()))/TMath::Sin(trackVector.at(j).GetAngleZAxis());
            lengthb=(maxradvec[i]-vertexmean).Mag();
            lengtha=(maxradvec[j]-vertexmean).Mag();
          }
          //Q02_Kine->Fill(x,y);
          if((trackquads.at(i)+2)%4==trackquads.at(j)){
            //array of the phi and the phi radius for the two relevant quadrants
            Phi0Array =quadvec->at(trackquads.at(i)).GetPhiArray();
            Phi0RArray =quadvec->at(trackquads.at(i)).GetPhiRArray();
            Phi2Array =quadvec->at(trackquads.at(j)).GetPhiArray();
            Phi2RArray =quadvec->at(trackquads.at(j)).GetPhiRArray();
            if(trackquads.at(i)<trackquads.at(j)){
              //assign x and y coordinates of kinematic plot based on quadrants
              x=trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg();
              y=trackVector.at(j).GetAngleZAxis()*TMath::RadToDeg();
            }
            else{
              y=trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg();
              x=trackVector.at(j).GetAngleZAxis()*TMath::RadToDeg();
            }
            if(Phi0Array->size()>5){
              //remove last 4 entries of phi
              for(Int_t del=0;del<4;del++){
                Phi0Array->pop_back();
                Phi0RArray->pop_back();
              }
            }
            if(Phi2Array->size()>5){
              for(Int_t del=0;del<4;del++){
                Phi2Array->pop_back();
                Phi2RArray->pop_back();
              }
            }
            Int_t erase0=0;
            while(erase0<Phi0RArray->size()){
              //remove phi counts that happen at radius<10mm
              if(Phi0RArray->at(erase0)<10.0){
                Phi0Array->erase(Phi0Array->begin()+erase0);
                Phi0RArray->erase(Phi0RArray->begin()+erase0);
              }
              else{
                erase0++;
              }
            }
            Int_t erase2=0;
            while(erase2<Phi2RArray->size()){
              if(Phi2RArray->at(erase2)<10.0){
                Phi2Array->erase(Phi2Array->begin()+erase2);
                Phi2RArray->erase(Phi2RArray->begin()+erase2);
              }
              else{
                erase2++;
              }
            }
            //calculating the alpha energy from the range table:
            bestindex=-1;
            bestdiff = lengtha*10000;
            for(Int_t i=0; i<96;i++){
              if(TMath::Abs(lengtha - AlphaRangeTab[i])<bestdiff){
                bestindex=i-1;
                bestdiff = TMath::Abs(lengtha - AlphaRangeTab[i]);
                bestrange=AlphaRangeTab[i-1];
              }
              //cout<<bestindex<<"\t"<<bestrange<<endl;
            }
            //calculating the be energy from the range table:
            bestindex1=-1;
            bestdiff1 = lengthb*10000;
            for(Int_t i=0; i<96;i++){
              if(TMath::Abs(lengthb - BeRangeTab[i])<bestdiff1){
                bestindex1=i-1;
                bestdiff1 = TMath::Abs(lengthb - BeRangeTab[i]);
                bestrange1=BeRangeTab[i-1];

              }
              //cout<<BeRangeTab[i]<<endl;
            }
            Double_t enb=BeEnTab[bestindex1]+(BeEnTab[bestindex1+1]-BeEnTab[bestindex1])*(lengthb-bestrange1)/(BeRangeTab[bestindex1+1]-bestrange1); //energy beryllium
            Double_t momb=TMath::Sqrt(enb*2.0*10000.0); //momentum beryllium
            Double_t moma=momb*TMath::Sin(angleb*TMath::DegToRad())/TMath::Sin(anglea*TMath::DegToRad());//momentum alpha
            Double_t ena = moma*moma/(2.0*4000.0);//energy alpha
            //ena=AlphaEnTab[bestindex]+(AlphaEnTab[bestindex+1]-AlphaEnTab[bestindex])*(lengtha-bestrange)/(AlphaRangeTab[bestindex+1]-bestrange); //option for calcluating alpha energy directly

            Int_t index=int(floor(vertexmean.z()));
            Double_t over = (vertexmean.z())-index;
            index = offset-index;
            if(index<=500&&index>0){
              energycm=2.0/7.0*(EnergyMM[index]+(EnergyMM[index]-EnergyMM[index+1])*over);
            }
            if(index>-500&&index<0){
              index=500+index;
              energycm=2.0/7.0*(38.0+(EnergyMM[index]+(EnergyMM[index]-EnergyMM[index+1])*over));
              //myfile<<intVertex[0]<<"\t"<<energycm1<<index<<endl;
            }
            Q02_Kine->Fill(x,y);
            if(TMath::Abs(avg(Phi0Array)-avg(Phi2Array))<5.0){
              is2B=kTRUE;//marking other two=body channels
            }
            Q13_Kine->Fill(energycm,2.0/7.0*(ena+enb));
            Vertex->Fill(energycm-2.0/7.0*(ena+enb));

            if(TMath::Abs(wideElasticup->IsInside(anglea,angleb))){
              is2B=kTRUE;
              PID->Fill(deltaE,totalE);
              thetacm=180-(2*anglea);
              if (thetacm<0) thetacm= thetacm+180.0;
              //calculating the energy from the center of mass position
              //cout<<evnt<<"\t"<<thetacm<<"\t"<<energycm<<endl;
              if(1==1){//TMath::Abs(avg(Phi0Array)-avg(Phi2Array))<15.0){ //not currently cutting on phi, resolution not good enough
                //Excitation_EL->Fill(energycm, 2.0/7.0*(ena+enb)); //resolution plot
                Excitation_EL->Fill(thetacm,energycm); //excitation function, elastic
                //    if(cut1->IsInside(energycm, 2.0/7.0*(ena+enb))){
                //    Q02_Kine->Fill(x,y);
                //   }
                //   if(cut2->IsInside(energycm, 2.0/7.0*(ena+enb))){
                //   Q13_Kine->Fill(x,y);
                // }

              }
            }
            if(inelasticcut->IsInside(anglea,angleb)&&cut1->IsInside(energycm,2.0/7.0*(ena+enb))){
              is2B=kTRUE;
              //Vertex1->Fill(enb);

              Double_t energylab = 7.0/2.0*energycm;
              Double_t thetalow=0.0;
              Double_t thetahigh=0.0;
              if(energylab>=43.0&&energylab<45.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist43[i]){
                    thetalow=anglist43[i]+(anglist43[i+1]-anglist43[i])*(enb-enlist43[i])/(enlist43[i+1]-enlist43[i]);
                    break;
                  }
                }
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist45[i]){
                    thetahigh=anglist45[i]+(anglist45[i+1]-anglist45[i])*(enb-enlist45[i])/(enlist45[i+1]-enlist45[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-43.0)/(45.0-43.0);
              }
              if(energylab>=40.0&&energylab<43.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist40[i]){
                    thetalow=anglist40[i]+(anglist40[i+1]-anglist40[i])*(enb-enlist40[i])/(enlist40[i+1]-enlist40[i]);
                    break;
                  }
                }
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist43[i]){
                    thetahigh=anglist43[i]+(anglist43[i+1]-anglist43[i])*(enb-enlist43[i])/(enlist43[i+1]-enlist43[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-40.0)/(43.0-40.0);
              }
              else if(energylab>=38.0&&energylab<40.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist38[i]){
                    thetalow=anglist38[i]+(anglist38[i+1]-anglist38[i])*(enb-enlist38[i])/(enlist38[i+1]-enlist38[i]);
                    break;
                  }
                }
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist40[i]){
                    thetahigh=anglist40[i]+(anglist40[i+1]-anglist40[i])*(enb-enlist40[i])/(enlist40[i+1]-enlist40[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-38.0)/(40.0-38.0);
              }
              else if(energylab>=35.0&&energylab<38.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist35[i]){
                    thetalow=anglist35[i]+(anglist35[i+1]-anglist35[i])*(enb-enlist35[i])/(enlist35[i+1]-enlist35[i]);
                    break;
                  }
                }
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist38[i]){
                    thetahigh=anglist38[i]+(anglist38[i+1]-anglist38[i])*(enb-enlist38[i])/(enlist38[i+1]-enlist38[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-35.0)/(38.0-35.0);
              }
              else if(energylab>=33.0&&energylab<35.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist33[i]){
                    thetalow=anglist33[i]+(anglist33[i+1]-anglist33[i])*(enb-enlist33[i])/(enlist33[i+1]-enlist33[i]);
                    break;
                  }
                }
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist35[i]){
                    thetahigh=anglist35[i]+(anglist35[i+1]-anglist35[i])*(enb-enlist35[i])/(enlist35[i+1]-enlist35[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-33.0)/(35.0-33.0);
              }
              else if(energylab>=30.0&&energylab<33.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist30[i]){
                    thetalow=anglist30[i]+(anglist30[i+1]-anglist30[i])*(enb-enlist30[i])/(enlist30[i+1]-enlist30[i]);
                    break;
                  }
                }
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist33[i]){
                    thetahigh=anglist33[i]+(anglist33[i+1]-anglist33[i])*(enb-enlist33[i])/(enlist33[i+1]-enlist33[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-30.0)/(33.0-30.0);
              }
              else if(energylab>=28.0&&energylab<30.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist28[i]){
                    thetalow=anglist28[i]+(anglist28[i+1]-anglist28[i])*(enb-enlist28[i])/(enlist28[i+1]-enlist28[i]);
                    break;
                  }
                }
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist30[i]){
                    thetahigh=anglist30[i]+(anglist30[i+1]-anglist30[i])*(enb-enlist30[i])/(enlist30[i+1]-enlist30[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-28.0)/(30.0-28.0);
              }
              else if(energylab>=25.0&&energylab<28.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist25[i]){
                    thetalow=anglist25[i]+(anglist25[i+1]-anglist25[i])*(enb-enlist25[i])/(enlist25[i+1]-enlist25[i]);
                    break;
                  }
                }
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist28[i]){
                    thetahigh=anglist28[i]+(anglist28[i+1]-anglist28[i])*(enb-enlist28[i])/(enlist28[i+1]-enlist28[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-25.0)/(28.0-25.0);
              }
              else if(energylab>=23.0&&energylab<25.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist23[i]){
                    thetalow=anglist23[i]+(anglist23[i+1]-anglist23[i])*(enb-enlist23[i])/(enlist23[i+1]-enlist23[i]);
                    break;
                  }
                }
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist25[i]){
                    thetahigh=anglist25[i]+(anglist25[i+1]-anglist25[i])*(enb-enlist25[i])/(enlist25[i+1]-enlist25[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-23.0)/(25.0-23.0);
              }
              else if(energylab>=20.0&&energylab<23.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist20[i]){
                    thetalow=anglist20[i]+(anglist20[i+1]-anglist20[i])*(enb-enlist20[i])/(enlist20[i+1]-enlist20[i]);
                    break;
                  }
                }
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist23[i]){
                    thetahigh=anglist23[i]+(anglist23[i+1]-anglist23[i])*(enb-enlist23[i])/(enlist23[i+1]-enlist23[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-20.0)/(23.0-20.0);
              }
              else if(energylab>=18.0&&energylab<20.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist18[i]){
                    thetalow=anglist18[i]+(anglist18[i+1]-anglist18[i])*(enb-enlist18[i])/(enlist18[i+1]-enlist18[i]);
                    break;
                  }
                }
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist20[i]){
                    thetahigh=anglist20[i]+(anglist20[i+1]-anglist20[i])*(enb-enlist20[i])/(enlist20[i+1]-enlist20[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-18.0)/(20.0-18.0);
              }
              else if(energylab>=15.0&&energylab<18.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist15[i]){
                    thetalow=anglist15[i]+(anglist15[i+1]-anglist15[i])*(enb-enlist15[i])/(enlist15[i+1]-enlist15[i]);
                    break;
                  }
                }
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist18[i]){
                    thetahigh=anglist18[i]+(anglist18[i+1]-anglist18[i])*(enb-enlist18[i])/(enlist18[i+1]-enlist18[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-15.0)/(18.0-15.0);
              }
              else if(energylab>=0.0&&energylab<15.0){
                for(Int_t i=0;i<100;i++){
                  if(enb>=enlist15[i]){
                    thetahigh=anglist15[i]+(anglist15[i+1]-anglist15[i])*(enb-enlist15[i])/(enlist15[i+1]-enlist15[i]);
                    break;
                  }
                }
                thetacm = thetalow + (thetahigh-thetalow)*(energylab-0.0)/(15.0-0.0);
              }
              thetacm=180-thetacm;
              if(thetacm>0.0){
                //Excitation_EL->Fill(energycm, 2.0/7.0*(ena+enb));
                Excitation_IN->Fill(thetacm,energycm);
                //Q13_Kine->Fill(anglea,thetacm);
              }
            }
          }
        }
      }
      Bool_t weirdreco =kFALSE;
      if(trackVector.size()>2){
        Int_t i =0;
        while(i<trackVector.size()){
          Int_t j=i+1;
          while(j<trackVector.size()){
            if(TMath::Abs(trackVector.at(i).GetAngleZAxis()-trackVector.at(j).GetAngleZAxis())<0.5*TMath::DegToRad()){
              trackVector.erase(trackVector.begin()+j);
              maxradvec.erase(maxradvec.begin()+j);
              maxrad.erase(maxrad.begin()+j);
              weirdreco=kTRUE;
            }
            else{
              j++;
            }
          }
          i++;
        }
          // if(maxrad.at(i)>110.0){
          //   trackVector.erase(trackVector.begin()+i);
          //   maxradvec.erase(maxradvec.begin()+i);
          //   maxrad.erase(maxrad.begin()+i);
          //}
          // if(maxrad.at(i)<10.0){
          //   trackVector.erase(trackVector.begin()+i);
          //   maxradvec.erase(maxradvec.begin()+i);
          //   maxrad.erase(maxrad.begin()+i);
          // }
          // else{
          //   i++;
          // }
          }
        if(trackVector.size()==3&&maxrad[0]<120.&&maxrad[1]<120.&&maxrad[2]<120.){
          Int_t first = randGen.Integer(3);
          Double_t range0 = (maxradvec[first]-trackVector.at(first).GetTrackVertex()).Mag();
          Double_t range1 = (maxradvec[(first+1)%3]-trackVector.at((first+1)%3).GetTrackVertex()).Mag();
          Double_t range2 = (maxradvec[(first+2)%3]-trackVector.at((first+2)%3).GetTrackVertex()).Mag();
          // range0 = TMath::Sqrt(range0);
          // range1 = TMath::Sqrt(range1);
          // range2 = TMath::Sqrt(range2);
          Double_t sumrange= range0+range1+range2;
          Double_t fracrange0 = range0/sumrange;
          Double_t fracrange1 = range1/sumrange;
          Double_t fracrange2 = range2/sumrange;
          Double_t dalitzx = TMath::Sqrt(3.0)/2.0*(fracrange1-fracrange0);
  				Double_t dalitzy = 1.0/2.0*(2*fracrange2-fracrange0-fracrange1);
  				Double_t dalitzx1 = TMath::Sqrt(3.0)/2.0*(fracrange2-fracrange1);
  				Double_t dalitzy1 = 1.0/2.0*(2*fracrange0-fracrange1-fracrange2);
  				Double_t dalitzx2 = TMath::Sqrt(3.0)/2.0*(fracrange0-fracrange2);
  				Double_t dalitzy2 = 1.0/2.0*(2*fracrange1-fracrange2-fracrange0);
  				Double_t dalitzx0 = TMath::Sqrt(3.0)/2.0*(fracrange0-fracrange1);
  				Double_t dalitzy0 = 1.0/2.0*(2*fracrange2-fracrange1-fracrange0);
  				Double_t dalitzx01 = TMath::Sqrt(3.0)/2.0*(fracrange1-fracrange2);
  				Double_t dalitzy01 = 1.0/2.0*(2*fracrange0-fracrange2-fracrange1);
  				Double_t dalitzx02 = TMath::Sqrt(3.0)/2.0*(fracrange2-fracrange0);
  				Double_t dalitzy02 = 1.0/2.0*(2*fracrange1-fracrange0-fracrange2);
          //dalitz->Fill(fracrange0,fracrange1,fracrange2);


          Double_t flipx;
          Double_t flipy;
          //cout<<sumrange<<endl;
          if(sumrange>100&&sumrange<255){
            dalitz->Fill(trackVector.at(first).GetAngleZAxis()*TMath::RadToDeg(),trackVector.at((first+1)%3).GetAngleZAxis()*TMath::RadToDeg(),trackVector.at((first+2)%3).GetAngleZAxis()*TMath::RadToDeg());
            // if(a->Eval(dalitzx)<dalitzy){
            //   flipx = 2*(dalitzx+dalitzy*slope)/(1+slope*slope)-dalitzx;
            //   flipy = 2*slope*(dalitzx+dalitzy*slope)/(1+slope*slope)-dalitzy;
            //   dalitzx = flipx;
            //   dalitzy = flipy;
            // }
            // if(b->Eval(dalitzx)<dalitzy){
            //   flipx = 2*(dalitzx - dalitzy*slope)/(1+slope*slope)-dalitzx;
            //   flipy = -2*slope*(dalitzx-dalitzy*slope)/(1+slope*slope)-dalitzy;
            //   dalitzx = flipx;
            //   dalitzy = flipy;
            // }
            // dalitzx = TMath::Abs(dalitzx);
            dalitz2d->Fill(dalitzx,dalitzy);
            dalitz2d->Fill(dalitzx1,dalitzy1);
            dalitz2d->Fill(dalitzx2,dalitzy2);
            dalitz2d->Fill(dalitzx0,dalitzy0);
            dalitz2d->Fill(dalitzx01,dalitzy01);
            dalitz2d->Fill(dalitzx02,dalitzy02);
            Vertex1->Fill(trackVector.at(1).GetTrackVertex().Z());
            // if(TMath::Abs(dalitzx-TMath::Abs(dalitzy))<0.1){
            //   cout<<evnt<<endl;
            // }
            // if(evnt==viewEvent){
            //   for(Int_t i=0;i<trackVector.size();i++){
            //     std::vector<Double_t> parFit = trackVector.at(i).GetFitPar();
            //     std::vector<ATHit>* trackhits = trackVector.at(i).GetHitArray();
            //     TVector3 vertexcoords = trackVector.at(i).GetTrackVertex();
            //     //cout<<"angle "<<trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg()<<endl;
            //     Double_t vertexrad = TMath::Sqrt(vertexcoords.x()*vertexcoords.x()+vertexcoords.y()*vertexcoords.y());
            //     //cout<<vertexrad<<"\t"<<vertexcoords.z()<<endl;
            //     //cout<<"quadrant "<<trackquads.at(i)<<endl;
            //     //cout<<"number of hits \t"<<trackhits->size()<<endl;
            //     // for(Int_t j=0; j<trackhits->size(); j++){
            //     // TVector3 coords=trackhits->at(j).GetPosition();
            //     // trackpic->Fill(coords.x(),coords.y(),coords.z());
            //     // cout<<coords.x()<<"\t"<<coords.y()<<"\t"<<coords.z()<<endl;
            //     // }
            //     cout<<RedChi2.at(i)<<"\t"<<trackhits->size()<<endl;
            //     cout<<trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg()<<endl;
            //     TPolyLine3D *dummy = new TPolyLine3D(2);
            //     TPolyLine *dummy2d = new TPolyLine(2);
            //     for (int j = 210; j<1210;++j){
            //       double t = j-210;
            //       double x,y,z,r;
            //       x = parFit[0] + parFit[1]*t;
            //       y = parFit[2] + parFit[3]*t;
            //       r = TMath::Sqrt(x*x+y*y);
            //       z = t;
            //       //myfile<<x<<"\t"<<y<<endl;
            //       dummy->SetPoint(j-210,x,y,z);
            //       dummy2d->SetPoint(j-210,y,z);
            //     }
            //     dummy->SetLineColor(kRed);
            //     dummy2d->SetLineColor(kRed);
            //
            //     tracksGraph.push_back(dummy);
            //   }
            //
            // }

          }
        }


    }





    numgood.clear();
    numspark.clear();
    if(evnt%10000==0)std::cout<<" Event : "<<evnt<<std::endl;
    evnt++;
  }
  file->Close();
}


c2->cd(1);
//gPad->SetLogz();
//PID->Draw("colz");
//pidcut->Draw("same");
Vertex->Draw();
c2->cd(2);
//Vertex_vs_Angle->Draw("colz");
Vertex1->Draw();

leg = new TLegend(0.1,0.1,0.2,0.2);
leg->AddEntry(Kine_AngRec_AngSca, "elastic","l");
leg->AddEntry(Kine_AngRec_AngSca_In, "inelastic","l");
c3->cd(1);
gPad->SetLogz();
Q02_Kine->Draw("colz");
Kine_AngRec_AngSca->Draw("C");
Kine_AngRec_AngSca_vert->Draw("C");
Kine_AngRec_AngSca_In->Draw("C");
Kine_AngRec_AngSca_In_vert->Draw("C");
wideElasticup->Draw("same");
//inelasticcut->Draw("same");
leg->Draw();
c3->cd(2);
gPad->SetLogz();
Q13_Kine->Draw("colz");
cut1->Draw("same");


c4->cd(1);
dalitz->Draw("same");

c4->cd(2);
dalitz2d->Draw("colz");
a->Draw("same");
b->Draw("same");
l->Draw("same");

c5->cd();
//PhiCompare->Draw("*");
// trackpic->Draw();
// for(Int_t i =0;i<tracksGraph.size();i++){
//   tracksGraph.at(i)->Draw("same");
// }
//c5->cd(2);
//PhiCompare1->Draw();

c6->cd(1);
Excitation_EL->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
Excitation_EL->Draw("colz");
//corr->Draw("C");
//cut1->Draw("same");
//cut2->Draw("same");
c6->cd(2);
Excitation_IN->SetBinErrorOption(TH1::EBinErrorOpt::kPoisson);
Excitation_IN->Draw("colz");

TString plotsfile;
ostringstream convert;
convert<<runnum;
TString suffix = convert.str();
plotsfile = "plots_"+suffix+".root";

TFile *MyFile = new TFile(plotsfile,"recreate");
c2->Write();
c3->Write();
c4->Write();
c5->Write();
c6->Write();
}
