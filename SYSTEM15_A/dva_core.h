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
#include "tone_manager.h"
#include "opn_driver.h"
#include "psg_driver.h"
#include "timbre_manager.h"

#define F_CPU 3333333UL
#include <util/delay.h>

void init_dva(void);

void dva_note_on(uint8_t,uint8_t,uint8_t);

void dva_note_off(uint8_t,uint8_t);

void dva_pitchbend(uint8_t);

void dva_vol(uint8_t);

void dva_exp(uint8_t);

void dva_hold(uint8_t);

void dva_program_change(uint8_t);

void dva_reset(uint8_t);

void dva_reset_ic(void);

void dva_all_sounds_off(uint8_t);

void dva_all_notes_off(uint8_t);

void dva_reset_all_controller(uint8_t);

void dva_change_assign(uint8_t*);

void dva_all_sounds_off(uint8_t ch);
	
void dva_all_notes_off(uint8_t ch);
	
void dva_reset_all_controller(uint8_t ch);
	
void dva_change_assign(uint8_t *data);
	
void dva_change_mixer(uint8_t* data);

void dva_change_envelope(uint8_t *data);
	
void dva_change_envelope_freq(uint8_t *data);
	
void dva_transfer_opn_voice(uint8_t *data);
	
void dva_save_opn_voice(uint8_t *data);

void dva_load_opn_voice(uint8_t *data);
	
void dva_change_opn_voice(uint8_t ch,uint8_t pos,uint8_t val);
	

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

typedef struct {
	uint8_t env_shape;				/*エンベロープの形状、不使用なら0xFF*/
	uint16_t env_freq;				/*エンベロープ周波数*/
	uint8_t noise_freq;				/*ノイズ周波数*/
	uint8_t output;					/*使用しているチャネル*/
}psg_param_t;


#define _BYTE1(x) (  x        & 0xFF )
#define _BYTE2(x) ( (x >>  8) & 0xFF )

#define BYTE_SWAP_16(x) ((uint16_t)( _BYTE1(x)<<8 | _BYTE2(x) ))

#endif /* DVA_CORE_H_ */