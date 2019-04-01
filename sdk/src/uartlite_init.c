/*
 * uartlite_init.c
 *
 *  Created on: Jan 23, 2019
 *      Author: Ainstein Zhenyu Hu
 */

#include "main.h"
#include "xparameters.h"
#include "xuartlite.h"
#include "stdio.h"
#include "xstatus.h"

int uartlite_initialization(){
	int Status;

	// UART-0
	Status = XUartLite_Initialize(&UartLite_0, XPAR_AXI_UARTLITE_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	// UART-1
	Status = XUartLite_Initialize(&UartLite_1, XPAR_AXI_UARTLITE_1_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	// UART-2
	Status = XUartLite_Initialize(&UartLite_2, XPAR_AXI_UARTLITE_2_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	// UART-3
	Status = XUartLite_Initialize(&UartLite_3, XPAR_AXI_UARTLITE_3_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	// UART-4
	Status = XUartLite_Initialize(&UartLite_4, XPAR_AXI_UARTLITE_4_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

//	printf("uart initialization successful ! \n\r");

//	unsigned int ReceivedCount = 0;
	// self-test
	Status = XUartLite_SelfTest(&UartLite_0);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Status = XUartLite_SelfTest(&UartLite_1);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Status = XUartLite_SelfTest(&UartLite_2);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Status = XUartLite_SelfTest(&UartLite_3);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Status = XUartLite_SelfTest(&UartLite_4);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	return Status;
}
