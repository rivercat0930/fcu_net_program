#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

int max(int numbers[4]);
int min(int numbers[4]);
int sum(int numbers[4]);

int main() {
	struct sockaddr_in server, client;
	int sock, csock;
	char clientSideData[1000];
	bzero(&server, sizeof(server));

	// fill arguments
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8787);

	sock = socket(PF_INET, SOCK_STREAM, 0);
	bind(sock, (struct sockaddr*)&server, sizeof(server));
	listen(sock, 5);
	int addressSize = sizeof(client);
	csock = accept(sock, (struct sockaddr*)&client, &addressSize);

	while (1) {
		// get data
		int readSize = recv(csock, clientSideData, sizeof(clientSideData), 0);
		clientSideData[readSize] = '\0';

		// check client side close or not
		if (strcmp(clientSideData, "EEOF") == 0 || strcmp(clientSideData, "") == 0) {
			printf("Client has closed the connection.\n");
			break;
		}

		// split string
		char* p = strtok(clientSideData, " ");
		int numbers[4];
		for (int i = 0; p; i++) {
			numbers[i] = atoi(p);
			p = strtok(NULL, " ");
		}

		// get request number
		int maxNumber = max(numbers);
		int minNumber = min(numbers);
		int sumNumber = sum(numbers);

		char answer[1000] = "";
		char temp[100];
		
		// connect all string
		strcat(answer, "Max Number: ");
		sprintf(temp, "%d", maxNumber);
		strcat(answer, temp);
		
		strcat(answer, "\nMin Number: ");
		sprintf(temp, "%d", minNumber);
		strcat(answer, temp);

		strcat(answer, "\nSum: ");
		sprintf(temp, "%d", sumNumber);
		strcat(answer, temp);

		// send back to client
		send(csock, answer, sizeof(answer), 0);
		printf("Already send back to client.\n");
	}

	close(sock);

	return 0;
}

int max(int numbers[4]) {
	int maxNumber = numbers[0];

	for (int i = 1; i < 4; i++) {
		if (maxNumber < numbers[i])
			maxNumber = numbers[i];
	}

	return maxNumber;
}

int min(int numbers[4]) {
	int minNumber = numbers[0];

	for (int i = 1; i < 4; i++) {
		if (minNumber > numbers[i])
			minNumber = numbers[i];
	}

	return minNumber;
}

int sum(int numbers[4]) {
	int sumNumber = 0;

	for (int i = 0; i < 4; i++) {
		sumNumber = sumNumber + numbers[i];
	}

	return sumNumber;
}
