#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include <SdFat.h>
#include <vs1053_SdFat.h>
#include <Bas.Button.h>

#include "arcana_logo.h"
#include "ssd1306_constants.h"
#include "adafruit_SSD1306_text_display.h"
#include "scrolling_list.h"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Bas::ScrollingList scrollingList;
Bas::Button upButton(A1, 20);
Bas::Button downButton(A0, 20);
Bas::AdafruitSSD1306TextDisplay textDisplay(display);

void onUpButtonPressed()
{
	scrollingList.previousItem();
}

void onDownButtonPressed()
{
	scrollingList.nextItem();
}

void initializeDisplay()
{
	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  	if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    	Serial.println(F("SSD1306 allocation failed"));
    	for (;;)
      	;  // Don't proceed, loop forever
  	}
}

void showSplashScreen()
{
	display.clearDisplay();
	display.drawBitmap(0,0, arcanaLogo, ARCANA_LOGO_WIDTH, ARCANA_LOGO_HEIGHT, 1);
	display.setTextSize(1);
	const char * versionText = "Vox v1.0.0";

	display.setCursor(SCREEN_WIDTH - strlen(versionText) * CHARACTER_WIDTH, SCREEN_HEIGHT - CHARACTER_HEIGHT);
	display.cp437(true);
	display.setTextColor(SSD1306_WHITE);
	display.write(versionText);

	display.display();
  	delay(1000);
}

void setup() {
	Serial.begin(9600);

	upButton.begin(onUpButtonPressed);
	downButton.begin(onDownButtonPressed);

	const char* myItems[] = { "[ Directory1 ]", "[ Directory2 ]", "Tsardas.mp3", "Filename 1.mp3", "Filename 2.mp3", "anotherfile.txt", "Tsardas.mp3", "Filename 1.mp3", "Filename 2.mp3", "anotherfile.txt" };
	scrollingList.begin();
	scrollingList.populate(myItems, sizeof(myItems) / sizeof(myItems[0]));

	initializeDisplay();
	showSplashScreen();
}

void loop() {
	upButton.update();
	downButton.update();
	scrollingList.update(textDisplay);
}
