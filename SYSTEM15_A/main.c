/*
 * SYSTEM15_A.c
 *
 * Created: 2022/01/21 22:30:32
 * Author : Owner
 */ 

#include <avr/io.h>

/*モジュール読み込み*/
#include "serial_manager.h"			//シリアル通信
#include "midi_parser.h"

/*割り込み処理*/
ISR(USART0_RXC_vect)
{
	uint8_t recv = USART0.RXDATAL;
	RingBuffer_Insert(&midi_buffer,recv);
}



int main(void)
{
	init_serial();
	init_midi_parser();
	PORTA.DIR |= 0xF0;
	
	PORTA.OUT |= 0xF0;
	
    /* Replace with your application code */
	xprintf("Hello,World!\n");
    while (1) 
    {
		
		if(RingBuffer_GetCount(&midi_buffer)){
			uint8_t data = RingBuffer_Remove(&midi_buffer);
			parse_midi_data(data);
		}
    }
}

