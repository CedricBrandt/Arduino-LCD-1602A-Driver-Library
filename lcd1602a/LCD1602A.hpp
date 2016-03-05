//
//  LCD1602A.hpp
//  lcd-1602a-arduino-lib
//
//  Created by 周栩乐 on 3/4/16.
//  Copyright © 2016 Marcus. All rights reserved.
//

#ifndef LCD1602A_hpp
#define LCD1602A_hpp

// Core library for code-sense - IDE-based
#if defined(WIRING) // Wiring specific
#   include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#   include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#   include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#   include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad specific
#   include "Energia.h"
#elif defined(LITTLEROBOTFRIENDS) // LittleRobotFriends specific
#   include "LRF.h"
#elif defined(MICRODUINO) // Microduino specific
#   include "Arduino.h"
#elif defined(SPARK) || defined(PARTICLE) // Particle / Spark specific
#   include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#   include "Arduino.h"
#elif defined(REDBEARLAB) // RedBearLab specific
#   include "Arduino.h"
#elif defined(ESP8266) // ESP8266 specific
#   include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#   include "Arduino.h"
#else // error
#   error Platform not defined
#endif // end IDE

#define LCD_4_PIN_MODE 0x00
#define LCD_8_PIN_MODE 0x10
#define LCD_DOUBLE_LINE 0x08
#define LCD_HD_DISPLAY 0x04

#define LCD_DISPLAT_FLAG 0x08
#define LCD_DISPLAY_FLAG_ENABLE 0x04
#define LCD_DISPLAY_FLAG_SHOW_CURSOR 0x01
#define LCD_DISPLAY_FLAG_BLINK_CURSOR 0x02

#define LCD_EXCEPTION_DATA_PORT_NUM_INVALID 0x03
#define LCD_EXCEPTION_PORT_INVALID 0x01
#define LCD_EXCEPTION_NOT_INITIALIZED 0x04
#define LCD_EXCEPTION_SINGLE_LINE_ONLY 0x08

class LCD1602A{
public:
    int error;
    void initialize(int RS, int RW, int E, const char* dataPorts);
    void initialize(int RS, int RW, int E, const char* dataPorts, int contrastPort);
    void initialize(int RS, int RW, int E, const char* dataPorts, int contrastPort, int backgroundLightPort);
    int getMode(){return mode;}
    void sendCommand(int flags);
    void sendCharacter(char character);
    void sendString(const char* string);
    void sendData(char value);
    void setBackgroundLight(int status);
    void setContrast(double contrast);
    void clearDisplay();
    void setCursor(int enable);
    void setBlinkCursor(int enable);
    void sendDisplayFlags();
    void sendWorkingMode();
    void sendInputMode(int moveRight, int staticCursor);
    void sendDataMode(int fourBitsMode, int doubleLine, int hd);
    void sendDataMode(int fourBitsMode){sendDataMode(fourBitsMode, 1, 1);}
    void setVisible(int value);
    void resetCursor();
    void seekCursor(int index, int line);
private:
    int parasedPorts[8];
    int mode;
    int backgroundLightPort;
    int contrastPort;
    int initialized;
    int RS, RW, E;
    int displayModeFlags;
    int inputMode;
    int doubleLine;
};

#endif /* LCD1602A_hpp */
