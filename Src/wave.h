#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <utilities.h>
#include <stm32f091xc.h>
#include <stm32f0xx.h>
#include <string.h>
#include "fp_trig.h"
#include "autocorrelate.h"
#include "analog_out.h"
#include "analog_in.h"
#include "dma.h"
#include "timer_config.h"
#include "stm32f091xx_USART.h"

/*
 *
 * Sine Wave Generation:
 * - Generates sine wave lookup tables for specific frequencies
 *   (440 Hz, 587 Hz, 659 Hz, 880 Hz) using fixed-point arithmetic.
 *
 * Waveform Switching:
 * - Dynamically updates the waveform output via DMA by selecting the
 *   active sine wave lookup table.
 *
 * Macros:
 * - Defines table sizes for each frequency.
 * - Includes sample sizes and constants for scaling and frequency calculation.
 *
 * Calculations:
 * - The `SAMPLE_SIZE` for each frequency is determined by:
 *   SAMPLE_SIZE = (1 / frequency) * Sampling_Frequency / 1000
 *
 * - The `SINE_TABLE_SIZE` is computed using:
 *   SINE_TABLE_SIZE = 1024 / (Samples_Per_Period)
 *   The resulting value is then scaled by multiplying with `SAMPLE_SIZE`.
 */


#define SINE_TABLE_SIZE 120

#define SINE_TABLE_SIZE_440Hz 981
#define SINE_TABLE_SIZE_587Hz 984
#define SINE_TABLE_SIZE_659Hz 1022
#define SINE_TABLE_SIZE_880Hz 990

#define SAMPLE_SIZE_440Hz 109
#define SAMPLE_SIZE_587Hz 82
#define SAMPLE_SIZE_659Hz 73
#define SAMPLE_SIZE_880Hz 55
#define WAVE_FREQ 1000         // Desired sine wave frequency (in Hz)



// Declare the sine wave tables
extern uint16_t sine_wave_440[DAC_BUFFER_SIZE];
extern uint16_t sine_wave_587[DAC_BUFFER_SIZE];
extern uint16_t sine_wave_659[DAC_BUFFER_SIZE];
extern uint16_t sine_wave_880[DAC_BUFFER_SIZE];
extern int period_value;

void Generate_SineWave(uint16_t *buffer, uint16_t buffer_size, uint16_t sample_size);


