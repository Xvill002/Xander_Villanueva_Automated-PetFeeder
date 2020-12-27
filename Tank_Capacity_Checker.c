
#include <avr/interrupt.h>

#include<avr/io.h>
#include<util/delay.h>




void send_ultrasonic_signal(int a, int b) // function used for sending a signal with ultrasonic sensor and recording its reading
{
	TCNT1 = 0; // initialize timer
	PORTC |= 1 << PINC0; 
	
	while(TCNT1 < 100);
	
	PORTC &= ~(1 << PINC0);

	TCNT1 = 0;
	while(!(PINC & (1 << PINC1)) && TCNT1 < 30000) ;  // check to see if echo is high.
	b = TCNT1;
	if(b < 30000)
	{
		TCNT1=0;
		while(PINC & (1 << PINC1));    // waiting for echo to go low
		a = TCNT1;
		if((a/58) > 100)
		{
			PORTD = 0x01; // set green LED on for indication of acceptable tank level
		}
		
		else
		{
			PORTD = 0x02; // set red LED for inidication of low tank level
		}
	}
	_delay_ms(50);
}




int main(void){
	
	TCCR1B |= 1<<CS10; // timer preface
	DDRA = 0x00; PORTA = 0xFF; //Ultrasonic sensor port
	DDRD = 0xFF; PORTD = 0x00; //LEDs port
	
	while(1)
	{
		
			send_ultrasonic_signal(a,b); // use ultrasonic sensor to detect tank level
	}
}


