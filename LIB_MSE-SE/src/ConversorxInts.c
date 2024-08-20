/*
 * Nombre: ConversorxInts.c
 * Autor: Eduardo L. Blotta
 * Comentarios: Manejo de conversor A/D c/interrupciones
 */
#include "ConversorxInts.h"
//======================================================
void ADC1_TIM3_Init(void)
{
	//======================================================
	//ADC1 GPIO Configuration
	//PA2------> ADC1_IN2
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//======================================================
	/* Configure the NVIC */
	/* NVIC configuration for ADC interrupt */
	HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);
	//======================================================
	//Configure the global features of the ADC (Clock, 
	//Resolution, Data Alignment and number of conversion)
	hADC1.Instance = ADC1;
	hADC1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hADC1.Init.Resolution = ADC_RESOLUTION_12B;
	hADC1.Init.ScanConvMode = DISABLE;
	hADC1.Init.ContinuousConvMode = DISABLE;
	hADC1.Init.DiscontinuousConvMode = DISABLE;
	hADC1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hADC1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
	hADC1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hADC1.Init.NbrOfConversion = 1;
	hADC1.Init.DMAContinuousRequests = DISABLE;
	hADC1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	if(HAL_ADC_Init(&hADC1) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
	//======================================================
	//Configure for the selected ADC regular channel its
	//corresponding rank in the sequencer and its sample time.
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if(HAL_ADC_ConfigChannel(&hADC1, &sConfig) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
	/***************************************************/
	/* Inicializacion Timer 3 (base de tiempo de ADC1) */
	/***************************************************/
	//uint32_t ftim3;
	  TIM_MasterConfigTypeDef sMasterConfig = {0};
	  TIM_OC_InitTypeDef sConfigOC = {0};
	  htim3.Instance = TIM3;
	  htim3.Init.Prescaler = PRSC_TIM-1;
	  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim3.Init.Period = PERIOD_TIM-1;
	  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = 0;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK)
	  {
		  Error_Handler();
	  }
	  //ftim3=HAL_RCC_GetPCLK1Freq()/PRSC_TIM;
}

void ADC_IRQHandler(void)
{
	HAL_ADC_IRQHandler(&hADC1);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hADC1)
{
	__IO uint16_t ConversorAD;
		// Funcion de lectura del conversor:
		ConversorAD=HAL_ADC_GetValue(hADC1);
		ADCIRQHook (ConversorAD);
}
