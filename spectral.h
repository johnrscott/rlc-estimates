/*****************************************************************************

Title: Spectral Estimates Header File
Filename: spectral.h

Version: 1.0

Created: 20th October 2016
Last Modified: 28th May 2018

Language: C++ header

******************************************************************************/

#include <valarray>
#include <complex>

template<typename T>
int Sxx(std::valarray<T> input, std::valarray<std::complex<double> >& output);

template<typename T>
int Sxy(std::valarray<T> x, std::valarray<T> y,
	std::valarray<std::complex<double> >& output);
