/*
 * UART.c
 *
 *  Created on: Dec 2, 2016
 *      Author: hankyang
 *  Implement software buffer
 */

#include <msp.h>
#include "UART.h"
#include <string.h>

static volatile unsigned char UARTTXBuffer[UART_TX_BUFFER_SIZE]; //Buffer array to hold TX data
static volatile unsigned char UARTTXBufferSendoutPointer = 0; //Index pointer of the next byte to be sent
static volatile unsigned char UARTTXBufferEndPointer = 0; //Index pointer of the next empty position

void UARTSetup(void)
{
	// Setup UART, 8N1 31250 baudrate, non-parity
	EUSCI_A2->CTLW1 |= UCMSB; //MSB first
	EUSCI_A2->CTLW0 |= UCSSEL_2; //Select clock rate (SMCLK)
	EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST; //reset parameters

	//Configure Baud Rate
	EUSCI_A2->MCTLW = 0u;   //UCOS16 determine whether CLK/BR > 16
	EUSCI_A2->BRW = 3u;    //CLK = 12MHz, BR = 31250, CLK/BR/16 = 384/16 =24||CLK = 3MHz, BR=31250, CLK/BR/16 = 96/16=6
	//eEUSCI_A2->MCTLW |= 0x0400;
	//UCA2BR0 = 24u;	    // SET BAUD RATE 31250 12M/31250 = 384 384/16 = 24
	//UCA2BR1 = 0;		    // N = clk/baud
	//UCA2MCTLW |= UCOS16;  // because 384>16 set UCOS16 = 1b

	// Configure UART pins
	P3->SEL0 |= BIT2 | BIT3;           // set 2-UART pin as second function
	//P3SEL0 |= BIT2 | BIT3;
	// UCA2CTL0 &= ~UCSWRST;	       // init uart

	// UCA2IE |= UCRXIE | UCTXIE;				// Enable interrupts both transmission and reception

	EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;// Initialize eUSCI
	EUSCI_A2->IE |= EUSCI_A_IE_RXIE;        // Enable USCI_A2 RX interrupt
	EUSCI_A2->IE |= EUSCI_A_IE_TXIE;        // Enable USCI_A2 TX interrupt

	//printf("Init UART.\n");
}

void UARTSendByte(char byte)
{
	UARTTXBuffer[UARTTXBufferEndPointer] = byte;
	UARTTXBufferEndPointer++;
	if(UARTTXBufferEndPointer == UART_TX_BUFFER_SIZE)
		UARTTXBufferEndPointer = 0;
	UCA2IE |= UCTXIE;    //Enable UART transmission interrupt
	return;
}

void UARTSendArray(char *TxArray, char ArrayLength)
{
	while(ArrayLength--)
	{
		//printf("%d\n", *TxArray);
		UARTTXBuffer[UARTTXBufferEndPointer] = *TxArray;
		TxArray++;
		UARTTXBufferEndPointer++;
		if(UARTTXBufferEndPointer == UART_TX_BUFFER_SIZE)
			UARTTXBufferEndPointer = 0;
	}
	UCA2IE |= UCTXIE;  //Enable UART transmission interrupt
	return;
}

void UARTSendIntArray(int *data, char length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		int current = data[i];
		//printf('current\n');
		char lowbyte = (char) (current & 0x00FF);
		char highbyte = (char) ((current >> 8) & 0x00FF);
		UARTSendByte(highbyte);
		UARTSendByte(lowbyte);
	}
	UCA2IE |= UCTXIE;  //Enable UART transmission interrupt
	return;
}


void EUSCIA2_IRQHandler(void)
{

	//printf('ISR');
	//if (EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG)
	if (UCTXIFG)  // If transmission interrupt (UCTXIFG is HIGH, BUFFER empty)
	{
		EUSCI_A2->IFG &=~ EUSCI_A_IFG_TXIFG;   // Clear interrupt
		//printf("TX\n");
		UCA2TXBUF = UARTTXBuffer[UARTTXBufferSendoutPointer];
		P4OUT = P4OUT ^ 1<<4;
		//BufVal = UCA2TXBUF;
		UARTTXBufferSendoutPointer++;
		if(UARTTXBufferSendoutPointer == UART_TX_BUFFER_SIZE)
			UARTTXBufferSendoutPointer = 0;
		if(UARTTXBufferSendoutPointer == UARTTXBufferEndPointer)
			UCA2IE &= ~UCTXIE;     //Disable Interrupt
	}
}



