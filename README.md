## STM32 FreeRTOS LWIP Auto Reconnect

### Intro

Attempt to make Ethernet reconnect after the board is initialized without the cable plugged in. Similar to
https://github.com/xmkllc/STM32_LWIP_Auto_Reconnect but with FreeRTOS.

### Hardware
- STM32F407VGTx Discovery
- STM32F4DIS-BB expansion board (PHY: Lan8742A)

### CubeMX Configuration
- Start without default settings
- Clock: enable HSE (PCLK1 42MHz)
   - Use TIM1 as timebase source
- ETH:
   - RMII
   - PHY addr: 0 (default is wrong)
   - Ethernet global interrupt: enabled
   - Auto negotiation: enabled
- FreeRTOS:
   - Interface: CMSIS_V2
   - Add nic monitoring timer
   - defaultTaks stack size: 512
- LWIP:
   - LWIP_NETIF_LINK_CALLBACKS: enabled
   - LWIP_PERF: enabled
- LEDs: