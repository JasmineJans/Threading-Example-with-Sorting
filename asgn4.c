/*
Assignment 3: CPSC 356-02, DePalma
Jasmine Jans and Jessica Peretti and Kaylin Hunter (jjans, jperetti, khunter3)
Submitted by jjans@zagmail.gonzaga.edu
10/03/2016

to compile: gcc-o asgn4 asgn4.c -lpthread
to run: ./asgn4 input output

This assignment is selection sort using p-threads. 
The program randomly creates 100 integers. It puts the integers into a file, input.
The numbers are then read from the file and put into an array. Those numbers are then
sorted by two pthreads. One sorts the first half, and the the second sorts the second half.
The third thread joins the two lists together, and places those integers into an output
file. You can read the output file, by running "more output" in the command line. 

*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

//creates a global array of 100 integers.
int array[100];


//declares all the functions
void createRandomNumberFile(FILE*);
void readFileToArray(FILE*);
void swap(int, int);
void* selectionSort(void*);
void* merge(void*);


int main(int argc, char* argv[])
{
   //declares the two files
	FILE *input, *output;
      
   //opens the file, and sets it to "write"
	input = fopen(argv[1],"w");
    
	createRandomNumberFile(input);
       
   //opens the file, and sets it to "read"
	input = fopen(argv[1],"r");

	readFileToArray(input);
 
	pthread_t tid1, tid2, tid3; 
 
   //creates flag to dictact which half of the array to sort
	int arrayHalfFlag = 1;

   //creates a pthread, and sends it to the given function
	pthread_create(&tid1, NULL, selectionSort, (void*)arrayHalfFlag);
	
   arrayHalfFlag = 0;
	pthread_create(&tid2, NULL, selectionSort, (void*)arrayHalfFlag);
	
   //joins the two threads, to ensure that they have completely run
   pthread_join(tid2,NULL); 
	pthread_join(tid1,NULL);
       
   //opens the output file, and sets it to write 
	output = fopen(argv[2], "w");
   
	pthread_create(&tid3, NULL, merge, (void*)output);
   pthread_join(tid3,NULL);  

	return 0;
} 


/*
This function creates 100 random integers,
and places them into the file, input, which is a
pass by reference parameter. It calls the srand()
function, and the rand() function to create the random integers
*/
void createRandomNumberFile(FILE* input)
{
	int i = 0;
	time_t t;
 	srand((unsigned) time(&t));
	
 	while (i != 100)
 	{
 		int random = rand()+1;
 		
 	 	fprintf(input, "%d\n", random);
 	 	i++;
 	}

 	fclose(input);
}


/*
This file take, an input file that (FILE*input)
 is a pass by reference parameter. It is then read, and the 
integers are placed into the array. 
*/
void readFileToArray(FILE* input)
{
	int i = 0;
	int k;
	int c;
	
	while((k = fscanf(input, "%d", &c)) != EOF)
	{
		array[i] = c;
		i++;
	}
	
	fclose(input);
}

/*
This function takes the current index, and switches it with
the current minimum value. It takes currI, and minI in as pass
by reference
*/
void swap(int currI, int minI)
{
	int temp = array[minI];
	array[minI] = array[currI];
	array[currI] = temp;
}

/*
Selection sort takes a void* pass by reference parameter
which signifies whether the thread should sort the first
or the second half of the array. It sorts the array using 
selection sort, and calls swap()
*/	
void* selectionSort(void* arrayHalfFlag)
{	
	if((int)arrayHalfFlag == 1)
	{	
		printf("Running: Thread 1\n");
		
		int i = 0;
		while( i < 50)
		{
			int min = array[i];
			int minIndex = i;
			for(int j = i + 1; j < 50; j++)
			{
				if(min >= array[j])
				{
					min = array[j];
					minIndex = j;
				}
			}
			swap(i, minIndex);
			i++;
		}

	}
	else
	{
		printf("Running: Thread 2\n");
		
		int k = 50;
		while( k < 100)
		{
			int min = array[k];
			int minIndex = k;
			for(int j = k + 1; j < 100; j++)
			{
				if(min >= array[j])
				{
					min = array[j];
					minIndex = j;
				}
			}
			swap(k, minIndex);
			k++;
		}
	}	
}

/*
This function takes a void* pass by reference parameter
that is the output file. This function takes the numbers from the first
half of the array, and the second half of the array, and sorts them placing
them into the output file as a fully sorted list. 
*/
void* merge(void* output)
{
	printf("Running: Thread 3\n");
	
	FILE *outPtr = output;
	int fIndex = 0;
	int sIndex = 50;
	
	while(fIndex < 50 && sIndex <100)
	{
		if(array[fIndex] <= array[sIndex])
		{
			fprintf(outPtr, "%d\n", array[fIndex]);
			fIndex++;
		}
		else
		{
			fprintf(outPtr, "%d\n", array[sIndex]);
			sIndex++;
		}
	}
	
	if(fIndex < sIndex-50)
	{
		while(fIndex < 50)
		{
			fprintf(outPtr, "%d\n", array[fIndex]);
			fIndex++;
		}
	}
	
	if(sIndex < fIndex+50)
	{
		while(sIndex < 50)
		{
			fprintf(outPtr, "%d\n", array[sIndex]);
			sIndex++;
		}
	}
	
	fclose(output);
}
