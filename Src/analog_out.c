#include "analog_out.h"

/*
 * @brief  Initializes the DAC peripheral for analog output.
 *
 * This function configures the DAC to output an analog signal on
 * pin PA4. It enables the necessary clocks for the DAC and GPIOA,
 * configures PA4 as an analog output, and sets up the DAC to use
 * DMA for data transfer.
 *
 * @details
 * - The GPIOA clock is enabled to configure PA4 as an analog pin.
 * - The DAC clock is enabled to initialize DAC1.
 * - PA4 is configured in analog mode by modifying the MODER register.
 * - The DAC is enabled on channel 1, with DMA support activated to
 *   facilitate continuous data transfer.
 */
void Init_DAC(void)
{
	 // Enable clocks for DAC and PA4
	    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	    // Init PA4 as analog by setting both MODER bits
	    MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER4, ESF_GPIO_MODER_ANALOG);

	    // Init DAC1, connecting it to PA4
	    DAC->CR |= DAC_CR_EN1 | DAC_CR_DMAEN2;
}



