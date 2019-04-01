# ZYNQ_readwrite_filesystem

Prerequisite:
There are some tools and files need to be installed before run scripts and generate project.
1.	Vivado version for this project is 2018.2. If any other version of Vivado, please modify some parameters in "ZYNQ_File_Operation.tcl" file manually.
2.	SDK version for this project is 2018.2.
3.	Under Windows OS, please install Make tool -- "Make for Windows" on "http://gnuwin32.sourceforge.net/packages/make.htm", and restart bash or command line tool.
	Under Linux OS, Make tool should be pre-installed with OS. 
4. ZYBO boards files. Please follow DIGILENT website instruction to install ZYBOZ7 board file on "https://reference.digilentinc.com/reference/software/vivado/board-files?redirect=1".



To regenerate project, please step into 'build' folder and follow the Makefile command. It is verified under Ubuntu and Windows 10.
1. "make setup" to generate Vivado BD project for PL development.
2. "make build" to compile project, generate bitstream file, export hardware file.
3. "make sdk" to generate and import bsp and app into workspace for PS development
