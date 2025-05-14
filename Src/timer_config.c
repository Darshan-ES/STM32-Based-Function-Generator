#include "timer_config.h"

#include "analog_out.h"
#include "analog_in.h"
#include "dma.h"
#include "wave.h"
//#include "wave.h"
#include"autocorrelate.h"
#define BUFFER_SIZE 1024
#define ADC_SAMPLE_FREG 96000
#define Timer6_ARR 999
#define Timer1_ARR 499
int count=0;
volatile uint32_t Systick_ms =0;
uint16_t InputAudioAnalysis[3]={0,0,0};



typedef enum {
    SINE_440_STATE,
    SINE_587_STATE,
    SINE_659_STATE,
    SINE_880_STATE
} SIN_State;

SIN_State current_SIN_state = SINE_440_STATE;  // Start with 440Hz sine wave


// Tim3_init
// --------------------------------------------------------------------
/*
 *Description : This Function initializes  TIM3 channel 2 in PWM mode
 *parameter   : None
 *return     : None
*/
void Tim3_init()
{
	Tim3_MSP_init();
	RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;
	TIM3->PSC = PRESCALAR;
	TIM3->ARR= AUTORELOAD;
	TIM3->DIER |= DIER_UIE;
	TIM3->CCMR1 |= TIM_OCMODE_PWM;
	TIM3->CCR2 = 100;
	TIM3->CCER |=TIM_OCOUTPUT_ENABLE ;
	TIM3->CNT =0;
	TIM3->CR1 |= CEN;


}

// Tim3_MSP_init
// --------------------------------------------------------------------
/*
 *Description : This Function initializes the PA7 for TIM3 channel 2
 *parameter   : None
 *return     : None
*/
void Tim3_MSP_init()
{

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	MODIFY_FIELD(GPIOA->MODER,GPIO_MODER_MODER7,ESF_GPIO_MODER_ALT_FUNC);
	MODIFY_FIELD(GPIOA->AFR[0],GPIO_AFRL_AFSEL7,1);


}


// Initialize Timer6
void Timer6_Init(void) {
    // Enable Timer6 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;



    TIM6->PSC = 0;      // Set prescaler
    TIM6->ARR = Timer6_ARR;    // Set auto-reload value


    TIM6->DIER |= TIM_DIER_UDE;


    // Enable Timer6
    TIM6->CR1 |= TIM_CR1_CEN;


}
//Initialize Timer1
void Timer1_init(void){
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	// Set up prescaler and counter to get 48 kHz
	TIM1->PSC = 0;
	TIM1->ARR= Timer1_ARR;
	TIM1->CR2|=TIM_CR2_MMS_1;
	TIM1->DIER = TIM_DIER_UIE;
	TIM1->DIER = TIM_DIER_TIE;
	// Enable counting
	TIM1->CR1 |= TIM_CR1_CEN;
}

// SysTick_Init
// --------------------------------------------------------------------
/*
 *Description : This Function initializes systick timer in 250ms
 *parameter   : None
 *return     : Systick_ms
*/
void Systick_Init(void) {
    SysTick_Config(SystemCoreClock / 4);  // 250 ms tick
}
// SysTick_Handler
// --------------------------------------------------------------------
/*
 *Description : This Function resets Systick_ms =0
 *parameter   : None
 *return     : None
*/
void Systick_Reset(void) {
	Systick_ms = 0;
}
// SysTick_Handler
// --------------------------------------------------------------------
/*
 *Description : This Function increments Systick_ms in every 1 ms
 *parameter   : None
 *return     : Systick_ms
*/
void SysTick_Handler(void) {
	Systick_ms++;
	    	   // Check if 2 seconds (2000 ms) have elapsed
	  if (get_Systick_time() >= 8) {
		            Systick_Reset(); // Reset SysTick time
		            AudioAnalysis(ADC_Buffer,InputAudioAnalysis,ADC_BUFFER_SIZE);
		            printf("Min: %d, Max: %d, Mean: %d, Period: %d, Sample Freq: %d\n\r",
		            		InputAudioAnalysis[0], InputAudioAnalysis[1], InputAudioAnalysis[2], period_value, ((ADC_SAMPLE_FREG/period_value)));


		 //            Switch to the next state
		            switch (current_SIN_state) {
		                case SINE_440_STATE:
		                	current_SIN_state = SINE_587_STATE;
		                    DMA_Init();
		                    Start_DMA((uint32_t *)sine_wave_587, SINE_TABLE_SIZE_587Hz);
		                    //printf("Switched to 587Hz sine wave v8\r\n");
		                    printf("\r\n\nGenerated %d Samples at 587 Hz. Computed Period = %d Samples\n\r",
		                    		SINE_TABLE_SIZE_587Hz, SAMPLE_SIZE_587Hz);
		                    break;

		                case SINE_587_STATE:
		                	current_SIN_state = SINE_659_STATE;
		                    DMA_Init();
		                    Start_DMA((uint32_t *)sine_wave_659, SINE_TABLE_SIZE_659Hz);
		                   // printf("Switched to 659Hz sine wave v8\r\n");
		                    printf("\r\n\nGenerated %d Samples at 659 Hz. Computed Period = %d Samples\n\r",
		                    	    SINE_TABLE_SIZE_659Hz, SAMPLE_SIZE_659Hz);
		                    break;

		                case SINE_659_STATE:
		                	current_SIN_state = SINE_880_STATE;
		                    DMA_Init();
		                    Start_DMA((uint32_t *)sine_wave_880, SINE_TABLE_SIZE_880Hz);
		                   // printf("Switched to 880Hz sine wave v8\r\n");
		                    printf("\r\n\nGenerated %d Samples at 880 Hz. Computed Period = %d Samples\n\r",
		                   	         SINE_TABLE_SIZE_880Hz, SAMPLE_SIZE_880Hz);
		                    break;

		                case SINE_880_STATE:
		                	current_SIN_state = SINE_440_STATE;
		                    DMA_Init();
		                    Start_DMA((uint32_t *)sine_wave_440, SINE_TABLE_SIZE_440Hz);
		                    //printf("Switched to 440Hz sine wave v8\r\n");
		                    printf("\r\n\nGenerated %d Samples at 440 Hz. Computed Period = %d Samples\n\r",
		                   	         SINE_TABLE_SIZE_440Hz, SAMPLE_SIZE_440Hz);
		                    break;
		            }
		            //AudioInputModule();
		        }
}
// get_Systick_time
// --------------------------------------------------------------------
/*
 *Description : This Function determines the State Duration Depending on Release and debug MOde
 *parameter   : None
 *return     : Systick_ms
*/
uint32_t get_Systick_time(void) {
    return Systick_ms;
}
/*
 *
 * @brief  Analyzes an audio buffer to compute minimum, maximum, and average values.
 *
 * This function processes a given audio data buffer to calculate the
 * minimum value, maximum value, and average value. The results are stored
 * in the `Analysis` array. Additionally, the function calculates the period
 * of the waveform using autocorrelation.
 *
 * @param[in]   Buffer_Data   Pointer to the audio data buffer containing
 *                            sampled values.
 * @param[out]  Analysis      Pointer to an array where analysis results
 *                            will be stored:
 *                            - `Analysis[0]`: Minimum value.
 *                            - `Analysis[1]`: Maximum value.
 *                            - `Analysis[2]`: Average value.
 * @param[in]   BufferSize    Number of elements in the audio data buffer.
 */
void AudioAnalysis(uint16_t *Buffer_Data,uint16_t *Analysis, uint16_t BufferSize){

	// Initialize min and max with the first element of the buffer
	    uint16_t min_value = Buffer_Data[0];
	    uint16_t max_value = Buffer_Data[0];
	    uint32_t Avg_sum = 0; // Use a 32-bit integer for sum to avoid overflow

	    // Iterate through the buffer to calculate min, max, and sum
	    for (uint16_t i = 0; i < BufferSize; i++) {
	        if (Buffer_Data[i] < min_value) {
	        	min_value = Buffer_Data[i];
	        }
	        if (Buffer_Data[i] > max_value) {
	        	max_value = Buffer_Data[i];
	        }
	        Avg_sum += Buffer_Data[i];
	    }

	    // Store results in the Analysis array
	    Analysis[0] = min_value;                   // Minimum value
	    Analysis[1] = max_value;                   // Maximum value
	    Analysis[2] = (uint16_t)(Avg_sum / BufferSize); // Average value
	    period_value= autocorrelate_detect_period(ADC_Buffer,ADC_BUFFER_SIZE , kAC_12bps_unsigned);

}
