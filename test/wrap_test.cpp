/*
	test of include/elgamal.hpp not include/mie/elgamal.hpp
*/
#define CYBOZU_TEST_DISABLE_AUTO_RUN
#include <cybozu/test.hpp>
#include <mie/ecparam.hpp>
#include <cybozu/benchmark.hpp>
#include <elgamal.hpp>

std::string g_ecStr;

CYBOZU_TEST_AUTO(testEc)
{
	elgamal::System::init(g_ecStr);
	elgamal::PrivateKey prv;
	prv.init();
	prv.setCache(-30000, 30000);
	const elgamal::PublicKey& pub = prv.getPublicKey();

	const int m1 = 1234;
	const int m2 = 1765;
	elgamal::CipherText c1, c2;
	pub.enc(c1, m1);
	pub.enc(c2, m2);
	int dec1 = prv.dec(c1);
	int dec2 = prv.dec(c2);
	// dec(enc) = id
	CYBOZU_TEST_EQUAL(dec1, m1);
	CYBOZU_TEST_EQUAL(dec2, m2);
	// iostream
	{
		elgamal::PublicKey pub2;
		elgamal::PrivateKey prv2;
		elgamal::CipherText cc1, cc2;
		{
			std::stringstream ss;
			ss << prv;
			ss >> prv2;
			prv2.setCache(-10000, 10000);
		}
		int d = prv2.dec(c1);
		CYBOZU_TEST_EQUAL(d, m1);
		{
			std::stringstream ss;
			ss << c1;
			ss >> cc1;
		}
		d = prv2.dec(cc1);
		CYBOZU_TEST_EQUAL(d, m1);
		{
			std::stringstream ss;
			ss << pub;
			ss >> pub2;
		}
		pub2.enc(cc2, m2);
		d = prv.dec(cc2);
		CYBOZU_TEST_EQUAL(d, m2);
	}
	// enc(m1) enc(m2) = enc(m1 + m2)
	c1.add(c2);
	dec1 = prv.dec(c1);
	CYBOZU_TEST_EQUAL(dec1, m1 + m2);
	// enc(m1) x = enc(m1 + x)
	const int x = 555;
	pub.add(c1, x);
	dec1 = prv.dec(c1);
	CYBOZU_TEST_EQUAL(dec1, m1 + m2 + x);
	// rerandomize
	c1 = c2;
	pub.rerandomize(c1);
	// verify c1 != c2
	CYBOZU_TEST_ASSERT(c1.toStr() != c2.toStr());
	dec1 = prv.dec(c1);
	// dec(c1) = dec(c2)
	CYBOZU_TEST_EQUAL(dec1, m2);

	// check neg
	{
		elgamal::CipherText c;
		int m = 1234;
		pub.enc(c, m);
		c.neg();
		int dec = prv.dec(c);
		CYBOZU_TEST_EQUAL(dec, -m);
	}
	// check mul
	{
		elgamal::CipherText c;
		int m = 123;
		int x = 111;
		pub.enc(c, m);
		int dec = prv.dec(c);
		c.mul(x);
		dec = prv.dec(c);
		m *= x;
		CYBOZU_TEST_EQUAL(dec, m);
	}

	// check negative value
	for (int i = -10; i < 10; i++) {
		elgamal::CipherText c;
		pub.enc(c, i);
		int dec = prv.dec(c);
		CYBOZU_TEST_EQUAL(dec, i);
	}

	// isZeroMessage
	for (int m = 0; m < 10; m++) {
		elgamal::CipherText c0;
		pub.enc(c0, m);
		if (m == 0) {
			CYBOZU_TEST_ASSERT(prv.isZeroMessage(c0));
		} else {
			CYBOZU_TEST_ASSERT(!prv.isZeroMessage(c0));
		}
	}
	// zkp
	{
		elgamal::Zkp zkp;
		elgamal::CipherText c;
		pub.encWithZkp(c, zkp, 0);
		CYBOZU_TEST_ASSERT(pub.verify(c, zkp));
//		zkp.s0 += 1;
//		CYBOZU_TEST_ASSERT(!pub.verify(c, zkp));
		pub.encWithZkp(c, zkp, 1);
		CYBOZU_TEST_ASSERT(pub.verify(c, zkp));
//		zkp.s0 += 1;
//		CYBOZU_TEST_ASSERT(!pub.verify(c, zkp));
		CYBOZU_TEST_EXCEPTION_MESSAGE(pub.encWithZkp(c, zkp, 2), cybozu::Exception, "encWithZkp");
	}
	// cache
	{
		const int m1 = 9876;
		const int m2 = -3142;
		elgamal::CipherText c1, c2;
		pub.enc(c1, m1);
		pub.enc(c2, m2);
		int dec1 = prv.dec(c1);
		int dec2 = prv.dec(c2);
		CYBOZU_TEST_EQUAL(m1, dec1);
		CYBOZU_TEST_EQUAL(m2, dec2);
		c1.add(c2);
		CYBOZU_TEST_EQUAL(m1 + m2, prv.dec(c1));
		// bench
		CYBOZU_BENCH("cache", prv.dec, c1);
		bool b;
		prv.dec(c1, &b);
		CYBOZU_TEST_ASSERT(b);
		prv.clearCache();
		prv.dec(c1, &b);
		CYBOZU_TEST_ASSERT(!b);
	}
}

int main(int argc, char *argv[])
	try
{
	static const char tbl[][32] = {
		"NIST_P192 sha256",
		"secp192k1 sha256",
		"NIST_P224 sha256",
		"secp224k1 sha256",
		"NIST_P256 sha256",
		"secp256k1 sha256",
		"NIST_P384 sha384",
		"NIST_P521 sha512",
	};
	g_ecStr = argc == 1 ? tbl[0] : argv[1];
	printf("ec %s\n", g_ecStr.c_str());
	return cybozu::test::autoRun.run(argc, argv);
} catch (std::exception& e) {
	printf("ERR %s\n", e.what());
}
