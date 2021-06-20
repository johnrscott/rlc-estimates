/*****************************************************************************

Title: File I/O Header File
Filename: file.h

Version: 1.0

Created: 20th October 2016
Last Modified: 20th October 2016

Language: C++ header

******************************************************************************/


#include <iostream>
#include <stdlib.h>
#include <valarray>
#include <time.h>
//#include <cmath>
#include <complex>
#include <random>
#include <fstream>
#include "data.h"

//using namespace std;

template<typename T>
void print(const std::valarray<T> & vec) {
    for (std::size_t k = 0; k < vec.size(); k++) {
	std::cout << vec[k] << std::endl;
    }
}

int writeDataToFile(std::string filename, dsignal<double> pressure, dsignal<double> flow);

template<typename T>
int writeFile(T fileData, std::string fileLabel);

