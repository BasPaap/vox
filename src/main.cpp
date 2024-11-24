#include <Arduino.h>
#include <Bas.Button.h>
#include "arcana_logo.h"
#include "video/ssd1306_constants.h"
#include "video/adafruit_SSD1306_display.h"
#include "scrolling_list.h"
#include "selected_track_dialog.h"
#include "inactivity_timer.h"
#include "filebrowser/sdfat_file_browser.h"
#include "audio/adafruit_VS1053_audio_player.h"
#include "motor/motor.h"

// Pins used by the sparkfun VS1053 shield
const pin_size_t playerResetPin = 8;
const pin_size_t playerChipSelectPin = 6;
const pin_size_t playerDataChipSelectPin = 7;
const pin_size_t playerDataRequestPin = 2;

// Const for an SSD1306 display
const int8_t oledResetPin = -1;		// Reset pin # (or -1 if sharing Arduino reset pin)
const uint8_t screenAddress = 0x3C; ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Const for SD card
const pin_size_t sdCardChipSelectPin = 9;

// Button pins
const pin_size_t upButtonPin = A1;
const pin_size_t downButtonPin = A0;
const pin_size_t selectButtonPin = A2;
const pin_size_t playButtonPin = A3;

// Motor pins
const pin_size_t motorAPin = 5;
const pin_size_t motorBPin = 10;

// Other consts
const size_t maxFilePathLength = 257; // 256 characters + zero terminator
const char *versionText = "Vox v1.0.0";
const unsigned long motorWarmupTime = 1000;

SdFs sdCard;
Adafruit_SSD1306 adafruitSsd1306Display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, oledResetPin);
Bas::AdafruitSSD1306Display display(SCREEN_WIDTH, SCREEN_HEIGHT, screenAddress, adafruitSsd1306Display);
Bas::ScrollingList scrollingList(display);
Bas::SelectedTrackDialog selectedTrackDialog(display);
Bas::Button upButton(upButtonPin, 20);
Bas::Button downButton(downButtonPin, 20);
Bas::Button selectButton(selectButtonPin, 20);
Bas::Button playButton(playButtonPin, 20);
Bas::InactivityTimer inactivityTimer;
Bas::SdFatFileBrowser fileBrowser(&sdCard);
Bas::AdafruitVS1053AudioPlayer audioPlayer(playerResetPin, playerChipSelectPin, playerDataChipSelectPin, playerDataRequestPin, &sdCard);
Bas::Motor motor(motorAPin, motorBPin);

char selectedFilePath[maxFilePathLength] = {0};
bool hasPlaybackStarted = false;
bool hasMotorStarted = false;
unsigned long motorStartTime;
bool ignoreNextFallingSignal = false;

void onActivity()
{
	inactivityTimer.reset();
	display.sleep(false);
}

void surroundWithSquareBrackets(char *text)
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

void onUpButtonPressed()
{
	if (inactivityTimer.getIsActive() &&
	    !selectedTrackDialog.getIsOpen() &&
		!audioPlayer.getIsPlaying())
	{
		scrollingList.previousItem();
	}

	onActivity();
}

void onDownButtonPressed()
{
	if (inactivityTimer.getIsActive() &&
		!selectedTrackDialog.getIsOpen() &&
		!audioPlayer.getIsPlaying())
	{
		scrollingList.nextItem();
	}

	onActivity();
}





void onPlayButtonToggled()
{
	Serial.println("Play button toggled.");
	// The motor could be running without the audioplayer having started yet,
	// so stop the motor either way if it runs.
	if (hasMotorStarted)
	{
		Serial.println("Stopping motor");
		hasMotorStarted = false;
		motor.stopSpinning();
	}

	if (audioPlayer.getIsPlaying())
	{
		Serial.println("Stopping playback");
		audioPlayer.stopPlaying();
		selectedTrackDialog.setSelectedMode();
		hasPlaybackStarted = false;
	}
	else
	{
		Serial.println("Ready to play!");
		// If no file has been selected, look for the first file in the current directory and select it.
		if (strlen(selectedFilePath) == 0)
		{
			Serial.println("No file selected when playing... searching first MP3.");

			bool isAtLastItem = false;
			do
			{
				size_t selectedItemIndex = scrollingList.getSelectedItemIndex();
				size_t fileIndex = fileBrowser.getIsAtRoot() ? selectedItemIndex : selectedItemIndex - 1;

				Serial.print(F("File at index "));
				Serial.print(fileIndex);
				Serial.print(" is ");
				if (fileBrowser.getIsDirectory(fileIndex))
				{
					Serial.print("directory... ");
					if (scrollingList.getIsAtLastItem())
					{
						Serial.println("and was the last one.");
						isAtLastItem = true;
					}
					else
					{
						Serial.println("trying next item.");
						scrollingList.nextItem();
					}
				}
				else
				{
					Serial.print("File with the name ");
					fileBrowser.getFilePath(fileIndex, selectedFilePath, maxFilePathLength);
					Serial.println(selectedFilePath);
				}
			} while (strlen(selectedFilePath) == 0 && !isAtLastItem);
		}

		if (strlen(selectedFilePath) > 0)
		{
			Serial.print(F("Playing "));
			Serial.println(selectedFilePath);
			selectedTrackDialog.setPlayingMode();
			motor.startSpinning();
			hasMotorStarted = true;
			motorStartTime = millis(); // Don't start playing the file just yet, the motor will start spinning for a few seconds and then the file will play.
		}
	}
}

void onSelectButtonPressed()
{
	if (inactivityTimer.getIsActive() &&
		!audioPlayer.getIsPlaying())
	{
		if (selectedTrackDialog.getIsOpen())
		{
			selectedTrackDialog.close();
		}
		else
		{
			selectedFilePath[0] = '\0';
			size_t selectedItemIndex = scrollingList.getSelectedItemIndex();

			if (!fileBrowser.getIsAtRoot() && selectedItemIndex == 0)
			{
				fileBrowser.goToParentDirectory();
				populateScrollingList();
			}
			else
			{
				size_t fileIndex = fileBrowser.getIsAtRoot() ? selectedItemIndex : selectedItemIndex - 1;

				if (fileBrowser.getIsDirectory(fileIndex))
				{
					fileBrowser.goToSubDirectory(fileIndex);
					populateScrollingList();
				}
				else
				{
					fileBrowser.getFilePath(fileIndex, selectedFilePath, maxFilePathLength);
					selectedTrackDialog.open(strrchr(selectedFilePath, '/') + 1);
					// onPlayButtonToggled();
				}
			}
		}
	}

	onActivity();
}



void showSplashScreen()
{
	adafruitSsd1306Display.clearDisplay();
	adafruitSsd1306Display.drawBitmap(0, 0, arcanaLogo, ARCANA_LOGO_WIDTH, ARCANA_LOGO_HEIGHT, 1);
	adafruitSsd1306Display.setTextSize(1);

	adafruitSsd1306Display.setCursor(SCREEN_WIDTH - strlen(versionText) * CHARACTER_WIDTH, SCREEN_HEIGHT - CHARACTER_HEIGHT);
	adafruitSsd1306Display.cp437(true);
	adafruitSsd1306Display.setTextColor(SSD1306_WHITE);
	adafruitSsd1306Display.write(versionText);

	adafruitSsd1306Display.display();
	delay(1000);
}

void onInactivity()
{
	display.sleep(true);
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

void rising()
{
	onPlayButtonToggled();
	ignoreNextFallingSignal = false;
}

void falling()
{
	if (!ignoreNextFallingSignal)
	{
		Serial.println("Falling");
		onPlayButtonToggled();
	}
	else
	{
		Serial.println("Falling, but ignored.");
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
	playButton.begin(falling, rising);
	scrollingList.begin();
	display.begin();
	fileBrowser.begin();
	audioPlayer.begin();
	motor.begin();

	Serial.println("Checking play button...");
	if (playButton.isPressed())
	{
		Serial.println("Ignoring next fall...");
		// The play button is already pressed, ignore it when it says the signal has fallen (this will happen if it is toggled to LOW on startup.)
		ignoreNextFallingSignal = true;
	}

	populateScrollingList();

	showSplashScreen();
}

void loop()
{
	// If the motor has started but playback has not yet started, and the motor has been running for the
	// required amout of time, start playback.
	if (hasMotorStarted && !hasPlaybackStarted && millis() - motorStartTime >= motorWarmupTime)
	{
		hasPlaybackStarted = true;
		audioPlayer.startPlayingFile(selectedFilePath);
	}

	if (hasPlaybackStarted && !audioPlayer.getIsPlaying())
	{
		// The track has finished.
		selectedTrackDialog.close();
		motor.stopSpinning();
		hasMotorStarted = false;
		hasPlaybackStarted = false;
	}

	inactivityTimer.update();
	upButton.update();
	downButton.update();
	selectButton.update();
	playButton.update();

	display.clear();
	scrollingList.update();
	selectedTrackDialog.update();
	display.update();
}
