#include<stdio.h>
#include<sys/time.h>
#include<stdlib.h>
#include<string.h>
#include "rnd.c"
#include "rnd.h"
#include "mpi.h"
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
void display_(float* array,int length,int r)
{
        //printf("size = %d \n",sizeof(array));
        for(int i=0;i<length;i++)
        {
     	     printf("%f %d\n", array[i],r);
        }
	printf("\n");

}
int func(const void* a,const void* b)
{
         if(*(const float*)a < *(const float*)b)
		{	
			return -1;
		}
	return (*(const float *)a - *(const float *)b);
}

float* sort(float* array,int length,int r)
{	

	// for(int i=0;i<max_num;i++)
	// {
	int k=0;
	      for(int j=0;j<length;j++)
	       {
	            k=j;
	            while(k>0 && array[k] < array[k-1])
	        {
	                	float temp = array[k];
	                      	array[k] = array[k-1];
	                     	array[k-1] = temp;
	          		 k--;
	        }
	   }
	                                                                         //}
	//printf("length in sorting function %d %d\n",length,r);
	//qsort(array,length,sizeof(float),func);
	//printf("After Sorting \n ");
	//for(int i=0;i<length;i++)
	//{
	//	printf("   %f  %d  sorted \n",array[i] ,r);
	//}
	return array;	
}
float* input;
//float* bucket;

int main(int argc,char *args[])
{
	//printf("here\n");

	if(argc!=4)
	{
		printf("Enter No_of_Processors : P\nProblem Size : N\nWrite to output file : y/n");
		return 0;
	}
	int no_of_cores = atoi(args[1]);
	int n = atoi(args[2]);
	int max_num = 100;
	double start,end;
	int own_count;
	input = malloc(sizeof(float)*n);
	random_number_generator_normal(input,n,max_num);
	//float bucket[no_of_cores][n];
	//float* bucket = (float *) malloc(no_of_cores*n*sizeof(float));
	//printf("%d\n",sizeof(bucket));
	float* bucket;
	//bucket = malloc(no_of_cores*n*sizeof(float));
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
	//int count[no_of_cores];
	//int limit = max_num/no_of_cores;
	int size,rank,len;
	int bucket_no=0;
	 //printf("%d %s\n",sizeof(count),*args[3]);
	//memset(count,0,max_num);
	char procname[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc,&args);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);	
	MPI_Get_processor_name(procname,&len);
	MPI_Status status;
	int count[size];
	//float bucket[size][n];
	 bucket = malloc(size*n*sizeof(float));
	int source,dest;
	//float bucket[size][n];
	int limit = max_num/size;
	for(int h=0;h<size;h++)
        {
                count[h] = 0;
        }


	printf("Rank %d Limit %d\n",rank,limit);
	FILE *fp;	
	
	if(rank == 0)
	{
		start = MPI_Wtime();
		if(*args[3]=='y')
		{
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
			//display_(input,n,rank);	
		}
		 for(int i=0;i<n;i++)
        	 {
               		  // printf("i %d rank %d\n",i,rank);
                      	 bucket_no = floor(input[i]/limit);
                 //  printf("bucket_no %d \n",bucket_no);
			 *((float *)bucket+bucket_no*n+count[bucket_no])=input[i] ;
                        // ((bucket_no*n)+count[bucket_no]) = input[i];
                         count[bucket_no] += 1;
                }
                  
		//sort the 1st bucket and send the rest
		for(int i=0;i<size;i++)
		{
			//printf("Bucket %d Count %d \n",i,count[i]);
		}
		float *f = sort((float *)bucket,count[0],0);
		//float * f  = sort(bucket[0],count[0],0);
		int k=0;
		for(int g=0;g<count[0];g++)
		{
			input [k] =  *((float *)bucket+0*n+g); 
			//input[k] = bucket[0][g];
			k++;
		}
		//printf("sorted Bucket %d count %d\n",0,count[0]);
		//display_(f,count[0],rank);
		for(dest=1;dest<size;dest++)
		{
			//printf("Sending bucket %d %d\n",dest,count[dest]);
			MPI_Send(&count[dest],1,MPI_INT,dest,0,MPI_COMM_WORLD);
			MPI_Send((float*)bucket+dest*n,count[dest],MPI_FLOAT,dest,0,MPI_COMM_WORLD);   
			//MPI_Send(bucket[dest],count[dest],MPI_FLOAT,dest,0,MPI_COMM_WORLD);	
		}
		for(int j=1;j<size;j++)
		{
			source = j;
			MPI_Recv((float*)bucket+j*n,count[j],MPI_FLOAT,source,1,MPI_COMM_WORLD,&status);

			//MPI_Recv(bucket[j],count[j],MPI_FLOAT,source,1,MPI_COMM_WORLD,&status);
			//MPI_Recv()
			//printf("received from %d %d\n",source,count[j]);
			for(int f=0;f<count[j];f++)
			{
				input[k] = *((float*)bucket+j*n+f);
				//input[k] = bucket[j][f];
				k++;
			}
		}
	end = MPI_Wtime();	
        if(*args[3]=='y')
         {        
                  fprintf(fp,"%s \n","AFTER ");
                  for(int h=0;h<n;h++)
                  {      
                         fprintf(fp,"%f \n",input[h]);
                  }
                  fprintf(fp,"\n");
                  fclose(fp);
         }
         else
         {
                 printf("After \n");
                 //display_(input,n,rank);
         }
 printf("Time Taken %f \n",end-start);

	}
	else//all other processes
	{	
		source = 0;

		MPI_Recv(&own_count,1,MPI_INT,source,0,MPI_COMM_WORLD,&status);
		//printf("own_count %d \n",own_count);
		float* arr;
		arr = malloc(sizeof(float)*own_count);
		//int own_count=n;
		MPI_Recv(arr,own_count,MPI_FLOAT,source,0,MPI_COMM_WORLD,&status);	
		
		//printf("received bucket %d count %d \n",rank,own_count);
		float* r = sort(arr,own_count,rank);
		//printf("sorted bucket %d %d\n",rank,own_count);
		//display_(r,own_count,rank);
		MPI_Send(r,own_count,MPI_FLOAT,0,1,MPI_COMM_WORLD);	
		//printf("Sent back %d \n",rank); 
	}
MPI_Finalize();
return(0);

}