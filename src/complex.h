#pragma once
#include <string>

class Complex 
{
public:
	Complex(double real, double imag);

	static Complex add(const Complex& c1, const Complex& c2);
	static Complex subtract(const Complex& c1, const Complex& c2);
	static Complex multiply(const Complex& c1, const Complex& c2);

	std::string toString();
private:
	double _real, _imag;
};
