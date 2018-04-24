#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#include <string>
//for video
#include <opencv2/opencv.hpp>

//#include <stdlib.h>
int main()
{
	std::cout << "start" << std::endl;
	cv::Mat img0 = cv::imread("12.jpg");
	//mat to char
	char* ptrbuf = new char[img0.cols * img0.rows * 3 + 24];

	union MyUnion
	{
		unsigned char r[4];
		int x;
	} rowC, colC;
	union 
	{
		unsigned char l[16];
		int l1;
	}Length;


	rowC.x = img0.rows;
	colC.x = img0.cols;
	int t = img0.total();
	//

	//int p1 = 0;
	//int p2 = 0;

	//p1 = *(int *)rowC.r;
	//p2 = *(int *)colC.r;

	struct MyStruct
	{
		uchar* row;
		uchar* col;
		uchar* r;
	} st;
	st.row = rowC.r;
	st.col = colC.r;
	st.r = img0.data;

	
	//testing: start
	//cv::Mat img1 = cv::Mat(img0.rows, img0.cols, CV_8UC3);

	//for (size_t i = 0; i <= img0.cols * img0.rows * 3; i++)
	//{
	//	img1.data[i] = ptrbuf[i + 27];
	//}
	////img1.data = ptrbuf;
	//cv::namedWindow("image", CV_WINDOW_AUTOSIZE);
	//cv::imshow("image", img1);
	//cv::waitKey(0);
	//testing: end
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

	const char* buf = ptrbuf;
	int sendResult = send(sock, buf, img0.cols * img0.rows * 3 + 26, 0); //bufLen
	if (sendResult != SOCKET_ERROR)
	{
		std::cout << "bytes send : " << sendResult << std::endl;
	}

	//do-while send receive
	//char buf[4096];
	//std::string userinput;
	//do
	//{
	//	//detect some user text
	//	std::cout << "> ";
	//	std::getline(std::cin, userinput);
	//	if (userinput.size() > 0)
	//	{
	//		//try send text
	//		int sendResult = send(sock, userinput.c_str(), userinput.size() + 1, 0);
	//		if (sendResult != SOCKET_ERROR)
	//		{
	//			//wait for response
	//			ZeroMemory(buf, 4096);
	//			int bytesReceived = recv(sock, buf, 4096, 0);
	//			if (bytesReceived > 0)
	//			{
	//				//echo response to console
	//				std::cout << "response from server: " << std::string(buf, 0, bytesReceived) << std::endl;
	//			}
	//		}
	//	}
	//	
	//} while (userinput.size() > 0);
	//close and clean
	closesocket(sock);
	WSACleanup();
	return 0;
}