CC = g++
CFLAGS = -std=c++11 -fmax-errors=3 -c
LFLAGS = -std=c++11

rlc-estimate: rlc-estimate.o estimation.o simulation.o file.o spectral.o fft.o 
	$(CC) $(LFLAGS) -o rlc-estimate rlc-estimate.o estimation.o simulation.o file.o spectral.o fft.o
	rm -f *.o *~

rlc-estimate.o: rlc-estimate.cpp data.h estimation.h simulation.h
	$(CC) $(CFLAGS) rlc-estimate.cpp

simulation.o: simulation.cpp simulation.h data.h
	$(CC) $(CFLAGS) simulation.cpp

estimation.o: estimation.cpp estimation.h data.h
	$(CC) $(CFLAGS) estimation.cpp

file.o: file.cpp file.h data.h
	$(CC) $(CFLAGS) file.cpp

spectral.o: spectral.cpp fft.o spectral.h
	$(CC) $(CFLAGS) spectral.cpp

fft.o: fft.cpp fft.h
	$(CC) $(CFLAGS) fft.cpp

clean:
	rm -f rlc-estimate.o estimation.o simulation.o file.o spectral.o fft.o
