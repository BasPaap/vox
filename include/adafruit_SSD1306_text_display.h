#ifndef _ADAFRUIT_SSD1306_TEXT_DISPLAY
#define _ADAFRUIT_SSD1306_TEXT_DISPLAY

#include <Adafruit_SSD1306.h>
#include "ssd1306_constants.h"
#include "text_display.h"

namespace Bas
{
    class AdafruitSSD1306TextDisplay : public TextDisplay
    {
        private:
            int8_t width;
            int8_t height;
            int8_t textSize = 1;
            const int8_t fullScaleCharacterWidth = CHARACTER_WIDTH;
            const int8_t fullScaleCharacterHeight = CHARACTER_HEIGHT;
            Adafruit_SSD1306 &display;

        public:
            AdafruitSSD1306TextDisplay(uint8_t width, uint8_t height, Adafruit_SSD1306 &adafruitSsd1306Display) : width(width), height(height), display(adafruitSsd1306Display) {}

            int8_t getWidth() override;
            int8_t getHeight() override;
            int16_t getCharacterWidth() override;
            int16_t getCharacterHeight() override;
            void clear() override;
            void enableWrapping(bool isEnabled = false) override;
            void setTextSize(uint8_t size) override;
            void setCursorPosition(int16_t x, int16_t y) override;
            void invertTextColor(bool isInverted = true) override;
            void write(uint8_t character) override;
            void write(const char *string) override;
            void write(const uint8_t *buffer, size_t size) override;
            void write(const char *buffer, size_t size) override;
            void update() override;
            void dim(bool isEnabled = true) override;
    };
}

#endif