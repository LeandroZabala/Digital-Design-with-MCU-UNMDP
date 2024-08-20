/*
 * Nombre: ConversorxInts.h
 * Autor: Eduardo L. Blotta
 * Comentarios: Manejo de conversor A/D c/interrupciones
 */
#include "CommonIO.h"
//======================================================
extern void Error_Handler(void);
void ADC1_TIM3_Init(void);
void ADC_IRQHandler(void);
void ADCIRQHook (uint16_t ConversorAD);
//======================================================
ADC_HandleTypeDef hADC1;
TIM_HandleTypeDef htim3;
#define PRSC_TIM	1600
#define PERIOD_TIM	2500
//======================================================
