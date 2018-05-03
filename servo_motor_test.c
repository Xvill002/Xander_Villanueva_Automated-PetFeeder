#include <avr/io.h>

#include <util/delay.h>

//Simple Wait Function
void Wait()
{
	uint8_t i;
	for(i=0;i<10;i++)
	{
		
		_delay_loop_2(0);
	}

}

void main()
{
	DDRB = 0x00; PORTB = 0xFF;
	char button = 0x00;
	char pushed = 0x00;
	TCCR1A |= (1 << COM1A1) | (1<<COM1B1) | (1<<WGM11);          
	//TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); Used for testing
	TCCR1B = 0x1B;
	ICR1 = 4999;  

	//DDRD|=(1<<PD4)|(1<<PD5);   Used for testing
	DDRD = 0xFF;

	while(1)
	{
		button = ~PINB;
		if(button == 0x01)
		{
			if(pushed == 0x00)
			{
				OCR1A=290; //close to 180 degrees
				Wait();
				pushed = 0x01;
			}
		
			else if(pushed == 0x01)
			{
				OCR1A=75; //close to 0 degrees
				Wait();
				pushed = 0x00;
			}
		}
	}
}
