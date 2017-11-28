#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int isprime(long long n) {
	int i, squareroot;
	if (n > 10) {
		squareroot = (int)sqrt(n);
		for (i = 3; i <= squareroot; i += 2)
			if (n % i == 0)
				return 0;
		return 1;
	}
	else
		return 0;
}

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	long long local_pc, pc;
	long long local_foundone, foundone;
	long long int n, limit;
	sscanf(argv[1],"%llu",&limit);
	if (rank == 0)
		printf("Starting. Numbers to be scanned= %lld\n", limit);
	local_pc=0;
	for (n = 11 + rank * 2; n <= limit; n += 2 * size) {
		if (isprime(n)) {
			local_pc++;
			local_foundone = n;
		}
	}
	MPI_Reduce(&local_pc, &pc, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&local_foundone, &foundone, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	if (rank == 0)
		printf("Done. Largest prime is %lld Total primes %lld\n", foundone, pc + 4);
	MPI_Finalize();
	return 0;
}
