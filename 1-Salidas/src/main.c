// ----------------------------------------------------------------------------
// Laboratorio Nº1: Salidas.
// ----------------------------------------------------------------------------
#include "SysTick.h"
#include "Salidas.h"
#define SYSTICK_FREQ_HZ 1000	//Frecuencia del System Tick
// ----- main() ---------------------------------------------------------------
int main(void)
{
	// System Tick Init
	SysTick_Init (SYSTICK_FREQ_HZ);
	/* Conf. Salidas */
	__HAL_RCC_GPIOD_CLK_ENABLE(); // Enable GPIOD Peripheral clock
	Out_Init(GPIOD, LED_ROJO_A|LED_AMARILLO_A|LED_VERDE_A|LED_ROJO_B|LED_AMARILLO_B|LED_VERDE_B);
	/*Todos los leds Off*/
	HAL_GPIO_WritePin(GPIOD, LED_ROJO_A|LED_AMARILLO_A|LED_VERDE_A|LED_ROJO_B|LED_AMARILLO_B|LED_VERDE_B, GPIO_PIN_RESET);
	while (1) // Loop infinito.
	{
	HAL_GPIO_WritePin(GPIOD, LED_ROJO_A, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, LED_VERDE_B, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOD, LED_ROJO_A, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED_VERDE_B, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOD, LED_AMARILLO_A, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, LED_AMARILLO_B, GPIO_PIN_SET);

	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOD, LED_AMARILLO_A, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED_AMARILLO_B, GPIO_PIN_RESET);


	HAL_GPIO_WritePin(GPIOD, LED_VERDE_A, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, LED_ROJO_B, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOD, LED_VERDE_A, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, LED_ROJO_B, GPIO_PIN_RESET);
	}

}

// ----------------------------------------------------------------------------
void SysTickHook (void) // Para colgarse del System Tick
{
	// Escriba su codigo aqui
}
