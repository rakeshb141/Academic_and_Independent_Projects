#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
int main(int argc, char *args[])
{
	if(argc!=3)
	{
	printf("Enter P : No_of_Processors N : Problem Size in the command line\n");  
return(0);
	}
	double start,end;
	//omp_set_num_threads(atoi(args[1]));
	int limit = atoi(args[2]);
//	printf ("Problem Size = %d\n",limit);
	int count=1;
	int largest=2;
	int i=0;
	int j=0;
	start=omp_get_wtime();

#pragma omp parallel for private(i,j) shared(limit) reduction(+:count)
 
	for( i=3;i<=limit;i=i+2)//every odd number till the limit
	{
		int flag=0;
		int sqrt_i=sqrt(i);
		for( j=3;j<=sqrt_i;j=j+2)//check if divisible by any number from 3 to 
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
	//		printf("%d %d\n",i,omp_get_thread_num());
			#pragma omp critical
			if(largest<i)
			{
			largest=i;
			}
		}


}
end=omp_get_wtime();
printf("\nNumber of prime numbers = %d \nLargest = %d \n",count,largest);
printf("Time Taken %f \n",end-start);

}
