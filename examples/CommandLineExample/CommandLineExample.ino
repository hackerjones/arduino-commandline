/*
 * CommandLine class example
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

#include <CommandLine.h>

/* Create a command line object 
 the templete parameters define the 
 maximum line length and number of arguments
 You can take the default templete values
 by defining a command line object like
 CommandLine<> commandLine
 */
CommandLine<64, 8> commandLine;

void setup()
{
    Serial.begin(9600);
    commandLine.begin(&Serial, handleCommand, true, ">>>");
}

void loop()
{
    /* run the command line processing code */
    commandLine.loop();
}

/* our command callback function */
void handleCommand(Stream& stream, byte argc, char* argv[])
{
    if (argc == 0)
    {
        Serial.println(F("<return>"));   
    }
    /* add two integers */
    else if (strcmp(argv[0], "add") == 0)
    {
        if (argc == 3)
        {
            int x = atoi(argv[1]);
            int y = atoi(argv[2]);
            Serial.print(x);
            Serial.print(" + ");
            Serial.print(y);
            Serial.print(" = ");
            Serial.println(x + y);
        }
        else
            stream.println(F("invalid number of arguments"));
    }
    /* subtract two integers */
    else if (strcmp(argv[0], "sub") == 0)
    {
        if (argc == 3)
        {
            int x = atoi(argv[1]);
            int y = atoi(argv[2]);
            Serial.print(x);
            Serial.print(" - ");
            Serial.print(y);
            Serial.print(" = ");
            Serial.println(x - y);
        }
        else
            stream.println(F("invalid number of arguments"));
    }
    /* print free memory */
    else if (strcmp(argv[0], "mem") == 0)
    {
        Serial.println(memoryFree());
    }
    /* print list of arguments */
    else
    {
        for(int i=0; i < argc; i++)
        {
            Serial.print("arg[");
            Serial.print(i);
            Serial.print("] = ");
            Serial.println(argv[i]);
        }
    }
}

extern int __bss_end;
extern void* __brkval;

int memoryFree()
{
    int tmp;

    if ((int)__brkval == 0)
    {
        return tmp = ((int)&tmp) - ((int)&__bss_end);
    }
    return tmp = ((int)&tmp) - ((int)__brkval);
}


