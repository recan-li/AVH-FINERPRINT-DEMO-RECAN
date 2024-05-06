
# Introduce

This is an ARM based AVH mirror simulation Cortex-M7 chip platform for building an finger print matching demo.

# Project file structure

```sh
├── AVH-CM7
│   ├── avh.yml 								# yml config file
│   ├── Board 									# Board files
│   ├── build.py 								# Build script
│   ├── LICENSE 		 						# LICENCE file for AVH-CM7				
│   ├── mdk_config.txt 							# MDK config file
│   ├── README.md 								# README for AVH-CM7 project
│   ├── requirements.txt 						# python requirements files
│   ├── RTE 									# RTE files
│   ├── vht_config.txt 							# VHT config file
│   ├── VHT_MPS2_Cortex-M7.cprj 				# MDK project file
│   ├── VHT_MPS2_Cortex-M7.uvoptx 				# MDK project file
│   └── VHT_MPS2_Cortex-M7.uvprojx 				# MDK project file
├── cmake
│   └── CMSIS-Build-Utils.cmake 				# Cmake config file
├── fvs-0.1.1-master
│   ├── build.sh 								# Build script for fvs-0.1.1-master project only
│   ├── fingerprint_bitmaps 					# Test fingerprint_bitmaps
│   ├── include 								# Include files for fvs-0.1.1-master component
│   └── src 									# Source files for fvs-0.1.1-master component
│   └── demos 									# Some demos for fvs-0.1.1-master component
├── LICENSE 									# The LICENSE for this project
├── Makefile									# Makefile for project building
└── README.md									# This readme file
```

# Steps for test this demo

1. Clone this project from github.
2. Read this README file and `make help` to get more help msg.
3. Install some other tools, such as cbuild and cpackget.
4. `make all` to build this project, or make it step by step: 
   `make source` -> `make clean` -> `make build` -> `make run`
5. When you see the following logs, you are lucky to run this demo well.

```sh
Running ...
/opt/VHT/bin/FVP_MPS2_Cortex-M7 --stat --simlimit 8000 -f AVH-CM7/vht_config.txt out/image.axf
telnetterminal2: Listening for serial connection on port 5000
telnetterminal1: Listening for serial connection on port 5001
telnetterminal0: Listening for serial connection on port 5002


Begin to match 10 finger print pictures ...
-----------------------------------------------------------
[bmp file 1] vs [bmp file 2] -> score, match result!
-----------------------------------------------------------
[   1-1.bmp] vs [   1-2.bmp] ->   3, Different Fingerprint!
[   1-1.bmp] vs [   1-3.bmp] ->   2, Different Fingerprint!
[   1-1.bmp] vs [   1-4.bmp] ->   3, Different Fingerprint!
[   1-1.bmp] vs [   1-5.bmp] ->   2, Different Fingerprint!
[   1-1.bmp] vs [   1-6.bmp] ->   2, Different Fingerprint!
[   1-1.bmp] vs [   1-7.bmp] ->   2, Different Fingerprint!
[   1-1.bmp] vs [   1-8.bmp] ->   5, Different Fingerprint!
[   1-1.bmp] vs [   1-9.bmp] ->   5, Different Fingerprint!
[   1-1.bmp] vs [  1-10.bmp] ->   5, Different Fingerprint!
[   1-2.bmp] vs [   1-3.bmp] -> 102, Same Fingerprint!
[   1-2.bmp] vs [   1-4.bmp] -> 109, Same Fingerprint!
[   1-2.bmp] vs [   1-5.bmp] -> 106, Same Fingerprint!
[   1-2.bmp] vs [   1-6.bmp] -> 107, Same Fingerprint!
[   1-2.bmp] vs [   1-7.bmp] -> 102, Same Fingerprint!

```
