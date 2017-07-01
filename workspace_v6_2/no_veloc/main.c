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

#define LOOP_COUNT_TIMEOUT 5

char keyStates[NUM_PORTS*8] = {0};
const char MIDINote[NUM_PORTS*8] = {0}; //todo



//uint8_t a = 0;                          //key offset
//static  uint8_t Pin2MIDI[NumOfKeys] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57,               //Map Pin diagram to Key notes
//                                                58,59,60,61,62,63,64,65,66,67,68,69,70,71,72};
// uint8_t flag[NumOfKeys] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
// uint8_t tout[NumOfKeys] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
// uint8_t MIDI[NumOfKeys];                                   //all of these arrays initalized to zero
//
//volatile char *On;
//volatile char *Off;
char MIDILength = 3;
char cindex = 0;                     //index into state array
volatile char MIDINoteOn[3] = {0x90, 0x3c, 0x00};
volatile char MIDINoteOff[3] = {0x80, 0x3c, 127};

void pinHandler(char keyIndex, char keyState);




void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

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
    switch(keyIndex)
    {
        case 0:
            if(keyState == 0)
            {
                keyIndex = 1;
                //UARTSendOn todo
            }

            else
                keyIndex = 0;
            break;

        default:
            if(keyState == 1)
                keyIndex++;
            else
                keyIndex = 1;

            if(keyIndex > LOOP_COUNT_TIMEOUT)
            {
                keyIndex = 0;
                //UARTSendOFF
            }
            break;
    }
}


