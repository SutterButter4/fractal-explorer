#include <iostream>
#include <limits.h>

#include "complex.h"
#include "percision_uint.h"

int main()
{
	// Complex c1(1.0, 1.0);
	// Complex c2(1.0, 1.0);

	// Complex c3 = Complex::add(c1, c2);
	// std::cout << c1.toString() << " + " << c2.toString() << " = " << c3.toString() << std::endl;

	//PercisionUInt<2> a(5);
	//PercisionUInt<2> b(3);
	//PercisionUInt<2> c(PercisionUInt<2>::add(a, b));
	//PercisionUInt<2> d(PercisionUInt<2>::multiply(a, b));

	//auto arr = multLong(10000000000, 1000000000000000000);
	//std::cout << arr[0] << ", " << arr[1] << std::endl;
	//std::cout << a.toString() << b.toString() << c.toString() << d.toString() << std::endl;
	
	std::cout << PercisionUInt<128>::bitshiftLeft(
		PercisionUInt<128>::pow(PercisionUInt<128>("12345678"), PercisionUInt<128>(1)),
		2).toHex() << std::endl;
	// std::cout << PercisionUInt<16>("1002349823421235123512").toString() << std::endl;
	// std::cout << PercisionUInt<16>(ULONG_MAX).toString() << std::endl;
	// std::cout << PercisionUInt<2>::multiply(PercisionUInt<2>(1), PercisionUInt<2>(1)).toString() << std::endl;
	// std::cout << PercisionUInt<2>::multiply(PercisionUInt<2>(1>>33), PercisionUInt<2>(1>>32)).toString() << std::endl;

	//std::cout << multLong(3, 5)[0] << multLong(3, 5)[1] << std::endl;
	//std::cout << 10460353203 <<","<<multLong(3, 10460353203)[0] << ","<< multLong(3, 10460353203)[1] << std::endl;

	return 0;
}
