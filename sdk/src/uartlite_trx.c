/*
 * uartlite_trx.c
 *
 *  Created on: Jan 23, 2019
 *      Author: Ainstein Zhenyu Hu
 */


#include "main.h"
#include "xparameters.h"
#include "xuartlite.h"
#include "stdio.h"
#include "xstatus.h"
#include "xuartlite_l.h"

void uart_oneradar_lidar(){
	int tx_checksum;
	int k_0;
	int lidar_header;
	int laser_data;
	int decimal_separator;
	int lidar_tail;
	lidar_header = 0;
	lidar_tail = 0;
	decimal_separator = 0;

	// checksum number reset
	for (int i = 0; i < 1; i++){
		*(rx_chk + i) = 0;
	}
	for (int i = 0; i < RX_BUFFER_SIZE; i++){
		ReceiveBuffer_0[i] = 0;
	}

	// Receive data from radar device 0,1,2
	for (int i = 0; i < RX_BUFFER_SIZE; i++){
		ReceiveBuffer_0[i] = XUartLite_RecvByte(XPAR_UARTLITE_0_BASEADDR);
	}

	// Receive data packets from laser device
	for (int i = 0; i < RX_LASER_BUFFER_SIZE; i++){
		ReceiveBuffer_4[i] = 0;
	}
	for (int i = 0; i < RX_LASER_BUFFER_SIZE; i++){
		ReceiveBuffer_4[i] = XUartLite_RecvByte(XPAR_UARTLITE_4_BASEADDR);
	}


	/// Find header from radar device 0,1,2
	for (k_0 = 0; k_0 < RX_BUFFER_SIZE; k_0++){
		if (*(ReceiveBuffer_0 + k_0) == 0xfe && *(ReceiveBuffer_0 + k_0 + 1) == 0x00){
			break;
		}
	}

	// checksum
	for (int i = 1; i <= 4; i++){
		rx_chk[0] += *(ReceiveBuffer_0 + k_0 + i); /// pointer
	}
	tx_chk[0] = ReceiveBuffer_0 + k_0 + 5; /// value

	// Verify data from Lidar
	// lidar_header is the index of 'SPACE' within received data packet
	for (lidar_header = 0; lidar_header < RX_LASER_BUFFER_SIZE;  lidar_header++){
		if (*(ReceiveBuffer_4 + lidar_header) == 32){
			break;
		}
	}

	//lidar_tail is the index of '\n\r' with received data packet
	for (lidar_tail = 0; lidar_tail < RX_LASER_BUFFER_SIZE;  lidar_tail++){
		if (*(ReceiveBuffer_4 + lidar_tail) == 13 && *(ReceiveBuffer_4 + lidar_tail + 1) == 10 && (lidar_tail > lidar_header)){
			break;
		}
	}
	// Obtain one entire data packet
	for (int i = 0; i < RX_LASER_BUFFER_SIZE/2; i++){
		*(LidarDataBuffer + i) = *(ReceiveBuffer_4 + lidar_header + i);
	}
	// decimal_separator is the index of '.' within received data packet
	for (decimal_separator = 0; decimal_separator < RX_LASER_BUFFER_SIZE/2; decimal_separator++){
		if (*(LidarDataBuffer + decimal_separator) == 46){
			break;
		}
	}
	for (int i = 0; i < RX_LASER_BUFFER_SIZE/2; i++){
		if (*(LidarDataBuffer + i) >= 48){
			*(LidarDataBuffer_valid + i) = *(LidarDataBuffer + i) - 48;
		}
		else{
			*(LidarDataBuffer_valid + i) = 0;
		}
	}


	if (decimal_separator == 2){
		laser_data = LidarDataBuffer_valid[1] * 100 + LidarDataBuffer_valid[3] * 10 + LidarDataBuffer_valid[4] * 1;
	}
	else if (decimal_separator == 3){
		laser_data = LidarDataBuffer_valid[1] * 1000 + LidarDataBuffer_valid[2] * 100 + LidarDataBuffer_valid[4] * 10 + LidarDataBuffer_valid[5] * 1;
	}
	else if (decimal_separator == 4){
		laser_data = LidarDataBuffer_valid[1] * 10000 + LidarDataBuffer_valid[2] * 1000 + LidarDataBuffer_valid[3] * 100 + LidarDataBuffer_valid[5] * 10 + LidarDataBuffer_valid[6] * 1;
	}
	else {
		laser_data = 0;
	}
	printf ("%d\n\r", laser_data);

	//Transmit reform data
	// 10 Bytes -- 0xFE, 0x00, LSB0, MSB0, LSB1, MSB1, LSB2, MSB2, 0x00, CHECKSUM
	tx_checksum = 0;
	SendBuffer[0] = Header;
	if (rx_chk[0] == *tx_chk[0]){
		SendBuffer[1] = *(ReceiveBuffer_0 + 1 + k_0);
		SendBuffer[2] = *(ReceiveBuffer_0 + 2 + k_0);
		SendBuffer[3] = *(ReceiveBuffer_0 + 3 + k_0);
	}
	SendBuffer[4] = 0;
	SendBuffer[5] = 0;
	SendBuffer[6] = 0;
	SendBuffer[7] = 0;
	SendBuffer[8] = laser_data;
	SendBuffer[9] = (laser_data >> 8);
	SendBuffer[10] = RESERVED_BYTE; 	// reserve
	for (int i = 1; i < 10; i++){	// checksum
		tx_checksum += SendBuffer[i];
	}
	SendBuffer[11] = (tx_checksum) & 0xFF;
}

void uart_tworadar_lidar(){
	int tx_checksum;
	int k_0, k_1;
	int lidar_header;
	int laser_data;
	int decimal_separator;
	int lidar_tail;
	lidar_header = 0;
	lidar_tail = 0;
	decimal_separator = 0;

	// checksum number reset
	for (int i = 0; i < 2; i++){
		*(rx_chk + i) = 0;
	}
	for (int i = 0; i < RX_BUFFER_SIZE; i++){
		ReceiveBuffer_0[i] = 0;
		ReceiveBuffer_1[i] = 0;
	}
	// Receive data from radar device 0,1,2
	for (int i = 0; i < RX_BUFFER_SIZE; i++){
		ReceiveBuffer_0[i] = XUartLite_RecvByte(XPAR_UARTLITE_0_BASEADDR);
	}
	for (int i = 0; i < RX_BUFFER_SIZE; i++){
		ReceiveBuffer_1[i] = XUartLite_RecvByte(XPAR_UARTLITE_1_BASEADDR);
	}

	// Receive data packets from laser device
	for (int i = 0; i < RX_LASER_BUFFER_SIZE; i++){
		ReceiveBuffer_4[i] = 0;
	}
	for (int i = 0; i < RX_LASER_BUFFER_SIZE; i++){
		ReceiveBuffer_4[i] = XUartLite_RecvByte(XPAR_UARTLITE_4_BASEADDR);
	}


	/// Find header from radar device 0,1,2
	for (k_0 = 0; k_0 < RX_BUFFER_SIZE; k_0++){
		if (*(ReceiveBuffer_0 + k_0) == 0xfe && *(ReceiveBuffer_0 + k_0 + 1) == 0x00){
			break;
		}
	}
	for (k_1 = 0; k_1 < RX_BUFFER_SIZE; k_1++){
		if (ReceiveBuffer_1[k_1] == 0xfe && ReceiveBuffer_1[k_1 + 1] == 0x00){
			break;
		}
	}

	// checksum
	for (int i = 1; i <= 4; i++){
		rx_chk[0] += *(ReceiveBuffer_0 + k_0 + i); /// pointer
		rx_chk[1] += *(ReceiveBuffer_1 + k_1 + i);
	}
	tx_chk[0] = ReceiveBuffer_0 + k_0 + 5; /// value
	tx_chk[1] = ReceiveBuffer_1 + k_1 + 5;

	// Verify data from Lidar
	// lidar_header is the index of 'SPACE' within received data packet
	for (lidar_header = 0; lidar_header < RX_LASER_BUFFER_SIZE;  lidar_header++){
		if (*(ReceiveBuffer_4 + lidar_header) == 32){
			break;
		}
	}
	//lidar_tail is the index of '\n\r' with received data packet
	for (lidar_tail = 0; lidar_tail < RX_LASER_BUFFER_SIZE;  lidar_tail++){
		if (*(ReceiveBuffer_4 + lidar_tail) == 13 && *(ReceiveBuffer_4 + lidar_tail + 1) == 10 && (lidar_tail > lidar_header)){
			break;
		}
	}
	// Obtain one entire data packet
	for (int i = 0; i < RX_LASER_BUFFER_SIZE/2; i++){
		*(LidarDataBuffer + i) = *(ReceiveBuffer_4 + lidar_header + i);
	}
	// decimal_separator is the index of '.' within received data packet
	for (decimal_separator = 0; decimal_separator < RX_LASER_BUFFER_SIZE/2; decimal_separator++){
		if (*(LidarDataBuffer + decimal_separator) == 46){
			break;
		}
	}

	for (int i = 0; i < RX_LASER_BUFFER_SIZE/2; i++){
		if (*(LidarDataBuffer + i) >= 48){
			*(LidarDataBuffer_valid + i) = *(LidarDataBuffer + i) - 48;
		}
		else{
			*(LidarDataBuffer_valid + i) = 0;
		}
	}

	if (decimal_separator == 2){
		laser_data = LidarDataBuffer_valid[1] * 100 + LidarDataBuffer_valid[3] * 10 + LidarDataBuffer_valid[4] * 1;
	}
	else if (decimal_separator == 3){
		laser_data = LidarDataBuffer_valid[1] * 1000 + LidarDataBuffer_valid[2] * 100 + LidarDataBuffer_valid[4] * 10 + LidarDataBuffer_valid[5] * 1;
	}
	else if (decimal_separator == 4){
		laser_data = LidarDataBuffer_valid[1] * 10000 + LidarDataBuffer_valid[2] * 1000 + LidarDataBuffer_valid[3] * 100 + LidarDataBuffer_valid[5] * 10 + LidarDataBuffer_valid[6] * 1;
	}
	else {
		laser_data = 0;
	}
	//Transmit reform data
	// 10 Bytes -- 0xFE, 0x00, LSB0, MSB0, LSB1, MSB1, LSB2, MSB2, 0x00, CHECKSUM
	tx_checksum = 0;
	SendBuffer[0] = Header;
	if (rx_chk[0] == *tx_chk[0]){
		SendBuffer[1] = *(ReceiveBuffer_0 + 1 + k_0);
		SendBuffer[2] = *(ReceiveBuffer_0 + 2 + k_0);
		SendBuffer[3] = *(ReceiveBuffer_0 + 3 + k_0);
	}
	if (rx_chk[1] == *tx_chk[1]){
		SendBuffer[4] = *(ReceiveBuffer_1 + 2 + k_1);
		SendBuffer[5] = *(ReceiveBuffer_1 + 3 + k_1);
	}
	SendBuffer[6] = 0;
	SendBuffer[7] = 0;
	SendBuffer[8] = laser_data;
	SendBuffer[9] = (laser_data >> 8);
	SendBuffer[10] = RESERVED_BYTE; 	// reserve

	for (int i = 1; i < 10; i++){	// checksum
		tx_checksum += SendBuffer[i];
	}
	SendBuffer[11] = (tx_checksum) & 0xFF;
}

void uart_threeradar_lidar(){
	int tx_checksum;
	int k_0, k_1, k_2;
	int lidar_header;
	int laser_data;
	int decimal_separator;
	int lidar_tail;
	lidar_header = 0;
	lidar_tail = 0;
	decimal_separator = 0;

	// checksum number reset
	for (int i = 0; i < 3; i++){
		*(rx_chk + i) = 0;
	}
	for (int i = 0; i < RX_BUFFER_SIZE; i++){
		ReceiveBuffer_0[i] = 0;
		ReceiveBuffer_1[i] = 0;
		ReceiveBuffer_2[i] = 0;
	}

	// Receive data from radar device 0,1,2
	for (int i = 0; i < RX_BUFFER_SIZE; i++){
		ReceiveBuffer_0[i] = XUartLite_RecvByte(XPAR_UARTLITE_0_BASEADDR);
//		printf ("%d\n\r", ReceiveBuffer_0[i]);
	}
	for (int i = 0; i < RX_BUFFER_SIZE; i++){
		ReceiveBuffer_1[i] = XUartLite_RecvByte(XPAR_UARTLITE_1_BASEADDR);
	}
	for (int i = 0; i < RX_BUFFER_SIZE; i++){
		ReceiveBuffer_2[i] = XUartLite_RecvByte(XPAR_UARTLITE_2_BASEADDR);
	}


	// Receive data packets from laser device
	for (int i = 0; i < RX_LASER_BUFFER_SIZE; i++){
		ReceiveBuffer_4[i] = 0;
	}
	for (int i = 0; i < RX_LASER_BUFFER_SIZE; i++){
		ReceiveBuffer_4[i] = XUartLite_RecvByte(XPAR_UARTLITE_4_BASEADDR);
	}


	/// Find header from radar device 0,1,2
	for (k_0 = 0; k_0 < RX_BUFFER_SIZE; k_0++){
		if (*(ReceiveBuffer_0 + k_0) == 0xfe && *(ReceiveBuffer_0 + k_0 + 1) == 0x00){
			break;
		}
	}
	for (k_1 = 0; k_1 < RX_BUFFER_SIZE; k_1++){
		if (ReceiveBuffer_1[k_1] == 0xfe && ReceiveBuffer_1[k_1 + 1] == 0x00){
			break;
		}
	}
	for (k_2 = 0; k_2 < RX_BUFFER_SIZE; k_2++){
		if (ReceiveBuffer_0[k_2] == 0xfe && ReceiveBuffer_1[k_1 + 1] == 0x00){
			break;
		}
	}


	// checksum
	for (int i = 1; i <= 4; i++){
		rx_chk[0] += *(ReceiveBuffer_0 + k_0 + i); /// pointer
		rx_chk[1] += *(ReceiveBuffer_1 + k_1 + i);
		rx_chk[2] += *(ReceiveBuffer_2 + k_2 + i);
	}
	tx_chk[0] = ReceiveBuffer_0 + k_0 + 5; /// value
	tx_chk[1] = ReceiveBuffer_1 + k_1 + 5;
	tx_chk[2] = ReceiveBuffer_2 + k_2 + 5;


	// Verify data from Lidar
	// lidar_header is the index of 'SPACE' within received data packet
	for (lidar_header = 0; lidar_header < RX_LASER_BUFFER_SIZE;  lidar_header++){
		if (*(ReceiveBuffer_4 + lidar_header) == 32){
			break;
		}
	}
	printf ("%d,", lidar_header);

	//lidar_tail is the index of '\n\r' with received data packet
	for (lidar_tail = 0; lidar_tail < RX_LASER_BUFFER_SIZE;  lidar_tail++){
		if (*(ReceiveBuffer_4 + lidar_tail) == 13 && *(ReceiveBuffer_4 + lidar_tail + 1) == 10 && (lidar_tail > lidar_header)){
			break;
		}
	}
	printf ("%d,", lidar_tail);
	// Obtain one entire data packet
	for (int i = 0; i < RX_LASER_BUFFER_SIZE/2; i++){
		*(LidarDataBuffer + i) = *(ReceiveBuffer_4 + lidar_header + i);
	}
	// decimal_separator is the index of '.' within received data packet
	for (decimal_separator = 0; decimal_separator < RX_LASER_BUFFER_SIZE/2; decimal_separator++){
		if (*(LidarDataBuffer + decimal_separator) == 46){
			break;
		}
	}
	printf ("%d, ", decimal_separator);
	//
	for (int i = 0; i < RX_LASER_BUFFER_SIZE/2; i++){
		if (*(LidarDataBuffer + i) >= 48){
			*(LidarDataBuffer_valid + i) = *(LidarDataBuffer + i) - 48;
		}
		else{
			*(LidarDataBuffer_valid + i) = 0;
		}
//		printf ("%d\n\r", LidarDataBuffer_valid[i]);
	}


	if (decimal_separator == 2){
		laser_data = LidarDataBuffer_valid[1] * 100 + LidarDataBuffer_valid[3] * 10 + LidarDataBuffer_valid[4] * 1;
	}
	else if (decimal_separator == 3){
		laser_data = LidarDataBuffer_valid[1] * 1000 + LidarDataBuffer_valid[2] * 100 + LidarDataBuffer_valid[4] * 10 + LidarDataBuffer_valid[5] * 1;
	}
	else if (decimal_separator == 4){
		laser_data = LidarDataBuffer_valid[1] * 10000 + LidarDataBuffer_valid[2] * 1000 + LidarDataBuffer_valid[3] * 100 + LidarDataBuffer_valid[5] * 10 + LidarDataBuffer_valid[6] * 1;
	}
	else {
		laser_data = 0;
	}
	printf ("%d\n\r", laser_data);

	//Transmit reform data
	// 10 Bytes -- 0xFE, 0x00, LSB0, MSB0, LSB1, MSB1, LSB2, MSB2, 0x00, CHECKSUM
	tx_checksum = 0;
	SendBuffer[0] = Header;
	if (rx_chk[0] == *tx_chk[0]){
		SendBuffer[1] = *(ReceiveBuffer_0 + 1 + k_0);
		SendBuffer[2] = *(ReceiveBuffer_0 + 2 + k_0);
		SendBuffer[3] = *(ReceiveBuffer_0 + 3 + k_0);
	}
	if (rx_chk[1] == *tx_chk[1]){
		SendBuffer[4] = *(ReceiveBuffer_1 + 2 + k_1);
		SendBuffer[5] = *(ReceiveBuffer_1 + 3 + k_1);
	}
	if (rx_chk[2] == *tx_chk[2]){
		SendBuffer[6] = *(ReceiveBuffer_2 + 2 + k_2);
		SendBuffer[7] = *(ReceiveBuffer_2 + 3 + k_2);
	}

	SendBuffer[8] = laser_data;
	SendBuffer[9] = (laser_data >> 8);
	SendBuffer[10] = RESERVED_BYTE; 	// reserve

	for (int i = 1; i < 10; i++){	// checksum
		tx_checksum += SendBuffer[i];
	}
	SendBuffer[11] = (tx_checksum) & 0xFF;
//	XUartLite_Send(&UartLite_3, SendBuffer, RX_LASER_BUFFER_SIZE/2);
}

void uart_trx_example(int uart_number){

	if (uart_number == 48){
		uart_oneradar_lidar();
		printf("one radar");
	}
	else if (uart_number == 49){
		uart_tworadar_lidar();
	}
	else if (uart_number == 50){
		uart_threeradar_lidar();
	}
	else{
		printf("Configuration Parameter Issue!!!");
	}

}
