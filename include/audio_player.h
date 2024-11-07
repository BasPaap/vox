#ifndef _AUDIO_PLAYER_H
#define _AUDIO_PLAYER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace Bas
{
    class AudioPlayer
    {
        public:
            virtual void setVolume(uint8_t leftVolume, uint8_t rightVolume) = 0;
			virtual void startPlayingFile(const char *filePath) = 0;
			virtual void stopPlaying() = 0;
			virtual bool getIsPlaying() = 0;
    };
}

#endif