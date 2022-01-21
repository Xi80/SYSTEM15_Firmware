/*
 * midi_manager.c
 *
 * Created: 2022/01/22 0:39:50
 *  Author: Owner
 */ 
#include "midi_manager.h"

void init_midi_mgr(void){
	/*‚Ü‚¾*/
	return;
}

void mgr_note_on(uint8_t ch,uint8_t note,uint8_t vel){
	xprintf("NOTE ON:ch%d note%d vel%d\n",ch,note,vel);
}

void mgr_note_off(uint8_t ch,uint8_t note){
	xprintf("NOTE OFF:ch%d note%d\n",ch,note);
}

void mgr_program_change(uint8_t ch ,uint8_t val){
	xprintf("PROGRAM CHANGE:ch%d val%d\n",ch,val);	
}

void mgr_pitchbend_sensitivity(uint8_t ch,uint8_t val){
	xprintf("PITCH BEND SENS:ch%d val%d\n",ch,val);
}
void mgr_pitchbend(uint8_t ch,uint16_t val){
	xprintf("PITCH BEND:ch%d val%d\n",ch,val);
}

void mgr_modulation(uint8_t ch,uint8_t val){
	xprintf("MODULATION:ch%d val%d\n",ch,val);
}

void mgr_volume(uint8_t ch,uint8_t val){
	xprintf("VOLUME:ch%d val%d\n",ch,val);
}

void mgr_expression(uint8_t ch,uint8_t val){
	xprintf("EXPRESSION:ch%d val%d\n",ch,val);
}

void mgr_hold(uint8_t ch,uint8_t val){
	xprintf("HOLD:ch%d val%d\n",ch,val);
}

void mgr_all_sounds_off(uint8_t ch){
	
}

void mgr_reset_all_controller(uint8_t ch){
	
}

void mgr_all_notes_off(uint8_t ch){
	
}

void mgr_reset(void){
	xprintf("RESET!\n");
}

void mgr_sysex(uint8_t *arr,uint8_t size){
	xprintf("SYSEX:");
	for(int i = 0;i < size;i++){
		xprintf("%02x,",arr[i]);
	}
	xprintf("\n");
}