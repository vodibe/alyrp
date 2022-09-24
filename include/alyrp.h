#ifndef _ALYRP_h
#define _ALYRP_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#endif

//#include <SPI.h>
#include <SdFat.h>
#include <Regexp.h>
#include <Bounce2.h>

//--- COMMENT IF I2C MODULE HAS NOT BEEN INSTALLED ---
#define LCD_I2C_EXISTS

#ifdef LCD_I2C_EXISTS
    #include <LiquidCrystal_I2C.h>
    //--- SET I2C MODULE ADDRESS --- 
    //i2c scanner at /test/i2c-scanner.c
    #define LCD_I2C_ADDR 0x27
#else
    //note that <LiquidCrystal.h> refers to /lib/LiquidCrystal library
    #include <LiquidCrystal.h>
    //--- SET LCD PINOUT AT alyrp.cpp ---
#endif

// LCD constants
//--- SET YOUR LCD DISPLAY ROWS AND COLS NUMBER ---
#define LCD_ROWS 4
#define LCD_COLS 20

// SD chip select pin
//--- SET YOUR SD CHIP SELECT PIN ---
#define SD_CS_PIN 10

// File name max length
#define FILE_NAME_MAX_LENGTH 60
// Name of directory containing all the lrc files
#define LRC_DIR "/lyrics/"
// Given a timestamp such as [xx:yy.zz], 10 is its length
#define LRC_TIMESTAMP_LENGTH 10

// LCD navigation constants
#define LCD_NAV_NEXT 'x'
#define LCD_NAV_OK 'c'
#define LCD_NAV_NEXT_OK 'k'

// Buttons pinout
// --- SET BUTTONS PINS ---
#define BTN1_PIN 9
#define BTN2_PIN 2

// Navigation buttons constants
#define DEBOUNCE_DELAY 50

// Prototypes
void browseDir(const char inByte);
bool openPrevious(SdBaseFile &folder, SdFile &file, uint8_t oflag);

void printError(const short errorId);
void printLcd(char *string, const uint8_t lcdRow, bool clearLcd);
void printLcdNav(const char navId);

boolean matchRegex(char *targetString, char *regexString);
boolean isTimestamp(char *targetString);
unsigned long timestampToMillis(char *timestamp);

void startKaraoke(SdFile *sdElemPtr);
#endif