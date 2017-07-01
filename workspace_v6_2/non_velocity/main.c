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
static volatile uint8_t Pin2MIDI[NumOfKeys] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57,               //Map Pin diagram to Key notes
												58,59,60,61,62,63,64,65,66,67,68,69,70,71,72};
volatile uint8_t flag[NumOfKeys] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
volatile uint8_t MIDI[NumOfKeys];

volatile char *On;
volatile char *Off;
char MIDILength = 3;
volatile char MIDINoteOn[3] = {0x90, 0x3c, 0x00};
volatile char MIDINoteOff[3] = {0x80, 0x3c, 127};

void main(void)
     {
	/*Enable all necessary clock registers*/

	CS->KEY = 0x0000695A;							//unlock all the registers to be able to access
	printf("\n CSKEY = %x\n", CS->KEY);

	/*Following are primary registers for necessary clock functions*/

	CS->CTL0 = 0x00810000;
	printf("\n CSCTL0 = %x\n", CS->CTL0);
	CS->CTL1 = 0x50300033;
	printf("\n CSCTL1 = %x\n", CS->CTL1);
	CS->CLKEN = 0x0000000E;
	printf("\n CLKEN = %x\n", CS->CLKEN);

	CS->KEY = 0x00010000;					//lock all clock registers
	printf("\n CSKEY = %x\n", CS->KEY);
	
	/*Configure necessary timer registers for our timer frequency*/
	TA1CTL = 0x0220;
	TA1EX0 = 0x0000;					//use this to further divide our clock

	/*Configure UART MODULE*/
	UARTset();

	//Setup all ports for ISR and inputs
	ISRsetup();

    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    //main loop checking state of comparator
    while(1){
    	P7OUT |= BIT4;
/*
    	uint16_t tim1 = TA1R;
    	uint16_t tim2 = tim1;
    	P7OUT = P7OUT ^ 1<<4;			//debugging pin
    	while((tim2 - tim1) < 40000){
    		tim2 = TA1R;
    	}
    	P7OUT = P7OUT ^ 1<<4;			//debugging pin

    	while(1){

    	}*/

/*check for comph 1 */
/*comph 1*/
     	if((P2IN & 0x80) && MIDI[P6_0]){
	   		MIDINoteOff[1] = Pin2MIDI[P6_0];
     		Off = MIDINoteOff;
    	    UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P6_0] = 0;									//clear MIDI send flag - sending note off

    	    	}

     	/*comph 1 on*/
     	if(((P2IN & 0x80)^1<<7) && !MIDI[P6_0]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P6_0];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P6_0] = 1;									//clear MIDI send flag - sending note off

     	    	    	}

/*comph 2 off*/
    	if((P2IN & 0x40) && MIDI[P3_2]){

    		MIDINoteOff[1] = Pin2MIDI[P3_2];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P3_2] = 0;									//clear MIDI send flag - sending note off
    		}

     	/*comph 2 on*/
     	if(((P2IN & 0x40)^1<<6) && !MIDI[P3_2]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P3_2];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P3_2] = 1;									//clear MIDI send flag - sending note off

     	    	    	}

/*comph 3 */
    	if((P2IN & 0x20) && MIDI[P6_1]){

    		MIDINoteOff[1] = Pin2MIDI[P6_1];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);		//send NoteOFF message
    		MIDI[P6_1] = 0;						//clear MIDI send flag - sending note off
    		//P7OUT = P7OUT ^ 1<<4;				//debugging pin
    	}

     	/*comph 3 on*/
     	if(((P2IN & 0x20)^1<<5) && !MIDI[P6_1]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P6_1];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P6_1] = 1;									//clear MIDI send flag - sending note off

     	    	    	}



/* comph 4 */
    	if((P2IN & 0x10) && MIDI[P3_3]){

    		MIDINoteOff[1] = Pin2MIDI[P3_3];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P3_3] = 0;									//clear MIDI send flag - sending note off

    	}

     	/*comph 4 on*/
     	if(((P2IN & 0x10)^1<<4) && !MIDI[P3_3]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P3_3];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P3_3] = 1;									//clear MIDI send flag - sending note off

     	    	    	}

/* comph 5 */
    	if((P3IN & 0x01) && MIDI[P4_0]){

    		MIDINoteOff[1] = Pin2MIDI[P4_0];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_0] = 0;									//clear MIDI send flag - sending note off

    	}

     	/*comph 5 on*/
     	if(((P3IN & 0x01)^1) && !MIDI[P4_0]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P4_0];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P4_0] = 1;									//clear MIDI send flag - sending note off

     	    	    	}

/* comph 6 */
    	if((P5IN & 0x40) && MIDI[P4_1]){

    		MIDINoteOff[1] = Pin2MIDI[P4_1];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_1] = 0;									//clear MIDI send flag - sending note off
    		//P7OUT = P7OUT ^ 1<<4;				//debugging pin
    	}

     	/*comph 6 on*/
     	if(((P5IN & 0x40)^1<<6) && !MIDI[P4_1]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P4_1];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P4_1] = 1;									//clear MIDI send flag - sending note off

     	    	    	}

/* comph 7 */
    	if((P5IN & 0x80) && MIDI[P4_2]){

    		MIDINoteOff[1] = Pin2MIDI[P4_2];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_2] = 0;									//clear MIDI send flag - sending note off

    	}

     	/*comph 7 on*/
     	if(!(P5IN & 0x80) && !MIDI[P4_2]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P4_2];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P4_2] = 1;									//clear MIDI send flag - sending note off

     	    	    	}
/* comph 8 */
    	if((P6IN & 0x40) && MIDI[P4_3]){

    		MIDINoteOff[1] = Pin2MIDI[P4_3];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_3]= 0;									//clear MIDI send flag - sending note off

    	}
     	/*comph 8 on*/
     	if(!(P6IN & 0x40) && !MIDI[P4_3]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P4_3];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P4_3] = 1;									//clear MIDI send flag - sending note off

     	    	    	}
/*comph 9 */
    	if((P6IN & 0x80) && MIDI[P4_4]){

    		MIDINoteOff[1] = Pin2MIDI[P4_4];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_4] = 0;									//clear MIDI send flag - sending note off
    	}
     	/*comph 9 on*/
     	if(!(P6IN & 0x80) && !MIDI[P4_4]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P4_4];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P4_4] = 1;									//clear MIDI send flag - sending note off

     	    	    	}
/* comph 10 */
    	if((P1IN & 0x40) && MIDI[P1_5]){

    		MIDINoteOff[1] = Pin2MIDI[P1_5];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P1_5] = 0;									//clear MIDI send flag - sending note off

    	}
     	/*comph 10 on*/
     	if(!(P1IN & 0x40) && !MIDI[P1_5]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P1_5];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P1_5] = 1;									//clear MIDI send flag - sending note off

     	    	    	}
/* comph 11 */
    	if((P7IN & 0x02) && MIDI[P4_5]){

    		MIDINoteOff[1] = Pin2MIDI[P4_5];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_5] = 0;									//clear MIDI send flag - sending note off

    	}
     	/*comph 11 on*/
     	if(!(P7IN & 0x02) && !MIDI[P4_5]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P4_5];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P4_5] = 1;									//clear MIDI send flag - sending note off

     	    	    	}

/* comph 12 */
    	if((P6IN & 0x08) && MIDI[P4_6]){

    		MIDINoteOff[1] = Pin2MIDI[P4_6];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_6] = 0;									//clear MIDI send flag - sending note off

    	}

     	/*comph 12 on*/
     	if(!(P6IN & 0x08) && !MIDI[P4_6]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P4_6];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P4_6] = 1;									//clear MIDI send flag - sending note off

     	    	    	}
/* comph 13 */
    	if((P7IN & 0x08) && MIDI[P4_7]){

    		MIDINoteOff[1] = Pin2MIDI[P4_7];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_7] = 0;									//clear MIDI send flag - sending note off

    	}

     	/*comph 13 on*/
     	if(!(P7IN & 0x08) && !MIDI[P4_7]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P4_7];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P4_7] = 1;									//clear MIDI send flag - sending note off

     	    	    	}
/* comph 14---- needs work (*/
    	if((P9IN & 0x08) && MIDI[P6_5]){

    		MIDINoteOff[1] = Pin2MIDI[P6_5];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P6_5] = 0;									//clear MIDI send flag - sending note off

    	}

     	/*comph 14 on*/
     	if(!(P9IN & 0x08) && !MIDI[P6_5]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P6_5];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P6_5] = 1;									//clear MIDI send flag - sending note off

     	    	    	}
/*comph 15 */
    	if((P6IN & 0x04) && MIDI[P5_4]){

    		MIDINoteOff[1] = Pin2MIDI[P5_4];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P5_4] = 0;									//clear MIDI send flag - sending note off

    	}

     	/*comph 15 on*/
     	if(!(P6IN & 0x04) && !MIDI[P5_4]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P5_4];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P5_4] = 1;									//clear MIDI send flag - sending note off

     	    	    	}
/*comph 16 */
    	if((P5IN & 0x08) && MIDI[P6_4]){

    		MIDINoteOff[1] = Pin2MIDI[P6_4];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P6_4] = 0;									//clear MIDI send flag - sending note off

    	}

     	/*comph 16 on*/
     	if(!(P5IN & 0x08) && !MIDI[P6_4]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P6_4];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P6_4] = 1;									//clear MIDI send flag - sending note off

     	    	    	}
/* comph 17 */
    	if((P9IN & 0x04) && MIDI[P5_5]){

    		MIDINoteOff[1] = Pin2MIDI[P5_5];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P5_5] = 0;									//clear MIDI send flag - sending note off

    	}
     	/*comph 17 on*/
     	if(!(P9IN & 0x04) && !MIDI[P5_5]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P5_5];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P5_5] = 1;									//clear MIDI send flag - sending note off

     	    	    	}
/* comph 18 */
    	if((P8IN & 0x08) && MIDI[P2_3]){

    		MIDINoteOff[1] = Pin2MIDI[P2_3];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P2_3] = 0;									//clear MIDI send flag - sending note off

    	}

     	/*comph 2 on*/
     	if(!(P8IN & 0x08) && !MIDI[P3_2]){
			MIDINoteOn[2] = 90;							//replace Note on buffer with proper velocity
			MIDINoteOn[1] = Pin2MIDI[P3_2];						//map proper MIDI note value to MIDI array
			On = MIDINoteOn;										//pointer initialization
			UARTSendArray(On,MIDILength);						//Send MIDI note!
       		MIDI[P3_2] = 1;									//clear MIDI send flag - sending note off

     	    	    	}
/* comph 19 */
    	if((P8IN & 0x04) && MIDI[P1_7]){

    		MIDINoteOff[1] = Pin2MIDI[P1_7];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P1_7] = 0;									//clear MIDI send flag - sending note off

    	}

/* comph 20 */
    	if((P9IN & 0x02) && MIDI[P5_1]){

    		MIDINoteOff[1] = Pin2MIDI[P5_1];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P5_1] = 0;									//clear MIDI send flag - sending note off

    	}

/* comph 21 */
    	if((P8IN & 0x10) && MIDI[P5_0]){

    		MIDINoteOff[1] = Pin2MIDI[P5_0];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P5_0] = 0;									//clear MIDI send flag - sending note off
    	}
/* comph 22 */
    	if((P8IN & 0x80) && MIDI[P3_5]){

       		MIDINoteOff[1] = Pin2MIDI[P3_5];
       		Off = MIDINoteOff;
       		UARTSendArray(Off,MIDILength);				//send NoteOFF message
       		MIDI[P3_5] = 0;									//clear MIDI send flag - sending note off

       	}

/* comph 23 */
    	if((P9IN & 0x01) && MIDI[P5_2]){

    		MIDINoteOff[1] = Pin2MIDI[P5_2];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P5_2] = 0;									//clear MIDI send flag - sending note off

    	}

/* comph 24 */
    	if((P8IN & 0x40) && MIDI[P3_7]){

    		MIDINoteOff[1] = Pin2MIDI[P3_7];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P3_7] = 0;									//clear MIDI send flag - sending note off

    	}
    	P7OUT = P7OUT ^ 1<<4;			//debugging pin
    }
}

void ISRsetup(void){

	//Configure P6 for inputs and interrupts
		P6SEL0 = 0x00;			//inputs
		P6SEL1 = 0x00;

		P6DIR = 0x00;			//set all inputs
		P6REN = 0x00;			//dont enable pulldowns/pullups

//Configure Port 3 for inputs and interrupts
		P3SEL0 = 0x00;			//inputs
		P3SEL1 = 0x00;

		P3DIR = 0x00;			//set all inputs
		P3REN = 0xFF;			//dont enable pulldowns/pullups -- change!!!!!!!!

//Configure Port 5 for inputs and interrupts
		P5SEL0 = 0x00;			//inputs
		P5SEL1 = 0x00;

		P5DIR = 0x00;			//set all inputs
		P5REN = 0x00;			//dont enable pulldowns/pullups


//Configure, Port 4 for inputs and interrupts
	    P4SEL0 = 0x00;
	    P4SEL1 = 0x00; 						//configure registers for i/o

	    P4DIR = 0x00;						//set all inputs
	    P4REN = 0x00;						//don't enable any pulldowns/pullups

//Configure Port 1 for inputs and interrupts
		P1SEL0 = 0x00;			//inputs
		P1SEL1 = 0x00;

		P1DIR = 0x00;			//set all inputs
		P1REN = 0x00;			//dont enable pulldowns/pullups


//Configure Port 2 for inputs and interrupts
		P2SEL0 = 0x00;			//inputs
		P2SEL1 = 0x00;

		P2DIR = 0x00;			//set all inputs,except P2.2
		P2REN = 0x00;			//dont enable pulldowns/pullups

/////////////*configure rest of ports *////////////////

/*port 9 */
		P9SEL1 = 0x00;			//inputs
		// P9SEL0 = 0x00; --> defined in uart code for MIDI
		P9REN = 0x00;			//dont enable pulldowns/pullups
		P9DIR = 0x80;			//set all inputs, except for P9.7(uart)

/*port 8 */
		P8SEL0 = 0x00;			//inputs
		P8SEL1 = 0x00;
		P8REN = 0x00;
		P8DIR = 0x00;

/*port 7
		P7SEL0 = 0x00;			//all inputs
		P7SEL1 = 0x00;
		P7REN = 0x00;
		P7DIR = 0x10;
		P7OUT = 0x00;			//set debugging output high
*/
}

void UARTset (void){
	
    // Setup UART
    UARTSetup();


    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    // Enable eUSCIA3 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA3_IRQn) & 31);
}
