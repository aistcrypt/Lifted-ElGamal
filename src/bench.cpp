#include <elgamal.hpp>
#include <cybozu/benchmark.hpp>
#include <mie/ecparam.hpp>

void bench(const char *param)
{
	printf("param %s\n", param);
	elgamal::System::init(param);
	elgamal::PrivateKey prv;
	CYBOZU_BENCH("init", prv.init);
	const elgamal::PublicKey& pub = prv.getPublicKey();

	elgamal::CipherText c;
	CYBOZU_BENCH("enc", pub.enc, c, 12345);
	// not set cache
	CYBOZU_BENCH_C("dec wo tbl", 10, prv.dec, c);
	prv.setCache(0, 20000);
	CYBOZU_BENCH("dec w  tbl", prv.dec, c);
	CYBOZU_BENCH("rand", pub.rerandomize, c);
}

int main(int argc, char *argv[])
	try
{
	static const char tbl[][16] = {
		"secp192k1",
		"NIST_P192",
		"secp224k1",
		"NIST_P224",
		"secp256k1",
		"NIST_P256",
		"secp384r1",
		"secp521r1",
	};
	if (argc == 1) {
		for (size_t i = 0; i < CYBOZU_NUM_OF_ARRAY(tbl); i++) {
			bench(tbl[i]);
		}
	} else {
		bench(argv[1]);
	}
} catch (std::exception& e) {
	printf("ERR %s\n", e.what());
}
