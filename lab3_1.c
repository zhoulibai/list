#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
void handler(int sig)
{
printf("Recieved signal: %d\n", sig);
}

void handler1(int sig)
{
printf("child process1 (pid=%d,ppid=%d) is killed by parent!\n",getpid(),getppid());
}
void handler2(int sig)
{
printf("child process2 (pid=%d,ppid=%d) is killed by parent!\n",getpid(),getppid());
}



int main(int argc, char *argv[])
{	//注册信号
	signal(SIGINT,handler1);
	signal(17,handler2);
	int pid1=fork();
    	int sig=2;
	if(pid1!=0) 
	{   
        int pid2=fork();
        int stat_val;
        if(pid2!=0) {
	//父进程重置信号，即屏蔽信号
	signal(SIGINT, SIG_IGN);
    	signal(17, SIG_IGN);

	printf("Caught SIGINT, input '.' to exit...\n");
		// wait signal caught
		char buf[1024] = {0};
		while (1) {
		printf("Please input: ");
		scanf("%s", buf);
		if (strcmp(buf, ".") == 0) {
			kill(pid1,SIGINT);
			kill(pid2,17);
			break;
			}
		}	
			
		wait(0);
		//wait(0);
		printf("Parent(pid=%d) process is killed!\n",getpid());
		exit(0);}
		
        else {
		sleep(10);
            exit(0);

        }
	}
	
	
    else {
		sleep(5);
	
	   	exit(0);
	}
	
      
}
