#ifndef _SCROLLING_LIST_H
#define _SCROLLING_LIST_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "text_display.h"
#include "ssd1306_constants.h"

namespace Bas
{
	class ScrollingList
	{
	public:
		enum class LogLevel { none = 0, normal };

	private:
		LogLevel logLevel;
		int16_t getCenterYPosition();
		int16_t getListHeight();
		int16_t getMinYPosition();
		int16_t getMaxYPosition();
		void updateSmoothListYPosition();

		int16_t currentListYPosition = 0;
		uint8_t textSize = 1;
		size_t selectedItemIndex = 0;
		static const int maxItems = 20;
		static const int maxItemLength = 25;
		char items[maxItems][maxItemLength] = {{ '\0' }};
		size_t numItems;

	public:
		ScrollingList(LogLevel logLevel = LogLevel::none);
		void begin();
		void populate(const char* items[], size_t count);
		void update(TextDisplay &display);
		void nextItem();
		void previousItem();
		size_t getNumItems();
	};
}

#endif // SCROLLING_LIST_H