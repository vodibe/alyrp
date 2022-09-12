# ALYRP
Alyrp (Arduino LYRics Player :microphone:) is a `.LRC` (and text files formatted as `.LRC` files) viewer Arduino Leonardo compatible. It requires a SD/MicroSD card and a LCD screen. The source code design follows a PlatformIO project.

## :satellite: Tested devices
- Microcontroller:

  - [x] Arduino Leonardo
  - [ ] Arduino UNO

- SD/MicroSD Card Adapter
  - [x] CATALEX Micro SD Card Module

- LCD Display
  - [ ] LCD 16x2 (with I2C module)
  - [x] LCD 20X4 (with I2C module)

## :electric_plug: Wiring
See `src/alyrp_<MICROCONTROLLER>/wiring` files.

## :arrow_down: Installation
```
git clone <REPOURL>
PIO Home > Projects > Add Existing
```
## :question: Other info
1. SD card file hierarchy
   ```
   |-- lyrics
       |-- song1.lrc
       |-- song2.txt
   ```
2. How to download `.LRC` files?
   
   Take a look at https://github.com/fashni/MxLRC

3. Where to find the LCD address?

    https://create.arduino.cc/projecthub/abdularbi17/how-to-scan-i2c-address-in-arduino-eaadda
