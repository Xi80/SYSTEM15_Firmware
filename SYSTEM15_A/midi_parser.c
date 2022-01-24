/*
 * midi_parser.c
 *
 * Created: 2022/01/22 0:20:19
 *  Author: Owner
 */ 
#include "midi_parser.h"

/*プライベート変数*/
uint8_t work_midi[3];			/*ワークメモリ*/
uint8_t ex_ptr;					/*エクスクルーシブ用ポインタ*/
uint8_t ex_buffer[64];			/*エクスクルーシブ用バッファ*/
uint8_t flag;					/*フラグ(bit0:3バイト目フラグ/bit1:SysExフラグ)*/
uint8_t running_status_buffer;	/*ランニングステータスバッファ*/
uint8_t rpn[32];				/*RPN*/

void init_midi_parser(void){
	/*各種メモリの初期化*/
	memset(&work_midi,0x00,3);
	memset(&rpn,0x7F,32);
	ex_ptr					= 0x00;
	flag					= 0x00;
	running_status_buffer	= 0x00;
	
	init_midi_mgr();
}

void parse_midi_data(uint8_t data){
		if(flag & 0x02){
			//SYSEX
			if(data == MSG_EOX){				/*エクスクルーシブ終了*/
				decode_exclusive();				/*SysEx解読ルーチン呼び出し*/
				flag &= ~0x02;
				} else {
				ex_buffer[ex_ptr++] = data;
			}
			} else {
			if(data & 0x80){
				if(data == MSG_SOX){			/*エクスクルーシブ開始*/
					flag |= 0x02;
					memset(ex_buffer, 0x00, 64);
					ex_ptr = 0x00;
					return;
				}
				if(data >= 0xF8)return;
				running_status_buffer = data;
				flag &= ~0x01;
				return;
				} else {
				if(flag & 0x01){
					work_midi[0] = running_status_buffer;
					work_midi[2] = data;
					flag &= ~0x01;
					decode_midi_message();	/*メッセージ解読ルーチン*/
					return;
				}
				
				if(!running_status_buffer)return;
				
				if(running_status_buffer < 0xC0){
					flag |= 0x01;
					work_midi[1] = data;
					return;
				}
				
				if(running_status_buffer < 0xE0){
					work_midi[0] = running_status_buffer;
					work_midi[1] = data;
					decode_midi_message();	/*メッセージ解読ルーチン*/
					return;
				}
				
				if(running_status_buffer < 0xF0){
					work_midi[1] = data;
					flag |= 0x01;
					return;
				}
				
				if (running_status_buffer == 0xF2) {
					running_status_buffer = 0x00;
					flag |= 0x01;
					return;
				}

				if (running_status_buffer == 0xF1 || running_status_buffer == 0xF3) {
					running_status_buffer = 0x00;
					return;
					} else {
					running_status_buffer = 0x00;
					return;
				}
			}
		}
}

void decode_midi_cc(void){
	switch(work_midi[1]){
		case CC_MOD:
			mgr_modulation(work_midi[0] & 0x0F,work_midi[2]);
			break;
		case CC_DE_MSB:
			if((rpn[(work_midi[0] & 0x0F) << 1] | (rpn[((work_midi[0] & 0x0F) << 1) + 1] << 8) )== 0x0000)
				mgr_pitchbend_sensitivity(work_midi[0] & 0x0F,work_midi[2]);
			rpn[((work_midi[0] & 0x0F) << 1)] = 0x7F;
			rpn[((work_midi[0] & 0x0F) << 1) + 1] = 0x7F;
			break;
		case CC_VOL:
			mgr_volume(work_midi[0] & 0x0F,work_midi[2]);
			break;
		case CC_EXP:
			mgr_expression(work_midi[0] & 0x0F,work_midi[2]);
			break;
		case CC_HOLD:
			mgr_hold(work_midi[0] & 0x0F,work_midi[2]);
			break;
		case CC_RPN_LSB:
			rpn[((work_midi[0] & 0x0F) << 1)] = work_midi[2];
			break;
		case CC_RPN_MSB:
			rpn[((work_midi[0] & 0x0F) << 1) + 1] = work_midi[2];
			break;
		case CC_ASO:
			mgr_all_sounds_off(work_midi[0] & 0x0F);
			break;
		case CC_RAC:
			mgr_reset_all_controller(work_midi[0] & 0x0F);
			break;
		case CC_ANO:
			mgr_all_notes_off(work_midi[0] & 0x0F);
			break;		
	}
}

void decode_midi_message(void){
	
	work_midi[0] = ((work_midi[0] & 0xF0) == MSG_NOTE_ON && work_midi[2] == 0x00)? 0x80 | (work_midi[0] & 0x0F) : work_midi[0];
	
	switch(work_midi[0] & 0xF0){
		case MSG_NOTE_ON:
			mgr_note_on(work_midi[0] & 0x0F,work_midi[1],work_midi[2]);
			break;
		case MSG_NOTE_OFF:
			mgr_note_off(work_midi[0] & 0x0F,work_midi[1]);
			break;
		case MSG_CC:
			decode_midi_cc();
			break;
		case MSG_PC:
			mgr_program_change(work_midi[0] & 0x0F,work_midi[1]);
			break;
		case MSG_PB:
			mgr_pitchbend(work_midi[0] & 0x0F,work_midi[1] | (work_midi[2] << 7));
			break;
	}
}

void decode_exclusive(void){
	const uint8_t gm_reset[] = { 0x7E, 0x7F, 0x09, 0x01 };
	const uint8_t xg_reset[] = { 0x43, 0x10, 0x4C, 0x00, 0x00, 0x7E, 0x00 };
	const uint8_t gs_reset[] = { 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x7F, 0x00, 0x41 };
	const uint8_t gs_mode1[] = { 0x41, 0x10, 0x42, 0x12, 0x00, 0x00, 0x7F, 0x00, 0x01 };
	if (!memcmp(ex_buffer, gm_reset, sizeof(gm_reset))) {
		mgr_reset(0);
	}

	if (!memcmp(ex_buffer, gs_reset, sizeof(gs_reset))) {
		mgr_reset(1);;
	}

	if (!memcmp(ex_buffer, gs_mode1, sizeof(gs_mode1))) {
		mgr_reset(2);
	}

	if (!memcmp(ex_buffer, xg_reset, sizeof(xg_reset))) {
		mgr_reset(3);
	}
	
	mgr_sysex(&ex_buffer[0],ex_ptr);
	return;	
}