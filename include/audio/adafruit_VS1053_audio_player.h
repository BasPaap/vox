#ifndef _ADAFRUIT_VS1053_AUDIO_PLAYER_H
#define _ADAFRUIT_VS1053_AUDIO_PLAYER_H

#include "audio_player.h"
#include <Adafruit_SPIDevice.h>
#include <SdFat.h>
#include "Adafruit_VS1053.h"

namespace Bas
{
	class AdafruitVS1053AudioPlayer : public AudioPlayer
	{
		private:
		Adafruit_VS1053_FilePlayer adafruitVs1053FilePlayer;

	public:
		AdafruitVS1053AudioPlayer(int8_t resetPin, int8_t sciChipSelectPin, int8_t sdiChipSelectPin, int8_t dataRequestPin, SdFs *sdCard) : adafruitVs1053FilePlayer(Adafruit_VS1053_FilePlayer(resetPin, sciChipSelectPin, sdiChipSelectPin, dataRequestPin, sdCard)) {};

		void begin();
		void setVolume(uint8_t leftVolume, uint8_t rightVolume) override;
		void startPlayingFile(const char *filePath) override;
		void stopPlaying() override;
		bool getIsPlaying() override;
	};
}

#endif