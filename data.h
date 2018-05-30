/*****************************************************************************

Title: Data types
Filename: data.h

Version: 1.0

Created: 25th October 2016
Last Modified: 30th October 2018

Language: C++ header

******************************************************************************/

#include <valarray>
#include <complex>

// This class is designed to contain a digital signal. The dependent
// variable is stored as an array and the independent variable is assumed
// to have a fixed step size. The type can be used to store time domain
// or frequency domain data.
//
template<typename T>
class dsignal
{
 private:
  valarray<T> data;
  double indep_start;
  double indep_end;
  double indep_step;
  
  
 public:
  dsignal(){}
  // Resize the signal array (does this make sense?) 
  int get_size(void) { return data.size(); }
  // Get the independent/dependent variable values at a point
  int get_x(int N, double& value) { value = indep_start + N*indep_step; return 0; }
  int get_y(int N, T& value) { value = data[N]; return 0; }
  // It would be good to have a description of the data stored in the type for debugging purposes.
  // int description(char * string) {}
};
