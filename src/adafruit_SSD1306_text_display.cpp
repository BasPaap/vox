#include "adafruit_SSD1306_text_display.h"

void Bas::AdafruitSSD1306TextDisplay::begin()
{
	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if (!display.begin(SSD1306_SWITCHCAPVCC, screenAddress))
	{
		Serial.println(F("SSD1306 allocation failed"));
		for (;;)
			; // Don't proceed, loop forever
	}
}



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
	if (isDimmed && millis() - dimStartTime > dimDuration)
	{
		isDimmed = false;
		display.ssd1306_command(SSD1306_DISPLAYOFF);
	}
    display.display();
}

void Bas::AdafruitSSD1306TextDisplay::sleep(bool isSleeping)
{
	isDimmed = isSleeping;
	display.dim(isSleeping);

	if (isSleeping)
	{
		dimStartTime = millis();
	}
	else
	{
		display.ssd1306_command(SSD1306_DISPLAYON);
	}
}

