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

void run_plot0(Int_t runnum=21)
{
  TString FileNameHead = "output_proto";
  TString fileKine="../Kinematics/Decay_kinematics/10Be_4He_19MeV.txt";
  TCanvas *c2 = new TCanvas("c2","c2",200,10,700,700);
  c2->Divide(2,1);
  TCanvas *c3 = new TCanvas("c3","c3",200,10,700,700);
  c3->Divide(2,1);
  TCanvas *c4 = new TCanvas("c4","c4",200,10,700,700);
  TCanvas *c5 = new TCanvas("c5","c5",200,10,700,700);
  c5->Divide(2,1);
  TCanvas *c6 = new TCanvas("c6","c6",200,10,700,700);
  c6->Divide(2,1);

  TH2D* Q02_Kine = new TH2D("Q02_Kine","Q02_Kine",180,0,180,180,0,180);
  Q02_Kine->SetMarkerColor(2);
  Q02_Kine->SetMarkerStyle(20);
  Q02_Kine->SetMarkerSize(0.7);
  TH2D* Q13_Kine = new TH2D("Q13_Kine","Q13_Kine",180,0,180,180,0,180);
  Q13_Kine->SetMarkerColor(2);
  Q13_Kine->SetMarkerStyle(20);
  Q13_Kine->SetMarkerSize(0.7);

  TH1D* Vertex = new TH1D("Vertex","Vertex",1000,0,500000);
  TH1D* Vertex1 = new TH1D("Vertex1","Vertex1",10,0,10);
  TH2D* Vertex_vs_Angle = new TH2D("Vertex_vs_Angle","Vertex_vs_Angle",2000,-1000,1000,600,0,600);

  TH2D* PID = new TH2D("PID","PID",100,0,10000,100,0,500000);

  TH2D* PhiCompare = new TH2D("PhiCompare","PhiCompare",180,0,90,180,0,90);
  TH1D* PhiCompare1 = new TH1D("PhiCompare1","PhiCompare1",500,-500,500);

  TH3D* trackpic = new TH3D("trackpic","trackpic",300,-150,150,300,-150,150,500,0,500);
  trackpic->SetMarkerColor(2);
  trackpic->SetMarkerStyle(20);
  trackpic->SetMarkerSize(0.7);

  TH3D* dalitz = new TH3D("Dalitz","Dalitz",90,0,90,90,0,90,90,0,90);
  dalitz->SetMarkerColor(2);
  dalitz->SetMarkerStyle(20);
  dalitz->SetMarkerSize(0.7);

  TH2D* Excitation_EL = new TH2D("Elastic", "Elastic",150,0,15,150,0,15);
  TH2D* Excitation_IN = new TH2D("Inelastic", "Inelastic",36,0,180,120,0,12);

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
  cut1->SetPoint(0,11.07,12.28);
  cut1->SetPoint(1,8.20,9.62);
  cut1->SetPoint(2,8.63,8.96);
  cut1->SetPoint(3,11.15,10.76);
  cut1->SetPoint(4,11.07,12.28);

  TCutG* cut2 = new TCutG("cut2",4);
  cut2->SetVarX("Evertex");
  cut2->SetVarY("Etracks");
  cut2->SetTitle("large");
  cut2->SetPoint(0,10.98,13.18);
  cut2->SetPoint(1,3.19,7.05);
  cut2->SetPoint(2,3.26,6.44);
  cut2->SetPoint(3,11.10,12.21);
  cut2->SetPoint(4,10.98,13.18);


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

  Int_t viewEvent = 4309; //choose which event to view

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
  Int_t offset = 500;//"true" detector entrance position. currently 500 because everything recalibrated already
  Double_t maxchi2=50.0;
  std::vector<TPolyLine3D*> tracksGraph;
  std::vector<Int_t> trackquads;
  std::vector<Int_t> hitquads;

  std::vector<double> RedChi2;
  std::vector<double> maxrad;
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
      std::vector<ATRANSACN::ATRansac::PairedLines> plines = ransac->GetPairedLinesArray();
      Float_t *MeshArray              = event->GetMesh();
      Int_t NumHits                   = event->GetNumHits();
      std::vector<ATProtoQuadrant> *quadvec   = protoevent->GetQuadrantArray();

      if(evnt==viewEvent)cout<<trackVector.size()<<endl;
      RedChi2.clear();
      trackquads.clear();
      maxrad.clear();
      maxradvec.clear();
      extrapoints.clear();
      if(trackVector.size()>0){
        for(Int_t i=0;i<trackVector.size();i++){
          RedChi2.push_back(trackVector.at(i).GetMinimum()/double(trackVector.at(i).GetHitArray()->size())); //calculate Reduced Chi square
          maxrad.push_back(0.0);
          maxradvec.push_back(zero);
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
        if(trackVector.at(i).GetAngleZAxis()<0.08){//}||RedChi2.at(i)>maxchi2){
          //removes beam-like tracks
          trackVector.erase(trackVector.begin()+i);
          RedChi2.erase(RedChi2.begin()+i);
          trackquads.erase(trackquads.begin()+i);
          numgood.erase(numgood.begin()+i);
          numspark.erase(numspark.begin()+i);
          maxrad.erase(maxrad.begin()+i);
          maxradvec.erase(maxradvec.begin()+i);
        }
        else if((trackhits->size()/2)<=numgood.at(i).size()){
          //if(evnt==viewEvent)cout<<trackhits->size()<<"\t"<<numgood.at(i).size()<<endl;
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
        }
        else if(vertexrad>125.0){//}||vertexcoords.z()>500.0||vertexcoords.z()<0){
          //remove tracks with non-physical vertex radii
          trackVector.erase(trackVector.begin()+i);
          RedChi2.erase(RedChi2.begin()+i);
          trackquads.erase(trackquads.begin()+i);
          numgood.erase(numgood.begin()+i);
          numspark.erase(numspark.begin()+i);
          maxrad.erase(maxrad.begin()+i);
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

    if(evnt==viewEvent){
      //view points and 3-d lines for the specified event
      for(Int_t i=0; i<NumHits; i++){
        ATHit *Hit = event->GetHit(i);
        TVector3 coords= Hit->GetPosition();
        //trackpic->Fill(coords.y(),coords.z());
        trackpic->Fill(coords.x(),coords.y(),coords.z());
        //cout<<coords.x()<<"\t"<<coords.y()<<"\t"<<coords.z()<<endl;
      }
      for(Int_t i=0;i<trackVector.size();i++){
        std::vector<Double_t> parFit = trackVector.at(i).GetFitPar();
        std::vector<ATHit>* trackhits = trackVector.at(i).GetHitArray();
        TVector3 vertexcoords = trackVector.at(i).GetTrackVertex();
        //cout<<"angle "<<trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg()<<endl;
        Double_t vertexrad = TMath::Sqrt(vertexcoords.x()*vertexcoords.x()+vertexcoords.y()*vertexcoords.y());
        //cout<<vertexrad<<"\t"<<vertexcoords.z()<<endl;
        //cout<<"quadrant "<<trackquads.at(i)<<endl;
        //cout<<"number of hits \t"<<trackhits->size()<<endl;
        // for(Int_t j=0; j<trackhits->size(); j++){
        // TVector3 coords=trackhits->at(j).GetPosition();
        // trackpic->Fill(coords.x(),coords.y(),coords.z());
        // cout<<coords.x()<<"\t"<<coords.y()<<"\t"<<coords.z()<<endl;
        //}
        //cout<<RedChi2.at(i)<<"\t"<<trackVector.at(i).GetNFree()<<endl;
        TPolyLine3D *dummy = new TPolyLine3D(1000);
        TPolyLine *dummy2d = new TPolyLine(1000);
        for (int j = 210; j<1210;++j){
          double t = j-210;
          double x,y,z,r;
          x = parFit[0] + parFit[1]*t;
          y = parFit[2] + parFit[3]*t;
          r = TMath::Sqrt(x*x+y*y);
          z = t;
          //myfile<<x<<"\t"<<y<<endl;
          dummy->SetPoint(j-210,x,y,z);
          dummy2d->SetPoint(j-210,y,z);
        }
        dummy->SetLineColor(kRed);
        dummy2d->SetLineColor(kRed);

        tracksGraph.push_back(dummy);
      }
      //uncomment break statement below if the only point is viewing
      //break;
    }

    if(trackVector.size()>1){
      for(Int_t i=0;i<trackVector.size();i++){
        for(Int_t j=i+1;j<trackVector.size();j++){
          vertexi = trackVector.at(i).GetTrackVertex();
          vertexj = trackVector.at(j).GetTrackVertex();
          vertexmean = 1/2.0*(vertexi+vertexj);
          Double_t vertexmeanrad = TMath::Sqrt(vertexmean.x()*vertexmean.x()+vertexmean.y()*vertexmean.y());
          if(trackVector.at(i).GetAngleZAxis()>trackVector.at(j).GetAngleZAxis()){
            //assigning alpha and beryllium angles
            anglea=trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg();
            angleb=trackVector.at(j).GetAngleZAxis()*TMath::RadToDeg();
            //calculating alpha and beryllium track lengths
            lengtha=(maxradvec[i]-vertexmean).Mag();
            lengthb=(maxradvec[j]-vertexmean).Mag();
          }
          else{
            angleb=trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg();
            anglea=trackVector.at(j).GetAngleZAxis()*TMath::RadToDeg();
            //lengthb=(maxrad[i]-TMath::Sqrt(vertexmean.x()*vertexmean.x()+vertexmean.y()*vertexmean.y()))/TMath::Sin(trackVector.at(i).GetAngleZAxis());
            //lengtha=(maxrad[j]-TMath::Sqrt(vertexmean.x()*vertexmean.x()+vertexmean.y()*vertexmean.y()))/TMath::Sin(trackVector.at(i).GetAngleZAxis());
            lengthb=(maxradvec[i]-vertexmean).Mag();
            lengtha=(maxradvec[j]-vertexmean).Mag();
          }
          //Q02_Kine->Fill(x,y);
          if((trackquads.at(i)+2)%4==trackquads.at(j)){
            //array of the phi and the phi radius for the two relevant quadrants
            std::vector<Double_t> *Phi0Array =quadvec->at(trackquads.at(i)).GetPhiArray();
            std::vector<Double_t> *Phi0RArray =quadvec->at(trackquads.at(i)).GetPhiRArray();
            std::vector<Double_t> *Phi2Array =quadvec->at(trackquads.at(j)).GetPhiArray();
            std::vector<Double_t> *Phi2RArray =quadvec->at(trackquads.at(j)).GetPhiRArray();
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

            Q02_Kine->Fill(x,y);
            if(wideElasticup->IsInside(anglea,angleb)){
              PID->Fill(deltaE,totalE);
              thetacm=180-(2*anglea);
              if (thetacm<0) thetacm= thetacm+180.0;
              //calculating the energy from the center of mass position
              Int_t index=int(floor(vertexmean.z()));
              Double_t over = (vertexmean.z())-index;
              if(index<=offset&&index>0){
                energycm=(2.0/7.0)*(EnergyMM[offset-index]+(EnergyMM[offset-index]-EnergyMM[offset-index+1])*over);
              }
              if(index>-1*offset&&index<0){
                index=TMath::Abs(index);
                energycm=(2.0/7.0)*(39.5+(EnergyMM[offset-index]+(EnergyMM[offset-index]-EnergyMM[offset-index+1])*over));
                //myfile<<intVertex[0]<<"\t"<<energycm1<<index<<endl;
              }
              if(1==1){//TMath::Abs(avg(Phi0Array)-avg(Phi2Array))<15.0){ //not currently cutting on phi, resolution not good enough
                Excitation_EL->Fill(energycm, 2.0/7.0*(ena+enb)); //resolution plot
                Excitation_IN->Fill(thetacm,energycm); //excitation function, elastic


              }
            }
          }
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

//filing in theoretical kinematic lines...
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
Double_t *ThetaCMS2 = new Double_t[20000];
Double_t *ThetaLabRec2 = new Double_t[20000];
Double_t *EnerLabRec2 = new Double_t[20000];
Double_t *ThetaLabSca2 = new Double_t[20000];
Double_t *EnerLabSca2 = new Double_t[20000];


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

c2->cd(1);
gPad->SetLogz();
PID->Draw("colz");
pidcut->Draw("same");
//Vertex->Draw();
c2->cd(2);
Vertex_vs_Angle->Draw("colz");

c3->cd(1);
gPad->SetLogz();
Q02_Kine->Draw("colz");
Kine_AngRec_AngSca->Draw("C");
Kine_AngRec_AngSca_vert->Draw("C");
Kine_AngRec_AngSca_In->Draw("C");
Kine_AngRec_AngSca_In_vert->Draw("C");
wideElasticup->Draw("same");
c3->cd(2);
gPad->SetLogz();
Q13_Kine->Draw("colz");


c4->cd();
// dalitz->Draw("same");
trackpic->Draw();
for(Int_t i =0;i<tracksGraph.size();i++){
  tracksGraph.at(i)->Draw("same");
}

c5->cd(1);
PhiCompare->Draw("*");
c5->cd(2);
PhiCompare1->Draw();

c6->cd(1);
gPad->SetLogz();
Excitation_EL->Draw("colz");
cut1->Draw("same");
cut2->Draw("same");
c6->cd(2);
gPad->SetLogz();
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
