/*
 * stm32f091xx_USART.h
 *
 *  Created on: Nov 7, 2024
 *      Author: Darshan Salian
 */

#ifndef STM32F091XX_USART_H_
#define STM32F091XX_USART_H_

#define PA2_AF_MODE (1 << 5)   // Set PA2 to Alternate Function mode
#define PA3_AF_MODE (1 << 7)   // Set PA3 to Alternate Function mode

// GPIO Alternate Function Register (AFR) settings for AF7 (USART2)
#define PA2_AF7 (1 << 8)       // Set PA2 to AF7 (USART2)
#define PA3_AF7 (1 << 12)      // Set PA3 to AF7 (USART2)

// USART Control Register 1 (CR1) bit settings
#define USART_TE (1 << 3)       // Transmit Enable (TE)
#define USART_RE (1 << 2)       // Receive Enable (RE)
#define USART_RXNEIE (1 << 5)   // RXNE Interrupt Enable (RXNEIE)
#define USART_TXEIE (1 << 7)    // TXE Interrupt Enable (TXEIE)


void Initialize_UART();
void uart_send_data(char data);
void putstr(const char* s);
void USART2_IRQHandler(void);
#endif /* STM32F091XX_USART_H_ */
