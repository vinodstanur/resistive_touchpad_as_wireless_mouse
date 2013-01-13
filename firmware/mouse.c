#include <avr/io.h>
#define F_CPU 9600000
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char data;
volatile char trigger;

void adc_init() {
		DIDR0 |= 1<<ADC2D;
		ADMUX |= 1<<MUX1;
		ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
		ADCSRA |= (1<<ADEN);
}

int adc_read(char a)
{
	ADMUX = a;
	int dig;
	ADCSRA |= 1<<ADSC;
	while(ADCSRA & (1<<ADSC));
	dig = ADCW;
	return dig;
}


void uart_send_byte(unsigned char data)
{
	unsigned char i;
	TIMSK0 &= ~(1<<OCIE0A);
	TIFR0 |= 1<<OCF0A;
	TCNT0 = 0;
	TIMSK0 |= 1<OCIE0A;
	PORTB &= ~(1<<PB3);
	while(!(TIFR0 & (1<<OCF0A)));
	TIFR0 |= 1<<OCF0A;
	for(i=0;i<8;i++) {
			if(data & 1)
					PORTB |= 1<<PB3;
			else 
					PORTB &= ~(1<<PB3);
			data >>= 1;
			while(!(TIFR0 & (1<<OCF0A)));
			TIFR0 |= 1<<OCF0A;
	}
	PORTB |= 1<<PB3;
	while(!(TIFR0 & (1<<OCF0A)));
	TIFR0 |= 1<<OCF0A;
}
	
print(char *p) {
		while(*p){
				uart_send_byte(*p++);
		}
}


void uart_print_num(int i)
{
	if (i < 0) {
		uart_send_byte('-');
		i *= -1;
	} else if (i == 0) {
		uart_send_byte('+');
		uart_send_byte('0');
		return;
	} else
		uart_send_byte('+');
	char count = 0;
	unsigned char b[5] = { 0 };
	while (i) {
		b[count++] = i % 10;
		i /= 10;
	}
	for (count = 4; count >= 0; count--)
		uart_send_byte(b[count] + '0');
}


main()
{
	int x,y;

	DDRB|= 1<<PB3;		
	PORTB |= 1<<PB3;
	TCCR0A |= 1<<WGM01; // compare mode
	TCCR0B |= (1<<CS00); //no prescaler
	adc_init();
	OCR0A = 83;  //for baudrate 57600
	char a=0;
	while(1) {
			DDRB |= (1<<PB2)|(1<<PB0);
			DDRB &= ~((1<<PB4)|(1<<PB1));//ADC 2
			PORTB &= ~((1<<PB4)|(1<<PB1));
			PORTB |= 1<<PB2;
			PORTB &= ~(1<<PB0);
	//		_delay_ms(10);
			x=adc_read(2);
			
			DDRB |= (1<<PB4)|(1<<PB1);
			DDRB &= ~((1<<PB2)|(1<<PB0));//ADC 1
			PORTB &= ~((1<<PB2)|(1<<PB0));
			PORTB |= 1<<PB1;
			PORTB &= ~(1<<PB4);
	//		_delay_ms(10);
			y=adc_read(1);
			if(x<900 && y<900) {
				print("X: ");
				uart_print_num(x);
				uart_send_byte(' ');
				print("Y: ");
				uart_print_num(y);
				uart_send_byte('\n');
			}
	}
}
