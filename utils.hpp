#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

void error(const std::string &err);
void advanceWhitespace(char *&expression);
double sqrt(double x);
long gcd(long a, long b);
void printFraction(double num, double den, bool isComplex = false);

#endif
