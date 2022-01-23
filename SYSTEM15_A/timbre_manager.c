/*
 * timbre_manager.c
 *
 * Created: 2022/01/23 9:44:50
 *  Author: Owner
 */ 

#include "timbre_manager.h"

void init_timbre_mgr(void){
	memset(&timbre_ram,0x00,38*64);
	xprintf("Timbre Mgr Init\n");
}

void store_timbre_ram(uint8_t id,uint8_t *data){
	memcpy(&timbre_ram[id * 38],data,38);
}

void load_timbre_ram(uint8_t id,uint8_t *data){
	memcpy(data,&timbre_ram[38*id],38);
}

void store_timbre_rom(uint8_t id,uint8_t *data){
	//EEPROM...
	if(id > 3)return; /*�͈͊O*/
	eeprom_busy_wait();
	eeprom_write_block(data,(38*id),(size_t)38);
	return;
}

void load_timbre_rom(uint8_t id,uint8_t* data){
	eeprom_busy_wait();
	eeprom_read_block(data,(38*id),(size_t)38);
}