#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <mie/elgamal.hpp>
#include <mie/ecparam.hpp>
#include <mie/fp.hpp>
#include <mie/ec.hpp>
#include <mie/gmp_util.hpp>
#include <cybozu/random_generator.hpp>
#include <cybozu/crypto.hpp>

#if defined(_WIN64) || defined(__x86_64__)
	#define ELGAMAL_OPT_FP
#endif

namespace elgamal {

namespace local {
struct TagZn;
} // elgamal::local

typedef mie::FpT<mie::Gmp, local::TagZn> Zn;

} // elgamal

namespace elgamal_impl {

namespace local {

template<class T>
void saveOne(std::ostream& os, const std::string& name, const T& x)
{
	if (!(os << x)) throw cybozu::Exception("elgamal:saveOne") << name;
}

template<class T>
void loadOne(std::istream& is, const std::string& name, T& x)
{
	if (!(is >> x)) throw cybozu::Exception("elgamal:loadOne") << name;
}

struct TagFp;
typedef mie::FpT<mie::Gmp, TagFp> Fp;

} // local

template<class _Fp>
struct ElgamalT {
	typedef _Fp Fp;
	typedef mie::EcT<Fp> Ec;
	typedef elgamal::Zn Zn;
	typedef mie::ElgamalT<Ec, Zn> Elgamal;

	struct System {
		static const mie::EcParam *ecParam;
		static cybozu::RandomGenerator rg;
		static cybozu::crypto::Hash::Name hashName;
		/*
			init system
			@param param [in] string such as "ecParamName hashName"
			@note NOT thread safe because setting global parameters of elliptic curve
			ex1) "secp192k1 sha256" // 192bit security + sha256
			ex2) "secp160k1 sha1" // 160bit security + sha1
			hashName : sha1 sha224 sha256 sha384 sha512
		*/
		static inline void init(const std::string& param)
		{
			std::istringstream iss(param);
			std::string ecParamStr;
			std::string hashNameStr;
			if (iss >> ecParamStr >> hashNameStr) {
				ecParam = mie::getEcParam(ecParamStr);
				Zn::setModulo(ecParam->n);
				Fp::setModulo(ecParam->p);
				Ec::setParam(ecParam->a, ecParam->b);
				hashName = cybozu::crypto::Hash::getName(hashNameStr);
				return;
			}
			throw cybozu::Exception("elgamal:System:init:bad param") << param;
		}
		static inline void save(std::ostream& os)
		{
			os << ecParam->name << ' ' << cybozu::crypto::Hash::getName(hashName) << std::endl;
		}
		static inline void load(std::istream& is)
		{
			std::string line;
			if (!std::getline(is, line)) throw cybozu::Exception("elgamal:System:init:load:cant' load line");
			std::istringstream iss(line);
			std::string ecParamStr;
			std::string hashNameStr;
			iss >> ecParamStr >> hashNameStr;
			if (ecParamStr != ecParam->name) {
				throw cybozu::Exception("elgamal::System:init:load:bad ecParam") << ecParamStr << ecParam->name;
			}
			const std::string curHashName = cybozu::crypto::Hash::getName(hashName);
			if (hashNameStr != curHashName) {
				throw cybozu::Exception("elgamal::System:init:load:bad hashName %s %s") << hashNameStr << curHashName;
			}
		}
	};

	struct CipherText : Elgamal::CipherText {
	};
	struct Zkp : Elgamal::Zkp {
	};

	struct PublicKey : Elgamal::PublicKey {
		void save(const std::string& fileName) const
		{
			std::ofstream ofs(fileName.c_str(), std::ios::binary);
			System::save(ofs);
			local::saveOne(ofs, "publicKey", *this);
		}
		void load(const std::string& fileName)
		{
			std::ifstream ifs(fileName.c_str(), std::ios::binary);
			System::load(ifs);
			local::loadOne(ifs, "publicKey", *this);
		}
		void enc(CipherText& c, const Zn& m) const
		{
			Elgamal::PublicKey::enc(c, m, System::rg);
		}
		void enc(CipherText&c, int m) const
		{
			Elgamal::PublicKey::enc(c, Zn(m), System::rg);
		}
		void encWithZkp(CipherText&c, Zkp& zkp, int m) const
		{
			cybozu::crypto::Hash hash(System::hashName);
			Elgamal::PublicKey::encWithZkp(c, zkp, m, hash, System::rg);
		}
		bool verify(const CipherText& c, const Zkp& zkp) const
		{
			cybozu::crypto::Hash hash(System::hashName);
			return Elgamal::PublicKey::verify(c, zkp, hash);
		}
		void rerandomize(CipherText& c) const
		{
			Elgamal::PublicKey::rerandomize(c, System::rg);
		}
	};

	struct PrivateKey : Elgamal::PrivateKey {
		void init()
		{
			const mie::EcParam *ecParam = System::ecParam;
			const Fp x0(ecParam->gx);
			const Fp y0(ecParam->gy);
			Ec P(x0, y0);
			// Zn::getModBitLen() may be greater than ecParam->bitLen
			((typename Elgamal::PrivateKey*)this)->init(P, Zn::getModBitLen(), System::rg);
		}
		void save(const std::string& fileName) const
		{
			std::ofstream ofs(fileName.c_str(), std::ios::binary);
			System::save(ofs);
			local::saveOne(ofs, "privateKey", *this);
		}
		void load(const std::string& fileName)
		{
			std::ifstream ifs(fileName.c_str(), std::ios::binary);
			System::load(ifs);
			local::loadOne(ifs, "privateKey", *this);
		}
		const PublicKey& getPublicKey() const
		{
			const typename Elgamal::PublicKey& pub = Elgamal::PrivateKey::getPublicKey();
			return *reinterpret_cast<const PublicKey*>(&pub);
		}
	};
};

template<class Fp> const mie::EcParam* ElgamalT<Fp>::System::ecParam;
template<class Fp> cybozu::RandomGenerator ElgamalT<Fp>::System::rg;
template<class Fp> cybozu::crypto::Hash::Name ElgamalT<Fp>::System::hashName;


} // elgamal_impl

#ifdef ELGAMAL_OPT_FP
#include <impl/elgamal_dispatch.hpp>
namespace elgamal {

typedef elgamal_disp::System System;
typedef elgamal_disp::CipherText CipherText;
typedef elgamal_disp::PublicKey PublicKey;
typedef elgamal_disp::PrivateKey PrivateKey;
typedef elgamal_disp::Zkp Zkp;

} // elgamal
#else

namespace elgamal {

typedef elgamal_impl::ElgamalT<elgamal_impl::local::Fp> Elgamal;

typedef Elgamal::System System;
typedef Elgamal::CipherText CipherText;
typedef Elgamal::PublicKey PublicKey;
typedef Elgamal::PrivateKey PrivateKey;
typedef Elgamal::Zkp Zkp;

} // elgamal
#endif
