/*
 * main.h
 *
 *  Created on: Jan 23, 2019
 *      Author: Ainstein Zhenyu Hu
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_



#endif /* SRC_MAIN_H_ */

#include "xuartlite.h"


#define RX_BUFFER_SIZE 				12
#define TX_BUFFER_SIZE 				12
#define Header 						0xFE
#define RESERVED_BYTE				0x00
#define RX_LASER_BUFFER_SIZE		18


XUartLite UartLite_0, UartLite_1, UartLite_2, UartLite_3, UartLite_4;
u8 ReceiveBuffer_0[RX_BUFFER_SIZE], ReceiveBuffer_1[RX_BUFFER_SIZE], ReceiveBuffer_2[RX_BUFFER_SIZE], ReceiveBuffer_3[RX_BUFFER_SIZE], ReceiveBuffer_4[RX_LASER_BUFFER_SIZE];
u8 ReceiveBuffer_0_header[RX_BUFFER_SIZE];
u8 *SendBuffer_Pointer[TX_BUFFER_SIZE];
unsigned char rx_chk_0, rx_chk_1, rx_chk_2;
unsigned char tx_chk_0, tx_chk_1, tx_chk_2;
unsigned char *tx_chk[3];
unsigned char rx_chk[3];
u8 LidarDataBuffer[9], LidarDataBuffer_valid[9];

u8 SendBuffer[TX_BUFFER_SIZE];
u8 SendBuffer_LASER[RX_LASER_BUFFER_SIZE];

int uartlite_initialization();
void uart_trx_example(int uart_number);
//int uart_trx_first();
//int uart_trx_comp(int j);
int sd_read(char *filename);
int sd_write(char *filename);
void uart_oneradar_lidar();
void uart_tworadar_lidar();
void uart_threeradar_lidar();
