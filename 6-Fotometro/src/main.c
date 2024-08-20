// ----------------------------------------------------------------------------
// Laboratorio Nº6: Fotometro.
// ----------------------------------------------------------------------------

// Agregue sus Includes aqui //
#include "ICTimer.h"
#include "SysTick.h"
#include "Salidas.h"

#define SYSTICK_FREQ_HZ 1000		//Frecuencia del System Tick. Cada 1ms tengo una interrupcion.

// ----------------------------------------------------------------------------
// Definicion de Constantes NTC
#define R (float) 10000			// Valor de la resistencia del timer 555
#define C (float) 0.00001		// Valor del capacitor del timer 555
#define ModuloContador (float) (2^32)-1		// Defino el modulo del contador de 32 bits. [(2^32)-1]
// ----------------------------------------------------------------------------
/* Variables globales */

uint16_t frec=0;		// Frecuencia calculada del "VCO" 555 va a ser entre 10 y 2000.
uint8_t F1=0;			// Digito 1
uint8_t F2=0;			// Digito 2
uint8_t F3=0;			// Digito 3
uint8_t F4=0;			// Digito 4

// ----- main() ---------------------------------------------------------------
int main(void)
{
	/* Conf. Variables */

	/* System Tick Init */
	SysTick_Init (SYSTICK_FREQ_HZ);		// Para hacer el multiplexado del display.

	/* Conf. Entradas */
	//No hay entradas...

	/* Conf. Salidas */
	__HAL_RCC_GPIOA_CLK_ENABLE();	// Habilito el Clock del puerto A. (donde entra la frecuencia del 555)
	__HAL_RCC_GPIOC_CLK_ENABLE(); // Enable GPIOC Peripheral clock	(Para seleccionar los digitos del display)
	__HAL_RCC_GPIOD_CLK_ENABLE(); // Enable GPIOD Peripheral clock	(Para seleccionar los segmentos del display)
	__HAL_RCC_TIM2_CLK_ENABLE();	  // Habilito el Timer 3. (el que me genera las interrupciones del ADC)
	Out_Init(GPIOC, SEL_MUX_0|SEL_MUX_1);	// Defino como salidas las entradas al decodificador.
												// Defino como salidas cada segmento de los displays.
	Out_Init(GPIOD, SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f|SEGMENTO_g|SEGMENTO_PD);
	// INICIALIZACION
	Init_ICTimer();				// Dentro de esta funcion CONFIGURO GPIOA, TIM2, Preescalers...


	while(1){

													//frec=2153
		F1=(uint8_t)frec/1000;						//F1=2
		F2=(uint8_t)(frec/100)-10*F1;				//F2=1
		F3=(uint8_t)(frec/10 -100*F1-10*F2);		//F3=5
		F4=(uint8_t)(frec -1000*F1 -100*F2 -10*F3);	//F4=3
		HAL_Delay(500);
	}
}
// ----------------------------------------------------------------------------
void SysTickHook (void) // Para colgarse del System Tick
{
	static uint8_t BufDisp[4] = {0,0,0,0};
	static uint8_t Digito =0;		// digito se inicializa en 0. Como se definio static
										// el programa no vuelve a pasar por aca. No vuelve a hacerse 0 cada
										// vez que pasa.

	//Conversion BCD -> 7 segmentos
	const uint32_t BIN7SEG[] = {
			SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f,	// Cero
			SEGMENTO_b|SEGMENTO_c,												// Uno
			SEGMENTO_a|SEGMENTO_b|SEGMENTO_g|SEGMENTO_e|SEGMENTO_d,				// Dos
			SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_g,				// Tres
			SEGMENTO_f|SEGMENTO_b|SEGMENTO_g|SEGMENTO_c,						// Cuatro
			SEGMENTO_a|SEGMENTO_f|SEGMENTO_g|SEGMENTO_c|SEGMENTO_d,				// Cinco
			SEGMENTO_f|SEGMENTO_e|SEGMENTO_d|SEGMENTO_c|SEGMENTO_g,				// Seis
			SEGMENTO_a|SEGMENTO_b|SEGMENTO_c,									// Siete
			SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f|SEGMENTO_g,	// Ocho
			SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_f|SEGMENTO_g};			// Nueve

		// Escriba su codigo aqui

	BufDisp[0]=F1;
	BufDisp[1]=F2;
	BufDisp[2]=F3;
	BufDisp[3]=F4;

	if(Digito == 4){
		Digito= 0;
	}

	switch(Digito){		// display organizado como: D3.D2.D1.D0
		case 0:
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_0, GPIO_PIN_RESET);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_0, GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_0, GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_0, GPIO_PIN_SET);
			break;
		}

	//	Out_Init(GPIOD, SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f|SEGMENTO_g|SEGMENTO_PD);
	HAL_GPIO_WritePin(GPIOD, SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f|SEGMENTO_g|SEGMENTO_PD , GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOD, BIN7SEG[BufDisp[-Digito+3]], GPIO_PIN_SET);	//Enciendo los segmentos correspondientes.
	Digito=Digito+1;

}

void ICTimerHook (uint32_t CapturedValue)
{
	static uint32_t Nactual =0;	//	Numero del contador actual
	static uint32_t Nanterior =0;	//	Numero del contador anterior
	static uint32_t Nciclos = 0;	//	Numero del contador anterior

	Nactual=CapturedValue;

	if(Nactual>Nanterior){
		Nciclos=Nactual-Nanterior;
		frec=2000000/Nciclos;
	}
	else if(Nactual<Nanterior){		//Significa que el contador se reseteó. el contador es de 32 bits.
		Nciclos=(ModuloContador-Nanterior)+Nactual;
		frec=2000000/Nciclos;
	}
	else{							//(Nactual==Nanterior)
		frec=0;
	}

	Nanterior=Nactual;
}
