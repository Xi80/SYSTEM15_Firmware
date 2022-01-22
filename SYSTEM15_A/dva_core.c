/*
 * dva_core.c
 *
 * Created: 2022/01/22 8:20:14
 *  Author: Owner
 */ 
#include "dva_core.h"

//[15]は未使用
voice_param_t voices[16];

void init_dva(void){
	for(uint8_t i = 0;i < 16;i++){
		voices[i].assign_map = 0xFFFF;	/*全チャネルに対して割り当て可能*/
		voices[i].assigned_ch = 0xFF;	/*どこにも割り当てない*/
		voices[i].note_number = 0xFF;
		voices[i].vel		 = 0x00;
		voices[i].note_state  = 0x00;	/*ノートオフ*/
		voices[i].priority    = 0x00;
		voices[i].sustain     = 0x00;
	}
	voices[3].assign_map = 0x0000;
	voices[4].assign_map = 0x0000;
	voices[5].assign_map = 0x0000;
	voices[13].assign_map = 0x0000;
	voices[14].assign_map = 0x0000;
	voices[15].assign_map = 0x0000;		/*ボイス15は使用不可(ないので)*/
	
	xprintf("init dva\n");
}

void dva_note_on(uint8_t ch,uint8_t note,uint8_t vel){
	uint8_t min_prio = 16;
	uint8_t min_vo   = 16;
	
	
	midi_channel[ch].voice_count = 0;
	xprintf("\e[2J");
	for(uint8_t i = 0;i < 16;i++){
		xprintf("%02x %04x %02x %02x %02x %02x %02x %02x\n",i,voices[i].assign_map,voices[i].assigned_ch,voices[i].note_number,voices[i].note_state,voices[i].priority,voices[i].sustain,voices[i].vel);
	}
	xprintf("\e[1;1H\e[1;1f");
	for(uint8_t i = 0;i < 16;i++){
		
		if((voices[i].assign_map & (1 << ch)) && !voices[i].sustain){
			
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
	voices[min_vo].sustain = 0;
	voices[min_vo].priority = midi_channel[ch].voice_count;
	
	for(uint8_t i = 0;i < 16;i++){
		if(voices[i].assigned_ch == ch){
			voices[i].priority -= (voices[i].priority)?  1 : 0;
		}
	}

}


void dva_note_off(uint8_t ch,uint8_t note){
	uint8_t off_count = 0;
	uint8_t off_prio  = 0;
	uint8_t off_vo    = 15;
	
	for(uint8_t i = 0;i < 16;i++){
		if((voices[i].assign_map & (1 << ch))){
			if(!voices[i].note_state)off_count++;
		}
		
		if(voices[i].assigned_ch == ch && voices[i].note_number == note){
			off_prio = voices[i].priority;
			off_vo = i;
		}
	}
	 
	/*ノートオフ処理*/
	voices[off_vo].note_state = 0;
	voices[off_vo].priority = off_count;
	for(uint8_t i = 0;i < 16;i++){
		if(voices[i].assigned_ch == ch){
			if(voices[i].note_state){
				voices[i].priority -= ((voices[i].priority > off_prio) && (voices[i].priority))? 1 : 0;
			}
		}
	}
	
}

void dva_pitchbend(uint8_t ch){
	
}

void dva_cc(uint8_t ch){
	
}