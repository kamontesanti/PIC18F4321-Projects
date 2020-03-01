#include <p18f4321.h>

// define the configurations:
#pragma config OSC = INTIO2
#pragma config BOR = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF


// definition of the main method:
void main()
{
	TRISCbits.TRISC7 = 0;		// configure PORTC bit 7 as output
	TRISDbits.TRISD7 = 1;		// configure PORTD bit 7 as input
	TRISDbits.TRISD6 = 1;		// configure PORTD bit 6 as input

	PORTD = 0x00;				// ensure that the LED is off prior to loop

	while(1)					// wait in infinite loop for interrupt
	{	// verify that the values are equivalent
		if ((PORTDbits.RD6) == (PORTDbits.RD7))
			PORTCbits.RC7 = 1;	// turn the LED on
		else
			PORTCbits.RC7 = 0;	// turn the LED off
	}						
	
}


