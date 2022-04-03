#include <p18F4520.h>         // ESSA LINHA DEVE SER INSERIDO O DISPOSITIVO.
#include <delays.h>
//#include <xc.h>
//#define _XTAL_FREQ 20000000
#define pino_dados PORTAbits.RA4
#define direcao TRISAbits.TRISA4

int i=0;
void pulso_reset();
void pulso_presenca();
void escreve_zero();
void escreve_um();
char le_bit();
char le_byte();
void escreve_comando(char comando);
void configura_precisao(char precisao);
int le_temperatura();


void main (void)

{

}

void pulso_reset()
{
direcao=0; // pino como saida
pino_dados=0; // nivel baixo
Delay10TCYx(50);
pino_dados=1; // nivel alto
Delay10TCYx(50);

}
void pulso_presenca()
{
	direcao=1; // pino entrada
	while (pino_dados==1); // laço  enquanto o pulso nivel baixo nao for detectado
	while (pino_dados==0); // laço  enquanto o pulso nivel alto nao for detectado
	Delay10TCYx(50);
	pino_dados=1; // nivel alto
	Delay10TCYx(50);

}
void escreve_zero()

{
direcao=0;// pino com saida
pino_dados=0;//nivel baixo
	Delay10TCYx(90);
pino_dados=1;//nivel alto
	Delay10TCYx(90);

}
void escreve_um()

{
direcao=0;// pino com saida
pino_dados=0;//nivel baixo
Delay10TCYx(5);
pino_dados=1;//nivel alto
Delay10TCYx(5);

}
char le_bit()

{
	char _bit=0;
	direcao=0; // pino com saida
	pino_dados=0; // nivel baixo
	Delay10TCYx(3);
	direcao=1; // pino com entrada
	Delay10TCYx(3);
	_bit=pino_dados;
	Delay10TCYx(90);
	return _bit;
}
char le_byte()

{
	char _byte=0;
	for (i=0;i<8;i++)
	{
		_byte=_byte || (le_bit()<<i);
	}
	return _byte;
}
void escreve_comando(char comando)
{
	for (i=0;i<8;i++)
	{
		if (comando & 0x01)
		{
			direcao=0;// pino com saida
			pino_dados=0;//nivel baixo
			Delay10TCYx(5);
			pino_dados=1;//nivel alto
			Delay10TCYx(5);
		}
		else 
		{
			direcao=0;// pino com saida
			pino_dados=0;//nivel baixo
			Delay10TCYx(90);
			pino_dados=1;//nivel alto
			Delay10TCYx(90);
			comando=comando>>1;
		}	
	}
	Delay10TCYx(10);
}


void configura_precisao(char precisao)
{
	char valor=0;
	switch (precisao)
	{
		case 9:
		valor=0;
		break;
		case 10:
		valor=32;
		break;
		case 11:
		valor=64;
		break;
		case 12:
		valor=96;
		break;
		default:
		valor=96;
	}
	 
	pulso_reset(); // envia reset
	pulso_presenca(); // detecta presenca de sensor

	//envia_comando(0xCC); //Skip ROM
	
	escreve_comando(0x4E);//comando de configuração
	escreve_comando(0x00);//registradores de alarme
	escreve_comando(0x00);//registradores de alarme
	escreve_comando(valor);//comando de configuração*/
}

//int le_temperatura()
//
//{
//	int temperatura=0;
//	pulso_reset(); // envia reset
//	pulso_presenca(); // detecta presenca de sensor
//
//	//envia_comando(0xCC); //Skip ROM
//
//	escreve_comando(0x44);// Faz a Leitura do sensor de temperatura
//	Delay10TCYx(800);
//
//	escreve_comando(0xBE); // Lê toda a memória
//	temperatura=le_byte();
//	temperatura=temperatura | (int)(le_byte()<<8);
//	
//	pulso_reset(); // envia reset
//	pulso_presenca(); // detecta presenca de sensor
//	return temperatura;
//
//}