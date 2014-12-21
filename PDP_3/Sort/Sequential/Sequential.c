#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>
#include<string.h>
#include "rnd.c"
#include "rnd.h"
void display(int row,int length,float array[][length])
{
	//printf("size = %d \n",sizeof(array));
	for(int i=0;i<row;i++)
	{	for(int j=0;j<length;j++)
		{
		printf("%f ", array[i][j]);
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

	if(argc!=4)
	{
		printf("./Sequential -t Problem_Size y/n\ny/n : write to file or not\n");
		return 0;
	}
	//int no_of_cores = atoi(args[1]);
	int n = atoi(args[2]);
	//int n = 100000000;
	int i;
	int max_num = 10*n;
	//int n = 100;
	//float* input;
	input = malloc(sizeof(float)*n);
	random_number_generator_normal(input,n,max_num);
	//float* bucket;
	bucket = malloc(sizeof(float)*n);
	int count=0;
	
	//printf("%d %s\n",sizeof(count),*args[3]);
	//memset(count,0,max_num);
	for (int l=0;l<n;l++)
	{	
		bucket[l] = 0.0;	
	}
	FILE *fp;
	if(*args[3]=='y')
	{
		//FILE *fp;
		fp = fopen("output.txt","w");
		fprintf(fp,"%s \n","BEFORE ");
		for(int h=0;h<n;h++)
		{
			fprintf(fp,"%f ",input[h]);
		}	
		fprintf(fp,"\n");	
	}
	else
	{
		


		printf("Before \n");
		//printf("size array %d %d\n",sizeof(input),sizeof(int));
		//display_(input,n); 
	}
	struct timeval time;
	gettimeofday(&time,NULL);
	double t1 = time.tv_sec + (time.tv_usec/1000000.0);
	
	//putting in the bucket
	//printf("here\n");
	for(i=0;i<n;i++)
	{	
		//printf("i %d  %d\n ",i,count[bucket_no]);
	//	int bucket_no = floor(input[i]);
		//printf("i %d  %d\n ",i,count[bucket_no]);
		bucket[count] = input[i];
		count++;
	}
	printf("Bucket \n");
	//display_(bucket,n);
	//Sorting the bucket
	printf(" count %d \n",count);
	//for(int g=0;g<max_num;g++)
	//{
	//	printf("%d ",count[g]);
	//}
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
	qsort(bucket,n,sizeof(float),func);
	printf("After bucket\n");
	//display_(bucket,n);
        //concatenate all the buckets
        for(int i=0;i<n;i++)
	{
		//for(int j=0;j<count[i];j++)
		//{
			input[i] = bucket[i];
		//}
	}
	
	gettimeofday(&time,NULL);
	double t2 = time.tv_sec + (time.tv_usec/1000000.0);
	//printf("Time Taken %f \n",t2-t1);
	if(*args[3]=='y')
	{
		 fprintf(fp,"%s \n","AFTER ");
                 for(int h=0;h<n;h++)
                 {
                        fprintf(fp,"%f ",input[h]);
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
