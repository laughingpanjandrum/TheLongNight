
#include "utility.h"

/*
Returns random integer from a to b inclusive.
If a and b are the same, returns a.
*/
int randint(int a, int b) {
	//If the range contains only one value, welp, that's pretty easy!
	if (a == b)
		return a;
	//Otherwise return random
	return std::rand() % (b - a) + a;
}

/*
Returns random integer from 0 to num-1.
Mostly useful for picking random array elements using the array size.
*/
int randrange(int num) {
	return randint(1, num) - 1;
}

/*
Returns
- minValue if n < minValue;
- maxValue if n < maxValue;
- otherwise, n.
*/
int constrain(int n, int minValue, int maxValue)
{
	if (n < minValue)
		n = minValue;
	else if (n > maxValue)
		n = maxValue;
	return n;
}

/*
Returns:
	-1 if destination is less than ourPt
	1 if destination is greater than ourPt
	0 otherwise
*/
int get1dVector(int ourPt, int destination)
{
	if (destination < ourPt)
		return -1;
	else if (destination > ourPt)
		return 1;
	return 0;
}
