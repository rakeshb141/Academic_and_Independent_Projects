#include<cuda.h>
#include<stdio.h>
#include<math.h>
#include<cuda_runtime.h>
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cuda_runtime.h>
#include <sys/time.h>

__global__ void find_prime(int N,int* a,float* b,int* c)
{	
	//*p_size = s+1;
	//__shared__ cuda_count = 1;
	//__shared__ cuda_largest = 2;
	
	int i = blockIdx.x;
	//atomicAdd(&cuda_count,i);
	//__syncthreads();
	//to find a[i] is prime or not
	int flag = 0;

	for(int j=3;j<=b[i];j=j+2)
	{
		if(a[i]%j==0)
		{
			flag = 1;	
			c[i] = -1;
			break;	
		}
	}
}

int* prime_numbers;
float* sqrt_prime;
int *cuda_prime;
float *cuda_sqrt;
int *is_prime;
int *cuda_is_prime;
int main(int argc,char *args[])
{
	if(argc!=3)
	{
		printf("./GPU_Prime -t Problem_Size\n");
		return 0;
	}
	struct timeval time;
	int count = 1;
	int largest = 2;
	int problem_size = atoi(args[2]);
	printf("Problem Size %d\n",problem_size);
	int no_of_elements = 0;
	problem_size = problem_size - 2;//for 1 and 2
	if(problem_size%2 == 0)
	{
		no_of_elements = problem_size/2;
	}
	else
	{
		no_of_elements = problem_size/2 + 1;
	}
	
	prime_numbers = (int *)malloc(no_of_elements*sizeof(int));
	sqrt_prime = (float *)malloc(no_of_elements*sizeof(float));
	is_prime = (int *)malloc(no_of_elements*sizeof(int));	
	
	int h = 3;
	for(int f=0;f<no_of_elements;f++)
	{
		prime_numbers[f] = h;
		sqrt_prime[f] = sqrt(h);
		is_prime[f] = 1;
		//printf("prime[%d] = %d sqrt[%d] = %f\n",f,prime_numbers[f],f,sqrt_prime[f]);
		h = h+2; 
	}
		
	gettimeofday(&time,NULL);
	double t1 = time.tv_sec + (time.tv_usec/1000000.0);

	cudaMalloc ( (void**)&cuda_prime, no_of_elements * sizeof (int) );
        cudaMalloc ( (void**)&cuda_sqrt, no_of_elements * sizeof (float) );
        cudaMalloc ( (void**)&cuda_is_prime, no_of_elements * sizeof (int) );


	
	cudaMemcpy( cuda_prime, prime_numbers, no_of_elements * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy( cuda_sqrt, sqrt_prime, no_of_elements * sizeof(float), cudaMemcpyHostToDevice);
	//cudaMemcpy( cuda_is_prime, is_prime, no_of_elements * sizeof(int), cudaMemcpyHostToDevice);
	find_prime<<<no_of_elements,1>>>(no_of_elements,cuda_prime,cuda_sqrt,cuda_is_prime);
	
	cudaMemcpy( is_prime, cuda_is_prime , no_of_elements * sizeof(int), cudaMemcpyDeviceToHost);
	
	for(int g=0;g<no_of_elements;g++)
	{	
		//printf("is_prime[%d] = %d number %d\n",g,is_prime[g],prime_numbers[g]);
		if(is_prime[g]!=-1)
		{
			count++;
			largest = prime_numbers[g];
		}
	}
	printf("Count %d\n Largest %d\n",count,largest);
	gettimeofday(&time,NULL);
	double t2 = time.tv_sec + (time.tv_usec / 1000000.0);
	printf("Time Taken %f \n",t2-t1);
	cudaFree(cuda_prime);
	cudaFree(cuda_sqrt);
	cudaFree(cuda_is_prime);

	
		
	
}
