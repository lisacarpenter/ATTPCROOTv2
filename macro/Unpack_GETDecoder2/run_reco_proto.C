void run_reco_proto(Int_t runNo = 100, Int_t evNo=100, TString parameterFile = "pATTPC.TRIUMF2015.par")
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
	TString inputDir = "/data/iris/root/unpack/";
	TString inputName = inputDir + FileName + ".root";
	TString outputDir = "/data/iris/root/reco/";
	TString outputName = outputDir + FileName + ".root";
	
	FairLogger *logger = FairLogger::GetLogger();
	logger -> SetLogFileName("ATTPC_RecoLog.log");
	logger -> SetLogToFile(kTRUE);
	logger -> SetLogToScreen(kTRUE);
	logger -> SetLogVerbosityLevel("MEDIUM");
	
	FairRunAna* run = new FairRunAna();
	run -> SetInputFile(inputName);
	run -> SetOutputFile(outputName);
	
	TString paramterFileWithPath = paraDir + parameterFile;
	
	FairRuntimeDb* rtdb = run->GetRuntimeDb();
	FairParAsciiFileIo* parIo1 = new FairParAsciiFileIo();
	parIo1 -> open(paramterFileWithPath.Data(), "in");
	rtdb -> setSecondInput(parIo1);
	
	ATPhiRecoTask *phirecoTask = new ATPhiRecoTask();
	phirecoTask -> SetPersistence();
	run -> AddTask(phirecoTask);
	
	ATHoughTask *HoughTask = new ATHoughTask();
	HoughTask->SetPhiReco();
	HoughTask->SetPersistence();
	HoughTask->SetLinearHough();
	HoughTask->SetRadiusThreshold(3.0); // Truncate Hough Space Calculation
	//HoughTask ->SetCircularHough();
	run ->AddTask(HoughTask);
	
	run->Init();
	
	run->Run(0,evNo);
	
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
