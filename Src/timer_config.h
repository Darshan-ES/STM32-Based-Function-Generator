#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <utilities.h>
#include <stm32f091xc.h>
#include <stm32f0xx.h>


/*********TIM**************/
#define PRESCALAR  (3000-1)
#define AUTORELOAD  (255)

/************Timer3-CH2*********/
#define CEN (1U<<0)
#define SR_UIF (1U<<0)
#define DIER_UIE (1<<0)
#define TIM_OCMODE_PWM       (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1)
#define TIM_OCPOLARITY_HIGH      0x00000000U
#define TIM_OCOUTPUT_ENABLE     (1U<<4)


#define TIMER6_FREQ 24000      // Timer6 frequency (in Hz)

extern volatile uint32_t Systick_ms;

extern uint16_t InputAudioAnalysis[3];

void Timer6_Init(void);

void Systick_Init(void);

void Timer1_init(void);

void Tim3_init(); //Timer3 init
void Tim3_MSP_init(); //Timer3 MSP init


void Systick_Reset(void);

void SysTick_Handler(void);

void Systick_Reset(void);


uint32_t get_Systick_time(void);

void AudioAnalysis(uint16_t *Buffer_Data,uint16_t *Analysis, uint16_t BufferSize);




