R__LOAD_LIBRARY(libfun4all_rus_file_manager)
R__LOAD_LIBRARY(libfun4all)
R__LOAD_LIBRARY(libdptrigger)
R__LOAD_LIBRARY(libembedding)
R__LOAD_LIBRARY(libevt_filter)
R__LOAD_LIBRARY(libktracker)
R__LOAD_LIBRARY(libcalibrator)

int Fun4Test(){
	const string Vect_in = "/pnfs/e1039/scratch/users/kenichi/RecoData2024/reco/run_005848/spill_001916124/out/run_005848_spill_001916124_spin_reco.root";
	//const string Vect_in = "/pnfs/e1039/scratch/users/kenichi/RecoData2024/reco/run_006059/spill_001932164/out/run_006059_spill_001932164_spin_reco.root";

	recoConsts* rc = recoConsts::instance();
	rc->set_IntFlag("RUNNUMBER", 6124);
	rc->set_DoubleFlag("FMAGSTR", -1.044);
	rc->set_DoubleFlag("KMAGSTR", -1.025);

	Fun4AllServer* se = Fun4AllServer::instance();
	se->setRun(6155);
	Fun4AllInputManager *in = new Fun4AllDstInputManager("DUMMY");
	se->registerInputManager(in);
	in->fileopen(Vect_in);


	/*
	   Fun4AllVectEventOutputManager* tree = new Fun4AllVectEventOutputManager();
	   tree->Verbosity(100);
	   tree->SetTreeName("tree");
	   tree->SetFileName("Vector-In-Mum.root");
	   se->registerOutputManager(tree);
	   */


	Fun4AllRUSEventOutputManager* tree = new Fun4AllRUSEventOutputManager();
	tree->Verbosity(100);
	tree->SetTreeName("tree");
	tree->SetMCMode(false);
	tree->SetRecoMode(false);
	tree->SetSaveOnlyDimuon(false);
	tree->SetFileName("RUS.root");
	se->registerOutputManager(tree);

	se->run(100);
	se->End();
	se->PrintTimer();
	std::cout << "All done" << std::endl;
	delete se;
	gSystem->Exit(0);
	return 0;
}
