// ----------------------------------------------------------------------------
// Laboratorio Nº2: EntradasSalidas.
// ----------------------------------------------------------------------------
// Agregue sus Includes aqui.
#include "SysTick.h"
#include "Salidas.h"
#define SYSTICK_FREQ_HZ 1000		//Frecuencia del System Tick
// ----- main() ---------------------------------------------------------------
int main(void)
{
	/* System Tick Init */
	SysTick_Init (SYSTICK_FREQ_HZ);
	/* Conf. Entradas */
	__HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA Peripheral clock
	In_Init(GPIOA, SW1|SW2|SW3);
	/* Conf. Salidas */
	__HAL_RCC_GPIOD_CLK_ENABLE(); // Enable GPIOD Peripheral clock
	Out_Init(GPIOD, LED_ROJO_A|LED_AMARILLO_A|LED_VERDE_A);

	/*Todos los leds Off*/
	HAL_GPIO_WritePin(GPIOD, LED_ROJO_A|LED_AMARILLO_A|LED_VERDE_A, GPIO_PIN_RESET);

	while (1) // // Loop infinito.
	{
		if(HAL_GPIO_ReadPin(GPIOA, SW1)==0){	// Detecta el pulsador apretado
			HAL_Delay(30);
			if(HAL_GPIO_ReadPin(GPIOA,SW1)==0){
				HAL_GPIO_TogglePin(GPIOD,LED_ROJO_A);
			}
			while(HAL_GPIO_ReadPin(GPIOA, SW1)==0){
			}

		}
		if(HAL_GPIO_ReadPin(GPIOA, SW2)==0){	// Detecta el pulsador apretado
			HAL_Delay(30);
			if(HAL_GPIO_ReadPin(GPIOA,SW2)==0){
				HAL_GPIO_TogglePin(GPIOD,LED_AMARILLO_A);
			}
			while(HAL_GPIO_ReadPin(GPIOA, SW2)==0){
			}

		}
		if(HAL_GPIO_ReadPin(GPIOA, SW3)==0){	// Detecta el pulsador apretado
			HAL_Delay(30);
			if(HAL_GPIO_ReadPin(GPIOA,SW3)==0){
				HAL_GPIO_TogglePin(GPIOD,LED_VERDE_A);
			}
			while(HAL_GPIO_ReadPin(GPIOA, SW3)==0){
			}

		}
	}
}




// ----------------------------------------------------------------------------
void SysTickHook (void) // Para colgarse del System Tick
{
	// Escriba su codigo aqui
}
