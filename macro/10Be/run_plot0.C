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

double avg(std::vector<Double_t> *v) {
  return 1.0 * std::accumulate(v->begin(), v->end(), 0LL) / v->size();
}

void run_plot0(TString FileNameHead = "output_proto",TString fileKine="../Kinematics/Decay_kinematics/10Be_4He_19MeV.txt")
{

  TCanvas *c2 = new TCanvas("c2","c2",200,10,700,700);
  c2->Divide(2,1);
  TCanvas *c3 = new TCanvas("c3","c3",200,10,700,700);
  TCanvas *c4 = new TCanvas("c4","c4",200,10,700,700);
  TCanvas *c5 = new TCanvas("c5","c5",200,10,700,700);
  c5->Divide(2,1);
  TCanvas *c6 = new TCanvas("c6","c6",200,10,700,700);
  c6->Divide(2,1);

  TH2D* Q02_Kine = new TH2D("Q02_Kine","Q02_Kine",180,0,180,180,0,180);
  Q02_Kine->SetMarkerColor(2);
  Q02_Kine->SetMarkerStyle(20);
  Q02_Kine->SetMarkerSize(0.7);
  TH2D* Q13_Kine = new TH2D("Q13_Kine","Q13_Kine",1000,0,180,1000,0,180);
  Q13_Kine->SetMarkerColor(2);
  Q13_Kine->SetMarkerStyle(20);
  Q13_Kine->SetMarkerSize(0.7);

  TH1D* Vertex = new TH1D("Vertex","Vertex",1000,0,500000);
  TH1D* Vertex1 = new TH1D("Vertex1","Vertex1",10,0,10);
  TH2D* Vertex_vs_Angle = new TH2D("Vertex_vs_Angle","Vertex_vs_Angle",1000,0,1000,200,0,180);

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

  TH2D* Excitation_EL = new TH2D("Elastic", "Elastic",36,0,180,200,-1000,1000);
  TH2D* Excitation_IN = new TH2D("Inelastic", "Inelastic",36,0,180,120,0,12);

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
  Int_t offset = 585;
  Double_t maxchi2=50.0;
  std::vector<TPolyLine3D*> tracksGraph;
  std::vector<Int_t> trackquads;
  std::vector<Int_t> hitquads;

  std::vector<double> RedChi2;
  Double_t x;
  Double_t y;
  ofstream myfile;
  myfile.open ("stuff.txt");

  TString workdir = getenv("VMCWORKDIR");
  TChain *chain = new TChain("cbmsim");
  TFileCollection *filecol = new TFileCollection();
  TString FileNameHead_num;
  TString FileNameHead_chain;
  TString FilePath = workdir + "/macro/10Be/newmesh/";
  TString FileNameTail = ".root";
  TString FileName     = FilePath + FileNameHead + FileNameTail;
  Int_t file_ini=21;
  Int_t file_end=21;
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


      RedChi2.clear();
      if(trackVector.size()>0){
        for(Int_t i=0;i<trackVector.size();i++){
          RedChi2.push_back(trackVector.at(i).GetMinimum()/double(trackVector.at(i).GetHitArray()->size()));
          //Vertex->Fill(RedChi2.at(i));
          //Vertex->Fill(trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg());
          //myfile<<trackVector.at(i).GetMinimum()<<"\t"<<trackVector.at(i).GetHitArray()->size()<<"\t"<<RedChi2.at(i)<<endl;
          hitquads.clear();
          for(Int_t j=0;j<trackVector.at(i).GetHitArray()->size();j++){
            ATHit *Hit = event->GetHit(i);
            TVector3 coords= Hit->GetPosition();
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
      std::vector<std::vector<int>> numgood(5);
      std::vector<std::vector<int>> numspark(5);
      std::vector<int> defkeep;
      if(trackVector.size()>0){
        Int_t i=0;
        while(i<trackVector.size()){
          std::vector<Double_t> parFit = trackVector.at(i).GetFitPar();
          std::vector<ATHit>* trackhits = trackVector.at(i).GetHitArray();
          TVector3 vertexcoords = trackVector.at(i).GetTrackVertex();
          Double_t vertexrad = TMath::Sqrt(vertexcoords.x()*vertexcoords.x()+vertexcoords.y()*vertexcoords.y());
          for(Int_t hit1=0; hit1<trackhits->size(); hit1++){
            TVector3 coords1=trackhits->at(hit1).GetPosition();
            for(Int_t hit2=hit1+1; hit2<trackhits->size(); hit2++){
              TVector3 coords2=trackhits->at(hit2).GetPosition();
              if(coords1.z()==coords2.z())numspark.at(i).push_back(hit1);
            }
            std::sort(numspark.at(i).begin(),numspark.at(i).end());
            auto last = std::unique(numspark.at(i).begin(),numspark.at(i).end());
            numspark.at(i).erase(last, numspark.at(i).end());
          }
          TPolyLine *dummy2d = new TPolyLine(1000);
          for (int j = 0; j<1000;++j){
            double t = j;
            double x,y,z,r;
            x = parFit[0] + parFit[1]*t;
            y = parFit[2] + parFit[3]*t;
            r = TMath::Sqrt(x*x+y*y);
            z = t;
            //myfile<<x<<"\t"<<y<<endl;
            dummy2d->SetPoint(j,r,z);
          }
          Int_t j=i+1;
          while(j<trackVector.size()){
            std::vector<ATHit>* trackhits2 = trackVector.at(j).GetHitArray();
            for(Int_t k=0; k<trackhits2->size(); k++){
              TVector3 coords=trackhits2->at(k).GetPosition();
              if(TMath::Abs(dummy2d->GetX()[int(floor(coords.z()))]-TMath::Sqrt(coords.x()*coords.x()+coords.y()*coords.y()))<3.5){
                numgood.at(j).push_back(k);
              }
            }
            std::sort(numgood.at(j).begin(),numgood.at(j).end());
            auto last = std::unique(numgood.at(j).begin(),numgood.at(j).end());
            numgood.at(j).erase(last, numgood.at(j).end());

            if(1==2){//ath::Abs(trackVector.at(i).GetAngleZAxis()-trackVector.at(j).GetAngleZAxis())<0.00){
              trackVector.erase(trackVector.begin()+j);
              RedChi2.erase(RedChi2.begin()+j);
              trackquads.erase(trackquads.begin()+j);
              numgood.erase(numgood.begin()+j);
              numspark.erase(numspark.begin()+j);
            }
            else{
              j++;
            }
          }
          if(trackVector.at(i).GetAngleZAxis()<0.08){//}||RedChi2.at(i)>maxchi2){
            trackVector.erase(trackVector.begin()+i);
            RedChi2.erase(RedChi2.begin()+i);
            trackquads.erase(trackquads.begin()+i);
            numgood.erase(numgood.begin()+i);
            numspark.erase(numspark.begin()+i);
          }
          else if((trackhits->size())/2<numgood.at(i).size()){
            //cout<<numgood[i].size()<<"\t"<<trackhits->size()<<endl;
            trackVector.erase(trackVector.begin()+i);
            RedChi2.erase(RedChi2.begin()+i);
            trackquads.erase(trackquads.begin()+i);
            numgood.erase(numgood.begin()+i);
            numspark.erase(numspark.begin()+i);
          }
          else if(numspark.at(i).size()>2){
            //cout<<numgood[i].size()<<"\t"<<trackhits->size()<<endl;
            trackVector.erase(trackVector.begin()+i);
            RedChi2.erase(RedChi2.begin()+i);
            trackquads.erase(trackquads.begin()+i);
            numgood.erase(numgood.begin()+i);
            numspark.erase(numspark.begin()+i);
          }
          /*else if(vertexrad>7.4||vertexcoords.z()>500.0||vertexcoords.z()<0){
          //cout<<numgood[i].size()<<"\t"<<trackhits->size()<<endl;
          trackVector.erase(trackVector.begin()+i);
          RedChi2.erase(RedChi2.begin()+i);
          trackquads.erase(trackquads.begin()+i);
          numgood.erase(numgood.begin()+i);
          numspark.erase(numspark.begin()+i);
        }*/
        else{
          i++;
        }
        //i++;
      }

    }

    Vertex1->Fill(trackVector.size());
    Double_t totalE = 0;
    for(Int_t i=0;i<410;i++){
      totalE = totalE+MeshArray[i];
    }
    Vertex->Fill(totalE);

    if(evnt==16150){
      cout<<trackVector.size()<<endl;
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
        cout<<"angle "<<trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg()<<endl;
        Double_t vertexrad = TMath::Sqrt(vertexcoords.x()*vertexcoords.x()+vertexcoords.y()*vertexcoords.y());
        cout<<vertexrad<<"\t"<<vertexcoords.z()<<endl;
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
      //break;
    }



    if(trackVector.size()>1){
      for(Int_t i=0;i<trackVector.size();i++){
        for(Int_t j=i+1;j<trackVector.size();j++){
          if(trackVector.at(i).GetAngleZAxis()>trackVector.at(j).GetAngleZAxis()){
            x=trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg();
            y=trackVector.at(j).GetAngleZAxis()*TMath::RadToDeg();
          }
          else{
            y=trackVector.at(i).GetAngleZAxis()*TMath::RadToDeg();
            x=trackVector.at(j).GetAngleZAxis()*TMath::RadToDeg();
          }
          //Q02_Kine->Fill(x,y);
          if((trackquads.at(i)+2)%4==trackquads.at(j)){
            std::vector<Double_t> *Phi0Array =quadvec->at(trackquads.at(i)).GetPhiArray();
            std::vector<Double_t> *Phi0RArray =quadvec->at(trackquads.at(i)).GetPhiRArray();
            std::vector<Double_t> *Phi2Array =quadvec->at(trackquads.at(j)).GetPhiArray();
            std::vector<Double_t> *Phi2RArray =quadvec->at(trackquads.at(j)).GetPhiRArray();
            if(Phi0Array->size()>5){
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
            //cout<<evnt<<"\t"<<avg(Phi0Array)<<"\t"<<avg(Phi2Array)<<endl;
            //PhiCompare->Fill(avg(Phi0Array),avg(Phi2Array));
            //PhiCompare1->Fill(avg(Phi0Array)-avg(Phi2Array));
            if(TMath::Abs(avg(Phi0Array)-avg(Phi2Array))<15.0){
              //Q02_Kine->Fill(x,y);
            }
            if(ransac->GetVertexMean().z()>00.0&&ransac->GetVertexMean().z()<100.0){
            //Q02_Kine->Fill(x,y);
            }
            Q02_Kine->Fill(x,y);
            if(!wideElasticup->IsInside(x,y)){
              //cout<<"noise "<<evnt<<endl;
            }
            if(x-y<10.0){
              cout<<"line "<<evnt<<endl;
            }
            if(wideElasticup->IsInside(x,y)){
              //Q02_Kine->Fill(x,y);
              thetacm=180-(2*x);
              if (thetacm<0) thetacm= thetacm+180.0;
              Int_t index=int(floor(ransac->GetVertexMean().z()));
              //cout<<EnergyMM[index]<<endl;
              Double_t over = ransac->GetVertexMean().z()-index;
              if(index<=offset&&index>0){
                energycm=(2.0/7.0)*(EnergyMM[offset-index]+(EnergyMM[offset-index]-EnergyMM[offset-index+1])*over);
              }
              if(index>-500&&index<0){
                index=TMath::Abs(index);
                energycm=(2.0/7.0)*(39.5+(EnergyMM[offset-index]+(EnergyMM[offset-index]-EnergyMM[offset-index+1])*over));
                //myfile<<intVertex[0]<<"\t"<<energycm1<<index<<endl;
              }
              if(TMath::Abs(avg(Phi0Array)-avg(Phi2Array))<15.0){
                Excitation_EL->Fill(thetacm,ransac->GetVertexMean().z());
                Excitation_IN->Fill(thetacm,energycm);}


              }
            }
          }
        }
      }






      if(evnt%10000==0)std::cout<<" Event : "<<evnt<<std::endl;
      evnt++;
    }
    file->Close();
  }

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
  Vertex->Draw();
  c2->cd(2);
  Vertex1->Draw();

  c3->cd();
  gPad->SetLogz();
  Q02_Kine->Draw("colz");
  // Kine_AngRec_AngSca->Draw("C");
  // Kine_AngRec_AngSca_vert->Draw("C");
  // Kine_AngRec_AngSca_In->Draw("C");
  // Kine_AngRec_AngSca_In_vert->Draw("C");
  // wideElasticup->Draw("same");


  c4->cd();
  //dalitz->Draw("same");
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
  c6->cd(2);
  gPad->SetLogz();
  Excitation_IN->Draw("colz");


}
