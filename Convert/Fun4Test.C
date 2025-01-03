R__LOAD_LIBRARY(libfun4all_rus_file_manager)
R__LOAD_LIBRARY(libfun4all)
R__LOAD_LIBRARY(libdptrigger)
R__LOAD_LIBRARY(libembedding)
R__LOAD_LIBRARY(libevt_filter)
R__LOAD_LIBRARY(libktracker)
R__LOAD_LIBRARY(libcalibrator)

int Fun4Test(){
	const string Vect_in = "/pnfs/e1039/scratch/users/kenichi/RecoData2024/reco/run_006155/spill_001941929/out/run_006155_spill_001941929_spin_reco.root";

	recoConsts* rc = recoConsts::instance();
	rc->set_IntFlag("RUNNUMBER", 6155);
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
	tree->SetDataTriggerEmu(true); //turn this one true if you turn off SetMCTriggerEmu
	tree->SetMCTriggerEmu(false);
	tree->SetRecoMode(true);
	tree->SetSaveOnlyDimuon(true);
	tree->SetFileName("DST.root");
	se->registerOutputManager(tree);

	se->run(100);
	se->End();
	se->PrintTimer();
	std::cout << "All done" << std::endl;
	delete se;
	gSystem->Exit(0);
	return 0;
}
