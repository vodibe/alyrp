# ALYRP
Alyrp (Arduino LYRics Player :microphone:) is a `.LRC` (and text files formatted as `.LRC` files) viewer Arduino Leonardo compatible. It requires a SD/MicroSD card and a LCD screen. The source code design follows a PlatformIO project.

## :satellite: Required components
- [x] Tested
- [ ] Not tested yet

- 1x Microcontroller:

  - [x] Arduino Leonardo
  - [ ] Arduino UNO

- 1x SD/MicroSD Card Adapter
  - [x] CATALEX Micro SD Card Module

- 1x LCD Display
  - [x] LCD 16x2 (with I2C module)
  - [x] LCD 20X4 (with I2C module) (recommended)

## :electric_plug: Wiring
See `/src/alyrp_<MICROCONTROLLER>/wiring` files.

## :arrow_down: Installation

1. `git clone <REPOURL>`
2. Install [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) extension in VS Code
3. Open VS Code
4. PIO Home > Projects > Add Existing > Selected downloaded folder in step 1.

## :question: Other info
1. SD card file hierarchy
   ```
   |-- lyrics
       |-- song1.lrc
       |-- ...
   ```
2. How to download `.LRC` files of my favourite songs?
   
   Take a look at https://github.com/fashni/MxLRC

3. Where to find the LCD address?

    https://create.arduino.cc/projecthub/abdularbi17/how-to-scan-i2c-address-in-arduino-eaadda
