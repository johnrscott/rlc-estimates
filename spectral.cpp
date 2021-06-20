/*****************************************************************************

Title: Spectral Estimates
Filename: spectral.cpp

Version: 1.0

Created: 20th October 2016
Last Modified: 28th May 2018

Language: C++

Compilation: 

Dependencies: fft.cpp

Description: Contains functions which compute the auto-spectrum and cross-
spectrum of data records. 


*****************************************************************************/

#include "spectral.h"
#include "fft.h"

template<typename T>
int Sxx(std::valarray<T> input, std::valarray<std::complex<double> >& output)
/* Compute the auto-spectral density of the input */
{
  std::valarray<std::complex<double> > fft;
  
  int result = fft_dit(input, fft);
  if(result != 0)
    {
      std::cerr << "Error: FFT function did not execute correctly. Terminating program."
	   << std::endl;
      return 1;
    }
  
  /* The auto-spectral density of x[n] is given by

     Sxx[k] = |X[k]|^2

     where X[k] is the Discrete Fourier Transform of x[n].
  */

  output.resize(input.size());
  output = pow(abs(fft),2);

  return 0;
}

template<typename T>
int Sxy(std::valarray<T> x, std::valarray<T> y,
	std::valarray<std::complex<double> >& output)
/* Compute the auto-spectral density of the input */
{
  std::valarray<std::complex<double> > fft_x;
  std::valarray<std::complex<double> > fft_y;

  int result = fft_dit(x, fft_x);
  if(result != 0)
    {
      std::cerr << "Error: FFT function did not execute correctly. Terminating program."
	   << std::endl;
      return 1;
    }

  result = fft_dit(y, fft_y);
  if(result != 0)
    {
      std::cerr << "Error: FFT function did not execute correctly. Terminating program."
	   << std::endl;
      return 1;
    }
  
  /* The cross-spectral density of x[n] and y[n] is given by

     Sxy[k] = X[k]^* Y[n], 

     where ^* denotes std::complex std::conjugation and where X[k] is 
     the Discrete Fourier Transform of x[n].
  */

  // Check that x and y are th same length
  if(x.size()!=y.size())
    {
      std::cerr << "Error: x and y have different lengths. Cannot compute Sxy.";
      return 1;
    }

  output.resize(x.size());
  output = fft_x.apply(std::conj)*fft_y;

  return 0;
}

template int Sxx<std::complex<double>>
(std::valarray<std::complex<double > > input, std::valarray<std::complex<double> >& output);

template int Sxx<double>
(std::valarray<double > input, std::valarray<std::complex<double> >& output);

template int Sxy<std::complex<double>>
(std::valarray<std::complex<double>> x,
 std::valarray<std::complex<double>> y,
 std::valarray<std::complex<double>>& output);

template int Sxy<double>
(std::valarray<double > x,
 std::valarray<double > y, 
 std::valarray<std::complex<double> >& output);

