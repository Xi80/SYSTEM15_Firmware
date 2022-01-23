/*
 * timbre_manager.h
 *
 * Created: 2022/01/23 9:44:35
 *  Author: Owner
 */ 


#ifndef TIMBRE_MANAGER_H_
#define TIMBRE_MANAGER_H_

#include <avr/io.h>
#include <avr/eeprom.h>
#include <string.h>
#include "xprintf.h"
void init_timbre_mgr(void);

void store_timbre_ram(uint8_t id,uint8_t *data);
void load_timbre_ram(uint8_t id,uint8_t *data);

void store_timbre_rom(uint8_t id,uint8_t *data);
void load_timbre_rom(uint8_t id,uint8_t *data);


/*âπêFRAM*/
uint8_t timbre_ram[64][38];

#endif /* TIMBRE_MANAGER_H_ */