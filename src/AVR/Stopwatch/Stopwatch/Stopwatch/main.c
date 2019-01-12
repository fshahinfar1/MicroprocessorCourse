/*
 * Stopwatch.c
 *
 * Created: 12/31/2018 7:55:07 PM
 * Author : Farbod Shahinfar 
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

// PORTA [5, 7] (3 bits)
// PORTA[0, 3] (4 bit)
// PORTA 4 (1 bit)

int ds = 0;
int sec = 0;
int min = 0;

int state = 0; // stop

/*
*	id= [1, 6]
*	val= [0, 9]
*/
void set_seven_segment(int ss_id, int val) {
	PORTA = ss_id << 5;
	PORTA |= (val % 10);
	PORTA |= 1 << 4;
	for (int i=0; i<10; i++);
	PORTA = 0;
}

// handle timer overflow interrupt for timer0
ISR(TIMER0_OVF_vect) 
{
	if (state != 1) {
		return;
	}
	
	ds++;
	ds %= 100;
	set_seven_segment(6, ds%10);
	set_seven_segment(5, (ds/10 % 10));
	if (ds == 0) {
		sec++;
		sec %= 60;
		set_seven_segment(4, sec%10);
		set_seven_segment(3, (sec/10 % 10));
		if (sec==0) {
			min++;
			set_seven_segment(2, min%10);
			set_seven_segment(1, (min/10 % 10));
		}
	}
	TCNT0 = -0x78;
}

void reset_timer() {
	state = 0;
	TCCR0 = 0x00;
	ds = sec = min = 0;
	set_seven_segment(6, 0);
	set_seven_segment(5, 0);
	set_seven_segment(4, 0);
	set_seven_segment(3, 0);
	set_seven_segment(2, 0);
	set_seven_segment(1, 0);
	TCNT0 = -0x78;
}

void start_timer() {
	state = 1;
	TCCR0 = 0X05;
}

void pause_timer() {
	state = 2;
	TCCR0 = 0x00;
}

void setup() {
	sei();
	// setup PORTA
	DDRA = 0xFF;
	// setup PORTD
	DDRD = 0x00;  // all input
	PORTD = 0xE0;  // active pull up
	// setup timer 1
	// (1 / 8000000) * 1024 * 78 = 0.009984 almost 0.01 seconds
	TCNT0 = -0X78; // count 78 clock pulse, it was calculated for achieving 0.01 delay with 1024 prescale
	TIMSK |= 1 << TOV0;  // enable time overflow interrupt for timer zero
}

int main(void)
{
	setup();
	
    while (1) 
    {
		
		if ((PIND & (1 << 5)) == 0 && state != 0) {
			// reset timer
			reset_timer();
		} else if ((PIND & (1 << 6)) == 0 && state != 1) {
			// start timer
			start_timer();
		} else if ((PIND & (1 << 7)) == 0 && state == 1) {
			// pause
			pause_timer();
		}
    }
}

