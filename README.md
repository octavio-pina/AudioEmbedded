# AudioEmbedded

A bare-metal WAV player on the **STM32F411RE** (NUCLEO-F411RE). It reads a WAV file from a microSD card through a hand-written FAT32 layer and streams the PCM samples over **I²S** to a **PCM5102A** DAC.

All peripheral drivers are written from scratch at register level. **No HAL, no LL and no FatFs.** The goal is to understand the full path from a file on the SD card to an audio sample at the DAC: registers, clocks, memory, the filesystem, buffering, interrupts and timing.

> **Status:** work in progress. See [Project status](#project-status).

---

## System overview

```
microSD ──SPI2──► SD driver ──► FAT32 ──► file reader ──► WAV parser ──► PCM frames
                                                                            │
speakers ◄── 3.5 mm AUX ◄── PCM5102A ◄──── I²S5 (Philips, 16-bit, 48 kHz) ◄─┘
```

| Layer | Modules |
|---|---|
| Application | `main.c`: board bring-up, playback loop, debug output |
| BSP / middleware | `sd_card`, `fat32`, `wav` |
| Drivers | `gpio`, `spi`, `i2s`, `uart`, `timer`, `nvic`, `ring_buffer` |
| Device header | `stm32f411xx.h`: register maps and base addresses |

## Hardware

| Part | Role |
|---|---|
| NUCLEO-F411RE (STM32F411RE, Cortex-M4F) | MCU board, on-board ST-Link for flashing, debugging and the virtual COM port |
| microSD module (SPI) | Stores the WAV files (SDHC, FAT32) |
| PCM5102A breakout | Stereo I²S DAC |
| Active speakers | Analog output through the DAC's 3.5 mm jack |

### Pin map

| Function | Peripheral | Pins | Notes |
|---|---|---|---|
| SD card | SPI2 | PB13 SCK, PB14 MISO, PB15 MOSI (AF5), PB12 CS (GPIO) | Mode 0, 8-bit, MSB first, software NSS |
| Audio | SPI5 / I²S5 | PB0 CK → BCK, PB1 WS → LRCK, PB8 SD → DIN (AF6) | Master TX, no MCLK output |
| Debug console | USART2 | PA2 TX, PA3 RX (AF7) | 115200 8N1, routed to the ST-Link virtual COM port |
| User button | GPIO / EXTI | PC13 | Falling-edge interrupt, debounced with TIM9 |
| Status LEDs | GPIO | PA5, PA6, PA7 | |

### PCM5102A configuration (breakout jumpers)

| Pin | Level | Meaning |
|---|---|---|
| FLT | Low | Normal-latency filter |
| DEMP | Low | De-emphasis off |
| XSMT | High | Soft mute off |
| FMT | Low | I²S format |
| SCK | GND | No external system clock; the DAC uses its internal PLL from BCK |

## Clocking

- **SYSCLK:** HSI at 16 MHz (reset default; the main PLL is not used yet).
- **I²S clock:** PLLI2S fed from HSI, with M = 8, N = 238, R = 5 (I2SCLK ≈ 95.2 MHz).
- **I²S prescaler:** I2SDIV = 31, ODD = 0, 16-bit frames:
  Fs = I2SCLK / (32 × (2 × I2SDIV + ODD)) ≈ **47.98 kHz**.

> These PLLI2S values put the VCO at 476 MHz, above the 432 MHz maximum in RM0383. They work on the bench, but they are being reworked to stay inside the specified range.

## Software modules

### Drivers (`Src/Drivers`, `Inc/Drivers`)

- **GPIO:** mode, output type, speed, pull-up/down, alternate function, EXTI interrupts.
- **SPI:** blocking and interrupt-driven transfers, plus byte-level helpers used by the SD layer.
- **I²S:** PLLI2S setup and I²S configuration on the SPI blocks: mode, standard, data and channel length, clock polarity, prescaler.
- **USART:** blocking TX, interrupt-driven RX into a ring buffer.
- **Timer:** basic timer with an update interrupt (TIM9, used for debouncing).
- **NVIC:** IRQ enable and priority helpers.
- **Ring buffer:** byte FIFO for USART RX.

### BSP (`Src/BSP`, `Inc/BSP`)

- **`sd_card`:** SPI-mode SD initialization (CMD0, CMD8, CMD55/ACMD41, CMD58) and block reads (CMD17), with a single-block and a multi-block read API. SDHC block addressing.
- **`fat32`:** a minimal FAT32 reader:
  - parses the BPB (bytes per sector, sectors per cluster, reserved sectors, FAT count and size, root cluster);
  - searches the root directory for 8.3 short names (long-file-name entries are skipped);
  - follows the FAT cluster chain, so files don't need to be contiguous;
  - `ReadNextBlock()` returns the file one 512-byte sector at a time and limits the last block to the file size.
- **`wav`:** a RIFF/WAVE parser:
  - validates the `RIFF` and `WAVE` tags;
  - walks the chunks, reads `fmt `, skips unknown chunks (for example `LIST`) and finds `data`;
  - `WAV_ReadFrame()` returns one stereo 16-bit frame (left, right) and pulls new sectors as it goes.

### Supported audio format

| Property | Value |
|---|---|
| Container | RIFF / WAVE |
| Encoding | PCM (format 1) |
| Channels | 2 |
| Sample rate | 48 kHz |
| Bits per sample | 16, little-endian, two's complement |
| File names | 8.3 short names (for example `DARDOS.WAV`) |

## Build and flash

### Requirements

- Arm GNU Toolchain (`arm-none-eabi-gcc`)
- CMake 3.20+ and Ninja
- OpenOCD or STM32CubeProgrammer (for the ST-Link)
- Optional: VS Code with the STM32Cube extension, or STM32CubeIDE (project files in `AudioEmbbedPrj/`)

### Build

```bash
cmake --preset Debug
cmake --build --preset Debug
```

The output is `build/Debug/AudioEmbdPrj.elf`. Use the `Release` preset for an optimized build.

### Flash with OpenOCD

```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
        -c "program build/Debug/AudioEmbdPrj.elf verify reset exit"
```

### Debug console

Open the Nucleo's virtual COM port at **115200 8N1**, for example with PuTTY. The firmware prints hex dumps and status messages there.

## Repository layout

```
.
├── Inc/
│   ├── BSP/               fat32.h, sd_card.h, wav.h
│   ├── Drivers/           gpio, spi, i2s, uart, timer, ring_buffer headers
│   ├── stm32f411xx.h      register definitions
│   └── stm32f411_drivers.h
├── Src/
│   ├── BSP/               fat32.c, sd_card.c, wav.c
│   ├── Drivers/           driver sources
│   ├── main.c
│   ├── syscalls.c, sysmem.c
├── Startup/               startup_stm32f411retx.s
├── cmake/                 toolchain and build settings
├── CMakeLists.txt
├── CMakePresets.json
└── STM32F411RETX_FLASH.ld
```

## Project status

| Stage | Status |
|---|---|
| GPIO, SPI, USART, timer, NVIC drivers | ✅ Done |
| SD card over SPI (init and block read) | ✅ Done |
| FAT32: BPB, root directory, cluster chain | ✅ Done |
| I²S5 + PLLI2S + PCM5102A: test tone audible | ✅ Done |
| WAV parser and frame reader | 🟡 Written, being verified on hardware |
| PCM playback from a RAM buffer | 🟡 In progress |
| WAV playback from SD (polling) | ⬜ Next |
| Measure SD read latency against the audio deadline | ⬜ Planned |
| DMA (DMA2, SPI5_TX) with double buffering for gapless playback | ⬜ Planned |
| SSD1306 OLED and buttons for file selection and status | ⬜ Planned |
| Migration to FreeRTOS (SD, audio and UI tasks) | ⬜ Planned |

### Known limitations

- Playback uses CPU polling of `TXE`. While a new SD sector is being read, the I²S transmitter isn't fed, so audible gaps are expected until DMA double buffering is in place.
- One 512-byte sector holds about 2.67 ms of audio at 48 kHz stereo 16-bit.
- Only 8.3 file names and the root directory are supported.
- Only 16-bit stereo PCM at 48 kHz is handled.

## References

- RM0383: STM32F411xC/E reference manual
- DS10314: STM32F411xC/E datasheet
- PM0214: STM32 Cortex-M4 programming manual
- UM1724: STM32 Nucleo-64 boards user manual
- TI PCM510xA datasheet (PCM5102A)
- SD Association: Physical Layer Simplified Specification
- Microsoft: FAT32 File System Specification
- RIFF / WAVE format (Multimedia Programming Interface and Data Specifications 1.0)

## Author

Octavio Piña
