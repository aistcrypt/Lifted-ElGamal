#include <cybozu/test.hpp>
#include <mie/power_window.hpp>
#include <mie/fp.hpp>
#include <mie/gmp_util.hpp>


CYBOZU_TEST_AUTO(int)
{
	typedef mie::FpT<mie::Gmp> Fp;
	Fp::setModulo("65537");

	typedef mie::PowerWindow<Fp> PW;
	const Fp g = 123;
	const size_t bitLen = 16;
	for (size_t winSize = 1; winSize <= 16; winSize++) {
		PW pw(g, bitLen, winSize);
		for (int i = 0; i < (1 << bitLen); i++) {
			Fp x, y;
			pw.power(x, i);
			Fp::power(y, g, i);
			CYBOZU_TEST_EQUAL(x, y);
		}
	}
}
