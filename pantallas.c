#include<pantallas.h>
#include <reg51.h>
#include <string.h>
#include <stdlib.h>

sbit sel_com = P0^7;				//Micro switch		


/*funciones externas*/
extern char putchar (char c);
extern void Block_read_Clock(unsigned char *datos_clock);
extern void DebugBufferMF(unsigned char *str,unsigned char num_char,char io);
extern void Debug_Dividir_texto();
extern void clean_tx();
extern void Debug_txt_Tibbo(unsigned char * str);
extern void          _nop_     (void);
extern void Debug_chr_Tibbo(unsigned char Dat);
extern unsigned char *Lee_No_Ticket();
extern void EscribirCadenaSoft_buffer(unsigned char *buffer,unsigned char tamano_cadena);
extern int printf   (const char *, ...);
extern void clean_tx();
extern char com_putchar (unsigned char c);
extern void Block_read_clock_ascii_rasberry(unsigned char *datos_clock);
void Raspberry_data (unsigned char *msjpantalla);
/*mensajes de salida desde 85 a 169*/

#define INGRESE									85					//0xDF //E3,E4,E5
#define SIN_INGRESO							86					//0XE6
#define SIN_PAGO								87						//0XE7
#define EXCEDE_GRACIA						88					//0XE8
#define MENSUAL_NO_PARK					89
#define DIRIJASE_CAJA						90
#define GRACIAS									91						//0XFF,01
/*mensajes mensual llega de lectura de wiegand de principal*/
#define LECTURA_WIEGAND					92					//0xB0
#define	NO_IN_PARK							93						//B2
#define EXPIRO									94						//B4
#define	EXCEDE_HORARIO					95
#define NO_MENSUAL_NI_PREPAGO		96
#define MENSUAL_NO_PAGO					97
#define HORARIO									98
#define IN_PARK									99
#define IN_HORARIO							0x9A
#define BIENVENIDO_WIEGAN				0x9b
#define LOTE_FULL								0x9c
#define	CUPOS										0x9d
#define NOREAD_CARD							0x9e
/*mensajes informativos*/



#define ERROR_LOOP							170					//0XE0
#define ERROR_COD_PARK					171					//0XE5
#define PULSE_BOTON							172
#define TARJETA_SIN_FORMATO	    173					//0xDF
#define OFF_LINE								174					//B6
#define UN_MOMENTO							175
#define NO_CARD_MENSUAL					176

#define TARJETA_INVALIDA				177
#define SIN_SALIDA							178
#define REGISTRA_INGRESO				179
#define TARJETA_VENCIDA					180
#define MENSUAL_FUERA_HORARIO		181
#define HORARIO_NO_PROG					182

#define NO_CARD									0XFA
#define BIENVENIDO							0XFE

#define LOW_CARD								0x01


#define RETIRE_TARJETA					0XA1

#define NOTIFIQUE_EVP						'N'
#define NO_NOTIFIQUE_EVP				'S'
#define COMPARANDO_PLACA				'P'						/*msj de eeror de placa*/
#define ENVIANDO_COD						'D'
#define INFO1										'I'
#define INFO2										'i'

#define True										0x01
#define False										0x00

/*variables externas */
extern unsigned char 	Raspberry;
extern idata unsigned char placa[];
extern bit placa_ready;
extern bit MenSual ;

//unsigned char xdata malloc_memoria[0x50];

/*------------------------------------------------------------------------------
Rutina de msj de pantalla
------------------------------------------------------------------------------*/
void PantallaLCD(unsigned char cod_msg)
{

unsigned char Ini_LCD_Line_one   []={0xaa,0x80,0x18,0x01,0x02,0x00} ;
//unsigned char Ini_LCD_Line_two   []={0xaa,0x80,0x18,0x02,0x02,0x00} ;
//unsigned char Ini_Off_Line []={0xaa,0x80,0x18,0x01,0x03,0x00} ;
	
unsigned char num_chr;
unsigned char xdata  *msjpantalla = 0;

	//init_mempool(malloc_memoria,40);
	//msjpantalla=malloc(40);
		sel_com=0;
	
		if (Raspberry==0)
		{
			LCD_txt (Ini_LCD_Line_one,0);
			
			switch (cod_msg)
			{
			/*mensajes de entrada lcd pto serie*/
					case INGRESE:
							num_chr=strlen((unsigned char  *) "INGRESE TARJETA ");										/*cmd 31 es en proceso de ejecucion del firtware*/
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"INGRESE TARJETA ",1);
							break;
				
				case SIN_INGRESO:
               
							num_chr=strlen((unsigned char  *) "TARJETA SIN INGRESO ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"TARJETA SIN INGRESO ",1);
               break;
						
				case SIN_PAGO:
               
							num_chr=strlen((unsigned char  *) "TARJETA NO REGISTRA PAGO ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"TARJETA NO REGISTRA PAGO ",1);
              break;
				case EXCEDE_GRACIA:
             	num_chr=strlen((unsigned char  *) "EXCEDE TIEMPO DE GRACIA ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"EXCEDE TIEMPO DE GRACIA ",1);
              break;	
				case MENSUAL_NO_PARK:
							num_chr=strlen((unsigned char  *) "MENSUAL NO ESTA EN PARQUEADERO ");										/*cmd 31 es en proceso de ejecucion del firtware*/
              tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"MENSUAL NO ESTA EN PARQUEADERO ",1);   
							break;
				case DIRIJASE_CAJA:
							num_chr=strlen((unsigned char  *) "DIRIJASE A CAJA ");										/*cmd 31 es en proceso de ejecucion del firtware*/
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"DIRIJASE A CAJA ",1); 
							break;
				case GRACIAS:
                 
							num_chr=strlen((unsigned char  *) "GRACIAS... ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"GRACIAS...",1); 
              break;
				case NO_IN_PARK:
							num_chr=strlen((unsigned char  *) "MENSUAL NO ESTA EN PARQUEADERO ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"MENSUAL NO ESTA EN PARQUEADERO ",1); 
              break;
				case EXPIRO:
							num_chr=strlen((unsigned char  *) "MENSUALIDAD VENCIDA ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"MENSUALIDAD VENCIDA ",1); 
              break;
				 case EXCEDE_HORARIO:
							num_chr=strlen((unsigned char  *) "MENSUALIDAD EXCEDE HORARIO ACERQUESE A CAJA ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"MENSUALIDAD EXCEDE HORARIO ACERQUESE A CAJA ",1);						
				      break;
					 				 
				 case NO_MENSUAL_NI_PREPAGO:
							num_chr=strlen((unsigned char  *) "NO ES MENSUALIDAD NI PREPAGO");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"NO ES MENSUALIDAD NI PREPAGO",1);						
				      break;
									 
					case MENSUAL_NO_PAGO:
							num_chr=strlen((unsigned char  *) "MENSUAL NO PAGO LOCACION HOY");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"MENSUAL NO PAGO LOCACION HOY",1);						
				      break;
								
				
				/*mensajes informativos por lcd pto serie*/
				
					 case ERROR_LOOP:
               
							num_chr=strlen((unsigned char  *) "SIN PRESENCIA VEHICULAR ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"SIN PRESENCIA VEHICULAR ",1); 
              break;
					
					 case ERROR_COD_PARK:
                 
							num_chr=strlen((unsigned char  *) "TARJETA NO ES DEL PARQUEADERO ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"TARJETA NO ES DEL PARQUEADERO ",1); 
              break;	
					 
					 case PULSE_BOTON:
              num_chr=strlen((unsigned char *) "BIENVENIDO. PULSE EL BOTON ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt((unsigned char *)        "BIENVENIDO. PULSE EL BOTON ",1);
							break;
					 
					  case TARJETA_SIN_FORMATO:
                
							num_chr=strlen((unsigned char  *) "TARJETA SIN FORMATO ");
						  tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"TARJETA SIN FORMATO ",1); 
              break;
						
						 case 	OFF_LINE:
					
							num_chr=strlen((unsigned char *) "FUERA DE LINEA ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt((unsigned char *)        "FUERA DE LINEA ",1);
				  		break;
						 
					 case UN_MOMENTO:
              num_chr=strlen((unsigned char *) "UN MOMENTO POR FAVOR ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt((unsigned char *)        "UN MOMENTO POR FAVOR ",1);
							break;
					 
					 case NO_CARD_MENSUAL:
              num_chr=strlen((unsigned char *) "NO ES TARJETA MENSUAL ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt((unsigned char *)        "NO ES TARJETA MENSUAL ",1);
							break;
						 
					 case TARJETA_INVALIDA:
               
							num_chr=strlen((unsigned char  *) "TARJETA INVALIDA ");
              tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt ((unsigned char *)"TARJETA INVALIDA ",1); 
              break;
						
					 case SIN_SALIDA:
              num_chr=strlen((unsigned char *) "TARJETA SIN SALIDA ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt((unsigned char *)        "TARJETA SIN SALIDA ",1);
							break;
					case REGISTRA_INGRESO:
              num_chr=strlen((unsigned char *) "REGISTRA INGRESO EN PARQUEADERO ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt((unsigned char *)        "REGISTRA INGRESO EN PARQUEADERO ",1);
							break;
					case MENSUAL_FUERA_HORARIO:
              num_chr=strlen((unsigned char *) "MENSUAL FUERA DE HORARIO ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt((unsigned char *)        "MENSUAL FUERA DE HORARIO ",1);
							break;
					case HORARIO_NO_PROG:
              num_chr=strlen((unsigned char *) "HORARIO NO PROGRAMADO ");
							tx_chrlcd(num_chr*2,0);
							LCD_txt((unsigned char *)        "HORARIO NO PROGRAMADO ",1);
							break;	
					
					
						
						case NO_CARD:
              num_chr=strlen((unsigned char *) "SIN TARJETAS ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt((unsigned char *)        "SIN TARJETAS ",1);
							break;
						case BIENVENIDO:
              num_chr=strlen((unsigned char *) "BIENVENIDO ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt((unsigned char *)        "BIENVENIDO ",1);
							break;
							case LOW_CARD:
              num_chr=strlen((unsigned char *) "NIVEL BAJO DE TARJETAS ");
							tx_chrlcd(0x00,0);
							tx_chrlcd(num_chr*2,0);
							LCD_txt((unsigned char *)        "NIVEL BAJO DE TARJETAS ",1);
							break;
						
						
							
							
							
							
								
						
						default:
			
						break;	
			}
				sel_com=1;	
	
		}
      else
      {
         sel_com=0;   
         switch (cod_msg)
         {
							/*msj de entrada*/
					 
						case INGRESE:
							strcpy(msjpantalla,"a;85;INGRESE TARJETA\n\0");
								Raspberry_data(msjpantalla);
								//	Raspberry_data((unsigned char  *) "a;85;INGRESE TARJETA\n\r\0");										/*cmd 31 es en proceso de ejecucion del firtware*/
                  break;
					 	case SIN_INGRESO:
               
									//Raspberry_data((unsigned char  *) "a;86;TARJETA SIN INGRESO \n\r\0");
								strcpy(msjpantalla,"a;86;TARJETA SIN INGRESO\n\0");
								Raspberry_data(msjpantalla);
                  break;
						
						case SIN_PAGO:
							strcpy(msjpantalla,"a;87;TARJETA NO REGISTRA PAGO\n\0");
									Raspberry_data (msjpantalla);
									//Raspberry_data((unsigned char  *) "a;87;TARJETA NO REGISTRA PAGO\n\r\0");
                  break;
						case EXCEDE_GRACIA:
							strcpy(msjpantalla,"a;88;EXCEDE TIEMPO DE GRACIA\n\0");
									Raspberry_data (msjpantalla);
                //	Raspberry_data((unsigned char  *) "a;88;EXCEDE TIEMPO DE GRACIA \n\r\0");
                  break;	
						case MENSUAL_NO_PARK:
							strcpy(msjpantalla,"a;89;MENSUAL NO ESTA EN PARQUEADERO\n\0");
									Raspberry_data (msjpantalla);
									//Raspberry_data((unsigned char  *) "a;89;MENSUAL NO ESTA EN PARQUEADERO\n\r\0");										/*cmd 31 es en proceso de ejecucion del firtware*/
                  break;
						case DIRIJASE_CAJA:
							strcpy(msjpantalla,"a;90;DIRIJASE A CAJA\n\0");
									Raspberry_data (msjpantalla);
									//Raspberry_data((unsigned char  *) "a;90;DIRIJASE A CAJA\n\r\0");										/*cmd 31 es en proceso de ejecucion del firtware*/
                  break;
					  case GRACIAS:
							strcpy(msjpantalla,"a;91;GRACIAS... \n\0");
									Raspberry_data (msjpantalla);
									//Raspberry_data((unsigned char  *) "a;91;GRACIAS... \n\r\0");
                  break;
            case NO_IN_PARK:
							strcpy(msjpantalla,"a;93;MENSUAL NO ESTA EN PARQUEADERO\n\0");
									Raspberry_data (msjpantalla);
									//Raspberry_data((unsigned char  *) "a;93;MENSUAL NO ESTA EN PARQUEADERO\n\r\0");
                  break;
						case IN_PARK:
									strcpy(msjpantalla,"a;93;MENSUAL ESTA EN PARQUEADERO\n\0");
									Raspberry_data (msjpantalla);
									//Raspberry_data((unsigned char  *) "a;93;MENSUAL NO ESTA EN PARQUEADERO\n\r\0");
                  break;
					 case EXPIRO:
						 strcpy(msjpantalla,"a;94;MENSUALIDAD VENCIDA\n\0");
									Raspberry_data(msjpantalla);
									//Raspberry_data((unsigned char  *) "a;94;MENSUALIDAD VENCIDA\n\r\0");
                  break;
					 			 
					 case EXCEDE_HORARIO:
						 strcpy(msjpantalla,"a;95;MENSUALIDAD EXCEDE HORARIO ACERQUESE A CAJA\n\0");
									Raspberry_data(msjpantalla);
								//Raspberry_data((unsigned char  *) "a;95;MENSUALIDAD EXCEDE HORARIO ACERQUESE A CAJA\n\r\0");
                  break;
					 				 
					 case NO_MENSUAL_NI_PREPAGO:
						 strcpy(msjpantalla,"a;96;NO ES MENSUALIDAD NI PREPAGO\n\0");
									Raspberry_data(msjpantalla);
									//Raspberry_data((unsigned char  *) "a;96;NO ES MENSUALIDAD NI PREPAGO\n\r\0");
                  break;
					 
					  case MENSUAL_NO_PAGO:
							strcpy(msjpantalla,"a;97;MENSUAL NO PAGO LOCACION HOY\n\0");
									Raspberry_data(msjpantalla);
									//Raspberry_data((unsigned char  *) "a;97;MENSUAL NO PAGO LOCACION HOY\n\r\0");
                  break;
					
						/*msj informativos */
						
						case ERROR_LOOP:
							strcpy(msjpantalla,"a;98;SIN PRESENCIA VEHICULAR\n\0");
									Raspberry_data (msjpantalla);
									//Raspberry_data((unsigned char  *) "a;98;SIN PRESENCIA VEHICULAR \n\r\0");
                  break;
						
					 case ERROR_COD_PARK:
						 strcpy(msjpantalla,"a;98;TARJETA NO ES DEL PARQUEADERO\n\0");
									Raspberry_data (msjpantalla);
								//Raspberry_data((unsigned char  *) "a;98;TARJETA NO ES DEL PARQUEADERO \n\r\0");
                  break;	
					 
					 case PULSE_BOTON:
						strcpy(msjpantalla,"a;00;BIENVENIDO. PULSE EL BOTON\n\0");
						Raspberry_data(msjpantalla);
					
					 
						break;
					  case TARJETA_SIN_FORMATO:
							strcpy(msjpantalla,"a;98;TARJETA SIN FORMATO\n\0");
									Raspberry_data (msjpantalla);
									//Raspberry_data((unsigned char  *) "a;98;TARJETA SIN FORMATO \n\r\0");
                  break;
						case OFF_LINE:
							strcpy(msjpantalla,"a;98;FUERA DE LINEA\n\0");
									Raspberry_data (msjpantalla);
              		//Raspberry_data((unsigned char  *) "a;98;FUERA DE LINEA\n\r\0");
                  break;
						case UN_MOMENTO:
							strcpy(msjpantalla,"a;98;UN MOMENTO POR FAVOR\n\0");
									Raspberry_data (msjpantalla);
								//	Raspberry_data((unsigned char  *) "\n\ra;98;UN MOMENTO POR FAVOR\n\r\0");
                  break;
						case NO_CARD_MENSUAL:
							strcpy(msjpantalla,"a;08;NO ES TARJETA MENSUAL\n\0");
							Raspberry_data (msjpantalla);
              //Raspberry_data((unsigned char  *) "a;08;NO ES TARJETA MENSUAL\n\r\0");
							break;
						
						case TARJETA_INVALIDA:
							strcpy(msjpantalla,"a;98;TARJETA INVALIDA\n\0");
							Raspberry_data (msjpantalla);
               		//Raspberry_data((unsigned char  *) "a;98;TARJETA INVALIDA\n\r\0");
                  break;
						
					case SIN_SALIDA:
						strcpy(msjpantalla,"a;98;TARJETA SIN SALIDA\n\0");
								Raspberry_data (msjpantalla);
            	//Raspberry_data((unsigned char  *)  "a;98;TARJETA SIN SALIDA\n\r\0");
							 break;
					 case REGISTRA_INGRESO:
						 strcpy(msjpantalla,"a;98;REGISTRA INGRESO EN PARQUEADERO\n\0");
							 Raspberry_data (msjpantalla);
               //Raspberry_data((unsigned char  *)  "a;98;REGISTRA INGRESO EN PARQUEADERO\n\r\0");
							 break;
					case TARJETA_VENCIDA:
						strcpy(msjpantalla,"a;07;� MENSUALIDAD VENCIDA !\n\0");
							 Raspberry_data (msjpantalla);
							//Raspberry_data((unsigned char  *)  "a;07;� MENSUALIDAD VENCIDA !\n\r\0");
							 break;	
					case MENSUAL_FUERA_HORARIO:
						strcpy(msjpantalla,"a;06;MENSUAL FUERA DE HORARIO\n\0");
							 Raspberry_data (msjpantalla);
               //Raspberry_data((unsigned char  *) "a;06;MENSUAL FUERA DE HORARIO\n\r\0");
						   break;
					
					case NO_CARD:
						strcpy(msjpantalla,"a;98;SIN TARJETAS\n\0");
							 Raspberry_data (msjpantalla);
							 //Raspberry_data((unsigned char  *) "a;98;SIN TARJETAS\n\r\0");
               break;
					case LOW_CARD:
						strcpy(msjpantalla,"a;98;NIVEL BAJO DE TARJETAS\n\0");
							 Raspberry_data (msjpantalla);
              // Raspberry_data((unsigned char  *) "a;98;NIVEL BAJO DE TARJETAS\n\r\0");
							 break;
					case HORARIO_NO_PROG:
						strcpy(msjpantalla,"a;98;HORARIO NO PROGRAMADO\n\0");
							Raspberry_data (msjpantalla);
							//Raspberry_data((unsigned char  *)  "a;98;HORARIO NO PROGRAMADO\n\r\0");	
							break;		
					
					case NOREAD_CARD:
							strcpy(msjpantalla,"a;98;ACERQUE SU TARJETA DE NUEVO\n\0");
							Raspberry_data (msjpantalla);
						
					break;
				
						
					case RETIRE_TARJETA:
						strcpy(msjpantalla,"a;01;RETIRE SU TARJETA\n\0");
							Raspberry_data (msjpantalla);
             // Raspberry_data((unsigned char  *) "\n\ra;01;RETIRE SU TARJETA\n\r\0");
							break;
					case BIENVENIDO:
						strcpy(msjpantalla,"a;03;BIENVENIDO\n\0");
							Raspberry_data (msjpantalla);
              //Raspberry_data((unsigned char  *) "a;03;BIENVENIDO\n\r\0");
							break;
					case LOTE_FULL:
						strcpy(msjpantalla,"a;99;LOTE ASIGNADO ESTA LLENO\n\0");
						Raspberry_data (msjpantalla);
						break;
			
					case IN_HORARIO:
						strcpy(msjpantalla,"a;06;MENSUAL FUERA DE HORARIO\n\0");
						Raspberry_data (msjpantalla);
						break;
					
				 
					
			
					default:
					break;	
         }
				// free(msjpantalla);
				 msjpantalla=0;
         sel_com=1;   
      }
      
     

}
void PantallaLCD_LINEA_2(unsigned char cod_msg, unsigned char * buffer)
{
	
unsigned char Ini_LCD_Line_one   []={0xaa,0x80,0x18,0x01,0x02,0x00} ;
unsigned char num_chr;
unsigned char msjpantalla [40]= {0};

	//init_mempool(malloc_memoria,40);
//msjpantalla=malloc(40);


	
sel_com=0;
	
		if (Raspberry == 0)
		{
		
			
			switch (cod_msg)
			{
		
				case	LECTURA_WIEGAND:
					LCD_txt (Ini_LCD_Line_one,0);
					num_chr=strlen((unsigned char *) "WIEGAND ");
					num_chr=num_chr+strlen(buffer)+1;
					tx_chrlcd(0x00,0);
					tx_chrlcd(num_chr*2,0);
					LCD_txt((unsigned char *)        "WIEGAND ",1);	 											/*funcion q trasmite el msj al LCD  y el (1) coloca los caracteres NULL*/
					LCD_txt(buffer,1);	 																									/*funcion q trasmite el msj al LCD  y el (1) coloca los caracteres NULL*/
					break;
				case GRACIAS:
					LCD_txt (Ini_LCD_Line_one,0);
					num_chr=strlen((unsigned char *) "GRACIAS ");
					num_chr=num_chr+strlen(buffer)+1;
					tx_chrlcd(0x00,0);
					tx_chrlcd(num_chr*2,0);
					LCD_txt((unsigned char *)        "GRACIAS ",1);	 											/*funcion q trasmite el msj al LCD  y el (1) coloca los caracteres NULL*/
					LCD_txt(buffer,1);		
				  break;	
				
				
				
			default:
			
				break;	
			
			}
				sel_com=1;	
		}
		else
		{
			 sel_com=0;  
			
       switch (cod_msg)
			 {
				 case	LECTURA_WIEGAND:
						
										 
						strcpy(msjpantalla,"a;92;WIEGAND ");
								 
						strcat(msjpantalla,buffer);
						
						strcat(msjpantalla,"\n\0");
						
				 
						Raspberry_data (msjpantalla);
						
					
						break;
				 
				  case GRACIAS:
            strcpy(msjpantalla,"a;91;GRACIAS ");
						num_chr=strlen(buffer);
						*(buffer+(num_chr-1))=0;
						strcat(msjpantalla,buffer);
						strcat(msjpantalla,"\n\0");
						Raspberry_data (msjpantalla);
					
            break;
					 case BIENVENIDO:
						 
            strcpy(msjpantalla,"a;03;BIENVENIDO ");
					 Raspberry_data (msjpantalla);
						 if(MenSual !=  True)
						{
							/*tarjeta de rotacion*/
						strcpy(msjpantalla, Lee_No_Ticket());
						//strcat(msjpantalla,ticket);
							 Raspberry_data (msjpantalla);
						}
						else
						{
							/*mensual nombre del mensual*/
							strcpy(msjpantalla, buffer);
						//strcat(msjpantalla,buffer);
							 Raspberry_data (msjpantalla);
							
						}
						if (placa_ready== True)
							{
							strcpy(msjpantalla, " PLACA");	
							//strcat(msjpantalla," PLACA:");
							strcat(msjpantalla,placa);	
								
								strcat(msjpantalla,"\n\0");
							}
							else
							{
								strcpy(msjpantalla, "\n");	
								//strcat(msjpantalla,"\n\r\0");
							}	
							Raspberry_data (msjpantalla);
						
						
            break;
						 case BIENVENIDO_WIEGAN:
							strcpy(msjpantalla,"a;03;BIENVENIDO ");
							strcat(msjpantalla,buffer);
							strcat(msjpantalla,"\n\0");
					
							Raspberry_data (msjpantalla);
						 
							 break;
					  case TARJETA_VENCIDA:
						strcpy(msjpantalla,"a;07; � MENSUALIDAD VENCIDA ! VENCIO 20");
						num_chr=strlen(buffer);
						*(buffer+(num_chr-1))=0;
						strcat(msjpantalla,buffer);
						strcat(msjpantalla,"\n\0");
						Raspberry_data (msjpantalla);
						
            break;
					 case CUPOS:
						 strcpy(msjpantalla,"c;90;");
						 strcat(msjpantalla,buffer);
						 strcat(msjpantalla,"\n\0");
					   Raspberry_data (msjpantalla);
						 break;
				default:
			
					break;	
			 }
			// free(msjpantalla);
			 msjpantalla [0]=0;
			 sel_com=1;	
		}
	}
/*------------------------------------------------------------------------------
transmite el caracter pto serie
data_com = al caracter a escribir
enable_char_add = si esta en (1) envia un null (0) adicional, si es (0) no envia caracter adicional
------------------------------------------------------------------------------*/
void tx_chrlcd (unsigned char data_com, unsigned char enable_char_add)
{
	unsigned char d;
	d=putchar(data_com);
	if (enable_char_add != 0)	d=putchar(0x00);
	
}

/*------------------------------------------------------------------------------
envia un msj asta null(0)
msg= apuntador del msj
enable_char_add = si esta en (1) envia un null (0) adicional, si es (0) no envia caracter adicional
------------------------------------------------------------------------------*/
void LCD_txt (unsigned char * msg,unsigned char enable_char_add )
{
	unsigned char i;
	 
	for (i=0; msg[i] != '\0'; i++)
	{
 	 	tx_chrlcd(msg[i],enable_char_add);
	}
}
/*------------------------------------------------------------------------------
Escribo el reloj en ascii en bloque 
msg= apuntador del msj
length_char longitud de la tram
enable_char_add = si esta en (1) envia un null (0) adicional, si es (0) no envia caracter adicional
------------------------------------------------------------------------------*/
void LCD_txt_num_char(unsigned char * msg,unsigned char length_char, unsigned char enable_char_add)
{
	unsigned char i;
	 
	for (i=0; i<length_char; i++)
	{
 	 	tx_chrlcd(msg[i],enable_char_add);
	}
}

void Raspberry_data (unsigned char *msjpantalla)
{
	
	unsigned char i;
	unsigned char lenth_cadena;
	unsigned char d;
	
	
	lenth_cadena=strlen(msjpantalla);
	
	for (i=0; i < lenth_cadena ; i++)
	{
			
		for (d=0; d<100; d++)
   {
	 }
		d=putchar(*(msjpantalla + i));

	}
	
		
		for (d=0; d<100; d++)
   {
    /*** DO NOTHING ***/
    }
	
}

/*------------------------------------------------------------------------------
Escribo el reloj en ascii en bloque 
AA 80 28 trama de inicio de configuracion de la pantalla
07 numero de caracteres de la trama de reloj
20 19 03 26 09 21 20  el dato del reloj
------------------------------------------------------------------------------*/
void Reloj_Pantalla_Lcd()
{

 unsigned char Ini_Clock_LCD   []={0xaa,0x80,0x28,0x07,0x20,0x00,0,0,0,0,20,0,0} ;

				if (Raspberry==0)
				{	
					sel_com=0;																																			/*switch del pto serie a la pantalla*/
					Block_read_Clock(Ini_Clock_LCD+5);																							/*Leo el reloj programado*/
					//Debug_Dividir_texto();																													/*lineas de separacion del texto*/
					//DebugBufferMF(Ini_Clock_LCD,12,0);																							/*muestra la trama por debug*/
					//Debug_Dividir_texto();																													/*linea de separacion de texto*/
					REN = 0;																																				/*inhabilita recepcion de datos*/
					LCD_txt_num_char(Ini_Clock_LCD,13,0);																						/*cmd de inicializacion del reloj del lcd*/
																											
					REN = 1;																																				/*habilita recepcion de datos*/
					sel_com=1;	
						/*switch pto serie a verificador o expedidor */
				}
				else
				{
						sel_com=0;
						Ini_Clock_LCD [0]=0;
						strcpy(Ini_Clock_LCD,"d;");
					
						Block_read_clock_ascii_rasberry(Ini_Clock_LCD+2);
						strcat(Ini_Clock_LCD,"\n\0");
					  Raspberry_data (Ini_Clock_LCD);
							
			
						sel_com=1;	
				}
}

		
	
