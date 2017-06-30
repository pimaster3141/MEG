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
uint8_t a = 0;							//key offset
static volatile uint8_t Pin2MIDI[NumOfKeys] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57,               //Map Pin diagram to Key notes
												58,59,60,61,62,63,64,65,66,67,68,69,70,71,72};
volatile uint8_t flag[NumOfKeys] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
volatile uint8_t tout[NumOfKeys] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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
	P7SEL0 = 0x00;			//all inputs
	P7SEL1 = 0x00;
	P7REN = 0x00;
	P7DIR = 0x10;
	P7OUT = 0x00;			//set debugging output high
	//printf("\n MIDI = %u\n",1);
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
    		P6IES |= 1  ; // writes 1 to enable interrupt

    	    	}


/*comph 2*/
    	if((P2IN & 0x40) && MIDI[P3_2]){

    		MIDINoteOff[1] = Pin2MIDI[P3_2];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P3_2] = 0;									//clear MIDI send flag - sending note off


    	}

/*comph 3 */
    	if((P2IN & 0x20) && MIDI[P6_1]){

    		MIDINoteOff[1] = Pin2MIDI[P6_1];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);		//send NoteOFF message
    		MIDI[P6_1] = 0;						//clear MIDI send flag - sending note off
    		//P7OUT = P7OUT ^ 1<<4;				//debugging pin
    	}

/* comph 4 */
    	if((P2IN & 0x10) && MIDI[P3_3]){

    		MIDINoteOff[1] = Pin2MIDI[P3_3];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P3_3] = 0;									//clear MIDI send flag - sending note off

    	}

/* comph 5 */
    	if((P3IN & 0x01) && MIDI[P4_0]){

    		MIDINoteOff[1] = Pin2MIDI[P4_0];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_0] = 0;									//clear MIDI send flag - sending note off

    	}

/* comph 6 */
    	if((P5IN & 0x40) && MIDI[P4_1]){

    		MIDINoteOff[1] = Pin2MIDI[P4_1];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_1] = 0;									//clear MIDI send flag - sending note off
    		//P7OUT = P7OUT ^ 1<<4;				//debugging pin
    	}

/* comph 7 */
    	if((P5IN & 0x80) && MIDI[P4_2]){

    		MIDINoteOff[1] = Pin2MIDI[P4_2];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_2] = 0;									//clear MIDI send flag - sending note off

    	}

/* comph 8 */
    	if((P6IN & 0x40) && MIDI[P4_3]){

    		MIDINoteOff[1] = Pin2MIDI[P4_3];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_3]= 0;									//clear MIDI send flag - sending note off

    	}

/*comph 9 */
    	if((P6IN & 0x80) && MIDI[P4_4]){

    		MIDINoteOff[1] = Pin2MIDI[P4_4];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_4] = 0;									//clear MIDI send flag - sending note off
    	}

/* comph 10 */
    	if((P1IN & 0x40) && MIDI[P1_5]){

    		MIDINoteOff[1] = Pin2MIDI[P1_5];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P1_5] = 0;									//clear MIDI send flag - sending note off

    	}

/* comph 11 */
    	if((P7IN & 0x02) && MIDI[P4_5]){

    		MIDINoteOff[1] = Pin2MIDI[P4_5];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_5] = 0;									//clear MIDI send flag - sending note off

    	}
/* comph 12 */
    	if((P6IN & 0x08) && MIDI[P4_6]){

    		MIDINoteOff[1] = Pin2MIDI[P4_6];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_6] = 0;									//clear MIDI send flag - sending note off

    	}
/* comph 13 */
    	if((P7IN & 0x08) && MIDI[P4_7]){

    		MIDINoteOff[1] = Pin2MIDI[P4_7];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P4_7] = 0;									//clear MIDI send flag - sending note off

    	}

/* comph 14---- needs work (*/
    	if((P9IN & 0x08) && MIDI[P6_5]){

    		MIDINoteOff[1] = Pin2MIDI[P6_5];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P6_5] = 0;									//clear MIDI send flag - sending note off

    	}

/*comph 15 */
    	if((P6IN & 0x04) && MIDI[P5_4]){

    		MIDINoteOff[1] = Pin2MIDI[P5_4];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P5_4] = 0;									//clear MIDI send flag - sending note off

    	}

/*comph 16 */
    	if((P5IN & 0x08) && MIDI[P6_4]){

    		MIDINoteOff[1] = Pin2MIDI[P6_4];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P6_4] = 0;									//clear MIDI send flag - sending note off

    	}

/* comph 17 */
    	if((P9IN & 0x04) && MIDI[P5_5]){

    		MIDINoteOff[1] = Pin2MIDI[P5_5];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P5_5] = 0;									//clear MIDI send flag - sending note off

    	}

/* comph 18 */
    	if((P8IN & 0x08) && MIDI[P2_3]){

    		MIDINoteOff[1] = Pin2MIDI[P2_3];
    		Off = MIDINoteOff;
    		UARTSendArray(Off,MIDILength);				//send NoteOFF message
    		MIDI[P2_3] = 0;									//clear MIDI send flag - sending note off

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

/*port ISR setup function - sets inputs and interrupts on all relevant ports */

void ISRsetup(void){

	//Configure P6 for inputs and interrupts
		P6SEL0 = 0x00;			//inputs
		P6SEL1 = 0x00;

		P6DIR = 0x00;			//set all inputs
		P6REN = 0x00;			//dont enable pulldowns/pullups

		P6IFG = 0x00; 			//no interrupt flags
		P6IES = 0x33;			//interrupts on 0,1,4,5
		MAP_Interrupt_enableInterrupt(INT_PORT6);

		/*set interrupts*/
		P6IE = 0x33;

//Configure Port 3 for inputs and interrupts
		P3SEL0 = 0x00;			//inputs
		P3SEL1 = 0x00;

		P3DIR = 0x00;			//set all inputs
		P3REN = 0x00;			//dont enable pulldowns/pullups

		P3IFG = 0x00; 			//no interrupt flags
		P3IES = 0xEC;			//interrupts on 0,1,4,5
		MAP_Interrupt_enableInterrupt(INT_PORT3);

	/*set interrupts  */
		P3IE = 0xEC;

//Configure Port 5 for inputs and interrupts
		P5SEL0 = 0x00;			//inputs
		P5SEL1 = 0x00;

		P5DIR = 0x00;			//set all inputs
		P5REN = 0x00;			//dont enable pulldowns/pullups

	   	P5IFG = 0x00; 			//no interrupt flags
		P5IES = 0x37;			//interrupts on 0,1,4,5
		MAP_Interrupt_enableInterrupt(INT_PORT5);

/*set interrupts */
		P5IE = 0x37;


//Configure, Port 4 for inputs and interrupts
	    P4SEL0 = 0x00;
	    P4SEL1 = 0x00; 						//configure registers for i/o

	    P4DIR = 0x00;						//set all inputs
	    P4REN = 0x00;						//don't enable any pulldowns/pullups

	    P4IFG = 0x00; 								//clear all interrupt flags right away
	    P4IES = 0xFF; 								//enable interrupt for port 4 pin 2 and pin 3
	    MAP_Interrupt_enableInterrupt(INT_PORT4);	//enable interrupt

	    //Set interrupt
	    P4IE = 0xFF;

//Configure Port 1 for inputs and interrupts
		P1SEL0 = 0x00;			//inputs
		P1SEL1 = 0x00;

		P1DIR = 0x00;			//set all inputs
		P1REN = 0x00;			//dont enable pulldowns/pullups

		P1IFG = 0x00; 			//no interrupt flags
		P1IES = 0xA0;			//interrupts
		MAP_Interrupt_enableInterrupt(INT_PORT1);

			/*set interrupts */
		P1IE = 0xA0;


//Configure Port 2 for inputs and interrupts
		P2SEL0 = 0x00;			//inputs
		P2SEL1 = 0x00;

		P2DIR = 0x00;			//set all inputs,except P2.2
		P2REN = 0x00;			//dont enable pulldowns/pullups
		P2IFG = 0x00; 			//no interrupt flags
		P2IES = 0x08;			//interrupts on 0,1,4,5
		MAP_Interrupt_enableInterrupt(INT_PORT2);

/*set interrupts */
		P2IE= 0x08;

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


//enable all interrupts to processor
	    MAP_Interrupt_enableMaster();

	    __enable_irq();
}


void UARTset (void){
	
    // Setup UART
    UARTSetup();


    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

    // Enable eUSCIA3 interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((EUSCIA3_IRQn) & 31);
}

/* this function converts velocity value to 1-127 volume value*/

uint8_t convert_velocity(uint16_t vel){

	//printf("\nveloc = %u\n",vel);
		if (vel > 4000){								//cap off to go in our range
			vel = 4000;
    			}

    	if ((vel < 500)){
    		vel = 500;
    			}
    	/*if (vel < 50){
    		vel = 3000;
    			}
*/
    	float temp = vel;
    	temp = ((temp-500)/(3500))*127.0;						//divide into our range (1-127)

    	uint8_t vol = temp; 							//get integer value

    	if(vol<10){
    		vol = 10;
    			}
    	if(vol > 120){
    		vol = 120;
    			}
    	uint8_t volume = 128 - vol;
    	//printf("\n Vol = %u\n",volume);
    		return volume;
}


void KeyISR6 (int pin,int shift){
    		if (flag[pin] == 1){							//check int transition value - here a falling edge
    				//Ttest1 = TA1R;
    				T1[pin] = TA1R; 								//grab current timer 1 value
    				//printf("\n Timer1 = %u\n",T1); 				//debug command
    				//P3OUT = P3OUT ^ 1<<3; 				//toggle output pin (debugging statement)
    				P6IES &= ~(1<<shift); // writes 0 to shift
    				flag[pin] = 0;
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
    				//printf("\nveloc = %u\n",velocity);
    				}

    			else{
    				velocity = T2[pin] - T1[pin];
    				//printf("\nveloc = %u\n",velocity);
    			}
    			//uint16_t veloc = Ttest2 - Ttest1;
    			//printf("\nveloc = %u\n",veloc);
    			if(velocity > 30){
        			velocity = convert_velocity(velocity);				//convert velocity value to volume value
        			MIDINoteOn[2] = velocity;							//replace Note on buffer with proper velocity
        			MIDINoteOn[1] = Pin2MIDI[pin];						//map proper MIDI note value to MIDI array
      				On = MIDINoteOn;										//pointer initialization
        			UARTSendArray(On,MIDILength);						//Send MIDI note!
        			MIDI[pin] = 1;
        			P6IES |= 1<<shift; // writes 1 to shift
        			P6IE &= ~(1<<shift); // writes 0 to interrupt
        			flag[pin] = 1; 									//reset ISR flag

    			}
    		}
}

void KeyISR4 (int pin,int shift){
    		if (flag[pin] == 1){							//check int transition value - here a falling edge

    			if(MIDI[pin])
    			{	}					//check whether MIDI note has been sent - if so, do nothing

    			else{
    				Ttest1 = TA1R;
    				T1[pin] = TA1R; 								//grab current timer 1 value
    				//printf("\n Timer1 = %u\n",T1); 				//debug command
    				//P3OUT = P3OUT ^ 1<<3; 				//toggle output pin (debugging statement)
    				P4IES &= ~(1<<shift); // writes 0 to shift

    				flag[pin] = 0;
    			}
    		}

    		else {											//this is now a rising edge
    			T2[pin] = TA1R;
    			Ttest2 = TA1R;
    			//printf("\n timer1= %u\n",T1);
    			//printf("\n timer2= %u\n",T2);
    			//printf("\n Timer2 = %u\n",T2);

    			/*check if our timer has overflowed in the meantime */
    			if (T1[pin] >= T2[pin]){
    				T1[pin] = (0xFFFF) - T1[pin];
    				velocity = T2[pin] + T1[pin];									//calculate velocity value and send MIDI note
    				//printf("\nveloc = %u\n",velocity);
    				}

    			else{
    				velocity = T2[pin] - T1[pin];
    				//printf("\nveloc = %u\n",velocity);
    			}
    			//uint16_t veloc = Ttest2 - Ttest1;
    			//printf("\nveloc = %u\n",veloc);
    			if(velocity > 100){
        			velocity = convert_velocity(velocity);				//convert velocity value to volume value
        			MIDINoteOn[2] = velocity;							//replace Note on buffer with proper velocity
        			MIDINoteOn[1] = Pin2MIDI[pin];						//map proper MIDI note value to MIDI array
      				On = MIDINoteOn;										//pointer initialization
        			UARTSendArray(On,MIDILength);						//Send MIDI note!
        			MIDI[pin] = 1;
    			}


    			            //notify a MIDI note ON has been sent
    			P4IES |= 1<<shift; // writes 1 to shift


    			flag[pin] = 1; 									//reset ISR flag


    		}
}

void KeyISR2 (int pin,int shift){
    		if (flag[pin] == 1){							//check int transition value - here a falling edge

    			if(MIDI[pin])
    			{	}					//check whether MIDI note has been sent - if so, do nothing

    			else{
    				Ttest1 = TA1R;
    				T1[pin] = TA1R; 								//grab current timer 1 value
    				//printf("\n Timer1 = %u\n",T1); 				//debug command
    				//P3OUT = P3OUT ^ 1<<3; 				//toggle output pin (debugging statement)
    				P2IES &= ~(1<<shift); // writes 0 to shift
    				flag[pin] = 0;
    			}
    		}

    		else {											//this is now a rising edge
    			T2[pin] = TA1R;
    			Ttest2 = TA1R;
    			//printf("\n timer1= %u\n",T1);
    			//printf("\n timer2= %u\n",T2);
    			//printf("\n Timer2 = %u\n",T2);

    			/*check if our timer has overflowed in the meantime */
    			if (T1[pin] >= T2[pin]){
    				T1[pin] = (0xFFFF) - T1[pin];
    				velocity = T2[pin] + T1[pin];									//calculate velocity value and send MIDI note
    				//printf("\nveloc = %u\n",velocity);
    				}

    			else{
    				velocity = T2[pin] - T1[pin];
    				//printf("\nveloc = %u\n",velocity);
    			}
    			//uint16_t veloc = Ttest2 - Ttest1;
    			//printf("\nveloc = %u\n",veloc);
    			if(velocity > 100){
        			velocity = convert_velocity(velocity);				//convert velocity value to volume value
        			MIDINoteOn[2] = velocity;							//replace Note on buffer with proper velocity
        			MIDINoteOn[1] = Pin2MIDI[pin];						//map proper MIDI note value to MIDI array
      				On = MIDINoteOn;										//pointer initialization
        			UARTSendArray(On,MIDILength);						//Send MIDI note!
        			MIDI[pin] = 1;
    			}


    			            //notify a MIDI note ON has been sent
    			P2IES |= 1<<shift; // writes 1 to shift
    			flag[pin] = 1; 									//reset ISR flag


    		}
}

void KeyISR5 (int pin,int shift){
    		if (flag[pin] == 1){							//check int transition value - here a falling edge

    			if(MIDI[pin])
    			{	}					//check whether MIDI note has been sent - if so, do nothing

    			else{
    				Ttest1 = TA1R;
    				T1[pin] = TA1R; 								//grab current timer 1 value
    				//printf("\n Timer1 = %u\n",T1); 				//debug command
    				//P3OUT = P3OUT ^ 1<<3; 				//toggle output pin (debugging statement)
    				P5IES &= ~(1<<shift); // writes 0 to shift
    				flag[pin] = 0;
    			}
    		}

    		else {											//this is now a rising edge
    			T2[pin] = TA1R;
    			Ttest2 = TA1R;
    			//printf("\n timer1= %u\n",T1);
    			//printf("\n timer2= %u\n",T2);
    			//printf("\n Timer2 = %u\n",T2);

    			/*check if our timer has overflowed in the meantime */
    			if (T1[pin] >= T2[pin]){
    				T1[pin] = (0xFFFF) - T1[pin];
    				velocity = T2[pin] + T1[pin];									//calculate velocity value and send MIDI note
    				//printf("\nveloc = %u\n",velocity);
    				}

    			else{
    				velocity = T2[pin] - T1[pin];
    				//printf("\nveloc = %u\n",velocity);
    			}
    			//uint16_t veloc = Ttest2 - Ttest1;
    			//printf("\nveloc = %u\n",veloc);
    			if(velocity > 100){
        			velocity = convert_velocity(velocity);				//convert velocity value to volume value
        			MIDINoteOn[2] = velocity;							//replace Note on buffer with proper velocity
        			MIDINoteOn[1] = Pin2MIDI[pin];						//map proper MIDI note value to MIDI array
      				On = MIDINoteOn;										//pointer initialization
        			UARTSendArray(On,MIDILength);						//Send MIDI note!
        			MIDI[pin] = 1;
    			}


    			            //notify a MIDI note ON has been sent
    			P5IES |= 1<<shift; // writes 1 to shift
    			flag[pin] = 1; 									//reset ISR flag


    		}
}

void KeyISR3 (int pin,int shift){
    		if (flag[pin] == 1){							//check int transition value - here a falling edge

    			if(MIDI[pin])
    			{	}					//check whether MIDI note has been sent - if so, do nothing

    			else{
    				Ttest1 = TA1R;
    				T1[pin] = TA1R; 								//grab current timer 1 value
    				//printf("\n Timer1 = %u\n",T1); 				//debug command
    				//P3OUT = P3OUT ^ 1<<3; 				//toggle output pin (debugging statement)
    				P3IES &= ~(1<<shift); // writes 0 to shift
    				flag[pin] = 0;
    			}
    		}

    		else {											//this is now a rising edge
    			T2[pin] = TA1R;
    			Ttest2 = TA1R;
    			//printf("\n timer1= %u\n",T1);
    			//printf("\n timer2= %u\n",T2);
    			//printf("\n Timer2 = %u\n",T2);

    			/*check if our timer has overflowed in the meantime */
    			if (T1[pin] >= T2[pin]){
    				T1[pin] = (0xFFFF) - T1[pin];
    				velocity = T2[pin] + T1[pin];									//calculate velocity value and send MIDI note
    				//printf("\nveloc = %u\n",velocity);
    				}

    			else{
    				velocity = T2[pin] - T1[pin];
    				//printf("\nveloc = %u\n",velocity);
    			}
    			//uint16_t veloc = Ttest2 - Ttest1;
    			//printf("\nveloc = %u\n",veloc);
    			if(velocity > 100){
        			velocity = convert_velocity(velocity);				//convert velocity value to volume value
        			MIDINoteOn[2] = velocity;							//replace Note on buffer with proper velocity
        			MIDINoteOn[1] = Pin2MIDI[pin];						//map proper MIDI note value to MIDI array
      				On = MIDINoteOn;										//pointer initialization
        			UARTSendArray(On,MIDILength);						//Send MIDI note!
        			MIDI[pin] = 1;
    			}


    			            //notify a MIDI note ON has been sent
    			P3IES |= 1<<shift; // writes 1 to shift
    			flag[pin] = 1; 									//reset ISR flag


    		}
}

void KeyISR1 (int pin,int shift){
    		if (flag[pin] == 1){							//check int transition value - here a falling edge

    			if(MIDI[pin])
    			{	}					//check whether MIDI note has been sent - if so, do nothing

    			else{
    				T1[pin] = TA1R; 								//grab current timer 1 value
    				//printf("\n Timer1 = %u\n",T1); 				//debug command
    				//P3OUT = P3OUT ^ 1<<3; 				//toggle output pin (debugging statement)
    				P1IES &= ~(1<<shift); // writes 0 to shift
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
    				//printf("\n MIDI = %u\n",1);


    				}
    			else{
    				velocity = T2[pin] - T1[pin];
    				velocity = convert_velocity(velocity);				//convert velocity value to volume value
    				MIDINoteOn[2] = velocity;							//replace Note on buffer with proper velocity
    				MIDINoteOn[1] = Pin2MIDI[pin];						//map proper MIDI note value to MIDI array
    				On = MIDINoteOn;										//pointer initialization
    				UARTSendArray(On, MIDILength);						//Send MIDI note!
    			}
    			MIDI[pin] = 1; 									//notify a MIDI note ON has been sent
    			P1IES |= 1<<shift; // writes 1 to shift
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
    		//printf("\nMIDI =%u \n",5);
    		break;

    		case 0x04 :
    		KeyISR4(P4_1,1);
    		//printf("\nMIDI =%u \n",6);
    		break;

    		case 0x06 :
    		KeyISR4(P4_2,2);
    		//printf("\nMIDI =%u \n",7);
    		break;

    		case 0x08 :
    		KeyISR4(P4_3,3);
    		//printf("\nMIDI =%u \n",8);
    		break;

    		case 0x0A :
    		KeyISR4(P4_4,4);
    		//printf("\nMIDI =%u \n",9);
    		break;

    		case 0x0C :
        	KeyISR4(P4_5,5);
        	//printf("\nMIDI =%u \n",11);
    		break;

    		case 0x0E :
        	KeyISR4(P4_6,6);
        	//printf("\nMIDI =%u \n",12);
    		break;

    		case 0x10 :
        	KeyISR4(P4_7,7);
    		break;
    }
}
void PORT3_IRQHandler(void)
{
    uint16_t status;

    status = P3IV & 0x001F;	 							//find out which interrupt triggered the interrupt
    													//interrupt also gets cleared by reading P3IV

    //Toggling the output debug pin,based on which pin trigger the interrupt - determined by switch statement

    switch(status){

    		case 0x00 :
			break;

    		case 0x02:
    		break;

    		case 0x04 :
    		break;

    		case 0x06 :
    		KeyISR3(P3_2,2);
    		//printf("\nMIDI =%u \n",2);
    		break;

    		case 0x08 :
    		KeyISR3(P3_3,3);
    		//printf("\nMIDI =%u \n",4);
    		break;

    		case 0x0A :
    		break;

    		case 0x0C :
        	KeyISR3(P3_5,5);
    		break;

    		case 0x0E :
        	KeyISR3(P3_6,6);
    		break;

    		case 0x10 :
        	KeyISR3(P3_7,7);
    		break;
    }
}

void PORT2_IRQHandler(void)
{
    uint16_t status;

    status = P2IV & 0x001F;	 							//find out which interrupt triggered the interrupt
    													//interrupt also gets cleared by reading P3IV

    //Toggling the output debug pin,based on which pin trigger the interrupt - determined by switch statement

    switch(status){

    		case 0x00 :
			break;

    		case 0x02:
    		break;

    		case 0x04 :
    		break;

    		case 0x06 :
    		break;

    		case 0x08 :
    		KeyISR2(P2_3,3);
    		break;

    		case 0x0A :
    		break;

    		case 0x0C :
    		break;

    		case 0x0E :
    		break;

    		case 0x10 :
    		break;
    }
}

void PORT5_IRQHandler(void)
{
    uint16_t status;

    status = P5IV & 0x001F;	 							//find out which interrupt triggered the interrupt
    													//interrupt also gets cleared by reading P3IV

    //Toggling the output debug pin,based on which pin trigger the interrupt - determined by switch statement

    switch(status){

    		case 0x00 :
			break;

    		case 0x02:
    		KeyISR5(P5_0,0);
    		break;

    		case 0x04 :
    		KeyISR5(P5_1,1);
    		break;

    		case 0x06 :
    		KeyISR5(P5_2,2);
    		break;

    		case 0x08 :
    		break;

    		case 0x0A :
    		KeyISR5(P5_4,4);
    		break;

    		case 0x0C :
    	    KeyISR5(P5_5,5);
    		break;

    		case 0x0E :
    		break;

    		case 0x10 :
    		break;
    }
}

void PORT1_IRQHandler(void)
{
    uint16_t status;

    status = P1IV & 0x001F;	 							//find out which interrupt triggered the interrupt
    													//interrupt also gets cleared by reading P3IV

    //Toggling the output debug pin,based on which pin trigger the interrupt - determined by switch statement

    switch(status){

    		case 0x00 :
			break;

    		case 0x02:
    		break;

    		case 0x04 :
    		break;

    		case 0x06 :
    		break;

    		case 0x08 :
    		break;

    		case 0x0A :
    		break;

    		case 0x0C :
    	    KeyISR1(P1_5,5);
    		break;

    		case 0x0E :
    		break;

    		case 0x10 :
    		KeyISR1(P1_7,7);
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
    		//printf("\nMIDI =%u \n",99);
    		KeyISR6(P6_0,0);
    		break;

    		case 0x04 :
    		KeyISR6(P6_1,1);
    		//printf("\nMIDI =%u \n",3);
    		break;

    		case 0x06 :
    		break;

    		case 0x08 :
    		break;

    		case 0x0A :
    		KeyISR6(P6_4,4);
    		//printf("\nMIDI =%u \n",4);
    		break;

    		case 0x0C :
    		KeyISR6(P6_5,5);
    		//printf("\nMIDI =%u \n",5);
    		break;

    		case 0x0E :
    		break;

    		case 0x10 :
    		break;
    }
}




