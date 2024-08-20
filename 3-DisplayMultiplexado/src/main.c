// ----------------------------------------------------------------------------
// Laboratorio Nº3: Display Multiplexado.
// ----------------------------------------------------------------------------
// Agregue sus "#includes" aqui
#include "SysTick.h"
#include "Salidas.h"

#define SYSTICK_FREQ_HZ 1000		//Frecuencia del System Tick. Cada 1ms tengo una interrupcion.
// ----------------------------------------------------------------------------
// Variables globales
uint8_t StartFlag =0;	//Si StartFlag=1 el cronometro Cuenta. Si es 0 esta detenido.
uint8_t ResetFlag =0;	//Si ResetFlag=1 el cronometro Se resetea. Si es 0 esta andando.

// ----- main() ---------------------------------------------------------------
int main(void)
{
	/* System Tick Init */
	SysTick_Init (SYSTICK_FREQ_HZ);

	/* Conf. Entradas */
	__HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA Peripheral clock
	In_Init(GPIOA, SW1|SW2);

	/* Conf. Salidas */
	__HAL_RCC_GPIOC_CLK_ENABLE(); // Enable GPIOC Peripheral clock
	__HAL_RCC_GPIOD_CLK_ENABLE(); // Enable GPIOD Peripheral clock

	Out_Init(GPIOC, SEL_MUX_0|SEL_MUX_1);	// Defino como salidas las entradas al decodificador.
											// Defino como salidas cada segmento de los displays.
	Out_Init(GPIOD, SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f|SEGMENTO_g|SEGMENTO_PD);


	while (1) // Loop infinito.
	{
		if(HAL_GPIO_ReadPin(GPIOA, SW1)==0){	// Detecta el pulsador apretado
			HAL_Delay(30);
			if(HAL_GPIO_ReadPin(GPIOA,SW1)==0){
					if(StartFlag==0){
						StartFlag=1;
					}else {
						StartFlag=0;
					}
			}
			while(HAL_GPIO_ReadPin(GPIOA, SW1)==0){
			}

		}
		if(HAL_GPIO_ReadPin(GPIOA, SW2)==0){	// Detecta el pulsador apretado
			HAL_Delay(30);
			if(HAL_GPIO_ReadPin(GPIOA,SW2)==0){
					if(ResetFlag==0 && StartFlag==0){
						ResetFlag=1;
					}else {
						ResetFlag=0;
					}
			}
			while(HAL_GPIO_ReadPin(GPIOA, SW2)==0){
			}

		}


	}	

}

// ----------------------------------------------------------------------------
void SysTickHook (void) // Para colgarse del System Tick
{
	static uint8_t Contams =0;
	static uint8_t ContaDECms =0;
	static uint8_t ContaCENTms =0;
	static uint8_t Contaseg =0;
	static uint8_t ContaDECseg =0;

	static uint8_t BufDisp[4] = {0,0,0,0};
	static uint8_t Digito =0;		// digito se inicializa en 0. Como se definio static
									// el programa no vuelve a pasar por aca. No vuelve a hacerse 0 cada
									// vez que pasa.
if (ResetFlag==1){
	Contams= 0;
	ContaDECms= 0;
	ContaCENTms= 0;
	Contaseg= 0;
	ContaDECseg=0;
	ResetFlag=0;
}

if(StartFlag==1){		//Si La bandera de Start es 1==> permite que se actualizen los contadores.
						//Por ende cambian los numeros presentados en el display.
	Contams=Contams+1;

	if(Contams == 10){
		Contams= 0;
		ContaDECms++;
		if(ContaDECms == 10){
			ContaDECms= 0;
			ContaCENTms++;
			if(ContaCENTms == 10){
					ContaCENTms= 0;
					Contaseg++;
					if(Contaseg == 10){
							Contaseg= 0;
							ContaDECseg++;
							if(ContaDECseg == 6){	//Cuando se llega al maximo, el cronometro queda en 0000
								Contams= 0;
								ContaDECms= 0;
								ContaCENTms= 0;
								Contaseg= 0;
								ContaDECseg=0;
							}
					}
			}
		}
	}

}
	//El buffer se actualiza cada 1ms, sin embargo esto no es necesario, ya que cambia cada 10ms.
	// Se podria ubicar este segmento de codigo en otra parte del programa, pero se deja asi por pulcritud de codigo.

	BufDisp[3]=ContaDECms;
	BufDisp[2]=ContaCENTms;
	BufDisp[1]=Contaseg;
	BufDisp[0]=ContaDECseg;



	// Tabla de Conversion BCD -> 7 segmentos
	const uint32_t BIN7SEG[] = {
	SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f,	// Cero
	SEGMENTO_b|SEGMENTO_c,												// Uno
	SEGMENTO_a|SEGMENTO_b|SEGMENTO_g|SEGMENTO_e|SEGMENTO_d,				// Dos
	SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_g,				// Tres
	SEGMENTO_f|SEGMENTO_b|SEGMENTO_g|SEGMENTO_c,						// Cuatro
	SEGMENTO_a|SEGMENTO_f|SEGMENTO_g|SEGMENTO_c|SEGMENTO_d,				// Cinco
	SEGMENTO_f|SEGMENTO_e|SEGMENTO_d|SEGMENTO_c|SEGMENTO_g,				// Seis
	SEGMENTO_a|SEGMENTO_b|SEGMENTO_c,									// Siete
	SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f|SEGMENTO_g,	// Ocho
	SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_f|SEGMENTO_g				// Nueve
								};
	// Escriba su codigo aqui


	if(Digito == 4){
		Digito= 0;
	}

	switch(Digito){
		case 0:
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_0, GPIO_PIN_RESET);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_0, GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_0, GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, SEL_MUX_0, GPIO_PIN_SET);
			break;
		}
//	Out_Init(GPIOD, SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f|SEGMENTO_g|SEGMENTO_PD);
	HAL_GPIO_WritePin(GPIOD, SEGMENTO_a|SEGMENTO_b|SEGMENTO_c|SEGMENTO_d|SEGMENTO_e|SEGMENTO_f|SEGMENTO_g|SEGMENTO_PD , GPIO_PIN_RESET);
	if(Digito==2){
		HAL_GPIO_WritePin(GPIOD, BIN7SEG[BufDisp[-Digito+3]]|SEGMENTO_PD, GPIO_PIN_SET);	// Si quiero encender el digito de los segundos
																							// enciendo el numero + el punto decimal
	}else{
			HAL_GPIO_WritePin(GPIOD, BIN7SEG[BufDisp[-Digito+3]], GPIO_PIN_SET);			// Si quiero encender cualquier otro digito que no
																							// sea el de los segundos, ilumino los segmentos correspondiente al numero unicamente.
	}																						// es decir, no le prendo el punto decimal


	Digito=Digito+1;

}
