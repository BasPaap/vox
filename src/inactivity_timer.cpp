#include "inactivity_timer.h"

Bas::InactivityTimer::InactivityTimer(LogLevel logLevel) : logLevel{logLevel}
{
}

void Bas::InactivityTimer::update()
{
	unsigned long now = millis();

	if (isRunning && now - startTime >= duration)
	{
		isRunning = false;
		if (this->logLevel == LogLevel::normal)
		{
			Serial.print("InactivityTimer ran out after ");
			Serial.print(now - startTime);
			Serial.println(" milliseconds.");
		}

		callbackCaller.call();
	}
}

void Bas::InactivityTimer::reset()
{
	isRunning = true;
	startTime = millis();
}
