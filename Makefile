CC = g++
CFLAGS = -std=c++11 -c
LFLAGS = -std=c++11

rlc-estimate: rlc-estimate.o 
	$(CC) $(LFLAGS) -o rlc-estimate rlc-estimate.o estimation.o simulation.o file.o spectral.o fft.o
	rm -f *.o *~

rlc-estimate.o: rlc-estimate.cpp estimation.o simulation.o
	$(CC) $(CFLAGS) rlc-estimate.cpp simulation.o estimation.o

simulation.o: simulation.cpp simulation.h data.h file.o spectral.o
	$(CC) $(CFLAGS) simulation.cpp file.o spectral.o

estimation.o: estimation.cpp estimation.h file.o spectral.o
	$(CC) $(CFLAGS) estimation.cpp file.o spectral.o

file.o: file.cpp file.h
	$(CC) $(CFLAGS) file.cpp

spectral.o: spectral.cpp fft.o spectral.h
	$(CC) $(CFLAGS) spectral.cpp

fft.o: fft.cpp fft.h
	$(CC) $(CFLAGS) fft.cpp

clean:
	rm -f rlc-estimate.o estimation.o simulation.o file.o spectral.o fft.o
