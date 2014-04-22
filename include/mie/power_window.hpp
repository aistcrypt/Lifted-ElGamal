#pragma once
/**
	@file
	@brief power window method
	Copyright (c) 2014, National Institute of Advanced Industrial
	Science and Technology All rights reserved.
	This source file is subject to BSD 3-Clause license.
*/
#include <vector>
#include <cybozu/exception.hpp>
#include <mie/power.hpp>

namespace mie {

template<class G>
class PowerWindow {
public:
	typedef TagMultiGr<G> TagG;
	typedef std::vector<G> GV;
	typedef std::vector<GV> GVV;
	size_t bitLen_;
	size_t winSize_;
	GVV tbl_;
	PowerWindow(const G& x, size_t bitLen, size_t winSize)
	{
		init(x, bitLen, winSize);
	}
	PowerWindow()
		: bitLen_(0)
		, winSize_(0)
	{
	}
	/*
		@param x [in] base index
		@param bitLen [in] exponent bit length
		@param winSize [in] window size
	*/
	void init(const G& x, size_t bitLen, size_t winSize)
	{
		bitLen_ = bitLen;
		winSize_ = winSize;
		const size_t tblNum = (bitLen + winSize) / winSize;
		const size_t r = size_t(1) << winSize;
		// alloc table
		tbl_.resize(tblNum);
		G t(x);
		for (size_t i = 0; i < tblNum; i++) {
			tbl_[i].resize(r);
			GV& w = tbl_[i];
			TagG::init(w[0]);
			for (size_t d = 1; d < r; d *= 2) {
				for (size_t j = 0; j < d; j++) {
					TagG::mul(w[j + d], w[j], t);
				}
				TagG::square(t, t);
			}
		}
	}
	template<class F>
	void power(G& z, const F& _y) const
	{
		typedef power_impl::TagInt<F> TagI;
		typedef typename TagI::BlockType BlockType;
		const G& x = tbl_[0][1];
		if (_y == 0) {
			TagG::init(z);
			return;
		}
		if (_y == 1) {
			z = x;
			return;
		}
		const bool isNegative = _y < 0;
		F y = isNegative ? -_y : _y;
		G out;
		TagG::init(out);
		const BlockType mask = (BlockType(1) << winSize_) - 1;
		size_t i = 0;
		while (y > 0) {
			BlockType v = TagI::getBlock(y, 0) & mask;
			assert(i < tbl_.size());
			if (i >= tbl_.size()) throw cybozu::Exception("mie:PowerWindow:power:bad value") << _y << i << tbl_.size();
			TagG::mul(out, out, tbl_[i][v]);
			TagI::shr(y, winSize_);
			i++;
		}
		z = out;
		if (isNegative) {
			TagG::inv(z, z);
		}
	}
};

} // mie
