#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int isprime(unsigned long long n) {
	unsigned long long int i, squareroot;
	if (n > 10) {
		squareroot = (unsigned long long int)sqrt(n);
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
	unsigned long long int local_pc, pc;
	unsigned long long int local_foundone, foundone;
	unsigned long long int n, limit;
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
	MPI_Reduce(&local_foundone, &foundone, 1, MPI_UNSIGNED_LONG_LONG, MPI_MAX, 0, MPI_COMM_WORLD);
	if (rank == 0)
		printf("Done. Largest prime is %llu Total primes %llu\n", foundone, pc + 4);
	MPI_Finalize();
	return 0;
}
