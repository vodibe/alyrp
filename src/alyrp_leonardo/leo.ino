#include <SPI.h>
#include "SdFat.h"
#include <LiquidCrystal_I2C.h>
#include <Regexp.h>

//SD constants
#define SD_CS_PIN 10
#define FILE_NAME_MAX_LENGTH 20
#define LRC_EXT "lrc"
#define FILE_LINE_MAX_LENGTH 100
#define LRC_TIMESTAMP_LENGTH 10
//LCD constants
#define LCD_ROWS 4
#define LCD_COLS 20
//LCD navigation constants (editable by the user)
#define LCD_NAV_PREV 'z'
#define LCD_NAV_NEXT 'x'
#define LCD_NAV_OK 'c'
//LCD navigation constants (NOT editable by the user)
#define LCD_NAV_PREV_NEXT_OK 'k'
#define LCD_NAV_PREV_NEXT 'i'


//dir where all LRC files are stored
SdFile lrcRootDir;
//current LRC file name being played
char lrcFileName[FILE_NAME_MAX_LENGTH];
//true if selected file has .lrc extension, false otherwise
bool validLrcFile;

//LCD display
LiquidCrystal_I2C lcd(0x27,LCD_COLS,LCD_ROWS);

void setup() {
  //SD card
  SdFat sd;
  
  //Serial initialization
  Serial.begin(9600);
  while(!Serial);

  //LCD initialization
  lcd.init();
  lcd.backlight();
  lcd.home();
  printLcd("Lyrics-ino v.beta",0,true);

  //SD initialization
  if(!sd.begin(SD_CS_PIN)) {
    printError(61);
  }
  if(!lrcRootDir.open("/lyrics/", O_READ)) {
    printError(62);
  }
  printLcd("Sd check OK",1,false);

  validLrcFile = false;
}


void loop() {
  char inByte;
  
  while(!Serial.available()) { ; }
  inByte = Serial.read();
  if(inByte != -1 && inByte != '\r' && inByte != '\n') {
    selectLrcFile(inByte);
  }
    
}

void selectLrcFile(char inByte) {
  Serial.println(inByte);
  SdFile sdElem;
  SdFile * lrcRootDirPtr = &lrcRootDir;
  if (inByte == LCD_NAV_PREV) {
    if(!openPrevious(lrcRootDir, sdElem, O_READ)) {
      printLcd("No previous file!",1,true);
      printLcdNav(LCD_NAV_NEXT);
      return;
    }
  }
  else if(inByte == LCD_NAV_NEXT) {
    Serial.println("eseguendo sdElem.openNext()");
    if(!sdElem.openNext(lrcRootDirPtr, O_READ)) {
      Serial.println("false");
      printLcd("No more files!",1,true);
      printLcdNav(LCD_NAV_PREV);
      return;
    }
    else {
      Serial.println("true");
    }
  }
  else if(inByte == LCD_NAV_OK) {
    if(validLrcFile) {
      getLyricsLinesFromFile(&sdElem);
      sdElem.close();
    }
    else {
      printLcd("NOPE",1,true);
    }
    return;
  }
  else {
    return;
  }

  //sdElem.getName(lrcFileName, FILE_NAME_MAX_LENGTH);
  //printLcd(lrcFileName,1,true);
  printLcd("ciaone",1,true);
  
  if(sdElem.isDir() || strncmp(lrcFileName[(strlen(lrcFileName)-strlen(LRC_EXT))], LRC_EXT, strlen(LRC_EXT)) != 0) {
    validLrcFile = false;
    printLcd("Not a file!",1,true);
    printLcdNav(LCD_NAV_PREV_NEXT);
  }
  else {
    validLrcFile = true;
    printLcdNav(LCD_NAV_PREV_NEXT_OK);
  }
  sdElem.close();
}

/*
 * Given a file in a certain folder, opens the previous file with oflag flags.
 * see https://forum.arduino.cc/t/sdfat-openprevious/195910
 */
bool openPrevious(SdBaseFile& folder, SdFile &file, uint8_t oflag) {
  // dir size is 32.
  uint16_t index = folder.curPosition()/32;
  if (index < 2) return false;
  // position to possible previous file location.
  index -= 2;
  do {
    if (file.open(&folder, index, oflag)) return true;
  } while (index-- > 0);
  return false;
}

/*
 * Error manager. According to the errorId, prints the error description.
 */
void printError(const short errorId)
{
  if     (errorId == 61)
    printLcd("SD initialization failed!",0,true);
  else if(errorId == 62)
    printLcd("No /lyrics/ folder!",0,true);
  else if(errorId == 63)
    printLcd("Error opening LRC file!",0,true);
  while(true);
}


void printLcd(char * stringToPrint, const uint8_t lcdRow, bool clearLcd) {
  if(clearLcd)
    lcd.clear();

  lcd.setCursor(0,lcdRow);
  for(int i=0; i< strlen(stringToPrint); i++) {
    lcd.write(stringToPrint[i]);
  }
}

void printLcdNav(char navId) {
  char navString[5] = "[ ] ";
  
  if(navId == LCD_NAV_PREV) {
    navString[1]='<';
    navString[3]=LCD_NAV_PREV;
    lcd.setCursor(0,LCD_ROWS - 1);
    lcd.print(navString);
  }
  else if(navId == LCD_NAV_NEXT) {
    navString[1]='>';
    navString[3]=LCD_NAV_NEXT;
    lcd.setCursor(0,LCD_ROWS - 1);
    for(int i=0; i<LCD_COLS - strlen(navString); i++) {
      lcd.write(' ');
    }
    lcd.print(navString);
  }
  else if(navId == LCD_NAV_PREV_NEXT_OK) {
    navString[1]='*';
    navString[3]=LCD_NAV_OK;
    printLcdNav(LCD_NAV_NEXT);
    printLcdNav(LCD_NAV_PREV);
    lcd.setCursor(LCD_COLS/2 - strlen(navString) + 2, LCD_ROWS - 1);
    lcd.print(navString);
  }
  else if(navId == LCD_NAV_PREV_NEXT) {
    printLcdNav(LCD_NAV_NEXT);
    printLcdNav(LCD_NAV_PREV);
  }
}

/* 
 * Applies regex (stored in regexString array) to string (stored in stringToCheck array).
 * Returns true if regex pattern applies to string. False otherwise.
 */
boolean applyRegex(char * stringToCheck, char * regexString)
{
  MatchState ms;
  char regexMatch;
  
  ms.Target(stringToCheck);
  regexMatch = ms.Match(regexString);
  Serial.println("...");
    Serial.println(stringToCheck);
    Serial.println(regexString);
  if (regexMatch == REGEXP_MATCHED) {
    Serial.println("******ok");
    return true;
  }
  else if (regexMatch == REGEXP_NOMATCH) {
    Serial.println("******no");
    return false;
  }
  else {
    Serial.println("******errrore");
    return false;
  }
}


/* 
 * Input: pointer of the string to check
 * Output: true (the string is a valid timestamp. It means that the string follows the pattern [MM:ss.mm])
 *         false otherwise
 * Since this function uses the Nick Gammon's Regexp library you can find the documentation at this url:
 * http://www.tads.org/t3doc/doc/sysman/regex.htm
 */
boolean isTimestamp(char * stringToCheck)
{
  //http://www.tads.org/t3doc/doc/sysman/regex.htm#wordBoundary
  char timestampRegex1[5] = "%b[]";
              
  char timestampRegex2[16] = "%d%d:%d%d%p%d%d";
  return (applyRegex(stringToCheck, timestampRegex1) &&
          applyRegex(stringToCheck, timestampRegex2) );
}


/* 
 * This function counts all valids lyric lines in the file.
 * All the ID tags (authors, albums...) are skipped.
 * More info on https://en.wikipedia.org/wiki/LRC_(file_format)
 */
void getLyricsLinesFromFile(SdFile * sdElemPtr)
{
  if(!sdElemPtr->open(lrcFileName, O_READ)) {
    Serial.println("errore file");
    return;
  }
  unsigned int lyricsLines = 0;
  char lyricsLine[FILE_LINE_MAX_LENGTH];
  char lyricsTimestamp[LRC_TIMESTAMP_LENGTH + 1];
  
  int i=0;
  char c;
  /*
  while((c = sdElemPtr->read()) >=0) {
    Serial.print(c);
  }
  */
  Serial.print("finito");
  
  
}
