 #define _GNU_SOURCE
 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>
 #include <linux/unistd.h>
 #include <linux/kernel.h>
 #include <linux/types.h>
 #include <sys/syscall.h>
 #include <pthread.h>
 
 #define gettid() syscall(__NR_gettid)
 #define SCHED_DEADLINE	6
 /* XXX use the proper syscall numbers */
 #ifdef __x86_64__
 #define __NR_sched_setattr		314
 #define __NR_sched_getattr		315
 #endif
 #ifdef __i386__
 #define __NR_sched_setattr		351
 #define __NR_sched_getattr		352
 #endif
 #ifdef __arm__
 #define __NR_sched_setattr		380
 #define __NR_sched_getattr		381
 #endif
 
 #define MAX_CNT	100000
 static volatile int done;
 struct sched_attr {
	__u32 size;
	__u32 sched_policy;
	__u64 sched_flags;
	/* SCHED_NORMAL, SCHED_BATCH */
	__s32 sched_nice;
	/* SCHED_FIFO, SCHED_RR */
	__u32 sched_priority;
	/* SCHED_DEADLINE (nsec) */
	__u64 sched_runtime;
	__u64 sched_deadline;
	__u64 sched_period;
 };
 int sched_setattr(pid_t pid,
		  const struct sched_attr *attr,
		  unsigned int flags)
 {
	return syscall(__NR_sched_setattr, pid, attr, flags);
 }
 int sched_getattr(pid_t pid,
		  struct sched_attr *attr,
		  unsigned int size,
		  unsigned int flags)
 {
	return syscall(__NR_sched_getattr, pid, attr, size, flags);
 }
 void *run_deadline(void *data)
 {
 	FILE *file;
	struct sched_attr attr;
	int x = 0;
	int ret;
	unsigned int flags = 0;
	printf("deadline thread started [%ld]\n", gettid());
	attr.size = sizeof(attr);
	attr.sched_flags = 0;
	attr.sched_nice = 0;
	attr.sched_priority = 0;
	/* This creates a 10ms/30ms reservation */
	attr.sched_policy = SCHED_DEADLINE;
	attr.sched_runtime = 10 * 1000 * 1000;
	attr.sched_period = attr.sched_deadline = 20 * 1000 * 1000;
	ret = sched_setattr(0, &attr, flags);
	if (ret < 0) {
		done = 0;
		perror("sched_setattr");
		exit(-1);
	}
	static struct timespec prev_tv;
	unsigned long long calc[MAX_CNT];
	
	//fopen(file, "deadline_interval.txt", "w");
	prev_tv.tv_sec = 0;
	prev_tv.tv_nsec = 0;
	int i = 0;
	
	while (!done) {
		//x++;
		sched_yield();
		// To do 
		/*
		struct timespec tv;
		clock_gettime(CLOCK_REALTIME, &tv);
		//printf("[Sec. : %ld][Nsec. : %ld]\n", tv.tv_sec, tv.tv_nsec);
		
		
		if(tv.tv_sec > prev_tv.tv_sec)
		{
			unsigned long long temp = 0;
			temp = tv.tv_nsec + 1000000000;
			calc[i] = temp - (unsigned long long)(prev_tv.tv_nsec);
		}
		else
		{
			calc[i] = (unsigned long long)(tv.tv_nsec) - (unsigned long long)(prev_tv.tv_nsec);
		}
		
		memcpy(&prev_tv, &tv, sizeof(struct timespec));
		
		if(i == MAX_CNT)
		{
			int index;
			FILE *file;
			char filename[64];
			sprintf(filename, "deadline_interval_test.txt");
			file = fopen(filename, "w");
			for(index = 0; index < MAX_CNT; index++)
			{
				fprintf(file, "CNT : %d  Interval :  %llu\n",index, calc[index]);
			}
			
			fclose(file);
			
			printf("##File Write Done\n");
			done = 1;
			
			
		}
		
		//printf("Inteval : %llu\n", calc);
		
		i++;*/
		
	}
	printf("deadline thread dies [%ld]\n", gettid());
	return NULL;
 }
 int main (int argc, char **argv)
 {
	pthread_t thread;
	printf("main thread [%ld]\n", gettid());
	//pthread_create(&thread, NULL, run_deadline, NULL); //make thread
	
	/*int pid = 0; //make process
	
	pid = fork();
	if(pid != 0)
	{
		run_deadline();
	}*/
	
	
	/*sleep(10);
	done = 1;*/

	pthread_join(thread, NULL);
	printf("main dies [%ld]\n", gettid());
	return 0;
 }
