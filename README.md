# Red Pitaya Data Acquisition for STEMlab 125-14

This repository provides all necessary files to use the **Red Pitaya STEMlab 125-14** board with Linux as a data acquisition system via its two ADC channels.  

The system supports two acquisition rates: **1 MSa/s** (high-speed) and **1 KSa/s** (low-speed).  

## Repository Contents

- **Bitstream files** for the two acquisition modes:  
  - `Data_acquisition_MSa.bit.bin` – high-speed acquisition (1 MSa/s)  
  - `Data_acquisition_KSa.bit.bin` – low-speed acquisition (1 KSa/s)  

- **Executable files** for data acquisition:  
  - `Data_acquisition_MSa.exe`  
  - `Data_acquisition_KSa.exe`  

- **Data plotting scripts**:  
  - `plot_data.m` – MATLAB script implementing the plotting algorithm  
  - `plot_data.py` – Python version of the same algorithm  

These scripts allow easy visualization and analysis of the data acquired from the ADC channels.

## Installation / Setup

1. Clone the repository:
```bash
git clone https://github.com/username/repository.git
cd repository
