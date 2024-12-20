#ifndef _DISPLAY_H
#define _DISPLAY_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace Bas
{
    class Display
    {
        public:
            virtual uint8_t getWidth() = 0;
            virtual uint8_t getHeight() = 0;
            virtual uint16_t getCharacterWidth() = 0;
            virtual uint16_t getCharacterHeight() = 0;
            virtual void clear() = 0;
            virtual void enableWrapping(bool isEnabled = true) = 0;
            virtual void setTextSize(uint8_t size) = 0;
            virtual void setCursorPosition(int16_t x, int16_t y) = 0;
			virtual int16_t getCenteredCursorPositionX(const char *string) = 0;
            virtual void invertTextColor(bool isInverted = true) = 0;
            virtual void write(uint8_t character) = 0;
            virtual void write(const char *string) = 0;
            virtual void write(const uint8_t *buffer, size_t size) = 0;
            virtual void write(const char *buffer, size_t size) = 0;
			virtual void update() = 0;
            virtual void sleep(bool isSleeping) = 0;
			virtual void drawFilledRectangle(int16_t marginLeft, int16_t marginTop, int16_t marginRight, int16_t marginBottom) = 0;
    };
}

#endif