#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
	struct sockaddr_in server;
	int sock;
	char buf[] = "D1081760";
	bzero(&server, sizeof(server));
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8787);
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	sendto(sock, buf, sizeof(buf), 0, (struct sockaddr*)&server, sizeof(server));
	close(sock); 
}
