#include "stdio.h"
#include <math.h>
#include <stdlib.h>
//#include "mpi.h"
#include <sys/time.h>
#include <stdint.h>
#include<cuda.h>
#include<math.h>
#include<cuda_runtime.h>
#include<string.h>


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


extern "C" int* invoke_cuda_find_prime(int from,int to,int r)
{	
	printf("from %d to %d\n",from,to);
	int n = to-from+1;
	if(from ==0)
	{
		from = 3;
		n = to - from+1;
	}
	if(from%2==0)
	{
		from = from+1;
	}
	int* prime_numbers;
	float* sqrt_;
	int* is_prime;
	int index =0;
	int size_of_prime_number=0;
	prime_numbers = (int*)malloc(sizeof(int)*n);
	sqrt_ = (float*)malloc(sizeof(float)*n);
	is_prime = (int*)malloc(sizeof(int)*n);
		
	int* cuda_prime;
	float* cuda_sqrt;
	int*cuda_is_prime;
	int sa=0;
	printf("from %d to %d rank %d\n",from,to,r);

	for(sa=from;sa<=to;sa++)
	{
		//printf("s %d\n",sa,r);
		if(sa%2!=0)
		{
			prime_numbers[index] = sa;
			sqrt_[index] = sqrt(sa);
			is_prime[index] = -1;
			printf("s %d prime[%d] %d sqrt %f rank %d \n",sa,index,prime_numbers[index],sqrt(sa),r);
			index++;
			size_of_prime_number++;
		}
	}
	
	cudaMalloc ( (void**)&cuda_prime, size_of_prime_number * sizeof (int) );
        cudaMalloc ( (void**)&cuda_sqrt, size_of_prime_number * sizeof (float) );
        cudaMalloc ( (void**)&cuda_is_prime, size_of_prime_number * sizeof (int) );
	
	cudaMemcpy( cuda_prime, prime_numbers, size_of_prime_number * sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy( cuda_sqrt, sqrt_, size_of_prime_number * sizeof(float), cudaMemcpyHostToDevice);
        
        find_prime<<<size_of_prime_number,1>>>(size_of_prime_number,cuda_prime,cuda_sqrt,cuda_is_prime);

        cudaMemcpy( is_prime, cuda_is_prime , size_of_prime_number * sizeof(int), cudaMemcpyDeviceToHost);
	int largest = 0;
	int count = 0;
	for(int g=0;g<size_of_prime_number;g++)
        {
                printf("is_prime[%d] = %d number %d\n",g,is_prime[g],prime_numbers[g]);
                if(is_prime[g]!=-1)
                {	printf("Prime Number %d\n",prime_numbers[g]);
                        count++;
                        largest = prime_numbers[g];
                }
        }
	int* b;
	b = (int*)malloc(sizeof(int)*3);
	b[0] = count;
	b[1] = largest;
        printf("Count %d\n Largest %d\n",count,largest);
        cudaFree(cuda_prime);
        cudaFree(cuda_sqrt);
	cudaFree(cuda_is_prime);
	return b;


}

