/* mbed ANSI/VT100 Terminal Library
 * Copyright (c) 2007-2009 sford
 * Released under the MIT License: http://mbed.org/license/mit
 */

#include "Terminal.h"
#include "Arduino.h"

#define ASCII_BLOCK     219
#define ASCII_BORDER_H  205
#define ASCII_BORDER_V  186
#define ASCII_BORDER_TL 201
#define ASCII_BORDER_TR 187
#define ASCII_BORDER_BL 200
#define ASCII_BORDER_BR 188
#define WIDTH 30

Terminal::Terminal()  {}

void Terminal::printf(const char* format, ...)
{
    char buffer[80];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(buffer, format, argptr);
    va_end(argptr);
    Serial.print(buffer);
}

void Terminal::cls() {
    Serial.print("\033[2J");
}

void Terminal::clear_to_eol() {
       Serial.print("\033[K"); 
}

void Terminal::HideCursor() {
    locate(50, 0);
}

void Terminal::locate(int column, int row) {
    // Cursor Home    <ESC>[{ROW};{COLUMN}H
    printf("\033[%d;%dH", row + 1, column + 1);
}

static int rgb888tobgr111(uint32_t colour) {
    int r = (colour >> 23) & 1;
    int g = (colour >> 15) & 1;
    int b = (colour >> 7) & 1;
    return (b << 2) | (g << 1) | (r << 0);
}

void Terminal::foreground(uint32_t colour) {
    // Set Attribute Mode    <ESC>[{n}m
    // Foreground Colours : 30 + bgr
    int c = 30 + rgb888tobgr111(colour);
    printf("\033[%dm", c);
}

void Terminal::background(uint32_t colour) {
    // Set Attribute Mode    <ESC>[{n}m
    // Background Colours : 40 + bgr
    int c = 40 + rgb888tobgr111(colour);
    printf("\033[%dm", c);
}

void Terminal::box(int x, int y, int w, int h) { 
     // corners
    locate(x, y);
    Serial.write(ASCII_BORDER_TL);
    locate(x + w - 1, y);
    Serial.write(ASCII_BORDER_TR);
    locate(x, y + h - 1);
    Serial.write(ASCII_BORDER_BL);
    locate(x + w - 1, y + h - 1);
    Serial.write(ASCII_BORDER_BR);
    
    // top
    locate(x + 1, y);
    for(int i=0; i<(w-2); i++){
        Serial.write(ASCII_BORDER_H);
    }
    
    // bottom
    locate(x + 1, y + h - 1);
    for(int i=0; i<(w-2); i++){
        Serial.write(ASCII_BORDER_H);
    }
    
    // left
    locate(x, y + 1);
    for(int i=1; i<(h-1); i++){
        Serial.write(ASCII_BORDER_V);
        Serial.println();
        Serial.write(0x08);
    }
    
    // right
    locate(x + w - 1, y + 1);
    for(int i=1; i<(h-1); i++){
        Serial.write(ASCII_BORDER_V);
        Serial.println();
        Serial.write(0x08);
    }  
} 


