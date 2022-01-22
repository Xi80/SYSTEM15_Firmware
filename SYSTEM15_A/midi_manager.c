/*
 * midi_manager.c
 *
 * Created: 2022/01/22 0:39:50
 *  Author: Owner
 */ 
#include "midi_manager.h"

midi_channel_t midi_channel[16];

void init_midi_mgr(void){
	/*‚Ü‚¾*/
	
	for(uint8_t i = 0;i < 16;i++){
		midi_channel[i].voice_assign = 15;
		midi_channel[i].voice_count = 0;
		midi_channel[i].pgm = 0;
		midi_channel[i].mod = 0;
		midi_channel[i].vol = 100;
		midi_channel[i].exp	= 127;
		midi_channel[i].hold= 0;
		midi_channel[i].pb  = 8192;
		midi_channel[i].pbs = 2;
	}
	
	init_dva();
	return;
}

void mgr_note_on(uint8_t ch,uint8_t note,uint8_t vel){
	//xprintf("NOTE ON:ch%d note%d vel%d\n",ch,note,vel);
	dva_note_on(ch,note,vel);
}

void mgr_note_off(uint8_t ch,uint8_t note){
	//xprintf("NOTE OFF:ch%d note%d\n",ch,note);
	dva_note_off(ch,note);
}

void mgr_program_change(uint8_t ch ,uint8_t val){
	//xprintf("PROGRAM CHANGE:ch%d val%d\n",ch,val);	
	midi_channel[ch].pgm = val;
}

void mgr_pitchbend_sensitivity(uint8_t ch,uint8_t val){
	//xprintf("PITCH BEND SENS:ch%d val%d\n",ch,val);
	midi_channel[ch].pbs = val;
}
void mgr_pitchbend(uint8_t ch,uint16_t val){
	//xprintf("PITCH BEND:ch%d val%d\n",ch,val);
	midi_channel[ch].pb = val;
}

void mgr_modulation(uint8_t ch,uint8_t val){
	//xprintf("MODULATION:ch%d val%d\n",ch,val);
	midi_channel[ch].mod = val;
}

void mgr_volume(uint8_t ch,uint8_t val){
	//xprintf("VOLUME:ch%d val%d\n",ch,val);
	midi_channel[ch].vol = val;
}

void mgr_expression(uint8_t ch,uint8_t val){
	//xprintf("EXPRESSION:ch%d val%d\n",ch,val);
	midi_channel[ch].exp = val;
	
}

void mgr_hold(uint8_t ch,uint8_t val){
	//xprintf("HOLD:ch%d val%d\n",ch,val);
	midi_channel[ch].hold = (val > 63);
	
}

void mgr_all_sounds_off(uint8_t ch){
	
}

void mgr_reset_all_controller(uint8_t ch){
	for(uint8_t i = 0;i < 16;i++){
		midi_channel[i].voice_count = 0;
		midi_channel[i].pgm = 0;
		midi_channel[i].mod = 0;
		midi_channel[i].vol = 100;
		midi_channel[i].exp	= 127;
		midi_channel[i].hold= 0;
		midi_channel[i].pb  = 8192;
		midi_channel[i].pbs = 2;
	}
		
}

void mgr_all_notes_off(uint8_t ch){
	
}

void mgr_reset(void){
	//xprintf("RESET!\n");
}

void mgr_sysex(uint8_t *arr,uint8_t size){
	/*
	xprintf("SYSEX:");
	for(int i = 0;i < size;i++){
		xprintf("%02x,",arr[i]);
	}
	xprintf("\n");
	*/
}