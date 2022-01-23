/*
 * psg_driver.c
 *
 * Created: 2022/01/22 18:20:10
 *  Author: Owner
 */ 

#include "psg_driver.h"

void psg_set_mfreq(uint8_t id,uint8_t ch,uint16_t tp){
	psg_write(id,REG_MFREQ_BASE + (ch << 1),tp & 0xFF);
	psg_write(id,REG_MFREQ_BASE + 1 + (ch << 1),(tp >> 8) & 0xFF);
}

void psg_set_nfreq(uint8_t id,uint8_t np){
	psg_write(id,REG_NFREQ,np);
}

void psg_set_efreq(uint8_t id,uint16_t ep){
	psg_write(id,REG_EFREQ_BASE,ep & 0x00FF);
	psg_write(id,REG_EFREQ_BASE + 1,(ep >> 8) & 0xFF);
}

void psg_set_mixer(uint8_t id,uint8_t val){
	psg_write(id,REG_MIXER,val);
}

void psg_set_volume(uint8_t id,uint8_t ch,uint8_t vol){
	psg_write(id,REG_VOL_BASE + ch,vol);
}

void psg_set_envelope(uint8_t id,uint8_t shape){
	psg_write(id,REG_ENV,(shape) & 0x0F);
}

void psg_reset(uint8_t id){
	psg_write(id,0x00, 0);
	psg_write(id,0x01, 0);
	psg_write(id,0x02, 0);
	psg_write(id,0x03, 0);
	psg_write(id,0x04, 0);
	psg_write(id,0x05, 0);
	psg_write(id,0x06, 0);

	psg_write(id,0x07, 0);
	psg_write(id,0x08, 0);
	psg_write(id,0x09, 0);
	psg_write(id,0x0a, 0);

	psg_write(id,0x0b, 0);
	psg_write(id,0x0c, 0);
	psg_write(id,0x0d, 0);	
}

void psg_select_ic(uint8_t id,uint8_t state){
	if(state){
		//アサート
		PORTC.OUT &= ~(0x02 << id);
		} else {
		//ネゲート
		PORTC.OUT |= (0x02 << id);
	}
}

void psg_write_addr(uint8_t addr){
	PORTC.OUT &= ~(0x01);	//A0を0(アドレスライトモード)
	PORTD.OUT = addr;		//アドレスをセット
	PORTE.OUT &= ~(0x01);	//WRをアサート
	//for(uint16_t i = 0;i < 20;i++)asm("nop;");
	PORTE.OUT |= 0x01;	//WRをネゲート
}

void psg_write_data(uint8_t data){
	PORTC.OUT |= 0x01;		//A0を1(データライトモード)
	PORTD.OUT = data;		//データをセット
	PORTE.OUT &= ~(0x01);	//WRをアサート
	//for(uint16_t i = 0;i < 20;i++)asm("nop;");
	PORTE.OUT |= 0x01;	//WRをネゲート
}


void psg_write(uint8_t id,uint8_t addr,uint8_t data){
	//xprintf("WRITE %02x %02x %02x\n",id,addr,data);
	psg_select_ic(id,1);
	psg_write_addr(addr);
	psg_select_ic(id,0);
	//for(uint16_t i = 0;i < 20;i++)asm("nop;");
	psg_select_ic(id,1);
	psg_write_data(data);
	psg_select_ic(id,0);;
}