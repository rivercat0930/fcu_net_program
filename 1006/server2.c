#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	struct sockaddr_in server, client;
	int sock, addressSize;
	int num1, num2, ans;

	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8787);
	sock = socket(PF_INET, SOCK_DGRAM, 0);

	bind(sock, (struct sockaddr*)&server, sizeof(server));

	addressSize = sizeof(client);
	
	while (1) {
		recvfrom(sock, &num1, sizeof(num1), 0, (struct sockaddr*)&client, &addressSize);
		printf("Read Message[1]: %d\n", num1);

		char temp[256] = "";

		recvfrom(sock, temp, sizeof(temp), 0, (struct sockaddr*)&client, &addressSize);
		temp[255] = '\0';
		
		if (strcmp(temp, "NONUM2") == 0) {
			ans = num1 * 100;
			sendto(sock, &ans, sizeof(ans), 0, (struct sockaddr*)&client, sizeof(client));
			continue;
		}
		
		printf("%s\n", temp);
		num2 = atoi(temp);
		printf("Read Message[2]: %d\n", num2);

		if (num2 != 0) {
			ans = num1 / num2;
		}
		
		printf("Answer: %d\n", ans);

		sendto(sock, &ans, sizeof(ans), 0, (struct sockaddr*)&client, sizeof(client));
	}

	close(sock);
}
