#ifndef _FUN4ALL_UNIVERSAL_OUTPUT_MANAGER__H_
#define _FUN4ALL_UNIVERSAL_OUTPUT_MANAGER__H_

#include <fun4all/Fun4AllOutputManager.h>
#include <string>
#include <vector>
#include <map>
#include <TStopwatch.h>

class TFile;
class TTree;
class PHCompositeNode;
class SQEvent;
class SQSpillMap;
class SQHitVector;
class SQTrackVector;
class SQTrackVector;
class SQDimuonVector;
class SQRun;

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

    void SetSaveOnlyDimuon(bool enable) { saveDimuonOnly = enable; }
    void SetBasketSize(int size) { m_basket_size = size; }
    void SetAutoFlush(int flush) { m_auto_flush = flush; }
    void SetCompressionLevel(int level) { m_compression_level = level; }

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
    SQTrackVector * m_vec_trk;
    SQTrackVector*  m_sq_trk_vec;
    SQDimuonVector* m_sq_dim_vec;
    SQRun* sq_run;  

int m_basket_size;
int m_auto_flush;
int m_compression_level;

bool mc_mode;
bool reco_mode;
bool saveDimuonOnly;

int runID;
int spillID;
int eventID;
int rfID;
//int turnID;
int rfIntensity[33];
int fpgaTrigger[5] = {0};
int nimTrigger[5] = {0};

std::vector<int> detectorID;
std::vector<int> elementID;
std::vector<double> tdcTime;
std::vector<double> driftDistance;
std::vector<bool> hitsInTime;
// MC track data
std::vector<int> mc_track_charges;
std::vector<int> mc_track_id;
std::vector<double> mc_pos_vtx_x;
std::vector<double> mc_pos_vtx_y;
std::vector<double> mc_pos_vtx_z;
std::vector<double> mc_mom_vtx_px;
std::vector<double> mc_mom_vtx_py;
std::vector<double> mc_mom_vtx_pz;

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

std::vector<double> mu_plus_vtx_px;
std::vector<double> mu_plus_vtx_py;
std::vector<double> mu_plus_vtx_pz;
std::vector<double> mu_minus_vtx_px;
std::vector<double> mu_minus_vtx_py;
std::vector<double> mu_minus_vtx_pz;

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

