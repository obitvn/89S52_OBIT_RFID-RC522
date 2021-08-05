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
	DE=0;		                       //��ֹ���ͣ���ֹ����
	RE=1;	
	LCD_Init ();   	
	while(!searchcard_f)		    //Ѱ��
	{
		write_f=0;
		searchcard_f=search();
		Delay_ms(5);
	}
	while(!key_f)				     //��������
	{							   
		searchcard_f=0;
		key_f=password();
		Delay_ms(5);
	}
	while(!check_f)				    //������֤
	{
	    key_f=0;
		check_f=check_password();
		Delay_ms(5);
	}
	while(!read_f)				    //������
	{
		check_f=0;
		read_f=read();
		Delay_ms(5);
	}
	DE=1;		                       //�����ͣ��������
	RE=0;
	Delay_ms(50);
	while(!Transmit_f)              //���ӻ�ͨ��
	{
		Transmit();	
	}
	dataprocess();
	DE=0;		                       //��ֹ���ͣ���ֹ����
	RE=1;
	Delay_ms(2);
	while(!write_f)			          //д����
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
/********************Ѱ������*********************/ 
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
/***********************��������******************/ 
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

/********************������֤*********************/ 
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

/********************����*********************/ 
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

/********************д��*********************/ 
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

/***************���͵��ֽ�����********************/ 
void Send(uchar m)
{
	TI = 0;
	DE = 0;
	RE = 0;
	SBUF = m;
    while(!TI);
	TI = 0;
}

/***************���յ��ֽ�����*******************/
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
	
/**********��ʱ1ms����*************/
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

/**********�޸Ŀ�����*************/
void dataprocess()
{
	uchar k;
	if(DATA[2]==0x0A)
	{
		DATA[2]=0x05;	  //�޸�Ϊ����״̬
	}		
	else if(DATA[2]==0x05)						  //����ǻ���״̬�Ļ����ȿ۷ѣ���תΪȡ��״̬
	{		
		if(DATA[0]!=0x00)
		{
			DATA[0]=DATA[0]-1;
			DATA[2]=0x0A;
		}
		else
			bee=1;	                               //������Ϊ0������������
		
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

/**********Һ����ʾ����״̬*************/
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
void LCD_Busy ()				     /*�ж�lcd�Ƿ���æ״̬���Ƿ���1���񷵻�0*/
{                          
   
	lcd_port = 0xff;
	RS = 0;
    RW = 1;
	EN = 1;
	while (rd);
	EN = 0;
} 

void LCD_Wcmd ( uchar cmd )	     	/*��lcdָ��Ĵ���д��ʾ����������*/
{                          
	LCD_Busy();
	RS = 0;
	RW = 0;
    lcd_port = cmd;					/*дָ��˿���*/ 
	EN = 1;
	_nop_ ();       
    EN = 0;  
}

void LCD_Wdat ( uchar dat )		   	/*��lcd���ݼĴ���д��ʾ����*/ 
{                          
	LCD_Busy();
    RS = 1;
    RW = 0;
    lcd_port = dat;				 	/*д���ݵ��˿���*/  
    EN = 1;
	_nop_ ();
    EN = 0; 
}

void LCD_Init ()			     	/*lcd��ʼ��*/
{     
	LCD_Wcmd ( 0x30 );      	 	/*��������*/     
	LCD_Wcmd ( 0x01 );			 	/*����*/ 
    LCD_Wcmd ( 0x0c );      	 	/*��ʾ�������أ������˸��*/   
	OutWord(0x80, 8, Wellcome);
	OutWord(0x88, 8, Unit);
	OutWord(0x90, 8, Design);
	OutWord(0x98, 8, Time);
	Delay_ms(200);
	Delay_ms(200);
}

void OutWord( uchar place, uchar unit, uchar charcode[] )				/*������Ļ��ַ�*/
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
		/* ���ʹӻ���ַ */
		TI = 0;
		TB8 = 1;				// ���͵�ַ֡	
		SBUF = addr;
		while(!TI);
		TI = 0;
		DE = 0;
		RE = 0;
		RI=0;
		/* ���մӻ�Ӧ�� */
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

	/* �������ݲ�����У����Ϣ��������յ�SUCC����ʾ�ӻ����ճɹ����������·��͸������� */	
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
		TI = 0;			// ��������	
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