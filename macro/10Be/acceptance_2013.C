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

int positive_modulo(int i, int n) {
  return (i % n + n) % n;
}

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

void acceptance_2013(){
  Int_t mcp = 300;
  Int_t width = 279;


  TVector3 vertex0;
  TVector3 vertex1;
  TVector3 vertexmean;
  Double_t anglea;
  Double_t angleb;
  Double_t lengtha;
  Double_t lengthb;
  Double_t energycm;
  Double_t thetacm;
  Bool_t   accept;
  Double_t threshold=200.0;
  Int_t    entrancetb;
  std::vector<double> RedChi2;
  std::vector<double> maxrad;
  std::vector<TVector3> maxradvec;
  std::vector<TVector3> extrapoints;
  std::vector<Int_t> trackquads;
  std::vector<Int_t> hitquads;
  TVector3 zero;
  zero.SetX(0.0);
  zero.SetY(0.0);
  zero.SetZ(0.0);
  std::vector<int> zlist;
  std::vector<int> numspark;

  Double_t *LengthMM = new Double_t[500];
  Double_t *EnergyMM = new Double_t[500];
  std::ifstream *energyTable = new std::ifstream("energyhigh.dat");
  Int_t numLen=0;
  while(!energyTable->eof()){
    *energyTable>>LengthMM[numLen]>>EnergyMM[numLen];
    numLen++;
  }

  TCanvas *c2 = new TCanvas("c2","c2",200,10,700,700);
  c2->Divide(3,1);
  TCanvas *c3 = new TCanvas("c3","c3",200,10,700,700);
  TCanvas *c4 = new TCanvas("c4","c4",200,10,700,700);
  //c4->Divide(4,1);

  TH2D* All = new TH2D("All", "All",60,0,180,75,0,15);
  TH2D* Reconstructed = new TH2D("Reconstructed", "Reconstructed",60,0,180,75,0,15);
  TH2D* Accepted = new TH2D("Accepted", "Accepted",60,0,180,75,0,15);
  TH1D* size = new TH1D("size", "size", 1000,0,1000);
  TH2D* Q02_Kine = new TH2D("Q02_Kine","Q02_Kine",75,0,15,75,0,15);
  TH2D* AllPoints0 = new TH2D("AllPoints0", "AllPoints0", 2600,-130,130,2600,-130,130);
  TH2D* AllPoints1 = new TH2D("AllPoints1", "AllPoints1", 2600,-130,130,2600,-130,130);
  TH2D* AllPoints2 = new TH2D("AllPoints2", "AllPoints2", 2600,-130,130,2600,-130,130);
  TH2D* AllPoints3 = new TH2D("AllPoints3", "AllPoints3", 2600,-130,130,2600,-130,130);

  TString workdir = getenv("VMCWORKDIR");
  TChain *chain = new TChain("cbmsim");
  TString FileNameHead_num;
  TString FileNameHead_chain;
  TString FilePath = workdir + "/macro/10Be/sim/";
  TString FileNameTail = ".root";

  FileNameHead_num.Form("00%i",21);
  FileNameHead_chain = "10Be_2013_run"+FileNameHead_num;
  std::cout<<" File : "<<FileNameHead_chain<<" added"<<std::endl;
  TString FileName     = FilePath + FileNameHead_chain + FileNameTail;

  std::cout<<" Opening File : "<<FileName.Data()<<std::endl;
  TFile* file = new TFile(FileName.Data(),"READ");
  //TFile* file = new TFile(FileNameHead_chain.Data(),"READ");
  TTree* tree = (TTree*) file -> Get("cbmsim");
  Int_t nEvents = tree -> GetEntriesFast();
  std::cout<<" Number of events : "<<nEvents<<std::endl;

  AtTpcPoint* point = new AtTpcPoint();
  TString mcFileNameHead = "sim/attpcsim_2";
  TString mcFileNameTail = ".root";
  TString mcFileName     = mcFileNameHead + mcFileNameTail;
  std:cout << " Analysis of simulation file  " << mcFileName << endl;
  TClonesArray *pointArray=0;
  TFile* mcfile = new TFile(mcFileName.Data(),"READ");
  TTree* mctree = (TTree*) mcfile -> Get("cbmsim");
  mctree = (TTree*) mcfile -> Get("cbmsim");
  mctree -> SetBranchAddress("AtTpcPoint", &pointArray);


  TTreeReader Reader1("cbmsim", file);
  TTreeReaderValue<TClonesArray> ransacArray(Reader1, "ATRansac");
  TTreeReaderValue<TClonesArray> eventArray(Reader1, "ATEventH");

  int iEvent = 0;
  while (Reader1.Next()) {
    ATRANSACN::ATRansac* ransac = (ATRANSACN::ATRansac*) ransacArray->At(0);
    ATEvent* event = (ATEvent*) eventArray->At(0);
    std::vector<ATTrack> trackVector = ransac->GetTrackCand();
    Float_t *MeshArray              = event->GetMesh();

    Double_t EnergyRecoil = 0.0;
    Double_t AngleRecoil = 0.0;
    Double_t EnergyScatter = 0.0;
    Double_t AngleScatter = 0.0;
    Int_t quada=0;
    Int_t quadb=0;
    Bool_t foundsca= kFALSE;
    Bool_t foundrec= kFALSE;

    TString VolName;
    mctree->GetEvent(iEvent);
    Int_t n = pointArray -> GetEntries();
    //cout<<n<<endl;
    for(Int_t i=0; i<n; i++) {
      point = (AtTpcPoint*) pointArray -> At(i);
      VolName=point->GetVolName();
      Int_t trackID = point -> GetTrackID();
      if(trackID==2 && VolName=="drift_volume"){//&&!foundrec){
        EnergyRecoil= point->GetEIni();
        AngleRecoil= point->GetAIni();
        //cout<<EnergyRecoil<<"\t"<<AngleRecoil<<"rec"<<endl;
        if(iEvent==193)AllPoints3->SetBinContent(AllPoints3->FindBin(point->GetXIn(),point->GetYIn()),point->GetEnergyLoss());
        if(point->GetYIn()>=0){
          if(point->GetXIn()>=0){
              //if(iEvent==189)AllPoints0->SetBinContent(AllPoints0->FindBin(point->GetXIn(),point->GetYIn()),point->GetEnergyLoss());
            quada=0;
          }
          else{
              //if(iEvent==189)AllPoints1->SetBinContent(AllPoints1->FindBin(point->GetXIn(),point->GetYIn()),point->GetEnergyLoss());

            quada=1;
          }
        }
        else{
          if(point->GetXIn()<=0){
            //  if(iEvent==189)AllPoints2->SetBinContent(AllPoints2->FindBin(point->GetXIn(),point->GetYIn()),point->GetEnergyLoss());
            quada=2;
          }
          else{
            //  if(iEvent==189)AllPoints3->SetBinContent(AllPoints3->FindBin(point->GetXIn(),point->GetYIn()),point->GetEnergyLoss());
            quada=3;
          }
        }
        foundrec=kTRUE;
      }
      if(trackID==1 && VolName=="drift_volume"){//&&!foundsca){
        EnergyScatter= point->GetEIni();
        AngleScatter= point->GetAIni();
        //cout<<EnergyScatter<<"\t"<<AngleScatter<<"sca"<<endl;
if(iEvent==193)AllPoints1->SetBinContent(AllPoints1->FindBin(point->GetXIn(),point->GetYIn()),point->GetEnergyLoss());
        if(point->GetYIn()>=0){
          if(point->GetXIn()>=0){
            //  if(iEvent==189)AllPoints0->SetBinContent(AllPoints0->FindBin(point->GetXIn(),point->GetYIn()),point->GetEnergyLoss());
            quadb=0;
          }
          else{
            //  if(iEvent==189)AllPoints1->SetBinContent(AllPoints1->FindBin(point->GetXIn(),point->GetYIn()),point->GetEnergyLoss());
            quadb=1;
          }
        }
        else{
          if(point->GetXIn()<=0){
          //    if(iEvent==189)AllPoints2->SetBinContent(AllPoints2->FindBin(point->GetXIn(),point->GetYIn()),point->GetEnergyLoss());
            quadb=2;
          }
          else{
            //  if(iEvent==189)AllPoints3->SetBinContent(AllPoints3->FindBin(point->GetXIn(),point->GetYIn()),point->GetEnergyLoss());
            quadb=3;
          }
        }
        foundsca=kTRUE;
      }
    }
    if(iEvent%2!=0){
      All->Fill(180.0-(2.0*AngleRecoil),2.0/7.0*(EnergyScatter+EnergyRecoil));
      //if(positive_modulo(quada-quadb,4)!=2)size->Fill(AngleRecoil);
      bool reco = kFALSE;
      accept=kFALSE;


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
        // else if(vertexrad>125.0||vertexcoords.z()>offset||vertexcoords.z()<offset-500.0){
        //   //remove tracks with non-physical vertex
        //   trackVector.erase(trackVector.begin()+i);
        //   RedChi2.erase(RedChi2.begin()+i);
        //   trackquads.erase(trackquads.begin()+i);
        //   numgood.erase(numgood.begin()+i);
        //   numspark.erase(numspark.begin()+i);
        //   maxrad.erase(maxrad.begin()+i);
        //   maxradvec.erase(maxradvec.begin()+i);
        // }
        else{
          i++;
        }
        //i++;
      }

    }
    //size->Fill(trackVector.size());
    if(trackVector.size()>1){
      //cout<<iEvent<<endl;
      anglea=0.0;
      angleb=0.0;
      vertex0 = trackVector.at(0).GetTrackVertex();
      vertex1 = trackVector.at(1).GetTrackVertex();
      vertexmean = 1.0/2.0*(vertex0+vertex1);
      if(trackVector.at(0).GetAngleZAxis()>trackVector.at(1).GetAngleZAxis()){
        anglea=trackVector.at(0).GetAngleZAxis()*TMath::RadToDeg();
        angleb=trackVector.at(1).GetAngleZAxis()*TMath::RadToDeg();
        //angleb = goodang*TMath::RadToDeg();

      }
      else{
        angleb=trackVector.at(0).GetAngleZAxis()*TMath::RadToDeg();
        anglea=trackVector.at(1).GetAngleZAxis()*TMath::RadToDeg();
        //angleb = goodang*TMath::RadToDeg();
      }

      //Q02_Kine->Fill(anglea,angleb);
      if(anglea>5.0&&anglea<20.0&&AngleRecoil>60.0&&trackVector.at(0).GetAngleZAxis()>trackVector.at(1).GetAngleZAxis()){
        //cout<<iEvent<<"\t"<<trackVector.size()<<endl;
      }
      Int_t index=int(floor(vertexmean.z()));
      Double_t over = (vertexmean.z())-index;
      size->Fill(vertexmean.z());
      index = index-150;
      if(index<=500&&index>0){
        energycm=2.0/7.0*(EnergyMM[index]+(EnergyMM[index]-EnergyMM[index+1])*over);
      }
      Q02_Kine->Fill(2.0/7.0*(EnergyScatter+EnergyRecoil),energycm);
      thetacm=180.0-(2.0*anglea);
      if (thetacm<0) thetacm= thetacm+180.0;
      Reconstructed->Fill(thetacm,energycm);
      reco = kTRUE;
      entrancetb = 600;
      for(Int_t tb = 512;tb>0;tb--){
        if(MeshArray[tb]>threshold){
          entrancetb = tb;
          if(entrancetb>mcp&&entrancetb-width<mcp){
            accept=kTRUE;
            Accepted->Fill(thetacm,energycm);
          }
          break;
        }
      }
    }
  if(reco==kFALSE) {
    //Q02_Kine->Fill(EnergyRecoil,EnergyScatter);
    //cout<<EnergyScatter<<"\t"<<EnergyRecoil<<endl;
    //size->Fill(EnergyRecoil);
  }
  }
  iEvent++;
}
c2->cd(1);
All->Draw("colz");
c2->cd(2);
Reconstructed->Draw("colz");
c2->cd(3);
Accepted->Draw("colz");

c3->cd();
Q02_Kine->Draw("colz");

c4->cd();
size->Draw();

}
