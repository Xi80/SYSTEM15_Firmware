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

void init_midi_mgr(void);					/*MIDI�}�l�[�W���̏�����*/

void mgr_note_on(uint8_t,uint8_t,uint8_t);	/*�`���l��,�m�[�g�ԍ�,�x���V�e�B*/
void mgr_note_off(uint8_t,uint8_t);			/*�`���l��,�m�[�g�ԍ�*/

void mgr_program_change(uint8_t,uint8_t);	/*�`���l��,�o�����[*/
void mgr_pitchbend(uint8_t,uint16_t);		/*�`���l��,�o�����[*/
void mgr_pitchbend_sensitivity(uint8_t,uint8_t); /*�`���l��,�o�����[*/
void mgr_modulation(uint8_t,uint8_t);		/*�`���l��,�o�����[*/
void mgr_volume(uint8_t,uint8_t);			/*�`���l��,�o�����[*/
void mgr_expression(uint8_t,uint8_t);		/*�`���l��,�o�����[*/
void mgr_hold(uint8_t,uint8_t);				/*�`���l��,�o�����[*/

void mgr_all_sounds_off(uint8_t);			/*�`���l��*/
void mgr_reset_all_controller(uint8_t);		/*�`���l��*/
void mgr_all_notes_off(uint8_t);			/*�`���l��*/

void mgr_reset(void);						/*(�������Z�b�g)*/

void mgr_sysex(uint8_t *,uint8_t);			/*�o�C�g��ւ̃|�C���^,�T�C�Y*/


/*�\����*/

typedef struct{
	uint8_t voice_assign;					/*���蓖�Ă鉹��*/
	uint8_t voice_count;					/*���݂̉���*/
	uint8_t pgm;							/*�v���O����*/
	uint8_t mod;							/*���W�����[�V����*/
	uint8_t vol;							/*�{�����[��*/
	uint8_t exp;							/*�G�N�X�v���b�V����*/
	uint8_t hold;							/*�z�[���h*/
	uint16_t pb;							/*�s�b�`�x���h*/
	uint8_t pbs;							/*�s�b�`�x���h�Z���V�e�B�r�e�B*/
}midi_channel_t;

extern midi_channel_t midi_channel[16];

#endif /* MIDI_MANAGER_H_ */