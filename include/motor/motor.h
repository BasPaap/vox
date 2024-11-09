#ifndef _MOTOR_H
#define _MOTOR_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace Bas
{
    class Motor
    {
		private:
		pin_size_t pinA;
		pin_size_t pinB;

		public:
		Motor(pin_size_t pinA, pin_size_t pinB) : pinA(pinA), pinB(pinB) {}
		void begin();
		void startSpinning();
		void stopSpinning();
    };
}

#endif