#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <map>

void printReducedForm(const std::map<long, double> &terms);
long getPolynomialDegree(const std::map<long, double> &terms);
void solveLinear(const std::map<long, double> &terms);
void solveQuadratic(const std::map<long, double> &terms);

#endif
