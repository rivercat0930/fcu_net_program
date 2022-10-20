#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	fd_set rfds, rfds1, rfds2;
	struct sockaddr_in server;
	struct timeval tv;
	int sock, readSize, addressSize;

	char data1[256] = "";
	char data2[256] = "";
	char data3[256] = "";
	char data4[256] = "";

	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8787);
	sock = socket(PF_INET, SOCK_DGRAM, 0);

	addressSize = sizeof(server);

	while (scanf("%s", data1) != EOF) {
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);

		tv.tv_sec = 3;
		tv.tv_usec = 0;

		int retval, retval2, retval3;
		
		sendto(sock, data1, sizeof(data1), 0, (struct sockaddr*)&server, sizeof(server));

		retval = select(1, &rfds, NULL, NULL, &tv);
		if (retval == -1) {
			perror("select error!");
		}
		else if (retval) {
			scanf("%s", data2);
			sendto(sock, data2, sizeof(data2), 0, (struct sockaddr*)&server, sizeof(server));
		}
		else {
			sendto(sock, "NULL", 5, 0, (struct sockaddr*)&server, sizeof(server));
		}

		FD_ZERO(&rfds);
		FD_SET(0, &rfds);

		tv.tv_sec = 3;
		tv.tv_usec = 0;

		retval2 = select(1, &rfds, NULL, NULL, &tv);
		if (retval2 == -1) {
			perror("select error!");
		}
		else if (retval2) {
			scanf("%s", data3);
			sendto(sock, data3, sizeof(data3), 0, (struct sockaddr*)&server, sizeof(server));
		}
		else {
			sendto(sock, "NULL", 5, 0, (struct sockaddr*)&server, sizeof(server));
		}

		FD_ZERO(&rfds);
		FD_SET(0, &rfds);

		tv.tv_sec = 3;
		tv.tv_usec = 0;
		
		retval3 = select(1, &rfds, NULL, NULL, &tv);
		if (retval3 == -1) {
			perror("select error!");
		}
		else if (retval3) {
			scanf("%s", data4);
			sendto(sock, data4, sizeof(data4), 0, (struct sockaddr*)&server, sizeof(server));
		}
		else {
			sendto(sock, "NULL", 5, 0, (struct sockaddr*)&server, sizeof(server));
		}

		printf("data1: %s\n", data1);
		printf("data2: %s\n", data2);
		printf("data3: %s\n", data3);
		printf("data4: %s\n", data4);
	
		memset(data1, 0, sizeof(data1));
		memset(data2, 0, sizeof(data2));
		memset(data3, 0, sizeof(data3));
		memset(data4, 0, sizeof(data4));

		char reply[1000] = "";
		recvfrom(sock, reply, sizeof(reply), 0, (struct sockaddr*)&server, &addressSize);
		reply[999] = '\0';
		printf("%s\n", reply);
	}

	char *closeMsg = "CTRL + D";
	sendto(sock, closeMsg, sizeof(closeMsg), 0, (struct sockaddr*)&server, sizeof(server));

	close(sock);
}

