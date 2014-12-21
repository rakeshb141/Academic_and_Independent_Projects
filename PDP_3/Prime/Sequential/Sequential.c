#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
int main(int argc, char *args[])
{
	if(argc!=3)
	{
	printf("./Sequential -t Problem_Size\n");  
return(0);
	}
struct timeval time;
	int limit = atoi(args[2]);
//	printf ("Problem Size = %d\n",limit);
	int count=1;
	int largest=2;
gettimeofday(&time,NULL);
double t1 = time.tv_sec + (time.tv_usec / 1000000.0);


	for(int i=3;i<=limit;i=i+2)//every odd number till the limit
	{
		int flag=0;
		int sqrt_i=sqrt(i);
		for(int j=3;j<=sqrt_i;j=j+2)//check if divisible by any number from 3 to 
		{
			
			if(i%j==0)
			{
				
				flag=1;
				break;
			}

		}
		if(flag==0)
		{
			count++;
			largest=i;
			printf("%d ",i);
		}


	}
printf("Number of prime numbers = %d \nLargest = %d \n",count,largest);
gettimeofday(&time,NULL);
double t2 = time.tv_sec + (time.tv_usec / 1000000.0);
printf("Time Taken %f \n",t2-t1);

}
