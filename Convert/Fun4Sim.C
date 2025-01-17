R__LOAD_LIBRARY(libfun4all_rus_file_manager)
R__LOAD_LIBRARY(libfun4all)
R__LOAD_LIBRARY(libPHPythia8)
R__LOAD_LIBRARY(libg4detectors)
R__LOAD_LIBRARY(libg4testbench)
R__LOAD_LIBRARY(libg4eval)
R__LOAD_LIBRARY(libg4dst)
R__LOAD_LIBRARY(libdptrigger)
R__LOAD_LIBRARY(libembedding)
R__LOAD_LIBRARY(libevt_filter)
R__LOAD_LIBRARY(libktracker)
R__LOAD_LIBRARY(libSQPrimaryGen)
R__LOAD_LIBRARY(libcalibrator)

//int Fun4All(const string fn_udst, const string DST_out, const int n_evt=20){
int Fun4Sim(const string Vect_in, const string DST_out, const int n_evt=20){
	Fun4AllServer* se = Fun4AllServer::instance();
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
        tree->SetFileName(DST_out);
        se->registerOutputManager(tree);

	 se->run(n_evt);
        se->End();
        se->PrintTimer();
        std::cout << "All done" << std::endl;
        delete se;
        gSystem->Exit(0);
        return 0;
}
