
#include <monitor.h>
#include <reg51.h>
#include <string.h>

/*variables globales */
extern unsigned char Timer_wait;
extern unsigned char USE_LPR;
extern unsigned char  Debug_Tibbo;
extern unsigned char ValTimeOutCom;		
extern unsigned char Tipo_Vehiculo;
idata unsigned char placa[]={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

/*configuracion bits*/

sbit rx_ip = P0^0;	
sbit lock = P1^7;						//Relevo 
sbit led_err_imp = P0^2;			//Error 						
/*externos bits*/

extern bit placa_ready;

/*funciones prototipo*/
extern unsigned char rx_Data(void);
extern unsigned char Dir_board();
extern void serie_ascii_siceros_l(unsigned char *serie);
extern void Block_read_clock_ascii(unsigned char *datos_clock);
extern void DebugBufferMF(unsigned char *str,unsigned char num_char,char io);
extern void tx_aux(unsigned char caracter);
extern unsigned char hex_bcd (unsigned char byte);
extern void serie_ascii_siceros_l(unsigned char *serie);
extern unsigned char rd_eeprom (unsigned char control,unsigned int Dir) ;
extern void Debug_monitor(unsigned char *buffer, unsigned char Length_trama );
extern void Debug_txt_Tibbo(unsigned char * str);
extern void Delay_10ms(unsigned int cntd_10ms);
extern void tx_aux(unsigned char caracter);
extern void Debug_Buffer_rs232_lcd(unsigned char *str,unsigned char num_char);

#define True										0x01
#define False										0x00

#define STX											02 
#define ETX											03 

#define EE_USE_LPR							0x000A
/*------------------------------------------------------------------------------
el tiempo de espera por caracter recibido 
rx_ip --> pin de recepcion de datos 
time_out ----> bit de tiempo vencido
contador ----> tiempo de espera del caracter
------------------------------------------------------------------------------*/
unsigned Timer_monitor_char()
{
unsigned char time_out;
unsigned int contador;		
				contador=0;
				time_out=0;
				while ((rx_ip==1)&&(time_out==0))
				{
					contador++;
					if (contador>60000)
					{
						time_out=1;
					}				
				}
				if(time_out==1)
				{ 
					return False;
					
				}
				else
				{
				return True;
				}
		
}
/*------------------------------------------------------------------------------
Rutina q recibe  los cmd de Monitor por el tibbo
return el num de caracteres recibidos
y almacena la trama en un apuntador
la trama de placa inicia 0x02<xxxxxx>0x03
02 41 03 cuando no hay placa
06 ask el aceptado
------------------------------------------------------------------------------*/

unsigned char recibe_cmd_Monitor(unsigned char *buffer_cmd)
{
	unsigned char j, NumDatos,MaxChrRx,temp;
	
	
		NumDatos=0;
		MaxChrRx=12;
		placa[0]=0x0;

/*timer que espera el dato de monitor*/
				if (Timer_monitor_char() == False)
				{
					NumDatos= 0;
				}
				else
				{
				*buffer_cmd=rx_Data();
				
					/*valido el inicio del cmd*/
					if (*buffer_cmd == 0x02 )
					{
						NumDatos++;
						buffer_cmd++;
						
						for (j=1; j<MaxChrRx; j++)
						{
							if (Timer_monitor_char() == False)
							{
								break;
							}
							else 
							{
								temp=rx_Data();
								if (temp==ETX)
								{
									*buffer_cmd=temp;
									j=MaxChrRx;
									NumDatos++;
									buffer_cmd++;
									buffer_cmd=0;
									
								}
								else if (temp == "A")
								{
									if (Timer_monitor_char() == False)
									{
										break;
									}
									else 
									{
										temp=rx_Data();
										if (temp==ETX)
										{
												j=MaxChrRx;
											NumDatos=11;
											*(buffer_cmd+NumDatos)=0;
										}
									}
								}
								else
								{
									*buffer_cmd=temp;
									NumDatos++;
									buffer_cmd++;
								}
								
							}
						}
					}
					else if (*buffer_cmd == 0x06 )
					{
						/*llego el ask*/
						*buffer_cmd=0;
					}
					else
					{
						/*no es 0x02 inicio de trama */
						NumDatos= 0;
						
					}
				
				}
				return NumDatos;
}

/*------------------------------------------------------------------------------
Rutina q valida los cmd de Monitor
------------------------------------------------------------------------------*/

void Valida_Trama_Monitor(unsigned char *buffer, unsigned char length_trama)
{		
	unsigned char j=0;
	unsigned char p=2;
	unsigned char cont=0;
	length_trama=1;
		
			/*habilita relevo abre barrera*/
		if	((*(buffer+1)=='P')) 																																						/* APERTURA DE BARRETA*/ 
				{
				
					lock=1;		
					
					Delay_10ms(70);					/*habilita el relevo ON*/
					tx_aux(06);							//ack		
				
	 			}
			/*se recive la placa O EL CANCEL Y NO_PLATE*/	
		else if ((*(buffer+1)=='<')|| (*(buffer+1)=='['))
		{
			/*placa 0 el cancel borra la fecha del mensual */
			do
			{
				placa[j]=*(buffer+p);
				p++;
				j++;
			}while ((*(buffer+p) != ETX) && (*(buffer+p) != 0) );
			*(buffer+p)=0;
				placa[j-1]=0;
			  placa_ready=1;
			ValTimeOutCom=10;
			led_err_imp = 0;
			
		}
		
	
}
void clear_placa()
{
	unsigned char i;
	for(i=0;i<9;i++)
	{
	 placa[i]=0x0;
	}
	 placa_ready=0;
	led_err_imp = 1;
}
void Rx_Monitor()
{
	unsigned char Length_trama;
	unsigned char buffer1[13];
		buffer1[0]=0;
		Length_trama=recibe_cmd_Monitor(buffer1);
		Debug_Buffer_rs232_lcd(buffer1,Length_trama);
		Debug_monitor(buffer1,Length_trama);
  	Valida_Trama_Monitor(buffer1,Length_trama);
}