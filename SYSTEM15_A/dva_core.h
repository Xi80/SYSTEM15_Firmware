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

/*�\����*/
typedef struct {
	uint8_t assigned_ch;			/*���蓖�Ă��Ă���`���l��*/
	uint8_t priority;				/*�D��x*/
	uint8_t note_state;				/*��������*/
	uint8_t vel;					/*�x���V�e�B*/
	uint8_t sustain;				/*�T�X�e�C����Ԃ�*/
	uint8_t note_number;			/*�m�[�g�i���o�[*/
	uint16_t assign_map;			/*���蓖�ĉ\�ȃ`���l��*/
}voice_param_t;


#endif /* DVA_CORE_H_ */