#ifndef _ADAFRUIT_SSD1306_DISPLAY
#define _ADAFRUIT_SSD1306_DISPLAY

#include <Adafruit_SSD1306.h>
#include "ssd1306_constants.h"
#include "display.h"

namespace Bas
{
    class AdafruitSSD1306Display : public Display
    {
        private:
            uint8_t width;
            uint8_t height;
			uint8_t screenAddress;
            uint8_t textSize = 1;
			Adafruit_SSD1306 &ssd1306Display;
            const uint8_t fullScaleCharacterWidth = CHARACTER_WIDTH;
            const uint8_t fullScaleCharacterHeight = CHARACTER_HEIGHT;
			unsigned long dimStartTime;
			const unsigned long dimDuration = 5000;
			bool isDimmed;

        public:
            AdafruitSSD1306Display(uint8_t width, uint8_t height, uint8_t screenAddress, Adafruit_SSD1306 &adafruitSsd1306Display) : width(width), height(height), screenAddress(screenAddress), ssd1306Display(adafruitSsd1306Display) {}
			void begin();
			uint8_t getWidth() override;
            uint8_t getHeight() override;
            uint16_t getCharacterWidth() override;
            uint16_t getCharacterHeight() override;
            void clear() override;
            void enableWrapping(bool isEnabled = false) override;
            void setTextSize(uint8_t size) override;
            void setCursorPosition(int16_t x, int16_t y) override;
			int16_t getCenteredCursorPositionX(const char *string) override;
            void invertTextColor(bool isInverted = true) override;
            void write(uint8_t character) override;
            void write(const char *string) override;
            void write(const uint8_t *buffer, size_t size) override;
            void write(const char *buffer, size_t size) override;
            void update() override;
            void sleep(bool isSleeping) override;
			void drawFilledRectangle(int16_t marginLeft, int16_t marginTop, int16_t marginRight, int16_t marginBottom) override;
    };
}

#endif