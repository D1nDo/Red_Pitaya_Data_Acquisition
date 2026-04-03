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

Suppose you want to use the "high-speed acquisition" (replace `rp-******` with your board's hostname):

1. **Downlad the Repository 
2. **Connect your Red Pitaya StemLab 125-14 to your pc using the ethernet cable
3. **From your PC terminal, load `Data_acquisition_MSa.bit.bin` and the `Data_acquisition_MSa.exe` on the board using Linux commands:
```scp Data_acquisition_MSa.bit.bin root@rp-******.local:/root``` and ```Data_acquisition_MSa.exe root@rp-******.local:/root```
4. **Enter on the Red Pitaya terminal digiting in your PC terminal: ```ssh root@rp-******.local```
5. **Charge the bitstream file on the Zynq7010 SoC using: fpgautil -b Data_acquisition_MSa.bit.bin
6. **Execute the `Data_acquisition_MSa.exe` using ```./Data_acquisition_MSa```
7. **Write the number of samples you want to acquire and press Enter
8. **To scaricare the data.txt file exit from the ssh window and execute on your terminal: ```scp root@rp-******.local:/root/data.txt C:\Users\My_profile\Desktop\data.txt```
9. **Use the Python or MATLAB scripts to plot your data acquisition.
