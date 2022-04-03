/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                     Programação em C18 - PIC18F4520                     *
 *                              PRÁTICA 01                                 *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *     VERSÃO : 1.0                                                        *
 *     DATA : 20/07/12                                                     *
 *     PROGRAMMER : David Maciel Silva                                     *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                         *
 * DEVELOPMENT BOARD: Kit C18 Proteus                                      *                                                  
 *                                                                         *
/* * * * * * * * *  * * * * * * * * * * * * * * * * * * * * * * * * * * * **
 *                             Descrição geral                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                DEFINIÇÃO DAS VARIÁVEIS INTERNAS DO PIC                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// O arquivo de definições do pic utilizado deve ser referenciado para que
//os nomes definidos pela Microchip possam ser utilizados, sem a necessidade
//de redigitação.

#include <p18F4520.h>         // ESSA LINHA DEVE SER INSERIDO O DISPOSITIVO.
#include "displayLCD.h"       //BIBLIOTECA QUE CONTEM FUNÇÕES PARA DISPLAY LCD

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                         Configurações para gravação                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#pragma config OSC = HS     // Oscilador HS com PLL habilitado, HS usado pelo USB
#pragma config PBADEN = OFF        // Desabilita conversor A/D no PORTB       
#pragma config PWRT = ON           // Habilita Power-up Timer
#pragma config WDT = OFF           // Watch-dog Timer desabilitado 
#pragma config LVP = OFF           // Desabilita programação por baixa tensão
#pragma config DEBUG = OFF         // Desabilita modo de DEGUB

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                INCLUDES DAS FUNÇÕES DE PERIFÉRICOS DO PIC             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <pwm.h>                 //PWM library functions
#include <adc.h>                  //ADC library functions
#include <timers.h>               //Timer library functions
#include <delays.h>               //Delay library functions
#include <i2c.h>                  //I2C library functions
#include <stdlib.h>               //Library functions
#include <stdio.h>               //Output functions
#include <usart.h>                //USART library functions
#include <math.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                       PROTOTIPAGEM DE FUNÇÕES                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void INT_TIMER0(void);
void ajusthora(void);
void alimentador(void);
void alarmeajus(void);
void relogio(void);
void temperatura(void);
void prog1(void);
void prog2(void);
void prog3(void);
void prog4(void);
void prog5(void);
void alimentar(void);
void led_pisca(void);
void led_ligado(void);
void oxi (void);
void liga_cooler(void);
void volts (void);
float temp_conv(float);




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                           Constantes internas                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//A definição de constantes facilita a programação e a manutenção.



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                 Definição e inicialização das variáveis                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//Neste bloco estão definidas as variáveis globais do programa.

unsigned int cont=0,cont2=0,x=0,hora=0,minuto=0,a=0,alamin=0,alahora=0,b=0,BUZE=0;
unsigned int menu=0,enter=0,prog=1,exit=0,mhora=0,sensor_temp,valor_temp=0,dec,dec2,un1=0;
unsigned int progh1=0,progh2=0,progh3=0,progh4=0,progh5=0,progm1=0,progm2=0,progm3=0,progm4=0,progm5=0,ativa1=0,ativa2=0,ativa3=0,ativa4=0,ativa5=0,giro;// variaveis para timer do alimentador.
unsigned char lcd[17],exibe,buffer[17];
unsigned int temp,dec1,dec2,volt,rest,store,t_volts,un,enter2;
unsigned float tensao, resultado_conv, resultado_conv1;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                    Declaração dos flags de software                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//A definição de flags ajuda na programação e economiza memória RAM.

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                ENTRADAS                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// As entradas devem ser associadas a nomes para facilitar a programação e
//futuras alterações do hardware.
#define ATIVA PORTBbits.RB0
#define BT1 PORTBbits.RB1
#define BT2 PORTBbits.RB2
#define BT3 PORTBbits.RB3
#define BT4 PORTBbits.RB4

//#define SENS1 PORTAbits.RA1
//#define SENS2 PORTAbits.RA2
#define ALIM2 PORTCbits.RC5


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                 SAÍDAS                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// AS SAÍDAS DEVEM SER ASSOCIADAS A NOMES PARA FACILITAR A PROGRAMAÇÃO E
//FUTURAS ALTERAÇÕES DO HARDWARE.

#define LED1 PORTCbits.RC0
#define LED2 PORTCbits.RC1
#define OXI1 PORTCbits.RC2//IN1
#define OXI2 PORTCbits.RC3//IN2
#define OXIGENADOR PORTCbits.RC4//IN3
#define OXIGENADOR2 PORTCbits.RC5//IN4
#define COO1 PORTCbits.RC6






/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                            Função Principal                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void main ()
   {
		
	OpenADC( ADC_FOSC_16        //Tempo de conversão de cada bit: TAD=1/(FOSC/16).
            &ADC_RIGHT_JUST     //Resultado é dado pelos bits mais significativos
            &ADC_2_TAD,         //Tempo de aquisição: tempo mínimo necessário para fazer a conversão de cada bit (2*TAD)
             ADC_CH0            //Seleciona o canal de conversão(no caso canal 1)
            &ADC_INT_OFF        //Desabilita a interrpção do canal AD
            &ADC_VREFPLUS_VDD   //Torna tensão de referencia igual a tensão de alimentação do PIC = 5V.
            &ADC_VREFMINUS_VSS, //Valor de referencia minimo igual a VSS = 0v.
             ADC_2ANA);        //Habilita 2 canais AD.

	OpenADC( ADC_FOSC_16        //Tempo de conversão de cada bit: TAD=1/(FOSC/16).
            &ADC_RIGHT_JUST     //Resultado é dado pelos bits mais significativos
            &ADC_2_TAD,         //Tempo de aquisição: tempo mínimo necessário para fazer a conversão de cada bit (2*TAD)
             ADC_CH1           //Seleciona o canal de conversão(no caso canal 1)
            &ADC_INT_OFF        //Desabilita a interrpção do canal AD
            &ADC_VREFPLUS_VDD   //Torna tensão de referencia igual a tensão de alimentação do PIC = 5V.
            &ADC_VREFMINUS_VSS, //Valor de referencia minimo igual a VSS = 0v.
             ADC_2ANA);         //Habilita 2 canais AD.

    SetChanADC(ADC_CH0);        //Seta canal AD 1.
    SetChanADC(ADC_CH1);        //Seta canal AD 2.
    Delay10TCYx(5);         //Tempo necessario para para configuração do canal e leitura.;
	
		
		   PORTA = 0X00;                       //Clear PORTA
		   PORTB = 0X00;                       //Clear PORTB
		   PORTC = 0X00;                       //Clear PORTC
		   PORTD = 0X00;                       //Clear PORTD
		   PORTE = 0x00;                       //Clear PORTE
		
		   LATA = 0X00;                       	//Clear PORTA
		   LATB = 0X00;                       	//Clear PORTB
		   LATC = 0X00;                  		//Clear PORTC
		   LATD = 0X00;                       	//Clear PORTD
		   LATE = 0x00;                       	//Clear PORTE
		
		   TRISA = 0b00000011;                  //CONFIG DIREÇÃO DOS PINOS PORTA
		   TRISB = 0b00011110;                  //CONFIG DIREÇÃO DOS PINOS PORTB
		   TRISC = 0b00000000;               	//CONFIG DIREÇÃO DOS PINOS PORTC
		   TRISD = 0b00000000;                  //CONFIG DIREÇÃO DOS PINOS PORTD
		   TRISE = 0b00000000;                  //CONFIG DIREÇÃO DOS PINOS PORTE        

		   ADCON2 = 13;                //DESLIGA CONVERSORES A/D
		
		   INTCONbits.GIE=1;          	//Habilita as interrupções da CPU -  Interrupções globais.
		   INTCONbits.GIEH=1;        	// Habilita interrupção no vetor de alta prioridade.
		   INTCONbits.GIEL=0;       	// Desabilita interrupção no vetor de baixa prioridade.
		   INTCONbits.PEIE=1;      		// Habilita interrupção de periféricos
		   RCONbits.IPEN=1;       		// Habilita prioridade de interrupção
		   INTCONbits.TMR0IE=1;  		//Habilita interrupção por estouro do timer 0
		   INTCONbits.TMR0IF=0;  		//Flag da interrupção do timer 0
		   INTCON2bits.TMR0IP=1; 		//Prioridade alta para interrupção pelo estouro do timer 0
		
		/***** Configurações Timer 0 *********/
		
		   OpenTimer0(TIMER_INT_ON
		           &T0_16BIT
		           &T0_SOURCE_INT
		           &T0_PS_1_128);      //Estouro a cada 209ms.
		
		   WriteTimer0(26474); // incia com o valor a ser estourado.
		
		/***** Configurações de Porta Serial RS232 *********/
      
   


		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		 *  			 Configurações de Portas de conversão AD				 *
		 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

 	   inicializa_lcd();
	   led_ligado();
	   led_pisca();
	   sprintf(lcd,"   AQUATRAVEL        ");
	   envia_byte_lcd(0,0x80);
	   putsXLCD(lcd);
	   sprintf(lcd,"                       ");
	   envia_byte_lcd(0,0xC0);
	   putsXLCD(lcd);
	   Delay10KTCYx(250);//Aguarda 
	   Delay10KTCYx(250);//Aguarda 
	
		menu=1;
		ATIVA=1;

		
		
		
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                            Rotina Principal                         *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	   while(1)// inicio da rotina principal.
	  	{   

    	oxi();
		liga_cooler();
		//temperatura();
		
			/*sprintf(lcd,"    Menu: %01d         ",menu);
	 	    envia_byte_lcd(0,0x80);
	        putsXLCD(lcd); */
			sprintf(lcd," AQUATRAVEL  M%1d    ",menu);
		    envia_byte_lcd(0,0x80);
			putsXLCD(lcd);
	        sprintf(lcd," Hora: %02d"":%02d"":%02d   " ,hora,minuto,cont);
	 	    envia_byte_lcd(0,0xC0);
	        putsXLCD(lcd);  
	




		
		 	
		   if (BT1==1)    //Inicio do menu
			{ 
				led_pisca();
				Delay10KTCYx(100);//Aguarda 
				menu ++; 
	    	
	   			if (menu==4)
	      			{
	       		 	menu = 1   ;
	     			 }
			}
		   if ((menu ==1)&&(BT3==1))
		 	{
				  led_pisca();
				  Delay10KTCYx(100);//Aguarda 
			      sprintf(lcd," Ajuste Hora");
			 	  envia_byte_lcd(0,0x80);
			      putsXLCD(lcd);
			      Delay10KTCYx(100);//Aguarda 
			  	  enter = 1;
			  	  ajusthora(); 
			} 
		   if ((menu ==2)&&(BT3==1)) // ENTRA NO MENU ALIMENTADOR
			{
				  led_pisca();
				  Delay10KTCYx(100);//Aguarda 
			      sprintf(lcd," 	Alimentador    ");
			 	  envia_byte_lcd(0,0x80);
			      putsXLCD(lcd);
			      Delay10KTCYx(150);//Aguarda 
			      enter = 1;
			      alimentador();
			} 
			if ((menu ==3)&&(BT3==1)) // ENTRA NO MENU ALIMENTADOR
			{
				  temperatura();

			}
					   if ((progh1==hora)&&(progm1==minuto)&&(ativa1==1)&&(cont==1))
					    {
					         sprintf(lcd,"  ALIMENTANDO       ");
					 	     envia_byte_lcd(0,0x80);
					         putsXLCD(lcd);
							 Delay10KTCYx(50);//Aguarda
					    	 enter = 1;
					    	 alimentar();
						}
					   if ((progh2==hora)&&(progm2==minuto)&&(ativa2==1)&&(cont==1))
					    {
				       		sprintf(lcd,"  ALIMENTANDO       ");
				    	    envia_byte_lcd(0,0x80);
				            putsXLCD(lcd);
					 	    Delay10KTCYx(50);//Aguarda
							enter = 1;
							alimentar();
						}
				       if ((progh3==hora)&&(progm3==minuto)&&(ativa3==1)&&(cont==1))
					    {
				    		sprintf(lcd,"  ALIMENTANDO       ");
				 	  		envia_byte_lcd(0,0x80);
				     		putsXLCD(lcd);
							Delay10KTCYx(50);//Aguarda
							enter = 1;
							alimentar();
						}
				      	if ((progh4==hora)&&(progm4==minuto)&&(ativa1==4)&&(cont==1))
					    {
				    		sprintf(lcd,"  ALIMENTANDO       ");
				 	  	    envia_byte_lcd(0,0x80);
				            putsXLCD(lcd);
				            Delay10KTCYx(50);//Aguarda
				  			enter = 1;
							alimentar();
						}
				       if ((progh5==hora)&&(progm5==minuto)&&(ativa5==1)&&(cont==1))
					    {
				    		sprintf(lcd,"  ALIMENTANDO       ");
				 	  		envia_byte_lcd(0,0x80);
				      		putsXLCD(lcd);
							Delay10KTCYx(50);//Aguarda
							enter = 1;
							alimentar();
						}

		}
	}




               
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                            Funções externas                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */




		void ajusthora (void)// Rotina para ajuste do relogia.
		{ 
			while(enter==1)
			{
				  led_pisca();
			      sprintf(lcd,"  Hora / Minuto         ");
			 	  envia_byte_lcd(0,0x80);
			      putsXLCD(lcd);
			      sprintf(lcd,"     %02d"":%02d        " ,hora,minuto);
			      envia_byte_lcd(0,0xC0);
			      putsXLCD(lcd); 
			  
						if(cont == 60)// limita segundos a 60.
							{
								cont = 0 ;
							}
								if (BT1 ==1)
					   			 {
									hora++;
									if (hora==24)// limita a hora a 24.
								 {
									hora=0;
								 }
									Delay10KTCYx(100);//Aguarda 
								 }
					    if (BT2 == 1)
					   	{
							led_pisca();
							minuto++;
							if (minuto == 60)//limita minutos a 60.
						{
							minuto = 0;
							hora++;
						}
						Delay10KTCYx(1);//Aguarda 
				    	}
					if (BT4 == 1)
				    	{
						  led_pisca();
						  Delay10KTCYx(100);//Aguarda
					      sprintf(lcd,"    Relogio         ");
					 	  envia_byte_lcd(0,0x80);
					      putsXLCD(lcd);
					      sprintf(lcd,"    Ajustado             ");
					      envia_byte_lcd(0,0xC0);
					      putsXLCD(lcd); 
						  enter=0;
						  menu=0;
						  main ();
						}
			}
		
		}
		void alimentador(void){	// Rotina para ajuste do temporizador 

			while(enter==1)
					{   	 
				      sprintf(lcd,"Programa Alime.      ");
				 	  envia_byte_lcd(0,0x80);
				      putsXLCD(lcd); 
				      sprintf(lcd,"    Timer: %01d   " ,prog);
				 	  envia_byte_lcd(0,0xC0);
				      putsXLCD(lcd);
				
						Delay10KTCYx(100);//Aguarda
					
						if (BT1==1)//Inicio do menu
						{ 
								prog ++; 
						 
						   if (prog==6)
						      {
						        	prog = 1   ;
						      } 
							Delay10KTCYx(100);//Aguarda 
						}
					if ((prog == 1)&&(BT3==1))//entra no primeiro ajuste do temporizador
					    {  
							  led_pisca();
							  sprintf(lcd,"   Programar         ");
						 	  envia_byte_lcd(0,0x80);
						      putsXLCD(lcd);
						      sprintf(lcd,"    Timer 1          ");
						      envia_byte_lcd(0,0xC0);
						      putsXLCD(lcd);
							  Delay10KTCYx(50);//Aguarda 
						      enter = 1;
							  prog1();
						}
					if ((prog == 2)&&(BT3==1))//entra no segundo ajuste do temporizador
					    {  
							  led_pisca();
							  sprintf(lcd,"   Programar  ");
						 	  envia_byte_lcd(0,0x80);
						      putsXLCD(lcd);
						      sprintf(lcd,"  Timer 2 ");
						      envia_byte_lcd(0,0xC0);
						      putsXLCD(lcd);
							  Delay10KTCYx(50);//Aguarda  
							  enter = 1;
							  prog2();
				 
						}
					if ((prog == 3)&&(BT3==1))//entra no terceiro ajuste do temporizador
					    {  
							  led_pisca();
							  sprintf(lcd,"   Programar  ");
						 	  envia_byte_lcd(0,0x80);
						      putsXLCD(lcd);
						      sprintf(lcd,"  Timer 3 ");
						      envia_byte_lcd(0,0xC0);
						      putsXLCD(lcd);
							  Delay10KTCYx(50);//Aguarda  
							  enter = 1;
							  prog3();
				 
						}
					if ((prog == 4)&&(BT3==1))//entra no quarto ajuste do teprizador
					    {  
							  led_pisca();
							  sprintf(lcd,"   Programar  ");
						 	  envia_byte_lcd(0,0x80);
						      putsXLCD(lcd);
						      sprintf(lcd,"  Timer 4 ");
						      envia_byte_lcd(0,0xC0);
						      putsXLCD(lcd);
							  Delay10KTCYx(100);//Aguarda 
						      enter = 1;
						      prog4();
				
						}
					if ((prog == 5)&&(BT3==1))//entra no quinto ajuste do teporizador
					    {  
							  led_pisca();
							  sprintf(lcd,"   Programar  ");
						 	  envia_byte_lcd(0,0x80);
						      putsXLCD(lcd);
						      sprintf(lcd,"  Timer 5 ");
						      envia_byte_lcd(0,0xC0);
						      putsXLCD(lcd);
						 	  Delay10KTCYx(100);//Aguarda 
						      enter = 1;
						      prog5();
						}
						if (BT4 == 1)//rotina de retorno
					    {  
						 Delay10KTCYx(100);//Aguarda 
				         enter==0; 
					 	 main ();
						}
				    }
		}
		void prog1(void)// rotina para programar alientaçao
		{
			while(enter==1)
				{
			 		if (ativa1==0)
					{
			  		     exibe ='N';
					} 
			
					if (BT3==1)
					{ 
						 ativa1 ++;
					}
			
				    if (ativa1==1)
					{
			    		exibe ='S';
					}
			    	if (ativa1==2)
					{
						ativa1 =0;
			 		 	exibe ='N';
					}
					Delay10KTCYx(100);//Aguarda
			if (BT1==1)
			{
					progh1++;
			
						if (progh1==24)
							{
								progh1=0;
							}
						Delay10KTCYx(100);//Aguar
							}
						if (BT2 == 1)
						    {
								progm1++;
					
						if (progm1 == 60)
							{
							progm1 = 0;
							}
						Delay10KTCYx(100);//Aguarda
							} 
							if (BT4 == 1)
						    {  
						Delay10KTCYx(100);//Aguarda 
					         enter==0; 
						 	 alimentador ();
							}
					      sprintf(lcd," Ativar: %01c     ",exibe);
					 	  envia_byte_lcd(0,0x80);
					      putsXLCD(lcd);
					      sprintf(lcd,"       %02d"":%02d        " ,progh1,progm1);
					      envia_byte_lcd(0,0xC0);
					      putsXLCD(lcd);
						}
		 }
		void prog2(void)// rotina para programar alientaçao
		{
			while(enter==1)
			{
		 		if (ativa2==0)
					{
			  		     exibe ='N';
					} 
		
				if (BT3==1)
					{ 
						 ativa2 ++;
					}
				
			 	if (ativa2==1)
					{
				    	exibe ='S';
					}
				if (ativa2==2)
					{
						ativa2 =0;
				 		exibe ='N';
					}
				Delay10KTCYx(100);//Aguarda
				if (BT1==1)
					{	
					progh2++;
			
					if (progh2==24)
					{
						progh2=0;
					}
					Delay10KTCYx(100);//Aguar
				}
				if (BT2 == 1)
			    {
					progm2++;
		
				if (progm2 == 60)
				{
					progm2 = 0;
				}
				Delay10KTCYx(100);//Aguarda
				} 
					if (BT4 == 1)
			    {  
					 Delay10KTCYx(100);//Aguarda 
			         enter==0; 
				 	 alimentador ();
				}
			      sprintf(lcd," Ativar: %01c     ",exibe);
			 	  envia_byte_lcd(0,0x80);
			      putsXLCD(lcd);
			      sprintf(lcd,"       %02d"":%02d        " ,progh2,progm2);
			      envia_byte_lcd(0,0xC0);
			      putsXLCD(lcd);
			}
		}
		void prog3(void)// rotina para programar alientaçao
		{
			while(enter==1)
			{
		 		if (ativa3==0)
				{
		  		     exibe ='N';
				} 
		
				if (BT3==1)
				{ 
					 ativa3 ++;
				}
		
			    if (ativa3==1)
				{
		    		exibe ='S';
				}
				if (ativa3==2)
				{
					ativa3 =0;
		 		 	exibe ='N';
				}
				Delay10KTCYx(100);//Aguarda
				if (BT1==1)
				{
					progh3++;
			
				if (progh3==24)
				{
					progh3=0;
				}
				Delay10KTCYx(100);//Aguar
				}
				if (BT2 == 1)
			    {
					progm3++;
		
				if (progm3 == 60)
				{
				progm3 = 0;
				}
				Delay10KTCYx(100);//Aguarda
				} 
				if (BT4 == 1)
			    {  
				Delay10KTCYx(100);//Aguarda 
		         enter==0; 
			 	 alimentador ();
				}
			      sprintf(lcd," Ativar: %01c     ",exibe);
			 	  envia_byte_lcd(0,0x80);
			      putsXLCD(lcd);
			      sprintf(lcd,"       %02d"":%02d        " ,progh3,progm3);
			      envia_byte_lcd(0,0xC0);
			      putsXLCD(lcd);
			}
		}
		void prog4(void)// rotina para programar alientaçao
		{
			while(enter==1)
			{
		 		if (ativa4==0)
			{
		  		     exibe ='N';
			} 
		
				if (BT3==1)
			{ 
					 ativa4 ++;
			}
		
			 if (ativa4==1)
			{
		    		exibe ='S';
			}
			if (ativa4==2)
			{
					ativa4 =0;
		 		 	exibe ='N';
			}
		Delay10KTCYx(100);//Aguarda
			if (BT1==1)
			{
					progh4++;
			
			if (progh4==24)
				{
					progh4=0;
				}
		Delay10KTCYx(100);//Aguar
				}
			if (BT2 == 1)
			    {
					progm4++;
		
			if (progm4 == 60)
				{
				progm4 = 0;
				}
		Delay10KTCYx(100);//Aguarda
				} 
				if (BT4 == 1)
			    {  
		Delay10KTCYx(100);//Aguarda 
		         enter==0; 
			 	 alimentador ();
				}
		 	  sprintf(lcd," Ativar: %01c     ",exibe);
		 	  envia_byte_lcd(0,0x80);
		      putsXLCD(lcd);
		      sprintf(lcd,"       %02d"":%02d        " ,progh4,progm4);
		      envia_byte_lcd(0,0xC0);
		      putsXLCD(lcd);
			}
		 }
		void prog5(void)// rotina para programar alientaçao
		{
			while(enter==1)
			{
				 		if (ativa5==0)
					{
				  		     exibe ='N';
					} 
				
						if (BT3==1)
					{ 
							 ativa5 ++;
					}
				
					 if (ativa5==1)
					{
				    		exibe ='S';
					}
					if (ativa5==2)
					{
							ativa5 =0;
				 		 	exibe ='N';
					}
				Delay10KTCYx(100);//Aguarda
					if (BT1==1)
					{
							progh5++;
					
					if (progh5==24)
						{
							progh5=0;
						}
				Delay10KTCYx(100);//Aguar
						}
					if (BT2 == 1)
					    {
							progm5++;
				
					if (progm5 == 60)
						{
						progm5 = 0;
						}
				Delay10KTCYx(100);//Aguarda
						} 
						if (BT4 == 1)
					    {  
				Delay10KTCYx(100);//Aguarda 
				         enter==0; 
					 	 alimentador ();
						}
				 sprintf(lcd," Ativar: %01c     ",exibe);
				 	  envia_byte_lcd(0,0x80);
				      putsXLCD(lcd);
				      sprintf(lcd,"       %02d"":%02d        " ,progh5,progm5);
				      envia_byte_lcd(0,0xC0);
				      putsXLCD(lcd);
			}
		}
			void alimentar(void)// 
			{
			while(enter==1)
			{
			      sprintf(lcd," Hora: %02d"":%02d"":%02d   " ,hora,minuto,cont);
			 	  envia_byte_lcd(0,0xC0);
			      putsXLCD(lcd);
				  giro=5;
			
				if (cont < giro)
				{
				 OXI1 = 1;
			 	 OXI2 = 0;
				}
				if (cont == 20)
				{
			        OXI1 = 0;
			        OXI2 = 0;
					enter=0;
					main ();
				}
			}
			
			
			}
			void led_ligado(void)
			{
				if (PORTEbits.RE3=1)
				{
				Delay10KTCYx(250);//Aguarda 
				LED1=1;
				}
			}
			
			void led_pisca(void)
			{
				if (BT1==1 || BT2==1 || BT3==1 || BT4==1)
				{
				 
				LED2=1;
				Delay10KTCYx(100);//Aguarda
				LED2=0;
				}
			}
//float temp_conv()
//{
//
//				SetChanADC(ADC_CH0);
////				ConvertADC(); // Inicia a conversão.
////			    while (BusyADC()); // Aguarda fim.
////			    sensor_temp=ReadADC(); //Armazena conversão.
////				valor_temp=5.0*sensor_temp*100.0/1023.0;
////				 Delay10KTCYx(100);//Aguarda 
//    ConvertADC(); // Inicia a conversão.
//    while (BusyADC()); // Aguarda fim.
//    resultado_conv = ReadADC(); //valor real float.
//    temp= resultado_conv * 0.004882812;
//    temp = temp/10;
//
//return (temp);
//}

			void temperatura(void)
	        
	 		{
			
				while (BT4==0)
				{
				SetChanADC(ADC_CH0);
	ConvertADC(); // Inicia a conversão.
    while (BusyADC()); // Aguarda fim.
    resultado_conv=ReadADC(); //Armazena conversão.
    temp=resultado_conv;
sensor_temp=(temp*0.0048875855327468);
valor_temp =sensor_temp;
//				ConvertADC();// inicia a conversão.
//			    while (BusyADC()); // aguarda fim.
//			    sensor_temp=ReadADC(); //armazena conversão.
//				valor_temp =5.0*sensor_temp*100.0/1023.0;
//				  Delay10KTCYx(100);//Aguarda 

			
			      sprintf(lcd,"TEMP.AQUA.:%2d%cC  		    ",valor_temp ,0xDF);
			 	  envia_byte_lcd(0,0x80);
			      putsXLCD(lcd);
			      Delay10KTCYx(150);//Aguarda
				  sprintf(lcd,"                         ");
			 	  envia_byte_lcd(0,0xC1);
			      putsXLCD(lcd); 
				 Delay10KTCYx(100);//Aguarda 
				}
			}


				void oxi(void)
			{
	

				SetChanADC(ADC_CH1);
				ConvertADC(); // Inicia a conversão.
		   	while (BusyADC()); // Aguarda fim.
		    	resultado_conv1=ReadADC(); //Armazena conversão.
				tensao=(resultado_conv1*0.004882812)/12;
				un=tensao;
			if (un<=2) 
				{
				 OXIGENADOR=1;
				 OXIGENADOR2=0;
				Delay10KTCYx(100);
				}
			if (un>2)
				{
				OXIGENADOR=0;
				OXIGENADOR2=0;
			Delay10KTCYx(100);	
			
	
			
			}
			}
			


			void liga_cooler(void)
{
		

				if (valor_temp>27)
					{
						COO1=1;
					Delay10KTCYx(100);//Aguarda 
					}
				if (valor_temp<=27)
					{
						COO1=0;
					Delay10KTCYx(100);//Aguarda 
					}
	
}


#pragma interrupt INT_TIMER0
void INT_TIMER0(void)
{
   // Verifica se o motivo da chamada da interrupção foi mudança de estado no PORTB
   if(INTCONbits.TMR0IF)
   {
      cont++;
      INTCONbits.TMR0IF=0;
      WriteTimer0(26474);
   }// end tratamento da interrupção do PORTB (INTCONbits.RBIF)
	if(cont == 60)     // cronometragem dos segundos 
		{
		cont = 0 ;     // zeramento dos segundos
		minuto++;	  // acrescimo dos minutos
		}

	if (minuto == 60)        // cronometragem dos minitos
		{
		minuto = 0;         // zeramento dos minutos
		hora++;				// acrescimo das horas
		}

	if (hora==24)            // cronometragem das horas
		{
		hora=0;				 // zeramento das horas
		}					//fim da cronometragem do relogio
}// end interrupt



/** EOF main.c ***************************************************************/

// Alocação da função de tratamento das interrupções de ALTA prioridade
// no endereço 0x808 da memória de programa.
//
#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = 0x08
void _high_ISR (void)
{

       _asm goto INT_TIMER0 _endasm


}

/** V E C T O R   R E M A P P I N G ******************************************/
// Seção necessária para informar ao compilador C18 onde são os novos endereços
//da memória de programa que ele deve alocar as rotinas de tratamento do evento
//de "reset" do microcontrolador.
//
//ATENÇÃO: COPIAR ESTA SEÇÃO DO CODIGO PARA TODO ARQUIVO "main.c" DOS PROJETOS
//QUE UTILIZAM O BOOTLOADER PARA GRAVAÇÃO IN-CIRCUIT.

extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code _RESET_INTERRUPT_VECTOR = 0x000800
void _reset (void)
{
    _asm goto _startup _endasm
}
#pragma code
