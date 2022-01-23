/*
 * dva_core.c
 *
 * Created: 2022/01/22 8:20:14
 *  Author: Owner
 */ 
#include "dva_core.h"

//[15]�͖��g�p
voice_param_t voices[16];

psg_param_t psgs[4];

uint8_t fm_params[3][38];

void init_dva(void){
	for(uint8_t i = 0;i < 16;i++){
		voices[i].assign_map = 0xFFFE;	/*�S�`���l���ɑ΂��Ċ��蓖�ĉ\*/
		voices[i].assigned_ch = 0xFF;	/*�ǂ��ɂ����蓖�ĂȂ�*/
		voices[i].note_number = 0xFF;
		voices[i].vel		 = 0x00;
		voices[i].note_state  = 0x00;	/*�m�[�g�I�t*/
		voices[i].priority    = 0x00;
		voices[i].sustain     = 0x00;
	}

	for(uint8_t i = 12;i < 15;i++){
		voices[i].assign_map = 0x0001;
	}
	
	voices[15].assign_map = 0x000;
	for(uint8_t i = 0;i < 16;i++){
		xprintf("%04x\n",voices[i].assign_map);
	}

	for(uint8_t i = 0;i < 4;i++){
		psg_reset(i);
		psg_set_mixer(i,0b00111000);
		psg_set_efreq(i,0b0001111010000100);
		psgs[i].env_freq = 0b0001111010000100;
		psgs[i].env_shape = 0xFF;
		psgs[i].noise_freq = 0x00;
		psgs[i].output = 0b00111111;
	}
	
	init_timbre_mgr();
	opn_reset();
	load_timbre_rom(0,&fm_params[0][0]);
	load_timbre_rom(0,&fm_params[1][0]);
	load_timbre_rom(0,&fm_params[2][0]);
	
	for(uint8_t i = 0;i < 3;i++){
		opn_pgm_set(i,&fm_params[i][0]);
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
	
	xprintf("%02x\n",ch);
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
		uint8_t opn_ch = min_vo - 12;
		opn_set_fnum(opn_ch,calc_fm_tone(note,midi_channel[ch].pbs,midi_channel[ch].pb));
		opn_note_on(opn_ch,0x0F);
		/*FM*/
	} else {
		/*PSG*/
		uint8_t psg_id = min_vo / 3;
		uint8_t psg_ch = min_vo % 3;
		uint16_t tone = calc_psg_tone(note,midi_channel[ch].pbs,midi_channel[ch].pb);
		
		psg_set_mfreq(psg_id,psg_ch,tone);
		
		if(midi_channel[ch].pgm != 0){
			psg_set_volume(psg_id,psg_ch,0x10);
			psg_set_envelope(psg_id,env_table[(midi_channel[ch].pgm - 1) % 10]);
		} else {
			psg_set_volume(psg_id,psg_ch,vol_curve[midi_channel[ch].exp >> 3][midi_channel[ch].vol >> 3]);
		}
		
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
		/*
		if(voices[i].sustain == 0x00 && voices[i].note_state == 0x00){
			if(i > 11){
					uint8_t opn_ch = i - 12;
					opn_note_off(opn_ch,0x0F);
				} else {
					uint8_t psg_id = i / 3;
					uint8_t psg_ch = i % 3;
					psg_set_volume(psg_id,psg_ch,0);
			}
		}
		*/
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
		uint8_t opn_ch = off_vo - 12;
		opn_note_off(opn_ch,0x0F);
	} else {
		/*PSG*/
		uint8_t psg_id = off_vo / 3;
		uint8_t psg_ch = off_vo % 3;
		//uint16_t tone = calc_psg_tone(note,midi_channel[ch].pbs,midi_channel[ch].pb);
		psg_set_volume(psg_id,psg_ch,0);
	}	
}

void dva_pitchbend(uint8_t ch){
	for(uint8_t i = 0;i < 15;i++){
		if(voices[i].assigned_ch == ch && voices[i].note_state){
			if(i > 11){
							
				} else {
				//PSG
				uint8_t psg_id = i / 3;
				uint8_t psg_ch = i % 3;
				uint16_t tone = calc_psg_tone(voices[i].note_number,midi_channel[ch].pbs,midi_channel[ch].pb);
				psg_set_mfreq(psg_id,psg_ch,tone);				
			}
		}
	}
}

void dva_vol(uint8_t ch){
	uint8_t vol = vol_curve[midi_channel[ch].exp >> 3][midi_channel[ch].vol >> 3];
	
	for(uint8_t i = 0;i < 15;i++){
		if(voices[i].assigned_ch == ch && voices[i].note_state){
			if(i > 11){
				
				} else {
				//PSG
				uint8_t psg_id = i / 3;
				uint8_t psg_ch = i % 3;
				psg_set_volume(psg_id,psg_ch,vol);
				
			}
		}
	}
}

void dva_exp(uint8_t ch){
	uint8_t vol = vol_curve[midi_channel[ch].exp >> 3][midi_channel[ch].vol >> 3];
	
	for(uint8_t i = 0;i < 15;i++){
		if(voices[i].assigned_ch == ch && voices[i].note_state){
			if(i > 11){
				
				} else {
				//PSG
				uint8_t psg_id = i / 3;
				uint8_t psg_ch = i % 3;
				psg_set_volume(psg_id,psg_ch,vol);
				
			}
		}
	}	
}

void dva_hold(uint8_t ch){
	
}