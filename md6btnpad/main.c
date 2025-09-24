/* megadrive 6 button pad(atmega168/16MHz)
 * md6btnpad.c
 *
 * Created: 2019/11/27 2:43:28
 * Author : takuya matsubara
 */ 
// 7     9      6      4      3      2      1
// sel | bit5 | bit4 | bit3 | bit2 | bit1 | bit0
//-----+------+------+------+------+------+------
// H   | C    | B    | RIGHT| LEFT | DOWN | UP   
// L   | START| A    | L    | L    | DOWN | UP
// H   | C    | B    | RIGHT| LEFT | DOWN | UP
// L   | START| A    | L    | L    | L    | L
// H   | H    | H    | MODE | X    | Y    | Z
// L   | START| A    | H    | H    | H    | H

#include <avr/io.h>

#define F_CPU 16000000	// CPUクロック周波数[Hz]
#define TMR_PS   1024     //プリスケーラ値(分周比)
#define TMR_PSCR 5        //プリスケーラ設定値

#define SELMASK (1<<0)
#define SELPORT PORTD
#define SELPIN PIND
#define SELDDR DDRD
#define WAITHIGH while((SELPIN & SELMASK)==0)
#define WAITLOW while((SELPIN & SELMASK)!=0)

#define LEDMASK (1<<1)
#define LEDPORT PORTD
#define LEDDDR DDRD
#define LEDON LEDPORT|=LEDMASK
#define LEDOFF LEDPORT&=~LEDMASK

int main(void)
{
	unsigned char paddata1,paddata2,paddata3;

	PIND &= ~((1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));
	PORTD |= ((1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));
	PINB &= ~((1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5));
	PORTB |= ((1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5));
	
	TCCR0A = 0;	// Timer/counter control register A
	TCCR0B = TMR_PSCR;	// Timer/counter control register B
		
    /* Replace with your application code */
	DDRC |= 0b111111;	//output
	PORTC = 0b111111;
	SELDDR &= ~SELMASK;	//input
	SELPORT |= SELMASK;	//pull up

	LEDDDR |= LEDMASK;
	LEDOFF;

	WAITLOW;

	while(1){
		paddata1 = 0b111111;
		paddata2 = 0b110011;
		paddata3 = 0b111111;
		if((PIND & (1<<2))==0){
			paddata1 &= ~(1<<0); //up
			paddata2 &= ~(1<<0); //up
		}
		if((PIND & (1<<3))==0){
			paddata1 &= ~(1<<1); //down
			paddata2 &= ~(1<<1); //down
		}
		if((PIND & (1<<4))==0) paddata1 &= ~(1<<2); //left
		if((PIND & (1<<5))==0) paddata1 &= ~(1<<3); //right
		if((PIND & (1<<6))==0) paddata2 &= ~(1<<4); //a
		if((PIND & (1<<7))==0) paddata1 &= ~(1<<4); //b
		
		if((PINB & (1<<0))==0) paddata1 &= ~(1<<5); //c
		if((PINB & (1<<1))==0) paddata2 &= ~(1<<5); //start
		if((PINB & (1<<2))==0) paddata3 &= ~(1<<3); //mode
		if((PINB & (1<<3))==0) paddata3 &= ~(1<<2); //x
		if((PINB & (1<<4))==0) paddata3 &= ~(1<<1); //y
		if((PINB & (1<<5))==0) paddata3 &= ~(1<<0); //z
		
		WAITHIGH;
		while(1){
			//----CYCLE1
			// SEL=H   | C    | B    | RIGHT| LEFT | DOWN | UP
			PORTC = paddata1;
			TCNT0 = 0; //timer start
			WAITLOW;
			// SEL=L   | START| A    | L    | L    | DOWN | UP
			PORTC = paddata2;
			WAITHIGH;
			LEDON;
			if(TCNT0 < 18)break; //under 1.1msec
		}

		//----CYCLE2
		// SEL=H   | C    | B    | RIGHT| LEFT | DOWN | UP
		PORTC = paddata1;
		WAITLOW;
		// SEL=L   | START| A    | L    | L    | L    | L
		PORTC = paddata2 & 0b110000;

		WAITHIGH;
		//----CYCLE3
		// SEL=H   | H    | H    | MODE | X    | Y    | Z
		PORTC = paddata3 | 0b110000;
		WAITLOW;
		//SEL=L   | START| A    | H    | H    | H    | H
		PORTC = paddata2 | 0b001111;

		WAITHIGH;
		//----CYCLE4
		// SEL=H   | C    | B    | RIGHT| LEFT | DOWN | UP
		PORTC = paddata1;
		TCNT0 = 0; //timer start
		LEDOFF;
		WAITLOW;
		// SEL=L   | START| A    | L    | L    | DOWN | UP
		PORTC = paddata2;
	}
}

