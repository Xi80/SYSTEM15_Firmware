/*
 * midi_parser.h
 *
 * Created: 2022/01/22 0:20:10
 *  Author: Owner
 */ 


#ifndef MIDI_PARSER_H_
#define MIDI_PARSER_H_

#include <avr/io.h>
#include <string.h>
#include "midi_manager.h"

void init_midi_parser(void);		/*MIDIパーサの初期化*/

void parse_midi_data(uint8_t);		/*MIDIデータのパース*/

void decode_midi_message(void);		/*MIDIメッセージの解読*/

void decode_exclusive(void);		/*SysExの解読*/

void decode_midi_cc(void);			/*CCの解読*/
/*マクロ*/
#define MSG_SOX			0xF0
#define MSG_EOX			0xF7

#define MSG_NOTE_ON		0x90
#define MSG_NOTE_OFF	0x80
#define MSG_CC			0xB0
#define MSG_PC			0xC0
#define MSG_PB			0xE0

#define CC_MOD			0x01
#define CC_DE_MSB		0x06
#define CC_VOL			0x07
#define CC_EXP			0x0B
#define CC_HOLD			0x40
#define CC_RPN_LSB		0x64
#define CC_RPN_MSB		0x65

#define CC_ASO			0x78
#define CC_RAC			0x79
#define CC_ANO			0x7B

#define EX_GS			0x01
#define EX_88			0x02
#define EX_XG			0x03
#define EX_GM			0x04

/*グローバル変数*/


#endif /* MIDI_PARSER_H_ */