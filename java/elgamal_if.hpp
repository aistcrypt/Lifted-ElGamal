#pragma once
#include "elgamal.hpp"

inline void SystemInit(const std::string& param) throw(std::exception)
{
	elgamal::System::init(param);
}

class CipherText {
	elgamal::CipherText self_;
	friend class PublicKey;
	friend class PrivateKey;
public:
	std::string toStr() const throw(std::exception) { return self_.toStr(); }
	std::string toString() const throw(std::exception) { return toStr(); }
	void fromStr(const std::string& str) throw(std::exception) { self_.fromStr(str); }

	void add(const CipherText& c) throw(std::exception) { self_.add(c.self_); }
	void mul(int m) throw(std::exception)
	{
		self_.mul(m);
	}
	void mul(const std::string& str) throw(std::exception)
	{
		elgamal::Zn zn(str);
		self_.mul(zn);
	}
};

class PublicKey {
	elgamal::PublicKey self_;
	friend class PrivateKey;
public:
	std::string toStr() const throw(std::exception) { return self_.toStr(); }
	std::string toString() const throw(std::exception) { return toStr(); }
	void fromStr(const std::string& str) throw(std::exception) { self_.fromStr(str); }

	void save(const std::string& fileName) const throw(std::exception)
	{
		self_.save(fileName);
	}
	void load(const std::string& fileName) throw(std::exception)
	{
		self_.load(fileName);
	}
	void enc(CipherText& c, int m) const throw(std::exception)
	{
		self_.enc(c.self_, m);
	}
	void enc(CipherText& c, const std::string& str) const throw(std::exception)
	{
		elgamal::Zn zn(str);
		self_.enc(c.self_, zn);
	}
	void rerandomize(CipherText& c) const throw(std::exception)
	{
		self_.rerandomize(c.self_);
	}
	void add(CipherText& c, int m) const throw(std::exception)
	{
		self_.add(c.self_, m);
	}
	void add(CipherText& c, const std::string& str) const throw(std::exception)
	{
		elgamal::Zn zn(str);
		self_.add(c.self_, zn);
	}
};

class PrivateKey {
	elgamal::PrivateKey self_;
public:
	std::string toStr() const throw(std::exception) { return self_.toStr(); }
	std::string toString() const throw(std::exception) { return toStr(); }
	void fromStr(const std::string& str) throw(std::exception) { self_.fromStr(str); }

	void save(const std::string& fileName) const throw(std::exception)
	{
		self_.save(fileName);
	}
	void load(const std::string& fileName) throw(std::exception)
	{
		self_.load(fileName);
	}
	void init() throw(std::exception)
	{
		self_.init();
	}
	PublicKey getPublicKey() const throw(std::exception)
	{
		PublicKey ret;
		ret.self_ = self_.getPublicKey();
		return ret;
	}
	int dec(const CipherText& c) const throw(std::exception)
	{
		return self_.dec(c.self_);
	}
	void setCache(int rangeMin, int rangeMax) throw(std::exception)
	{
		self_.setCache(rangeMin, rangeMax);
	}
	void clearCache() throw(std::exception)
	{
		self_.clearCache();
	}
};
