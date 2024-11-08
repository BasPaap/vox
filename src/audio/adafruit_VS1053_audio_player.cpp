#include "audio/adafruit_VS1053_audio_player.h"

void Bas::AdafruitVS1053AudioPlayer::begin()
{
	if (!adafruitVs1053FilePlayer.begin())
	{
		Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
		while (1);
	}

	adafruitVs1053FilePlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

	setVolume(255, 255);
}

void Bas::AdafruitVS1053AudioPlayer::setVolume(uint8_t leftVolume, uint8_t rightVolume)
{
	adafruitVs1053FilePlayer.setVolume(255 - leftVolume, 255 - rightVolume);	// Invert the volume values, because for the Adafruit VS1053 library lower numbers mean louder volume.
}

void Bas::AdafruitVS1053AudioPlayer::startPlayingFile(const char *filePath)
{
	adafruitVs1053FilePlayer.startPlayingFile(filePath);
}

void Bas::AdafruitVS1053AudioPlayer::stopPlaying()
{
	adafruitVs1053FilePlayer.stopPlaying();
}

bool Bas::AdafruitVS1053AudioPlayer::getIsPlaying()
{
	return !adafruitVs1053FilePlayer.stopped();
}
