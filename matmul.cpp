#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <omp.h>
#include <unistd.h>
#include <wait.h>
#include <fstream>
#include <stdlib.h>
#define maxsize 100 

using namespace std;
using namespace std::chrono;
int n;

random_device seeder;
mt19937 rng(seeder());
uniform_real_distribution<double> gen(0,1);

double a[maxsize][maxsize], b[maxsize][maxsize], c[maxsize][maxsize];

void add_double(int i, int j)
{
	int k = 0;
	c[i][j] = 0;
	for(k=0;k<n;++k)
		c[i][j] += a[i][k]*b[k][j];
	
}

void mul()
{
	int tid, nthreads, i, j, k, chunk;
	chunk = 10;                    /* set loop iteration chunk size */

	/*** Spawn a parallel region explicitly scoping all variables ***/
	#pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
	  {
	  tid = omp_get_thread_num();
	  if (tid == 0)
	    {
	    nthreads = omp_get_num_threads();
	  //  printf("Starting matrix multiple example with %d threads\n",nthreads);
	   // printf("Initializing matrices...\n");
	    }

	  /*** Do matrix multiply sharing iterations on outer loop ***/
	  /*** Display who does which iterations for demonstration purposes ***/
	  //printf("Thread %d starting matrix multiply...\n",tid);
	  #pragma omp for schedule (static, chunk)
	  for (i=0; i<n; i++)    
	    {
	   // printf("Thread=%d did row=%d\n",tid,i);
	    c[i][j]= 0;
	    for(j=0; j<n; j++)       
	      for (k=0; k<n; k++)
		c[i][j] += a[i][k] * b[k][j];
	    }
	  } 

	  /*** End of parallel region ***/
}
void function()
{
	int i, j;
	for(i=0;i<n;++i)
		for(j=0;j<n;++j)
		{
			add_double(i, j);
		}	
}

int main(int argc, char *argv[])
{
	int i, j;
	n = stoi(argv[1]);
	/*
		Write the results to file for plotting
	*/
	
	//	ofstream f1;
	//	f1.open("stat_mul.txt", ios::app);
	
	for(i=0;i<n;++i)
		for(j=0;j<n;++j)
		{
			a[i][j] = gen(rng);
			b[i][j] = gen(rng);
		}
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	function();
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>( t2 - t1 ).count();		
	cout << "Non-threaded = " << duration <<"\n";		

	high_resolution_clock::time_point t3 = high_resolution_clock::now();
	mul();
	high_resolution_clock::time_point t4 = high_resolution_clock::now();
	auto duration2 = duration_cast<microseconds>( t4 - t3 ).count();
	cout <<  "Threaded = " <<  duration2<<"\n";
	//	f1<<n<<" "<<duration<<" "<<duration2<<"\n";
	//	f1.close();
	return 0;
}


