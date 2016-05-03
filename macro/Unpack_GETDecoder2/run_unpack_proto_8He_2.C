void run_unpack_proto_8He_2(Int_t runNo = 100,Int_t EvNo=0, TString parameterFile = "pATTPC.TRIUMF2015.par")
{
	// -----   Timer   --------------------------------------------------------
	TStopwatch timer;
	timer.Start();
  	// ------------------------------------------------------------------------

	gSystem->Load("libXMLParser.so");	

	TString FileNameRunNo;
	FileNameRunNo.Form("%i",runNo);
	TString FileName = "run_"+ FileNameRunNo;
	TString scriptfile = "LookupProto20150331.xml";
	TString protomapfile = "proto.map";
	TString dir = getenv("VMCWORKDIR");
	TString scriptdir = dir + "/scripts/"+ scriptfile;
	TString protomapdir = dir + "/scripts/"+ protomapfile;
	TString geo = "proto_geo_hires.root";
	TString paraDir = dir + "/parameters/";
	TString dataDir = dir + "/runlists/";
	TString dataFile = dataDir + FileName + ".txt";

	TString outputDir = "/data/iris/root/unpack/";
	TString outputName = outputDir + FileName + ".root";
	
	FairLogger *logger = FairLogger::GetLogger();
	logger -> SetLogFileName("ATTPCLog.log");
	logger -> SetLogToFile(kTRUE);
	logger -> SetLogToScreen(kTRUE);
	logger -> SetLogVerbosityLevel("MEDIUM");
	
	FairRunAna* run = new FairRunAna();
	run -> SetOutputFile(outputName);
	
	TString paramterFileWithPath = paraDir + parameterFile;
	
	FairRuntimeDb* rtdb = run->GetRuntimeDb();
	FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
	parIo1 -> open(paramterFileWithPath.Data(), "in");
   	rtdb -> setSecondInput(parIo1);

   	ATDecoder2Task *decoderTask = new ATDecoder2Task();
   	//decoderTask ->SetDebugMode(kTRUE);
   	decoderTask ->SetMapOpt(1); // ATTPC : 0  - Prototype: 1 |||| Default value = 0

   	if (dataFile.EndsWith(".txt")){
    	std::ifstream listFile(dataFile.Data());
      	TString dataFileWithPath;
			while (dataFileWithPath.ReadLine(listFile)) {
				decoderTask -> AddData(dataFileWithPath);
			}
	}
	else decoderTask -> AddData(dataFile.Data());

	decoderTask ->SetGeo(geo.Data());
   	decoderTask ->SetProtoMap(protomapdir.Data());
   	decoderTask ->SetMap((Char_t const*) scriptdir.Data());
   	decoderTask -> SetPersistence();
   	run -> AddTask(decoderTask);

   	ATPSATask *psaTask = new ATPSATask();
   	psaTask -> SetPersistence();
   	psaTask -> SetBackGroundPeakFinder(kFALSE); // Suppress background of each pad for noisy data (Larger computing Time)
   	psaTask -> SetThreshold(20);
   	psaTask -> SetPeakFinder(); //Note: For the moment not affecting the prototype PSA Task
   	run -> AddTask(psaTask);

   	run->Init();

	if(EvNo==0)	{ run->RunOnTBData(); }
	else { run->Run(0,EvNo); }

 	// -----   Finish   -------------------------------------------------------
	timer.Stop();
	Double_t rtime = timer.RealTime();
	Double_t ctime = timer.CpuTime();
	cout << endl << endl;
	cout << "Macro finished succesfully." << endl;
	cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
	cout << endl;
	gApplication->Terminate();
  	// ------------------------------------------------------------------------
}
