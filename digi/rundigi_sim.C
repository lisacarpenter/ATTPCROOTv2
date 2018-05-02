void rundigi_sim()
{


  // -----   Timer   --------------------------------------------------------
 TStopwatch timer;
 timer.Start();
 // ------------------------------------------------------------------------
 TString mcFile = "~/fair_install/ATTPCROOTv2/digi/attpcsim_2.root";
 TString scriptfile = "LookupProto10Be.xml";
 TString protomapfile = "proto.map";
 TString dir = getenv("VMCWORKDIR");
 TString scriptdir = dir + "/scripts/"+ scriptfile;
 TString protomapdir = dir + "/scripts/"+ protomapfile;
 TString geo = "proto_geo_hires.root";
 TString paraDir = dir + "/parameters/";
 TString parameterFile = "pATTPC.Feb2013.par";
 TString paramterFileWithPath = paraDir + parameterFile;
  // __ Run ____________________________________________
  FairRunAna* fRun = new FairRunAna();
              fRun -> SetInputFile(mcFile);
              fRun -> SetOutputFile("~/fair_install/ATTPCROOTv2/macro/10Be/output_sim.root");


  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
              FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
              parIo1 -> open(paramterFileWithPath.Data(), "in");
              rtdb -> setFirstInput(parIo1);
              //FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
              //parIo2 -> open(trigParFile.Data(), "in");

  // __ AT digi tasks___________________________________

  ATClusterizeTask* clusterizer = new ATClusterizeTask();
                clusterizer -> SetPersistence(kFALSE);

  ATPulseTask* pulse = new ATPulseTask();
  pulse ->SetMapOpt(1);
  pulse ->SetGeo(geo.Data());
  pulse ->SetProtoMap(protomapdir.Data());
  pulse ->SetMap((Char_t const*) scriptdir.Data());
  pulse -> SetPersistence(kTRUE);

      // ATPSATask *psaTask = new ATPSATask();
      // psaTask -> SetPersistence(kTRUE);
      // psaTask -> SetThreshold(20);
      // psaTask -> SetPSAMode(1); //NB: 1 is ATTPC - 2 is pATTPC
      // //psaTask -> SetPeakFinder(); //NB: Use either peak finder of maximum finder but not both at the same time
      // psaTask -> SetMaxFinder();
      // psaTask -> SetBaseCorrection(kTRUE); //Directly apply the base line correction to the pulse amplitude to correct for the mesh induction. If false the correction is just saved
      // psaTask -> SetTimeCorrection(kFALSE); //Interpolation around the maximum of the signal peak
      //
      // ATTriggerTask *trigTask = new ATTriggerTask();
      // trigTask  ->  SetAtMap(mapParFile);
      // trigTask  ->  SetPersistence(kTRUE);


  fRun -> AddTask(clusterizer);
  fRun -> AddTask(pulse);
//  fRun -> AddTask(psaTask);
  //fRun -> AddTask(trigTask);

  // __ Init and run ___________________________________
  fRun -> Init();
  fRun -> Run(0,10000);
  //fRun -> RunOnTBData();

  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully."  << std::endl << std::endl;
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------
}
