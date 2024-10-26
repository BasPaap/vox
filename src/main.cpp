#include <Arduino.h>
// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
// #include <SdFat.h>
//#include <vs1053_SdFat.h>
#include <Bas.Button.h>

#include "arcana_logo.h"
#include "ssd1306_constants.h"
#include "adafruit_SSD1306_text_display.h"
#include "scrolling_list.h"
#include "inactivity_timer.h"
#include "sdfat_file_browser.h"
#include "Common.h"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1		// Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Bas::AdafruitSSD1306TextDisplay textDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ADDRESS, display);
Bas::ScrollingList scrollingList(textDisplay);
Bas::Button upButton(A1, 20);
Bas::Button downButton(A0, 20);
Bas::Button selectButton(A2, 20);
Bas::InactivityTimer inactivityTimer;
Bas::SdFatFileBrowser fileBrowser;

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
		Serial.println("Select button pressed.");
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

void setup()
{
	Serial.begin(9600);
	waitForSerial();

	Serial.print(F("Starting "));
	Serial.println(versionText);

	inactivityTimer.begin(10000, onInactivity);

	upButton.begin(onUpButtonPressed);
	downButton.begin(onDownButtonPressed);
	selectButton.begin(onSelectButtonPressed);

	scrollingList.begin();

	textDisplay.begin();

	fileBrowser.begin();

	populateScrollingList();

	//fileBrowser.goToSubDirectory(8);
	//fileBrowser.goToParentDirectory();
	showSplashScreen();
}

void populateScrollingList()
{
	scrollingList.clear();

	if (!fileBrowser.getIsAtRoot())
	{
		scrollingList.addItem("[..]");
	}

	bool isDirectory;
	char fileName[256];
	size_t fileNameLength;
	while (fileBrowser.read(isDirectory, fileName, fileNameLength))
	{
		scrollingList.addItem(fileName);
	}
}

void loop()
{
	inactivityTimer.update();
	upButton.update();
	downButton.update();
	selectButton.update();
	scrollingList.update();
}
