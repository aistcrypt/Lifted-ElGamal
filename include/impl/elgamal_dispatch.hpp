/*
	Don't modify elgamal_dispatch.hpp directly.
	How to create elgamal_dispatch.hpp
	python tool/dispatch.py tool/dispatch_tmpl.txt include/impl/elgamal_dispatch.hpp
*/
#include <string>
#include <mie/mont_fp.hpp>

namespace elgamal_disp {

namespace local {

typedef elgamal_impl::local::Fp FpType0;
typedef mie::MontFpT<3> FpType3;
typedef mie::MontFpT<4> FpType4;
typedef mie::MontFpT<6> FpType6;
typedef mie::MontFpT<9> FpType9;

} // elgamal_disp::local

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4290)
#endif

struct System {
	static inline void init(const std::string& param)
	{
		std::istringstream is(param);
		std::string ecParamStr;
		std::string hashParamStr;
		if (is >> ecParamStr >> hashParamStr) {
			const mie::EcParam *ecParam = mie::getEcParam(ecParamStr);
			std::string gmpStr;
			is >> gmpStr;
			if (gmpStr != "gmp") {
				if (ecParam->bitLen <= 64 * 3) {
					elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::System::init(param);
					getFpTypeInstance() = 1;
					return;
				}
				if (ecParam->bitLen <= 64 * 4) {
					elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::System::init(param);
					getFpTypeInstance() = 2;
					return;
				}
				if (ecParam->bitLen <= 64 * 6) {
					elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::System::init(param);
					getFpTypeInstance() = 3;
					return;
				}
				if (64 * 8 < ecParam->bitLen && ecParam->bitLen <= 64 * 9) {
					elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::System::init(param);
					getFpTypeInstance() = 4;
					return;
				}
			}
		}
		elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::System::init(param);
	}
	static inline int getFpType() { return getFpTypeInstance(); }
private:
	static inline int& getFpTypeInstance()
	{
		static int fpType;
		return fpType;
	}
};

class CipherText {
	int fpType_;
	void *self_;
	friend class PublicKey;
	friend class PrivateKey;
public:
	CipherText()
		: fpType_(System::getFpType())
		, self_(0)
	{
		switch (fpType_) {
		case 0: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText(); break;
		case 1: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText(); break;
		case 2: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText(); break;
		case 3: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText(); break;
		case 4: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText(); break;
		default: throw cybozu::Exception("elgamal_disp:CipherText:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	CipherText(const CipherText& rhs)
		: fpType_(rhs.fpType_)
		, self_(0)
	{
		switch (fpType_) {
		case 0: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)rhs.self_); break;
		case 1: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)rhs.self_); break;
		case 2: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)rhs.self_); break;
		case 3: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)rhs.self_); break;
		case 4: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)rhs.self_); break;
		default: throw cybozu::Exception("elgamal_disp:CipherText:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	~CipherText()
	{
		switch (fpType_) {
		case 0: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)self_; break;
		case 1: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)self_; break;
		case 2: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)self_; break;
		case 3: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)self_; break;
		case 4: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)self_; break;
		default: throw cybozu::Exception("elgamal_disp:CipherText:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	CipherText& operator=(const CipherText& rhs)
	{
		CipherText tmp(rhs);
		swap(tmp);
		return *this;
	}
	void swap(CipherText& rhs) throw()
	{
		std::swap(fpType_, rhs.fpType_);
		std::swap(self_, rhs.self_);
	}
	std::string toStr() const throw(std::exception)
	{
		switch (fpType_) {
		case 0: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)self_)->toStr(); break;
		case 1: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)self_)->toStr(); break;
		case 2: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)self_)->toStr(); break;
		case 3: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)self_)->toStr(); break;
		case 4: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)self_)->toStr(); break;
		default: throw cybozu::Exception("elgamal_disp:CipherText:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void fromStr(const std::string& str) throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)self_)->fromStr(str); break;
		case 1: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)self_)->fromStr(str); break;
		case 2: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)self_)->fromStr(str); break;
		case 3: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)self_)->fromStr(str); break;
		case 4: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)self_)->fromStr(str); break;
		default: throw cybozu::Exception("elgamal_disp:CipherText:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	friend inline std::ostream& operator<<(std::ostream& os, const CipherText& self) throw(std::exception)
	{
		switch (self.fpType_) {
		case 0: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)(self.self_); break;
		case 1: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)(self.self_); break;
		case 2: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)(self.self_); break;
		case 3: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)(self.self_); break;
		case 4: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)(self.self_); break;
		default: throw cybozu::Exception("elgamal_disp:CipherText:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	friend inline std::istream& operator>>(std::istream& is, CipherText& self) throw(std::exception)
	{
		switch (self.fpType_) {
		case 0: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)(self.self_); break;
		case 1: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)(self.self_); break;
		case 2: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)(self.self_); break;
		case 3: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)(self.self_); break;
		case 4: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)(self.self_); break;
		default: throw cybozu::Exception("elgamal_disp:CipherText:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void add(const CipherText& c) throw(std::exception)
	{
		if (fpType_ != c.fpType_) throw cybozu::Exception("CipherText:add:bad fpType") << fpType_ << c.fpType_;
		switch (fpType_) {
		case 0: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)self_)->add(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)c.self_); break;
		case 1: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)self_)->add(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)c.self_); break;
		case 2: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)self_)->add(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)c.self_); break;
		case 3: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)self_)->add(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)c.self_); break;
		case 4: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)self_)->add(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)c.self_); break;
		default: throw cybozu::Exception("elgamal_disp:CipherText:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void mul(const elgamal::Zn& zn) throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)self_)->mul(zn); break;
		case 1: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)self_)->mul(zn); break;
		case 2: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)self_)->mul(zn); break;
		case 3: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)self_)->mul(zn); break;
		case 4: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)self_)->mul(zn); break;
		default: throw cybozu::Exception("elgamal_disp:CipherText:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void neg() throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)self_)->neg(); break;
		case 1: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)self_)->neg(); break;
		case 2: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)self_)->neg(); break;
		case 3: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)self_)->neg(); break;
		case 4: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)self_)->neg(); break;
		default: throw cybozu::Exception("elgamal_disp:CipherText:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
};

class PublicKey {
	int fpType_;
	void *self_;
	friend class PrivateKey;
public:
	PublicKey()
		: fpType_(System::getFpType())
		, self_(0)
	{
		switch (fpType_) {
		case 0: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey(); break;
		case 1: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey(); break;
		case 2: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey(); break;
		case 3: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey(); break;
		case 4: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey(); break;
		default: throw cybozu::Exception("elgamal_disp:PublicKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	PublicKey(const PublicKey& rhs)
		: fpType_(rhs.fpType_)
		, self_(0)
	{
		switch (fpType_) {
		case 0: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey*)rhs.self_); break;
		case 1: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey*)rhs.self_); break;
		case 2: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey*)rhs.self_); break;
		case 3: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey*)rhs.self_); break;
		case 4: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey*)rhs.self_); break;
		default: throw cybozu::Exception("elgamal_disp:PublicKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	~PublicKey()
	{
		switch (fpType_) {
		case 0: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey*)self_; break;
		case 1: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey*)self_; break;
		case 2: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey*)self_; break;
		case 3: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey*)self_; break;
		case 4: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey*)self_; break;
		default: throw cybozu::Exception("elgamal_disp:PublicKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	PublicKey& operator=(const PublicKey& rhs)
	{
		PublicKey tmp(rhs);
		swap(tmp);
		return *this;
	}
	void swap(PublicKey& rhs) throw()
	{
		std::swap(fpType_, rhs.fpType_);
		std::swap(self_, rhs.self_);
	}
	std::string toStr() const throw(std::exception)
	{
		switch (fpType_) {
		case 0: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey*)self_)->toStr(); break;
		case 1: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey*)self_)->toStr(); break;
		case 2: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey*)self_)->toStr(); break;
		case 3: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey*)self_)->toStr(); break;
		case 4: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey*)self_)->toStr(); break;
		default: throw cybozu::Exception("elgamal_disp:PublicKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void fromStr(const std::string& str) throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey*)self_)->fromStr(str); break;
		case 1: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey*)self_)->fromStr(str); break;
		case 2: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey*)self_)->fromStr(str); break;
		case 3: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey*)self_)->fromStr(str); break;
		case 4: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey*)self_)->fromStr(str); break;
		default: throw cybozu::Exception("elgamal_disp:PublicKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	friend inline std::ostream& operator<<(std::ostream& os, const PublicKey& self) throw(std::exception)
	{
		switch (self.fpType_) {
		case 0: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey*)(self.self_); break;
		case 1: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey*)(self.self_); break;
		case 2: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey*)(self.self_); break;
		case 3: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey*)(self.self_); break;
		case 4: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey*)(self.self_); break;
		default: throw cybozu::Exception("elgamal_disp:PublicKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	friend inline std::istream& operator>>(std::istream& is, PublicKey& self) throw(std::exception)
	{
		switch (self.fpType_) {
		case 0: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey*)(self.self_); break;
		case 1: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey*)(self.self_); break;
		case 2: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey*)(self.self_); break;
		case 3: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey*)(self.self_); break;
		case 4: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey*)(self.self_); break;
		default: throw cybozu::Exception("elgamal_disp:PublicKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void save(const std::string& pubFile) const throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey*)self_)->save(pubFile); break;
		case 1: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey*)self_)->save(pubFile); break;
		case 2: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey*)self_)->save(pubFile); break;
		case 3: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey*)self_)->save(pubFile); break;
		case 4: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey*)self_)->save(pubFile); break;
		default: throw cybozu::Exception("elgamal_disp:PublicKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void load(const std::string& pubFile) throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey*)self_)->load(pubFile); break;
		case 1: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey*)self_)->load(pubFile); break;
		case 2: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey*)self_)->load(pubFile); break;
		case 3: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey*)self_)->load(pubFile); break;
		case 4: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey*)self_)->load(pubFile); break;
		default: throw cybozu::Exception("elgamal_disp:PublicKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void add(CipherText& c, const elgamal::Zn& zn) const throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey*)self_)->add(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)c.self_), zn); break;
		case 1: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey*)self_)->add(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)c.self_), zn); break;
		case 2: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey*)self_)->add(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)c.self_), zn); break;
		case 3: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey*)self_)->add(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)c.self_), zn); break;
		case 4: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey*)self_)->add(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)c.self_), zn); break;
		default: throw cybozu::Exception("elgamal_disp:PublicKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void enc(CipherText& c, const elgamal::Zn& zn) const throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey*)self_)->enc(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)c.self_), zn); break;
		case 1: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey*)self_)->enc(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)c.self_), zn); break;
		case 2: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey*)self_)->enc(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)c.self_), zn); break;
		case 3: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey*)self_)->enc(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)c.self_), zn); break;
		case 4: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey*)self_)->enc(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)c.self_), zn); break;
		default: throw cybozu::Exception("elgamal_disp:PublicKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void rerandomize(CipherText& c) const throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey*)self_)->rerandomize(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)c.self_)); break;
		case 1: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey*)self_)->rerandomize(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)c.self_)); break;
		case 2: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey*)self_)->rerandomize(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)c.self_)); break;
		case 3: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey*)self_)->rerandomize(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)c.self_)); break;
		case 4: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey*)self_)->rerandomize(*((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)c.self_)); break;
		default: throw cybozu::Exception("elgamal_disp:PublicKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
};

class PrivateKey {
	int fpType_;
	void *self_;
public:
	PrivateKey()
		: fpType_(System::getFpType())
		, self_(0)
	{
		switch (fpType_) {
		case 0: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey(); break;
		case 1: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey(); break;
		case 2: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey(); break;
		case 3: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey(); break;
		case 4: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey(); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	PrivateKey(const PrivateKey& rhs)
		: fpType_(rhs.fpType_)
		, self_(0)
	{
		switch (fpType_) {
		case 0: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)rhs.self_); break;
		case 1: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)rhs.self_); break;
		case 2: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)rhs.self_); break;
		case 3: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)rhs.self_); break;
		case 4: self_ = (void*)new elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey(*(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)rhs.self_); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	~PrivateKey()
	{
		switch (fpType_) {
		case 0: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)self_; break;
		case 1: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)self_; break;
		case 2: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)self_; break;
		case 3: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)self_; break;
		case 4: delete (elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)self_; break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	PrivateKey& operator=(const PrivateKey& rhs)
	{
		PrivateKey tmp(rhs);
		swap(tmp);
		return *this;
	}
	void swap(PrivateKey& rhs) throw()
	{
		std::swap(fpType_, rhs.fpType_);
		std::swap(self_, rhs.self_);
	}
	std::string toStr() const throw(std::exception)
	{
		switch (fpType_) {
		case 0: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)self_)->toStr(); break;
		case 1: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)self_)->toStr(); break;
		case 2: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)self_)->toStr(); break;
		case 3: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)self_)->toStr(); break;
		case 4: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)self_)->toStr(); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void fromStr(const std::string& str) throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)self_)->fromStr(str); break;
		case 1: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)self_)->fromStr(str); break;
		case 2: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)self_)->fromStr(str); break;
		case 3: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)self_)->fromStr(str); break;
		case 4: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)self_)->fromStr(str); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	friend inline std::ostream& operator<<(std::ostream& os, const PrivateKey& self) throw(std::exception)
	{
		switch (self.fpType_) {
		case 0: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)(self.self_); break;
		case 1: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)(self.self_); break;
		case 2: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)(self.self_); break;
		case 3: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)(self.self_); break;
		case 4: return os << *(const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)(self.self_); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	friend inline std::istream& operator>>(std::istream& is, PrivateKey& self) throw(std::exception)
	{
		switch (self.fpType_) {
		case 0: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)(self.self_); break;
		case 1: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)(self.self_); break;
		case 2: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)(self.self_); break;
		case 3: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)(self.self_); break;
		case 4: return is >> *(elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)(self.self_); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void init() throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)self_)->init(); break;
		case 1: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)self_)->init(); break;
		case 2: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)self_)->init(); break;
		case 3: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)self_)->init(); break;
		case 4: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)self_)->init(); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void save(const std::string& prvFile) const throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)self_)->save(prvFile); break;
		case 1: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)self_)->save(prvFile); break;
		case 2: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)self_)->save(prvFile); break;
		case 3: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)self_)->save(prvFile); break;
		case 4: ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)self_)->save(prvFile); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void load(const std::string& prvFile) throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)self_)->load(prvFile); break;
		case 1: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)self_)->load(prvFile); break;
		case 2: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)self_)->load(prvFile); break;
		case 3: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)self_)->load(prvFile); break;
		case 4: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)self_)->load(prvFile); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void setCache(int rangeMin, int rangeMax) throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)self_)->setCache(rangeMin, rangeMax); break;
		case 1: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)self_)->setCache(rangeMin, rangeMax); break;
		case 2: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)self_)->setCache(rangeMin, rangeMax); break;
		case 3: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)self_)->setCache(rangeMin, rangeMax); break;
		case 4: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)self_)->setCache(rangeMin, rangeMax); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	void clearCache() throw(std::exception)
	{
		switch (fpType_) {
		case 0: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)self_)->clearCache(); break;
		case 1: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)self_)->clearCache(); break;
		case 2: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)self_)->clearCache(); break;
		case 3: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)self_)->clearCache(); break;
		case 4: ((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)self_)->clearCache(); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	PublicKey getPublicKey() const throw(std::exception)
	{
		PublicKey ret;
		switch (fpType_) {
		case 0: *((elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PublicKey*)ret.self_) = ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)self_)->getPublicKey(); break;
		case 1: *((elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PublicKey*)ret.self_) = ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)self_)->getPublicKey(); break;
		case 2: *((elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PublicKey*)ret.self_) = ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)self_)->getPublicKey(); break;
		case 3: *((elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PublicKey*)ret.self_) = ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)self_)->getPublicKey(); break;
		case 4: *((elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PublicKey*)ret.self_) = ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)self_)->getPublicKey(); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
		return ret;
	}
	int dec(const CipherText& c) const throw(std::exception)
	{
		switch (fpType_) {
		case 0: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)self_)->dec(*((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)c.self_)); break;
		case 1: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)self_)->dec(*((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)c.self_)); break;
		case 2: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)self_)->dec(*((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)c.self_)); break;
		case 3: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)self_)->dec(*((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)c.self_)); break;
		case 4: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)self_)->dec(*((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)c.self_)); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
	bool isZeroMessage(const CipherText& c) const throw(std::exception)
	{
		switch (fpType_) {
		case 0: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::PrivateKey*)self_)->isZeroMessage(*((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType0>::CipherText*)c.self_)); break;
		case 1: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::PrivateKey*)self_)->isZeroMessage(*((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType3>::CipherText*)c.self_)); break;
		case 2: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::PrivateKey*)self_)->isZeroMessage(*((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType4>::CipherText*)c.self_)); break;
		case 3: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)self_)->isZeroMessage(*((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)c.self_)); break;
		case 4: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)self_)->isZeroMessage(*((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)c.self_)); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
};

} // elgamal_disp
3: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::PrivateKey*)self_)->isZeroMessage(*((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType6>::CipherText*)c.self_)); break;
		case 4: return ((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::PrivateKey*)self_)->isZeroMessage(*((const elgamal_impl::ElgamalT<elgamal_disp::local::FpType9>::CipherText*)c.self_)); break;
		default: throw cybozu::Exception("elgamal_disp:PrivateKey:bad fpType_") << 9 << __FILE__ << __LINE__;
		}
	}
};

} // elgamal_disp
