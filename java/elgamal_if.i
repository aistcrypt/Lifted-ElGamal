%module Elgamal

%include "std_string.i"
%include "std_except.i"

%{
#include "elgamal_if.hpp"
%}
%include cpointer.i
%pointer_functions(bool, p_bool);

%include "elgamal_if.hpp"
