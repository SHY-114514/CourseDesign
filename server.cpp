#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>			   //SOCKET编程
#pragma comment(lib, "ws2_32.lib") // gcc 编译时加入-lws2_32参数

#define TRUE 1
#define FALSE 0
#define BUF_SIZE 1024			   // 缓冲区大小
#define SERVER_IP_ADDR "127.0.0.1" // 服务器IP地址
#define SERVER_PORT 80			   // 服务器端口号
#define CONCURRENCY 100			   // 并发量
#define LOGV(msg) printf("\033[0;32;40m【INFO    】 %s\033[0m\n", msg)
#define LOGW(msg) printf("\033[0;33;40m【WARNING 】 %s\033[0m\n", msg)
#define LOGE(msg)                                                  \
	{                                                              \
		printf("\033[0;31;40m【ERROR   】 %s,退出\033[0m\n", msg); \
		exit(-1);                                                  \
	}

// typedef struct
// {
// 	SOCKET client;
// 	char *msg;
// } Request;

// typedef struct
// {
// 	Request *base;
// 	int front;
// 	int rear;
// } SqQueue;

// SqQueue requests;
// SqQueue threads;

// int initqueue(SqQueue *q);
// int enqueue(SqQueue *queue, Request *data);
// int dequeue(SqQueue *queue, Request *data);

int Exit = FALSE;

// 初始化工件
int initer(SOCKET *server);
void listener(SOCKET server);
// 请求处理器
void handler(SOCKET s, const char *msg);
// GET请求响应器
void responser(SOCKET s, const char *url);

int main(void)
{
	SOCKET server;
	if (!initer(&server))
		LOGE("初始化失败");
	listener(server);
	LOGV("退出系统");
	return 0;
}

int initer(SOCKET *server)
{
	// 初始化动态链接库
	LOGV("初始化动态链接库 ...");
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return FALSE;
	LOGV("创建套接字 ...");
	*server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*server == INVALID_SOCKET)
		return FALSE;
	LOGV("配置IP、绑定端口 ...");
	struct sockaddr_in ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SERVER_PORT);
	ServerAddr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
	if (bind(*server, (SOCKADDR *)&ServerAddr, sizeof(struct sockaddr)) == SOCKET_ERROR)
		return FALSE;
	return TRUE;
}

void responser(SOCKET client, const char *url)
{
	int filesize = 0;
	char filetype[5], filepath[21], info[25];
	sscanf(url, "%*[^.].%s", filetype);
	sprintf(filepath, "./resources/%s%s", filetype, url);
	sprintf(info, "响应%s", filepath);
	LOGV(info);
	FILE *fp = fopen(filepath, "rb");
	if (fp == NULL)
	{
		LOGW("打开文件失败");
		return;
	}
	char temp[1024];
	do
	{
		fgets(temp, 1024, fp);
		send(client, temp, strlen(temp), 0);
	} while (!feof(fp));
	fclose(fp);
	// char content[401*1024],file[400*1024];
	// fseek(fp, 0, SEEK_END);
	// filesize = ftell(fp);
	// fseek(fp, 0, SEEK_SET);
	// fread(file, 1, filesize, fp);
	// fclose(fp);
	// if (strcmp(filetype, "png") == 0)
	// 	strcat(filetype, "image/png");
	// else
	// 	strcat(filetype, "text/html");
	// int a = sprintf(content, "HTTP/1.1 200 OK\r\n"
	// 						//  "Accept-Ranges: bytes\r\n"
	// 						 "Server: localhost\r\n"
	// 						 "Content-Type: %s\r\n"
	// 						 "Content-Length: %d\r\n"
	// 						 "Connection: keep-alive\r\n\r\n",
	// 				filetype, filesize);
	// memcpy(content + a, file, strlen(file));
	// send(client, content, strlen(content), 0);
}

void handler(SOCKET client, char *msg)
{
	char method[7], url[51], version[10], info[55];
	sscanf(msg, "%s %s %s", method, url, version);
	sprintf(info, "请求%s", url);
	LOGV(info);
	if (strcmp(method, "GET") == 0)
		responser(client, url); // GET请求直接响应目标文件
	else if (strcmp(method, "POST") == 0)
	{
		LOGV("接收到POST请求");
	}
}

void listener(SOCKET server)
{
	SOCKET client;
	struct sockaddr_in ClientAddr;
	int rval, length;
	char revbuf[BUF_SIZE], info[31];
	// 自动打开网站首页(测试用)
	system("start http://localhost:80/login.html");
	while (!Exit)
	{
		printf("\n");
		LOGV("监听请求 ...");
		if (listen(server, 10) == SOCKET_ERROR)
			LOGW("失败");
		// 接受客户端请求建立连接
		length = sizeof(struct sockaddr);
		client = accept(server, (SOCKADDR *)&ClientAddr, &length);
		if (client == INVALID_SOCKET)
			LOGW("连接客户端失败");
		sprintf(info, "%s:%d已连接", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));
		LOGV(info);
		// 接收客户端请求数据
		rval = recv(client, revbuf, BUF_SIZE, 0);
		if (rval <= 0)
		{
			LOGW("接收客户端数据失败");
			closesocket(client);
			continue;
		}
		handler(client, revbuf);
		closesocket(client);
	}
	// 关闭套接字
	closesocket(server);
	// 停止Winsock
	WSACleanup();
	printf("退出系统");
	// free(requests.base);
}

// int initqueue(SqQueue *q)
// {
// 	q->base = (Request *)malloc(sizeof(Request) * CONCURRENCY);
// 	if (!q->base)
// 		return FALSE;
// 	q->front = q->rear = 0;
// 	return TRUE;
// }

// int enqueue(SqQueue *queue, Request *data)
// {
// 	if ((queue->front + 1) % CONCURRENCY == queue->rear)
// 		return FALSE;							   // 队空
// 	queue->base[queue->rear] = *data;			   // 插入到队尾
// 	queue->rear = (queue->rear + 1) % CONCURRENCY; // 队尾指针+1
// 	return TRUE;
// }

// int dequeue(SqQueue *queue, Request *data)
// {
// 	if (queue->front == queue->rear)
// 		return FALSE;								 // 队满
// 	*data = queue->base[queue->front];				 // 返回队头的元素
// 	queue->front = (queue->front + 1) % CONCURRENCY; // 队头指针-1
// 	return TRUE;
// }