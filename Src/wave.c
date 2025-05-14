#include "wave.h"


uint16_t sine_wave_440[DAC_BUFFER_SIZE];
uint16_t sine_wave_587[DAC_BUFFER_SIZE];
uint16_t sine_wave_659[DAC_BUFFER_SIZE];
uint16_t sine_wave_880[DAC_BUFFER_SIZE];
int period_value=0;


/*
 * @brief  Generates a sine wave and stores it in a buffer.
 *
 * This function generates a sine wave of a specified frequency by
 * populating the given buffer with calculated sine values. The sine
 * wave is scaled and shifted using the `TRIG_SCALE_FACTOR` to fit within
 * the range of the DAC output.
 *
 * @param[in]  buffer       Pointer to the buffer where sine wave values
 *                          will be stored.
 * @param[in]  buffer_size  Number of elements in the buffer.
 * @param[in]  sample_size  Number of samples per sine wave cycle.
 */
void Generate_SineWave(uint16_t *buffer, uint16_t buffer_size, uint16_t sample_size) {
    memset(buffer, 0, buffer_size * sizeof(uint16_t)); // Clear the buffer
    for (int i = 0; i < buffer_size; i++) {
        buffer[i] = fp_sin(i * TWO_PI / sample_size) + TRIG_SCALE_FACTOR;
    }
}






