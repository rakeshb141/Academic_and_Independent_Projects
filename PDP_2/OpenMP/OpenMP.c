#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>
#include<string.h>
#include "rnd.c"
#include "rnd.h"
#include<omp.h>


void display(int row,int length,float* array)
{
        //printf("size = %d \n",sizeof(array));
                for(int i=0;i<row;i++)
               {    
		   for(int j=0;j<length;j++)
                   {
                         printf("%f ", *((float*)array+i*length+j));
                   }
           printf("\n");
                }
}
        



void display_(float* array,int length)
{
        //printf("size = %d \n",sizeof(array));
        for(int i=0;i<length;i++)
        {
     	     printf("%f \n", array[i]);
        }
	printf("\n");

}
int func(const void* a,const void* b)
{
	return (*(float *)a - *(float *)b);
}
float* input;
float* bucket;

int main(int argc,char *args[])
{
	//printf("here\n");

	if(argc!=5)
	{
		printf("Enter No_of_Processors : P\n -t Problem Size : N\nWrite to output file : y/n");
		printf("\nExample ./OpenMP P -t N y\n");
		return 0;
	}
	int no_of_cores = atoi(args[1]);
	int n = atoi(args[3]);
	//int n = 100000000;
	int i;
	int max_num = 100;
	double start,end;
	//float input[n];
	input = malloc(sizeof(float)*n);
	random_number_generator_normal(input,n,max_num);
	//float bucket[no_of_cores][n];
	//float* bucket = (float *) malloc(no_of_cores*n*sizeof(float));
	//float* bucket;
	//printf("%d\n",sizeof(bucket));
	bucket = malloc(no_of_cores*n*sizeof(float));
	//bucket = malloc(no_of_cores*sizeof(float*));
	//for(int j=0;j<no_of_cores;j++)
	{	
	//	bucket[j] = malloc(sizeof(float)*n);
	}
	//float** bucket = (float**) malloc(no_of_cores*sizeof(*float));
	//for(int i=0;i<no_of_cores;i++)
	{
	//	bucket[i] = malloc(n*sizeof(float));
	}	
	int count[no_of_cores];
	int limit = max_num/no_of_cores;
	
	
	//printf("%d %s\n",sizeof(count),*args[3]);
	//memset(count,0,max_num);
	for (int l=0;l<no_of_cores;l++)
	{	
		for(int k=0;k<n;k++)
		{
			*((float *)bucket+l*n+k) = 0.0;
			//bucket[l][k] = 0.0;
		}	
	count[l]=0;
	}
	FILE *fp;
	if(*args[4]=='y')
	{
		//FILE *fp;
		fp = fopen("output.txt","w");
		fprintf(fp,"%s \n","BEFORE ");
		for(int h=0;h<n;h++)
		{
			fprintf(fp,"%f \n",input[h]);
		}	
		fprintf(fp,"\n");	
	}
	else
	{
	

		printf("Before \n");
		//printf("size array %d %d\n",sizeof(input),sizeof(int));
	//	display_(input,n); 
	}
	struct timeval time;
	gettimeofday(&time,NULL);
	double t1 = time.tv_sec + (time.tv_usec/1000000.0);
	
	//putting in the bucket
	//printf("here\n");
	int bucket_no = 0;
	omp_set_num_threads(no_of_cores);
	//printf("limit %d \n",limit);
	#pragma omp parallel for private(i) shared(n,count)
	for(int i=0;i<n;i++)
	{	
		//printf("i %d  %d\n ",i,count[bucket_no]);
	//int bucket_no = floor(input[i]);
		//int count_temp[no_of_cores];
	//	printf("i %d %d\n ",i,omp_get_thread_num());
		bucket_no = floor(input[i]/limit);
		//printf("Input data %f in Bucket %d\n",input[i],bucket_no);
	//	printf("bucket_no %d \n" ,bucket_no);
		//*((float *)bucket+bucket_no*n+count[bucket_no]) = input[i];
		int tem;

	//	bucket[bucket_no][count[bucket_no]] = input[i];
		//count_temp[bucket_no] = count_temp[bucket_no] + 1;
		#pragma omp critical
		{	tem = count[bucket_no];
			count[bucket_no] += 1;
		}	
		 *((float *)bucket+bucket_no*n+tem) = input[i];

	}
	printf("Bucket \n");
	//display(no_of_cores,n,bucket);
	//Sorting the bucket
	printf(" count \n");
	for(int g=0;g<no_of_cores;g++)
	{
	//	printf("%d ",count[g]);
	}
	printf("\n");
	int k;
	//for(int i=0;i<max_num;i++)
	//{
	//	for(int j=0;j<count[i];j++)
	//	{
	//		k=j;
	//		while(k>0 && bucket[i][k] < bucket[i][k-1])
	//		{
	//			float temp = bucket[i][k];
	//			bucket[i][k] = bucket [i][k-1];
	//			bucket[i][k-1] = temp;
	//			k--;
	//			
	//		}


	//	}
	//}
	#pragma omp parallel for private(i) shared(no_of_cores)
	for(int i=0;i<no_of_cores;i++)
	{	
		//printf(" thread num %d\n",omp_get_thread_num());
		int k=0;
              for(int j=0;j<count[i];j++)
               {
                    k=j;
			//printf("k %d\n",k);
                    while(k>0 && *((float *)bucket+i*n+k)  < *((float *)bucket+i*n+k-1))                 		{
			float temp = *((float *)bucket+i*n+k);
			*((float *)bucket+i*n+k) =*((float *)bucket+i*n+k-1) ;
 	
			*((float *)bucket+i*n+k-1) = temp;
			k--;

			
                                //float temp = bucket[i][k];
                                //bucket[i][k] = bucket[i][k-1];
                                //bucket[i][k-1] = temp;
                                // k--;
                	}
           }

	}

	printf("After bucket\n");
	//display(no_of_cores,n,bucket);
        //concatenate all the buckets
        k=0;
	int no_to_add =0;
	#pragma omp parallel for shared(no_of_cores) private(i,no_to_add)
        for(int i=0;i<no_of_cores;i++)
	{	
		no_to_add = 0;
	//	printf("i %d thread Num %d \n",i ,omp_get_thread_num());
		int d=i;
		while(d>0)
		{
			no_to_add += count[d-1];
			d--;
		}
	//	printf("Thread_num %d no_to_add %d \n",omp_get_thread_num(),no_to_add);
		for(int j=0;j<count[i];j++)
		{	
			//printf("Putting back index %d %f\n",(k+no_to_add),bucket[i][j]);
			input[j+no_to_add] =*((float *)bucket+i*n+j);
			k++;
		}
	}
	
	gettimeofday(&time,NULL);
	double t2 = time.tv_sec + (time.tv_usec/1000000.0);
	//printf("Time Taken %f \n",t2-t1);
	if(*args[4]=='y')
	{
		 fprintf(fp,"%s \n","AFTER ");
                 for(int h=0;h<n;h++)
                 {
                        fprintf(fp,"%f \n",input[h]);
                 }               
                 fprintf(fp,"\n");
		fclose(fp);
	}
	else{
	printf("After \n");
        //display_(input,n);
	}
printf("Time Taken %f \n",t2-t1);


}
