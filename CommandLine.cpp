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
 
#include "CommandLine.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

CommandLine::CommandLine()
{
}

/**
 * Resets the command line processor with a new stream and callback
 * function.
 */
void CommandLine::begin(Stream* stream, CommandLineCallback func)
{
    _stream = stream;
    _func = func;
    _count = 0;
}

/** 
 * Executes command processing code 
 * Call from your sketch's loop() function
 */
void CommandLine::loop()
{
    read();
}

void CommandLine::read()
{
    while(_stream->available() > 0) 
    {
        char c = _stream->read();

        /* overflow check */
        if (_count >= ARRAY_SIZE(_buffer) - 1)
        {
            _stream->println(F("command too long"));
            _count = 0;
        }
        else
        {
            /* backspace */
            if (c == '\b' && _count > 0)
            {
                _count--;
                _buffer[_count] = 0;
            }
            /* line terminator */
            else if (c == '\r' || c == '\n')
            {
                _buffer[_count] = 0;
                split();
                _count = 0;
            }
            /* normal character */
            else
            {
                _buffer[_count++] = c;
            }   
        }
    }
}

void CommandLine::split()
{
    char* state;

    _argc = 0;
    _tokens = strtok_r(_buffer, " ", &state);
    if (_tokens)
    {
        do
        {
            if (_argc >= ARRAY_SIZE(_argv))
            {
                _stream->println(F("too many arguments"));
                return;
            }
            _argv[_argc++] = _tokens;
            _tokens = strtok_r(NULL, " ", &state); 
        }
        while(_tokens);
    }
    else
    {
        /* blank line */
    }

    if (_argc > 0)
    {
        _func(_stream, _argc, _argv);
    }
}
