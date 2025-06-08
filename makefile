all: non_parallel.o parallel.cxx-mpi

non_parallel.o: non_parallel.cpp
	g++ non_parallel.cpp -o non_parallel.o -lssl -lcrypto

clean:
	rm -rf *.o *~