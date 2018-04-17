#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#include <string>

int main()
{
	std::string ipAddress = "127.0.0.1";
	int port = 54000;
	//init winsocket
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &wsData);
	if (wsResult != 0)
	{
		std::cerr << "cant start winsock, Error #" << WSAGetLastError() << std::endl;
		return -1;
	}

	//create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "cant create socket Error #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	//hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	
	//connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "cant connect to server with error #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	//do-while send receive
	char buf[4096];
	std::string userinput;
	do
	{
		//detect some user text
		std::cout << "> ";
		std::getline(std::cin, userinput);
		if (userinput.size() > 0)
		{
			//try send text
			int sendResult = send(sock, userinput.c_str(), userinput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				//wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					//echo response to console
					std::cout << "response from server: " << std::string(buf, 0, bytesReceived) << std::endl;
				}
			}
		}
		
	} while (userinput.size() > 0);
	//close and clean
	closesocket(sock);
	WSACleanup();
	return 0;
}