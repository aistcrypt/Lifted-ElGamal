@echo off
call set-java-path.bat
set JAVA_INCLUDE=%JAVA_DIR%\include
set SWIG=..\..\swig\swigwin-3.0.2\swig.exe
set PACKAGE_NAME=jp.go.aist.cbrc.elgamal
set PACKAGE_DIR=%PACKAGE_NAME:.=\%

echo [[run swig]]
mkdir %PACKAGE_DIR%
echo %SWIG% -java -package %PACKAGE_NAME% -outdir %PACKAGE_DIR% -c++ -Wall elgamal_if.i
%SWIG% -java -package %PACKAGE_NAME% -outdir %PACKAGE_DIR% -c++ -Wall elgamal_if.i
echo [[make dll]]
cl /MD /DNOMINMAX /LD /Ox /EHsc elgamal_if_wrap.cxx -I%JAVA_INCLUDE% -I%JAVA_INCLUDE%\win32 -I../include -I../../cybozulib/include -I../../mie/include -I../../cybozulib_ext/openssl/include -I../../cybozulib_ext/mpir/include -I../../xbyak -I../../Lifted-ElGamal/include /link /LIBPATH:../../cybozulib_ext/mpir/lib /LIBPATH:../../cybozulib_ext/openssl/lib /OUT:../bin/elgamal_if_wrap.dll

call run.bat
call run-elgamal.bat

echo [[make jar]]
%JAVA_DIR%\bin\jar cvf elgamal.jar jp
