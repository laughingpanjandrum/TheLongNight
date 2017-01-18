

#ifndef UTILITY_H
#define UTILITY_H

#include <cstdlib>
#include <string>

int randint(int a, int b);
int randrange(int num);
int constrain(int n, int minValue, int maxValue);
int get1dVector(int ourPt, int destination);

std::string centreText(std::string txt, int width);

#endif