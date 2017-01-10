
/*
A counter has a maximum value and a current value.
Most of the built-in functions are just easy ways to mess with the value.
*/

#ifndef COUNTER_H
#define COUNTER_H

#include <string>

class counter
{
public:
	//Construction
	counter();
	counter(int maxValue, int value = -1);
	~counter();
	//Setting
	void setValue(int v) { value = v; }
	void setMaxValue(int mv) { maxValue = mv; }
	void setTo(int v) { value = v; maxValue = v; }
	//Getting
	int getValue() { return value; }
	int getMaxValue() { return maxValue; }
	float getPercent();
	//Getting string representation
	std::string getAsString();
	//Incrementing
	void add(int amount);
	void increase(int amount = 1) { add(amount); }
	void decrease(int amount = 1) { add(-amount); }
	//Resetting
	void clear() { value = 0; }
	void restore() { value = maxValue; }
	//Checking
	bool isEmpty() { return value == 0; }
	bool isFull() { return value == maxValue; }
private:
	int value;
	int maxValue;
};

#endif
