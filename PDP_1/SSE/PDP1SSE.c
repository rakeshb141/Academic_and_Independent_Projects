#include <stdio.h>
#include <xmmintrin.h>
#include <math.h>
#include <emmintrin.h>  
#include <sys/time.h>
#include <smmintrin.h>

int main(int argc,char** argv){
	if(argc!=2)
	{
	printf("Enter N : Problem Size in the command line\n ");
	return(0);
	}
	int limit = atoi(argv[1]);
        //printf ("Problem Size = %d\n",limit);
        int count=1;
        int largest=2;
	float i_array[4];
	int size_of_j=0;
	float j_array[(int)sqrt(limit)];
	
	for(int i=0,j=3;j<=sqrt(limit)+5;i++,j+=2)
	{
		j_array[i]=j;
		size_of_j++;
	}
	while(size_of_j%4!=0)
	{
		j_array[size_of_j++]=2;
	}
	//printf("size of j %d \n",size_of_j);
	for(int i=0;i<size_of_j;i++)
	{
	//	printf("%d %f\n",i,j_array[i]); 
	}
	//printf("here\n");
	__m128 X,Y,Z;
	//__m128i I;
	float var[4];
	//float first_div[4];



struct timeval time;
gettimeofday(&time,NULL);
double t1 = time.tv_sec + (time.tv_usec / 1000000.0);

	for(int i=3;i<=limit;i+=2)
	{	
	//	printf("i %d\n",i);
		int flag=0;
		int sqrt_i=sqrt(i);
		for(int f=0;f<4;f++)
		{
			i_array[f]=i;
		}
		X = _mm_load_ps(&i_array[0]);
		//printf("here\n");
		for(int j=0;j<size_of_j-4 && j_array[j]<=sqrt_i;j=j+4)
		{	
			//if(j>sqrt(i)){break;}
			
		//	printf("j %d %f %f %f %f\n",j,j_array[j],j_array[j+1],j_array[j+2],j_array[j+3]);
			Y = _mm_load_ps(&j_array[j]);
			//printf("here inside\n");
			Z = _mm_div_ps(X,Y);
		//	_mm_store_ps(first_div,Z);
                  //      printf("Div %f %f %f %f \n",first_div[0],first_div[1],first_div[2],first_div[3]);
			Z =  _mm_floor_ps(Z);
		//	_mm_store_ps(var,Z);
		//	printf("INT %f %f %f %f \n",var[0],var[1],var[2],var[3]);
	                Z = _mm_mul_ps(Y,Z);
		//	_mm_store_ps(var,Y);
                  //      printf("Y %f %f %f %f \n",var[0],var[1],var[2],var[3]);
		//	_mm_store_ps(var,Z);
                  //      printf("MUL %f %f %f %f \n",var[0],var[1],var[2],var[3]);

                        
			Z = _mm_sub_ps(X,Z);
			_mm_store_ps(var,Z);
                  //      printf("REM %f %f %f %f \n",var[0],var[1],var[2],var[3]);
			if((var[0]==0)||(var[1]==0)||(var[2]==0)||(var[3]==0))
			{
				flag=1;
				break;	
			}

		}
		if(flag==0)
		{
	//		printf("%d is Prime\n",i);
			count++;
			largest=i; 
		}
	//	else
	//	{
	//		printf("%d is not prime\n",i);
	//	}


	
	}	
gettimeofday(&time,NULL);
double t2 = time.tv_sec + (time.tv_usec / 1000000.0);
printf("COUNT  = %d Largest = %d\n",count,largest);
printf("Time Taken %f \n",t2-t1);

//printf("end\n");
}
