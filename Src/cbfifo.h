/*
 * cbfifo.h
 *
 *  Created on: Nov 11, 2024
 *      Author: Darshan Salian
 */

#ifndef CBFIFO_H_
#define CBFIFO_H_
#include <stdint.h>  // for uint8_t
#include <stdlib.h>  // for size_t
#include "Lib.h"

#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 128

typedef struct {
    uint8_t buffer[RX_BUFFER_SIZE];
    size_t Front;
    size_t Rear;
} cbfifo_t;

// Functions for RX queue
size_t cbfifo_rx_enqueue(void *buf, size_t nbyte);
size_t cbfifo_rx_dequeue(void *buf, size_t nbyte);
size_t cbfifo_rx_length();
size_t cbfifo_rx_capacity();

// Functions for TX queue
size_t cbfifo_tx_enqueue(void *buf, size_t nbyte);
size_t cbfifo_tx_dequeue(void *buf, size_t nbyte);
size_t cbfifo_tx_length();
size_t cbfifo_tx_capacity();


#endif /* CBFIFO_H_ */
