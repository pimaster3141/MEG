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
#define P6_1 1
#define P4_0 2
#define P4_1 3
#define P4_2 4
#define P4_3 5
#define P4_4 6
#define P4_5 7
#define P4_6 8
#define P4_7 9
#define P6_5 10
#define P6_4 11

/*
#define P2_5 2
#define P2_6 3
#define P2_7 4
#define P4_0 5
#define P4_1 6
#define P4_2 7
#define P4_3 8
#define P4_4 9
#define P4_5 10
#define P4_6 11
#define P4_7 12
#define P5_0 13
#define P5_1 14
#define P5_2 15
#define P5_4 16
#define P5_5 17
#define P5_6 18
#define P5_7 19
#define P6_0 20
#define P6_1 21
#define P6_4 22
#define P6_5 23
#define P6_6 24
#define P6_7 25
*/
#define NumOfKeys 25

static volatile uint8_t Pin2MIDI[NumOfKeys] = {48, 49, 50, 51, 52, 53, 54, 55, 60, 57,               //Map Pin diagram to Key notes
												58,59,60,61,62,63,64,65,66,67,68,69,70,71,72};
volatile uint8_t flag[NumOfKeys] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
volatile uint8_t MIDI[NumOfKeys];									//all of these arrays initalized to zero
volatile uint16_t T1[NumOfKeys];
volatile uint16_t T2[NumOfKeys];
volatile uint16_t Ttest1;
volatile uint16_t Ttest2;
volatile uint16_t velocity;
//uint16_t velocity[NumOfKeys] = [0];

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
    /* quick test for timer */


    	if((P8IN & 0x20) && MIDI[P6_0]){
    		MIDINoteOff[1] = Pin2MIDI[P6_0];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				    //send NoteOFF message
    		P2OUT = 0x00;
    		MIDI[P6_0] = 0;									//clear MIDI send flag - sending note off
    		P2OUT = P2OUT ^ 1<<4;
    	}

    	if((P9IN & 0x01) && MIDI[P6_1]){

    		MIDINoteOff[1] = Pin2MIDI[P6_1];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P6_1] = 0;									//clear MIDI send flag - sending note off
    		printf("\nMIDI = %u\n",0);				//debug statement
    		Ttest1 = 0;
    		    	int sem = 0;
    		    	uint16_t dumm = 0;
    		    	Ttest2 = 2;
    		    	if(sem == 0){
    		    	while(Ttest2 - Ttest1 < 50){
    		    		if (sem == 0){
    		    			sem = 1;
    		    			Ttest1 = TA1R;
    		    			P2OUT = P2OUT ^ 1<<5;
    		    		}
    		    		Ttest2 = TA1R;
    		    		if(Ttest2 < Ttest1){
    		    			dumm = Ttest1;
    		    			Ttest1 = Ttest1 - Ttest2;
    		    			Ttest2 = 65535;
    		    		}

    		    	}
    		    	P2OUT = P2OUT ^ 1<<5;
    		    	}

    		}


    	if((P8IN & 0x10) && MIDI[P4_0]){

    		MIDINoteOff[1] = Pin2MIDI[P4_0];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_0] = 0;									//clear MIDI send flag - sending note off
    		P2OUT = P2OUT ^ 1<<4;

    	}
    	if((P8IN & 0x04) && MIDI[P4_1]){

    		MIDINoteOff[1] = Pin2MIDI[P4_1];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_1] = 0;									//clear MIDI send flag - sending note off
    		P2OUT = P2OUT ^ 1<<4;

    	}
    	if((P9IN & 0x04) && MIDI[P4_2]){

    		MIDINoteOff[1] = Pin2MIDI[P4_2];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_2] = 0;									//clear MIDI send flag - sending note off
    		P2OUT = P2OUT ^ 1<<4;

    	}
    	if((P6IN & 0x04) && MIDI[P4_3]){

    		MIDINoteOff[1] = Pin2MIDI[P4_3];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_3] = 0;									//clear MIDI send flag - sending note off
    		P2OUT = P2OUT ^ 1<<4;

    	}
    	if((P8IN & 0x40) && MIDI[P4_4]){

    		MIDINoteOff[1] = Pin2MIDI[P4_4];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_4] = 0;									//clear MIDI send flag - sending note off
    		P2OUT = P2OUT ^ 1<<4;

    	}
    	if((P8IN & 0x80) && MIDI[P4_5]){

    		MIDINoteOff[1] = Pin2MIDI[P4_5];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_5] = 0;									//clear MIDI send flag - sending note off
    		P2OUT = P2OUT ^ 1<<4;

    	}
    	if((P9IN & 0x02 ) && MIDI[P4_6]){

    		MIDINoteOff[1] = Pin2MIDI[P4_6];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_6] = 0;									//clear MIDI send flag - sending note off
    		P2OUT = P2OUT ^ 1<<4;

    	}
    	if((P8IN & 0x08) && MIDI[P4_7]){

    		MIDINoteOff[1] = Pin2MIDI[P4_7];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_7] = 0;									//clear MIDI send flag - sending note off
    		P2OUT = P2OUT ^ 1<<4;

    	}
    	if((P5IN & 0x08) && MIDI[P6_5]){

    		MIDINoteOff[1] = Pin2MIDI[P6_5];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P6_5] = 0;									//clear MIDI send flag - sending note off
    		P2OUT = P2OUT ^ 1<<4;

    	}
    	if((P9IN & 0x08) && MIDI[P6_4]){

    		MIDINoteOff[1] = Pin2MIDI[P6_4];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P6_4] = 0;									//clear MIDI send flag - sending note off
    		P2OUT = P2OUT ^ 1<<4;

    	}

    }
}

/*port ISR setup function - sets inputs and interrupts on all relevant ports */

void ISRsetup(void){
	P2SEL0 = 0x00;
	P2SEL1 = 0x00;
	P2DIR = 0x20;					//set p2.5 to output
	P2REN = 0x00;
	P2OUT = 0x00;					//SET DEUBUG PIN TO LOW INITIALLY
	//Configure P6 for inputs and interrupts
		P6SEL0 = 0x00;			//inputs
		P6SEL1 = 0x00;

		P6DIR = 0x00;			//set all inputs
		P6REN = 0x00;			//dont enable pulldowns/pullups

		P6IFG = 0x00; 			//no interrupt flags
		P6IE = 0x33;			//interrupts on 0,1,4,5
		MAP_Interrupt_enableInterrupt(INT_PORT6);

		/*set transition of interrupts to high-low first */
		P6IES = 0x33;


	//Configure, P4.2 and P4.3 as input interrupt pin and P4.0 and P4.4  as input comparator
	    P4SEL0 = 0x00;
	    P4SEL1 = 0x00; 						//configure registers for i/o

	    P4DIR = 0x00;						//set all inputs
	    P4REN = 0x00;						//don't enable any pulldowns/pullups

	    P4IFG = 0x00; 								//clear all interrupt flags right away
	    P4IE = 0xFF; 								//enable interrupt for port 4 pin 2 and pin 3
	    MAP_Interrupt_enableInterrupt(INT_PORT4);	//enable interrupt

	    //Set transition of interrupt - high-low transition to begin
	    P4IES = 0xFF;

	    //configure port 8,5 and 9 for all inputs

		P8SEL0 = 0x00;			//inputs
		P8SEL1 = 0x00;
		P5SEL0 = 0x00;			//inputs
		P5SEL1 = 0x00;
		P9SEL0 = 0x00;			//inputs
		P9SEL1 = 0x00;


		P8DIR = 0x00;			//set all inputs
		P8REN = 0x00;			//dont enable pulldowns/pullups
		P5DIR = 0x00;			//set all inputs
		P5REN = 0x00;			//dont enable pulldowns/pullups
		P9DIR = 0x00;			//set all inputs
		P9REN = 0x00;			//dont enable pulldowns/pullups

	    //enable all interrupts to processor
	    MAP_Interrupt_enableMaster();

	    __enable_irq();
}


void UARTset (void){
	
    // Setup UART
    UARTSetup();

    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    // Enable eUSCIA2 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA2_IRQn) & 31);
}

/* this function converts velocity value to 1-127 volume value*/

uint8_t convert_velocity(uint16_t vel){


		if (vel > 15000){								//cap off to go in our range
			vel = 15000;
    			}

    	if (vel < 1100 & (vel != 5)){
    		vel = 1100;
    			}
    	if (vel == 5){
    		vel = 15000;
    			}

    	float temp = vel;
    	temp = ((temp-1100)/(13000))*127.0;						//divide into our range (1-127)

    	uint8_t vol = temp; 							//get integer value

    	if(vol<10){
    		vol = 10;
    			}
    	if(vol > 120){
    		vol = 120;
    			}
    	uint8_t volume = 128 - vol;

    		return volume;
}


void KeyISR6 (int pin,int shift){
    		if (flag[pin] == 1){							//check int transition value - here a falling edge

    			if(MIDI[pin])
    			{	}					//check whether MIDI note has been sent - if so, do nothing

    			else{
    				T1[pin] = TA1R; 								//grab current timer 1 value
    				//printf("\n Timer1 = %u\n",T1); 				//debug command
    				//P3OUT = P3OUT ^ 1<<3; 				//toggle output pin (debugging statement)
    				P6IES = P6IES ^ 1<<shift;					//toggle Int transition
    				flag[pin] = 0;
    			}
    		}

    		else {											//this is now a rising edge
    			T2[pin] = TA1R;
    			//printf("\n timer1= %u\n",T1);
    			//printf("\n timer2= %u\n",T2);
    			//printf("\n Timer2 = %u\n",T2);

    			/*check if our timer has overflowed in the meantime */
    			if (T1[pin] >= T2[pin]){
    				T1[pin] = (0xFFFF) - T1[pin];
    				velocity = T2[pin] + T1[pin];									//calculate velocity value and send MIDI note
    				velocity = convert_velocity(velocity);				//convert velocity value to volume value
    				MIDINoteOn[2] = velocity;							//replace Note on buffer with proper velocity
    				MIDINoteOn[1] = Pin2MIDI[pin];						//map proper MIDI note value to MIDI array
    				On = MIDINoteOn;										//pointer initialization
    				UARTSendArray(On,MIDILength);						//Send MIDI note!
    				printf("\n MIDI = %u\n",1);
    				Ttest1 = TA1R;


    				}
    			else{
    				velocity = T2[pin] - T1[pin];
    				velocity = convert_velocity(velocity);				//convert velocity value to volume value
    				MIDINoteOn[2] = velocity;							//replace Note on buffer with proper velocity
    				MIDINoteOn[1] = Pin2MIDI[pin];						//map proper MIDI note value to MIDI array
    				On = MIDINoteOn;										//pointer initialization
    				UARTSendArray(On, MIDILength);						//Send MIDI note!
    				P2OUT = P2OUT ^ 1<<4;
    				//printf("\n MIDI = %u\n",1);
    				Ttest1 = TA1R;
    			}
    			if(Pin2MIDI[pin] == 48){
    				//P2OUT = 0x20;
    			}
    			MIDI[pin] = 1; 									//notify a MIDI note ON has been sent
    			P6IES = P6IES ^ 1<<shift;						//also toggle IES flag to change the interrupt edge
    			flag[pin] = 1; 									//reset ISR flag
    			//printf("\n MIDI = %u\n", MIDI[pin]);
    			//printf("MIDI = %u", MIDI[pin]);
    		}
}

void KeyISR4 (int pin,int shift){
    		if (flag[pin] == 1){							//check int transition value - here a falling edge

    			if(MIDI[pin])
    			{	}					//check whether MIDI note has been sent - if so, do nothing

    			else{
    				T1[pin] = TA1R; 								//grab current timer 1 value
    				//printf("\n Timer1 = %u\n",T1); 				//debug command
    				//P3OUT = P3OUT ^ 1<<3; 				//toggle output pin (debugging statement)
    				P4IES = P4IES ^ 1<<shift;					//toggle Int transition
    				flag[pin] = 0;
    			}
    		}

    		else {											//this is now a rising edge
    			T2[pin] = TA1R;
    			//printf("\n timer1= %u\n",T1);
    			//printf("\n timer2= %u\n",T2);
    			//printf("\n Timer2 = %u\n",T2);

    			/*check if our timer has overflowed in the meantime */
    			if (T1[pin] >= T2[pin]){
    				T1[pin] = (0xFFFF) - T1[pin];
    				velocity = T2[pin] + T1[pin];									//calculate velocity value and send MIDI note
    				velocity = convert_velocity(velocity);				//convert velocity value to volume value
    				MIDINoteOn[2] = velocity;							//replace Note on buffer with proper velocity
    				MIDINoteOn[1] = Pin2MIDI[pin];						//map proper MIDI note value to MIDI array
    				On = MIDINoteOn;										//pointer initialization
    				UARTSendArray(On,MIDILength);						//Send MIDI note!
    				P2OUT = P2OUT ^ 1<<4;
    				//printf("\n MIDI = %u\n",1);


    				}
    			else{
    				velocity = T2[pin] - T1[pin];
    				velocity = convert_velocity(velocity);				//convert velocity value to volume value
    				MIDINoteOn[2] = velocity;							//replace Note on buffer with proper velocity
    				MIDINoteOn[1] = Pin2MIDI[pin];						//map proper MIDI note value to MIDI array
    				On = MIDINoteOn;										//pointer initialization
    				UARTSendArray(On, MIDILength);						//Send MIDI note!

    				//printf("\n MIDI = %u\n",1);
    			}

    			MIDI[pin] = 1; 									//notify a MIDI note ON has been sent
    			P4IES = P4IES ^ 1<<shift;						//also toggle IES flag to change the interrupt edge
    			flag[pin] = 1; 									//reset ISR flag
    			//printf("\n MIDI = %u\n", MIDI[pin]);
    			//printf("MIDI = %u", MIDI[pin]);
    		}
}

void PORT4_IRQHandler(void)
{
    uint16_t status;

    status = P4IV & 0x001F;	 							//find out which interrupt triggered the interrupt
    													//interrupt also gets cleared by reading P3IV

    //Toggling the output debug pin,based on which pin trigger the interrupt - determined by switch statement

    switch(status){

    		case 0x00 :
			break;

    		case 0x02:
    		KeyISR4(P4_0,0);
    		break;

    		case 0x04 :
    		KeyISR4(P4_1,1);
    		break;

    		case 0x06 :
    		KeyISR4(P4_2,2);
    		break;

    		case 0x08 :
    		KeyISR4(P4_3,3);
    		break;

    		case 0x0A :
    		KeyISR4(P4_4,4);
    		break;

    		case 0x0C :
        	KeyISR4(P4_5,5);
    		break;

    		case 0x0E :
        	KeyISR4(P4_6,6);
    		break;

    		case 0x10 :
        	KeyISR4(P4_7,7);
    		break;
    }
}

void PORT6_IRQHandler(void)
{
    uint16_t status;

    status = P6IV & 0x001F;	 							//find out which interrupt triggered the interrupt
    													//interrupt also gets cleared by reading P3IV

    //Toggling the output debug pin,based on which pin trigger the interrupt - determined by switch statement

    switch(status){

    		case 0x00 :
			break;

    		case 0x02:
    		KeyISR6(P6_0,0);
    		break;

    		case 0x04 :
    		KeyISR6(P6_1,1);
    		break;

    		case 0x06 :
    		break;

    		case 0x08 :
    		break;

    		case 0x0A :
    		KeyISR6(P6_4,4);
    		break;

    		case 0x0C :
    		KeyISR6(P6_5,5);
    		break;

    		case 0x0E :
    		break;

    		case 0x10 :
    		break;
    }
}




