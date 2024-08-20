/*
 * Nombre: Buzzer.h
 * Autor: Eduardo L. Blotta
 * Comentarios: Manejo de Timer (modo PWM) para generar un tono en el buzzer
 */
#include "CommonIO.h"
// ----------------------------------------------------------------------------
// --- Constantes Input Capture ---
#define TIM2_PSC 16	 	// Prescaler
#define FREC_528hz 1893	// Periodo para una frec. de 528hz
#define FREC_264hz 3787	// Periodo para una frec. de 264hz (nota DO)
#define FREC_132hz 7566	// Periodo para una frec. de 264hz (nota DO)
// ----------------------------------------------------------------------------
void Buzzer_Init(void);
void play_Buzzer(uint16_t, uint16_t);
void stop_Buzzer(void);
void Error_Handler(void);
// ----------------------------------------------------------------------------

