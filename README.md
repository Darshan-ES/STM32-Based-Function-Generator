
# Embedded System Waveform Generator and Analyzer

## Overview

This project demonstrates a waveform generation and analysis system built on the STM32F091 microcontroller. It integrates peripherals such as ADC, DAC, DMA, and timers to generate sine waves of configurable frequencies and analyze real-time analog inputs. The system leverages STM32's peripheral capabilities to provide efficient and precise audio signal processing.

---

## Features

### Waveform Generation
- Generates sine waves at standard frequencies:
  - **440 Hz**
  - **587 Hz**
  - **659 Hz**
  - **880 Hz**
- Unified function to dynamically generate sine wave buffers for different frequencies.
- Continuous waveform output using DAC and DMA for seamless signal generation.
- Supports real-time transitions between frequencies without tearing.

### Audio Input and Analysis
- Samples analog signals via ADC on pin **PA4**.
- Processes sampled data to compute:
  - Minimum value
  - Maximum value
  - Average value
- Detects waveform period using **autocorrelation**.

### Peripheral Configurations
- **ADC**: Configured to sample analog signals with external triggering from TIM1.
- **DAC**: Outputs analog signals with DMA-based data transfers.
- **DMA**: Handles efficient memory-to-peripheral data movement for DAC.
- **Timers**:
  - TIM6: Triggers DMA transfers for waveform generation.
  - TIM1: Synchronizes ADC sampling for precise timing.
  - SysTick: Manages state transitions and real-time operations.

### Real-Time State Management
- Implements a state machine to transition between waveforms every 2 seconds.
- Uses SysTick to track elapsed time and trigger state changes.

---

## Project Structure

### **Source Files**

| File               | Description                                                                          |
|--------------------|--------------------------------------------------------------------------------------|
| `analog_in.c`      | Configures and manages ADC for analog input sampling.                               |
| `analog_out.c`     | Configures and manages DAC for analog waveform output.                              |
| `dma.c`            | Initializes and manages DMA transfers for the DAC.                                 |
| `timer_config.c`   | Configures TIM1, TIM6, and SysTick timers for various purposes.                     |
| `wave.c`           | Implements sine wave buffer generation for configurable frequencies.                |

### **Header Files**

| File               | Description                                                                          |
|--------------------|--------------------------------------------------------------------------------------|
| `analog_in.h`      | Header for ADC-related configurations and functions.                                |
| `analog_out.h`     | Header for DAC-related configurations and functions.                                |
| `dma.h`            | Header for DMA configurations and functions.                                       |
| `timer_config.h`   | Header for timer configurations and utility functions.                              |
| `wave.h`           | Header for sine wave generation functions and parameters.                          |

---

## Functional Description

### 1. **Analog Input (ADC)**
- **Pin**: PA4 is configured as an analog input.
- **Sampling**: ADC is triggered by TIM1 to ensure consistent sampling intervals.
- **Buffering**: Samples are stored in a global `ADC_Buffer` for further processing.

#### Key Functions
- `ADC_init`: Initializes the ADC for single-channel operation on PA4.
- `AudioInputModule`: Captures audio samples into the `ADC_Buffer`.

---

### 2. **Waveform Generation (DAC & DMA)**
- **Pin**: PA4 is configured as an analog output.
- **Waveform Output**: Sine waves are generated and sent to the DAC via DMA.
- **Transition Handling**: Smooth frequency transitions without signal tearing.

#### Key Functions
- `Init_DAC`: Configures the DAC for analog output on PA4.
- `DMA_Init`: Sets up DMA for continuous data transfer to the DAC.
- `Start_DMA`: Starts DMA transfers for waveform generation.

---

### 3. **Timer Configuration**
- **TIM6**: Generates periodic triggers for DMA transfers.
- **TIM1**: Synchronizes ADC sampling via external trigger.
- **SysTick**: Tracks elapsed time for state transitions.

#### Key Functions
- `Timer6_Init`: Configures TIM6 for waveform generation timing.
- `Timer1_init`: Sets up TIM1 for ADC triggering.
- `Systick_Init`: Initializes SysTick for 1-second ticks.

---

### 4. **Signal Analysis**
- **Input Analysis**: Computes minimum, maximum, and average values from the ADC buffer.
- **Period Detection**: Uses autocorrelation to detect the waveform period.

#### Key Functions
- `AudioAnalysis`: Processes the ADC buffer to compute signal characteristics.
- `autocorrelate_detect_period`: Determines the period of the sampled waveform.

---

### 5. **State Machine**
- **Functionality**: Transitions between sine wave frequencies every 2 seconds.
- **States**:
  - `SINE_440`
  - `SINE_587`
  - `SINE_659`
  - `SINE_880`
### 5. **Sample /Period Calculation**
 - Sine Wave Generation: Generates sine wave lookup tables for specific frequencies

 -  (440 Hz, 587 Hz, 659 Hz, 880 Hz) using fixed-point math.

 - Waveform Switching: Dynamically changes the current waveform being output via

   DMA by updating the active sine wave table.

 - Macros:

 - Table sizes for each frequency.

 - Sample sizes and constants for scaling and frequency calculation.

 

  The `SAMPLE_SIZE` for each frequency is calculated using the formula:

 *   SAMPLE_SIZE = (1 / frequency) * Sampling_Frequency / 1000

 *

 * The `SINE_TABLE_SIZE` is calculated using:

 *

 *   SINE_TABLE_SIZE = 1024 / (Samples_Per_Period)

 *

 *   The result is multiplied by the `SAMPLE_SIZE`

#### Key Function
- `SysTick_Handler`: Implements the state transition logic.

---

## Usage

### 1. **Hardware Setup**
- Connect an oscilloscope to pin **PA4** to observe the generated waveforms.
- Optionally, connect an analog signal source to PA4 for ADC input testing.


