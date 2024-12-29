## Fun4AllRUSEventInputManager
The `Fun4AllRUSEventInputManager` is an input file manager within the Fun4All framework, designed to handle and register various types of input data, including triggers, hits, true track information, and event details, from a ROOT file. It supports a wide range of data formats, such as integers, doubles, and booleans, and can store this data in flat, vector, or array formats. 

## Fun4AllRUSEventOutputManager
The `Fun4AllRUSEventOutputManager` is an output file manager within the Fun4All framework, designed to handle and register various types of input data, including triggers, hits, true track information, and event details, from a ROOT file. It supports a wide range of data formats, such as integers, doubles, and booleans, and can store this data in flat, vector, or array formats. 

## Goal
The Fun4All framework does not support random indexing in the Fun4All event processing server. Therefore, translating the data into a convenient format enables us to perform pre- and post-processing using the input/output managers.

# Updated Variable Names and Types

## Event-Level Variables
| Variable Name      | Type               | Description                          |
|--------------------|--------------------|--------------------------------------|
| `eventID`          | `int`              | Unique identifier for the event      |
| `runID`            | `int`              | Identifier for the current run       |
| `spillID`          | `int`              | Identifier for the spill in the run  |
| `fpgaTriggers`     | `int[5]`           | Array of FPGA trigger counts         |
| `nimTriggers`      | `int[5]`           | Array of NIM trigger counts          |
| `rfIntensities`    | `int[33]`          | Array for QIE RF intensities         |

## Hit-Level Variables
| Variable Name           | Type                     | Description                                  |
|-------------------------|--------------------------|----------------------------------------------|
| `detectorIDs`           | `std::vector<int>`       | Detector IDs for all hits                    |
| `elementIDs`            | `std::vector<int>`       | Element IDs associated with each hit         |
| `driftDistances`        | `std::vector<double>`    | Drift distances for each hit                 |
| `tdcTimes`              | `std::vector<double>`    | TDC timing values for each hit               |
| `hitInTime`             | `std::vector<bool>`      | Flags indicating if hits are within time     |

## Trigger-Level Hit Variables
| Variable Name              | Type                     | Description                                  |
|----------------------------|--------------------------|----------------------------------------------|
| `trigDetectorIDs`           | `std::vector<int>`       | Detector IDs for triggered hits              |
| `trigElementIDs`            | `std::vector<int>`       | Element IDs for triggered hits               |
| `trigDriftDistances`        | `std::vector<double>`    | Drift distances for triggered hits           |
| `trigTdcTimes`              | `std::vector<double>`    | TDC timing values for triggered hits         |
| `trigHitInTimes`            | `std::vector<bool>`      | Flags indicating if triggered hits are in    |
|			     |				|   time					       |		

## Track-Level Variables
| Variable Name              | Type                     | Description                                  |
|----------------------------|--------------------------|----------------------------------------------|
| `mc_track_id`              | `std::vector<int>`       | Track IDs for Monte Carlo tracks             |
| `mc_track_charges`         | `std::vector<int>`       | Charges of the Monte Carlo tracks            |
| `mc_pos_vtx_x`             | `std::vector<double>`    | X-coordinate of the production vertex        |
| `mc_pos_vtx_y`             | `std::vector<double>`    | Y-coordinate of the production vertex        |
| `mc_pos_vtx_z`             | `std::vector<double>`    | Z-coordinate of the production vertex        |
| `mc_mom_vtx_px`            | `std::vector<double>`    | X-component of the momentum at the vertex    |
| `mc_mom_vtx_py`            | `std::vector<double>`    | Y-component of the momentum at the vertex    |
| `mc_mom_vtx_pz`            | `std::vector<double>`    | Z-component of the momentum at the vertex    |

``` Compilation
source setup.sh
cmake-this
make-this
```

### Uses

1. **Convert the DST file to a vector file**  
   If you'd like to convert the DST file to a vector format for easier event processing, use the following commands:

   ```bash
   cd Convert
   root -b ConvertToVect.C
   ```
2. **Run the Pre- or Post-Process Data in the Fun4All Server**

   After completing the event processing, if you would like to add the triggers, hits, and event information into the Fun4All framework's event processing server, follow these steps:

   Go to the `work_reco` directory, where you will find two subdirectories:
     Run `Fun4Sim.C` and make sure to use the correct input file in DST format. (This directory was created to verify that both input file systems yield the same results in the output.)

### Grid uses

Will be added

