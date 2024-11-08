#include "video/adafruit_SSD1306_display.h"

void Bas::AdafruitSSD1306Display::begin()
{
	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if (!ssd1306Display.begin(SSD1306_SWITCHCAPVCC, screenAddress))
	{
		Serial.println(F("SSD1306 allocation failed"));
		for (;;)
			; // Don't proceed, loop forever
	}
}



int8_t Bas::AdafruitSSD1306Display::getWidth()
{
    return width;
}

int8_t Bas::AdafruitSSD1306Display::getHeight()
{
    return height;
}

int16_t Bas::AdafruitSSD1306Display::getCharacterWidth()
{
    return fullScaleCharacterWidth * textSize;
}

int16_t Bas::AdafruitSSD1306Display::getCharacterHeight()
{
    return fullScaleCharacterHeight * textSize;
}

void Bas::AdafruitSSD1306Display::clear()
{
    ssd1306Display.clearDisplay();
}

void Bas::AdafruitSSD1306Display::enableWrapping(bool isEnabled)
{
    ssd1306Display.setTextWrap(isEnabled);
}

void Bas::AdafruitSSD1306Display::setTextSize(uint8_t size)
{
    textSize = size;
    ssd1306Display.setTextSize(textSize);
}

void Bas::AdafruitSSD1306Display::setCursorPosition(int16_t x, int16_t y)
{
    ssd1306Display.setCursor(x, y);
}

void Bas::AdafruitSSD1306Display::invertTextColor(bool isInverted)
{
    if (isInverted)
    {
        ssd1306Display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    }
    else
    {
        ssd1306Display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    }
}

void Bas::AdafruitSSD1306Display::write(uint8_t character)
{
    ssd1306Display.write(character);
}

void Bas::AdafruitSSD1306Display::write(const char *string)
{
    ssd1306Display.write(string);
}

void Bas::AdafruitSSD1306Display::write(const uint8_t *buffer, size_t size)
{
    ssd1306Display.write(buffer, size);
}

void Bas::AdafruitSSD1306Display::write(const char * buffer, size_t size)
{
    ssd1306Display.write(buffer, size);
}

void Bas::AdafruitSSD1306Display::update()
{
	if (isDimmed && millis() - dimStartTime > dimDuration)
	{
		isDimmed = false;
		ssd1306Display.ssd1306_command(SSD1306_DISPLAYOFF);
	}
    ssd1306Display.display();
}

void Bas::AdafruitSSD1306Display::sleep(bool isSleeping)
{
	isDimmed = isSleeping;
	ssd1306Display.dim(isSleeping);

	if (isSleeping)
	{
		dimStartTime = millis();
	}
	else
	{
		ssd1306Display.ssd1306_command(SSD1306_DISPLAYON);
	}
}

