# ALYRP
Alyrp (Arduino LYRics Player) è un visualizzatore di file `.LRC` (o file di testo formattati come file `.LRC`) per Arduino Leonardo. Richiede una scheda SD/MicroSD e uno schermo LCD. Il codice sorgente segue la struttura di un progetto PlatformIO.

## :satellite: Supported Components
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
## :question: FAQ
1. Dove ottenere i file `.LRC`?
   
   https://github.com/fashni/MxLRC
