/*
 * UART.h
 *
 *  Created on: Dec 2, 2016
 *      Author: hankyang
 *  Implement UART software buffers for MIDI testing
 */

#ifndef UART_H_
#define UART_H_


// define the buffer size to be 50 bytes (50*8 bits)
#define UART_TX_BUFFER_SIZE 50u

void UARTSetup(void);
void UARTSendByte(char byte);
void UARTSendArray(char *TxArray, char ArrayLength);
void UARTSendIntArray(int *data, char length);
extern void EUSCIA2_IRQHandler(void);

#endif /* UART_H_ */
