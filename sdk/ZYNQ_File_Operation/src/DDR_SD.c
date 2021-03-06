/*
 * DDR_SD.c
 *
 *  Created on: Jul 10, 2018
 *      Author: Ainstein Zhenyu Hu
 */


#include "DDR_SD.h"
#include "stdio.h"
#include "main.h"
#include "string.h"
#include "stdlib.h"

int FfsSdPolledExample(u32  byte_number, int file_number)
{
	FRESULT Res;
	UINT NumBytesRead;
	UINT NumBytesWritten;
	u32 BuffCnt;
	u32 Byte_number_input = byte_number;
	u32 FileSize = Byte_number_input; /// 10 Bytes of data
//	char FileName[64] = "10sass00.bin";
	/*
	 * To test logical drive 0, Path should be "0:/"
	 * For logical drive 1, Path should be "1:/"
	 */
	TCHAR *Path = "0:/";

	Platform = XGetPlatform_Info();
	if (Platform == XPLAT_ZYNQ_ULTRA_MP) {
		/*
		 * Since 8MB in Emulation Platform taking long time, reduced
		 * file size to 8KB.
		 */
		FileSize = 16*1024;
	}

	for(BuffCnt = 0; BuffCnt < FileSize; BuffCnt++){
		SourceAddress[BuffCnt] = Xil_In8(SendBuffer+BuffCnt);
	}

	/*
	 * Register volume work area, initialize device
	 */
	Res = f_mount(&fatfs, Path, 0);

	if (Res != FR_OK) {
		return XST_FAILURE;
	}

	/*
	 * Path - Path to logical driver, 0 - FDISK format.
	 * 0 - Cluster size is automatically determined based on Vol size.
	 */
//	Res = f_mkfs(Path, 0, 0);
//	if (Res != FR_OK) {
//		return XST_FAILURE;
//	}

	/*
	 * Open file with required permissions.
	 * Here - Creating new file with read/write permissions. .
	 * To open file with write permissions, file system should not
	 * be in Read Only mode.
	 */
	sprintf(FileName, "TEST%d", file_number); //
//	snprintf(FileName, sizeof(char) * 32, "T_%d", file_number);

	SD_File = (char *)FileName;

	Res = f_open(&fil, SD_File, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if (Res) {
		return XST_FAILURE;
	}

	/*
	 * Pointer to beginning of file .
	 */
//	Res = f_lseek(&fil, 0);
//	if (Res) {
//		return XST_FAILURE;
//	}

	/*
	 * Write data to file.
	 */
	Res = f_write(&fil, (const void*)SourceAddress, FileSize,
			&NumBytesWritten);
	if (Res) {
		return XST_FAILURE;
	}

//	/*
//	 * Pointer to beginning of file .
//	 */
//	Res = f_lseek(&fil, 0);
//	if (Res) {
//		return XST_FAILURE;
//	}
//
//	/*
//	 * Read data from file.
//	 */
//	Res = f_read(&fil, (void*)DestinationAddress, FileSize,
//			&NumBytesRead);
//	if (Res) {
//		return XST_FAILURE;
//	}
//
//	/*
//	 * Data verification
//	 */
//	for(BuffCnt = 0; BuffCnt < FileSize; BuffCnt++){
//		if(SourceAddress[BuffCnt] != DestinationAddress[BuffCnt]){
//			return XST_FAILURE;
//		}
//	}

	/*
	 * Close file.
	 */
	Res = f_close(&fil);
	if (Res) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}
