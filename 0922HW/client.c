#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
	struct sockaddr_in server;
	int sock, readSize;
	
	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8787);
	sock = socket(PF_INET, SOCK_STREAM, 0);
	connect(sock, (struct sockaddr*)&server, sizeof(server));
	
	char userInputData[1000] = "";
	while (scanf("%[^\n]", userInputData) != EOF) {
		// get trash
		getchar();

		// send data to server
		send(sock, userInputData, sizeof(userInputData), 0);
		
		// receive data from server
		char receiveDataFromServer[1000];
		readSize = recv(sock, receiveDataFromServer, sizeof(receiveDataFromServer), 0);
		receiveDataFromServer[readSize] = '\0';
		
		// print data
		printf("%s\n", receiveDataFromServer);
	}

	send(sock, "EEOF", 5, 0);

	close(sock);

	return 0;
}
