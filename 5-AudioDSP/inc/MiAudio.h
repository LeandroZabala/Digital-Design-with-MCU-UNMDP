/**
******************************************************************************
 * Proyecto: "Implementación de un filtro de audio".
* Autor: Diego Sebastián Comas
* Fecha: 27/09/2017
* Archivo: Audio.h
* Autor archivo: Diego Sebastián Comas
* Comentario: Archivo de declaraciones de "Audio.c".
******************************************************************************
*/
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ---------Definición de inclusiones:
#include "CommonIO.h"
#include "stm32f4_discovery_audio.h"
#include "arm_math.h" // Librería de funciones DSP
#include "stdlib.h"
// ----------------------------------------------------------------------------

// ---------Definicion de Constantes AUDIO:
#define  BLOCK_SIZE 			(uint32_t) 		1000  // = BUFFER_SIZE/4
/* Es el nro de muestras que se van a procesar en cada iteracion. A partir de BLOCK_SIZE muestras mono se
 * generan 2*BLOCK_SIZE muestras estereo (canales L+R). */
#define  FRECUENCIA_MUESTREO 	(uint32_t) 		22050 // En Hz
#define  VOLUMEN 				(uint8_t) 		70    // En %
#define  T_REBOTE 				(uint8_t) 		20    // El tiempo de espera en ticks para el rebote de SWITCHES!


//------------------------------------------
TIM_HandleTypeDef TimHandle; /* Timer handler definition */
TIM_IC_InitTypeDef sICConfig; /* Timer Input Capture Configuration Structure definition */
//----------------------------------------------------------------------------
//---------Definición de tipos ENUM---------
typedef enum
{
  PLAY,
  STOP
} Tipo_Modo_AUDIO; // Modos de Reproduccion de Audio. Comandados por SW1,SW2 y SW3)

typedef enum
{
  HECHO,
  MITAD,
  TODO
} Tipo_Estado_Buffer;

typedef enum
{
  OFF,
  ON
} Tipo_Estado_Filtro;

// ----------------------------------------------------------------------------
// ---------Declaración de funciones------
void Procesar_Buffer(uint16_t *puntero_Inicio_Vector, uint16_t *puntero_Inicio_Buffer, uint32_t nDatos);
void Copiar_Buffer(uint16_t* pAudio, uint16_t* pBuffer, uint32_t nDatos);
void Filtrar_Buffer(uint16_t* pAudio, uint16_t* pBuffer, uint32_t nDatos);
void ROM2BUFFER(uint16_t *pAudio, uint16_t *pBuffer, uint32_t nDatos);
void UINT16toFLOAT32(uint16_t* pAudio, float32_t* pAuxBuffer, uint32_t nDatos);
void FLOAT32toUINT16(float32_t* outBuffer, uint16_t* pBuffer, uint32_t nDatos);
void FIRFilter(float32_t* pAuxBuffer, float32_t* outBuffer, uint32_t nDatos);
void FIR_Init(void);
// ----------------------------------------------------------------------------
// ---------Rutinas de interrupción del CODEC DE AUDIO-------
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void OTG_FS_IRQHandler(void);
void I2S2_IRQHandler(void);
void I2S3_IRQHandler(void);
void TIM4_IRQHandler(void);
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
