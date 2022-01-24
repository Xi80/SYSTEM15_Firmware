/*
 * SYSTEM15_A.c
 *
 * Created: 2022/01/21 22:30:32
 * Author : Owner
 */ 



/*���W���[���ǂݍ���*/
#include "serial_manager.h"			//�V���A���ʐM
#include "midi_parser.h"

#include <avr/io.h>
#include <util/delay.h>
/*���荞�ݏ���*/
ISR(USART0_RXC_vect)
{
	uint8_t recv = USART0.RXDATAL;
	RingBuffer_Insert(&midi_buffer,recv);
}

void init_io(void){
	xprintf("init io\n");
	PORTD.DIR = 0xFF;		//�f�[�^�o�X�����ׂďo�͂ɐݒ�
	PORTC.DIR = 0x3F;		//PC0~PC5���o�͂ɐݒ�
	PORTE.DIR = 0x07;		//PE0~PE2���o�́APE3����͂ɐݒ�
		
	PORTE.OUT |= 0x07;		//WR,RD,IC���l�Q�[�g
	PORTC.OUT |= 0x3F;		//CS�����ׂăl�Q�[�g
	PORTC.OUT &= ~(0x01);	//A0��0��
	PORTD.OUT = 0x00;		//�f�[�^�o�X��0x00��
}



int main(void)
{
	init_serial();
	init_io();
	init_midi_parser();
    /* Replace with your application code */
	
	while (1) 
    {
		
		if(RingBuffer_GetCount(&midi_buffer)){
			uint8_t data = RingBuffer_Remove(&midi_buffer);
			parse_midi_data(data);
		}
    }
}

