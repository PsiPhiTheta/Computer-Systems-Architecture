#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <math.h>

#define MAXIDX 33554432
#define MAXSTRIDE 16384
#define NMAX 10000000


int main()
{
	int wssize = 1;
	FILE *fp;
	fp = fopen("cache_arm.csv", "w");

	do{
		struct timeb t1, t2;
		char *data;
		int idx;
		int stride = 1;
		double rstride = 1.0;
		
		int n;
		char v;
		int secs, msecs;
	
		double et, mat, mat_baseline;
	
		
		data = (char *)malloc(wssize); 		//create a large array of type byte
	
		do {
			//measure the loop timing without the additional memory accsses...
			idx = 0;
			ftime(&t1);	//get the current time into t1
	
			for (n = 1; n < NMAX; n++) 
			{
				idx = (idx + stride) % wssize; // increment idx, wrap if necessary
				idx = (idx + stride) % wssize;
			}
	
			ftime(&t2);	//get the current time into t2
			secs = t2.time - t1.time;
			msecs = t2.millitm - t1.millitm;
			et = secs + msecs / 1000.0; 	//elapsed time in seconds
			mat_baseline = 1e9 * et / (2 * NMAX); //print in nanoseconds
	
			//measure the loop timing with the additional memory accesses ...
			idx = 0;
			ftime(&t1);
	
			for (n = 1; n < NMAX; n++)
			{
				v = data[idx];	//additional memory access
				idx = (idx + stride) % wssize;
				data[idx] = v;	//additional memroy access
				idx = (idx + stride) % wssize;
			}
	
			ftime(&t2);
			secs = t2.time - t1.time;
			msecs = t2.millitm - t1.millitm;
			et = secs + msecs / 1000.0; 	//elapsed time in seconds
			mat = 1e9 * et / (2 * NMAX); //print in nanoseconds
	
			fprintf(fp, "%d, %d, %g\n",wssize, stride, mat - mat_baseline);
			printf("%d, %d, %g\n",wssize, stride, mat - mat_baseline);
			
			stride = 2*stride;
	
		} while (stride <= MAXSTRIDE);
	
		wssize = 2*wssize;

	}while(wssize <= MAXIDX); 
	
	fclose(fp);
}
