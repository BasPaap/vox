#include "selected_track_dialog.h"

Bas::SelectedTrackDialog::SelectedTrackDialog(Display &display) : display(display)
{
}

void Bas::SelectedTrackDialog::update()
{
	if (isOpen)
	{
		const uint16_t horizontalMargin = 0;
		const uint16_t verticalMargin = 0;
		const uint16_t horizontalTextMargin = 2;
		const uint16_t verticalTextMargin = 2;

		display.clear();
		display.drawFilledRectangle(horizontalMargin, verticalMargin, horizontalMargin, verticalMargin);
		display.setTextSize(2);
		display.invertTextColor();
		display.setCursorPosition(horizontalMargin + horizontalTextMargin, verticalMargin + verticalTextMargin);
		display.write(currentFileName);
		display.setTextSize(1);

		const char *statusText;

		if (currentMode == Mode::playing)
		{
		 	statusText = "speelt nu...";
		}
		else
		{
			statusText = "klaar om te spelen";
		}

		display.setCursorPosition(display.getCenteredCursorPositionX(statusText), (display.getCharacterHeight() * 2) + verticalMargin + verticalTextMargin + 1);
		display.write(statusText);
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
