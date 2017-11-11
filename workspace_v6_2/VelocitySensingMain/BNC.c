
#include "msp432p401r.h"
#include "BNC.H"

void BNCEncode(char data)
{
	P10OUT |= data & 0x3F;
}

void BNCDeassert(void)
{
	P10OUT &= ~(0x3F);
}
