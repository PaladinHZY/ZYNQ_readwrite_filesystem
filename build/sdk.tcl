sdk setws ../sdk/
sdk createhw -name ZYNQ_File_Operation_hw_platform -hwspec ../sdk/ZYNQ_File_Operation.hdf
createbsp -name ZYNQ_File_Operation_bsp -hwproject ZYNQ_File_Operation_hw_platform -proc ps7_cortexa9_0 -os standalone
setlib -bsp ZYNQ_File_Operation_bsp -lib xilffs
regenbsp -bsp ZYNQ_File_Operation_bsp
createapp -name ZYNQ_File_Operation -bsp ZYNQ_File_Operation_bsp -app {Empty Application} -hwproject ZYNQ_File_Operation_hw_platform -proc ps7_cortexa9_0
importprojects ../sdk/ZYNQ_File_Operation
