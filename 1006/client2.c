#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
	fd_set rfds;
	struct sockaddr_in server;
	struct timeval tv;
	int sock, readSize, addressSize;

	int retval;
	int num1, num2, ans;

	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8787);
	sock = socket(PF_INET, SOCK_DGRAM, 0);

	addressSize = sizeof(server);

	while (scanf("%d", &num1) != EOF) {
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);

		tv.tv_sec = 3;
		tv.tv_usec = 0;
		
		sendto(sock, &num1, sizeof(num1), 0, (struct sockaddr*)&server, sizeof(server));
		
		retval = select(1, &rfds, NULL, NULL, &tv);

		if (retval == -1) {
			perror("select error!");
		}
		else if (retval) {
			char temp[256] = "";
			scanf("%s", temp);
			sendto(sock, temp, sizeof(temp), 0, (struct sockaddr*)&server, sizeof(server));
		}
		else {
			sendto(sock, "NONUM2", 7, 0, (struct sockaddr*)&server, sizeof(server));
		}
		
		readSize = recvfrom(sock, &ans, sizeof(ans), 0, (struct sockaddr*)&server, &addressSize);
		printf("Read Message: %d\n", ans);
	}

	close(sock);
}
