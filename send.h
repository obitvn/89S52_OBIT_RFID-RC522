#ifndef	_SEND_H 				// ��ֹRFID_main.h���ظ�����

#define	_SEND_H

#include <reg52.h>			    // ���ñ�׼���ͷ�ļ�
#include <stdio.h>
#include <string.h>
#include"intrins.h"

#define uchar unsigned char 
#define uint unsigned int
#define lcd_port P0
/* �����źź궨�� */
#define SUCC 0x02		// ���ճɹ�
#define ERR 0x04		// ���մ���

uchar code SEARCHCARD[5]={0x47,0x02,0x01,0x00,0x44};
uchar code KEY[12]={0x47,0x09,0x05,0x00,0x06,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x4D};
uchar code CHECK[6]={0x47,0x03,0x09,0x00,0x08,0x45};
uchar code READDATA[5]={0x47,0x02,0x07,0x40,0x02};
idata uchar WRITEDATA[21]={0x47,0x12,0x08,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
                    ,0x00,0x00,0x00,0x00,0x00};
idata uchar DATA[3];
idata uchar return_frame[21];
uchar code Wellcome[]="��ӭʹ������ϵͳ";
uchar code Unit[]="��ũ������Ϣ����";
uchar code Design[]="�ź��ֺ��¬С��";
uchar code Time[]="2009��04��10��  ";
idata uchar Borrow_bicycle[]="����    ��λȡ��";
idata uchar Return_bicycle[]="����    ��λ����";
idata uchar Account[]="�������:     Ԫ";
uchar addr;
uchar code Transmit_error[]=("��ѡ��������λ");
bit Transmit_f=0;
 
void Send(uchar m);				                     // ���͵��ֽ�����
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
sbit rd =P0^7;		/*��ȡæ�źţ��ߵ�ƽ��ʾæ���͵�ƽ��ʾ��æ*/
sbit RS = P2^0;     /*RSΪ�Ĵ���ѡ�񣬸ߵ�ƽʱѡ�����ݼĴ������͵�ƽʱѡ��ָ��Ĵ���*/        
sbit RW = P2^1;	    /*RWΪ��д�ź��ߣ��ߵ�ƽʱ���ж��������͵�ƽʱ����д����*/
sbit EN = P2^2;		/*E��Ϊʹ�ܶˣ���E���ɸߵ�ƽ����ɵ͵�ƽʱ��Һ��ģ��ִ������*/
sbit bee= P2^3;			//�������ܽ�
sbit DE = P2^4;
sbit RE = P2^5;
#endif