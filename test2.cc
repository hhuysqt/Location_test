#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdio>


using namespace std;

int main() {

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
		printf("Failed to create socket..\n");
        return -1;
    }
	printf("Socket OK!\n");

    sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    srand(time(NULL)); 
    int port = 8889;
    servaddr.sin_port = htons(port); // TODO: 6666 is a temperory number

    if (bind(listenfd, (sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		printf("Bind ERROR!\n");
        return -1;
    }
	printf("Bind succeeded!\n");

    if (listen(listenfd, 1000) < 0) { // TODO: 1000 is a temperory number
	printf("Listen ERROR!\n");
        exit(-1);
    }
    printf("Srart listening Port%d\n", port);

	FILE *receiveData = fopen("1socketReceive.txt", "a");
	if(receiveData == NULL)
	{
		printf("socketReceive.txt not found..\n");
		exit(-1);
	}

    while (true) {
        int connfd = accept(listenfd, (sockaddr*)NULL, NULL);
        
        if (connfd < 0) {
			printf("Failed to accept...\n");
			break;
        }
		else if(connfd == 0){
			printf("Shutdown...\n");
			break;
		}
       
        char buff[1025];
		int receiveNum;
        bzero(buff, sizeof(buff));
		printf("Waiting for message...\n");
        if ((receiveNum = read(connfd, buff, 1024)) < 0) {
			close(connfd);
			printf("Read error!\n");
			break;
        }
/*
	for(int i = 0; i < sizeof(buff); i ++){
       	 printf("%2x ", buff[i]);
	}
*/
		printf("%s\n",buff);
		if(fwrite(buff, 1, receiveNum, receiveData) < 0)
		{
			printf("write message error...\n");
		}
		//write(connfd, "Hello!!!", 8);
		fflush(receiveData);
		close(connfd);
    }

    return 0;
}



