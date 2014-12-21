#include<cuda.h>
#include<stdio.h>
#include<math.h>
#include<cuda_runtime.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include <sys/time.h>
#include "rnd.c"
#include "rnd.h"

__global__ void sort(int n_c,float* a,int* b)
{	
	int i = blockIdx.x;
	int len_of_block = b[i];
	for(int j=0;j<len_of_block;j++)
	{
		int k=j;
		while(k>0 && *((float*)a+i*n_c+k) < *((float*)a+i*n_c+k-1))
		{
			float temp = *((float*)a+i*n_c+k);
			*((float*)a+i*n_c+k) = *((float*)a+i*n_c+k-1);
			*((float*)a+i*n_c+k-1) = temp;
			k--;
		}	
	}
	
}

 void display(float* array,int length)
  {
          //printf("size = %d \n",sizeof(array));
          for(int i=0;i<length;i++)
          {
               printf("%f \n",*((float*) array+i));
          }
          printf("\n");
  
  }


void display_(float* array,int length)
{
        //printf("size = %d \n",sizeof(array));
        for(int i=0;i<length;i++)
        {
             printf("%f ",*((float*) array+i));
        }
        printf("\n");

}

float* input;
float* bucket;
float* cuda_bucket;
int* cuda_count;
float* output;

int main(int argc,char *args[])
{
	if(argc!=3)
        {
                printf("./GPU_Prime -t Problem_Size\n");
                return 0;
        }
	int n = atoi(args[2]);
	int max_num = n*10;
	input = (float*)malloc(sizeof(float)*n);
	output = (float*)malloc(sizeof(float)*n);
	random_number_generator_normal(input,n,max_num);
	int no_of_buckets = 4;
	bucket = (float *)malloc(sizeof(float)*no_of_buckets*n);
	int* count;
   	count = (int *)malloc(sizeof(int)*no_of_buckets);
	for(int k=0;k<no_of_buckets;k++)
	{
		count[k] = 0;
	}
	printf("Before :\n");
	display(input,n);
	int bucket_no = 0;
	int limit = max_num/no_of_buckets;
	printf("limit %d\n",limit);
	printf("putting in bucket\n");
	for(int i=0;i<n;i++)
	{
		bucket_no = floor(input[i]/limit);
         	*((float *)bucket+bucket_no*n+count[bucket_no])=input[i] ;
                count[bucket_no] += 1;
		
	}
	for(int j=0;j<no_of_buckets;j++)
	{
		printf("j %d %d ",j,count[j]);
		display_(((float*)bucket+j*n),count[j]);
		
	}
	struct timeval time;
        gettimeofday(&time,NULL);
        double t1 = time.tv_sec + (time.tv_usec/1000000.0);
	
	cudaMalloc((void**)&cuda_bucket,sizeof(float)*no_of_buckets*n);
	cudaMalloc((void**)&cuda_count,sizeof(int)*no_of_buckets);
	cudaMemcpy(cuda_bucket,bucket,sizeof(float)*no_of_buckets*n,cudaMemcpyHostToDevice);
	cudaMemcpy(cuda_count,count,sizeof(int)*no_of_buckets,cudaMemcpyHostToDevice);

	sort<<<no_of_buckets,1>>>(n,cuda_bucket,cuda_count);

	cudaMemcpy(bucket,cuda_bucket,sizeof(float)*no_of_buckets*n,cudaMemcpyDeviceToHost);
	
	
	printf("Bucket After :\n");
	int o_index = 0;
        for(int j=0;j<no_of_buckets;j++)
        {
                printf("j %d %d ",j,count[j]);
                display_(((float*)bucket+j*n),count[j]);
	        for(int k=0;k<count[j];k++)
                {
                        output[o_index] = *((float*)bucket+j*n+k);
                        o_index++;
                }

        }	
	printf("After :\n");
        display(output,n);


	gettimeofday(&time,NULL);
        double t2 = time.tv_sec + (time.tv_usec/1000000.0);
        printf("Time Taken %f \n",t2-t1);
	//cudaFree(cuda_input);
	//cudaFree(cuda_answer);

}
