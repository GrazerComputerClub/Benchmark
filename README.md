# Benchmarks for SBC


## GPIO (WiringPi)
* Raspjamming 20.04, WiringPi 2.60.200326:
  - BananaPi M2 Zero v1.0, 1008 MHz: 0,878 us
  - Raspberry Pi Zero v1.3, 1000 MHz: 0,209 us
  - Raspberry Pi Zero v1.3, 1150 MHz: 0,183 us

 
## SPI ADC with MCP3202 (WiringPi-SPI /dev/spidev0.1, 4 MHz)
* Raspjamming 20.04, WiringPi 2.60.200410:
  - BananaPi M2 Zero v1.0, 1008 MHz: 30.867 us
  
  
## SPI ADC/VectorGaming (WiringPi GPIO, SPI via /dev/spidev0.0, 6 MHz) - Pong
* Raspjamming 20.04, WiringPi 2.60.200410:
  - BananaPi M2 Zero v1.0, 1008 MHz: 65 us
* Raspjamming 20.04, WiringPi 2.52:
  - Raspberry Pi Zero v1.3, 1000 MHz: 85 us
  - Raspberry Pi Zero v1.3, 1150 MHz: 72 us
