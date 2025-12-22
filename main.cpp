#include "utils.hpp"
#include "parser.hpp"
#include "solver.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	string inputLine;
	char *expression;

	if (argc == 1)
	{
		getline(cin, inputLine);
		expression = &inputLine[0];
	}
	else if (argc == 2)
		expression = argv[1];
	else
	{
		cerr << "Usage: " << argv[0] << " [equation]" << endl;
		return 1;
	}

	map<long, double> terms = parseExpression(expression);

	printReducedForm(terms);

	long degree = getPolynomialDegree(terms);

	if (degree == -1)
		cout << "Any real number is a solution." << endl;
	else if (degree == 0)
		cout << "No solution." << endl;
	else
		cout << "Polynomial degree: " << degree << endl;

	if (degree == 1)
		solveLinear(terms);
	else if (degree == 2)
		solveQuadratic(terms);
	else if (degree > 2)
		cout << "The polynomial degree is strictly greater than 2, I can't solve." << endl;

	return 0;
}
