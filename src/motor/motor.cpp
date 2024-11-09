#include "motor/motor.h"

void Bas::Motor::begin()
{
	pinMode(this->pinA, OUTPUT);
	pinMode(this->pinB, OUTPUT);

	stopSpinning();
}

void Bas::Motor::startSpinning()
{
	const int speed = 127;
	analogWrite(this->pinA, LOW);
	analogWrite(this->pinB, 127);
}

void Bas::Motor::stopSpinning()
{
	analogWrite(this->pinA, LOW);
	analogWrite(this->pinB, LOW);
}
