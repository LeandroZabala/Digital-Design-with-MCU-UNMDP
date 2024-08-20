/**
 ************************************************************************************************************************************************************
 * Proyecto: "Implementacion de un filtro FIR de audio por DSP".
 * Autor del proyecto: Ezequiel Porto.
 * Basado en el proyecto "Implementacion de un filtro de audio" de Diego Sebastian Comas.
 *
 * Archivo: "main.c"
 * Fecha: 30/09/2019
 * Comentario: Programa Principal.
 ******************************************************************************
 */

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ---------DECLARACION DE INCLUSIONES:
#include "MiAudio.h"
#include <Audio_3.h> // Indica cual archivo de audio usar!
#include "SysTick.h"
#include "Salidas.h"
#include "Entradas.h"


#define SYSTICK_FREQ_HZ 1000		// Frecuencia del System Tick. Cada 1ms tengo una interrupcion.
									// Se usa para el manejo de los botones (rebote)

// ----------------------------------------------------------------------------
// ---------DECLARACION DE FUNCIONES:
static void SystemClock_Config(void);
void Error_Handler(void);

// ----------------------------------------------------------------------------
// ---------DEFINICION DE VARIABLES GLOBALES:
Tipo_Modo_AUDIO modo_AUDIO = PLAY; // Modo de reproduccion. Comienza detenido.
Tipo_Estado_Buffer Estado_BUFFER = HECHO; // Flag de estado de transferencia al buffer.
Tipo_Estado_Filtro modo_FILTRO=OFF; // Estado del filtro. Comienza en OFF.
uint32_t nMuestras; // La cantidad de muestras del archivo de audio!
uint32_t nRestantes; // La cantidad de datos que aun no fueros reproducidos.
uint16_t Audio_Buffer[4*BLOCK_SIZE]; // Tamaño de Audio_Buffer=2*auxBuffer bytes
/* El Buffer de RAM Audio_Buffer almacena muestras de sonido ESTEREO. Para generar el segundo canal
 * se repite la misma muestra dos veces, una a continuacion de la otra.
 * El Buffer de RAM se va a procesar en mitades. Y para completar una mitad, se necesita
 * un cuarto del tamaño total de muestras MONO. */



// ----------------------------------------------------------------------------
// ---------PROGRAMA PRINCIPAL:
int main(void)
{
	SystemClock_Config(); 	// Configura el clock a 168 MHz:
	/* System Tick Init */
	SysTick_Init (SYSTICK_FREQ_HZ);
	// ---------------------------------------------
	/* Conf. Entradas */
	__HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA Peripheral clock
	In_Init(GPIOA, SW1|SW3);

	/* Conf. Salidas */
		__HAL_RCC_GPIOD_CLK_ENABLE(); // Enable GPIOD Peripheral clock	Para LEDS.
		Out_Init(GPIOD, LED_ROJO_A|LED_AMARILLO_A|LED_VERDE_A);

	// ---Seteos iniciales
	/*Todos los leds Off*/
		HAL_GPIO_WritePin(GPIOD, LED_ROJO_A|LED_AMARILLO_A|LED_VERDE_A, GPIO_PIN_RESET);

	FIR_Init(); // Inicializo el filtro FIR.
	/* Como el audio está en 16 bits (2 bytes), la cantidad de muestras de audio
	 * es el tamaño en bytes dividido 2. */
	nMuestras=(uint32_t)(sizeof(Audio_File)/2); // "sizeof" da el tamaño en bytes.
	nRestantes=nMuestras; // Inicializa contador de muestras restantes a reproducir.
	// ---------------------------------
	while (1)	// --- Lazo infinito ---
	{	// Modo de audio: manejado x el usuario con SW1.


		while (modo_AUDIO != PLAY)
		{
		// El dispositivo está detenido o en pausa.
			//Prendo los Leds Correspondientes
			HAL_GPIO_WritePin(GPIOD, LED_VERDE_A, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED_ROJO_A, GPIO_PIN_SET);
		}
		// El usuario comienza/reanuda la reproduccion.
		// Inicializa el driver de audio antes de reproducir.
		BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE,VOLUMEN,FRECUENCIA_MUESTREO);
		// Copia o filtra una porcion del audio al buffer.
		/* LA 1º VEZ COPIA O FILTRA EL BUFFER COMPLETO (DOS MITADES).
		 * Luego, dentro del lazo copia por mitades. */
		// Se procesa (copia o filtra) la 1º mitad de un bloque de datos de audio en [ROM] a buffer en [RAM].
		Procesar_Buffer((uint16_t*)Audio_File+(nMuestras-nRestantes), Audio_Buffer, BLOCK_SIZE);
		// Se procesa (copia o filtra) la 2º mitad de un bloque de datos de audio en [ROM] a buffer en [RAM].
		Procesar_Buffer((uint16_t*)Audio_File+(nMuestras-nRestantes), &Audio_Buffer[2*BLOCK_SIZE], BLOCK_SIZE);
		BSP_AUDIO_OUT_Play(Audio_Buffer, sizeof(Audio_Buffer)); // Ordena reproducir nMuestras de audio.
		while (modo_AUDIO == PLAY) // ->LAZO DE REPRODUCCION:
		{
			// mientras está en "PLAY"
			//Prendo los Leds Correspondientes
			HAL_GPIO_WritePin(GPIOD, LED_ROJO_A, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, LED_VERDE_A, GPIO_PIN_SET);

			if(Estado_BUFFER == MITAD)	// Terminó de transferir la primera mitad del Buffer!
			{	// Renueva la primera mitad del buffer:
				Procesar_Buffer((uint16_t*)Audio_File+(nMuestras-nRestantes), Audio_Buffer, BLOCK_SIZE);
				Estado_BUFFER = HECHO; 	// COPIA CONCLUIDA
			}
			if(Estado_BUFFER == TODO)	// Terminó de transferir la segunda mitad del buffer!
			{	// Renueva la segunda mitad del buffer:
				Procesar_Buffer((uint16_t*)Audio_File+(nMuestras-nRestantes), &Audio_Buffer[2*BLOCK_SIZE], BLOCK_SIZE);
				Estado_BUFFER = HECHO; 	// COPIA CONCLUIDA
			}
		} // ---SALIDA DEL LAZO DE REPRODUCCION (Porque se presionó SW1):
		BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW); // Detiene el audio.
	}
}
//------------------------------------------------------
void SysTickHook (void) // Para colgarse del System Tick
{

//------------------------------------------------------------------------------
//		CONTROL DE SW1 PLAY-STOP
//------------------------------------------------------------------------------
	static uint8_t  contaSW1=0;
	static uint8_t flag2_SW1=0;
	static uint8_t flag_SW1=0;	//

if(HAL_GPIO_ReadPin(GPIOA,SW1)==0){
	flag_SW1=1;
}


if(flag_SW1==1){
	if (contaSW1>=30){
		if(flag2_SW1==1){
			if(HAL_GPIO_ReadPin(GPIOA,SW1)==1){			// Detecta que se solto el pulsador
				contaSW1=0;
				//toggleo variable
				if(modo_AUDIO == PLAY){
					modo_AUDIO=STOP;
				}else {
					modo_AUDIO=PLAY;
				}
				flag_SW1=0;
				flag2_SW1=0;

			}
		}
		else{
			if(HAL_GPIO_ReadPin(GPIOA,SW1)==0){			// Detecta el pulsador apretado
				flag2_SW1=1;
			}
			else{
				contaSW1=0;
				flag_SW1=0;

			}
			}
		}


	else{
		contaSW1++;
	}
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//		CONTROL DE SW3 FILTRO ON - FILTRO OFF
//------------------------------------------------------------------------------
static uint8_t  contaSW3=0;
static uint8_t flag2_SW3=0;
static uint8_t flag_SW3=0;

if(HAL_GPIO_ReadPin(GPIOA,SW3)==0){
	flag_SW3=1;
}


if(flag_SW3==1){
	if (contaSW3>=30){
		if(flag2_SW3==1){
			if(HAL_GPIO_ReadPin(GPIOA,SW3)==1){			// Detecta que se solto el pulsador
				contaSW3=0;
				//toggleo variable
				if(modo_FILTRO == ON){
					modo_FILTRO=OFF;
					HAL_GPIO_WritePin(GPIOD, LED_AMARILLO_A, GPIO_PIN_RESET);
				}else {
					modo_FILTRO=ON;
					HAL_GPIO_WritePin(GPIOD, LED_AMARILLO_A, GPIO_PIN_SET);
				}
				flag_SW3=0;
				flag2_SW3=0;

			}
		}
		else{
			if(HAL_GPIO_ReadPin(GPIOA,SW3)==0){			// Detecta el pulsador apretado
				flag2_SW3=1;
			}
			else{
				contaSW3=0;
				flag_SW3=0;

			}
			}
		}


	else{
		contaSW3++;
	}
}

//------------------------------------------------------------------------------


}


// ----------------------------------------------------------------------------
// Función: "Error_Handler"
void Error_Handler(void)
// Esta función se ejecuta en caso de que ocurra un error.
{
	// Enciende LEDs, indicando la condición de "ERROR":
	HAL_GPIO_WritePin(GPIOD,LED_ROJO_B|LED_VERDE_B|LED_AMARILLO_B, GPIO_PIN_SET);
	// Lazo infinito:
	while(1) {}
}
// ----------------------------------------------------------------------------
// Función: "SystemClock_Config"
static void SystemClock_Config(void)
/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 168000000
 *            HCLK(Hz)                       = 168000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 336
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}

	/* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
	if (HAL_GetREVID() == 0x1001)
	{
		/* Enable the Flash prefetch */
		__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	}
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
