#include "complex.h"


Complex::Complex(double real, double imag)
	:_real(real), _imag(imag)
{}

Complex Complex::add(const Complex& c1, const Complex& c2)
{
	return Complex(c1._real + c2._real, c1._imag + c2._imag);
}

std::string Complex::toString()
{
	return std::to_string(_real) + " + " + std::to_string(_imag) + "i";
}