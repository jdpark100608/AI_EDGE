#include <vxworks.h>
//#include <taskLib.h>
#include <stdio.h>  // printf
#include <taskLib.h>
#include <taskLibCommon.h>
//#include <string.h> // memcpy, memset
#include <time.h>
//#include <semLib.h>
//#include "global_sched.h"
/* Overrun Test */
#define MAX_COUNT			10000
int expTimeStamp0[MAX_COUNT+1];
int oprTimeStamp0[MAX_COUNT+1];
unsigned int	curCpu0[MAX_COUNT+1];
int globalCount = 0;

#define EXPERIMENT			1

#define AFFINITY_CHECK 1
#define MATRIX_SIZE 30
void randomInitMatrix0(int mat[][MATRIX_SIZE])
{
	srand(time(0));
	int i, j;
	for (i = 0; i < MATRIX_SIZE; i++)
	{
		for(j = 0; j < MATRIX_SIZE; j++)
		{
			mat[i][j] = rand();
		}
	}
}
void calcMatrix0(int a[][MATRIX_SIZE], int b[][MATRIX_SIZE], int c[][MATRIX_SIZE])
{
	int i,j,k, temp;
	for(i = 0; i < MATRIX_SIZE; i++)
	{
		for(j = 0; j < MATRIX_SIZE; j++)
		{
			temp = 0;
			for(k = 0; k < MATRIX_SIZE; k++)
			{
				temp += (a[i][k] * b[k][j]);
			}
			c[i][j] = temp;
		}
	}
}
void testThread()
{
	printf("Created testThread!!\n");
	while(1)
	{
		taskDelay(100);

	}
}
void entryPoint_0(int coreNumber)
{
	int myId = coreNumber;
	int count = 0;
	printf("EntryPoint Called [Core : %d]\n", myId);
	unsigned int myCore0[MAX_COUNT+1];
#if AFFINITY_CHECK
		cpuset_t getAffinity;

	   if (taskCpuAffinityGet (0, &getAffinity) == OK) /*Check My Affinity Core (0 is my task)*/
	   {
	   if (CPUSET_ISZERO(getAffinity))
		   {
		   /* Task has no affinity */
			   printf("Task has no affinity [%d]\n", myId);
		   }
	   else
		   {
		   /* Task has an affinity */
			   printf("My core is [%08lx]\n",getAffinity);
		   }
	   }
	   printf("MyCoreId (%d): %08lx\n", coreNumber, vxCpuIdGet());

#endif
	   


	   ofpLoopReady(myId);
	   printf("Core %d OFP Sync Done\n", coreNumber);
	   
	   // 파일 로딩 쓰레드를 생성한다.
	   taskSpawn("testThread", 171, VX_FP_TASK, 1000, (FUNCPTR)testThread, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	while(1)
	{
		ofpLoopStart(myId);
		
#if 0
		if(count < MAX_COUNT)
		{
			setTimeStamp(&oprTimeStamp0[count]);
			
			if(count > 0)
			{
				expTimeStamp0[count-1] = getTimeStamp(&oprTimeStamp0[count-1]);
				curCpu0[count-1] = vxCpuIdGet();
				
				int mA0[MATRIX_SIZE][MATRIX_SIZE] = {0,};
				int mB0[MATRIX_SIZE][MATRIX_SIZE] = {0,};
				int mC0[MATRIX_SIZE][MATRIX_SIZE] = {0,};
				randomInitMatrix0(mA0);
				randomInitMatrix0(mB0);
				calcMatrix0(mA0, mB0, mC0);
			}
		}
#endif
		count++;
#if 0
		if(count == MAX_COUNT)
		{
			int i;
			FILE *file;
			char filename[15];
			sprintf(filename, "core#_%d.txt", coreNumber);
			file = fopen(filename, "w");
			for(i = 0; i < MAX_COUNT; i++)
			{
				fprintf(file, "count: %d time : %d\t CurCore : %x\n",i, expTimeStamp0[i], curCpu0[i]);
			}
			
			fclose(file);
			
			printf("##File Core %d Write Done\n", coreNumber);
		}
#endif
		
		ofpLoopEnd(myId);
	}
	
}
