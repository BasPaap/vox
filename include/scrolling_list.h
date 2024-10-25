#ifndef _SCROLLING_LIST_H
#define _SCROLLING_LIST_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "text_display.h"

namespace Bas
{
	class ScrollingList
	{
	public:
		enum class LogLevel { none = 0, normal };

	private:
		TextDisplay &display;
		LogLevel logLevel;
		int16_t currentListYPosition = 0;
		size_t selectedItemIndex = 0;
		static const int maxItems = 50;
		static const int maxItemLength = 255;
		char items[maxItems][maxItemLength] = {{ '\0' }};
		size_t numItems;
		const int16_t maxYPosition = 0;
		int16_t getCenterYPosition();
		int16_t getListHeight();
		int16_t getMinYPosition();
		void updateSmoothListYPosition();

	public:
		ScrollingList(TextDisplay &display, LogLevel logLevel = LogLevel::none);
		void begin();
		void populate(const char* items[], size_t count);
		void update();
		void nextItem();
		void previousItem();
		size_t getNumItems();
	};
}

#endif // SCROLLING_LIST_H