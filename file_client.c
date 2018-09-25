#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


#pragma pack(1)
typedef struct file
{
	char name[51];
	unsigned int size;
}file;
#pragma pack()

int main(int argc,char** argv)
{
	file f1;
	
	//第一步：创建套接字
	int sock;
	
	sock = socket(AF_INET,SOCK_STREAM,0);

	//第二步：绑定地址（可以省略）
	struct sockaddr_in myaddr;
	
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;//绑定本机随机IP
	myaddr.sin_port = htons(9999);

	if(bind(sock,(struct sockaddr*)&myaddr,sizeof(myaddr)) == -1)
	{
		perror("bind:error");
		//exit(1);
	}
	
	//第三步：连接服务器
	//指定服务器地址
	struct sockaddr_in srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = inet_addr(argv[1]);
	srvaddr.sin_port = htons(atoi(argv[2]));	


	if(connect(sock,(struct sockaddr*)&srvaddr,sizeof(srvaddr))== -1 )
	{
		perror("connect:error");
		exit(1);

	}
	//第四步：收发数据

	printf("1\n");	
	char buff[1024];
	int ret;
	
	ret = recv(sock,&f1,sizeof(f1),0);
	if(ret != 55)
	{
		fprintf(stderr,"接收错误！");
		exit(1);
	}

	FILE* fp = NULL;
	fp = fopen(f1.name,"wb");
	while(1)
	{
		ret = recv(sock,buff,sizeof(buff),0);
	
		fwrite(buff,1,ret,fp);	
		if(0 == ret)break;
	}
	close(sock);
	return 0;
}

