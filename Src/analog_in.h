#include <stdint.h>
#include <math.h>
#include <utilities.h>
#include <stm32f091xc.h>
#include <stm32f0xx.h>



#define ADC_BUFFER_SIZE 1024

extern uint16_t ADC_Buffer[ADC_BUFFER_SIZE];



void ADC_init(void);

void AudioInputModule(void);


