/*****************************************************************************

Title: File I/O
Filename: file.cpp

Version: 1.0

Created: 20th October 2016
Last Modified: 20th October 2016

Language: C++

Compilation: 

Dependencies:

Description: Contains functions for reading and writing to the disk. 

*****************************************************************************/

#include <iostream>
#include <valarray>
#include <complex>
#include <fstream>
#include <string>
#include "data.h"

int writeDataToFile(std::string filename, dsignal<double> pressure, dsignal<double> flow)
{
    // Write the FFT data to a file, space seperated
    std::ofstream file;
    //std::string fileName;

    if (pressure.size() != flow.size()) {
	std::cerr << "Error: Pressure and flow data must be same size.";
	return 1; // Error writing to file.	
    }

    // Write the data to a file
    file.open(filename);
    if(!file.is_open())
    {
	std::cerr << "Error: Failed to open file for writing.";
	return 1; // Error writing to file.
    }

    file << "pressure,flow" << std::endl;
    for(int n=0; n < pressure.size(); n++)
    {
	file << pressure[n] << "," << flow[n] << std::endl;
    }
    file.close();
  
    std::cout << std::endl;
    return 0;
}

template<typename T>
int writeFile(T fileData, std::string fileLabel)
{
  // Write the FFT data to a file, space seperated
  std::ofstream file;
  std::string fileName;
  
  // User inputs desired file names
  std::cout << "Save " << fileLabel << " as:" << std::endl;
  std::cout << "File name = ";
  std::cin >> fileName;

  // Write the data to a file
  file.open(fileName);
  if(!file.is_open())
    {
      std::cerr << "Error: Failed to open file for writing.";
      return 1; // Error writing to file.
    }
  for(int n=0; n < fileData.size(); n++)
    {
      file << fileData[n] << std::endl;
    }
  file.close();
  
  std::cout << std::endl;
  
return 0; // File operation successfully completed
}

// Explicit 

template int writeFile<std::valarray<double> >
(std::valarray<double> fileData, std::string fileLabel);

template int writeFile<std::valarray<std::complex<double> > >
(std::valarray<std::complex<double> > fileData, std::string fileLabel);

template int writeFile<dsignal<double> >
(dsignal<double> fileData, std::string fileLabel);

template int writeFile<dsignal<std::complex<double> > >
(dsignal<std::complex<double> > fileData, std::string fileLabel);
