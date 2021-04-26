#include <iostream>

#include <ws2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;
string o = ":(";

int main() {
	//init
	WSADATA wsa;
	int wsaOk = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (wsaOk != 0) { cerr << ":(" << endl; return -1; }

	//create socket
	SOCKET listens = socket(AF_INET, SOCK_STREAM, 0);
	if (listens == INVALID_SOCKET) { cerr << o << endl; return -1; }

	//bind socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listens, (sockaddr*)&hint, sizeof(hint));

	//listen
	listen(listens, SOMAXCONN);

	//wait to connect
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSock = accept(listens, (sockaddr*)&client, &clientSize);
	if (clientSock == INVALID_SOCKET) { cerr << o << endl; return -1; }

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0))
	{
		cout << host << " port " << service << endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "on port " << ntohs(client.sin_port) << endl;
	}

	closesocket(listens);

	//talk
	char buf[4096];

	while (true) {
		ZeroMemory(buf, 4096);

		int bytesrecv = recv(clientSock, buf, 4096, 0);
		if (bytesrecv == SOCKET_ERROR) { cerr << o << endl; break; }
		if (bytesrecv == 0) { cerr << "nooooooooo" << endl; break; }

		cout << string(buf, 0, bytesrecv);

		send(clientSock, buf, bytesrecv + 1, 0);
	}
	//close socket
	closesocket(clientSock);
	//cleanup
	WSACleanup();

	system("pause>nul");
	return 0;
} 
