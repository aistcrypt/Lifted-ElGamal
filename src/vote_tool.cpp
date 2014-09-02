/*
	vote sample tool
	Copyright (c) 2014, National Institute of Advanced Industrial
	Science and Technology All rights reserved.
	This source file is subject to BSD 3-Clause license.
*/
#include <cybozu/option.hpp>
#include <iostream>
#include <fstream>
#include <cybozu/random_generator.hpp>
#include <cybozu/crypto.hpp>
#include <mie/fp.hpp>
#include <mie/gmp_util.hpp>
#include <mie/elgamal.hpp>
#include <mie/ecparam.hpp>
#if defined(_WIN64) || defined(__x86_64__)
	#define USE_MONT_FP
#endif
#ifdef USE_MONTFP
#include <mie/mont_fp.hpp>
typedef mie::MontFpT<3> Fp;
#else
typedef mie::FpT<mie::Gmp> Fp;
#endif

struct ZnTag;

typedef mie::EcT<Fp> Ec;
typedef mie::FpT<mie::Gmp, ZnTag> Zn; // use ZnTag because Zn is different class with Fp
typedef mie::ElgamalT<Ec, Zn> Elgamal;

cybozu::RandomGenerator rg;

const std::string pubFile = "vote_pub.txt";
const std::string prvFile = "vote_prv.txt";
const std::string resultFile = "vote_ret.txt";

std::string GetSheetName(size_t n)
{
	return std::string("vote_") + cybozu::itoa(n) + ".txt";
}

struct Param {
	std::string mode;
	std::string voteList;
	Param(int argc, const char *const argv[])
	{
		cybozu::Option opt;
		opt.appendOpt(&voteList, "11001100", "l", ": list of voters for vote mode(eg. 11001100)");
		opt.appendHelp("h", ": put this message");
		opt.appendParam(&mode, "mode", ": init/vote/count/open");
		if (!opt.parse(argc, argv)) {
			opt.usage();
			exit(1);
		}
		printf("mode=%s\n", mode.c_str());
		if (mode == "vote") {
			printf("voters=%s\n", voteList.c_str());
			size_t pos = voteList.find_first_not_of("01");
			if (pos != std::string::npos) {
				printf("bad char %c\n", voteList[pos]);
				exit(1);
			}
		}
	}
};

void SysInit()
{
	const mie::EcParam& para = mie::ecparam::secp192k1;
	Zn::setModulo(para.n);
	Fp::setModulo(para.p);
	Ec::setParam(para.a, para.b);
}

template<class T>
bool Load(T& t, const std::string& name, bool doThrow = true)
{
	std::ifstream ifs(name.c_str(), std::ios::binary);
	if (!ifs) {
		if (doThrow) throw cybozu::Exception("Load:can't read") << name;
		return false;
	}
	if (ifs >> t) return true;
	if (doThrow) throw cybozu::Exception("Load:bad data") << name;
	return false;
}

template<class T>
void Save(const std::string& name, const T& t)
{
	std::ofstream ofs(name.c_str(), std::ios::binary);
	ofs << t;
}

void Init()
{
	const mie::EcParam& para = mie::ecparam::secp192k1;
	const Fp x0(para.gx);
	const Fp y0(para.gy);
	const Ec P(x0, y0);
	const size_t bitLen = Zn(-1).getBitLen();

	Elgamal::PrivateKey prv;
	prv.init(P, bitLen, rg);
	const Elgamal::PublicKey& pub = prv.getPublicKey();
	printf("make privateKey=%s, publicKey=%s\n", prvFile.c_str(), pubFile.c_str());
	Save(prvFile, prv);
	Save(pubFile, pub);
}

struct CipherWithZkp {
	Elgamal::CipherText c;
	Elgamal::Zkp zkp;
	bool verify(const Elgamal::PublicKey& pub) const
	{
		cybozu::crypto::Hash hash;
		return pub.verify(c, zkp, hash);
	}
};

inline std::ostream& operator<<(std::ostream& os, const CipherWithZkp& self)
{
	return os << self.c << std::endl << self.zkp;
}
inline std::istream& operator>>(std::istream& is, CipherWithZkp& self)
{
	return is >> self.c >> self.zkp;
}

void Vote(const std::string& voteList)
{
	Elgamal::PublicKey pub;
	Load(pub, pubFile);
	puts("shuffle");
	std::vector<size_t> idxTbl(voteList.size());
	for (size_t i = 0; i < idxTbl.size(); i++) {
		idxTbl[i] = i;
	}
	cybozu::shuffle(idxTbl, rg);
	puts("each voter votes");
	for (size_t i = 0; i < voteList.size(); i++) {
		CipherWithZkp c;
		cybozu::crypto::Hash hash;
		pub.encWithZkp(c.c, c.zkp, voteList[i] - '0', hash, rg);
		const std::string sheetName = GetSheetName(idxTbl[i]);
		printf("make %s\n", sheetName.c_str());
		Save(sheetName, c);
	}
}

void Count()
{
	Elgamal::PublicKey pub;
	Load(pub, pubFile);
	Elgamal::CipherText result;
	puts("aggregate votes");
	for (size_t i = 0; ; i++) {
		const std::string sheetName = GetSheetName(i);
		CipherWithZkp c;
		if (!Load(c, sheetName, false)) break;
		if (!c.verify(pub)) throw cybozu::Exception("bad cipher text") << i;
		printf("add %s\n", sheetName.c_str());
		result.add(c.c);
	}
	printf("create result file : %s\n", resultFile.c_str());
	Save(resultFile, result);
}

void Open()
{
	Elgamal::PrivateKey prv;
	Load(prv, prvFile);
	Elgamal::CipherText c;
	Load(c, resultFile);
	Zn n;
	prv.dec(n, c);
	std::cout << "result of vote count " << n << std::endl;
#if 0
	puts("open real value");
	for (size_t i = 0; ; i++) {
		Elgamal::CipherText c;
		const std::string sheetName = GetSheetName(i);
		if (!Load(c, sheetName, false)) break;
		Zn n;
		prv.dec(n, c);
		std::cout << sheetName << " " << n << std::endl;
	}
#endif
}

int main(int argc, char *argv[])
	try
{
	const Param p(argc, argv);
	SysInit();
	if (p.mode == "init") {
		Init();
	} else
	if (p.mode == "vote") {
		Vote(p.voteList);
	} else
	if (p.mode == "count") {
		Count();
	} else
	if (p.mode == "open") {
		Open();
	} else
	{
		printf("bad mode=%s\n", p.mode.c_str());
		return 1;
	}
} catch (std::exception& e) {
	printf("ERR %s\n", e.what());
}

