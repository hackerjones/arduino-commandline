/*
 * Command line processor for Arduino Streams
 * Copyright (C) 2012  Mark Jones (markalanj@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _COMMANDLINE_H_
#define _COMMANDLINE_H_

#include <Arduino.h>

typedef void (*CommandLineCallback)(Stream*, byte, char**);

/**
 * Command line processing class for Arduino Streams
 */
class CommandLine 
{
private:
    static const byte BufferSize = 64;
    static const byte ArgvSize = 8;

    Stream*             _stream;
    char                _buffer[BufferSize];
    byte                _count;
    char*               _argv[ArgvSize];
    byte                _argc;
    char*               _tokens;
    CommandLineCallback _func;

    void read();
    void split();
public:
    CommandLine();
    void begin(Stream* stream, CommandLineCallback func);
    void loop();
};

#endif // _CONSOLE_H_


