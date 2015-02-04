#include <cybozu/test.hpp>
#include <mie/fp.hpp>
#include <mie/gmp_util.hpp>
#include <mie/elgamal.hpp>
#include <cybozu/random_generator.hpp>
#include <mie/ecparam.hpp>
#include <cybozu/crypto.hpp>
#include <cybozu/benchmark.hpp>
#if defined(_WIN64) || defined(__x86_64__)
	#define USE_MONT_FP
#endif
#ifdef USE_MONT_FP
#include <mie/mont_fp.hpp>
typedef mie::MontFpT<3> Fp;
#else
typedef mie::FpT<mie::Gmp> Fp;
#endif
typedef mie::EcT<Fp> Ec;

struct TagFp;
struct TagEc;

const mie::EcParam& para = mie::ecparam::secp192k1;
cybozu::RandomGenerator rg;

CYBOZU_TEST_AUTO(testFp)
{
	typedef mie::FpT<mie::Gmp, TagFp> Zn;
	typedef mie::ElgamalT<Fp, Zn> ElgamalFp;
	/*
		Zn = (Z/mZ) - {0}
	*/
	const int m = 65537;
	{
		std::ostringstream os;
		os << m;
		Fp::setModulo(os.str());
	}
	{
		std::ostringstream os;
		os << m - 1;
		Zn::setModulo(os.str());
	}
	ElgamalFp::PrivateKey prv;

	/*
		3^(m-1) = 1
	*/
	const int f = 3;
	{
		Fp x(f);
		Fp::power(x, x, m - 1);
		CYBOZU_TEST_EQUAL(x, 1);
	}
	prv.init(f, 17, rg);
	const ElgamalFp::PublicKey& pub = prv.getPublicKey();

	const int m1 = 12345;
	const int m2 = 17655;
	ElgamalFp::CipherText c1, c2;
	pub.enc(c1, m1, rg);
	pub.enc(c2, m2, rg);
	// BitVector
	{
		cybozu::BitVector bv;
		c1.appendToBitVec(bv);
		ElgamalFp::CipherText c3;
		c3.fromBitVec(bv);
		CYBOZU_TEST_EQUAL(c1.c1, c3.c1);
		CYBOZU_TEST_EQUAL(c1.c2, c3.c2);
	}
	Zn dec1, dec2;
	prv.dec(dec1, c1);
	prv.dec(dec2, c2);
	// dec(enc) = id
	CYBOZU_TEST_EQUAL(dec1, m1);
	CYBOZU_TEST_EQUAL(dec2, m2);
	// iostream
	{
		ElgamalFp::PublicKey pub2;
		ElgamalFp::PrivateKey prv2;
		ElgamalFp::CipherText cc1, cc2;
		{
			std::stringstream ss;
			ss << prv;
			ss >> prv2;
		}
		Zn d;
		prv2.dec(d, c1);
		CYBOZU_TEST_EQUAL(d, m1);
		{
			std::stringstream ss;
			ss << c1;
			ss >> cc1;
		}
		d = 0;
		prv2.dec(d, cc1);
		CYBOZU_TEST_EQUAL(d, m1);
		{
			std::stringstream ss;
			ss << pub;
			ss >> pub2;
		}
		pub2.enc(cc2, m2, rg);
		prv.dec(d, cc2);
		CYBOZU_TEST_EQUAL(d, m2);
	}
	// enc(m1) enc(m2) = enc(m1 + m2)
	c1.add(c2);
	prv.dec(dec1, c1);
	CYBOZU_TEST_EQUAL(dec1, m1 + m2);
	// enc(m1) x = enc(m1 + x)
	const int x = 555;
	pub.add(c1, x);
	prv.dec(dec1, c1);
	CYBOZU_TEST_EQUAL(dec1, m1 + m2 + x);
	// rerandomize
	c1 = c2;
	pub.rerandomize(c1, rg);
	// verify c1 != c2
	CYBOZU_TEST_ASSERT(c1.c1 != c2.c1);
	CYBOZU_TEST_ASSERT(c1.c2 != c2.c2);
	prv.dec(dec1, c1);
	// dec(c1) = dec(c2)
	CYBOZU_TEST_EQUAL(dec1, m2);

	// check neg
	{
		ElgamalFp::CipherText c;
		Zn m = 1234;
		pub.enc(c, m, rg);
		c.neg();
		Zn dec;
		prv.dec(dec, c);
		CYBOZU_TEST_EQUAL(dec, -m);
	}
	// check mul
	{
		ElgamalFp::CipherText c;
		Zn m = 1234;
		int x = 111;
		pub.enc(c, m, rg);
		c.mul(x);
		Zn dec;
		prv.dec(dec, c);
		m *= x;
		CYBOZU_TEST_EQUAL(dec, m);
	}
	// check negative value
	for (int i = -10; i < 10; i++) {
		ElgamalFp::CipherText c;
		const Zn mm = i;
		pub.enc(c, mm, rg);
		Zn dec;
		prv.dec(dec, c, 1000);
		CYBOZU_TEST_EQUAL(dec, mm);
	}

	// isZeroMessage
	for (int m = 0; m < 10; m++) {
		ElgamalFp::CipherText c0;
		pub.enc(c0, m, rg);
		if (m == 0) {
			CYBOZU_TEST_ASSERT(prv.isZeroMessage(c0));
		} else {
			CYBOZU_TEST_ASSERT(!prv.isZeroMessage(c0));
		}
	}
	// zkp
	{
		ElgamalFp::Zkp zkp;
		ElgamalFp::CipherText c;
		cybozu::crypto::Hash hash(cybozu::crypto::Hash::N_SHA256);
		pub.encWithZkp(c, zkp, 0, hash, rg);
		CYBOZU_TEST_ASSERT(pub.verify(c, zkp, hash));
		zkp.s0 += 1;
		CYBOZU_TEST_ASSERT(!pub.verify(c, zkp, hash));
		pub.encWithZkp(c, zkp, 1, hash, rg);
		CYBOZU_TEST_ASSERT(pub.verify(c, zkp, hash));
		zkp.s0 += 1;
		CYBOZU_TEST_ASSERT(!pub.verify(c, zkp, hash));
		CYBOZU_TEST_EXCEPTION_MESSAGE(pub.encWithZkp(c, zkp, 2, hash, rg), cybozu::Exception, "encWithZkp");
	}
}

CYBOZU_TEST_AUTO(testEc)
{
	typedef mie::FpT<mie::Gmp, TagEc> Zn;
	typedef mie::ElgamalT<Ec, Zn> ElgamalEc;
	Fp::setModulo(para.p);
	Zn::setModulo(para.n);
	Ec::setParam(para.a, para.b);
	const Fp x0(para.gx);
	const Fp y0(para.gy);
	const size_t bitLen = Zn(-1).getBitLen();
	const Ec P(x0, y0);
	/*
		Zn = <P>
	*/
	ElgamalEc::PrivateKey prv;
	prv.init(P, bitLen, rg);
	prv.setCache(0, 60000);
	const ElgamalEc::PublicKey& pub = prv.getPublicKey();

	const int m1 = 12345;
	const int m2 = 17655;
	ElgamalEc::CipherText c1, c2;
	pub.enc(c1, m1, rg);
	pub.enc(c2, m2, rg);
	// BitVector
	{
		cybozu::BitVector bv;
		c1.appendToBitVec(bv);
		ElgamalEc::CipherText c3;
		c3.fromBitVec(bv);
		CYBOZU_TEST_EQUAL(c1.c1, c3.c1);
		CYBOZU_TEST_EQUAL(c1.c2, c3.c2);
	}
	Zn dec1, dec2;
	prv.dec(dec1, c1);
	prv.dec(dec2, c2);
	// dec(enc) = id
	CYBOZU_TEST_EQUAL(dec1, m1);
	CYBOZU_TEST_EQUAL(dec2, m2);
	// iostream
	{
		ElgamalEc::PublicKey pub2;
		ElgamalEc::PrivateKey prv2;
		ElgamalEc::CipherText cc1, cc2;
		{
			std::stringstream ss;
			ss << prv;
			ss >> prv2;
		}
		prv.setCache(-200, 60000);
		Zn d;
		prv2.dec(d, c1);
		CYBOZU_TEST_EQUAL(d, m1);
		{
			std::stringstream ss;
			ss << c1;
			ss >> cc1;
		}
		d = 0;
		prv2.dec(d, cc1);
		CYBOZU_TEST_EQUAL(d, m1);
		{
			std::stringstream ss;
			ss << pub;
			ss >> pub2;
		}
		pub2.enc(cc2, m2, rg);
		prv.dec(d, cc2);
		CYBOZU_TEST_EQUAL(d, m2);
	}
	// enc(m1) enc(m2) = enc(m1 + m2)
	c1.add(c2);
	prv.dec(dec1, c1);
	CYBOZU_TEST_EQUAL(dec1, m1 + m2);
	// enc(m1) x = enc(m1 + x)
	const int x = 555;
	pub.add(c1, x);
	prv.dec(dec1, c1);
	CYBOZU_TEST_EQUAL(dec1, m1 + m2 + x);
	// rerandomize
	c1 = c2;
	pub.rerandomize(c1, rg);
	// verify c1 != c2
	CYBOZU_TEST_ASSERT(c1.c1 != c2.c1);
	CYBOZU_TEST_ASSERT(c1.c2 != c2.c2);
	prv.dec(dec1, c1);
	// dec(c1) = dec(c2)
	CYBOZU_TEST_EQUAL(dec1, m2);

	// check neg
	{
		ElgamalEc::CipherText c;
		Zn m = 1234;
		pub.enc(c, m, rg);
		c.neg();
		Zn dec;
		prv.dec(dec, c);
		CYBOZU_TEST_EQUAL(dec, -m);
	}
	// check mul
	{
		ElgamalEc::CipherText c;
		Zn m = 123;
		int x = 111;
		pub.enc(c, m, rg);
		Zn dec;
		prv.dec(dec, c);
		c.mul(x);
		prv.dec(dec, c);
		m *= x;
		CYBOZU_TEST_EQUAL(dec, m);
	}

	// check negative value
	for (int i = -10; i < 10; i++) {
		ElgamalEc::CipherText c;
		const Zn mm = i;
		pub.enc(c, mm, rg);
		Zn dec;
		prv.dec(dec, c, 1000);
		CYBOZU_TEST_EQUAL(dec, mm);
	}

	// isZeroMessage
	for (int m = 0; m < 10; m++) {
		ElgamalEc::CipherText c0;
		pub.enc(c0, m, rg);
		if (m == 0) {
			CYBOZU_TEST_ASSERT(prv.isZeroMessage(c0));
		} else {
			CYBOZU_TEST_ASSERT(!prv.isZeroMessage(c0));
		}
	}
	// zkp
	{
		ElgamalEc::Zkp zkp;
		ElgamalEc::CipherText c;
//		cybozu::Sha1 hash;
		cybozu::crypto::Hash hash(cybozu::crypto::Hash::N_SHA256);
		pub.encWithZkp(c, zkp, 0, hash, rg);
		CYBOZU_TEST_ASSERT(pub.verify(c, zkp, hash));
		zkp.s0 += 1;
		CYBOZU_TEST_ASSERT(!pub.verify(c, zkp, hash));
		pub.encWithZkp(c, zkp, 1, hash, rg);
		CYBOZU_TEST_ASSERT(pub.verify(c, zkp, hash));
		zkp.s0 += 1;
		CYBOZU_TEST_ASSERT(!pub.verify(c, zkp, hash));
		CYBOZU_TEST_EXCEPTION_MESSAGE(pub.encWithZkp(c, zkp, 2, hash, rg), cybozu::Exception, "encWithZkp");
	}
	// cache
	{
		const int m1 = 9876;
		const int m2 = -3142;
		ElgamalEc::CipherText c1, c2;
		pub.enc(c1, m1, rg);
		pub.enc(c2, m2, rg);
		prv.setCache(-10000, 10000);
		int dec1 = prv.dec(c1);
		int dec2 = prv.dec(c2);
		CYBOZU_TEST_EQUAL(m1, dec1);
		CYBOZU_TEST_EQUAL(m2, dec2);
		c1.add(c2);
		bool b;
		int dec = prv.dec(c1, &b);
		CYBOZU_TEST_EQUAL(m1 + m2, dec);
		CYBOZU_TEST_ASSERT(b);
		prv.clearCache();
		prv.dec(c1, &b);
		CYBOZU_TEST_ASSERT(!b);
	}
	// benchmark
	{
		int m = 12345;
		ElgamalEc::CipherText c;
		CYBOZU_BENCH("enc", pub.enc, c, m, rg);
		prv.setCache(0, 20000);
		CYBOZU_BENCH("dec", prv.dec, c);
		CYBOZU_BENCH("rand", pub.rerandomize, c, rg);
	}
}
