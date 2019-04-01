/*
 * main.c
 *
 *  Created on: Jan 23, 2019
 *      Author: Ainstein Zhenyu Hu
 */


#include "unistd.h"
#include <stdio.h>
//#include "platform.h"

#include "xparameters.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"

#include "xuartlite.h"
#include "main.h"
#include "DDR_SD.h"


#define FILE "config.txt"
/// main function

int main(){
	int Status;
	int byte_number;
	byte_number = TX_BUFFER_SIZE;
	int file_number = 0;
	int uart_number;
	// Initialize UARTLite
	Status = uartlite_initialization();
	if (Status != XST_SUCCESS) {
		printf("uart initialization failed ! \n\r");
		return XST_FAILURE;
	}
	printf("uart initialization successful ! \n\r");
//	sd_write("CONFIG.txt");
	uart_number = sd_read("CONFIG.txt");   /// uart_number --> ascii


	while (1){
		uart_trx_example(uart_number);
		Status = FfsSdPolledExample(byte_number, file_number);
//		usleep(10000);
		file_number++;
		printf("%d\n\r", file_number);
	}
}


