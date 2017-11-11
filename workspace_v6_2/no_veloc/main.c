//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include "UART.h"
#include "driverlib.h"


#define NUM_PORTS 9
#define LOOP_COUNT_TIMEOUT 25
#define KEY_OFFSET 47
#define KN 255 //null key
#define MIDI_ON 0x90
#define MIDI_OFF 0x80


char keyStates[NUM_PORTS*8] = {};
const char MIDINote[NUM_PORTS*8] =
{
	KN,	KN,	KN,	KN,	KN,	KN,	10,	KN,		//Port1
	KN,	KN,	KN,	KN,	4,	3,	2,	1,		//Port2
	5,	KN,	KN,	KN,	KN,	KN,	KN,	KN,		//Port3
	KN,	KN,	KN,	KN,	KN,	KN,	KN,	KN,		//Port4
	KN,	KN,	KN,	16,	KN,	KN,	6,	7,		//Port5
	KN,	KN,	15,	12,	KN,	KN,	8,	9,		//Port6
	KN,	11,	KN,	13,	KN,	KN,	KN,	KN,		//Port7
	KN,	KN,	19,	18,	21,	25,	24,	22,		//Port8
	23,	20,	17,	14,	KN,	KN,	KN,	KN 		//Port9
};

void pinHandler(char keyIndex, char keyState);
void MIDIOn(char pitch, char volume);
void MIDIOff(char pitch);
void UARTSet();
void setupClock();
void setupISRs();






void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    setupClock();
    /*Configure UART MODULE*/
    UARTSet();

    setupISRs();



    char i;
    char state;

	while(1){
	    char cindex = 0;                 //index into state array
	    
	     state = P1IN;                   //get status of port one
	     for(i = 0; i < 8; i++){
	         pinHandler(cindex, state & 0x01);
	         state = state>>1;                    //bit shift our state
	         cindex++;
	         }

        state = P2IN;                   //get status of port one
        for(i = 0; i < 8; i++){
            pinHandler(cindex, state & 0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P3IN;                   //get status of port one
        for(i = 0; i < 8; i++){
            pinHandler(cindex, state & 0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P4IN;                   //get status of port one
        for(i = 0; i < 8; i++){
            pinHandler(cindex, state & 0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P5IN;                   //get status of port one
        for(i = 0; i < 8; i++){
            pinHandler(cindex, state & 0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P6IN;                   //get status of port one
        for(i = 0; i < 8; i++){
            pinHandler(cindex, state & 0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P7IN;                   //get status of port one
        for(i = 0; i < 8; i++){
            pinHandler(cindex, state & 0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P8IN;                   //get status of port one
        for(i = 0; i < 8; i++){
            pinHandler(cindex, state & 0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }

        state = P9IN;                   //get status of port one
        for(i = 0; i < 8; i++){
            pinHandler(cindex, state & 0x01);
            state = state>>1;                    //bit shift our state
            cindex++;
            }
	  }
}

void setupClock()
{
    CS->KEY = 0x0000695A;                           //unlock all the registers to be able to access
    /*Following are primary registers for necessary clock functions*/
    CS->CTL0 = 0x00810000;
    CS->CTL1 = 0x50300033;
    CS->CLKEN = 0x0000000E;
    CS->KEY = 0x00010000;                   //lock all clock registers
}

void setupISRs()
{
    MAP_Interrupt_enableMaster();
    __enable_irq();
}

void UARTSet(){

    // Setup UART
    UARTSetup();


    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    // Enable eUSCIA3 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA3_IRQn) & 31);
}

void pinHandler(char keyIndex, char keyState)
{
//    printf("\n keyState: %u",keyState);
//    printf("\n keyIndex: %u\n",keyIndex);

    switch(keyStates[keyIndex])
    {
        case 0:
            if(keyState == 0)
            {
                __no_operation();
                keyStates[keyIndex] = 1;
                if(MIDINote[keyIndex] != KN)
                	MIDIOn(MIDINote[keyIndex], 127);
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
                if(MIDINote[keyIndex] != KN)
                	MIDIOff(MIDINote[keyIndex]);
            }
            break;
    }
}

void MIDIOn(char pitch, char volume)
{
	char payload[3] = {MIDI_ON, pitch + KEY_OFFSET, volume};
	UARTSendArray(payload, 3);
	__no_operation();
}

void MIDIOff(char pitch)
{
	char payload[3] = {MIDI_OFF, pitch + KEY_OFFSET, 0};
	UARTSendArray(payload, 3);
    __no_operation();
}


