#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

string ipaddr = "127.0.0.1";
int port = 54000;

int main() {
	//init wsa
	WSADATA wsadata;
	int ws = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (ws != 0) { return 0; }

	//make socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) { WSACleanup(); return 0; }

	// fill in info of structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipaddr.c_str(), &hint.sin_addr);

	//connect
	int conn = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if(conn == SOCKET_ERROR) { WSACleanup(); return 0; }

	//do something
	char buff[4096];
	string input;

	do {
		cout << ">";
		getline(cin, input);

		if (input.size() > 0) {
			int sendRes = send(sock, input.c_str(), input.size(), 0);
			if (sendRes != SOCKET_ERROR) {
				ZeroMemory(buff, 4096);
				int bytesRec = recv(sock, buff, 4096, 0);
				if (bytesRec > 0) {
					cout << "server:" << string(buff, 0, bytesRec) << endl;
				}
			}
		}

	} while (input.size() > 0);

	//cleanup
	closesocket(sock);
	WSACleanup();	

	return 0;
}