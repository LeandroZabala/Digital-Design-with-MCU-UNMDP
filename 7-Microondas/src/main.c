/* Programa para controlar un porton corredizo */
/* Integración de la maquina de estados creada por Yakindu*/
/**********************************************************/
//Agregue todos los includes necesarios
//para los perifericos que use.
#include "Microondas.h"
#include "Microondas_required.h"
#include "sc_timer_service.h"

// Agregue sus includes aqui
#include "Entradas.h"
#include "SysTick.h"
#include "Salidas.h"
#include "Buzzer.h"
#include  "CommonIO.h"

#define SYSTICK_FREQ_HZ 1000		//Frecuencia del System Tick. Cada 1ms tengo una interrupcion.

Microondas ls; // Estructura de datos de State Machine
#define MAX_TIMERS 10 // Lugar para los Timers de la StateChart (por defecto)
static sc_timer_t timers[MAX_TIMERS];
static sc_timer_service_t timer_service;
uint8_t UpdateTimerFlag=false;

extern uint16_t vStopBuzzer;

int main(void)
{
	SysTick_Init (SYSTICK_FREQ_HZ);		// Para hacer el multiplexado del display.

	/* Inicialice sus Perifericos AQUI*/
	/* Conf. Entradas */
	__HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA Peripheral clock
	In_Init(GPIOA, SW1|SW2|SW3);
	/* Conf. Salidas */
	__HAL_RCC_GPIOC_CLK_ENABLE(); // Enable GPIOC Peripheral clock	(Para seleccionar los digitos del display)
	__HAL_RCC_GPIOD_CLK_ENABLE(); // Enable GPIOD Peripheral clock	(Para seleccionar los segmentos del display)
	__HAL_RCC_TIM2_CLK_ENABLE();	  // Habilito el Timer 2. (el que me genera las interrupciones del ADC)
	Buzzer_Init();
	Out_Init(GPIOC, SEL_MUX_0|SEL_MUX_1);	// Defino como salidas las entradas al decodificador.
	Out_Init(GPIOD, SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f|SEGMENTO_g|SEGMENTO_PD);


	/*Inicialización del servicio de timers de la StateChart*/


	sc_timer_service_init(&timer_service, timers, MAX_TIMERS,
			(sc_raise_time_event_fp) &microondas_raise_time_event);
	/*Inicializa la maquina de estados*/
	microondas_init(&ls);
	/*Ingresa a la maquina de estados*/
	microondas_enter(&ls);
	while (1) // Loop infinito.
	{
		if (UpdateTimerFlag)	// TRUE?->Entonces se ejecuta un nuevo ciclo de la SC
		{
			// Manejo de Eventos de Entrada
			// Ej: Estado del SW1->Evento
			if ((GPIOA->IDR & SW1)==0)  // Estado de SW1?
				microondas_eSW1_raise_press (&ls);
			else
				microondas_eSW1_raise_nopress (&ls);

			if ((GPIOA->IDR & SW2)==0)  // Estado de SW1?
				microondas_eSW2_raise_press (&ls);
			else
				microondas_eSW2_raise_nopress (&ls);

			if ((GPIOA->IDR & SW3)==0)  // Estado de SW1?
				microondas_eSW3_raise_press (&ls);
			else
				microondas_eSW3_raise_nopress (&ls);


			sc_timer_service_proceed(&timer_service, 1/*MS*/); // Incrementa el timer de la SC.
			microondas_run_cycle(&ls);	// Se ejecuta un ciclo de la StateChart.
			UpdateTimerFlag=false;
		}
		//__WFI(); // Wait for Interrupt (se va a dormir)
	}
}

// ----------------------------------------------------------------------------
void SysTickHook (void) // Para colgarse del System Tick
{
	/*Sincronizacion del timer del micro con los timers de la maquina de estados*/
		UpdateTimerFlag=true;
		if (vStopBuzzer>0){
			vStopBuzzer--; //para ir decrementando el contador de duracion del buzzer. cada vez que entra resta 1ms
			if (vStopBuzzer==0){
				stop_Buzzer();
			}
		}
}


void microondas_selector(const Microondas* handle,uint8_t digito  )
{
	switch(digito){		// display organizado como: D3.D2.D1.D0
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
}
void microondas_display(const Microondas* handle, uint8_t numero)
{
	// Tabla de Conversion BCD -> 7 segmentos
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

	};

	HAL_GPIO_WritePin(GPIOD, SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f|SEGMENTO_g|SEGMENTO_PD , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, BIN7SEG[numero], GPIO_PIN_SET);	//Enciendo los segmentos correspondientes.

	}

void microondas_displayapagado(const Microondas* handle, uint8_t dispoff)
{
	if (dispoff==1){
		HAL_GPIO_WritePin(GPIOD, SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f|SEGMENTO_g|SEGMENTO_PD , GPIO_PIN_RESET);
	}

}

void microondas_decimalPoint (const Microondas* handle, uint8_t dp  )
{
	if(dp==1){
		HAL_GPIO_WritePin(GPIOD, SEGMENTO_PD , GPIO_PIN_SET);
	}
	else
		HAL_GPIO_WritePin(GPIOD, SEGMENTO_PD , GPIO_PIN_RESET);
}

void microondas_play(const Microondas* handle, uint16_t periodo, uint16_t duracion)
{
	play_Buzzer(periodo, duracion);
}
// Continue aqui
// ...

//! callback implementation for the setting up time events
void microondas_set_timer(Microondas* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic)
{
    sc_timer_set(&timer_service, handle, evid, time_ms, periodic);
}
//
////! callback implementation for canceling time events.
void microondas_unset_timer(Microondas* handle, const sc_eventid evid)
{
    sc_timer_unset(&timer_service, evid);
}
