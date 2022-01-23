/*
 * opn_driver.c
 *
 * Created: 2022/01/23 9:42:25
 *  Author: Owner
 */ 
#include "opn_driver.h"


void opn_set_fnum(uint8_t ch,uint16_t fnum){
	opn_write(0xA4 + ch,(fnum & 0xFF00) >> 8);
	opn_write(0xA0 + ch,fnum & 0x00FF);
}


void opn_set_dt_mul(uint8_t ch,uint8_t op,uint8_t dt,uint8_t mul){
	opn_write(0x30 + (op << 2) + ch,(dt << 4) | mul);
}

void opn_set_tl(uint8_t ch,uint8_t op,uint8_t val){
	opn_write(0x40 + (op << 2) + ch,val);
}

void opn_set_ks_ar(uint8_t ch,uint8_t op,uint8_t ks,uint8_t ar){
	opn_write(0x50 + (op << 2) + ch,(ks << 6) | ar);
}

void opn_set_dr(uint8_t ch,uint8_t op,uint8_t val){
	opn_write(0x60 + (op << 2) + ch,val);
}

void opn_set_sr(uint8_t ch,uint8_t op,uint8_t val){
	opn_write(0x70 + (op << 2) + ch,val);
}

void opn_set_sl_rr(uint8_t ch,uint8_t op,uint8_t sl,uint8_t rr){
	opn_write(0x80 + (op << 2) + ch,(sl << 4) | rr);
}

void opn_set_fb_cn(uint8_t ch,uint8_t fb,uint8_t cn){
	opn_write(0xB0 + ch,(fb << 3) | cn);
}

void opn_write_addr(uint8_t addr){
	PORTC.OUT &= ~(0x01);	//A0を0(アドレスライトモード)
	PORTD.OUT = addr;		//アドレスをセット
	PORTE.OUT &= ~(0x01);	//WRをアサート
	for(uint16_t i = 0;i < 60;i++)asm("nop;");
	PORTE.OUT |= 0x01;	//WRをネゲート
}

void opn_write_data(uint8_t data){
	PORTC.OUT |= 0x01;		//A0を1(データライトモード)
	PORTD.OUT = data;		//データをセット
	PORTE.OUT &= ~(0x01);	//WRをアサート
	for(uint16_t i = 0;i < 400;i++)asm("nop;");
	PORTE.OUT |= 0x01;	//WRをネゲート
}

void opn_select_ic(uint8_t id,uint8_t state){
	if(state){
		//アサート
		PORTC.OUT &= ~(0x02 << id);
		} else {
		//ネゲート
		PORTC.OUT |= (0x02 << id);
	}
}


void opn_write(uint8_t addr,uint8_t data){
	//xprintf("WRITE %02x %02x %02x\n",id,addr,data);
	opn_select_ic(4,1);
	opn_write_addr(addr);
	opn_select_ic(4,0);
	for(uint16_t i = 0;i < 60;i++)asm("nop;");
	opn_select_ic(4,1);
	opn_write_data(data);
	opn_select_ic(4,0);;
}

void opn_reset(void){
	//特に何もしない(今のところは)
}

void opn_note_on(uint8_t  ch,uint8_t op){
	opn_write(0x28,(ch & 0x03) | (op << 4));
}

void opn_note_off(uint8_t ch,uint8_t op){
	opn_write(0x28,(ch & 0x03) | (((!op) & 0x0F)<< 4));
}

void opn_pgm_set(uint8_t ch,uint8_t* data){
	opn_set_fb_cn(ch,data[1],data[0]);
	for(uint8_t i = 0;i < 4;i++){
		uint8_t index = 2 + (i * 9);
		opn_set_ks_ar(ch,i,data[index + 6],data[index]);
		opn_set_dr(ch,i,data[index+1]);
		opn_set_sr(ch,i,data[index+2]);
		opn_set_sl_rr(ch,i,data[index+4],data[index+3]);
		opn_set_tl(ch,i,data[index+5]);
		opn_set_dt_mul(ch,i,data[index+8],data[index+7]); 
	}
}
