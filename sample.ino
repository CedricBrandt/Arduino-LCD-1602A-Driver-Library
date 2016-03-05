//
// lcd-1602a-arduino-lib
//
// Description of the project
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author 		周栩乐
// 				Marcus
//
// Date			3/4/16 10:22 PM
// Version		<#version#>
//
// Copyright	© 周栩乐, 2016
// Licence		<#licence#>
//
// See         ReadMe.txt for references
//


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

#include "lcd1602a/LCD1602A.hpp"
#include "lcd1602a/LCD1602A.cpp"

LCD1602A lcd;

void setup(){
    Serial.begin(9600);
    lcd.initialize(12, 11, 10, "6789", 5, 4);
    Serial.println(lcd.error);
    Serial.println(lcd.getMode());
    lcd.setBackgroundLight(1);
    lcd.setContrast(0.5);
}

void loop(){
    delay(300);
    lcd.clearDisplay();
    lcd.seekCursor(2, 0);
    lcd.sendString("Hello there");
    lcd.seekCursor(1, 1);
    lcd.sendString("This is Marcus");
    delay(3000);
    lcd.clearDisplay();
    lcd.seekCursor(0, 0);
    lcd.sendString("Your LCD");
    lcd.seekCursor(0, 1);
    lcd.sendString("is working");
    delay(3000);
}
