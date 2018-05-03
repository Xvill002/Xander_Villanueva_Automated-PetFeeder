/*
 * FSR_Test.c
 *
 * Created: 4/28/2018 3:03:46 PM
 *  Author: xcvmi
 */ 

#include <avr/io.h>
#include <util/delay.h>




void A2D_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//       analog to digital conversions.
}


int main(void)
{
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	unsigned short x = 0;
	
	A2D_init();
	
	_delay_ms(50);
	
    while(1)
    {
		x = ADC; //force value read from sensor
		
		if(x > 600)
		{
			PORTD = 0x40; //turn on LED
			
		}
		
		else
		{
			PORTD = 0x00; // turn off LED
		}
		
		
		
		
    }
}
