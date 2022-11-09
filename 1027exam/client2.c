#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void modeMenu(int *mode) {
	printf("1: Broadcast\n");
	printf("2: Private\n");
	printf("3: Group\n");
	printf("> ");
	scanf("%d", mode);
	getchar();
}

int main() {
	struct sockaddr_in server;
	int sock, readSize;
	char buf[] = "Hello";
	char sendBuf[256] = "";
	char readBuf[256] = "";
	pid_t pid;
	int mode = 0;
	
	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8787);
	sock = socket(PF_INET, SOCK_STREAM, 0);

	connect(sock, (struct sockaddr*)&server, sizeof(server));
	read(sock, readBuf, sizeof(readBuf));
	printf("%s\n", readBuf);

	pid = fork();

	while (1) {
		if (pid == -1) {
			perror("Fork");
			break;
		}
		else if (pid == 0) {
			int test = read(sock, readBuf, sizeof(readBuf));
			
			if (test == 0) {
				kill(pid, SIGKILL);
				break;
			}
			
			//printf("RSock ID: %d\n", sock);
			printf("Read Message: %s\n\n", readBuf);
		}
		else if (pid > 0) {
			if (mode == 0)
				modeMenu(&mode);

			if (scanf("%27[^\n]%*c", sendBuf) != EOF) {
				if (strcmp(sendBuf, "EXIT!") == 0) {
					kill(pid, SIGKILL);
					break;
				}

				if (strcmp(">Back", sendBuf) == 0 || mode == 0) {
					modeMenu(&mode);
					strcat(sendBuf, " ");
				}
				
				if (mode == 1) {
					write(sock, sendBuf, sizeof(sendBuf));
					printf("WSock ID: %d \n", sock);
					printf("Send Message: %s\n\n", sendBuf);
				}
				else if (mode == 2) {
					printf("Enter socket number: ");
					int t;
					scanf("%d", &t);

					char nbuf[256];
					sprintf(nbuf, "/msg%d %s", t, sendBuf);
					strcat(nbuf, "\nPRIVATE");
					write(sock, nbuf, sizeof(nbuf));
				}
				else if (mode == 3) {
					printf("Enter Group socket number: ");
					char a[100];
					scanf("%s", a);
					for(int i = 0; i < 100; i++) {
						if (a[i] != ' ') {
							char nbuf[256];
							sprintf(nbuf, "/msg%d %s", a[i], sendBuf);
							strcat(nbuf, "\nGROUP");
							write(sock, nbuf, sizeof(nbuf));
						}
					}
				}
			}
			else {
				printf("EOF!!\n");
				kill(pid, SIGKILL);
				break;
			}
		}
	}
	
	close(sock);
	printf("Client close");

	return 0;
}
