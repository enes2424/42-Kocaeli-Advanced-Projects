#include "solver.hpp"
#include "utils.hpp"
#include <iostream>

using namespace std;

void printReducedForm(const map<long, double> &terms)
{
	cout << "Reduced form: ";

	for (const auto &term : terms)
	{
		if (term.second < 0)
		{
			cout << '-';

			if (term != *terms.begin())
				cout << ' ';
		}
		else if (term != *terms.begin() && term.second >= 0)
			cout << "+ ";

		if (term.second >= 0)
			cout << term.second;
		else
			cout << -term.second;

		if (term.first != 0)
		{
			cout << " * X";

			if (term.first != 1)
				cout << "^" << term.first;
		}

		cout << " ";
	}

	cout << "= 0" << endl;
}

long getPolynomialDegree(const map<long, double> &terms)
{
	long degree = -1;

	for (const auto &term : terms)
	{
		if (term.second != 0)
			degree = term.first;
	}

	return degree;
}

void solveLinear(const map<long, double> &terms)
{
	double a = terms.count(1) ? terms.at(1) : 0;
	double b = terms.count(0) ? terms.at(0) : 0;

	cout << "The solution is:" << endl
		 << endl;
	cout << "X = -b / a" << endl
		 << endl;
	cout << "X = -(" << b << ") / " << a << endl;

	if (b == 0)
		cout << 0 << endl;
	else
	{
		cout << "X = ";
		cout << -b / a << " => ";
		printFraction(-b, a);
		cout << endl;
	}
}

void solveQuadratic(const map<long, double> &terms)
{
	double a = terms.count(2) ? terms.at(2) : 0;
	double b = terms.count(1) ? terms.at(1) : 0;
	double c = terms.count(0) ? terms.at(0) : 0;

	cout << endl
		 << "Discriminant Δ = b² - 4ac" << endl
		 << endl;
	cout << "Δ = (" << b << ")² - 4 * " << a << " * " << c << endl;

	double discriminant = b * b - 4 * a * c;

	cout << "Δ = " << discriminant << endl
		 << endl;

	if (discriminant > 0)
	{
		cout << "Discriminant is strictly positive, the two solutions are:" << endl;

		double sqrtDiscriminant = sqrt(discriminant);

		cout << "X = (-b ± √Δ) / 2a" << endl
			 << endl;
		cout << "X₁ = (-(" << b << ") - √" << discriminant << ") / (2 * " << a << ")" << endl;
		cout << "X₁ = (" << -b << " - " << sqrtDiscriminant << ") / " << 2 * a << endl;
		cout << "X₁ = ";
		cout << (-b - sqrtDiscriminant) / (2 * a) << " => ";
		printFraction(-b - sqrtDiscriminant, 2 * a);

		cout << endl
			 << endl;

		cout << "X₂ = (-(" << b << ") + √" << discriminant << ") / (2 * " << a << ")" << endl;
		cout << "X₂ = (" << -b << " + " << sqrtDiscriminant << ") / " << 2 * a << endl;
		cout << "X₂ = ";
		cout << (-b + sqrtDiscriminant) / (2 * a) << " => ";
		printFraction(-b + sqrtDiscriminant, 2 * a);
	}
	else if (discriminant == 0)
	{
		cout << "Discriminant is zero, the solution is:" << endl;
		cout << "X = -b / 2a" << endl
			 << endl;
		cout << "X = -(" << b << ") / (2 * " << a << ")" << endl;
		cout << "X = " << -b << " / " << 2 * a << endl;
		cout << "X = ";
		cout << -b / (2 * a) << " => ";
		printFraction(-b, 2 * a);
	}
	else if (discriminant < 0)
	{
		cout << "Discriminant is strictly negative, the two complex solutions are:" << endl;

		double sqrtDiscriminant = sqrt(-discriminant);

		cout << "X = (-b ± i√|Δ|) / 2a" << endl
			 << endl;

		cout << "X₁ = ";
		cout << -b / (2 * a) << " + " << sqrtDiscriminant / (2 * a) << "i => ";
		printFraction(-b, 2 * a);

		cout << " + ";

		printFraction(sqrtDiscriminant, 2 * a, true);

		cout << endl;

		cout << "X₂ = ";
		cout << -b / (2 * a) << " - " << sqrtDiscriminant / (2 * a) << "i => ";
		printFraction(-b, 2 * a);

		cout << " - ";

		printFraction(sqrtDiscriminant, 2 * a, true);
	}

	cout << endl;
}
