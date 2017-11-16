
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "msp432p401r.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <UART432.h>


/* ---------------------- HARDWARE DEFINES ---------------------- */
#define NUM_PORTS 9
#define NUM_KEYS 25
#define KEY_OFFSET 47
//#define KEY_OFFSET 0
#define KN 0 //null key
#define KO 128 //high comparator offset
#define MIDI_ON 0x90
#define MIDI_OFF 0x80
#define HACK_VALUE  50

#define LOW_TIME 500
#define HIGH_TIME 30000
#define NUM_TANGENTS 5

#define P1_LOW 0xA0
#define P2_LOW 0x08
#define P3_LOW 0xEC
#define P4_LOW 0xFF
#define P5_LOW 0x37
#define P6_LOW 0x33
/* ---------------------- END HARDWARE DEFINES ---------------------- */



/* ---------------------- KEYMAPPING AND KEYSTATES ---------------------- */
//const uint16_t velocityCutoffs[NUM_TANGENTS] = {0, 20860, 24982, 27417, 29152};
//const uint16_t velocitySlopes[NUM_TANGENTS] = {679, 138, 82, 58, 49};
//const char velocityIntercepts[NUM_TANGENTS] = {0, 30, 59, 88, 117};

const uint16_t velocityCutoffs[NUM_TANGENTS] = {0, 19902, 24000, 26426, 28156};
const uint16_t velocitySlopes[NUM_TANGENTS] = {784, 161, 96, 68, 53};
const char velocityIntercepts[NUM_TANGENTS] = {0, 25, 50, 75, 100};

// data structure to hold key timing values
static volatile uint16_t keyTimes[NUM_PORTS*8] = {};

// data structure to hold if MIDI has been sent out
static volatile char MIDISent[NUM_KEYS] = {};

// data structure to hold key mappings
const char MIDINote[NUM_PORTS*8] =
{//  0      1      2      3      4      5      6      7     //BIT
    KN   , KN   , KN   , KN   , KN   , 10   , KO+10, 19   ,     //Port1
    KN   , KN   , KN   , 18   , KO+4 , KO+3 , KO+2 , KO+1 ,     //Port2
    KO+5 , KN   , 2    , 4    , KN   , 22   , 25   , 24   ,     //Port3
    5    , 6    , 7    , 8    , 9    , 11   , 12   , 13   ,     //Port4
    21   , 20   , 23   , KO+16, 15   , 17   , KO+6 , KO+7 ,     //Port5
    1    , 3    , KO+15, KO+12, 16   , 14   , KO+8 , KO+9 ,     //Port6
    KN   , KO+11, KN   , KO+13, KN   , KN   , KN   , KN   ,     //Port7
    KN   , KN   , KO+19, KO+18, KO+21, KO+25, KO+24, KO+22,     //Port8
    KO+23, KO+20, KO+17, KO+14, KN   , KN   , KN   , KN         //Port9
};
/* ---------------------- END KEYMAPPING AND KEYSTATES---------------------- */



/* ---------------------- FUNCTION DECLARATIONS ---------------------- */
void keyRelease(char port, char portData);
void setup(void);
void SysSet(void);
void UARTSet (void);
void BNCSet(void);
void TimerSet(void);
void ISRSet(void);
void pinHandler(char portIndex, char pinIndex, char fallingEdge);
char convertVelocity(uint16_t deltaTime);
void MIDIOn(char pitch, char volume);
void MIDIOff(char pitch);
void PORT1_IRQHandler(void);
void PORT2_IRQHandler(void);
void PORT3_IRQHandler(void);
void PORT4_IRQHandler(void);
void PORT5_IRQHandler(void);
void PORT6_IRQHandler(void);
/* ---------------------- END FUNCTION DECLARATIONS ---------------------- */




int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    /* Init Piano */
    setup();

    while(1)
    {
        keyRelease(1, P1IN);
        keyRelease(2, P2IN);
        keyRelease(3, P3IN);
        keyRelease(4, P4IN);
        keyRelease(5, P5IN);
        keyRelease(6, P6IN);
        keyRelease(7, P7IN);
        keyRelease(8, P8IN);
        keyRelease(9, P9IN);
    }
}

void keyRelease(char port, char portData)
{
    char mask = 0x01;
    char bit;
    for(bit = 0; bit < 8; bit++)
    {
        char keyIndex = (port - 1)*8 + bit;
        char key = MIDINote[keyIndex];               //get key value from keymapping
        if((MIDINote[keyIndex] & BIT7)  && MIDISent[key & ~(BIT7)] && (portData & (mask << bit))) //IF(isHighComparator, MidiSentOut, pinIsHigh)
        {
            MIDIOff(MIDINote[keyIndex] & ~(BIT7));
            keyTimes[keyIndex] = 0;
            MIDISent[key & ~(BIT7)] = 0;
        }
    }
}



/* ---------------------- SETUP CODE ---------------------- */
// Sets up the board
void setup(void)
{
    SysSet();
    TimerSet();
    UARTSet();  //Init UART device and ISR
    BNCSet();   //Init BNC devices and ISR(?)
    ISRSet();   //Init Key device and ISR
    return;
}


void SysSet(void)
{

    /*unlock all the registers to be able to access*/

        CS->KEY = 0x0000695A;

    /*Following are primary registers for necessary clock functions*/

        CS->CTL0 = 0x00810000; //DCOEN, DCORSEL_1(3MHz),
        CS->CTL1 = 0x50300033; //DIVS(/32), DIVHS(/8), SELS(DCO), SELM(DCO)
        CS->CLKEN = 0x0000000E; //SMCLK_EN, HSMCLK_EN, MCLK_EN
        CS->KEY = 0x00010000;        //lock all clock registers


}

// Sets Up UART and associated ISR in NVIC
void UARTSet (void)
{
    UARTSetup();    // Setup UART Hardware
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR
    NVIC->ISER[0] = 1 << ((EUSCIA3_IRQn) & 31); // Enable eUSCIA2 interrupt in NVIC module
    return;
}

// Sets Up BNC and associated ISR if needed
void BNCSet(void)
{
    P10DIR = 0x3F;              //set outputs for BNC
    return;
}

// Sets up the timer for key deltaVelocity
void TimerSet(void)
{
/*Configure necessary timer registers for our timer frequency*/
    TA1CTL = 0x0220;
    TA1EX0 = 0x0000;            //use this to further divide our clock
    return;
}

// Sets up comparator interrupts and associated ISR in NVIC for all ports/keys
void ISRSet(void)
{
    /*Port 1*/
    P1IE = (P1_LOW);
    MAP_Interrupt_enableInterrupt(INT_PORT1);
    P1IES |= P1_LOW;                  //high to low transition

    /*Port 2*/
    P2IE |= (P2_LOW);                             //enable all interrupts
    MAP_Interrupt_enableInterrupt(INT_PORT2);
    P2IES |= P2_LOW;

    /*Port 3*/
    P3IE |= P3_LOW;        //enable all interrupts
    MAP_Interrupt_enableInterrupt(INT_PORT3);
    P3IES |= P3_LOW;

    /*Port 4*/
    P4IE |= P4_LOW;                               //enable all interrupts
    MAP_Interrupt_enableInterrupt(INT_PORT4);
    P4IES |= P4_LOW;

    /*Port 5*/
    P5IE |= P5_LOW; //enable all interrupts
    MAP_Interrupt_enableInterrupt(INT_PORT5);
    P5IES |= P5_LOW;

    /*Port 6*/
    P6IE |= P6_LOW;        //enable all interrupts
    MAP_Interrupt_enableInterrupt(INT_PORT6);
    P6IES |= P6_LOW;

    /*enable all interrupts to processor*/
    MAP_Interrupt_enableMaster();
    __enable_irq();
    return;
}
/* ---------------------- END SETUP CODE ---------------------- */




/* ---------------------- KEY OUTPUT CODE ---------------------- */
/* Unified key action function call
 * @param
 *  char portIndex - port of event [1-9]
 *  char pinIndex - pin of event [0-7]
 *  char fallingEdge - rising or falling edge event [0, 1]
 */
void pinHandler(char portIndex, char pinIndex, char fallingEdge)
{
//    printf("fallingEdge = %u\n", fallingEdge);
    char keyIndex = (portIndex-1)*8 + pinIndex;  //generate key lookup index
    char key = MIDINote[keyIndex];               //get key value from keymapping

    if(!MIDISent[key & ~(BIT7)])
    {
        uint16_t newTime = TA1R;        //get current timer time;
        uint16_t oldTime = keyTimes[keyIndex];
        uint16_t deltaTime = newTime - oldTime; //calculate velocity
        if(!fallingEdge && deltaTime > HACK_VALUE)              //low comparator rising, first edge
        {
            char deltaVelocity = convertVelocity(deltaTime);
            MIDIOn((key & ~(BIT7)), deltaVelocity);        //Play Note
            MIDISent[key & ~(BIT7)] = 1;
        }
        else                        // low comparator falling (start timer)
        {
            keyTimes[keyIndex] = newTime;          // update key time
        }
    }
    else
    {
        __no_operation();
    }
}

/* Converts a time delta to Velocity
 * @param
 *  uint32_t deltaTime - 32bit unsigned timer value
 * @return
 *  char - velocity value [0-127]
 */
//char convertVelocity(uint16_t deltaTime)
//{
////    printf("Delta = %u \n", deltaTime);
//
//    if (deltaTime > 15000){                               //cap off to go in our range
//            deltaTime = 15000;
//                }
//
//        if (deltaTime < 1100 & (deltaTime != 5)){
//            deltaTime = 1100;
//                }
//
//        float temp = deltaTime;
//        temp = ((temp-1100)/(13000))*127.0;                     //divide into our range (1-127)
//
//        uint8_t vol = temp;                             //get integer value
//
//        if(vol<10){
//            vol = 10;
//                }
//        if(vol > 120){
//            vol = 120;
//                }
//        uint8_t volume = 128 - vol;
//
//            return volume;
//
////    return 127;
//}

char convertVelocity(uint16_t deltaTime)
{
    if(deltaTime < LOW_TIME)    //limit values
        deltaTime = LOW_TIME;
    if(deltaTime > HIGH_TIME)
        deltaTime = HIGH_TIME;

    deltaTime = HIGH_TIME - deltaTime;  //Invert and scale from 0

    char tangent;
    for(tangent = 0; tangent < NUM_TANGENTS; tangent++)
    {
        char index = NUM_TANGENTS - 1 - tangent;
        if(deltaTime > velocityCutoffs[index])
            return ((deltaTime - velocityCutoffs[index])/velocitySlopes[index] + velocityIntercepts[index]) & (127);
    }
    return 0;
}

/* Inits a note on MIDI
 * @param
 *  char pitch - pitch of note offset by amount(?) [0-127]
 *  char volume - volume of note [0-127]
 *
 */
void MIDIOn(char pitch, char volume)
{
    P10OUT = pitch | BIT5;
    char payload[3] = {MIDI_ON, pitch + KEY_OFFSET, volume};
    UARTSendArray(payload, 3);
//    P10OUT = 0;
//    __no_operation();
}

/* Halts note on MIDI
 * @param
 *  char pitch - pitch of note offset by amount(?) [0-127]
 */
void MIDIOff(char pitch)
{
    P10OUT = pitch;
    char payload[3] = {MIDI_OFF, pitch + KEY_OFFSET, 0};
    UARTSendArray(payload, 3);
//    P10OUT = 0;
//    __no_operation();
}
/* ---------------------- END KEY OUTPUT CODE ---------------------- */



/* ---------------------- KEY ISR HANDLERS ---------------------- */
void PORT1_IRQHandler(void)
{
    uint16_t pin = (P1IV >> 1) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P1IES & mask);
    P1IES = (P1IES & ~mask) | (P1IN & mask);

    pinHandler(1, pin, fallingEdge);
}

void PORT2_IRQHandler(void)
{
    uint16_t pin = (P2IV >> 1) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P2IES & mask);
    P2IES = (P2IES & ~mask) | (P2IN & mask);

    pinHandler(2, pin, fallingEdge);
}

void PORT3_IRQHandler(void)
{
    uint16_t pin = (P3IV >> 1) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P3IES & mask);
    P3IES = (P3IES & ~mask) | (P3IN & mask);

    pinHandler(3, pin, fallingEdge);
}

void PORT4_IRQHandler(void)
{
    uint16_t pin = (P4IV >> 1) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P4IES & mask);
    P4IES = (P4IES & ~mask) | (P4IN & mask);

    pinHandler(4, pin, fallingEdge);
}

void PORT5_IRQHandler(void)
{
    uint16_t pin = (P5IV >> 1) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P5IES & mask);
    P5IES = (P5IES & ~mask) | (P5IN & mask);

    pinHandler(5, pin, fallingEdge);
}

void PORT6_IRQHandler(void)
{
    uint16_t pin = (P6IV >> 1) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P6IES & mask);
    P6IES = (P6IES & ~mask) | (P6IN & mask);

    pinHandler(6, pin, fallingEdge);
}
/* ---------------------- END KEY ISR HANDLERS ---------------------- */
