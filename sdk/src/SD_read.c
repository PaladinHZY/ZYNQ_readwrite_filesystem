

#include "DDR_SD.h"
#include "stdio.h"
#include "main.h"
#include "string.h"
#include "stdlib.h"
#include "xstatus.h"
#include "xil_io.h"
#include "xsdps.h"		/* SD device driver */
#include "xil_printf.h"
#include "ff.h"
#include "xil_cache.h"
#include "xplatform_info.h"


int sd_write(char *filename){
	FRESULT Res;
	u32 FileSize = 1; ///
	UINT NumBytesWritten;
	int SendBufferTest[1];
	SendBufferTest[0] = 49; // ascii -- '1'

	for(u32 i = 0; i < 1; i++){
		SourceAddressTest[i] = Xil_In32(SendBufferTest+i);
	}

	Res = f_mount(&fatfs, "", 0);

	if (Res != FR_OK) {
		return XST_FAILURE;
	}

	Res = f_open(&fil, filename,  FA_CREATE_ALWAYS | FA_WRITE);
	if (Res) {
		return XST_FAILURE;
	}

	Res = f_lseek(&fil, 0);
	if (Res) {
		return XST_FAILURE;
	}

	Res = f_write(&fil, (void*)SourceAddressTest, FileSize,
			&NumBytesWritten);
	if (Res) {
		return XST_FAILURE;
	}
	printf("%d\n\r",*SourceAddressTest);

	Res = f_close(&fil);
	if (Res) {
		return XST_FAILURE;
	}
	printf("File created successfully! \n\r");
	return XST_SUCCESS;
}

int sd_read(char *filename){
	FRESULT Res;
	UINT NumBytesRead;
	u32 FileSize = 1; /// 10 Bytes of data

//	printf("SD read! \n\r");
//	char FileName[64] = "10sass00.bin";
	/*
	 * To test logical drive 0, Path should be "0:/"
	 * For logical drive 1, Path should be "1:/"
	 */
//	TCHAR *Path = "0:/";

	Platform = XGetPlatform_Info();
	if (Platform == XPLAT_ZYNQ_ULTRA_MP) {
		/*
		 * Since 8MB in Emulation Platform taking long time, reduced
		 * file size to 8KB.
		 */
		FileSize = 16*1024;
	}

	/*
	 * Register volume work area, initialize device
	 */
	Res = f_mount(&fatfs, "", 0);

	if (Res != FR_OK) {
		return XST_FAILURE;
	}


	/*
	 * Open file with required permissions.
	 * Here - Creating new file with read/write permissions. .
	 * To open file with write permissions, file system should not
	 * be in Read Only mode.
	 */
//	static char FileName_Read[32] = "CONFIG.txt";
//	SD_File = (char *)FileName_Read;
	Res = f_open(&fil, filename,  FA_READ);
	if (Res) {
		return XST_FAILURE;
	}
//	printf("SD read2! \n\r");

	Res = f_lseek(&fil, 0);
	if (Res) {
		return XST_FAILURE;
	}
	/*
	 * Read data from file.
	 */
	Res = f_read(&fil, (void*)DestinationAddress, FileSize,
			&NumBytesRead);
	if (Res) {
		return XST_FAILURE;
	}
	printf("%d\n\r",*DestinationAddress);
	Res = f_close(&fil);
	if (Res) {
		return XST_FAILURE;
	}
	printf("File read successfully! \n\r");
	return *DestinationAddress;
}


