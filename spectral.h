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

using namespace std;

template<typename T>
int Sxx(T input, valarray<complex<double> >& output);

template<typename T>
int Sxy(T x, T y, valarray<complex<double> >& output);
