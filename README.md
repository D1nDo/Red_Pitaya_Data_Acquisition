# Red Pitaya Data Acquisition for STEMlab 125-14

This repository provides a ready-to-use data acquisition system for the Red Pitaya STEMlab 125-14 board (tested on Linux OS 2.04), utilizing both ADC channels for versatile measurements. The system requiring minimal configuration.

The system supports two acquisition rates: **5 MSa/s** (high-speed) and **500 KSa/s** (low-speed).  

The system acquires data from both ADCs.

## Repository Contents

- **Bitstream files** for the two acquisition modes:  
  - `Data_acquisition_5MSa.bit.bin` – high-speed acquisition (5 MSa/s)  
  - `Data_acquisition_500KSa.bit.bin` – low-speed acquisition (500 KSa/s)  

- **Cpp files** for data acquisition:  
  - `Data_acquisition.cpp` 

- **Data plotting scripts**:  
  - `plot_data.m` – MATLAB script implementing the plotting algorithm  
  - `plot_data.py` – Python version of the same algorithm  

These scripts allow easy visualization and analysis of the data acquired from the ADC channels.

## Installation / Setup

Suppose you want to use the "high-speed acquisition" (replace `rp-******` with your board's hostname):

### High-Speed Acquisition 
1. **Downlad the Repository** 
2. **Connect your Red Pitaya** to your PC via Ethernet.
3. **Transfer the bitstream and executable** to the board using SCP:
```scp Data_acquisition_5MSa.bit.bin root@rp-******.local:/root``` and ```Data_acquisition.cpp root@rp-******.local:/root```
4. **Access the Red Pitaya terminal** via SSH: ```ssh root@rp-******.local```
5. **Load the bitstream** onto the Zynq-7010 SoC: fpgautil -b Data_acquisition_5MSa.bit.bin
6. **Compile the cpp code** : g++ Data_acquisition.cpp -o Data_acquisition
7. **Run the executable** using ```./Data_acquisition```
8. **Insert the number of samples you want to acquire and press Enter** 
9. **Download the acquired data** `external_data.bin` to your PC: ```scp root@rp-******.local:/tmp/external_data.bin ./```
10. **Plot your data acquisition** using the MATLAB or Python codes.


For the low-speed data acquisition repeat the same procedure using the `Data_acquisition_500KSa.bit.bin` file, using the same cpp code.

**Technical note** : All the firmwares come with an anti-aliasing Low Pass Filter with -3db at Nyquist freqeuncy.

## Motivaions
I created this small project since, while Red Pitaya offers official tools for data acquisition, these options have some practical limitations. (The data logger apps support only up to 16,384 samples, which can be restrictive for longer measurements or for experiments requiring different acquisition rates).

This project aims to address these limitations by allowing dual-frequency acquisition: a high-rate mode at 5 MSa/s for faster signals, and a low-rate mode at 500 kSa/s for slower processes. It also supports longer acquisitions of at least 500,000 samples, thanks to the use of DMA with scatter/gather, extending the capabilities provided by the standard Red Pitaya tools.

If you have any question or aim to contact me please send an e-mail to : dindelli.ludovico@gmail.com
