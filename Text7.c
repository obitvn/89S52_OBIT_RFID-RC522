#include "send.h"

void main(void)
{
	bit searchcard_f=0,key_f=0,check_f=0,read_f=0,write_f=0;
	bee=0;
	TMOD = 0x20;
	SCON = 0xd0;
	TH1 = 250;
	TL1 = 250;
	TR1 = 1;
	PCON = 0x80;						// SMOD=1
	EA = 0;
	DE=0;		                       //禁止发送，禁止接收
	RE=1;	
	LCD_Init ();   	
	while(!searchcard_f)		    //寻卡
	{
		write_f=0;
		searchcard_f=search();
		Delay_ms(5);
	}
	while(!key_f)				     //发送密码
	{							   
		searchcard_f=0;
		key_f=password();
		Delay_ms(5);
	}
	while(!check_f)				    //密码验证
	{
	    key_f=0;
		check_f=check_password();
		Delay_ms(5);
	}
	while(!read_f)				    //读数据
	{
		check_f=0;
		read_f=read();
		Delay_ms(5);
	}
	DE=1;		                       //允许发送，允许接收
	RE=0;
	Delay_ms(50);
	while(!Transmit_f)              //主从机通信
	{
		Transmit();	
	}
	dataprocess();
	DE=0;		                       //禁止发送，禁止接收
	RE=1;
	Delay_ms(2);
	while(!write_f)			          //写数据
	{
	    read_f=0;		
		write_f=write();
		bee=write_f;
		Delay_ms(255);
		Delay_ms(255);
		bee=0;
	}
	Display();
	Delay_ms(255);
while(1);
}
/********************寻卡过程*********************/ 
bit search()
{
	uchar i;
	TB8=1;
	Send(SEARCHCARD[0]);
	TB8=0;
	Send(SEARCHCARD[1]);
	Send(SEARCHCARD[2]);
	Send(SEARCHCARD[3]);
	TB8=1;
	Send(SEARCHCARD[4]);
	for(i=0;i<9;i++)
	{
		return_frame[i]=Receive();
		if(return_frame[0]!=0x47)
		return 0;
	}
	for(i=0;i<9;i++)
	{
			Send(return_frame[i]);
	}
	if(return_frame[2]!=0x00)
		return 0;
	else
	{
		
		return 1;
	}
	
}
/***********************发送密码******************/ 
bit password()
{
	uint i;
	TB8=1;
	Send(KEY[0]);
	TB8=0;
	Send(KEY[1]);
	Send(KEY[2]);
	Send(KEY[3]);	
	Send(KEY[4]);
	TB8=1;
	Send(KEY[5]);
	Send(KEY[6]);
	Send(KEY[7]);	
	Send(KEY[8]);
	Send(KEY[9]);
	Send(KEY[10]);
	Send(KEY[11]);	
	for(i=0;i<5;i++)
	{
		return_frame[i]=Receive();
		if(return_frame[0]!=0x47)
		return 0;
	}
	if(return_frame[2]!=0x00)
		return 0;
	for(i=0;i<5;i++)
	{
		Send(return_frame[i]);
	}
	return 1;
}

/********************密码验证*********************/ 
bit check_password()
{
	uchar i;
	TB8=1;
	Send(CHECK[0]);
	TB8=0;
	Send(CHECK[1]);
	Send(CHECK[2]);
	Send(CHECK[3]);
	Send(CHECK[4]);
	TB8=1;
	Send(CHECK[5]);

	for(i=0;i<5;i++)
	{
		return_frame[i]=Receive();
		if(return_frame[0]!=0x47)
		return 0;
	}
	if(return_frame[2]!=0x00)
		return 0;
	for(i=0;i<5;i++)
	{
		Send(return_frame[i]);
	}
	return 1;	
}

/********************读卡*********************/ 
bit read()
{
    uchar i;
	TB8=1;
	Send(READDATA[0]);
	TB8=0;
	Send(READDATA[1]);
	Send(READDATA[2]);
	Send(READDATA[3]);
	TB8=1;
	Send(READDATA[4]);

	for(i=0;i<21;i++)
	{
		return_frame[i]=Receive();
		if(return_frame[0]!=0x47)
		return 0;
	}
	if(return_frame[2]!=0x00)
		return 0;
	for(i=0;i<21;i++)
	{
		Send(return_frame[i]);
	}
	DATA[0]=return_frame[4];
	DATA[1]=return_frame[5];
	DATA[2]=return_frame[6];
	return 1;
}

/********************写卡*********************/ 
bit write()
{
	uchar i;
	TB8=1;
	Send(WRITEDATA[0]);
	TB8=0;
	Send(WRITEDATA[1]);
	Send(WRITEDATA[2]);
	Send(WRITEDATA[3]);
	TB8=1;
	Send(WRITEDATA[4]);
	TB8=0;
	Send(WRITEDATA[5]);
	Send(WRITEDATA[6]);
	for(i=7;i<20;i++)
	{
		Send(WRITEDATA[i]);
	}
	TB8=0;
	Send(WRITEDATA[20]);		
	for(i=0;i<5;i++)
	{
		return_frame[i]=Receive();
		if(return_frame[0]!=0x47)
		return 0;
	}
	for(i=0;i<5;i++)
	{
		Send(return_frame[i]);
	}
	if(return_frame[2]!=0x00)
		return 0;
	else
	{
		
		return 1;
	}
}

/***************发送单字节数据********************/ 
void Send(uchar m)
{
	TI = 0;
	DE = 0;
	RE = 0;
	SBUF = m;
    while(!TI);
	TI = 0;
}

/***************接收单字节数据*******************/
uchar Receive()
{
	uchar tmp,j=0,i;
	DE = 1;
	RE = 1;
	RI = 0;
	while ((!RI)&&j<200)
	{
		for(i=0;i<50;i++);
		j++;
	}
	if(RI)
	{
		tmp = SBUF;
		RI=0;
		return tmp;	
	}
	else
	return 0xff;		
} 
	
/**********延时1ms程序*************/
void Delay_ms(uint count)
{
	uchar i,j;
	for(;count>0;count--)
  	{ 
		for(j=2;j>0;j--)
		for(i=0;i<248;i++)
			;
  	}
} 

/**********修改卡数据*************/
void dataprocess()
{
	uchar k;
	if(DATA[2]==0x0A)
	{
		DATA[2]=0x05;	  //修改为还车状态
	}		
	else if(DATA[2]==0x05)						  //如果是还车状态的话，先扣费，再转为取车状态
	{		
		if(DATA[0]!=0x00)
		{
			DATA[0]=DATA[0]-1;
			DATA[2]=0x0A;
		}
		else
			bee=1;	                               //如果余额为0，蜂鸣器长鸣
		
	}		
	for(k=0;k<3;k++)
	{
		WRITEDATA[4+k]=DATA[k];
	}
	if(DATA[2]==0x0A)
	{
		WRITEDATA[20]=0x97^DATA[0]+0x80;
	}
	else if(DATA[2]==0x05)
	{
		WRITEDATA[20]=0x98^DATA[0]+0x80;
	}
}

/**********液晶显示余额及卡状态*************/
void Display()
{
	Account[10]=DATA[0]/100%10+0x30;
	Account[11]=DATA[0]/10%10+0x30;
 	Account[12]=DATA[0]%10+0x30;
    Borrow_bicycle[4]=addr/100%10+0x30;
	Borrow_bicycle[5]=addr/10%10+0x30;
	Borrow_bicycle[6]=addr%10+0x30;
	Return_bicycle[4]=addr/100%10+0x30;
	Return_bicycle[5]=addr/10%10+0x30;
	Return_bicycle[6]=addr%10+0x30;
	if(DATA[2]==0x05)
	{	
		LCD_Wcmd ( 0x01 );
		OutWord(0x88, 8, Account); 
		OutWord(0x90, 8, Borrow_bicycle);
	}
	else if(DATA[2]==0x0a)
	{
		LCD_Wcmd ( 0x01 );
		OutWord(0x88, 8, Account); 
		OutWord(0x90, 8, Return_bicycle);
	}
	Delay_ms(200);
	Delay_ms(200);
}
void LCD_Busy ()				     /*判断lcd是否处于忙状态，是返回1，否返回0*/
{                          
   
	lcd_port = 0xff;
	RS = 0;
    RW = 1;
	EN = 1;
	while (rd);
	EN = 0;
} 

void LCD_Wcmd ( uchar cmd )	     	/*对lcd指令寄存器写显示控制命令字*/
{                          
	LCD_Busy();
	RS = 0;
	RW = 0;
    lcd_port = cmd;					/*写指令到端口上*/ 
	EN = 1;
	_nop_ ();       
    EN = 0;  
}

void LCD_Wdat ( uchar dat )		   	/*对lcd数据寄存器写显示数据*/ 
{                          
	LCD_Busy();
    RS = 1;
    RW = 0;
    lcd_port = dat;				 	/*写数据到端口上*/  
    EN = 1;
	_nop_ ();
    EN = 0; 
}

void LCD_Init ()			     	/*lcd初始化*/
{     
	LCD_Wcmd ( 0x30 );      	 	/*功能设置*/     
	LCD_Wcmd ( 0x01 );			 	/*清屏*/ 
    LCD_Wcmd ( 0x0c );      	 	/*显示开，光标关，光标闪烁关*/   
	OutWord(0x80, 8, Wellcome);
	OutWord(0x88, 8, Unit);
	OutWord(0x90, 8, Design);
	OutWord(0x98, 8, Time);
	Delay_ms(200);
	Delay_ms(200);
}

void OutWord( uchar place, uchar unit, uchar charcode[] )				/*输出中文或字符*/
{
 	uchar i;
 	LCD_Wcmd ( place );
 	for ( i=0; i < unit*2; i++)
 		LCD_Wdat ( charcode[i] );
}

void Transmit()
{
	uchar tmp,i;
	P1= 0xFF;
	addr=P1;
	tmp = addr-1;
	i=0;
	while(tmp!=addr)			
	{
		uint j=0;
		i++;
		DE = 1;
		RE = 1;
		/* 发送从机地址 */
		TI = 0;
		TB8 = 1;				// 发送地址帧	
		SBUF = addr;
		while(!TI);
		TI = 0;
		DE = 0;
		RE = 0;
		RI=0;
		/* 接收从机应答 */
		while ((!RI)&&j<1000)
		{
			j++;
		}
		if(RI)
		{
			tmp = SBUF;
			RI=0;
		}
		else if(i==10)
		{
			tmp=addr;
			i=0;	
		}
	}

	/* 发送数据并接收校验信息，如果接收到SUCC，表示从机接收成功，否则将重新发送该组数据 */	
	tmp = ERR;
	i=0;
  	while(tmp!=SUCC)
    {
		uint j=0;
		i++;
		DE = 1;
		RE = 1;
		TI = 0;
		TB8 = 0;				
		SBUF =DATA[2];
		while(!TI);
		TI = 0;			// 发送数据	
		DE = 0;
		RE = 0;
		RI = 0;
		while ((!RI)&&j<1000)
		{
			j++;
		}
		if(RI)
		{
			tmp	= SBUF;
			if(tmp==SUCC)
			{	
				Transmit_f=1;	
			}
			else if(tmp==ERR)
			{
				tmp=SUCC;
				LCD_Wcmd(0x01);
				OutWord(0x88, 8, Transmit_error);
			//	Delay_ms(1000);
			}
			RI = 0;
		}
		else if(i==10)
		{
			tmp=SUCC;
			i=0;
			LCD_Wcmd(0x01);
			OutWord(0x88, 8, Transmit_error);
	//		Delay_ms(1000);
		}	
		
	}	
}