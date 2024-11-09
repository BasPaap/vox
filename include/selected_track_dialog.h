
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
	private:
		Display &display;
		bool isOpen = false;
		enum class Mode { selected, playing };
		Mode currentMode;
		const char *currentFileName;

	public:
		SelectedTrackDialog(Display &display);
		void update();
		void open(const char *fileName);
        void close();
        void setSelectedMode();
        void setPlayingMode();
		bool getIsOpen();
	};
}

#endif // _SELECTED_TRACK_DIALOG_H