/* Programa para controlar un porton corredizo */
/* Integración de la maquina de estados creada por Yakindu*/
/**********************************************************/
//Agregue todos los includes necesarios
//para los perifericos que use.
#include "Control.h"
#include "sc_timer_service.h"

Control ls; // Estructura de datos de State Machine

#define MAX_TIMERS 4 // Lugar para los Timers de la StateChart (por defecto)
static sc_timer_t timers[MAX_TIMERS];
static sc_timer_service_t timer_service;
uint8_t UpdateTimerFlag=false;
int main(void)
{
	/* Inicialice sus Perifericos AQUI*/
	// ...
	/*Inicialización del servicio de timers de la StateChart*/
	sc_timer_service_init(&timer_service, timers, MAX_TIMERS,
						  (sc_raise_time_event_fp) &control_raise_time_event);
	/* System Tick Init */
	SysTick_Config (/*Programe su SysTick*/);
	/*Inicializa la maquina de estados*/
	control_init(&ls);
	/*Ingresa a la maquina de estados*/
	control_enter(&ls);
	while (1) // Loop infinito.
		{
		if (UpdateTimerFlag)	// TRUE?->Entonces se ejecuta un nuevo ciclo de la SC
		{
		// Manejo de Eventos de Entrada
			// Ej: Estado del SW1->Evento
			if ((GPIOA->IDR & SW1)==0)  // Estado de SW1?
				control_eSW1_raise_press (&ls);
			else
				control_eSW1_raise_nopress (&ls);
			// Continúe aquí
			// ...
			sc_timer_service_proceed(&timer_service, /*MS*/); // Incrementa el timer de la SC.
			control_run_cycle(&ls);	// Se ejecuta un ciclo de la StateChart.
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
}


//Definicion de funciones de acciones (operaciones)
void control_aledR_on(const Control* handle)
{
	HAL_GPIO_WritePin(GPIOD, LED_ROJO_A, GPIO_PIN_SET);
}
// Continue aqui
// ...

//! callback implementation for the setting up time events
void control_set_timer(Control* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic)
{
    sc_timer_set(&timer_service, handle, evid, time_ms, periodic);
}

//! callback implementation for canceling time events.
void control_unset_timer(Control* handle, const sc_eventid evid)
{
    sc_timer_unset(&timer_service, evid);
}
