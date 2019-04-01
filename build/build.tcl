open_project ZYNQ_File_Operation/ZYNQ_File_Operation.xpr
launch_runs impl_1 -to_step write_bitstream
wait_on_run impl_1 
write_hwdef -force ../sdk/ZYNQ_File_Operation.hdf
