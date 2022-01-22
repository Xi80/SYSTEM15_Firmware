/*
 * dva_core.c
 *
 * Created: 2022/01/22 8:20:14
 *  Author: Owner
 */ 
#include "dva_core.h"

//[15]�͖��g�p
voice_param_t voices[16];

void init_dva(void){
	for(uint8_t i = 0;i < 16;i++){
		voices[i].assign_map = 0xFFFF;	/*�S�`���l���ɑ΂��Ċ��蓖�ĉ\*/
		voices[i].assigned_ch = 0xFF;	/*�ǂ��ɂ����蓖�ĂȂ�*/
		voices[i].note_number = 0xFF;
		voices[i].vel		 = 0x00;
		voices[i].note_state  = 0x00;	/*�m�[�g�I�t*/
		voices[i].priority    = 0x00;
		voices[i].sustain     = 0x00;
	}
	
	for(uint8_t i = 12;i < 16;i++){
		voices[i].assign_map = 0x0000;
	}

	xprintf("init dva\n");
	for(uint8_t i = 0;i < 4;i++){
		//psg_reset(i);
		psg_set_mixer(i,0b00111000);
	}
	
	psg_set_mfreq(0,0,TP_F(2000));
	psg_set_volume(0,0,15);
	_delay_ms(100);
	psg_set_mfreq(0,0,TP_F(1000));
	_delay_ms(50);
	psg_set_volume(0,0,0);	
}

void dva_note_on(uint8_t ch,uint8_t note,uint8_t vel){
	uint8_t min_prio = 16;
	uint8_t min_vo   = 16;
	
	
	midi_channel[ch].voice_count = 0;
	for(uint8_t i = 0;i < 16;i++){
		
		if((voices[i].assign_map & (1 << ch)) && !voices[i].sustain && !voices[i].note_state){
			
			/*���̃`���l���ɑ΂��Ċ��蓖�ĉ\*/
			if(voices[i].priority <= min_prio){
				/*���D�揇�ʂ�����*/
				
				min_prio = voices[i].priority;
				min_vo   = i;
				
			}
			
		}
		
		if(voices[i].assigned_ch == ch){
			midi_channel[ch].voice_count++;
		}
	}
	
	if(min_vo == 16)return;	//�󂫂��Ȃ�
	
	/*�m�[�g�I������*/
	
	voices[min_vo].assigned_ch = ch;
	voices[min_vo].note_number = note;
	voices[min_vo].vel = vel;
	voices[min_vo].note_state = 1;
	voices[min_vo].sustain = 0;
	voices[min_vo].priority = midi_channel[ch].voice_count;
	
	if(min_vo > 11){
		/*FM*/
	} else {
		/*PSG*/
		uint8_t psg_id = min_vo / 3;
		uint8_t psg_ch = min_vo % 3;
		uint16_t tone = calc_psg_tone(note,midi_channel[ch].pbs,midi_channel[ch].pb);
		//xprintf("%02x %02x %d \n",psg_id,psg_ch,tone);
		psg_set_mfreq(psg_id,psg_ch,tone);
		psg_set_volume(psg_id,psg_ch,vel_curve[vel]);
	}
	
	for(uint8_t i = 0; i < 16;i++){
		if(voices[i].assigned_ch == ch){
			voices[i].priority -= (voices[i].priority)? 1 : 0;
		}
	}
}


void dva_note_off(uint8_t ch,uint8_t note){
	uint8_t off_count = 0;
	uint8_t off_prio  = 0;
	uint8_t off_vo    = 16;
	
	for(uint8_t i = 0;i < 16;i++){
		if((voices[i].assign_map & (1 << ch))){
			if(!voices[i].note_state)off_count++;
		}
		
		if(voices[i].assigned_ch == ch && voices[i].note_number == note && voices[i].note_state){
			off_prio = voices[i].priority;
			off_vo = i;
		}
		
		if(voices[i].sustain == 0x00 && voices[i].note_state == 0x00){
			if(off_vo > 11){
				/*FM*/
				} else {
				/*PSG*/
				uint8_t psg_id = off_vo / 3;
				uint8_t psg_ch = off_vo % 3;
				psg_set_volume(psg_id,psg_ch,0);
				psg_set_mfreq(psg_id,psg_ch,0);
			}
		}
	}
	 
	if(off_vo == 16)return;
	/*�m�[�g�I�t����*/
	voices[off_vo].note_state = 0;
	voices[off_vo].priority = off_count;
	for(uint8_t i = 0;i < 16;i++){
		if((voices[i].assigned_ch == ch ) && voices[i].note_state){
			if(voices[i].note_state){
				voices[i].priority += (voices[i].priority < off_prio)? 1 : 0;
			}
		}
	}
	
	if(off_vo > 11){
		/*FM*/
	} else {
		/*PSG*/
		uint8_t psg_id = off_vo / 3;
		uint8_t psg_ch = off_vo % 3;
		//uint16_t tone = calc_psg_tone(note,midi_channel[ch].pbs,midi_channel[ch].pb);
		psg_set_volume(psg_id,psg_ch,0);
		psg_set_mfreq(psg_id,psg_ch,0);
	}	
}

void dva_pitchbend(uint8_t ch){
	
}

void dva_cc(uint8_t ch){
	
}