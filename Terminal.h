/* mbed Terminal TextDisplay Library
 * Copyright (c) 2007-2009 sford
 * Released under the MIT License: http://mbed.org/license/mit
 *
 * Implementation of ANSI/VT100 Terminal escape codes
 * for use with e.g. Teraterm, Hyperterminal
 */

#include "Arduino.h"

#ifndef MBED_TERMINAL_H
#define MBED_TERMINAL_H

class Terminal {
public:

    Terminal();

    // printf(), put(), baud() etc - inherited from Serial

    void cls();
    void clear_to_eol();
    void HideCursor();
    void locate(int column, int row);
    void foreground(int colour);
    void background(int colour);
    void box(int x, int y, int w, int h); 
	void printf(const char* format, ...);

};

#endif
