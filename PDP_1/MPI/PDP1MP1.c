#include "stdio.h"
#include <math.h>
#include <stdlib.h>
#include "mpi.h"
#include <sys/time.h>
#include <stdint.h>


int* calc_count_largest(int fr,int t,int thread);
int main(int argc, char *args[])
{
	if(argc!=2)
	{
		printf("Enter P : No_of_Processors N : Problem Size in the command line\n");  
		return(0);
	}
	double start,end;
	int limit = atoi(args[1]);
	//printf ("Problem Size = %d\n",limit);
	int count=0,total_count=1;
	int total_largest=2;
	int largest=0;
	int own_count;
	int rank, size;
	int len;
	int* A;
	//char procname[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc, &args);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//MPI_Get_processor_name(procname, &len);
	MPI_Status status;
	int source,dest,i,j,k;
        int from=1;
	int to=0;
	if(size!=1)
	{
	 int no_elements_to_send=(limit/(size-1));
	// printf("size %d %d thread %d\n",size,no_elements_to_send,rank);
	}
	if(rank==0)
	{
		//printf ("Problem Size = %d\n",limit);
		//printf("Rank %d\n",rank);
		from=3;		
		start=MPI_Wtime();
		for(dest=1;dest<size;dest++)
		{
			if(dest==size-1)
			{
				to=limit;
			}
			else
			{
				if(dest==1)
				{to=(limit/(size-1));}
				else{ to=from+(limit/(size-1))-1;}
				
			}
		//	printf("sending from to %d %d to %d \n",from,to,dest);
			MPI_Send(&from, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
			MPI_Send(&to, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);

			from=to+1;			

		}
		if(size==1)
		{
			to=limit;
			A=calc_count_largest(from,to,rank);
		}
		else
		{
			A=malloc(3*sizeof(int));
			A[0]=0;A[1]=0;
		}
		if(total_largest<A[1])
		{total_largest=A[1]; }
		total_count+=A[0];
		//printf("Rank 0 initial_count %d from to %d %d largest %d\n",*(A+0),from,to,total_largest);
		for(j=1;j<size;j++)
		{
			source=j;
			MPI_Recv(A,2,MPI_INT,source,1,MPI_COMM_WORLD,&status);
		//	printf("received count at master %d received largest %d\n",A[0],A[1]);
			if(A[1]>total_largest)
			{
				total_largest=A[1];
			}
		//printf("total_largest %d\n",total_largest);
		total_count+=A[0];			
//MPI_Recv(&largest,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);
		}
	end=MPI_Wtime();
	printf("Total Count is %d Largest Final %d\nTime Taken = %f\n",total_count,total_largest,end-start);

	}
	else//other processes
	{
//printf("Thread in else %d \n",rank);	
			source=0;
			MPI_Recv(&from,1,MPI_INT,source,0,MPI_COMM_WORLD,&status);
			MPI_Recv(&to,1,MPI_INT,source,0,MPI_COMM_WORLD,&status);
			A=calc_count_largest(from,to,rank);
		//	printf("from to %d %d own_count %d thread %d largest %d\n",from,to,A[0],rank,A[1]);

			MPI_Send(A, 2, MPI_INT, 0, 1, MPI_COMM_WORLD);


	

	}
	//printf("Just before finalize\n"); 
	MPI_Finalize();
	return(0);
	
}
int* calc_count_largest(int fr,int t,int thread){
	int count=0;
	int i,j;
	int largest=0;
	//char name[MPI_MAX_PROCESSOR_NAME];
	//int num;
	//MPI_Get_processor_name(name,&num);
	//printf("name %s",name);
        if(fr%2==0)
	{
		fr=fr+1;

	}	
	for(i=fr;i<=t;i=i+2)//every odd number till the limit
	{
		int flag=0;
		for(j=3;j<=sqrt(i);j=j+2)//check if divisible by any number from 3 to 
		{
			if(i%j==0)
			{
				
				flag=1;
				break;
			}

		}
       // printf("maximum threads %d ",omp_get_num_threads());
		if(flag==0)
		{
			
			count=count+1;
	//		printf("%d %s\n",i,name);
			if(largest<i)
			{
			largest=i;
			}
		}
}
int *b;
b=malloc(sizeof(int)*3);
b[0]=count;
b[1]=largest;
return b;
}
