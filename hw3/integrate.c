#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define PI 3.1415926535

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	long long i, num_intervals;
	double local_sum;
	double rect_width, area, sum, x_middle; 
	sscanf(argv[1], "%llu", &num_intervals);
	rect_width = PI / num_intervals;
	local_sum = 0.0;
	for(i = rank + 1; i < num_intervals + 1; i += size) {
		x_middle = (i - 0.5) * rect_width;
		area = sin(x_middle) * rect_width; 
		local_sum += area;
	}
	MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) printf("The total area is: %f\n", (float)sum);
	MPI_Finalize();
	return 0;
}   
