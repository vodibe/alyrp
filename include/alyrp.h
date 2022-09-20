#ifndef _ALYRP_h
#define _ALYRP_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"

#endif

#include <SPI.h>
#include <SdFat.h>
#include <Regexp.h>

// SD constants
#define SD_CS_PIN 10
#define FILE_NAME_MAX_LENGTH 60
#define LRC_DIR "/lyrics/"
#define LRC_EXT "lrc"
#define LRC_TIMESTAMP_LENGTH 10

// LCD constants
#define LCD_ROWS 4
#define LCD_COLS 20

// LCD navigation constants (editable by the user)
#define LCD_NAV_NEXT 'x'
#define LCD_NAV_OK 'c'

// LCD navigation constants (NOT editable by the user)
#define LCD_NAV_NEXT_OK 'k'

#define LCD_I2C_EXISTS 0

#if LCD_I2C_EXISTS >= 1
    #define LCD_I2C_USED
    #include <LiquidCrystal_I2C.h>
#else
    #include <LiquidCrystal.h>
#endif

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