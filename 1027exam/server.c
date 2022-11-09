#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void *connection_handler(void *);

int sockList[100] = {0};
int working = 0;
char user[5][5] = {"AA", "BB", "CC", "DD", "EE"};
char pass[5][10] = {"", "", "", "", ""};

int main() {
	struct sockaddr_in server, client;
	unsigned int sock, csock, addressSize;
	char buf[256];
	pthread_t sniffer_thread;

	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8787);
	sock = socket(PF_INET, SOCK_STREAM, 0);

	bind(sock, (struct sockaddr*)&server, sizeof(server));
	listen(sock, 5);

	addressSize = sizeof(client);

	while(csock = accept(sock, (struct sockaddr*)&server, &addressSize)) {
		working++;
		printf("Online connect: %d\n", working);

		if (pthread_create(&sniffer_thread, 0, connection_handler, (void *)&csock) != 0) {
			perror("Thread creation");
		}
		else {
			pthread_detach(sniffer_thread);
		}

		if (csock < 0)
			perror("Csock");
	}

	close(sock);
	printf("Socket Close\n");

	return 0;
}

void *connection_handler(void* sock) {
	int csock = *(int *) sock;
	int readSize;
	int number = working - 1;
	char buf[256];
	char temp[256];

	while((readSize = read(csock, buf, sizeof(buf))) != 0) {
		if (sockList[number] == 0) {
			buf[readSize] = '\0';

			// 0 user does not exist
			// 1 password incorrect!
			// 2 is ok
			int check = 0;
			for (int i = 0; i < 5; i++) {
				if (strcmp(user[i], buf) == 0 && strcmp(pass[i], "") == 0) {
					write(csock, "Enter new passwd: ", 20);
					readSize = read(csock, buf, sizeof(buf));
					buf[readSize] = '\0';
					strcpy(pass[i], buf);
					check = 2;
				}
				else if (strcmp(user[i], buf) == 0) {
					write(csock, "Enter passwd: ", 20);
					readSize = read(csock, buf, sizeof(buf));
					buf[readSize] = '\0';
					if (strcmp(buf, pass[i]) == 0)
						check = 2;
					else
						check = 1;
				}
			}

			if (check == 0) {
				write(csock, "User does not exist!\n", 30);
				readSize = 0;
				break;
			}
			else if(check == 1) {
				write(csock, "Password incorrent!\n", 30);
				readSize = 0;
				break;
			}
			
			if (number >= 5) {
				write(csock, "Server full\n", 15);
				readSize = 0;
				break;
			}
			
			sockList[number] = csock;
		}
		
		printf("CSock ID: %d\n", csock);
		printf("Read Message: %s\n", buf);

		memcpy(temp, buf, 4);
		int socknum = buf[4] - '0';
		int privateMsg = 0;
		
		if (strcmp("/msg", temp) == 0)
			privateMsg = 1;

		char online[100] = "";
		for (int i = 0; i < 100; i++) {
			if (sockList[i] != 0) {
				char t[5];
				sprintf(t, "%d ", sockList[i]);
				strcat(online, t);
			}
		}


		for (int i = 0; i < 100; i++) {
			if (privateMsg) {
				if (socknum == sockList[i]) {
					strcat(buf, "\nPRIVATE\n");
					write(sockList[i], buf, sizeof(buf));
				}
			}
			else if (sockList[i] != 0) {
				if (i == number) {
					strcat(buf, "\nNumber: ");
					sprintf(temp, "%d", number + 1);
					strcat(buf, temp);
				}
				else {
					sprintf(temp, "%d", csock);
					strcat(buf, "\nSendPersonSockID: ");
					strcat(buf, temp);
				}
				
				strcat(buf, "\n");
				strcat(buf, online);
				write(sockList[i], buf, sizeof(buf));
			}
		}
		printf("\n");
	}

	if (readSize == 0) {
		printf("Client Disconnect!\n");
		printf("CSock ID: %d\n", csock);

		int delete;
		working--;

		for (int i = 0; i < 100; i++) {
			if (sockList[i] == csock)
				delete = i;
		}

		for (int i = delete; i < 100; i++) {
			sockList[i] = sockList[i + 1];
			sockList[99] = 0;
		}
	}

	close(csock);
	printf("Client Close\n\n");
	pthread_exit(0);
}
