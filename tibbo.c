#include <reg51.h>
#include "tibbo.h" 

/*funciones externas*/
extern void Delay_20us(unsigned int cnt);
extern void          _nop_     (void);
extern unsigned char hex_bcd (unsigned char byte);
extern char putchar (char c);
/*variable externas*/
extern unsigned char Debug_Tibbo;

sbit rx_ip = P0^0;					//		
sbit txd2 = P1^0;					//Transmision Aux Datos	IP				
sbit sel_com = P0^7;	//*

#define True										0x01
#define False										0x00
#define SIN_MSJ									0x02
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void time_bit()
{
	unsigned char j;
	for (j=0; j<=15; j++) 				//18 para 19200  ...   41 Para 9600
	{
	}

//	for (j=0; j<=7; j++) 				//18 para 19200  ...   41 Para 9600	 //42 142us //7 a 9600 transmision
//	{
//		_nop_();
//		_nop_();
//		_nop_();
//		_nop_();
//		_nop_();
//		_nop_();
//		_nop_();
//		_nop_();
//		_nop_();
		
//	}
//		_nop_();
//  	_nop_();
//	  _nop_();
//		_nop_();
//	  _nop_();		//91
//		_nop_();
//		_nop_(); //92
//	 _nop_();
//	 _nop_();			//93
//	 _nop_();			
//	 _nop_();			//94
}
/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void time_mbit(void)
{
	unsigned char j;
	//unsigned char j;

	for (j=0; j<=5; j++) 				//18 para 19200  ...   41 Para 9600
	{
//	for (j=0; j<=4; j++)
	//{
	//	_nop_();
	//	_nop_();
	//	_nop_();
	//	_nop_();
	//	_nop_();
	//	_nop_();
	//	_nop_();
	}

}

/*------------------------------------------------------------------------------
Transmite un caracter  por tibbo a 9600 bd
------------------------------------------------------------------------------*/
void tx_aux(unsigned char caracter)
{
	unsigned char j, temporal, bitTX;

	EA=False;
	txd2=False;
	time_bit();
		_nop_();
		_nop_();
  	temporal=caracter;
	bitTX=caracter&0x01;
	if (bitTX==0x00)
	{
	 	txd2=False;
	}
	else
	{
	   	txd2=True;
	}
	time_bit();
	for (j=1; j<=7; j++)
	{
		temporal>>=1;
		bitTX=temporal&(0x01);
		if (bitTX==0x00)
		{
	 		txd2=False;
		}
		else
		{
	   		txd2=True;
		}
		time_bit();
	}
	txd2=True;
	time_bit();
	time_bit(); 

	EA=True;
}


void Debug_HexDec(unsigned char xfc)
{
	unsigned int valor;
	unsigned char centena, decena, unidad;
	valor=0;

	centena=0;
	decena=0;
	unidad=0;
	if (Debug_Tibbo==1)
	{	 
	 	while (xfc>=0x064) 				// resto 100
		{
			xfc=xfc-0x64;
			centena=centena+1;
		}
		while (xfc>=0x0a)				// resto 10
		{
			xfc=xfc-0x0a;
			decena=decena+1;
		}
		unidad=xfc;
	    tx_aux(centena|0x30);
		tx_aux(decena|0x30);
	   	tx_aux(unidad|0x30);
		
	}
}

/*------------------------------------------------------------------------------
Transmito un caracter pasandolo a ascii 
------------------------------------------------------------------------------*/
void Debug_chr_Tibbo(unsigned char Dat)
{
	unsigned char temp;
	if (Debug_Tibbo==True)
	{
		temp=(Dat&0xf0)>>4;
		(temp>0x09)?(temp=temp+0x37):(temp=temp+0x30);
			
		tx_aux(temp);
							 
		temp=(Dat&0x0f);
		(temp>0x09)?(temp=temp+0x37):(temp=temp+0x30);
		tx_aux(temp);
		tx_aux(' ');
	
	}
}
void Debug_chr_rs232_lcd(unsigned char Dat)
{
	unsigned char temp,d;
	temp=(Dat&0xf0)>>4;
		(temp>0x09)?(temp=temp+0x37):(temp=temp+0x30);
				d=putchar(temp);
	temp=(Dat&0x0f);
		(temp>0x09)?(temp=temp+0x37):(temp=temp+0x30);
	d=putchar(temp);
	d=putchar(' ');
}
/*------------------------------------------------------------------------------
Transmito un Buffer x y lo pasa a ascii 
io=0 datos enviados
io=1 datos recibidos
io=2 no hay texto
------------------------------------------------------------------------------*/
void DebugBufferMF(unsigned char *str,unsigned char num_char,char io)
{
  unsigned char j;
 
  
  if (Debug_Tibbo == True)
  {
		if(io == True)
		{
  	Debug_txt_Tibbo((unsigned char *) "Datos Recibidos del Transporte: ");
		}
		else if  (io == False)
		{
			Debug_txt_Tibbo((unsigned char *) "Datos Enviados al Transporte: ");
		}
		
		for (j=0; j<num_char; j++)
		{
		Debug_chr_Tibbo(*str);
		str++;
		}
		tx_aux('\r');
		tx_aux('\n');
  }

}
void Debug_Buffer_rs232_lcd(unsigned char *str,unsigned char num_char)
{
	unsigned char j,d;
	sel_com=0;
	for (j=0; j<num_char; j++)
		{
		Debug_chr_rs232_lcd(*str);
		str++;
		}
		d=putchar('\n');
		
  
	sel_com=1;
}
/*------------------------------------------------------------------------------
imprime la trama hasta el caracter null
------------------------------------------------------------------------------*/
void Debug_txt_Tibbo(unsigned char * str)
{
	unsigned char i;
	i=0;
	
	if (Debug_Tibbo==True)
	{
		for (i=0; str[i] != '\0'; i++)
		{
 	  		tx_aux(str[i]);
		}
		
	}
}
void Debug_txt_rs232(unsigned char * str)
{
	unsigned char i,d;
	i=0;
	
	
		for (i=0; str[i] != '\0'; i++)
		{
 	  		d=putchar(str[i]);
		}
		
	
}
void Debug_Dividir_texto()
{
	Debug_txt_Tibbo((unsigned char *) "/*---------------------------------------*/\n\r");
}

/*------------------------------------------------------------------------------
Recibe la trama del tibbo a 9600bd
------------------------------------------------------------------------------*/
unsigned char rx_Data(void)
{
	unsigned char temporal;
		
		temporal=0xff;
		time_mbit();
//--------------------------------------------------------------------------------------------------
		time_bit();
//--------------------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
//------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
//------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
//------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
//------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
//------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
//------------------------------------------------------------------------------------
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
		temporal>>=1;
//------------------------------------------------------------------------------------
		time_bit();
		temporal=(rx_ip==0)?(temporal&0x7f):(temporal|0x80);
//------------------------------------------------------------------------------------
		time_bit();
	  	while (rx_ip==0)
		{
		}
		
//------------------------------------------------------------------------------------
 	return temporal; 

}	
//void Debug_Fecha_actual(unsigned char *buffer)
//{
//	Debug_txt_Tibbo((unsigned char *) "Fecha Actual en Board: ");
//			Debug_chr_Tibbo(hex_bcd(*buffer));														/*a�o*/
//			tx_aux('/');
//			Debug_chr_Tibbo(hex_bcd(*(buffer+1)));												/*mes*/
//			tx_aux('/');
//			Debug_chr_Tibbo(hex_bcd(*(buffer+2)));												/*dia*/
//			tx_aux(' ');
//			Debug_chr_Tibbo(hex_bcd(*(buffer+3)));												/*hora*/
//			tx_aux(':');
//			Debug_chr_Tibbo(hex_bcd(*(buffer+4)));												/*minutos*/
//			Debug_txt_Tibbo((unsigned char *) "\r\n\r\n");
//}
/*------------------------------------------------------------------------------
Condiciones iniciales de los pines
------------------------------------------------------------------------------*/
void cond_ini_tibbo(void)
{

	txd2=True;
	rx_ip=True;
}
void Debug_pto_paralelo(unsigned char *buffer, unsigned char Length_trama )
{
	
	Debug_txt_Tibbo((unsigned char *) "Recibe trama pto paral = ");					/*trama recibida pto paralelo */
	DebugBufferMF(buffer,Length_trama,SIN_MSJ);																/*imprimo la trama recibida*/
	Debug_txt_Tibbo((unsigned char *) "longitud de la trama: ");		/*msj longitud de la trama */
	Debug_chr_Tibbo(Length_trama);																			/*numero de caracteres recibidos*/
	Debug_txt_Tibbo((unsigned char *) "\r\n");
	Debug_Dividir_texto();																							/*divido el texto*/
			
}	
void Debug_monitor(unsigned char *buffer, unsigned char Length_trama )
{
	Debug_Dividir_texto();																							/*se divide el texto */			
	Debug_txt_Tibbo((unsigned char *) "Recibe trama Monitor= ");				
	Debug_txt_Tibbo(buffer);
	Debug_txt_Tibbo((unsigned char *) "\r\n");
	Debug_txt_Tibbo((unsigned char *) "longitud de la trama: ");		/*msj longitud de la trama */
	Debug_chr_Tibbo(Length_trama);																			/*numero de caracteres recibidos*/
	Debug_txt_Tibbo((unsigned char *) "\r\n");				
	Debug_Dividir_texto();	
}	
