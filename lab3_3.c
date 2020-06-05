#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#define N 100
#define producerNum  15
#define consumerNum  5

int buffer[N] = {0},value=0;
int in=0,out=0;
//pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER；

int mutex=1,empty=N,full=0,proCmutex=1;  //mutex缓冲区操作的互斥信号量，empty判断缓冲区是否满，full判断缓冲区是否为空
void * producer(void * a){
    while(1){
        while(proCmutex <= 0);  //判断是否有其它生产者线程生产数据
        proCmutex--;   //若没有，可生产消息
        value++;
        printf("生产一个消息%d, 缓冲区位置是%d\n",value,in);
        proCmutex++;

        while(empty <= 0){
            	printf("缓冲区已满！\n"); //判断缓冲区是否已满
		sleep(2);
        }
        empty--;

        while(mutex <= 0);//判断是否有其它线程操作缓冲区
        mutex--;         //互斥信号量-1
        //pthread_mutex_lock；

        buffer[in] = value;//往缓冲区中写数据
        in = (in + 1) % N;  //缓冲区位置

        mutex++;
        full++;
        sleep(3);
    }
}


void * consumer(void *b){
    while(1){
        while(full <= 0){
            	printf("缓冲区为空！\n");
		sleep(2);
        }
        full--;

        while(mutex <= 0);//互斥信号量
        mutex--;

        int nextc = buffer[out];//取数据条件成立，取出数据
        buffer[out] = 0;//消费完将缓冲区设置为0

        out = (out + 1) % N;

        mutex++;
        empty++;

        printf("\t\t\t\t取出一个消息ID%d,缓冲区位置为%d\n", nextc,out);
        sleep(2);
    }
}

int main() {
    //sem_init();
    //code here
    pthread_t threadPool[producerNum+consumerNum];
    int i;
    for(i = 0; i < producerNum; i++){
        pthread_t temp;
        if(pthread_create(&temp, NULL, producer, NULL) == -1){
            printf("ERROR, fail to create producer%d\n", i);
            exit(1);
        }
        threadPool[i] = temp;
    }//创建生产者线程

	for(i = 0; i < consumerNum; i++){
        pthread_t temp;
        if(pthread_create(&temp, NULL, consumer, NULL) == -1){
            printf("ERROR, fail to create consumer%d\n", i);
            exit(1);
        }
        threadPool[i+producerNum] = temp;
		}//创建消费者线程

	
    

    void * result;
    for(i = 0; i < producerNum+consumerNum; i++){
        if(pthread_join(threadPool[i], &result) == -1){
            printf("fail to recollect\n");
            exit(1);
        }
    }//运行线程
    return 0;
}
