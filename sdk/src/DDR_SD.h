

#include "xparameters.h"	/* SDK generated parameters */
#include "xsdps.h"		/* SD device driver */
#include "xil_printf.h"
#include "ff.h"
#include "xil_cache.h"
#include "xplatform_info.h"

#include "ffconf.h"
#include "xil_io.h"

#include "diskio.h"
#include "xil_types.h"


#define BYTE_NUMBER 128
#define DDR_BASEARDDR      XPAR_DDR_MEM_BASEADDR + 0x04000000


/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
int FfsSdPolledExample(u32  byte_number, int file_number);

/************************** Variable Definitions *****************************/
FIL fil;		/* File object */
FATFS fatfs;
/*
 * To test logical drive 0, FileName should be "0:/<File name>" or
 * "<file_name>". For logical drive 1, FileName should be "1:/<file_name>"
 */
//static char FileName_Common[32] = "Test.bin";
char FileName[32];
char *SD_File;
u32 Platform;


#ifdef __ICCARM__
#pragma data_alignment = 32
u8 DestinationAddress[(BYTE_NUMBER+4)*1024*1024];
u8 SourceAddress[(BYTE_NUMBER+4)*1024*1024];
#pragma data_alignment = 4
#else
u8 DestinationAddress[32] __attribute__ ((aligned(32)));
//u8 SourceAddress[(BYTE_NUMBER+4)*1024*1024] __attribute__ ((aligned(32)));

u8 SourceAddress[10] __attribute__ ((aligned(32)));
u32 SourceAddressTest[1] __attribute__ ((aligned(32)));


#endif

#define TEST 7




