# Alyrp
Alyrp (Arduino LYRics Player :microphone:) is a `.LRC` (and text files formatted as `.LRC` files) viewer for Arduino UNO and Arduino Leonardo. It requires a SD/MicroSD card and a LCD screen. The source code design follows a PlatformIO project.

## :satellite: Supported components
- 1x Microcontroller:
  - Arduino Leonardo
  - Arduino UNO
  
- 1x SD/MicroSD Card Adapter
  - CATALEX Micro SD Card Module

- 1x LCD Display (I2C module is optional)
  - LCD 16x2
  - LCD 20x4 (recommended)

## :electric_plug: Wiring
See `/src/wiring` files.

## :arrow_down: Installation

1. `git clone` this repo
2. Install [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) extension in VS Code
3. Open VS Code
4. *PIO Home* > *Projects* > *Add Existing* > Select downloaded folder in step 1.

## :warning: Error guide
todo

## :question: Other info
1. SD card file hierarchy
   ```
   |-- lyrics (mandatory)
   |   |-- song1.lrc
   |   |-- ...
   |-- (other optional directories)
   ```
2. What is a `.LRC` file? 

   https://en.wikipedia.org/wiki/LRC_(file_format)
   Note that right now, the code ignores all the tags id, including the `[offset:+/-<OFFSET>]` one.
   
2. How to download `.LRC` files of my favourite songs?
   
   `.LRC` file may be included with the song. It usually has the same name of the song itself. You can also take a look at https://github.com/fashni/MxLRC
    
4. Extended ASCII table

    https://www.ascii-code.com/
