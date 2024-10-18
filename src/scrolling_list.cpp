#include "scrolling_list.h"

int16_t Bas::ScrollingList::getCenterYPosition()
{
	return (SCREEN_HEIGHT / 2) - ((CHARACTER_HEIGHT * textSize) /2);
}

int16_t Bas::ScrollingList::getListHeight()
{
	return CHARACTER_HEIGHT * textSize * numItems;
}

int16_t Bas::ScrollingList::getMinYPosition()
{
	return SCREEN_HEIGHT <= getListHeight() ? SCREEN_HEIGHT - getListHeight() : 0;
}

int16_t Bas::ScrollingList::getMaxYPosition()
{
	return 0;
}

int16_t Bas::ScrollingList::getSmoothListYPosition(int16_t cursorY)
{
	int16_t unconstrainedTargetYPosition = getCenterYPosition() - (selectedItemIndex * CHARACTER_HEIGHT * textSize);
	int16_t targetYPosition = constrain(unconstrainedTargetYPosition, getMinYPosition(), getMaxYPosition());
	int16_t currentListYPosition = cursorY - getListHeight();

	if (currentListYPosition != targetYPosition)
	{
		currentListYPosition += targetYPosition > currentListYPosition ? 1 : -1;
	}

	return currentListYPosition;
}

Bas::ScrollingList::ScrollingList(LogLevel logLevel) : logLevel{ logLevel }
{
}

void Bas::ScrollingList::begin()
{
	if (this->logLevel == LogLevel::normal)
	{
		Serial.println("Initializing Scrolling List");
	}
}

void Bas::ScrollingList::populate(const char* items[], size_t numItems)
{
	size_t cappedNumItems = numItems <= maxItems ? numItems : maxItems;

	if (this->logLevel == LogLevel::normal)
	{
		Serial.print("Populating list with ");
		Serial.print(cappedNumItems);
		Serial.println(" items.");
	}

	for (size_t i = 0; i < cappedNumItems; i++)
	{
		strncpy(this->items[i], items[i], maxItemLength-1);	// Copy each item string into statically allocated memory
		this->items[i][maxItemLength -1] = '\0'; // Ensure null termination
	}

	this->numItems = cappedNumItems;
}

void Bas::ScrollingList::update(Adafruit_SSD1306 &display)
{
	display.clearDisplay();
	display.setTextWrap(false);
	display.setTextSize(textSize);
	display.setCursor(0, getSmoothListYPosition(display.getCursorY()));	// Smoothly scroll the list to keep the selected item in view.

	for (size_t i = 0; i < numItems; i++)
	{
		if (selectedItemIndex == i)
		{
			display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);	// If this is the selected item, print it with the inversed colors.
		}
		else
		{
			display.setTextColor(SSD1306_WHITE, SSD1306_BLACK); // Otherwise, print it with normal white on black.
		}

		display.write(items[i]);
		display.write('\n');
	}

	display.display();
}

void Bas::ScrollingList::nextItem()
{
	if (selectedItemIndex < numItems - 1)
	{
		selectedItemIndex++;
	}
}

void Bas::ScrollingList::previousItem()
{
	if (selectedItemIndex > 0)
	{
		selectedItemIndex--;
	}
}

size_t Bas::ScrollingList::getNumItems()
{
	return numItems;
}
