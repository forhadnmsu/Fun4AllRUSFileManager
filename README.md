## Goal
We are introducing a new file format to the Fun4All server for the convenience of its users. Unlike formats such as DST or the E906 SRaw ROOT file, which require dynamic libraries or proper dictionaries for data read/write operations on another computer, we have introduced a ROOT-based file format called **RUS (ROOT Universal Structure)**. This format simplifies the process by eliminating the need for specific libraries or dictionaries other than ROOT. For example, an analyzer can use **Fun4AllRUSEventOutputManager** to access data from the Fun4All Server in the RUS format, perform any necessary preprocessing, and then register the data through **Fun4AllRUSEventInputManager** for post-processing tasks such as reconstruction, vertexing, and more.


## Fun4AllRUSEventInputManager

The `Fun4AllRUSEventInputManager` is an input file manager within the Fun4All framework, designed to handle and register various types of input data, including triggers, hits, true track information, and event details, from a ROOT file. It supports a wide range of data formats, such as integers, doubles, and booleans, and can store this data in flat, vector, or array formats. Set `SetMCMode` in your macro to `true` if you want to include true track variables in the RUS file for simulated data. 

## Fun4AllRUSEventOutputManager

The `Fun4AllRUSEventOutputManager` is an output file manager within the Fun4All framework, designed to handle and register various types of input data, including triggers, hits, true/reco track or dimuon information, and event details, from a ROOT file. It supports a wide range of data formats, such as integers, doubles, and booleans, and can store this data in flat, vector, or array formats. Set `SetMCMode` in your macro to `true` if you want to include true variables in the RUS file for simulated data.

### Instructions for Adding Reconstructed Variables to RUS File

To include reconstructed variables in your RUS file, follow these steps:

1. **Set the appropriate trigger emulation option:**
   - For simulated data, set `SetMCTriggerEmu` to `true`.
   - For experimental data, set `SetDataTriggerEmu` to `true`.

2. **Enable reconstruction mode:**
   - Set `SetRecoMode` to `true` to add the reconstructed variables.

# Updated Variable Names and Types

## Event-Level Variables
| Variable Name      | Type               | Description                          |
|--------------------|--------------------|--------------------------------------|
| `runID`            | `int`              | Identifier for the current run       |
| `spillID`          | `int`              | Identifier for the spill in the run  |
| `eventID`          | `int`              | Unique identifier for the event      |
| `rfID`             | `int`              | Identifier for the RF                |
| `fpgaTrigger`     | `int[5]`           | Array of FPGA trigger counts         |
| `nimTrigger`      | `int[5]`           | Array of NIM trigger counts          |
| `rfIntensity`    | `int[33]`          | Array for QIE RF intensities         |

## Hit-Level Variables
| Variable Name           | Type                     | Description                                  |
|-------------------------|--------------------------|----------------------------------------------|
| `detectorID`           | `std::vector<int>`       | Detector IDs for all hits                    |
| `elementID`            | `std::vector<int>`       | Element IDs associated with each hit         |
| `driftDistance`        | `std::vector<double>`    | Drift distances for each hit                 |
| `tdcTime`              | `std::vector<double>`    | TDC timing values for each hit               |
| `hitInTime`             | `std::vector<bool>`      | Flags indicating if hits are within time     |

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


