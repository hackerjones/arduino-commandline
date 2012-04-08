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

typedef void (*CommandLineCallback)(Stream&, byte, char**);

/**
 * Command line processing class for Arduino Streams
 */
template <byte BufferSize = 64, byte MaxArguments = 8>
class CommandLine
{
private:
    Stream*             _stream;
    char                _buffer[BufferSize];
    byte                _count;
    char*               _argv[MaxArguments];
    byte                _argc;
    char*               _tokens;
    CommandLineCallback _func;
    bool                _echo;
    String              _prompt;

    void read()
    {
        while (_stream->available() > 0)
        {
            char c = _stream->read();

            if (_echo)
            {
                _stream->write(c);
            }

            /* overflow check */
            if (_count >= BufferSize - 1)
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
                    if (_echo)
                    {
                        _stream->print(" \b");
                    }
                }
                /* line terminator */
                else if (c == '\r' || c == '\n')
                {
                    _buffer[_count] = 0;
                    split();
                    displayPrompt();
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

    void split()
    {
        char* state;

        _argc = 0;
        _tokens = strtok_r(_buffer, " ", &state);
        if (_tokens)
        {
            do
            {
                if (_argc >= MaxArguments)
                {
                    _stream->println(F("too many arguments"));
                    return;
                }
                _argv[_argc++] = _tokens;
                _tokens = strtok_r(NULL, " ", &state);
            }
            while (_tokens);
        }

        _func(*_stream, _argc, _argv);
    }

    void displayPrompt()
    {
        if (_prompt.length() > 0)
            _stream->print(_prompt);
    }

public:
    CommandLine()
    {
    }

    /**
     * Resets the command line processor with a new stream and callback
     * and options.
     */
    void begin(Stream* stream, CommandLineCallback func,
               bool echo = true, String prompt = String())
    {
        _stream = stream;
        _echo = echo;
        _func = func;
        _count = 0;
        _prompt = prompt;

        displayPrompt();
    }

    /**
    * Executes command processing code
    * Call from your sketch's loop() function
    */
    void loop()
    {
        read();
    }
};

#endif // _CONSOLE_H_


