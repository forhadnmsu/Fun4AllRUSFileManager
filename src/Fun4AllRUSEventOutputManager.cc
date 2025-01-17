#include <cstdlib>
#include <string>
#include <iostream>
#include <iomanip>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <phool/phool.h>
#include <phool/getClass.h>
#include <phool/PHNode.h>
#include <phool/PHNodeIOManager.h>
#include <interface_main/SQEvent.h>
#include <interface_main/SQRun.h>
#include <interface_main/SQTrackVector.h>
#include <interface_main/SQDimuonVector.h>
#include <ktracker/SRecEvent.h>
#include <interface_main/SQSpillMap.h>
#include <interface_main/SQHitVector.h>
#include <ktracker/SRecEvent.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include "Fun4AllRUSEventOutputManager.h"
#include <fun4all/Fun4AllOutputManager.h>

using namespace std;

Fun4AllRUSEventOutputManager::Fun4AllRUSEventOutputManager(const std::string &myname)
  : Fun4AllOutputManager(myname),
    m_file(0),
    m_tree(0),
    m_tree_name("tree"),
    m_file_name("output.root"),
    m_evt(0),
    m_sp_map(0),
    sq_run(0),
    m_hit_vec(0),
    m_sq_trk_vec(0),
    m_sq_dim_vec(0),
    saveDimuonOnly(false),
    mc_mode(false),
    reco_mode(false)
{
 ;
}

Fun4AllRUSEventOutputManager::~Fun4AllRUSEventOutputManager() {
    CloseFile();
}

int Fun4AllRUSEventOutputManager::OpenFile(PHCompositeNode* startNode) {
	std::cout << "Fun4AllRUSEventOutputManager::OpenFile(): Attempting to open file: " << m_file_name << " with tree: " << m_tree_name << std::endl;
	m_file = new TFile(m_file_name.c_str(), "RECREATE");
	m_file->SetCompressionAlgorithm(ROOT::kLZMA);
	m_file->SetCompressionLevel(5);

	if (!m_file || m_file->IsZombie()) {
		std::cerr << "Error: Could not create file " << m_file_name << std::endl;
		exit(1);
	} else {
		std::cout << "File " << m_file->GetName() << " opened successfully." << std::endl;
	}

	m_tree = new TTree(m_tree_name.c_str(), "Tree for storing events");
	if (!m_tree) {
		std::cerr << "Error: Could not create tree " << m_tree_name << std::endl;
		exit(1);
	} else {
		std::cout << "Tree " << m_tree->GetName() << " created successfully." << std::endl;
	}

	m_tree->Branch("runID", &runID, "runID/I");
	m_tree->Branch("spillID", &spillID, "spillID/I");
	m_tree->Branch("eventID", &eventID, "eventID/I");
	m_tree->Branch("rfID", &rfID, "rfID/I");

	m_tree->Branch("rfIntensity", rfIntensity, "rfIntensity[33]/I");
	m_tree->Branch("fpgaTrigger", fpgaTrigger, "fpgaTrigger[5]/I");
	m_tree->Branch("nimTrigger", nimTrigger, "nimTrigger[5]/I");

	m_tree->Branch("detectorID", &detectorID);
	m_tree->Branch("elementID", &elementID);
	m_tree->Branch("tdcTime", &tdcTime);
	m_tree->Branch("driftDistance", &driftDistance);
	m_tree->Branch("hitsInTime", &hitsInTime);
	if (mc_mode==true){
		m_tree->Branch("mc_track_charges", &mc_track_charges);
		m_tree->Branch("mc_track_id", &mc_track_id);
		m_tree->Branch("mc_pos_vtx_x", &mc_pos_vtx_x);
		m_tree->Branch("mc_pos_vtx_y", &mc_pos_vtx_y);
		m_tree->Branch("mc_pos_vtx_z", &mc_pos_vtx_z);
		m_tree->Branch("mc_mom_vtx_px", &mc_mom_vtx_px);
		m_tree->Branch("mc_mom_vtx_py", &mc_mom_vtx_py);
		m_tree->Branch("mc_mom_vtx_pz", &mc_mom_vtx_pz);
	}

	if (reco_mode==true){
		m_tree->Branch("dimuon_vtx_x", &dimuon_vtx_x);
		m_tree->Branch("dimuon_vtx_y", &dimuon_vtx_y);
		m_tree->Branch("dimuon_vtx_z", &dimuon_vtx_z);
		m_tree->Branch("dimuon_vtx_px", &dimuon_vtx_px);
		m_tree->Branch("dimuon_vtx_py", &dimuon_vtx_py);
		m_tree->Branch("dimuon_vtx_pz", &dimuon_vtx_pz);
		m_tree->Branch("dimuon_mass", &dimuon_mass);
		m_tree->Branch("dimuon_x1", &dimuon_x1);
		m_tree->Branch("dimuon_x2", &dimuon_x2);
		m_tree->Branch("dimuon_xF", &dimuon_xF);

		m_tree->Branch("mu_plus_vtx_x", &mu_plus_vtx_x);
		m_tree->Branch("mu_plus_vtx_y", &mu_plus_vtx_y);
		m_tree->Branch("mu_plus_vtx_z", &mu_plus_vtx_z);
		m_tree->Branch("mu_minus_vtx_x", &mu_minus_vtx_x);
		m_tree->Branch("mu_minus_vtx_y", &mu_minus_vtx_y);
		m_tree->Branch("mu_minus_vtx_z", &mu_minus_vtx_z);

		m_tree->Branch("mu_plus_vtx_px", &mu_plus_vtx_px);
		m_tree->Branch("mu_plus_vtx_py", &mu_plus_vtx_py);
		m_tree->Branch("mu_plus_vtx_pz", &mu_plus_vtx_pz);
		m_tree->Branch("mu_minus_vtx_px", &mu_minus_vtx_px);
		m_tree->Branch("mu_minus_vtx_py", &mu_minus_vtx_py);
		m_tree->Branch("mu_minus_vtx_pz", &mu_minus_vtx_pz);	


		m_tree->Branch("mu_plus_chi2_target", &mu_plus_chi2_target);
		m_tree->Branch("mu_plus_chi2_dump", &mu_plus_chi2_dump);
		m_tree->Branch("mu_plus_chi2_upstream", &mu_plus_chi2_upstream);

		m_tree->Branch("mu_minus_chi2_target", &mu_minus_chi2_target);
		m_tree->Branch("mu_minus_chi2_dump", &mu_minus_chi2_dump);
		m_tree->Branch("mu_minus_chi2_upstream", &mu_minus_chi2_upstream);


		m_tree->SetAutoFlush(2500);
    	       m_tree->SetBasketSize("*", 64000);
	}
	if (mc_mode) {
		m_vec_trk = findNode::getClass<SQTrackVector>(startNode, "SQTruthTrackVector");
		if (!m_vec_trk) {
			return Fun4AllReturnCodes::ABORTEVENT;
		}
	}

	if (reco_mode) {
		m_sq_trk_vec = findNode::getClass<SQTrackVector>(startNode, "SQRecTrackVector");
		m_sq_dim_vec = findNode::getClass<SQDimuonVector>(startNode, "SQRecDimuonVector_PM");

		if (!m_sq_trk_vec) {
			std::cerr << "Error: m_sq_trk_vec is null!" << std::endl;
			return Fun4AllReturnCodes::ABORTEVENT;
		}

		if (!m_sq_dim_vec) {
			std::cerr << "Error: m_sq_dim_vec is null!" << std::endl;
			return Fun4AllReturnCodes::ABORTEVENT;
		}
	}

	m_evt = findNode::getClass<SQEvent>(startNode, "SQEvent");
	m_hit_vec = findNode::getClass<SQHitVector>(startNode, "SQHitVector");

	if (!m_evt || !m_hit_vec) {
		return Fun4AllReturnCodes::ABORTEVENT;
	}
	
	return Fun4AllReturnCodes::EVENT_OK;
}
int Fun4AllRUSEventOutputManager::Write(PHCompositeNode* startNode) {
	if (!m_file || !m_tree) {
		OpenFile(startNode);
	}

	ResetBranches();

	if (! m_evt->get_trigger(SQEvent::MATRIX1)) {
		return Fun4AllReturnCodes::EVENT_OK;
 	 }
	if (saveDimuonOnly && m_sq_dim_vec->empty()) {
		return 0;  // Skip this event if no dimuons are present and the mode is enabled
        }

	runID = m_evt->get_run_id();
	spillID = m_evt->get_spill_id();
	rfID = m_evt->get_qie_rf_id();
	eventID = m_evt->get_event_id();
	//turnID = m_evt->get_qie_turn_id();

	fpgaTrigger[0] = m_evt->get_trigger(SQEvent::MATRIX1);
	fpgaTrigger[1] = m_evt->get_trigger(SQEvent::MATRIX2);
	fpgaTrigger[2] = m_evt->get_trigger(SQEvent::MATRIX3);
	fpgaTrigger[3] = m_evt->get_trigger(SQEvent::MATRIX4);
	fpgaTrigger[4] = m_evt->get_trigger(SQEvent::MATRIX5);

	nimTrigger[0] = m_evt->get_trigger(SQEvent::NIM1);
	nimTrigger[1] = m_evt->get_trigger(SQEvent::NIM2);
	nimTrigger[2] = m_evt->get_trigger(SQEvent::NIM3);
	nimTrigger[3] = m_evt->get_trigger(SQEvent::NIM4);
	nimTrigger[4] = m_evt->get_trigger(SQEvent::NIM5);
	for (int i = -16; i < 16; ++i) {
    // cout << "intensity index: i" << i+16 << endl;
    	rfIntensity[i+ 16] = m_evt->get_qie_rf_intensity(i);
}

if (m_hit_vec) {
    for (int ihit = 0; ihit < m_hit_vec->size(); ++ihit) {
        SQHit* hit = m_hit_vec->at(ihit);
	//if (hit->is_in_time() ==0) continue;
	//cout << "is_in_time(): "<< hit->is_in_time() << endl;
        detectorID.push_back(hit->get_detector_id());
        elementID.push_back(hit->get_element_id());
        tdcTime.push_back(hit->get_tdc_time());
        driftDistance.push_back(hit->get_drift_distance());
    }
}

if(mc_mode==true){
	for (unsigned int ii = 0; ii < m_vec_trk->size(); ii++) {
		SQTrack* trk = m_vec_trk->at(ii);
		
		mc_track_charges.push_back(trk->get_charge());
		mc_track_id.push_back(trk->get_track_id());
		mc_pos_vtx_x.push_back(trk->get_pos_vtx().X());
		mc_pos_vtx_y.push_back(trk->get_pos_vtx().Y());
		mc_pos_vtx_z.push_back(trk->get_pos_vtx().Z());
		mc_mom_vtx_px.push_back(trk->get_mom_vtx().Px());
		mc_mom_vtx_py.push_back(trk->get_mom_vtx().Py());
		mc_mom_vtx_pz.push_back(trk->get_mom_vtx().Pz());
	}
}

if(reco_mode==true){
	for (auto it = m_sq_dim_vec->begin(); it != m_sq_dim_vec->end(); it++) {
		SRecDimuon& sdim = dynamic_cast<SRecDimuon&>(**it);
		sdim.calcVariables(); //eventually need to assign 1 or 2 depending on the target or dump; more at  https://github.com/E1039-Collaboration/e1039-core/pull/149

		int trk_id_pos = sdim.get_track_id_pos();
		int trk_id_neg = sdim.get_track_id_neg();
		SRecTrack& trk_pos = dynamic_cast<SRecTrack&>(*(m_sq_trk_vec->at(trk_id_pos))); 
		SRecTrack& trk_neg = dynamic_cast<SRecTrack&>(*(m_sq_trk_vec->at(trk_id_neg))); 
		
		// Fill dimuon vectors
		dimuon_vtx_x.push_back(sdim.get_pos().X());
		dimuon_vtx_y.push_back(sdim.get_pos().Y());
		dimuon_vtx_z.push_back(sdim.get_pos().Z());
		dimuon_vtx_px.push_back(sdim.get_mom().Px());
		dimuon_vtx_py.push_back(sdim.get_mom().Py());
		dimuon_vtx_pz.push_back(sdim.get_mom().Pz());

		cout << "----------------------------------"<< endl;

		cout << "dim mass "<< sdim.get_mom().M() <<endl;
		cout << "dim mass 2 "<<sdim.mass <<endl;
		cout << "dim x1 "<<sdim.x1 <<endl;
		cout << "dim x2 "<< sdim.x2<<endl;
		cout << "dim Pz "<< sdim.get_mom().Pz()<<endl;

		dimuon_mass.push_back(sdim.get_mom().M());
		dimuon_x1.push_back(sdim.x1);
		dimuon_x2.push_back(sdim.x2);
		dimuon_xF.push_back(sdim.xF);
		// Fill mu_plus vectors
		mu_plus_vtx_x.push_back(trk_pos.get_pos_vtx().X());
		mu_plus_vtx_y.push_back(trk_pos.get_pos_vtx().Y());
		mu_plus_vtx_z.push_back(trk_pos.get_pos_vtx().Z());

		mu_plus_vtx_px.push_back(trk_pos.get_mom_vtx().X());
		mu_plus_vtx_py.push_back(trk_pos.get_mom_vtx().Y());
		mu_plus_vtx_pz.push_back(trk_pos.get_mom_vtx().Z());

		// Fill mu_minus vectors
		mu_minus_vtx_x.push_back(trk_neg.get_pos_vtx().X());
		mu_minus_vtx_y.push_back(trk_neg.get_pos_vtx().Y());
		mu_minus_vtx_z.push_back(trk_neg.get_pos_vtx().Z());

		mu_minus_vtx_px.push_back(trk_neg.get_mom_vtx().X());
		mu_minus_vtx_py.push_back(trk_neg.get_mom_vtx().Y());
		mu_minus_vtx_pz.push_back(trk_neg.get_mom_vtx().Z());

	}
}
	m_tree->Fill();
	return 0;
}

void Fun4AllRUSEventOutputManager::CloseFile() {
    if (!m_file) return;
    std::cout << "Fun4AllRUSEventOutputManager::CloseFile(): Closing file: " << m_file_name << std::endl;
    m_file->Write();
    m_file->Close();
    delete m_file;
    m_file = nullptr;
}

void Fun4AllRUSEventOutputManager::ResetBranches() {
	detectorID.clear();
	elementID.clear();
	tdcTime.clear();
	driftDistance.clear();
	//mc events
	mc_track_charges.clear();
	mc_track_id.clear();
	mc_pos_vtx_x.clear();
	mc_pos_vtx_y.clear();
	mc_pos_vtx_z.clear();
	mc_mom_vtx_px.clear();
	mc_mom_vtx_py.clear();
	mc_mom_vtx_pz.clear();

	//reco variables
	dimuon_vtx_x.clear();
	dimuon_vtx_y.clear();
	dimuon_vtx_z.clear();
	dimuon_vtx_px.clear();
	dimuon_vtx_py.clear();
	dimuon_vtx_pz.clear();
	dimuon_mass.clear();
	dimuon_x1.clear();
	dimuon_x2.clear();
	dimuon_xF.clear();

	mu_plus_vtx_x.clear();
	mu_plus_vtx_y.clear();
	mu_plus_vtx_z.clear();
	mu_minus_vtx_x.clear();
	mu_minus_vtx_y.clear();
	mu_minus_vtx_z.clear();
	mu_plus_vtx_px.clear();
	mu_plus_vtx_py.clear();
	mu_plus_vtx_pz.clear();
	mu_minus_vtx_px.clear();
	mu_minus_vtx_py.clear();
	mu_minus_vtx_pz.clear();

	mu_plus_chi2_target.clear();
	mu_plus_chi2_dump.clear();
	mu_plus_chi2_upstream.clear();

	mu_minus_chi2_target.clear();
	mu_minus_chi2_dump.clear();
	mu_minus_chi2_upstream.clear();
}
