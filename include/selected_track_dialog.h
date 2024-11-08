
#ifndef _SELECTED_TRACK_DIALOG_H
#define _SELECTED_TRACK_DIALOG_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "video/display.h"

namespace Bas
{
	class SelectedTrackDialog
	{
	public:
		enum class LogLevel { none = 0, normal };

	private:
		Display &display;
		LogLevel logLevel;

	public:
		SelectedTrackDialog(Display &display, LogLevel logLevel = LogLevel::none);
		void begin();
		void update();
		void open();
        void close();
        void setSelectedMode();
        void setPlayingMode();
	};
}

#endif // _SELECTED_TRACK_DIALOG_H