#ifndef _LYRE_LEONARDO_h
    #define _LYRE_LEONARDO_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"

#endif

#include <SPI.h>
#include <SdFat.h>
#include <LiquidCrystal_I2C.h>
#include <Regexp.h>

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