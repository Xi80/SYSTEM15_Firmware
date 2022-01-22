/*
 * midi_manager.h
 *
 * Created: 2022/01/22 0:39:33
 *  Author: Owner
 */ 


#ifndef MIDI_MANAGER_H_
#define MIDI_MANAGER_H_

#include <avr/io.h>
#include "midi_parser.h"
#include "dva_core.h"
#include "xprintf.h"

void init_midi_mgr(void);					/*MIDIマネージャの初期化*/

void mgr_note_on(uint8_t,uint8_t,uint8_t);	/*チャネル,ノート番号,ベロシティ*/
void mgr_note_off(uint8_t,uint8_t);			/*チャネル,ノート番号*/

void mgr_program_change(uint8_t,uint8_t);	/*チャネル,バリュー*/
void mgr_pitchbend(uint8_t,uint16_t);		/*チャネル,バリュー*/
void mgr_pitchbend_sensitivity(uint8_t,uint8_t); /*チャネル,バリュー*/
void mgr_modulation(uint8_t,uint8_t);		/*チャネル,バリュー*/
void mgr_volume(uint8_t,uint8_t);			/*チャネル,バリュー*/
void mgr_expression(uint8_t,uint8_t);		/*チャネル,バリュー*/
void mgr_hold(uint8_t,uint8_t);				/*チャネル,バリュー*/

void mgr_all_sounds_off(uint8_t);			/*チャネル*/
void mgr_reset_all_controller(uint8_t);		/*チャネル*/
void mgr_all_notes_off(uint8_t);			/*チャネル*/

void mgr_reset(void);						/*(音源リセット)*/

void mgr_sysex(uint8_t *,uint8_t);			/*バイト列へのポインタ,サイズ*/


/*構造体*/

typedef struct{
	uint8_t voice_assign;					/*割り当てる音数*/
	uint8_t voice_count;					/*現在の音数*/
	uint8_t pgm;							/*プログラム*/
	uint8_t mod;							/*モジュレーション*/
	uint8_t vol;							/*ボリューム*/
	uint8_t exp;							/*エクスプレッション*/
	uint8_t hold;							/*ホールド*/
	uint16_t pb;							/*ピッチベンド*/
	uint8_t pbs;							/*ピッチベンドセンシティビティ*/
}midi_channel_t;

extern midi_channel_t midi_channel[16];

#endif /* MIDI_MANAGER_H_ */