#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
	struct sockaddr_in server, client;
	int sock, addressSize;
	
	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8787);
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	
	bind(sock, (struct sockaddr*)&server, sizeof(server));

	addressSize = sizeof(client);

	while (1) {
		char data1[256] = "";
		char data2[256] = "";
		char data3[256] = "";
		
		recvfrom(sock, data1, sizeof(data1), 0, (struct sockaddr*)&client, &addressSize);
		data1[255] = '\0';
		
		if (strcmp("CTRL + D", data1) == 0 || strcmp("", data1) == 0) {
			printf("CLOSE\n");
			break;
		}
		
		recvfrom(sock, data2, sizeof(data2), 0, (struct sockaddr*)&client, &addressSize);
		data2[255] = '\0';
		
		recvfrom(sock, data3, sizeof(data3), 0, (struct sockaddr*)&client, &addressSize);
		data3[255] = '\0';
		
		int times = atoi(data3);

		printf("Get data\n");

		char ans[1000] = "";

		if (strcmp(data1, "1") == 0) {
			char temp[256] = "";
			for (int i = 0; i < sizeof(data2); i++) {
				char f = toupper(data2[i]);
				temp[i] = f;
			}

			for (int i = 0; i < times; i++) {
				strcat(ans, temp);
				strcat(ans, " ");
			}
		}
		else {
			char temp[256] = "";
			for (int i = 0; i < sizeof(data2); i++) {
				char f = tolower(data2[i]);
				temp[i] = f;
			}

			for (int i = 0; i < times; i++) {
				strcat(ans, temp);
				strcat(ans, " ");
			}
		}

		sendto(sock, ans, sizeof(ans), 0, (struct sockaddr*)&client, sizeof(client));
		printf("%s\n", ans);
		printf("Data already send!!!\n");
	}

	close(sock);
}

