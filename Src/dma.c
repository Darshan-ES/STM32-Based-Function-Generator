#include "dma.h"
extern uint8_t DMA_FLAG;

/*
 * @brief  Configures the DMA controller for DAC data transfer.
 *
 * This function initializes the DMA2 controller to transfer data
 * from memory to the DAC's data register. It sets up the DMA channel
 * in circular mode with 16-bit data transfers and enables the interrupt
 * for handling transfer completion.
 *
 * @details
 * - Enables the DMA2 clock to configure DMA channels.
 * - Configures DMA2_Channel3 for memory-to-peripheral mode with
 *   circular mode enabled to allow continuous data transfer.
 * - Sets the data size to 16 bits for both memory and peripheral transfers.
 * - Assigns a high priority to the DMA channel for uninterrupted operation.
 * - Configures the DMA request source to be triggered by TIM6 update events.
 * - Enables the NVIC interrupt for DMA channel 3 to handle transfer completion.
 *@SOURCE  Embedded system Fundamentals with Arm cortex M-Based Microcontrollers By DR Alexander G.Dean
 * 			chapter : Direct Memory Access (Listing 9.9)
 */
void DMA_Init(void) {
	 // Enable DMA2 clock
	    RCC->AHBENR |= RCC_AHBENR_DMA2EN;

	    // Memory to peripheral mode, 16-bit data
	    DMA2_Channel3->CCR = DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_CIRC;
	    MODIFY_FIELD(DMA2_Channel3->CCR, DMA_CCR_MSIZE, 1);
	    MODIFY_FIELD(DMA2_Channel3->CCR, DMA_CCR_PSIZE, 1);
	    MODIFY_FIELD(DMA2_Channel3->CCR, DMA_CCR_PL, 3);

	    NVIC_SetPriority(DMA1_Ch4_7_DMA2_Ch3_5_IRQn, 3);
	    NVIC_ClearPendingIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
	    NVIC_EnableIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);

	    // DMA2 Channel 3 requests come from TIM6 UP update (CxS = 0001)
	    MODIFY_FIELD(DMA2->CSELR, DMA_CSELR_C3S, 1);
}

/*
 * @brief  Starts a DMA transfer to the DAC.
 *
 * This function initiates a DMA transfer from the specified memory source
 * to the DAC's data register. It configures the memory address, transfer
 * length, and starts the DMA channel for operation.
 *
 * @param[in]   source  Pointer to the memory buffer containing the data
 *                      to be transferred to the DAC.
 * @param[in]   length  Number of data elements to transfer from the source buffer.
 *
 * @SOURCE  Embedded system Fundamentals with Arm cortex M-Based Microcontrollers By DR Alexander G.Dean
 * 			chapter : Direct Memory Access (Listing 9.10)
 */
void Start_DMA(uint32_t *source, uint32_t length) {
    DMA2_Channel3->CCR |= DMA_CCR_TCIE;
    DMA2_Channel3->CNDTR = length;
    DMA2_Channel3->CMAR = (uint32_t) source;
    // Peripheral: address of DAC->DHR12R1 data register
    DMA2_Channel3->CPAR = (uint32_t) &(DAC->DHR12R1);
    DMA2_Channel3->CCR |= DMA_CCR_EN;
}

/*
 * @brief  DMA interrupt handler for transfer complete events.
 *
 * This function handles the DMA2_Channel3 interrupt triggered by a
 * transfer complete (TC) event. It clears the interrupt flags and
 * sets a global flag (`DMA_FLAG`) to indicate that the transfer has
 * completed.
 *
 * @details
 * - Checks if the transfer complete flag (TCIF) is set for DMA2_Channel3.
 * - Clears the interrupt flags for transfer complete, half-transfer,
 *   and transfer error.
 * - Updates the global `DMA_FLAG` to notify other parts of the application
 *   that the transfer is complete.
 @  SOURCE  Embedded system Fundamentals with Arm cortex M-Based Microcontrollers By DR Alexander G.Dean
 * 			chapter : Direct Memory Access (Listing 9.11)
 */
void DMA1_CH4_5_6_7_DMA2_CH3_4_5_IRQHandler(void) {
    // Red LED on

    if (DMA2->ISR & DMA_ISR_TCIF3) {
        // DMA2 Channel 3 transfer complete, so do something
        // if needed here. Restart, etc
    	DMA_FLAG=1;
    }
    DMA2->IFCR |= DMA_IFCR_CTCIF3 | DMA_IFCR_CHTIF3 | DMA_IFCR_CTEIF3;
    // Red LED off

}


