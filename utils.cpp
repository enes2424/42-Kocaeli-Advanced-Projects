#include "utils.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;

void error(const string &err)
{
	cerr << "Error: " << err << endl;
	exit(1);
}

void advanceWhitespace(char *&expression)
{
	while (isspace(*expression))
		expression++;
}

double sqrt(double x)
{
	double guess = x;
	double prev;
	double diff;

	do
	{
		prev = guess;
		guess = 0.5 * (guess + x / guess);

		if (guess > prev)
			diff = guess - prev;
		else
			diff = prev - guess;

	} while (diff > 1e-15);

	return guess;
}

long gcd(long a, long b)
{
	if (a < 0)
		a = -a;
	if (b < 0)
		b = -b;

	long temp;

	while (b != 0)
	{
		temp = b;
		b = a % b;
		a = temp;
	}

	return a;
}

void printFraction(double num, double den, bool isComplex)
{
	long numerator = (long)(num * 1000000);
	long denominator = (long)(den * 1000000);

	long divisor = gcd(numerator, denominator);

	numerator /= divisor;
	denominator /= divisor;

	if (denominator < 0)
	{
		numerator = -numerator;
		denominator = -denominator;
	}

	if (denominator == 1 || numerator % denominator == 0)
		cout << numerator / denominator << (isComplex ? "i" : "");
	else
		cout << numerator << (isComplex ? "i" : "") << "/" << denominator;
}
