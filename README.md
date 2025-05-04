# EtherSense

A FreeRTOS-based STM32F103 + W5500 project for streaming real-time sensor data over Ethernet (UDP).

## Features

- STM32F103RB + W5500 Ethernet module
- SPI-based W5500 driver with static IP configuration
- FreeRTOS task for network handling
- Verified ping and basic IP stack communication
- Ready for UDP sensor data transmission

## Architecture

[STM32F103RB] ↔️ SPI ↔️ [W5500] ↔️ RJ45 ↔️ [Ethernet] ⬑ Static IP: 192.168.100.77

