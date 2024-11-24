#include "scrolling_list.h"

int16_t Bas::ScrollingList::getCenterYPosition()
{
	return (display.getHeight() / 2) - (display.getCharacterHeight() /2);
}

int16_t Bas::ScrollingList::getListHeight()
{
	return display.getCharacterHeight() * numItems;
}

int16_t Bas::ScrollingList::getMinYPosition()
{
	return display.getHeight() <= getListHeight() ? display.getHeight() - getListHeight() : 0;
}

void Bas::ScrollingList::updateSmoothListYPosition()
{
	int16_t unconstrainedTargetYPosition = getCenterYPosition() - (selectedItemIndex * display.getCharacterHeight());
	int16_t targetYPosition = constrain(unconstrainedTargetYPosition, getMinYPosition(), maxYPosition);

	if (currentListYPosition != targetYPosition)
	{
		int16_t distance = abs(currentListYPosition - targetYPosition);
		int incrementValue;

		// If the list is getting too far behind the target position, increase the scroll speed.
		if (distance > display.getCharacterHeight() * 1.5)
			incrementValue = 3;
		else if (distance >= display.getCharacterHeight())
			incrementValue = 2;
		else
			incrementValue = 1;

		currentListYPosition += targetYPosition > currentListYPosition ? incrementValue : 0 - incrementValue;
	}
}

Bas::ScrollingList::ScrollingList(Display &display, LogLevel logLevel) : display{ display }, logLevel{ logLevel }
{
}

void Bas::ScrollingList::begin()
{
	if (this->logLevel == LogLevel::normal)
	{
		Serial.println("Initializing Scrolling List");
	}
}

void Bas::ScrollingList::update()
{
	display.enableWrapping(false);
	display.setTextSize(1);
	updateSmoothListYPosition(); // Smoothly scroll the list to keep the selected item in view.
	display.setCursorPosition(0, currentListYPosition);

	for (size_t i = 0; i < numItems; i++)
	{
		if (selectedItemIndex == i)
		{
			display.invertTextColor(); // If this is the selected item, print it with the inversed colors.
		}
		else
		{
			display.invertTextColor(false); // Otherwise, print it with normal white on black.
		}

		display.write(items[i]);
		display.write('\n');
	}
}

void Bas::ScrollingList::clear()
{
	selectedItemIndex = 0;

	for (size_t i = 0; i < numItems; i++)
	{
		items[i][0] = '\0';
	}

	numItems = 0;
}

void Bas::ScrollingList::addItem(const char *text)
{
	if (numItems <= maxItems)
	{
		strncpy(items[numItems], text, maxItemLength);
		numItems++;
	}
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

size_t Bas::ScrollingList::getSelectedItemIndex()
{
    return selectedItemIndex;
}

bool Bas::ScrollingList::getIsAtLastItem()
{
	return selectedItemIndex == numItems - 1;
}
