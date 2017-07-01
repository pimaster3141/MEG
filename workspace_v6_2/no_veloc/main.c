//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp.h"
#include "string.h"
#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>
#include "UART.h"
#include "math.h"

/*
uint8_t flag = 1;
uint8_t MIDI = 0;
uint16_t T1 = 0;
uint16_t T2 = 0;
uint16_t velocity = 0;
*/
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

#define NumOfKeys 25
uint8_t a = 0;                          //key offset
static volatile uint8_t Pin2MIDI[NumOfKeys] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57,               //Map Pin diagram to Key notes
                                                58,59,60,61,62,63,64,65,66,67,68,69,70,71,72};
volatile uint8_t flag[NumOfKeys] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
volatile uint8_t tout[NumOfKeys] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
volatile uint8_t MIDI[NumOfKeys];                                   //all of these arrays initalized to zero

volatile char *On;
volatile char *Off;
char MIDILength = 3;
volatile char MIDINoteOn[3] = {0x90, 0x3c, 0x00};
volatile char MIDINoteOff[3] = {0x80, 0x3c, 127};

void main(void)
{
	
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer


	// test comment
}
