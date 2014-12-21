#include "stdio.h"
#include <math.h>
#include <stdlib.h>
#include "mpi.h"
#include <sys/time.h>
#include <stdint.h>
//#include<cuda.h>
#include<math.h>
//#include<cuda_runtime.h>
#include<string.h>

int* prime_numbers;
float* sqrt_prime;
int* is_prime;
int main(int argc,char *args[])
{
	if(argc!=3)
        {
                printf("./GPU_Prime -t Problem_Size\n");
                return 0;
        }
	double start,end;
	struct timeval time;
        int no_of_elements = 0;
	int count = 1;
        int largest = 2;
	int own_count;
	int rank,size;
	int source,dest,i,j,k;
	int to=0;
	int from = 0;
        int problem_size = atoi(args[2]);

	int* A;

        printf("Problem Size %d\n",problem_size);	
	int h = 3;
	int f=0;
 
	MPI_Init(&argc, &args);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        //MPI_Get_processor_name(procname, &len);
        MPI_Status status;
	if(rank == 0)
	{
		from = 0;
		start = MPI_Wtime();
		for(dest=1;dest<size;dest++)
		{
			if(dest==size-1)
			{
				to = problem_size;
			}
			else
			{
				to = from + (problem_size/(size-1));	
			}
			printf("Sending from %d to %d to %d\n",from,to,dest);
         		MPI_Send(&from, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
                        MPI_Send(&to, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);

			from = to+1;
		}
		int total_count = 1;
		int total_largest = 2;
		A = (int*)malloc(3*sizeof(int));

		for(j=1;j<size;j++)
		{
			source = j;
			 printf("Receiving from rank %d\n",source);
	
			MPI_Recv(A,2,MPI_INT,source,1,MPI_COMM_WORLD,&status);
			printf("received count %d largest %d",A[0],A[1]);
			//MPI_Recv(&largest,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);
			total_count = total_count + A[0];
			printf("largest %d total_largest %d\n",A[1],total_largest);
			if(total_largest < A[1])
			{
				total_largest = A[1];
			}
		}
	end = MPI_Wtime();
	printf("Total Count is %d Largest Final %d\nTime Taken = %f\n",total_count,total_largest,end-start);

	}
        else
	{
		source =0;
		MPI_Recv(&from,1,MPI_INT,source,0,MPI_COMM_WORLD,&status);
                MPI_Recv(&to,1,MPI_INT,source,0,MPI_COMM_WORLD,&status);
		A = (int*)malloc(3*sizeof(int));

		//cuda stuff
		
		A = invoke_cuda_find_prime(from,to,rank);
		 printf("received in else count %d largest %d",A[0],A[1]);

		printf("Rank %d from %d to %d\n",rank,from,to);
		//own_count = from;
		MPI_Send(A,2,MPI_INT,0,1,MPI_COMM_WORLD);
	}
	printf("done rank %d\n",rank);
	MPI_Finalize();
	return(0);
                                        
	
}
