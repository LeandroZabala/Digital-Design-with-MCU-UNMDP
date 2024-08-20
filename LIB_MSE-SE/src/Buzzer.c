/*
 * Nombre: Buzzer.c
 * Autor: Eduardo L. Blotta
 * Comentarios: Manejo de Timer (modo PWM) para generar un tono en el buzzer
 */
#include "Buzzer.h"
TIM_HandleTypeDef htim2;
TIM_ClockConfigTypeDef sClockSourceConfig = {0};
TIM_MasterConfigTypeDef sMasterConfig = {0};
TIM_OC_InitTypeDef sConfigOC = {0};
uint16_t vStopBuzzer=0; // Duracion buzzer en ms
//======================================================
void Buzzer_Init(void)
{
	volatile uint32_t PCLK1;
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 16;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 200;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 100;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(htim2.Instance==TIM2)
	{
		//__HAL_RCC_GPIOA_CLK_ENABLE();
		/**TIM2 GPIO Configuration
	      PA3     ------> TIM2_CH4 */
		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
	PCLK1=HAL_RCC_GetPCLK1Freq(); // Frecuencia PCLK1
 }


void play_Buzzer(uint16_t periodo, uint16_t duracion)
{
	if (periodo>2 && duracion>0)
	{
		htim2.Init.Period = periodo;
		if (HAL_TIM_Base_Init(&htim2) != HAL_OK) // actualizo periodo
			{
				Error_Handler();
			}
		sConfigOC.Pulse = (periodo>>1);
		if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK) // actualizo ciclo de trabajo
			{
				Error_Handler();
			}
		vStopBuzzer=duracion;
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	}
}

void stop_Buzzer(void)
{
	HAL_TIM_PWM_Stop (&htim2, TIM_CHANNEL_4);
}

void Error_Handler(void)
{
    while(1)
    {
    	//Stay here on Error
    }
}

