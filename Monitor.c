
#include <monitor.h>
#include <reg51.h>
#include <string.h>

/*variables globales */
extern unsigned char Timer_wait;
extern unsigned char USE_LPR;
extern unsigned char  Debug_Tibbo;
extern unsigned char ValTimeOutCom;		
extern unsigned char Tipo_Vehiculo;
unsigned char placa[]={0x30,0x30,0x30,0x30,0x30,0x30,0x0,0x0,0x0};

/*configuracion bits*/

sbit rx_ip = P0^0;	
sbit lock = P1^7;						//Relevo 

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

#define STX											02 
#define ETX											03 

#define EE_USE_LPR							0x000A
/*------------------------------------------------------------------------------
Rutina q recibe  los cmd de Monitor por el tibbo
return el num de caracteres recibidos
y almacena la trama en un apuntador
------------------------------------------------------------------------------*/
unsigned char recibe_cmd_Monitor(unsigned char *buffer_cmd)
{
	unsigned char j, NumDatos,time_out,MaxChrRx;
	unsigned int contador;
	
		NumDatos=0;
		MaxChrRx=11;

	if (USE_LPR==1)
	{
			for (j=0; j<MaxChrRx; j++)
			{
				contador=0;
				time_out=0;
				while ((rx_ip==1)&&(time_out==0))
				{
					contador++;
					if (contador>65000)
					{
						time_out=1;
						j=MaxChrRx;
					}				
				}
				if(time_out==1)break;
					NumDatos++;
	 				*buffer_cmd=rx_Data();
						buffer_cmd++;
			}

			*buffer_cmd=0;
			

	}
	return	NumDatos;
}	
/*------------------------------------------------------------------------------
Rutina q valida los cmd de Monitor
------------------------------------------------------------------------------*/

void Valida_Trama_Monitor(unsigned char *buffer, unsigned char length_trama)
{		
	unsigned char j=0;
	unsigned char p=2;
	length_trama=1;
		if	((*(buffer+2)==ETX)&&(*(buffer+1)=='P')) 																																						/* APERTURA DE BARRETA*/ 
				{
					lock=1;																																																						/*habilita el relevo ON*/
					Timer_wait=0;
	 			}
		else if (*(buffer+1)=='<')
		{
			/*placa*/
			do
			{
				placa[j]=*(buffer+p);
				p++;
				j++;
			}while (*(buffer+p) != ETX);
			*(buffer+p)=0;
				placa[j-1]=0;
			  placa_ready=1;
			ValTimeOutCom=10;
			/*placa no play <NO_PLATE>*/ 
			
		}
		else if (*(buffer+1)=='[')
		{
			/*cancel*/
		}
}
void clear_placa()
{
	unsigned char i;
	for(i=0;i<6;i++)
	{
	 placa[i]=0x30;
	}
	 placa_ready=0;
}
void Rx_Monitor()
{
	unsigned char Length_trama;
	unsigned char buffer1[12];
		Length_trama=recibe_cmd_Monitor(buffer1);
		Debug_monitor(buffer1,Length_trama);
  	Valida_Trama_Monitor(buffer1,Length_trama);
}