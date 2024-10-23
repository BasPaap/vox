#ifndef _INACTIVITY_TIMER_H
#define _INACTIVITY_TIMER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <CallbackCaller.h>

namespace Bas
{
	class InactivityTimer
	{
	public:
		enum class LogLevel
		{
			none = 0,
			normal
		};

	private:
		CallbackCaller callbackCaller;
		bool isRunning = false;
		unsigned long duration;
		LogLevel logLevel;
		unsigned long startTime;

	public:
		InactivityTimer(LogLevel logLevel = LogLevel::none);

		template <typename Function>
		void begin(unsigned long duration, Function callback)
		{
			if (this->logLevel == LogLevel::normal)
			{
				Serial.println("Initializing InactivityTimer.");
			}

			this->duration = duration;
			this->callbackCaller.begin(callback);
			isRunning = true;
			reset();
		}

		void update();
		void reset();
	};
}

#endif