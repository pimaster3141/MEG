
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
    KN   , KN   , KN   , 18   , K0+4 , KO+3 , KO+2 , KO+1 ,     //Port2
    KO+5 , KN   , KN   , 4    , KN   , 22   , 25   , 24   ,     //Port3
    5    , 6    , 7    , 8    , 9    , 11   , 12   , 13   ,     //Port4
    21   , 20   , 23   , KO+16, 15   , 17   , KO+6 , KO+7 ,     //Port5
    1    , 3    , KO+15, KO+12, 16   , 14   , KO+8 , KO+9 ,     //Port6
    KN   , K0+11, KN   , KO+13, KN   , KN   , KN   , KN   ,     //Port7
    KN   , KN   , KO+19, KO+18, KO+21, KO+25, KO+24, KO+22,     //Port8
    K0+23, KO+20, KO+17, KO+14, KN   , KN   , KN   , KN         //Port9
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
    UARTSet();  //Init UART device and ISR
    BNCSet();   //Init BNC devices and ISR(?)
    TimerSet();
    ISRSet();   //Init Key device and ISR
    return;
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
    return;
}

// Sets up the timer for key velocity
void ISRSet(void)
{
    return;
}

// Sets up comparator interrupts and associated ISR in NVIC for all ports/keys
void ISRSet(void)
{
    return;
}
/* ---------------------- END SETUP CODE ---------------------- */




/* ---------------------- KEY OUTPUT CODE ---------------------- */
/* Unified key action function call TODO: copied from non velocity - may need editing
 * @param
 *  char portIndex - port of event [1-9]
 *  char pinIndex - pin of event [0-7]
 *  char risingEdge - rising or falling edge event [0, 1]
 */
void pinHandler(char portIndex, char pinIndex, char risingEdge)
{
    char keyIndex = (portIndex-1)*8 + pinIndex;  //generate key lookup index
    char key = MIDINote[keyIndex];               //get key value from keymapping

    if(key & BIT7)              //rising high comparator
        MIDIOff(key & ~(BIT7));

    else
    {
        uint32_t newTime = ;        //Todo: get current timer time;

        if(risingEdge)              //low comparator rising (play note)
        {
            uint32_t oldTime = keyTimes[keyIndex];
            uint32_t deltaTime = newTime - oldTime; //calculate velocity
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
char convertVelocity(uint32_t deltaTime)
{
    // TODO:
    return 127;
}

/* Inits a note on MIDI
 * @param
 *  char pitch - pitch of note offset by amount(?)
 *  char volume - volume of note
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
 *  char pitch - pitch of note offset by amount(?)
 */
void MIDIOff(char pitch)
{
    char payload[3] = {MIDI_OFF, pitch + KEY_OFFSET, 0};
    UARTSendArray(payload, 3);
//    __no_operation();
}
/* ---------------------- END KEY OUTPUT CODE ---------------------- */


/* ---------------------- KEY ISR HANDLERS ---------------------- */


void PORT6_IRQHandler(void)
{

    uint16_t status = P6IV >> 1;                             //find out which interrupt triggered the interrupt
                                                             //interrupt also gets cleared by reading PxIV

    pinHandler(6, status >> 1, )


}
