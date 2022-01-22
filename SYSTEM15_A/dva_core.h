/*
 * dva_core.h
 *
 * Created: 2022/01/22 8:20:02
 *  Author: Owner
 */ 


#ifndef DVA_CORE_H_
#define DVA_CORE_H_

#include <avr/io.h>
#include "midi_manager.h"

void init_dva(void);

void dva_note_on(uint8_t,uint8_t,uint8_t);

void dva_note_off(uint8_t,uint8_t);

void dva_pitchbend(uint8_t);

void dva_cc(uint8_t);

/*構造体*/
typedef struct {
	uint8_t assigned_ch;			/*割り当てられているチャネル*/
	uint8_t priority;				/*優先度*/
	uint8_t note_state;				/*発音中か*/
	uint8_t vel;					/*ベロシティ*/
	uint8_t sustain;				/*サステイン状態か*/
	uint8_t note_number;			/*ノートナンバー*/
	uint16_t assign_map;			/*割り当て可能なチャネル*/
}voice_param_t;


#endif /* DVA_CORE_H_ */