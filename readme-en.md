# **Lifted-ElGamal**

C++ Library for the Lifted-ElGamal Cryptosystem

# Summary
This library provides for the implementation of the lifted-ElGamal cryptosystem with additively homomorphic properties [1] and the non-interactive zero knowledge proof for restricting message space [2].

* [1] T. ElGamal, A Public Key Cryptosystem and a Signature Scheme Based on Discrete Logarithms, IEEE Transactions on Information Theory, Vol.31, No.4, pp.469-472, 1985, [doi:10.1109/TIT.1985.1057074](http://dx.doi.org/10.1109/TIT.1985.1057074).
* [2] Y. Sakai, K. Emura, G. Hanaoka, Y. Kawai, and K. Omote, Methods for Restricting Message Space in Public-Key Encryption, IEICE Transactions on Fundamentals of Electronics, Communications and Computer Sciences, Vol.E96-A, No.6, pp.1156-1168, 2013, [doi:10.1587/transfun.E96.A.1156](http://dx.doi.org/10.1587/transfun.E96.A.1156).

# List of Supported CPUs and Operating Systems

* Intel 64-bit Core 2 Duo or newer Intel 64-bit CPUs
* 64-bit Windows 7 or newer
* 64-bit Linux (tested on ubuntu 13.10)
* Windows 7 + Cygwin 6.1 (32-bit) or newer

# List of Supported Compilers and Prerequisite Tools
* Visual Studio 2012 + MPIR 2.6.0 on Windows 7
* gcc 4.8.1 + OpenSSL 1.0.1e + GMP 5.1.2
* clang++ 3.0 or newer
* gcc 4.5.3 on cygwin or newer

# Installation Requirements

Create a working directory (e.g., work) and clone the following repositories.

       mkdir work
       cd work
       git clone git://github.com/aistcrypt/Lifted-ElGamal.git
       git clone git://github.com/herumi/xbyak.git
       git clone git://github.com/herumi/mie.git
       git clone git://github.com/herumi/cybozulib.git
       git clone git://github.com/herumi/cybozulib_ext.git

* Cybozulib_ext is a prerequisite for running OpenSSL and GMP on VC (Visual C++).
* Xbyak is a prerequisite for optimizing the operations in the finite field on Intel CPUs.
* OpenSSL and libgmp-dev are available via apt-get (or other similar commands) if using Linux.

# Installation
Linux (64-bit) or Cygin (32-bit):

      cd lifted-elgamal
      make -j # for debug
      make -j RELEASE=1 # for release
      make -j RELEASE=1 USE_TCMALLOC=1

* use tcmalloc (optimal) for Linux; sudo apt-get install libgoogle-perftools-dev

Windows (64-bit):

      # for Visual Studio 2012
      open lifted-elgamal/elgamal.sln and build it
      # for Visual Studio 2013
      open lifted-elgamal/elgamal12.sln and build it

Linux (32-bit):

      make -j BIT=32 # for debug
      make -j BIT=32 RELEASE=1 # for release

# Prerequisite Files and Libraries for Running Your Application

Linux:
	* OpenSSL
	* GMP (libgmp-dev)

Windows:
	* bin/libeay32.dll, bin/ssleay32.dll

# Sample code
See vote_tool-en.md

# Copyright Notice

Copyright (C) 2014, National Institute of Advanced Industrial Science and Technology (AIST)
All rights reserved.

# License

Lifted-ElGamal (files in this repository) is distributed under the [BSD 3-Clause License] (http://opensource.org/licenses/BSD-3-Clause "The BSD 3-Clause License").

# Licenses of External Libraries

Licenses of external libraries are listed as follows.

* cybozulib: BSD-3-Clause
* mie: BSD-3-Clause
* Xbyak: BSD-3-Clause
* MPIR: LGPL2
* OpenSSL: Apache License Version 1.0 + SSLeay License

Software including any of those libraries is allowed to be used for commercial purposes without releasing its source code as long as the regarding copyright statements described as follows are included in the software.

* This product includes software that was developed by an OpenSSL project in order to use OpenSSL toolkit.
* This product includes cybozulib, mie, and Xbyak.
* This product includes MPIR.

The licenses of those libraries are stored in the folder named 'license'

# Contact Information

* [Research Institute for Secure Systems](https://www.risec.aist.go.jp/index-en.html "RISEC"), [Computational Biology Research Center](http://www.cbrc.jp/index.html "CBRC"), National Institute of Advanced Industrial Science and Technology. Tel: +81 29-861-5284

# History

2014/April/23; initial version
