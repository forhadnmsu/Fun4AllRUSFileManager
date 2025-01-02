#include <string>
#include <iostream>
R__LOAD_LIBRARY(libfun4all_rus_file_manager)
R__LOAD_LIBRARY(libcalibrator)
R__LOAD_LIBRARY(libktracker)

int ConvertToVect (){
	const char* fn_udst= "uDST.root";
	Fun4AllServer* se = Fun4AllServer::instance();
	Fun4AllInputManager *in = new Fun4AllDstInputManager("DUMMY");
	se->registerInputManager(in);
	in->fileopen(fn_udst);
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
        tree->SetFileName("RUS_SpinQuest_Exp_Data_Std_Occu.root");
        se->registerOutputManager(tree);


	Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", "DST.root");
        se->registerOutputManager(out);

	se->run(0);
	se->End();
	return 0;
}
