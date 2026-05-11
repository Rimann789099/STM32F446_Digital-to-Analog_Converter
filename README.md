STM32 DAC Control (CMSIS)

A bare-metal C project for the STM32F446 demonstrating precise analog output control using register-level programming (CMSIS) instead of HAL.
Features

    Manual Voltage Control: Cycles through voltage levels on each BOOT0 button press.
    Low-Level Implementation: Direct register access for DAC, GPIO, and RCC for maximum efficiency.
    Serial Telemetry: Custom USART library to stream real-time voltage data via Virtual COM Port (VCP).
    Interrupt Handling: Uses NVIC for responsive button debouncing.

Tech Stack

    Hardware: STM32F446RE (Cortex-M4)
    Language: C
    Standard: CMSIS
    Peripherals: DAC (12-bit), GPIO, USART, RCC
