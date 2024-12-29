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
#include <ktracker/SRecEvent.h>
#include <interface_main/SQSpillMap.h>
#include <interface_main/SQHitVector.h>
#include <interface_main/SQDimuonVector.h>
#include <ktracker/SRecEvent.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include "Fun4AllRUSEventOutputManager.h"
#include <interface_main/SQTrackVector.h>

using namespace std;

Fun4AllRUSEventOutputManager::Fun4AllRUSEventOutputManager(const std::string &myname)
  : Fun4AllOutputManager(myname),
    m_file(0),
    m_tree(0),
    m_tree_name("tree"),
    m_file_name("output.root"),
    m_evt(0),
    m_sp_map(0),
    m_hit_vec(0),
    mc_mode(false),
    reco_mode(false),
    data_trig_mode(false),
    mc_trig_mode(true)
{
 ;
}

Fun4AllRUSEventOutputManager::~Fun4AllRUSEventOutputManager() {
    CloseFile();
}

int Fun4AllRUSEventOutputManager::OpenFile(PHCompositeNode* startNode) {
	std::cout << "Fun4AllRUSEventOutputManager::OpenFile(): Attempting to open file: " << m_file_name << " with tree: " << m_tree_name << std::endl;
	m_file = new TFile(m_file_name.c_str(), "RECREATE");

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
	m_tree->Branch("turnID", &turnID, "turnID/I");
	m_tree->Branch("rfIntensities", rfIntensities, "rfIntensities[33]/I");
	m_tree->Branch("fpgaTriggers", fpgaTriggers, "fpgaTriggers[5]/I");
	m_tree->Branch("nimTriggers", nimTriggers, "nimTriggers[5]/I");

	m_tree->Branch("detectorIDs", &detectorIDs);
	m_tree->Branch("elementIDs", &elementIDs);
	m_tree->Branch("tdcTimes", &tdcTimes);
	m_tree->Branch("driftDistances", &driftDistances);
	m_tree->Branch("hitsInTime", &hitsInTime);

	m_tree->Branch("triggerDetectorIDs", &triggerDetectorIDs);
	m_tree->Branch("triggerElementIDs", &triggerElementIDs);
	m_tree->Branch("triggerTdcTimes", &triggerTdcTimes);
	m_tree->Branch("triggerDriftDistances", &triggerDriftDistances);
	m_tree->Branch("triggerHitsInTime", &triggerHitsInTime);

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


cout << "============Reco Mode:  "<< reco_mode << endl;

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

		m_tree->Branch("mu_plus_chi2_target", &mu_plus_chi2_target);
		m_tree->Branch("mu_plus_chi2_dump", &mu_plus_chi2_dump);
		m_tree->Branch("mu_plus_chi2_upstream", &mu_plus_chi2_upstream);

		m_tree->Branch("mu_minus_chi2_target", &mu_minus_chi2_target);
		m_tree->Branch("mu_minus_chi2_dump", &mu_minus_chi2_dump);
		m_tree->Branch("mu_minus_chi2_upstream", &mu_minus_chi2_upstream);

		m_tree->Branch("top_bot", &top_bot);
		m_tree->Branch("bot_top", &bot_top);
	}

	if(mc_mode ==true) m_vec_trk  = findNode::getClass<SQTrackVector >(startNode, "SQTruthTrackVector");
	if(reco_mode ==true) m_srec       = findNode::getClass<SRecEvent  >(startNode, "SRecEvent");
	m_evt = findNode::getClass<SQEvent>(startNode, "SQEvent");
	m_hit_vec = findNode::getClass<SQHitVector>(startNode, "SQHitVector");
	m_trig_hit_vec = findNode::getClass<SQHitVector>(startNode, "SQTriggerHitVector");

	if (!m_evt || !m_hit_vec || !m_trig_hit_vec) return Fun4AllReturnCodes::ABORTEVENT;


	if(reco_mode ==true && data_trig_mode ==true){
		SQRun* sq_run = findNode::getClass<SQRun>(startNode, "SQRun");
		if (!sq_run) return Fun4AllReturnCodes::ABORTEVENT;
		int LBtop = sq_run->get_v1495_id(2);
		int LBbot = sq_run->get_v1495_id(3);
		int ret = m_rs.LoadConfig(LBtop, LBbot);
		if (ret != 0) {
			cout << "!!WARNING!!  OnlMonTrigEP::InitRunOnlMon():  roadset.LoadConfig returned " << ret << ".\n";
		}
		cout <<"Roadset " << m_rs.str(1) << endl;

	}

	if(reco_mode ==true && mc_trig_mode ==true){
		int ret = m_rs.LoadConfig(131);
		if (ret != 0) {
			cout << "!!WARNING!!  OnlMonTrigEP::InitRunOnlMon():  roadset.LoadConfig returned " << ret << ".\n";
		}
		cout <<"Roadset " << m_rs.str(1) << endl;
	}


	return Fun4AllReturnCodes::EVENT_OK;
}
int Fun4AllRUSEventOutputManager::Write(PHCompositeNode* startNode) {
	if (!m_file || !m_tree) {
		OpenFile(startNode);
	}
	ResetBranches();
	runID = m_evt->get_run_id();
	spillID = m_evt->get_spill_id();
	rfID = m_evt->get_qie_rf_id();
	eventID = m_evt->get_event_id();
	turnID = m_evt->get_qie_turn_id();

	fpgaTriggers[0] = m_evt->get_trigger(SQEvent::MATRIX1);
	fpgaTriggers[1] = m_evt->get_trigger(SQEvent::MATRIX2);
	fpgaTriggers[2] = m_evt->get_trigger(SQEvent::MATRIX3);
	fpgaTriggers[3] = m_evt->get_trigger(SQEvent::MATRIX4);
	fpgaTriggers[4] = m_evt->get_trigger(SQEvent::MATRIX5);

	nimTriggers[0] = m_evt->get_trigger(SQEvent::NIM1);
	nimTriggers[1] = m_evt->get_trigger(SQEvent::NIM2);
	nimTriggers[2] = m_evt->get_trigger(SQEvent::NIM3);
	nimTriggers[3] = m_evt->get_trigger(SQEvent::NIM4);
	nimTriggers[4] = m_evt->get_trigger(SQEvent::NIM5);
	for (int i = -16; i < 16; ++i) {
    // cout << "intensity index: i" << i+16 << endl;
    	rfIntensities[i + 16] = m_evt->get_qie_rf_intensity(i);
}

if (m_hit_vec) {
    for (int ihit = 0; ihit < m_hit_vec->size(); ++ihit) {
        SQHit* hit = m_hit_vec->at(ihit);
        detectorIDs.push_back(hit->get_detector_id());
        elementIDs.push_back(hit->get_element_id());
        tdcTimes.push_back(hit->get_tdc_time());
        driftDistances.push_back(hit->get_drift_distance());
        // cout << "get drift distance: " << hit->get_drift_distance() << endl;
        hitsInTime.push_back(hit->is_in_time());
    }
}

if (m_trig_hit_vec) {
    for (int ihit = 0; ihit < m_trig_hit_vec->size(); ++ihit) {
        SQHit* hit = m_trig_hit_vec->at(ihit);
        triggerDetectorIDs.push_back(hit->get_detector_id());
        triggerElementIDs.push_back(hit->get_element_id());
        triggerTdcTimes.push_back(hit->get_tdc_time());
        triggerDriftDistances.push_back(hit->get_drift_distance());
        triggerHitsInTime.push_back(hit->is_in_time());
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
	int n_dim = m_srec->getNDimuons();
	for (int i_dim = 0; i_dim < n_dim; i_dim++) {
		SRecDimuon sdim = m_srec->getDimuon(i_dim);
		SRecTrack trk_pos = m_srec->getTrack(sdim.get_track_id_pos());
		SRecTrack trk_neg = m_srec->getTrack(sdim.get_track_id_neg());
		// Fill dimuon vectors
		dimuon_vtx_x.push_back(sdim.get_pos().X());
		dimuon_vtx_y.push_back(sdim.get_pos().Y());
		dimuon_vtx_z.push_back(sdim.get_pos().Z());
		dimuon_vtx_px.push_back(sdim.get_mom().Px());
		dimuon_vtx_py.push_back(sdim.get_mom().Py());
		dimuon_vtx_pz.push_back(sdim.get_mom().Pz());
		//cout << "sdim mass "<< sdim.get_mom().M() <<endl;
		//cout << "sdim mass 2 "<<sdim.mass <<endl;
		//cout << "sdim x1 "<<sdim.x1 <<endl;
		//cout << "sdim x2 "<< sdim.x2<<endl;
		dimuon_mass.push_back(sdim.get_mom().M());
		dimuon_x1.push_back(sdim.x1);
		dimuon_x2.push_back(sdim.x2);
		dimuon_xF.push_back(sdim.xF);
		// Fill mu_plus vectors
		mu_plus_vtx_x.push_back(trk_pos.get_pos_vtx().X());
		mu_plus_vtx_y.push_back(trk_pos.get_pos_vtx().Y());
		mu_plus_vtx_z.push_back(trk_pos.get_pos_vtx().Z());
		// Fill mu_minus vectors
		mu_minus_vtx_x.push_back(trk_neg.get_pos_vtx().X());
		mu_minus_vtx_y.push_back(trk_neg.get_pos_vtx().Y());
		mu_minus_vtx_z.push_back(trk_neg.get_pos_vtx().Z());
		// Fill chi-squared values for the positive track
		mu_plus_chi2_target.push_back(trk_pos.get_chisq_target());
		mu_plus_chi2_dump.push_back(trk_pos.get_chisq_dump());
		mu_plus_chi2_upstream.push_back(trk_pos.get_chisq_upstream());
		// Fill chi-squared values for the negative track
		mu_minus_chi2_target.push_back(trk_neg.get_chisq_target());
		mu_minus_chi2_dump.push_back(trk_neg.get_chisq_dump());
		mu_minus_chi2_upstream.push_back(trk_neg.get_chisq_upstream());
		// Fill boolean road match conditions
		bool pos_top = m_rs.PosTop()->FindRoad(trk_pos.getTriggerRoad());
		bool pos_bot = m_rs.PosBot()->FindRoad(trk_pos.getTriggerRoad());
		bool neg_top = m_rs.NegTop()->FindRoad(trk_neg.getTriggerRoad());
		bool neg_bot = m_rs.NegBot()->FindRoad(trk_neg.getTriggerRoad());
		bool top_bot_ = pos_top && neg_bot;
		bool bot_top_ =  pos_bot && neg_top;
		top_bot.push_back(top_bot_);
		bot_top.push_back(bot_top_);
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
	detectorIDs.clear();
	elementIDs.clear();
	tdcTimes.clear();
	driftDistances.clear();
	hitsInTime.clear();

	triggerDetectorIDs.clear();
	triggerElementIDs.clear();
	triggerTdcTimes.clear();
	triggerDriftDistances.clear();
	triggerHitsInTime.clear();

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

	mu_plus_chi2_target.clear();
	mu_plus_chi2_dump.clear();
	mu_plus_chi2_upstream.clear();

	mu_minus_chi2_target.clear();
	mu_minus_chi2_dump.clear();
	mu_minus_chi2_upstream.clear();

	top_bot.clear();
	bot_top.clear();

}

