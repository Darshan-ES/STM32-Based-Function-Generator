/*
 * Lib.h
 *
 *  Created on: Nov 7, 2024
 *      Author: Darshan Salian
 */

#ifndef LIB_H_
#define LIB_H_

#include <stdint.h>
#include"utilities.h"
#include"stm32f091xc.h"
#include"stm32f0xx.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "fp_trig.h"
#include "cbfifo.h"
#include "stm32f091xx_USART.h"

#define BUFFER_SIZE 128
#define BACKSPACE 0x08
#define DELETE 0x7F
#define ENTER '\r'
#define LED_PIN 5

#endif /* LIB_H_ */
