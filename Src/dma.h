#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <utilities.h>
#include <stm32f091xc.h>
#include <stm32f0xx.h>
#include <string.h>
#include "fp_trig.h"
#include "autocorrelate.h"


void DMA_Init(void);

void Start_DMA(uint32_t *source, uint32_t length);
