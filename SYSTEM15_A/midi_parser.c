/*
 * midi_parser.c
 *
 * Created: 2022/01/22 0:20:19
 *  Author: Owner
 */ 
#include "midi_parser.h"

/*�\����*/
midi_msg_t msg;

/*�v���C�x�[�g�ϐ�*/
uint8_t work_midi[3];			/*���[�N������*/
uint8_t ex_ptr;					/*�G�N�X�N���[�V�u�p�|�C���^*/
uint8_t flag;					/*�t���O(bit0:3�o�C�g�ڃt���O/bit1:SysEx�t���O)*/
uint8_t running_status_buffer;	/*�����j���O�X�e�[�^�X�o�b�t�@*/
uint8_t rpn[2];					/*RPN*/

void init_midi_parser(void);{
	/*�e�탁�����̏�����*/
	memset(&work_midi,0x00,3);
	memset(&rpn,0x00,2);
	ex_ptr					= 0x00;
	flag					= 0x00;
	running_status_buffer	= 0x00;
}

void parse_midi_data(void);{
	
}

void decode_midi_message(void);{
	
}

void decode_exclusive(void);{
	
}