/*
 * opn_driver.h
 *
 * Created: 2022/01/23 9:42:06
 *  Author: Owner
 */ 


#ifndef OPN_DRIVER_H_
#define OPN_DRIVER_H_

#include <avr/io.h>

void opn_reset(void);

void opn_note_on(uint8_t,uint8_t);		//CH,OP(基本0x0F)
void opn_note_off(uint8_t,uint8_t);		//CH,OP(基本0x0F)

void opn_pgm_set(uint8_t,uint8_t*);		//CH,音色
void opn_set_fnum(uint8_t,uint16_t);	//CH,F-NumberとBlcok連結させたデータ

void led_show(uint16_t data);
#endif /* OPN_DRIVER_H_ */