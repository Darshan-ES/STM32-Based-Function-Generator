/*
 * stm32f091xx_USART.c
 *
 *  Created on: Nov 7, 2024
 *      Author: Darshan Salian
 */
#include "stm32f091xx_USART.h"
#include "Lib.h"
#include "cbfifo.h"

#define BAUDRATE 19200
#define PARITY_ENABLE  1 // 1: Parity Enable  0: Parity Disable
#define PARITY_ODD  1 // 1: Parity odd  0: Parity EVEN
#define DATABIT9 1  //1: Databits 9 0: Databits 8


int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        cbfifo_tx_enqueue(&ptr[i], 1);
    }
    USART2->CR1 |= USART_CR1_TXEIE; // Enable TX interrupt
    return len;
}

// Implement the _read function
int _read(int file, char *ptr, int len) {
    while (cbfifo_rx_length() == 0); // Wait until data is available
    return cbfifo_rx_dequeue(ptr, len);
}



void Initialize_UART() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;   // Enable GPIOA clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  // Enable USART2 clock

    GPIOA->MODER |= PA2_AF_MODE | PA3_AF_MODE;   // Set PA2, PA3 to AF mode
    GPIOA->AFR[0] |= PA2_AF7 | PA3_AF7; // Set AF7 for USART2

    USART2->BRR = (24000000/BAUDRATE);                  // Set baud rate
    USART2->CR1 |= USART_TE | USART_RE | USART_RXNEIE | USART_TXEIE; // TX, RX, RX interrupT
    if(DATABIT9==1)
    {
    	USART2->CR1 |=(USART_CR1_M0);
    }
    else
    {
    	USART2->CR1 &=~(USART_CR1_M0);
    }
    USART2->CR1 &=~(USART_CR1_M1);
    if(PARITY_ENABLE==1)
    {
      USART2->CR1 |= USART_CR1_PCE;
    }
    else
    {
    	USART2->CR1 &=~(USART_CR1_PCE);
    }
    if(PARITY_ODD==1)
    {
    	USART2->CR1 |= USART_CR1_PS;
    }
    else
    {
    	USART2->CR1 &=~(USART_CR1_PS);
    }

    USART2->CR2 &=~(USART_CR2_STOP);
    NVIC_EnableIRQ(USART2_IRQn);
    USART2->CR1 |= (1 << 0);              // Enable UART
}

void uart_send_data(char data) {
    while (!(USART2->ISR & USART_ISR_TXE));  // Wait until TX is ready
    USART2->TDR = data;                     // Send data
}

void putstr(const char* s) {
    while (*s) {
        uart_send_data(*s++);                    // Send each character
    }
}

void USART2_IRQHandler(void) {

	// TX Interrupt
		if (USART2->ISR & USART_ISR_TXE) {
			char next_Ch;
			if (cbfifo_tx_dequeue(&next_Ch, 1)) {
				USART2->TDR = next_Ch;  // Send next character
			} else {
				USART2->CR1 &= ~USART_CR1_TXEIE;  // Disable TXE interrupt if queue is empty
			}
		}

	// RX Interrupt
	    if (USART2->ISR & USART_ISR_RXNE) {
	        char received_Ch = USART2->RDR;  // Read received data
	        cbfifo_rx_enqueue(&received_Ch, 1);  // Enqueue to RX queue


	        // Immediately enqueue to TX queue for echo
	        if (cbfifo_tx_enqueue(&received_Ch, 1)) {
	            USART2->CR1 |= USART_CR1_TXEIE;  // Enable TXE interrupt if data was successfully enqueued
	        }
	    }


}
