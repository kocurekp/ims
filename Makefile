CPP=g++
CF=-Wall -Wextra -pedantic -std=c++11 -g

all: ims_main.o
	$(CPP) $(CF) -o ims_main ims_main.o -lsimlib -lm
	rm -f *.o
clean:
	rm -f ims_main

run: 
	./ims_main $(args)
