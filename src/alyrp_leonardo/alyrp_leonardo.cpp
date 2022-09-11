#include "alyrp_leonardo.h"

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

// SD card
SdFat sd;

// current LRC file name being played
char lrcFileName[FILE_NAME_MAX_LENGTH];

// true if selected file has .lrc extension, false otherwise
bool validLrcFile;

// dir where all LRC files are stored
File lrcRootDir;

// LCD display
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

// Setup 
void setup() {
    // Serial initialization
    Serial.begin(9600);
    while (!Serial) { ; }

    // LCD initialization
    lcd.init();
    lcd.backlight();
    lcd.home();
    printLcd("Lyre v.beta", 0, true);

    // SD initialization
    if(!sd.begin(SD_CS_PIN)) {
        printError(61);
    }
    if(!lrcRootDir.open(LRC_DIR, O_RDONLY) | !sd.chdir(LRC_DIR)) {
        printError(62);
    }
    printLcdNav(LCD_NAV_NEXT);

    validLrcFile = false;
}

// Loop
void loop() {
    
    char inByte;

    while (!Serial.available()) { ; }
    inByte = Serial.read();
    if (inByte != -1 && inByte != '\r' && inByte != '\n') {
        browseDir(inByte);
    }
    
}

/** 
 * Browses the next file in the directory named in LRC_DIR.
 */
void browseDir(const char inByte) {
    SdFile sdElem;

    if (inByte == LCD_NAV_NEXT) {
        if (!sdElem.openNext(&lrcRootDir, O_RDONLY)) {
            //end of directory: re-open it in order to browse the first file
            lrcRootDir.close();
            if(!lrcRootDir.open(LRC_DIR, O_RDONLY) | !sd.chdir(LRC_DIR)) {
                printError(62);
            }
            printError(64);
            return;
        }
        //next file exists: display file name
        sdElem.getName(lrcFileName, FILE_NAME_MAX_LENGTH);
        printLcd(lrcFileName, 0, true);

        if (sdElem.isDir()) {
            validLrcFile = false;
            printLcd("Not a file!", 2, false);
            printLcdNav(LCD_NAV_NEXT);
        }
        else {
            validLrcFile = true;
            printLcdNav(LCD_NAV_NEXT_OK);
        }
        sdElem.close();
    }
    else if (inByte == LCD_NAV_OK) {
        if (validLrcFile) {
            startKaraoke(&sdElem);
            sdElem.close();

            //after playing lyrics, display the current file name
            lcd.clear();
            printLcd(lrcFileName, 0, true);
            printLcdNav(LCD_NAV_NEXT_OK);
        }
        return;
    }
    else {
        return;
    }
}


/**
 * Prints an error description, according to its unique identifier.
 * Every error sets a fatal flag in order to need the arduino reboot.
 */
void printError(const short errorId) {
    boolean fatal;
    if     (errorId == 61) {
        printLcd("SD init failed!", 0, true);
        fatal = true;
    }  
    else if(errorId == 62) {
        printLcd("No /lyrics/ folder!", 0, true);
        fatal = true;
    } 
    else if(errorId == 63) {
        printLcd("Error opening file!", 0, true);
        fatal = false;
    }
    else if(errorId == 64) {
        printLcd("End of directory!", 0, true);
        printLcdNav(LCD_NAV_NEXT);
        fatal = false;
    }

    while (fatal) { ; }   
}

/**
 * Clears the screen according to clearlcd flag; Sets the lcd cursor to the
 * lcdRow-th row; prints string on the lcd.
 */
void printLcd(char *string, const uint8_t lcdRow, bool clearLcd) {
    if (clearLcd)
        lcd.clear();

    lcd.setCursor(0, lcdRow);
    for (size_t i = 0; i < strlen(string); i++) {
        lcd.write(string[i]);
    }
}

/**
 * Prints the navigation symbols according to the navId.
 */
void printLcdNav(const char navId) {
    char navString[5] = "[ ] ";

    if (navId == LCD_NAV_NEXT) {
        navString[1] = '>';
        navString[3] = LCD_NAV_NEXT;
        lcd.setCursor(0, LCD_ROWS - 1);
        for (int i = 0; i < LCD_COLS - strlen(navString); i++) {
            lcd.write(' ');
        }
        lcd.print(navString);
    }
    else if (navId == LCD_NAV_NEXT_OK) {
        navString[1] = '*';
        navString[3] = LCD_NAV_OK;
        printLcdNav(LCD_NAV_NEXT);
        lcd.setCursor(LCD_COLS / 2 - strlen(navString) + 2, LCD_ROWS - 1);
        lcd.print(navString);
    }
}

/**
 * Applies regexString to targetString.
 * Returns true if regex pattern applies to string. False otherwise.
 */
boolean matchRegex(char *targetString, char *regexString) {
    MatchState ms;
    char regexMatch;

    ms.Target(targetString);
    regexMatch = ms.Match(regexString);
    if (regexMatch == REGEXP_MATCHED) {
        return true;
    }
    else if (regexMatch == REGEXP_NOMATCH) {
        return false;
    }
    else {
        return false;
    }
}

/**
 * Returns true if the targetString is a valid timestamp that is a string that follows the pattern [MM:ss.mm])
 * Returns false otherwise.
 * 
 * @see regex syntax at this url: http://www.tads.org/t3doc/doc/sysman/regex.htm
 */
boolean isTimestamp(char *targetString) {
    // http://www.tads.org/t3doc/doc/sysman/regex.htm#wordBoundary
    char timestampRegex1[5] = "%b[]";

    char timestampRegex2[16] = "%d%d:%d%d%p%d%d";

    return (matchRegex(targetString, timestampRegex1) && matchRegex(targetString, timestampRegex2));
}

/**
 * Converts timestamp in milliseconds 
 * [12:34.56]  => 12 minutes, 34 seconds, 5 tenth of seconds, 6 hundreds of seconds.
 *                12 * 60 * 1000 milliseconds +
 *                34 * 1000      milliseconds +
 *                5  * 100       milliseconds +
 *                6  * 10        milliseconds
 */
unsigned long timestampToMillis(char *timestamp) {
  return    (   (timestamp[1]-'0') * (unsigned long)10 + (timestamp[2]-'0')    ) * (unsigned long)60000
            +
            (   (timestamp[4]-'0') * (unsigned long)10 + (timestamp[5]-'0')    ) * (unsigned long)1000
            +
            (   (timestamp[7]-'0') * (unsigned long)100 + (timestamp[8]-'0') * (unsigned long)10    );
}

/**
 * Reads the lyrics from sdElemPtr skipping timestamp and LRC ID tags (author, album,...)
 * 
 * @see https://en.wikipedia.org/wiki/LRC_(file_format)
 */
void startKaraoke(SdFile *sdElemPtr) {
    if (!sdElemPtr->open(lrcFileName, O_RDONLY)) {
        printError(63);
    }
    char timestampBuffer[LRC_TIMESTAMP_LENGTH + 1];

    unsigned long currentTime = 0;
    unsigned long lastTime = 0;

    //if true it means that the parser is reading a timestamp
    bool readTimestamp = true;
    //if true it means that the parser must output the read character
    bool printChar = false;

    lcd.clear();

    //timestump buffer index
    int i = 0;
    int16_t c;
    while((c = sdElemPtr->read()) > 0) {
        if(readTimestamp) {
            if(c == '\n') {
                //reset the timestump buffer index when the file current line length is
                //smaller than the timestump length
                i = 0;
                continue;
            }

            timestampBuffer[i] = c;
            i++;

            if(i == LRC_TIMESTAMP_LENGTH) {
                timestampBuffer[LRC_TIMESTAMP_LENGTH]='\0';

                if(isTimestamp(timestampBuffer)) {
                    currentTime = timestampToMillis(timestampBuffer);
                    delay(currentTime-lastTime);
                    lastTime = currentTime;
                    
                    i = 0;
                    printChar = true;
                    lcd.clear();
                }
                else {
                    printChar = false;
                }
                readTimestamp = false;
            }
        }
        else if(c == '\r') {
            continue;
        }
        else if(c == '\n') {
            readTimestamp = true;
            i = 0;
        }
        else if(printChar) {
            lcd.write(c);
        }
    }
}

