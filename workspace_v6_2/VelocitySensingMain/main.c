
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>


/* ---------------------- HARDWARE DEFINES ---------------------- */
#define NUM_PORTS 9
#define KEY_OFFSET 47
#define KN 0 //null key
#define KO 128 //high comparator offset
#define MIDI_ON 0x90
#define MIDI_OFF 0x80
/* ---------------------- END HARDWARE DEFINES ---------------------- */



/* ---------------------- KEYMAPPING AND KEYSTATES ---------------------- */
// data structure to hold key timing values
uint32_t keyTimes[NUM_PORTS*8] = {};

// data structure to hold key mappings
const char MIDINote[NUM_PORTS*8] =
{//  0      1      2      3      4      5      6      7     //BIT
    KN   , KN   , 2    , KN   , KN   , 10   , KO+10, 19   ,     //Port1
    KN   , KN   , KN   , 18   , KO+4 , KO+3 , KO+2 , KO+1 ,     //Port2
    KO+5 , KN   , KN   , 4    , KN   , 22   , 25   , 24   ,     //Port3
    5    , 6    , 7    , 8    , 9    , 11   , 12   , 13   ,     //Port4
    21   , 20   , 23   , KO+16, 15   , 17   , KO+6 , KO+7 ,     //Port5
    1    , 3    , KO+15, KO+12, 16   , 14   , KO+8 , KO+9 ,     //Port6
    KN   , KO+11, KN   , KO+13, KN   , KN   , KN   , KN   ,     //Port7
    KN   , KN   , KO+19, KO+18, KO+21, KO+25, KO+24, KO+22,     //Port8
    KO+23, KO+20, KO+17, KO+14, KN   , KN   , KN   , KN         //Port9
};
/* ---------------------- END KEYMAPPING AND KEYSTATES---------------------- */




int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    /* Init Piano */
    setup();

    while(1)
    {
        
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

        CS->CTL0 = 0x00810000;
        CS->CTL1 = 0x50300033;
        CS->CLKEN = 0x0000000E;
        CS->KEY = 0x00010000;        //lock all clock registers


}

// Sets Up UART and associated ISR in NVIC
void UARTSet (void)
{
    UARTSetup();    // Setup UART Hardware
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR
    NVIC->ISER[0] = 1 << ((EUSCIA2_IRQn) & 31); // Enable eUSCIA2 interrupt in NVIC module
    return;
}

// Sets Up BNC and associated ISR if needed
void BNCSet(void)
{
    P10DIR = 0x3F;              //set outputs for BNC
    return;
}

// Sets up the timer for key velocity
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
    P1IE = 0xFF;
    MAP_Interrupt_enableInterrupt(INT_PORT1);

    P1IES = 0xFF;           //high to low transition

    /*Port 2*/
    P2IE = 0xFF;            //enable all interrupts
    MAP_Interrupt_enableInterrupt(INT_PORT2);

    P2IES = 0xFF;

    /*Port 3*/
    P3IE = 0xFF;            //enable all interrupts
    MAP_Interrupt_enableInterrupt(INT_PORT3);

    P3IES = 0xFF;

    /*Port 4*/
    P4IE = 0xFF;            //enable all interrupts
    MAP_Interrupt_enableInterrupt(INT_PORT4);

    P4IES = 0xFF;

    /*Port 5*/
    P5IE = 0xFF;            //enable all interrupts
    MAP_Interrupt_enableInterrupt(INT_PORT5);

    P5IES = 0xFF;

    /*Port 6*/
    P6IE = 0xFF;            //enable all interrupts
    MAP_Interrupt_enableInterrupt(INT_PORT6);

    P6IES = 0xFF;

    /*Port 7*/
    P7IE = 0xFF;            //enable all interrupts
    MAP_Interrupt_enableInterrupt(INT_PORT7);

    P7IES = 0xFF;

    /*Port 8*/
    P8IE = 0xFF;            //enable all interrupts
    MAP_Interrupt_enableInterrupt(INT_PORT8);

    P8IES = 0xFF;

    /*Port 9*/
    P9IE = 0x3F;            //enable interrupts, except 9.7, 9.6
    MAP_Interrupt_enableInterrupt(INT_PORT9);

    P9IES = 0x3F;

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
    char keyIndex = (portIndex-1)*8 + pinIndex;  //generate key lookup index
    char key = MIDINote[keyIndex];               //get key value from keymapping

    if((key & BIT7) && !fallingEdge)              //rising high comparator
        MIDIOff(key & ~(BIT7));

    else
    {
        uint16_t newTime = ;        //Todo: get current timer time;

        if(!fallingEdge)              //low comparator rising (play note)
        {
            uint16_t oldTime = keyTimes[keyIndex];
            uint16_t deltaTime = newTime - oldTime; //calculate velocity
            char velocity = convertVelocity(deltaTime);
            MIDIOn((key & ~(BIT7)), velocity);        //Play Note
        }
        else                        // low comparator falling (start timer)
        {
            keyTimes[keyIndex] = newTime;          // update key time
        }
    }
}

/* Converts a time delta to velocity
 * @param
 *  uint32_t deltaTime - 32bit unsigned timer value
 * @return
 *  char - velocity value [0-127]
 */
char convertVelocity(uint16_t deltaTime)
{
    // TODO:

    if (vel > 15000){                               //cap off to go in our range
            vel = 15000;
                }

        if (vel < 1100 & (vel != 5)){
            vel = 1100;
                }

        float temp = vel;
        temp = ((temp-1100)/(13000))*127.0;                     //divide into our range (1-127)

        uint8_t vol = temp;                             //get integer value

        if(vol<10){
            vol = 10;
                }
        if(vol > 120){
            vol = 120;
                }
        uint8_t volume = 128 - vol;

            return volume;

    return 127;
}

/* Inits a note on MIDI
 * @param
 *  char pitch - pitch of note offset by amount(?) [0-127]
 *  char volume - volume of note [0-127]
 *
 */
void MIDIOn(char pitch, char volume)
{
    char payload[3] = {MIDI_ON, pitch + KEY_OFFSET, volume};
    UARTSendArray(payload, 3);
//    __no_operation();
}

/* Halts note on MIDI
 * @param
 *  char pitch - pitch of note offset by amount(?) [0-127]
 */
void MIDIOff(char pitch)
{
    char payload[3] = {MIDI_OFF, pitch + KEY_OFFSET, 0};
    UARTSendArray(payload, 3);
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
    P1IES ^= mask;

    pinHandler(1, pin, fallingEdge);
}

void PORT2_IRQHandler(void)
{
    uint16_t pin = (P1IV >> 2) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P2IES & mask);
    P2IES ^= mask;

    pinHandler(2, pin, fallingEdge);
}

void PORT3_IRQHandler(void)
{
    uint16_t pin = (P1IV >> 3) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P3IES & mask);
    P3IES ^= mask;

    pinHandler(3, pin, fallingEdge);
}

void PORT4_IRQHandler(void)
{
    uint16_t pin = (P4IV >> 1) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P4IES & mask);
    P4IES ^= mask;

    pinHandler(4, pin, fallingEdge);
}

void PORT5_IRQHandler(void)
{
    uint16_t pin = (P5IV >> 1) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P5IES & mask);
    P5IES ^= mask;

    pinHandler(5, pin, fallingEdge);
}

void PORT6_IRQHandler(void)
{
    uint16_t pin = (P6IV >> 1) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P6IES & mask);
    P6IES ^= mask;

    pinHandler(6, pin, fallingEdge);
}

void PORT7_IRQHandler(void)
{
    uint16_t pin = (P7IV >> 1) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P7IES & mask);
    P7IES ^= mask;

    pinHandler(7, pin, fallingEdge);
}

void PORT8_IRQHandler(void)
{
    uint16_t pin = (P8IV >> 1) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P8IES & mask);
    P8IES ^= mask;

    pinHandler(8, pin, fallingEdge);
}

void PORT9_IRQHandler(void)
{
    uint16_t pin = (P9IV >> 1) - 1;                          //find out which interrupt triggered the interrupt
                                                             //returns value [0-7]
    char mask = 0x01 << pin;
    char fallingEdge = (P9IES & mask);
    P9IES ^= mask;

    pinHandler(9, pin, fallingEdge);
}
/* ---------------------- END KEY ISR HANDLERS ---------------------- */
