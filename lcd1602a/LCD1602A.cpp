//
//  LCD1602A.cpp
//  lcd-1602a-arduino-lib
//
//  Created by 周栩乐 on 3/4/16.
//  Copyright © 2016 Marcus. All rights reserved.
//

#include "LCD1602A.hpp"

#define VALID_PORT(port) (port >= 0 && port <= 13)

void LCD1602A::initialize(int RS, int RW, int E, const char* dataPorts){
    initialize(RS, RW, E, dataPorts, -1, -1);
}

void LCD1602A::initialize(int RS, int RW, int E, const char* dataPorts, int contrastPort){
    initialize(RS, RW, E, dataPorts, contrastPort, -1);
}

void LCD1602A::initialize(int RS, int RW, int E, const char* dataPorts, int contrastPort, int backgroundLightPort){
    if(!(VALID_PORT(RS) && VALID_PORT(RW) && VALID_PORT(E))){
        this->error |= LCD_EXCEPTION_PORT_INVALID;
        return;
    }
    
    this->RS = RS;
    this->RW = RW;
    this->E = E;
    this->contrastPort = contrastPort;
    this->backgroundLightPort = backgroundLightPort;
    
    int i;
    
    int pointer = 0;
    for(i = 0; i < 8; ++i){//Clean buffer
        parasedPorts[i] = 0;
    }
    
    while (*dataPorts) {
        int port = *dataPorts - '0';
        if(VALID_PORT(port)){
            parasedPorts[pointer++] = port;
        }
        ++dataPorts;
    }
    
    if(pointer == 4){//4-bits mode
        this->mode = LCD_4_PIN_MODE;
    }else if(pointer == 8){//8-bits mode
        this->mode = LCD_8_PIN_MODE;
    }else{//Unknown mode
        this->error |= LCD_EXCEPTION_DATA_PORT_NUM_INVALID;
    }
    
    for(i = 0; i < pointer; ++i){//Initialize all ports
        pinMode(parasedPorts[i], OUTPUT);
    }
    
    if(VALID_PORT(backgroundLightPort)){
        pinMode(backgroundLightPort, OUTPUT);
    }
    
    if(VALID_PORT(contrastPort)){
        pinMode(contrastPort, OUTPUT);
    }
    
    pinMode(RS, OUTPUT);
    pinMode(RW, OUTPUT);
    pinMode(E, OUTPUT);
    
    delay(100);
    
    setVisible(1);
    setCursor(0);
    setBlinkCursor(0);
    
    sendWorkingMode();
    sendInputMode(1, 0);
    sendDisplayFlags();
    sendCommand(0x80);//Buffer from zero
    clearDisplay();
    
    this->initialized = 1;
}

void LCD1602A::sendCommand(int flags){
    digitalWrite(RS, LOW);
    digitalWrite(RW, LOW);
    sendData(flags);
    delay(50);
}

void LCD1602A::sendCharacter(char character){
    error |= initialized ? 0 : LCD_EXCEPTION_NOT_INITIALIZED;
    digitalWrite(RS, HIGH);
    digitalWrite(RW, LOW);
    sendData(character);
}

void LCD1602A::sendData(char value){
    int i, temp;
    
    if(mode == LCD_4_PIN_MODE){//Write command in 4-bits
        digitalWrite(E, LOW);
        
        temp = value & 0xf0;
        for(i = 0; i <= 4; i++){
            digitalWrite(parasedPorts[i], temp & 0x80);
            temp <<= 1;
        }
        
        digitalWrite(E, HIGH);
        delayMicroseconds(1);
        digitalWrite(E, LOW);
        
        temp = (value & 0x0f) << 4;
        for(i = 0; i <= 4; i++){
            digitalWrite(parasedPorts[i], temp & 0x80);
            temp <<= 1;
        }
        
        digitalWrite(E, HIGH);
        delayMicroseconds(1);
        digitalWrite(E, LOW);
    }else{
        for(i = 0; i < 8; i++){
            digitalWrite(parasedPorts[i], value & 0x01);
            value >>= 1;
        }
        
        digitalWrite(E, LOW);
        delayMicroseconds(1);
        digitalWrite(E, HIGH);
        delayMicroseconds(1);
        digitalWrite(E, LOW);
        delayMicroseconds(1);
    }
}

void LCD1602A::setContrast(double contrast){
    if(VALID_PORT(contrastPort)){
        analogWrite(contrastPort, (uint8_t)(260 + (contrast * 100)));
    }
}

void LCD1602A::setBackgroundLight(int status){
    if(VALID_PORT(backgroundLightPort)){
        digitalWrite(backgroundLightPort, status);
    }
}

void LCD1602A::clearDisplay(){
    sendCommand(0x01);
}

void LCD1602A::setCursor(int enable){
    if(enable){
        displayModeFlags |= LCD_DISPLAY_FLAG_SHOW_CURSOR;
    }else{
        displayModeFlags &= ~(LCD_DISPLAY_FLAG_SHOW_CURSOR);
    }
}

void LCD1602A::setBlinkCursor(int enable){
    if(enable){
        displayModeFlags |= LCD_DISPLAY_FLAG_BLINK_CURSOR;
    }else{
        displayModeFlags &= ~(LCD_DISPLAY_FLAG_BLINK_CURSOR);
    }
}

void LCD1602A::sendDisplayFlags(){
    sendCommand(displayModeFlags | LCD_DISPLAT_FLAG);
}

void LCD1602A::sendWorkingMode(){
    sendDataMode(mode, 1, 0);
}

void LCD1602A::sendInputMode(int moveRight, int staticCursor){
    sendCommand(0x04 | (moveRight ? 0x02 : 0) | (staticCursor ? 0x01 : 0));
}

void LCD1602A::sendDataMode(int fourBitsMode, int doubleLine, int hd){
    int flags;
    flags = 0x20;//Set working mode command
    flags |= fourBitsMode == LCD_4_PIN_MODE ? LCD_4_PIN_MODE : LCD_8_PIN_MODE;
    flags |= doubleLine ? LCD_DOUBLE_LINE : 0;
    flags |= hd ? LCD_HD_DISPLAY : 0;
    this->doubleLine = doubleLine;
    sendCommand(flags);
}

void LCD1602A::setVisible(int value){
    displayModeFlags |= LCD_DISPLAY_FLAG_ENABLE;
}

void LCD1602A::seekCursor(int index, int line){
    int address;
    address = line == 0 ? 0x80 | index : 0xc0 | index;
    error |= initialized ? 0 : LCD_EXCEPTION_NOT_INITIALIZED;//Check if is initialized
    error |= (!doubleLine && line) ? LCD_EXCEPTION_SINGLE_LINE_ONLY : 0;//Check single line
    sendCommand(address);
}

void LCD1602A::sendString(const char *string){
    while (*string) {
        sendCharacter(*string);
        ++string;
    }
}

void LCD1602A::resetCursor(){
    sendCommand(0x02);
}
