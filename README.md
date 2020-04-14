# Benchmarks for SBC

## Synthetic benchmarks 

### GPIO (WiringPi)
* Raspjamming 20.04, WiringPi 2.60.200326:
  - BananaPi M2 Zero v1.0, 1008 MHz: 0,878 us
  - Raspberry Pi Zero v1.3, 1000 MHz: 0,209 us
  - Raspberry Pi Zero v1.3, 1150 MHz: 0,183 us
  
* Raspjamming 20.02, WiringPi 2.52:
  - Raspberry Pi Zero v1.3, 1000 MHz: 0,187 us
  - Raspberry Pi Zero v1.3, 1150 MHz: 0,184 us
 
 
### SPI ADC with MCP3202 (WiringPi-SPI /dev/spidev0.1, 4 MHz)
* Raspjamming 20.04, WiringPi 2.60.200410:
  - BananaPi M2 Zero v1.0, 1008 MHz: 34,721 us
* Raspjamming 20.02, WiringPi 2.52:
  - Raspberry Pi Zero v1.3, 1000 MHz: 51,084 us
  - Raspberry Pi Zero v1.3, 1150 MHz: 45,741 us  
 
## Project benchmarks
  
### SPI DAC/VectorGaming (WiringPi GPIO, SPI via /dev/spidev0.0, 6 MHz) - Pong
* Raspjamming 20.04, WiringPi 2.60.200410:
  - BananaPi M2 Zero v1.0, 1008 MHz: 65 us
* Raspjamming 20.02, WiringPi 2.52:
  - Raspberry Pi Zero v1.3, 1000 MHz: 91 us
  - Raspberry Pi Zero v1.3, 1150 MHz: 77 us

### I2C MCP23017 GBA dumper (WiringPi GPIO, I2C-1 via /dev/i2c-1, 400 kHz) - GB Testris (32 KiB) 
* Raspjamming 20.04, WiringPi 2.60.200410:
  - BananaPi M2 Zero v1.0, 1008 MHz: 475 us
* Raspjamming 20.02, WiringPi 2.52:
  - Raspberry Pi Zero v1.3, 1000 MHz: 788 us
  - Raspberry Pi Zero v1.3, 1150 MHz: 932 us

