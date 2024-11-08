#include <Arduino.h>
#include <Bas.Button.h>
#include "arcana_logo.h"
#include "video/ssd1306_constants.h"
#include "video/adafruit_SSD1306_text_display.h"
#include "scrolling_list.h"
#include "inactivity_timer.h"
#include "filebrowser/sdfat_file_browser.h"
#include "audio/adafruit_VS1053_audio_player.h"

// Pins used by the sparkfun VS1053 shield
const pin_size_t playerResetPin = 8;
const pin_size_t playerChipSelectPin = 6;
const pin_size_t playerDataChipSelectPin = 7;
const pin_size_t playerDataRequestPin = 2;

// Const for an SSD1306 display
const int8_t oledResetPin = -1; // Reset pin # (or -1 if sharing Arduino reset pin)
const uint8_t screenAddress = 0x3C; ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Const for SD card
const pin_size_t sdCardChipSelectPin = 9;

// Button pins
const int upButtonPin = A1;
const int downButtonPin = A0;
const int selectButtonPin = A2;

SdFs sdCard;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, oledResetPin);
Bas::AdafruitSSD1306TextDisplay textDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, screenAddress, display);
Bas::ScrollingList scrollingList(textDisplay);
Bas::Button upButton(upButtonPin, 20);
Bas::Button downButton(downButtonPin, 20);
Bas::Button selectButton(selectButtonPin, 20);
Bas::InactivityTimer inactivityTimer;
Bas::SdFatFileBrowser fileBrowser(&sdCard);
Bas::AdafruitVS1053AudioPlayer audioPlayer(playerResetPin, playerChipSelectPin, playerDataChipSelectPin, playerDataRequestPin, &sdCard);

const char *versionText = "Vox v1.0.0";

void onActivity()
{
	inactivityTimer.reset();
	textDisplay.sleep(false);
}

void onUpButtonPressed()
{
	if (inactivityTimer.getIsActive())
	{
		scrollingList.previousItem();
	}

	onActivity();
}

void onDownButtonPressed()
{
	if (inactivityTimer.getIsActive())
	{
		scrollingList.nextItem();
	}

	onActivity();
}

void onSelectButtonPressed()
{
	if (inactivityTimer.getIsActive())
	{
		size_t selectedItemIndex = scrollingList.getSelectedItemIndex();

		if (!fileBrowser.getIsAtRoot() && selectedItemIndex == 0)
		{
			// Serial.println("Going to parent directory.");
			fileBrowser.goToParentDirectory();
			// Serial.println("Populating scrolling list");
			populateScrollingList();
		}
		else
		{
			size_t fileIndex = fileBrowser.getIsAtRoot() ? selectedItemIndex : selectedItemIndex - 1;

			if (fileBrowser.getIsDirectory(fileIndex))
			{
				// Serial.print(F("Going to sub directory "));
				// Serial.println(fileIndex);
				fileBrowser.goToSubDirectory(fileIndex);
				populateScrollingList();
			}
			else
			{
				// Serial.print(F("Playing file "));
				// Serial.println(fileIndex);

				// Select file to play.
				const size_t maxPathLength = 257;
				char filePath[maxPathLength];
				fileBrowser.getFilePath(fileIndex, filePath, maxPathLength);
				audioPlayer.startPlayingFile(filePath);
			}
		}
	}

	onActivity();
}

void showSplashScreen()
{
	display.clearDisplay();
	display.drawBitmap(0, 0, arcanaLogo, ARCANA_LOGO_WIDTH, ARCANA_LOGO_HEIGHT, 1);
	display.setTextSize(1);

	display.setCursor(SCREEN_WIDTH - strlen(versionText) * CHARACTER_WIDTH, SCREEN_HEIGHT - CHARACTER_HEIGHT);
	display.cp437(true);
	display.setTextColor(SSD1306_WHITE);
	display.write(versionText);

	display.display();
	delay(1000);
}

void onInactivity()
{
	textDisplay.sleep(true);
}

void waitForSerial()
{
	const unsigned long serialWaitDuration = 3000;
	unsigned long startTime = millis();

	while (!Serial && millis() - startTime < serialWaitDuration)
	{
		yield();
	} // Wait until serial is available, or until the specified	time has elapsed
}

void surroundWithSquareBrackets(char* text)
{
	char oldChar = text[0];
	text[0] = '[';

	for (size_t i = 1; i < strlen(text) + 1; i++)
	{
		char nextChar = text[i];
		text[i] = oldChar;
		oldChar = nextChar;
	}

	size_t newLength = strlen(text);
	text[newLength] = ']';
	text[newLength + 1] = 0;
}

void populateScrollingList()
{
	scrollingList.clear();

	if (!fileBrowser.getIsAtRoot())
	{
		scrollingList.addItem("[..]");
	}

	bool isDirectory;
	const int maxDirectoryTextLength = 255 + 2 + 1; // max directory name length + brackets + 0 terminator
	char fileName[maxDirectoryTextLength];

	while (fileBrowser.read(isDirectory, fileName))
	{
		if (isDirectory)
		{
			surroundWithSquareBrackets(fileName);
		}

		scrollingList.addItem(fileName);
	}
}

void setup()
{
	Serial.begin(9600);
	waitForSerial();

	Serial.print(F("Starting "));
	Serial.println(versionText);

	if (!sdCard.begin(SdSpiConfig(sdCardChipSelectPin, SHARED_SPI, SD_SCK_MHZ(50))))
	{
		sdCard.initErrorHalt(&Serial);
	}

	inactivityTimer.begin(10000, onInactivity);
	upButton.begin(onUpButtonPressed);
	downButton.begin(onDownButtonPressed);
	selectButton.begin(onSelectButtonPressed);
	scrollingList.begin();
	textDisplay.begin();
	fileBrowser.begin();
	audioPlayer.begin();

	populateScrollingList();

	showSplashScreen();
}

bool isDone = false;
void loop()
{
if (!isDone && !audioPlayer.getIsPlaying()) {
    Serial.println("Done playing music");
	isDone = true;
  }

	inactivityTimer.update();
	upButton.update();
	downButton.update();
	selectButton.update();
	scrollingList.update();
}
