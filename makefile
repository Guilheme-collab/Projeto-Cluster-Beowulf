all: non_parallel parallel.cxx-mpi

non_parallel: non_parallel.cpp
	g++ non_parallel.cpp -std=c++11 -L/usr/lib -lcrypto -o non_parallel

parallel.cxx-mpi: parallel.cpp
	mpicxx parallel.cpp -O2 -D_MPI -std=c++11 -L/usr/lib -lcrypto -o parallel.cxx-mpi

clean:
	rm -rf *.o *~ non_parallel parallel.cxx-mpi *.cxx-mpi

.PHONY: all clean