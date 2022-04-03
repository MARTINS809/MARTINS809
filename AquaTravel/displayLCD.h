/******************************************************************************
 *
 *                      Certifica��o ONLINE - C18 - UDTEC
 *                              WWW.UDTEC.COM.BR
 *                    COPRIGHT@: UDTEC PROIBIDA DISTRIBUI��O
 ******************************************************************************
 * Nome do Arquivo:     displayLCD.h
 * Dependencias:        Veja se��o INCLUDES
 * Processador:         PIC18F4520
 * Op��o de Clock:      HS 20MHz (externo)   
 * Compilador:          C18 v3.20 ou superior
 * Empresa:             UDTEC
 *
 * Plataforma:          KIT PIC18F#UDTEC
 *
 * Autor:               Diogo Rafael de Oliveira Xavier
 *                      David Maciel
 *                     
 *
 * Data:                v1 - 29/Jul/2010
 *                      v2 - 20/Out/2010
 *                      v3 - 15/Out/2011
 *
 *
 *
 * Descri��o:           Modifica��o da biblioteca fornecida no tutorial Comunica��o de
 *                      LCD em 4 vias - Autor: Eduardo Souza Ramos. A biblioteca utilizada
 *                      no tutorial trata da comunica��o com display de LCD por meio de
 *                      4-bits (length), utilizando o PORTD tanto para os 4 bits de dados
 *                      quanto para os 3 bits de controle (RS, RW e E).
 *                      No projeto do Kit I/O FLEX o display LCD pode ser ligado a qualquer portas
 *                      digitais do PIC 18F4550. Por�m esta biblioteca utiliza
 *                      os seguintes pinos para comunica��o com o display LCD:
 *
 *
 *   Notas:
 *      - Essa bliblioteca de rotinas foi escrita pensando em um LCD
 *        16x2 FDCC1602E implementado com o controlador SPLC780D, mas pode
 *         ser utilizado com qualquer LCD 16x2 com controladores equivalentes
 *        como, por exemplo, o controlador Hitachi HD44780 LCD controller.
 *
 *         -- O sincronismo da comunica��o com o LCD:
 *             As temporiza��es entre o envio de comandos, n�o foi utilizado
 *            o bit de Busy do LCD, mas sim, atrav�s de fun��es de atraso.
 *            A fun��o utilizada foi a Delay10KTCYx() da bilioteca padr�o C18
 *            delays.h que aparece na se��o INCLUDE deste arquivo.
 *             Os tempos de delay escolhidos foram utilizando-se um cristal de 20MHz,
 *            caso se utilize um cristal de valor diferente talvez haja a necessidade de
 *            altera��o nas rotinas de tempo para que as mesmas se adequem ao
 *            tempo m�nimo requerido pelo display de LCD (5ms o de sincronismo e
 *            15ms o inicial).
 *   v3  - Inser��o da fun��o Liga e Desliga BACKLIGHT LCD.
 *         Inser��o da fun��o de deslocamento de mensagens.
 *
 *****************************************************************************/

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <delays.h>

/** D E F I N E S ************************************************************/
#define D4       PORTDbits.RD4      // Define a porta RD4 como D4
#define D5       PORTDbits.RD5      // Define a porta RD5 como D5
#define D6       PORTDbits.RD6      // Define a porta RD6 como D6
#define D7       PORTDbits.RD7      // Define a porta RD7 como D7
#define RW       PORTDbits.RD3      // Define a porta RD3 como R/W
#define RS_PIN   PORTDbits.RD2     // Define a porta RD2 como RS_PIN
#define E_PIN    PORTDbits.RD1      // Define a porta RD3 como E_PIN
#define BCK      PORTEbits.RE0      // Define a porta RE1 como BCK_LIGHT

/**  P R O T O T I P O S   P R I V A D O S ***********************************/

void envia_nibble_lcd(unsigned int dado);
void envia_byte_lcd(unsigned char endereco, unsigned int dado);
void limpa_lcd(void);
void inicializa_lcd(void);
void escreve_lcd(unsigned char c);
void putsXLCD(char *buffer);
void putrsXLCD(const rom char *buffer);
void Delay_ms(unsigned int z);
void Delay_us(unsigned int z);
void Delay_seg(unsigned int z);
void Delay_s(unsigned int z);
void desloca_mensagem_direita (char texto[],char linha);
void desloca_mensagem_esquerda(char texto[],char linha);
void Liga_BCKLIGHT (unsigned int mode);
void caracter_esp(void);
/**  VARI�VEIS GLOBAIS ***********************************/

unsigned char msgem[17];
unsigned char Smile[8]={0x0E,0x1D,0x1F,0x1F,0x18,0x1F,0x1F,0x0E};

int i;

/** F U N C O E S ************************************************************/

/******************************************************************************
 * Funcao:      void envia_nibble_lcd(unsigned int dado)
 * Entrada:     dado: dado que ter� seu nibble inferior enviado ao display LCD
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Esta rotina l� o "Nibble" inferior de uma vari�vel e envia para o LCD.
 *****************************************************************************/
void envia_nibble_lcd(unsigned int dado)
{
 if(dado&0b00000001)D4=1;
 else D4=0;
 if(dado&0b00000010)D5=1;
 else D5=0;
 if(dado&0b00000100)D6=1;
 else D6=0;
 if(dado&0b00001000)D7=1;
 else D7=0;

 E_PIN = 1;
 Delay1TCY();//Aguarda 200nS
 Delay1TCY();//Aguarda 200nS
 Delay1TCY();//Aguarda 200nS
 Delay1TCY();//Aguarda 200nS
 Delay1TCY();//Aguarda 200nS
 E_PIN = 0;

 return;
}

/******************************************************************************
 * Funcao:      void envia_byte_lcd(unsigned char endereco, unsigned int dado)
 * Entrada:     endere�o: indica se o dado enviado ser� uma instru��o ou caractere
 *              dado: dado ou instru��o que ser� enviado ao display LCD
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Esta rotina ir� enviar um dado ou um comando para o LCD conforme abaixo:
 *              ENDERE�O = 0 -> a vari�vel DADO ser� uma instru��o
 *              ENDERE�O = 1 -> a vari�vel DADO ser� um caractere
 *****************************************************************************/
void envia_byte_lcd(unsigned char endereco, unsigned int dado)
{
 if(endereco == 0)RS_PIN = 0;
 else RS_PIN = 1;
 Delay100TCYx(5);//Aguarda 100uS
 E_PIN = 0;
 envia_nibble_lcd(dado>>4);
 envia_nibble_lcd(dado & 0x0f);
 Delay100TCYx(2);//Aguarda 40uS
 return;
}

/******************************************************************************
 * Funcao:      void limpa_lcd(void)
 * Entrada:     Nenhuma (void)
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Esta rotina limpa o LCD.
 *****************************************************************************/
void limpa_lcd(void)
{
 envia_byte_lcd(0,0x01); // Envia instru��o para limpar o LCD
 Delay10KTCYx(1); // Aguarda 2ms para estabilizar o LCD
 return; // Retorna ao ponto de chamada da fun��o
}

/******************************************************************************
 * Funcao:      void inicializa_lcd(void)
 * Entrada:     Nenhuma (void)
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Configura os parametros de comunica��o
 *              com o display, tais como:
 *               - modo de opera��o (4 ou 8 data bits)
 *               - tipo de cursor
 *               - tamanho da fonte
 *****************************************************************************/
void inicializa_lcd(void)
{
 
 RW = 0;
 D4 = 0;                // Garante que o pino DB4 est�o em 0 (low)
 D5 = 0;                // Garante que o pino DB5 est�o em 0 (low)
 D6 = 0;                // Garante que o pino DB6 est�o em 0 (low)
 D7 = 0;                // Garante que o pino DB7 est�o em 0 (low)
 RS_PIN = 0;            // Garante que o pino RS est�o em 0 (low)
 E_PIN=0;               // Garante que o pino ENABLE est�o em 0 (low)
 Delay10KTCYx(75);      // Aguarda 15ms para estabilizar o LCD
 envia_nibble_lcd(0x03);// Envia comando para inicializar o display
 Delay10KTCYx(25);      // Aguarda 5ms para estabilizar o LCD
 envia_nibble_lcd(0x03);// Envia comando para inicializar o display
 Delay10KTCYx(25);      // Aguarda 5ms para estabilizar o LCD
 envia_nibble_lcd(0x03);// Envia comando para inicializar o display
 Delay10KTCYx(25);      // Aguarda 5ms para estabilizar o LCD
 envia_nibble_lcd(0x02);// CURSOR HOME - Envia comando para zerar o contador de
                        // caracteres e retornar � posi��o inicial (0x80).
 Delay10KTCYx(5);       // Aguarda 1ms para estabilizar o LCD
 envia_byte_lcd(0,0x28);// FUNCTION SET - Configura o LCD para 4 bits,
                        // 2 linhas, fonte 5X7.
 envia_byte_lcd(0,0x0c);// DISPLAY CONTROL - Display ligado, sem cursor
 limpa_lcd();           // Limpa o LCD
 envia_byte_lcd(0,0x06);// ENTRY MODE SET - Desloca o cursor para a direita


 return;                // Retorna ao ponto de chamada da fun��o

}

/******************************************************************************
 * Funcao:      void escreve_lcd(unsigned char c)
 * Entrada:     c = caractere que ser� enviado ao LCD
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Esta rotina envia um caractere ao LCD.
 *****************************************************************************/
void escreve_lcd(unsigned char c)
{
envia_byte_lcd(1,c);
}

/******************************************************************************
 * Funcao:      void putrsXLCD(const rom char *buffer)
 * Entrada:     *buffer: apontador para string na memoria de programa(ROM)
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Essa rotina copia uma string terminada com caracter nulo
 *              da memoria de programa do microcontrolador para o display de LCD.
 *              A string apontada por *buffer � copiada no display a partir da
 *              posi��o da memoria RAM do display escolhida com a fun��o
 *              envia_byte_lcd().
 *****************************************************************************/
void putrsXLCD(const rom char *buffer)
{
        while(*buffer)                  // escreve dados no LCD ate achar caracter nulo
        {
                escreve_lcd(*buffer); // escreve caracter no LCD
                buffer++;               // incrementa apontador
        }
}

/******************************************************************************
 * Funcao:      void putsXLCD(char *buffer)
 * Entrada:     *buffer: apontador para string na memoria de dados(RAM)
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Essa rotina copia uma string terminada com caracter nulo
 *              da memoria de dados do microcontrolador para o display de LCD.
 *              A string apontada por *buffer � copiada no display a partir da
 *              posi��o da memoria RAM do display escolhida com a fun��o
 *              envia_byte_lcd().
 *****************************************************************************/
void putsXLCD(char *buffer)
{
        while(*buffer)                  // escreve dados no LCD ate achar caracter nulo
        {
                escreve_lcd(*buffer); // escreve caracter no LCD
                buffer++;               // incrementa apontador
        }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Funcao:      Delay_ms
 * Entrada:     *z , valor de 0 a 65535.
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Essa fun��o � capaz de gerar 65535ms,de temporiza��o.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Delay_ms(unsigned int z)
{
    int y;
    for(y=0;y<z;y++)
    {
         Delay100TCYx(120);

    }
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Funcao:      Delay_us
 * Entrada:     *z , valor de 0 a 65535.
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Essa fun��o � capaz de gerar 65535us,de temporiza��o.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  void Delay_us(unsigned int z)
{
    int y;
    z=z/2;
    for(y=0;y<z;y++)
    {
        Delay1TCY();Delay1TCY();Delay1TCY();
        Delay1TCY();Delay1TCY();Delay1TCY();
        Delay1TCY();Delay1TCY();Delay1TCY();
        Delay1TCY();Delay1TCY();Delay1TCY();
    }
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Funcao:      Delay_seg
 * Entrada:     *z , valor de 0 a 65535.
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Essa fun��o � capaz de gerar 65535s,de temporiza��o.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Delay_seg(unsigned int z)
{
    int y;
    for(y=0;y<z;y++)
    {
        Delay10KTCYx(250);
        Delay10KTCYx(250);
        
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Funcao:      desloca_mensagem_direita
 * Entrada:     Texto (frase m�xima de 16 caracteres,e a linha do Display 
 *              que ser� apresentada a informa��o.
 *              1- Primeira Linha  , 2- Segund alinha.  
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Essa fun��o � capaz de gerar o deslocamento de mensagens no
 *              LCD.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void desloca_mensagem_direita (char texto[],char linha)
{

int dados;
int cont;
int inicio;
int final;
unsigned char bufferA[17]={" "};
unsigned char *ponteiro={""};
sprintf(bufferA,texto);
ponteiro=bufferA;



if (linha==1) {dados=0x80;}
if (linha==2) {dados=0xC0;}

for (cont=16;cont>=0;cont--) 

{
                       
                sprintf(msgem,"%s",ponteiro+cont);
                envia_byte_lcd(0,dados);
                putsXLCD(msgem);
                Delay_ms(150);
               
                
}

Delay_seg(1);


}

 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Funcao:      desloca_mensagem_esquerda
 * Entrada:     Texto (frase m�xima de 16 caracteres,e a linha do Display 
 *              que ser� apresentada a informa��o.
 *              1- Primeira Linha  , 2- Segund alinha.  
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Essa fun��o � capaz de gerar o deslocamento de mensagens no
 *              LCD.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void desloca_mensagem_esquerda(char texto2[],char linha)
{

int dados;
int inicio;
int final;
unsigned char bufferB[17]={"  "};
unsigned char *ponteiro={""};
sprintf(bufferB,texto2);
ponteiro=bufferB;


if (linha==1) {inicio=0x80; final=0X8F;}
if (linha==2) {inicio=0xC0; final=0xCF;}
         
               for(dados=final;dados>=inicio;dados--)

               {    
                sprintf(msgem,"%s",ponteiro);
                envia_byte_lcd(0,dados);
                putsXLCD(msgem);
                Delay_ms(150);
               
                
               }
        

Delay_seg(1);
   
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Funcao:      Liga_BCKLIGHT
 * Entrada:     Mode : 1- Ligado - 0 Desligado.
 * Sa�da:       Nenhuma (void)
 * Descri��o:   Essa fun��o � capaz de ligar/Desligar o Backlight do LCD.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void Liga_BCKLIGHT (unsigned int mode)

{

    if (mode==0) BCK=0; // Desliga BACKLIGHT
    if (mode==1) BCK=1; // Liga BACKLIGHT



}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                            Caracter especial                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  void caracter_esp()
{

    unsigned char Smile[8]={0x0E,0x1D,0x1F,0x1F,0x18,0x1F,0x1F,0x0E};
    unsigned char Cruz [8]={0x04,0x04,0x1F,0x1F,0x04,0x04,0x04,0x04};
    
    RW=0;
    RS_PIN=0;

    Delay_us(5);

    envia_byte_lcd(0,0x40);

    Delay_us(5);

    RS_PIN=1;

    Delay_ms(5);

    for(i=0;i<=7;i++)
    {
        envia_byte_lcd(1,Smile[i]);
        Delay_ms(5);
    }

    for(i=0;i<=7;i++)
    {
        envia_byte_lcd(1,Cruz[i]);
        Delay_ms(5);
    }



     return;

}
