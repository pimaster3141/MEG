
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>


/* ---------------------- HARDWARE DEFINES ---------------------- */
#define NUM_PORTS 9
#define KEY_OFFSET 47
#define KN 255 //null key
#define MIDI_ON 0x90
#define MIDI_OFF 0x80
/* ---------------------- END HARDWARE DEFINES ---------------------- */



/* ---------------------- KEYMAPPING AND KEYSTATES ---------------------- */
// data structure to hold key timing values
char keyStates[NUM_PORTS*8] = {};

// data structure to hold key mappings
const char MIDINote[NUM_PORTS*8] =
{
    KN, KN, KN, KN, KN, KN, 10, KN,     //Port1
    KN, KN, KN, KN, 4,  3,  2,  1,      //Port2
    5,  KN, KN, KN, KN, KN, KN, KN,     //Port3
    KN, KN, KN, KN, KN, KN, KN, KN,     //Port4
    KN, KN, KN, 16, KN, KN, 6,  7,      //Port5
    KN, KN, 15, 12, KN, KN, 8,  9,      //Port6
    KN, 11, KN, 13, KN, KN, KN, KN,     //Port7
    KN, KN, 19, 18, 21, 25, 24, 22,     //Port8
    23, 20, 17, 14, KN, KN, KN, KN      //Port9
};
/* ---------------------- END KEYMAPPING ---------------------- */




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

// Sets up comparator interrupts and associated ISR in NVIC for all ports/keys
void ISRSet(void)
{
    return;
}
/* ---------------------- END SETUP CODE ---------------------- */




/* ---------------------- KEY OUTPUT CODE ---------------------- */
/* Unified key action function call TODO: copied from non velocity - may need editing
 * @param
 *  char keyIndex - index of key [1-25]
 *  char keyState - TODO: something needs to be changed here...
 */
void pinHandler(char keyIndex, char keyState)
{
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
