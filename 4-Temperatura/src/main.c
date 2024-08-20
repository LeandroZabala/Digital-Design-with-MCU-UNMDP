// ----------------------------------------------------------------------------
// Laboratorio N�4: Temperatura.
// ----------------------------------------------------------------------------

// Agregue sus Includes aqui //
#include <math.h>
#include "SysTick.h"
#include "Salidas.h"
#include "ConversorxInts.h"

#define SYSTICK_FREQ_HZ 1000		//Frecuencia del System Tick. Cada 1ms tengo una interrupcion.

// ----------------------------------------------------------------------------
// Definicion de Constantes NTC
#define B (float) 4050	// [K] Constante ecuacion NTC
#define To (float) 273		// [K] Constante ecuacion NTC
#define Ro (float) 34.71	// [Kohms] Constante ecuacion NTC
// ----------------------------------------------------------------------------
// Definicion de Constantes ADC
#define ResADC (float) 3/4095	// [V] Resolucion en Volts del ADC Vref/(2^n-1)

// ----------------------------------------------------------------------------
void Error_Handler(void);
// ----------------------------------------------------------------------------
/* Variables globales */
uint16_t lectura =0;	//	Tension que lee el ADC. comprendido entre 0 y 4095.
uint8_t T1=0;			// Digito 1
uint8_t T2=0;			// Digito 2
uint8_t T3=0;			// Digito 3

int main(void)
{
	/* Conf. Variables */
	float Vmedido = 0;
	float Rntc ;
	float T ;

	/* System Tick Init */
	SysTick_Init (SYSTICK_FREQ_HZ);

	/* Conf. Entradas */
	//No hay entradas...

	/* Conf. Salidas */
	__HAL_RCC_ADC1_CLK_ENABLE();	//Habilito el Clock del ADC1
	__HAL_RCC_GPIOA_CLK_ENABLE();	// Habilito el Clock del puerto A. (donde entra la señal analogica)
	__HAL_RCC_GPIOC_CLK_ENABLE(); // Enable GPIOC Peripheral clock
	__HAL_RCC_GPIOD_CLK_ENABLE(); // Enable GPIOD Peripheral clock
	__HAL_RCC_TIM3_CLK_ENABLE();	  // Habilito el Timer 3. (el que me genera las interrupciones del ADC)

	ADC1_TIM3_Init();				// Dentro de esta funcion CONFIGURO GPIOA, ADC1, TIM3
	HAL_ADC_Start_IT(&hADC1);		// Habilito las interrupciones que le genera el ADC al micro
	Out_Init(GPIOC, SEL_MUX_0|SEL_MUX_1);	// Defino como salidas las entradas al decodificador.
											// Defino como salidas cada segmento de los displays.
	Out_Init(GPIOD, SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f|SEGMENTO_g|SEGMENTO_PD);

	while(1){
		Vmedido=lectura*ResADC;
		Rntc=Vmedido*10000/(5-Vmedido);
		T=B*To/(To*log(Rntc/Ro)+B);
		T=10*T;						//Si era 21.5xxx pasa a ser 215.xxx
		T=(uint8_t)T;					//T=215
		T1=(uint8_t)T/100;				//T1=2
		T2=(uint8_t)(T/10)-10*T1;		//T2=1
		T3=(uint8_t)(T-100*T1-10*T2);	//T3=5
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
			SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_f|SEGMENTO_g,				// Nueve
			SEGMENTO_a|SEGMENTO_f|SEGMENTO_b|SEGMENTO_g};						// Grado Centigrado
	//-----------------------------------------------------------------------------

BufDisp[0]=T1;
BufDisp[1]=T2;
BufDisp[2]=T3;
BufDisp[3]=0;


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
	if(Digito==0){
		HAL_GPIO_WritePin(GPIOD, BIN7SEG[10], GPIO_PIN_SET);}	// Enciendo el simbolo de grados centigrados
	else if (Digito==2){
		HAL_GPIO_WritePin(GPIOD, BIN7SEG[BufDisp[-Digito+3]]|SEGMENTO_PD, GPIO_PIN_SET);
		}
	else{
		HAL_GPIO_WritePin(GPIOD, BIN7SEG[BufDisp[-Digito+3]], GPIO_PIN_SET);		// Si quiero encender cualquier otro digito que no
		}																			//  no le prendo el punto decimal



Digito=Digito+1;
}


void ADCIRQHook (uint16_t ConversorAD)
{
	lectura=ConversorAD;
}

void Error_Handler(void)
{
  /* Trap for error*/
  while(1)
  {
  }
}
