#ifndef _FUN4ALL_UNIVERSAL_OUTPUT_MANAGER__H_
#define _FUN4ALL_UNIVERSAL_OUTPUT_MANAGER__H_

#include <fun4all/Fun4AllOutputManager.h>
#include <string>
#include <vector>
#include <map>
#include <TStopwatch.h>
#include <UtilAna/TrigRoadset.h>

class TFile;
class TTree;
class PHCompositeNode;
class SQEvent;
class SQSpillMap;
class SQHitVector;
class SQTrackVector;
class SRecEvent;


class Fun4AllRUSEventOutputManager : public Fun4AllOutputManager {
public:
    Fun4AllRUSEventOutputManager(const std::string &myname = "UNIVERSALOUT");
    virtual ~Fun4AllRUSEventOutputManager();

    void SetTreeName(const std::string& name) { m_tree_name = name; }
    void SetFileName(const std::string& name) { m_file_name = name; }
    virtual int Write(PHCompositeNode* startNode);
    void ResetBranches();
    void SetMCMode(bool enable) { mc_mode = enable; }
    void SetRecoMode(bool enable) { reco_mode = enable; }
    void SetDataTriggerEmu(bool enable) { data_trig_mode = enable; }
    void SetMCTriggerEmu(bool enable) { mc_trig_mode = enable; }
protected:
    int OpenFile(PHCompositeNode* startNode);
    void CloseFile();

private:
    std::string m_tree_name;
    std::string m_file_name;
    std::string m_dir_base;
    bool m_dimuon_mode;
   
    TFile* m_file;
    TTree* m_tree;

    SQEvent* m_evt;
    SQSpillMap* m_sp_map;
    SQHitVector* m_hit_vec;
    SQHitVector* m_trig_hit_vec;
    SQTrackVector * m_vec_trk;
    SRecEvent*   m_srec;

UtilTrigger::TrigRoadset m_rs;
bool mc_mode;
bool reco_mode;
bool data_trig_mode;
bool mc_trig_mode;

int runID;
int spillID;
int eventID;
int rfID;
int turnID;
int rfIntensities[33];
int fpgaTriggers[5] = {0};
int nimTriggers[5] = {0};

std::vector<int> detectorIDs;
std::vector<int> elementIDs;
std::vector<double> tdcTimes;
std::vector<double> driftDistances;
std::vector<bool> hitsInTime;

std::vector<int> triggerDetectorIDs;
std::vector<int> triggerElementIDs;
std::vector<double> triggerTdcTimes;
std::vector<double> triggerDriftDistances;
std::vector<bool> triggerHitsInTime;

// MC track data
std::vector<int> mc_track_charges;
std::vector<int> mc_track_id;
std::vector<float> mc_pos_vtx_x;
std::vector<float> mc_pos_vtx_y;
std::vector<float> mc_pos_vtx_z;
std::vector<float> mc_mom_vtx_px;
std::vector<float> mc_mom_vtx_py;
std::vector<float> mc_mom_vtx_pz;

//Dimuon variables for MC or Data
std::vector<double> dimuon_vtx_x;
std::vector<double> dimuon_vtx_y;
std::vector<double> dimuon_vtx_z;
std::vector<double> dimuon_vtx_px;
std::vector<double> dimuon_vtx_py;
std::vector<double> dimuon_vtx_pz;
std::vector<double> dimuon_mass;
std::vector<double> dimuon_x1;
std::vector<double> dimuon_x2;
std::vector<double> dimuon_xF;

std::vector<double> mu_plus_vtx_x;
std::vector<double> mu_plus_vtx_y;
std::vector<double> mu_plus_vtx_z;
std::vector<double> mu_minus_vtx_x;
std::vector<double> mu_minus_vtx_y;
std::vector<double> mu_minus_vtx_z;

std::vector<double> mu_plus_chi2_target;
std::vector<double> mu_plus_chi2_dump;
std::vector<double> mu_plus_chi2_upstream;

std::vector<double> mu_minus_chi2_target;
std::vector<double> mu_minus_chi2_dump;
std::vector<double> mu_minus_chi2_upstream;

std::vector<bool> top_bot;
std::vector<bool> bot_top;
};

#endif /* _FUN4ALL_VECT_EVENT_OUTPUT_MANAGER__H_ */
