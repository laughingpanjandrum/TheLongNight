#include "counter.h"



counter::counter()
{
}

/*
If 'value' is not provided, the default starting value is the max value.
*/
counter::counter(int maxValue, int value): maxValue(maxValue)
{
	if (value == -1)
		this->value = maxValue;
	else
		this->value = value;
}


counter::~counter()
{
}

/*
Set both maximum and starting values.
If no starting value is provided, defaults to max value.
*/
void counter::setTo(int maxVal, int startVal)
{
	maxValue = maxVal;
	if (startVal == -1)
		value = maxVal;
	else
		value = startVal;
}

/*
	ADJUSTING VALUE
*/

/*
Returns our current value as a fraction of our maximum value.
*/
float counter::getPercent()
{
	float f = (float)value / (float)maxValue;
	return f;
}

/*
Returns string representation of value/maxValue.
*/
std::string counter::getAsString()
{
	return std::to_string(value) + '/' + std::to_string(maxValue);
}

/*
Adjust current value. Caps to max or min.
*/
void counter::add(int amount)
{
	value += amount;
	if (value > maxValue)
		value = maxValue;
	else if (value < 0)
		value = 0;
}

/*
Changes the maximum value.
If adjustCurrentValue is true, we will apply the same change to the maximum value.
*/
void counter::increaseMaxValue(int amount, bool adjustCurrentValue)
{
	maxValue += amount;
	if (adjustCurrentValue)
		value += amount;
}
