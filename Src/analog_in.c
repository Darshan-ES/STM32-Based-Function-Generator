#include "analog_in.h"
uint16_t ADC_Buffer[ADC_BUFFER_SIZE];


/*
 *
 * @brief  Initializes the ADC peripheral for audio sampling.
 *
 * This function configures the ADC peripheral to sample data from
 * the analog input pin PA4. It sets up the ADC clock, input mode,
 * sampling time, and enables the ADC for operation.
 *
 * @details
 * - The ADC clock source is configured to use the HSI14 oscillator.
 * - PA4 is configured as an analog input to channel 4.
 * - The sampling time is set to the shortest possible for faster conversions.
 * - External trigger for conversions is configured using TIM1_TRGO.
 *
 * @note  The function waits for the HSI14 oscillator and ADC readiness
 *        to ensure proper operation.
 *
 * @note  External triggering ensures precise timing for ADC sampling.
 *
 * @SOURCE  Embedded system Fundamentals with Arm cortex M-Based Microcontrollers By DR Alexander G.Dean
 * 			chapter : Analog Interfacing (Listing 6.6)
 */
void ADC_init(void) {
	    // Enable peripheral clocks for ADC and GPIOA
	    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;    // Enable ADC clock
	    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;   // Enable GPIOA clock

	    // Configure PA4 as analog input
	    //GPIOA->MODER |= (3U << GPIO_MODER_MODER4_Pos); // Set MODER4[1:0] = 11 (Analog mode)

	    // Enable HSI14 oscillator for ADC and wait for it to be ready
	    RCC->CR2 |= RCC_CR2_HSI14ON;    // Enable HSI14
	    while (!(RCC->CR2 & RCC_CR2_HSI14RDY)) {
	        // Add a timeout here for robustness if needed
	    }

	    // Configure ADC clock to use HSI14 (CKMODE = 00)
		MODIFY_FIELD(ADC1->CFGR2, ADC_CFGR2_CKMODE, 0);

	    // Set the sampling time (shortest time for faster conversion)
	    ADC1->SMPR = 0; // SMP[2:0] = 000 (1.5 ADC clock cycles)

	    // Configure ADC channel selection for PA4 (channel 4)
	    ADC1->CHSELR = ADC_CHSELR_CHSEL4;

	    // Configure ADC external trigger for TIM1_TRGO
		ADC1->CR |= ADC_CR_ADSTART;
		ADC1->CFGR1|=0<<ADC_CFGR1_CONT_Pos;
	    ADC1->CFGR1 |= (0 << ADC_CFGR1_EXTSEL_Pos)| ADC_CFGR1_EXTEN_1;

	    // Enable ADC
	    if (ADC1->ISR & ADC_ISR_ADRDY) {
	        ADC1->ISR |= ADC_ISR_ADRDY; // Clear ADRDY flag if set
	    }
	    ADC1->CR |= ADC_CR_ADEN; // Enable ADC
	    while (!(ADC1->ISR & ADC_ISR_ADRDY)) {
	        // Wait until ADC is ready (add timeout if required)
	    }

}


/**
 * @brief  Captures audio samples using the ADC into a buffer.
 *
 * This function fills the `ADC_Buffer` array with audio samples by
 * triggering ADC conversions and reading the converted values. The
 * conversions are performed sequentially in a polling loop.
 *
 * @details
 * - The function initiates a new ADC conversion for each sample.
 * - It waits for the conversion to complete by polling the End-of-Conversion (EOC) flag.
 * - Once complete, the converted value is read from the ADC data register (DR)
 *   and stored in the `ADC_Buffer` array.
 */

void AudioInputModule(void){

for(int i=0; i<ADC_BUFFER_SIZE;i++)
{
	uint16_t Adc_Value;
	ADC1->CR |= ADC_CR_ADSTART;

	while(!(ADC1->ISR & ADC_ISR_EOC)){}
	Adc_Value = ADC1->DR;
	ADC_Buffer[i]= Adc_Value;
}

}

