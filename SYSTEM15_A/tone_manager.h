/*
 * tone_manager.h
 *
 * Created: 2022/01/22 18:08:58
 *  Author: Owner
 */ 


#ifndef TONE_MANAGER_H_
#define TONE_MANAGER_H_


#include <avr/io.h>
#include "xprintf.h"
uint16_t calc_psg_tone(uint8_t,uint8_t,uint16_t);		/*ノート番号,ピッチベンドセンシティビティ,ピッチベンド*/
uint16_t calc_fm_tone(uint8_t,uint8_t,uint16_t);		/*ノート番号,ピッチベンドセンシティビティ,ピッチベンド*/

const uint16_t fm_tone_table[768];
const uint16_t psg_tone_table[768];

const uint8_t vol_curve[16][16];

const uint8_t env_table[10];
#endif /* TONE_MANAGER_H_ */