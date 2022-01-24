/*
 * dva_core.c
 *
 * Created: 2022/01/22 8:20:14
 *  Author: Owner
 */ 
#include "dva_core.h"

//[15]は未使用
voice_param_t voices[16];

psg_param_t psgs[4];

uint8_t fm_params[3][38];

uint16_t voice_state;

void init_dva(void){
	dva_reset_ic();
	for(uint8_t i = 0;i < 16;i++){
		voices[i].assign_map = 0x0000;	/*全チャネルに対して割り当て可能*/
		voices[i].assigned_ch = 0xFF;	/*どこにも割り当てない*/
		voices[i].note_number = 0xFF;
		voices[i].vel		 = 0x00;
		voices[i].note_state  = 0x00;	/*ノートオフ*/
		voices[i].priority    = 0x00;
		voices[i].sustain     = 0x00;
	}

	for(uint8_t i = 12;i < 15;i++){
		voices[i].assign_map = 0x0000;
	}

	voices[15].assign_map = 0x0000;

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
	voice_state = 0x0000;
	load_timbre_rom(0,&fm_params[0][0]);
	load_timbre_rom(0,&fm_params[1][0]);
	load_timbre_rom(0,&fm_params[2][0]);
	store_timbre_ram(0,&fm_params[0][0]);
	for(uint8_t i = 0;i < 3;i++){
		opn_pgm_set(i,&fm_params[i][0]);
	}
	
	psg_set_mfreq(0,0,TP_F(2000));
	psg_set_volume(0,0,15);
	_delay_ms(100);
	psg_set_mfreq(0,0,TP_F(1000));
	_delay_ms(50);
	psg_set_volume(0,0,0);	
	
	for(uint8_t i = 0;i < 15;i++){
		voice_state |= (1 << i);
		led_show(BYTE_SWAP_16(voice_state));
		_delay_ms(50);
	}
	voice_state = 0x00;
	led_show(BYTE_SWAP_16(voice_state));
}

void dva_note_on(uint8_t ch,uint8_t note,uint8_t vel){
	uint8_t min_prio = 16;
	uint8_t min_vo   = 16;
	
	
	midi_channel[ch].voice_count = 0;
	for(uint8_t i = 0;i < 16;i++){
		
		if(!voices[i].note_state){
			if(!voices[i].sustain)voices[i].assigned_ch = 16;
			voices[i].note_state = 0;
			voices[i].priority = 0;
			voices[i].sustain = 0;
					
			if(i > 11){
				/*FM*/
				uint8_t opn_ch = i - 12;
				opn_note_off(opn_ch,0x0F);
				} else {
				/*PSG*/
				uint8_t psg_id = i / 3;
				uint8_t psg_ch = i % 3;
				//uint16_t tone = calc_psg_tone(note,midi_channel[ch].pbs,midi_channel[ch].pb);
				psg_set_volume(psg_id,psg_ch,0);
			}
		}
				
		if((voices[i].assign_map & (1 << ch)) && !voices[i].sustain && !voices[i].note_state){
			
			/*そのチャネルに対して割り当て可能*/
			if(voices[i].priority <= min_prio){
				/*より優先順位が高い*/
				
				min_prio = voices[i].priority;
				min_vo   = i;
				
			}
			
		}
		
		
		if(voices[i].assigned_ch == ch){
			midi_channel[ch].voice_count++;
		}
		
		
	}
	
	if(min_vo == 16)return;	//空きがない
	
	/*ノートオン処理*/
	voices[min_vo].assigned_ch = ch;
	voices[min_vo].note_number = note;
	voices[min_vo].vel = vel;
	voices[min_vo].note_state = 1;
	voices[min_vo].sustain = midi_channel[ch].hold;
	voices[min_vo].priority = midi_channel[ch].voice_count;
	
	voice_state |= (1 << min_vo);
	led_show(BYTE_SWAP_16(voice_state));
	
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
		
		if(psgs[psg_id].env_shape != 0xFF && midi_channel[ch].pgm != 0){
			psg_set_volume(psg_id,psg_ch,0x10);
			psg_set_envelope(psg_id,psgs[psg_id].env_shape);
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
	/*ノートオフ処理*/
	
	voice_state &= ~(1 << (off_vo));
	led_show(BYTE_SWAP_16(voice_state));
	if(!voices[off_vo].sustain)voices[off_vo].assigned_ch = 16;
	voices[off_vo].note_state = 0;
	voices[off_vo].priority = off_count;
	for(uint8_t i = 0;i < 16;i++){
		if((voices[i].assigned_ch == ch ) && voices[i].note_state){
			if(voices[i].note_state){
				voices[i].priority += (voices[i].priority < off_prio)? 1 : 0;
			}
		}
	}
	
	if(voices[off_vo].sustain)return;
	
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
	if(midi_channel[ch].hold){

	} else {
		for(uint8_t i = 0;i < 16;i++){

			if(voices[i].sustain && voices[i].assigned_ch == ch){
				voices[i].sustain = 0x00;

				if(voices[i].note_state == 0x00){
					if(i > 11){
						uint8_t opn_ch = i - 12;
						opn_note_off(opn_ch,0x0F);
						} else {
						uint8_t psg_id = i / 3;
						uint8_t psg_ch = i % 3;
						psg_set_volume(psg_id,psg_ch,0);
					}					
				}

			}
		}
	}

}

void dva_program_change(uint8_t ch){
	for(uint8_t i = 12;i < 15;i++){
		if(voices[i].assign_map & (1 << ch)){
			load_timbre_ram(midi_channel[ch].pgm,&fm_params[i - 12][0]);
			opn_pgm_set(i - 12,&fm_params[i - 12][0]);
		}
	}
}

void dva_reset(uint8_t type){

	dva_reset_ic();
	for(uint8_t i = 0;i < 16;i++){
		voices[i].assign_map = 0x0000;	/*全チャネルに対して割り当て可能*/
		voices[i].assigned_ch = 0xFF;	/*どこにも割り当てない*/
		voices[i].note_number = 0xFF;
		voices[i].vel		 = 0x00;
		voices[i].note_state  = 0x00;	/*ノートオフ*/
		voices[i].priority    = 0x00;
		voices[i].sustain     = 0x00;
	}

	for(uint8_t i = 12;i < 15;i++){
		voices[i].assign_map = 0x0000;
	}

	voices[15].assign_map = 0x000;

	for(uint8_t i = 0;i < 4;i++){
		psg_reset(i);
		psg_set_mixer(i,0b00111000);
		psg_set_efreq(i,0b0001111010000100);
		psgs[i].env_freq = 0b0001111010000100;
		psgs[i].env_shape = 0xFF;
		psgs[i].noise_freq = 0x00;
		psgs[i].output = 0b00111111;
	}
	opn_reset();
	voice_state = 0x0000;
	
	load_timbre_rom(0,&fm_params[0][0]);
	load_timbre_rom(0,&fm_params[1][0]);
	load_timbre_rom(0,&fm_params[2][0]);
	
	for(uint8_t i = 0;i < 3;i++){
		opn_pgm_set(i,&fm_params[i][0]);
	}
}

void dva_all_sounds_off(uint8_t ch){
	for(uint8_t i = 0;i < 15;i++){
		if(voices[i].assigned_ch != ch)continue;
		voices[i].note_state = 0;
		voices[i].priority = 0;
		voices[i].sustain = 0;
		if(i > 11){
			/*FM*/
			uint8_t opn_ch = i - 12;
			opn_note_off(opn_ch,0x0F);
			} else {
			/*PSG*/
			uint8_t psg_id = i / 3;
			uint8_t psg_ch = i % 3;
			//uint16_t tone = calc_psg_tone(note,midi_channel[ch].pbs,midi_channel[ch].pb);
			psg_set_volume(psg_id,psg_ch,0);
		}
	}
}

void dva_all_notes_off(uint8_t ch){
	dva_all_sounds_off(ch);
}

void dva_reset_all_controller(uint8_t ch){
	dva_vol(ch);
	dva_pitchbend(ch);
}

void dva_change_assign(uint8_t *data){
	for(uint8_t j = 0;j < 15;j++){
		voices[j].assign_map &= ~(1 << data[0]);
		for(uint8_t i = 2;i < data[1] + 2;i++){
			if(data[i] == j){
				voices[j].assign_map |= (1 << data[0]);
			}
		}
	}
}

void dva_change_mixer(uint8_t* data){
	psg_set_mixer(data[0],data[1]);
}

void dva_change_envelope(uint8_t *data){
	psgs[data[0]].env_shape = data[1];
}

void dva_change_envelope_freq(uint8_t *data){
	uint16_t freq = data[1] << 7 | data[2];
	psg_set_efreq(data[0],(125000 / freq));	
	
	for(uint8_t i = 0;i < 15;i++){
		xprintf("%04x\n",voices[i].assign_map);
	}
}

void dva_transfer_opn_voice(uint8_t *data){
	store_timbre_ram(data[0],&data[1]);
}

void dva_save_opn_voice(uint8_t *data){
	uint8_t tmp[38];
	load_timbre_ram(data[0],&tmp[0]);
	store_timbre_rom(data[0],&tmp[0]);
}

void dva_load_opn_voice(uint8_t *data){
	uint8_t tmp[38];
	load_timbre_rom(data[0],&tmp[0]);
	store_timbre_ram(data[0],&tmp[0]);
}

void dva_change_opn_voice(uint8_t ch,uint8_t pos,uint8_t val){
	uint8_t op = pos / 38;
	
	fm_params[op][pos % 38] = val;
}

void dva_reset_ic(void){
	PORTE.OUT &= ~(0x04);	//ICをアサート
	for(uint16_t i = 0;i < 1024;i++){
		asm("nop;");
	}
	PORTE.OUT |= 0x04;		//ICをネゲート
	for(uint16_t i = 0;i < 16384;i++){
		asm("nop;");
	}
}
