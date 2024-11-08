#ifndef _SCROLLING_LIST_H
#define _SCROLLING_LIST_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "video/display.h"

namespace Bas
{
	class ScrollingList
	{
	public:
		enum class LogLevel { none = 0, normal };

	private:
		Display &display;
		LogLevel logLevel;
		int16_t currentListYPosition = 0;
		size_t selectedItemIndex = 0;
		static const int maxItems = 50;
		static const int maxItemLength = 255 + 2 + 1; // max item length + 2 square brackets + 0 terminator
		char items[maxItems][maxItemLength] = {{ '\0' }};
		size_t numItems;
		const int16_t maxYPosition = 0;
		int16_t getCenterYPosition();
		int16_t getListHeight();
		int16_t getMinYPosition();
		void updateSmoothListYPosition();

	public:
		ScrollingList(Display &display, LogLevel logLevel = LogLevel::none);
		void begin();
		void update();
		void clear();
		void addItem(const char* text);
		void nextItem();
		void previousItem();
		size_t getNumItems();
		size_t getSelectedItemIndex();
	};
}

#endif // SCROLLING_LIST_H