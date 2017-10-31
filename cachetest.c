//0. Code information
    //Author: Thomas Hollis, John Oakley
    //Subject: CSA
    //Year: 2017
    //Version: 2.0

//1. Var info
//wssize = working set size is the amount of memory needed to compute the answer to a problem.
//mat = Memory Access Time
//stride = the stride of an array (also referred to as increment, pitch or step size) is the number of locations in memory between beginnings of successive array elements, measured in bytes or in units of the size of the array's elements.

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <math.h>

#define MAXindex 33554432
#define MAXSTRIDE 16384 // 4^7 = 2^14
#define NMAX 10000000 

int main() 
{
	struct timeb t1, t2;
	char *data;
	int index;
	int stride = 1; 
	double rstride = 1.0;
	int n;
	char v;
	int secs, msecs;
	double elapsed_time, mat, mat_baseline; 

	FILE *fp;
	fp = fopen("cache_arm.csv", "w"); 
	data = (char*) malloc(MAXindex);

	// create a large array of type byte
	do
	{ 
		// PART 1. measure the loop timing without the additional memory accesses...
		index = 0;
		ftime(&t1); // get the current time into t1

		for (n = 1; n < NMAX; n++) //increments the variable index by two 10M times
		{
			index = (index + stride) % MAXindex; // increment index, wrap if necessary
			index = (index + stride) % MAXindex; // increment index, wrap if necessary
		}

		ftime(&t2); // get the current time into t2 
		secs = t2.time - t1.time; 
		msecs = t2.millitm - t1.millitm;
		elapsed_time = secs + msecs / 1000.0; //elapsed time in seconds 
		mat_baseline = 1e9 * elapsed_time / (2 * NMAX); // writes memory access time (baseline) to increase index by 1 to variable (in nanoseconds)


		// PART 2. measure the loop timing with the additional memory accesses... 
		index = 0;
		ftime(&t1);

		for (n = 1; n < NMAX; n++)
		{
			v = data[index];
			index = (index + stride) % MAXindex;
			data[index] = v;
			index = (index + stride) % MAXindex;
		}

		ftime(&t2);
		secs = t2.time - t1.time;
		msecs = t2.millitm - t1.millitm;
		elapsed_time = secs + msecs / 1000.0; //elapsed time in seconds 
		mat = 1e9 * elapsed_time / (2 * NMAX); // writes memory access time to acess a memloc & increment index by stride to variable (in nanoseconds)
		
		fprintf(fp, "%d, %g\n", stride, mat - mat_baseline); 

		printf("%d, %g\n", stride, mat - mat_baseline); 
		
		do // ensure exponential increment of scride (with no duplicate integers - ?) until max stride
		{
			rstride = rstride * pow(2, 2.1/12.1);
		} while ((int)rstride <= stride);
		
		stride = (int)rstride;

	} while(stride <= MAXSTRIDE);
	
	fclose(fp);
}
