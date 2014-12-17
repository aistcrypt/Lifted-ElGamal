
# **Lifted-ElGamal**

Lifted-ElGamal暗号ライブラリ

# 概要

このライブラリは、加法準同型性を持つlifted-ElGamal暗号方式[文献1]と、制限された平文空間に対するゼロ知識証明[文献2]を実装したソフトウェアライブラリです。

* [文献1] T. ElGamal, A Public Key Cryptosystem and a Signature Scheme Based on Discrete Logarithms, IEEE Transactions on Information Theory, Vol.31, No.4, pp.469-472, 1985, [doi:10.1109/TIT.1985.1057074](http://dx.doi.org/10.1109/TIT.1985.1057074).
* [文献2] Y. Sakai, K. Emura, G. Hanaoka, Y. Kawai, and K. Omote, Methods for Restricting Message Space in Public-Key Encryption, IEICE Transactions on Fundamentals of Electronics, Communications and Computer Sciences, Vol.E96-A, No.6, pp.1156-1168, 2013, [doi:10.1587/transfun.E96.A.1156](http://dx.doi.org/10.1587/transfun.E96.A.1156).

# 対応CPUとOS

* Intel Core i3以降の64bit Intelプロセッサ
* 64bit Windows 7 or later
* 64bit Linux
* OSX

# 確認コンパイラとツール

* Windows 7 Professional SP1 + Visual Studio 2012/2013 + swig-3.0.2 + java 1.7.0_60
* Ubuntu 14.04.1 LTS + gcc 4.8.2/clang 3.5 + java 1.7.0_65
* OSX Yosemite 10.10.1 + Apple LLVM version 6.0 (clang-600.0.56) + java 1.7.0_71-b14

# 設定

作業ディレクトリ(たとえばwork)を作りその中で次のリポジトリを取得する

    mkdir work
    cd work
    git clone git://github.com/aistcrypt/Lifted-ElGamal.git
    git clone git://github.com/herumi/xbyak.git
    git clone git://github.com/herumi/mie.git
    git clone git://github.com/herumi/cybozulib.git
    git clone git://github.com/herumi/cybozulib_ext.git

* cybozulib_extはVC(Visual C++)でOpenSSLとGMPライブラリを使うために必要
* XbyakはIntel系CPUにおいて有限体の演算の高速化のために必要
* Linuxではapt-getなどによりOpenSSLとlibgmp-devを取得すること

# ビルドの確認

* Linux or OSX:
```
  cd Lifted-ElGamal
  make -j test # for debug
  make -j test RELEASE=1 # for release
```
* javaのテスト
```
  cd java
  make -j RELEASE=1
  make -j test
```
* Windows(64bit):

    # for Visual Studio 2012
    open lifted-elgamal/elgamal.sln and build it
    # for Visual Studio 2013
    open lifted-elgamal/elgamal12.sln and build it

# アプリの実行に必要なファイルとライブラリ

Linux:

* OpenSSL
* GMP(libgmp-dev)

Windows:

* bin/libeay32.dll, bin/ssleay32.dll

# サンプルプログラム

vote_tool.mdを参照すること。

# Copyright Notice

Copyright (C) 2014, National Institute of Advanced Industrial Science and Technology (AIST)
All rights reserved.

# ライセンス

Lifted-ElGamal (files in this repository) is distributed under the [BSD 3-Clause License](http://opensource.org/licenses/BSD-3-Clause "The BSD 3-Clause License").

# 外部ライブラリのライセンス

それぞれの外部ライブラリのライセンスは次の通り

* cybozulib ; 修正BSDライセンス
* mie       ; 修正BSDライセンス
* Xbyak     ; 修正BSDライセンス
* MPIR      ; LGPL2
* OpenSSL   ; Apache License Version 1.0 + SSLeay License

これらのライブラリを用いたソフトウェアは、

* この製品はOpenSSLツールキットを利用するためにOpenSSL
プロジェクトによって開発されたソフトウェアを含む
* cybozulib, mie, Xbyakのライブラリを使っている
* MPIRのライブラリを使っている

と記述していれば、そのソフトウェアのソースコードを公開する義務はなく、
商業利用も可能である。

licenseフォルダにはオリジナルのライセンスファイルがある。

# Contact Information

* [Research Institute for Secure Systems](https://www.risec.aist.go.jp/index-en.html "RISEC"), [Computational Biology Research Center](http://www.cbrc.jp/index.html "CBRC"), National Institute of Advanced Industrial Science and Technology. Tel: +81 29-861-5284

# history

2014/April/23 ; initial version
2014/Dec/18 ; support wrapper api and java
