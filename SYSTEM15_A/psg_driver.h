/*
 * psg_driver.h
 *
 * Created: 2022/01/22 18:20:01
 *  Author: Owner
 */ 


#ifndef PSG_DRIVER_H_
#define PSG_DRIVER_H_

#include <avr/io.h>
#include "xprintf.h"

void psg_set_mfreq(uint8_t,uint8_t,uint16_t);	//ID,ch,tp		�����f�B�[���g��
void psg_set_nfreq(uint8_t,uint8_t);			//ID,np			�m�C�Y���g��
void psg_set_efreq(uint8_t,uint16_t);			//ID,ep			�G���x���[�v���g��

void psg_set_mixer(uint8_t,uint8_t);	//ID,�~�L�V���O

void psg_set_volume(uint8_t,uint8_t,uint8_t);	//ID,ch,�{�����[��(0x10�ŃG���x���[�v)

void psg_set_envelope(uint8_t,uint8_t);			//ID,�`��

void psg_reset(uint8_t);						//ID
void psg_write(uint8_t,uint8_t,uint8_t);		//ID,addr,data

/*�}�N��*/
#define REG_MFREQ_BASE	0x00
#define REG_NFREQ		0x06
#define REG_EFREQ_BASE	0x0B
#define REG_MIXER		0x07
#define REG_VOL_BASE	0x08
#define REG_ENV			0x0D

#define BASE_FREQ		125000

#define TP_F(x)			((BASE_FREQ / x) & 0x0FFF)
#endif /* PSG_DRIVER_H_ */