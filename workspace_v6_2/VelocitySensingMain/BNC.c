#include <MSP.H>

void BNCEncode(char data)
{
	P10OUT |= data & 0x3F;
}

void BNCDeassert(char data)
{
	P10OUT &= ~(0x3F);
}