#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>

#define TRUE 1
#define FALSE 0
#define TIME 1000
#define VAR 500
#define CONCURRENCY 100 //并发量

typedef struct {
    int client;
    char url[21];
} Request;

typedef struct {
    Request* base;
    int front;
    int rear;
} SqQueue;

int initqueue(SqQueue *q){
    q->base = (Request*) malloc(sizeof(Request)*CONCURRENCY);
    if(!q->base) return FALSE;
    q->front = q->rear = 0;
    return TRUE;
}

int enqueue(SqQueue* queue, Request* request){
    if((queue->front+1)%CONCURRENCY ==queue->rear) return FALSE;    //队空
    queue->base[queue->rear] = *request;    //插入到队尾
    queue->rear = (queue->rear+1)%CONCURRENCY;  //队尾指针+1
    return TRUE;
}

int dequeue(SqQueue* queue,Request *request){
    if(queue->front == queue->rear) return FALSE;   //队满
    *request = queue->base[queue->front];   //返回队头的元素
    queue->front = (queue->front+1)%CONCURRENCY;    //队头后移一
    return TRUE;
}

// int main(void){
//     // Request r = {99,"abc"};
//     // SqQueue q;
//     // initqueue(&q);
//     // enqueue(&q,&r);
//     // free(q.base);
//     char content[1024];
//     int a = sprintf(content,"HTTP/1.1 200 OK\r\n"
//                 "Accept-Ranges: bytes\r\n"
//                 "Server: localhost\r\n"
//                 "Content-Type: text/html\r\n"
//                 "Content-Length: 100\r\n"
//                 "Connection: keep-alive\r\n\r\n");
//     printf("%s\n",content);
//     return 0;
// }

// int main(void){
//     Request r = {1,"abc"},t;
//     SqQueue q;
//     initqueue(&q);
//     enqueue(&q,&r);
//     dequeue(&q,&t);
//     printf("%s%d\n",t.url,t.client);
//     return 0;
// }

// int main(void){
//     char url[] = "/login.html";
//     char tmp[11],str[5];
//     sscanf(url,"%*[^.].%s",str);
//     printf("%s\n",str);
// }

void* fun1(void*){
    int i = VAR;
    while (i-->0)
    {
        printf("%d\t",i);
    }
    return NULL;
}

void* fun2(void*){
    int i = VAR;
    while (i++<TIME)
    {
        printf("HELLO\t");
    }
    return NULL;
}

int main(void){
    pthread_t a,b;
    int status = 0,i=50;
    status = pthread_create(&a,NULL,fun1,NULL);
    if(status) printf("线程1创建失败!\n");
    status = pthread_create(&b,NULL,fun2,NULL);
    if(status) printf("线程2创建失败!\n");
    pthread_join(a,NULL);
    pthread_join(b,NULL);
    return 0;
}