#include <stdio.h>
#include <sys/timeb.h>
#include <math.h>
#define MAXIDX 33554432
#define MAXSTRIDE 16384
#define NMAX 10000000

int main() 
{
	struct timeb t1, t2;
	char *data;
	int idx;
	int stride = 1; 
	double rstride = 1.0;
	int n;
	char v;
	int secs, msecs;
	double elapsed_time, mat, mat_baseline; //mat=Memory Access Time

	FILE *fp;
	fp = fopen("cache_arm.csv", "w"); data = (char*) malloc(MAXIDX);

	// create a large array of type byte
	do
	{ // measure the loop timing without the additional memory accesses...
		idx = 0;
		ftime(&t1); // get the current time into t1

		for (n = 1; n < NMAX; n++)
		{
			idx = (idx + stride) % MAXIDX; // increment idx, wrap if necessary
			idx = (idx + stride) % MAXIDX;
		}

		ftime(&t2); // get the current time into t2 
		secs = t2.time - t1.time; 
		msecs = t2.millitm - t1.millitm;
		elapsed_time = secs + msecs / 1000.0; //elapsed time in seconds 
		mat_baseline = 1e9 * elapsed_time / (2 * NMAX); // print in nanoseconds

		// measure the loop timing with the additional memory accesses... 
		idx = 0;

		ftime(&t1);

		for (n = 1; n < NMAX; n++)
		1
		{
			v = data[idx];
			idx = (idx + stride) % MAXIDX;
			data[idx] = v;
			idx = (idx + stride) % MAXIDX;
			// additional memory access
			// additional memory access
		}

		ftime(&t2);
		secs = t2.time - t1.time;
		msecs = t2.millitm - t1.millitm;
		elapsed_time = secs + msecs / 1000.0;
		mat = 1e9 * elapsed_time / (2 * NMAX); // print in nanoseconds
		
		fprintf(fp, "%d, %g\n", stride, mat - mat_baseline); printf("%d, %g\n", stride, mat - mat_baseline); 
		
		do // ensure exponential increment with no duplicate integers
		{
			rstride = rstride * pow(2, 2.1/12.1);
		} while ((int)rstride <= stride);
		
		stride = (int)rstride;

	} while(stride <= MAXSTRIDE);
	
	fclose(fp);
}
