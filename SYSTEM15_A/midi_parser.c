/*
 * midi_parser.c
 *
 * Created: 2022/01/22 0:20:19
 *  Author: Owner
 */ 
#include "midi_parser.h"

/*構造体*/
midi_msg_t msg;

/*プライベート変数*/
uint8_t work_midi[3];			/*ワークメモリ*/
uint8_t ex_ptr;					/*エクスクルーシブ用ポインタ*/
uint8_t flag;					/*フラグ(bit0:3バイト目フラグ/bit1:SysExフラグ)*/
uint8_t running_status_buffer;	/*ランニングステータスバッファ*/
uint8_t rpn[2];					/*RPN*/

void init_midi_parser(void);{
	/*各種メモリの初期化*/
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