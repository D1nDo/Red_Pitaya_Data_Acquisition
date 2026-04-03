# Red_Pitaya_Data_Acquisition
Red Pitaya Data Acquisition for STEMlab 125-14
This repository provides all necessary files to use the Red Pitaya STEMlab 125-14 board with Linux as a data acquisition system via its two ADC channels.
The system supports two acquisition rates: 1 MSa/s and 1 KSa/s.
Repository Contents
Bitstream files for both acquisition modes:
Data_acquisition_MSa.bit.bin – for high-speed acquisition (1 MSa/s)
Data_acquisition_KSa.bit.bin – for low-speed acquisition (1 KSa/s)
Executable files for data acquisition:
Data_acquisition_MSa.exe
Data_acquisition_KSa.exe
Data plotting scripts:
plot_data.m – MATLAB script implementing the data visualization algorithm
plot_data.py – Python version of the same algorithm
These scripts allow easy visualization and analysis of the data acquired from the ADC channels.
