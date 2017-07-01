//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include <UART.h>

/*
#include "string.h"
#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>
#include "UART.h"
#include "math.h"
*/

/*
uint8_t flag = 1;
uint8_t MIDI = 0;
uint16_t T1 = 0;
uint16_t T2 = 0;
uint16_t velocity = 0;

#define P6_0 0
#define P3_2 1
#define P6_1 2
#define P3_3 3
#define P4_0 4
#define P4_1 5
#define P4_2 6
#define P4_3 7
#define P4_4 8
#define P1_5 9
#define P4_5 10
#define P4_6 11
#define P4_7 12
#define P6_5 13
#define P5_4 14
#define P6_4 15
#define P5_5 16

#define P2_3 17
#define P1_7 18
#define P5_1 19
#define P5_0 20
#define P3_5 21
#define P5_2 22
#define P3_7 23
#define P3_6 24
*/

#define NumOfKeys 25
#define NUM_PORTS 9
#define KEY_OFFSET 48
#define KN 255 //null key

#define LOOP_COUNT_TIMEOUT 5

char keyStates[NUM_PORTS*8] = {};
const char MIDINote[NUM_PORTS*8] = {
	KN,	KN,	KN,	KN,	KN,	KN,	10,	KN,		//Port1
	KN,	KN,	KN,	KN,	4,	3,	2,	1,		//Port2
	5,	KN,	KN,	KN,	KN,	KN,	KN,	KN,		//Port3
	KN,	KN,	KN,	KN,	KN,	KN,	KN,	KN,		//Port4
	KN,	KN,	16,	KN,	KN,	KN,	6,	7,		//Port5
	KN,	15,	KN,	12,	KN,	KN,	8,	9,		//Port6
	KN,	11,	KN,	13,	KN,	KN,	KN,	KN,		//Port7
	KN,	KN,	19,	18,	21,	25,	24,	22,		//Port8
	23,	20,	17,	14,	KN,	KN,	KN,	KN 		//Port9
};

char MIDILength = 3;
char cindex = 0;                     //index into state array
volatile char MIDINoteOn[3] = {0x90, 0x3c, 0x00};
volatile char MIDINoteOff[3] = {0x80, 0x3c, 127};

void pinHandler(char keyIndex, char keyState);


void UARTset (void){

    // Setup UART
    UARTSetup();


    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    // Enable eUSCIA3 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA3_IRQn) & 31);
}



void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    CS->KEY = 0x0000695A;                           //unlock all the registers to be able to access

    /*Following are primary registers for necessary clock functions*/

    CS->CTL0 = 0x00810000;

    CS->CTL1 = 0x50300033;

    CS->CLKEN = 0x0000000E;

    CS->KEY = 0x00010000;                   //lock all clock registers

    /*Configure UART MODULE*/
    UARTset();

    char i;
    char state;
	while(1){

	    state = P1IN;                   //get status of port one
	    for(i = 0;i<8;i++){
	        pinHandler(cindex,state&&0x01);
	        state = state>>1;                    //bit shift our state
	        cindex++;
	        }

        state = P2IN;                   //get status of port one
        for(i = 0;i<8;i++){
            pinHandler(cindex,state&&0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P3IN;                   //get status of port one
        for(i = 0;i<8;i++){
            pinHandler(cindex,state&&0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P4IN;                   //get status of port one
        for(i = 0;i<8;i++){
            pinHandler(cindex,state&&0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P5IN;                   //get status of port one
        for(i = 0;i<8;i++){
            pinHandler(cindex,state&&0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P6IN;                   //get status of port one
        for(i = 0;i<8;i++){
            pinHandler(cindex,state&&0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P7IN;                   //get status of port one
        for(i = 0;i<8;i++){
            pinHandler(cindex,state&&0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P8IN;                   //get status of port one
        for(i = 0;i<8;i++){
            pinHandler(cindex,state&&0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P9IN;                   //get status of port one
        for(i = 0;i<8;i++){
            pinHandler(cindex,state&&0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }
	  }
}

void pinHandler(char keyIndex, char keyState)
{
    switch(keyStates[keyIndex])
    {
        case 0:
            if(keyState == 0)
            {
                keyStates[keyIndex] = 1;
                //UARTSendOn todo
            }

            else
                keyStates[keyIndex] = 0;
            break;

        default:
            if(keyState == 1)
                keyStates[keyIndex]++;
            else
                keyStates[keyIndex] = 1;

            if(keyStates[keyIndex] > LOOP_COUNT_TIMEOUT)
            {
                keyStates[keyIndex] = 0;
                //UARTSendOFF
            }
            break;
    }
}


