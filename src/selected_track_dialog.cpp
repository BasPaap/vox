#include "selected_track_dialog.h"

Bas::SelectedTrackDialog::SelectedTrackDialog(Display &display) : display(display)
{
}

void Bas::SelectedTrackDialog::update()
{
	if (isOpen)
	{
		display.drawFilledRectangle(0, 2, 0, 2);
		display.setTextSize(2);
		display.invertTextColor();
		display.setCursorPosition(2,4);
		display.write(currentFileName);
		// Draw a rectangle
		// Draw the current track name in double size
		// Draw the current mode in single size
	}
}

void Bas::SelectedTrackDialog::open(const char *fileName)
{
	currentFileName = fileName;
	isOpen = true;
}

void Bas::SelectedTrackDialog::close()
{
	isOpen = false;
}

void Bas::SelectedTrackDialog::setSelectedMode()
{
	currentMode = Mode::selected;
}

void Bas::SelectedTrackDialog::setPlayingMode()
{
	currentMode = Mode::playing;
}

bool Bas::SelectedTrackDialog::getIsOpen()
{
	return isOpen;
}
