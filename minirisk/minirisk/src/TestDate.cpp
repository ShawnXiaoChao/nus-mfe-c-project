#include <iomanip>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include "Date.h"
#include <ctime>
#include <array>

using namespace::minirisk;

void test1()
{
	srand(time(NULL));
	std::cout << time(NULL) << std::endl;
	std::array<unsigned, 1000> test_d;
	std::array<unsigned, 1000> test_m;
	std::array<unsigned, 1000> test_y;
	std::array<Date, 1000> test_date;

	for (size_t i = 0; i < test_d.size(); ++i) {
		test_d.at(i) = rand();
		test_m.at(i) = rand();
		test_y.at(i) = rand();
	}

	for (unsigned i = 0; i < 2; ++i) {
		try {
			std::cout << test_d[i] << std::endl;
			Date temp_date(test_d[i], test_m[i], test_y[i]);
		}
		catch (std::invalid_argument & e) {
			std::cout << "The Date is wrong" << std::endl;
		}
	}
}

void test2()
{
}

void test3()
{
}

int main()
{
	//Date m(2000,10,2);
	test1();
	//test2();
	//test3();
	return 0;
}

