#include "adafruit_SSD1306_text_display.h"

int8_t Bas::AdafruitSSD1306TextDisplay::getWidth()
{
    return width;
}

int8_t Bas::AdafruitSSD1306TextDisplay::getHeight()
{
    return height;
}

int16_t Bas::AdafruitSSD1306TextDisplay::getCharacterWidth()
{
    return fullScaleCharacterWidth * textSize;
}

int16_t Bas::AdafruitSSD1306TextDisplay::getCharacterHeight()
{
    return fullScaleCharacterHeight * textSize;
}

void Bas::AdafruitSSD1306TextDisplay::clear()
{
    display.clearDisplay();
}

void Bas::AdafruitSSD1306TextDisplay::enableWrapping(bool isEnabled)
{
    display.setTextWrap(isEnabled);
}

void Bas::AdafruitSSD1306TextDisplay::setTextSize(uint8_t size)
{
    textSize = size;
    display.setTextSize(textSize);
}

void Bas::AdafruitSSD1306TextDisplay::setCursorPosition(int16_t x, int16_t y)
{
    display.setCursor(x, y);
}

void Bas::AdafruitSSD1306TextDisplay::invertTextColor(bool isInverted)
{
    if (isInverted)
    {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    }
    else
    {
        display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    }
}

void Bas::AdafruitSSD1306TextDisplay::write(uint8_t character)
{
    display.write(character);
}

void Bas::AdafruitSSD1306TextDisplay::write(const char *string)
{
    display.write(string);
}

void Bas::AdafruitSSD1306TextDisplay::write(const uint8_t *buffer, size_t size)
{
    display.write(buffer, size);
}

void Bas::AdafruitSSD1306TextDisplay::write(const char * buffer, size_t size)
{
    display.write(buffer, size);
}

void Bas::AdafruitSSD1306TextDisplay::update()
{
    display.display();
}

void Bas::AdafruitSSD1306TextDisplay::sleep(bool isSleeping)
{
	display.ssd1306_command(isSleeping ? SSD1306_DISPLAYOFF : SSD1306_DISPLAYON);
}

