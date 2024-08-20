/*
 * Nombre: SysTick.c
 * Autor: Eduardo L. Blotta
 * Comentarios: Manejo de System Tick
 * Año 2021
 */
#include "SysTick.h"
// ----------------------------------------------------------------------------
// ----- SysTick_Handler() ----------------------------------------------------
// ----------------------------------------------------------------------------
void SysTick_Handler (void)
{
	HAL_IncTick();	//Incrementa Tick del sistema (usado por algunas funciones de HAL Driver)
	SysTickHook (); // Función para colgarse de la interrupción del System Tick
}

void SysTick_Init(uint16_t Frec_Ticks_Hz){
	SysTick_Config (SystemCoreClock / Frec_Ticks_Hz);
}
