/*****************************************************************************

Title: Fast Fourier Transform Header
Filename: fft.h

Version: 1.0

Created: 19th October 2016
Last Modified: 19th October 2016

Language: C++ header

******************************************************************************/

#include <iostream>
#include <valarray>
#include <cmath>
#include <complex>
#include <bitset>

//using namespace std;

template<typename T>
int fft_dit(std::valarray<T> input, std::valarray<std::complex<double> >& output);
