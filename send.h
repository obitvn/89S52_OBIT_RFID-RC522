#ifndef	_SEND_H 				// 防止RFID_main.h被重复引用

#define	_SEND_H

#include <reg52.h>			    // 引用标准库的头文件
#include <stdio.h>
#include <string.h>
#include"intrins.h"

#define uchar unsigned char 
#define uint unsigned int
#define lcd_port P0
/* 握手信号宏定义 */
#define SUCC 0x02		// 接收成功
#define ERR 0x04		// 接收错误

uchar code SEARCHCARD[5]={0x47,0x02,0x01,0x00,0x44};
uchar code KEY[12]={0x47,0x09,0x05,0x00,0x06,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x4D};
uchar code CHECK[6]={0x47,0x03,0x09,0x00,0x08,0x45};
uchar code READDATA[5]={0x47,0x02,0x07,0x40,0x02};
idata uchar WRITEDATA[21]={0x47,0x12,0x08,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
                    ,0x00,0x00,0x00,0x00,0x00};
idata uchar DATA[3];
idata uchar return_frame[21];
uchar code Wellcome[]="欢迎使用租赁系统";
uchar code Unit[]="华农电子信息工程";
uchar code Design[]="张虹林宏典卢小冰";
uchar code Time[]="2009年04月10日  ";
idata uchar Borrow_bicycle[]="请在    号位取车";
idata uchar Return_bicycle[]="请在    号位还车";
idata uchar Account[]="您的余额:     元";
uchar addr;
uchar code Transmit_error[]=("请选择其它车位");
bit Transmit_f=0;
 
void Send(uchar m);				                     // 发送单字节数据
void Delay_ms(uint count);
uchar Receive();
bit search();
bit password();
bit check_password();
bit read();
bit write();
void dataprocess();
void Display();
void LCD_Busy ();
void LCD_Wcmd ( uchar cmd );
void LCD_Wdat ( uchar dat );
void LCD_Init ();
void OutWord( uchar place, uchar unit, uchar charcode[] );
uchar bicycle_ctr(uchar status);
void Transmit();
sbit rd =P0^7;		/*读取忙信号，高电平表示忙，低电平表示不忙*/
sbit RS = P2^0;     /*RS为寄存器选择，高电平时选择数据寄存器、低电平时选择指令寄存器*/        
sbit RW = P2^1;	    /*RW为读写信号线，高电平时进行读操作，低电平时进行写操作*/
sbit EN = P2^2;		/*E端为使能端，当E端由高电平跳变成低电平时，液晶模块执行命令*/
sbit bee= P2^3;			//蜂鸣器管脚
sbit DE = P2^4;
sbit RE = P2^5;
#endif