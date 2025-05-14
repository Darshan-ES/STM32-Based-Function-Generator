#include "cbfifo.h"

// Separate cbfifo instances for RX and TX
static cbfifo_t cbfifo_rx = {{0}, 0,0 };
static cbfifo_t cbfifo_tx = {{0}, 0,0 };

/**
 * @brief Enqueues data into the RX FIFO buffer.
 *
 * @param buf Pointer to the buffer containing data to enqueue.
 * @param nbyte Number of bytes to enqueue from buf.
 * @return size_t The number of bytes successfully enqueued, or -1 if buf is NULL or nbyte <= 0.
 */
size_t cbfifo_rx_enqueue(void *buf, size_t nbyte) {
	if((buf==NULL) || (nbyte<=0))
	    {
	        return (size_t) -1;       //return if buf is NULL or nbytes is less than equal to Zero
	    }
    cbfifo_t *fifo = &cbfifo_rx;
    size_t enqueued = 0;
    uint8_t *data = (uint8_t *)buf;

    for (; nbyte > 0; nbyte--) {
        size_t next_Front = (fifo->Front + 1) % RX_BUFFER_SIZE;
        if (next_Front == fifo->Rear) {
            break; // FIFO is full
        }
        fifo->buffer[fifo->Front] = *data++;
        fifo->Front = next_Front;
        enqueued++;
    }
    return enqueued;
}
/**
 * @brief Dequeues data from the RX FIFO buffer.
 *
 * @param buf Pointer to the buffer to store dequeued data.
 * @param nbyte Number of bytes to dequeue into buf.
 * @return size_t The number of bytes successfully dequeued, or 0 if buf is NULL or nbyte <= 0.
 */
size_t cbfifo_rx_dequeue(void *buf, size_t nbyte) {
	if((buf==NULL) || (nbyte<=0))
	{
		return 0;       //return if buf is NULL or nbytes is less than equal to Zero
	}
    cbfifo_t *fifo = &cbfifo_rx;
    size_t dequeued = 0;
    uint8_t *data = (uint8_t *)buf;

    for (; nbyte > 0; nbyte--) {
        if (fifo->Front == fifo->Rear) {
            break; // FIFO is empty
        }
        *data++ = fifo->buffer[fifo->Rear];
        fifo->Rear = (fifo->Rear + 1) % RX_BUFFER_SIZE;
        dequeued++;
    }
    return dequeued;
}
/**
 * @brief Returns the current number of bytes stored in the RX FIFO buffer.
 *
 * @return size_t The number of bytes in the RX FIFO buffer.
 */
size_t cbfifo_rx_length() {
    cbfifo_t *fifo = &cbfifo_rx;
    return (fifo->Front - fifo->Rear + RX_BUFFER_SIZE) % RX_BUFFER_SIZE;
}

/**
 * @brief Returns the total capacity of the RX FIFO buffer.
 *
 * @return size_t The maximum number of bytes the RX FIFO buffer can hold.
 */
size_t cbfifo_rx_capacity() {
    return RX_BUFFER_SIZE;
}

/**
 * @brief Enqueues data into the TX FIFO buffer.
 *
 * @param buf Pointer to the buffer containing data to enqueue.
 * @param nbyte Number of bytes to enqueue from buf.
 * @return size_t The number of bytes successfully enqueued, or -1 if buf is NULL or nbyte <= 0.
 */
size_t cbfifo_tx_enqueue(void *buf, size_t nbyte) {
	if((buf==NULL) || (nbyte<=0))
		    {
		        return (size_t) -1;       //return if buf is NULL or nbytes is less than equal to Zero
		    }
	    cbfifo_t *fifo = &cbfifo_tx;
	    size_t enqueued = 0;
	    uint8_t *data = (uint8_t *)buf;

	    for (; nbyte > 0; nbyte--) {
	        size_t next_Front = (fifo->Front + 1) % TX_BUFFER_SIZE;
	        if (next_Front == fifo->Rear) {
	            break; // FIFO is full
	        }
	        fifo->buffer[fifo->Front] = *data++;
	        fifo->Front = next_Front;
	        enqueued++;
	    }
	    return enqueued;
}

/**
 * @brief Dequeues data from the TX FIFO buffer.
 *
 * @param buf Pointer to the buffer to store dequeued data.
 * @param nbyte Number of bytes to dequeue into buf.
 * @return size_t The number of bytes successfully dequeued, or 0 if buf is NULL or nbyte <= 0.
 */
size_t cbfifo_tx_dequeue(void *buf, size_t nbyte) {
	if((buf==NULL) || (nbyte<=0))
		{
			return 0;       //return if buf is NULL or nbytes is less than equal to Zero
		}
	 cbfifo_t *fifo = &cbfifo_tx;
	    size_t dequeued = 0;
	    uint8_t *data = (uint8_t *)buf;

	    for (; nbyte > 0; nbyte--) {
	        if (fifo->Front == fifo->Rear) {
	            break; // FIFO is empty
	        }
	        *data++ = fifo->buffer[fifo->Rear];
	        fifo->Rear = (fifo->Rear + 1) % TX_BUFFER_SIZE;
	        dequeued++;
	    }
	    return dequeued;
}
/**
 * @brief Returns the current number of bytes stored in the TX FIFO buffer.
 *
 * @return size_t The number of bytes in the TX FIFO buffer.
 */
size_t cbfifo_tx_length() {
    cbfifo_t *fifo = &cbfifo_tx;
    return (fifo->Front - fifo->Rear + TX_BUFFER_SIZE) % TX_BUFFER_SIZE;
}
/**
 * @brief Returns the total capacity of the TX FIFO buffer.
 *
 * @return size_t The maximum number of bytes the TX FIFO buffer can hold.
 */
size_t cbfifo_tx_capacity() {
    return TX_BUFFER_SIZE;
}
