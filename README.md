# Red Pitaya Data Acquisition for STEMlab 125-14

This repository provides all necessary files to use the **Red Pitaya STEMlab 125-14** board with Linux (tested on OS 2.04) as a data acquisition system via its two ADC channels.  

The system supports two acquisition rates: **500 KSa/s** (high-speed) and **1 KSa/s** (low-speed).  

## Repository Contents

- **Bitstream files** for the two acquisition modes:  
  - `Data_acquisition_500KSa.bit.bin` – high-speed acquisition (500 KSa/s)  
  - `Data_acquisition_1KSa.bit.bin` – low-speed acquisition (1 KSa/s)  

- **Cpp files** for data acquisition:  
  - `Data_acquisition_500KSa.cpp`  
  - `Data_acquisition_1KSa.cpp`  

- **Data plotting scripts**:  
  - `plot_data.m` – MATLAB script implementing the plotting algorithm  
  - `plot_data.py` – Python version of the same algorithm  

These scripts allow easy visualization and analysis of the data acquired from the ADC channels.

## Installation / Setup

Suppose you want to use the "high-speed acquisition" (replace `rp-******` with your board's hostname):

### High-Speed Acquisition (1 MSa/s)
1. **Downlad the Repository** 
2. **Connect your Red Pitaya** to your PC via Ethernet.
3. **Transfer the bitstream and executable** to the board using SCP:
```scp Data_acquisition_500KSa.bit.bin root@rp-******.local:/root``` and ```Data_acquisition_500KSa.cpp root@rp-******.local:/root```
4. **Access the Red Pitaya terminal** via SSH: ```ssh root@rp-******.local```
5. **Load the bitstream** onto the Zynq-7010 SoC: fpgautil -b Data_acquisition_MSa.bit.bin
6. **Run the executable** `Data_acquisition_MSa.exe` using ```./Data_acquisition_MSa```
7. **Write the number of samples you want to acquire and press Enter
8.**Download the acquired data** `data.txt` to your PC: ```scp root@rp-******.local:/root/data.txt C:\Users\My_profile\Desktop\data.txt```
9. **Plot your data acquisition** using the MATLAB or Python codes.

## Contact
