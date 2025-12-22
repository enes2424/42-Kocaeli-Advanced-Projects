#include "parser.hpp"
#include "utils.hpp"
#include <cstring>
#include <limits>

using namespace std;

map<long, double> parseExpression(char *expression)
{
	if (strchr(expression, '=') == NULL)
		error("Equation must contain '=' sign");
	else if (strchr(expression, 'X') == NULL)
		error("Equation must contain variable 'X'");

	char *endPtr;
	map<long, double> terms;
	double coeff;
	long exp;
	bool onRightSide = false;
	bool firstTerm = true;
	short int sign;

	advanceWhitespace(expression);

	while (*expression)
	{
		if (*expression == '-')
		{
			sign = -1;
			expression++;
		}
		else
		{
			if (*expression == '+')
				expression++;
			else if (!firstTerm)
				error("Expected '+' or '-' or '=' between terms");

			sign = 1;
		}

		advanceWhitespace(expression);

		if (*expression == 'X')
			coeff = 1;
		else
		{
			if (*expression == '+' || *expression == '-')
				error("Invalid sign sequence (e.g., '-+' or '--')");

			if (!isdigit(*expression) && *expression != '.')
				error("Expected number or 'X'");

			coeff = strtod(expression, &expression);

			if (coeff == numeric_limits<double>::infinity())
				error("Coefficient out of range");

			advanceWhitespace(expression);

			if (*expression == '*')
			{
				expression++;

				advanceWhitespace(expression);

				if (*expression != 'X')
					error("Expected 'X' after '*'");
			}
			else if (*expression == 'X')
				error("Expected '*' before 'X'");
		}

		if (!*expression || *expression == '+' || *expression == '-' || *expression == '=')
			exp = 0;
		else if (*expression == 'X')
		{
			expression++;

			advanceWhitespace(expression);

			if (*expression != '^')
				exp = 1;
			else
			{
				expression++;

				advanceWhitespace(expression);

				if (!isdigit(*expression))
					error("Expected exponent after '^'");

				exp = strtol(expression, &endPtr, 10);

				if (exp < 0)
					error("Negative exponents are not supported");

				if (exp == 9223372036854775807 && strncmp(expression, "9223372036854775807", endPtr - expression))
					error("Exponent out of range");

				expression = endPtr;
			}
		}
		else
			error("Unexpected character in expression");

		advanceWhitespace(expression);

		terms[exp] += sign * (onRightSide ? -coeff : coeff);

		if (*expression == '=')
		{
			if (onRightSide)
				error("Multiple '=' signs in expression");

			onRightSide = true;
			firstTerm = true;
			expression++;
		}
		else if (*expression && *expression != '+' && *expression != '-')
			error("Unexpected character in expression");
		else
			firstTerm = false;

		advanceWhitespace(expression);
	}

	if (firstTerm)
		error("Incomplete expression");

	return terms;
}
