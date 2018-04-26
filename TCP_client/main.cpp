#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#include <string>
//for video
#include <opencv2/opencv.hpp>

//#include <stdlib.h>
int main()
{
	//std::cout << "start" << std::endl;
	cv::VideoCapture vid("videoFile.avi");
	if (!vid.isOpened())
	{
		std::cout << "can't open video" << std::endl;
	}
	cv::Mat frame;

	cv::namedWindow("video", CV_WINDOW_NORMAL);
	cv::resizeWindow("video", 800, 600);
	while (vid.read(frame))
	{
		cv::imshow("video", frame);
		if (cv::waitKey(1000)>= 0)
		{
			break;
		}
	}
	//for(;;)
	//{
	//	ZeroMemory(frame.data, frame.cols*frame.rows * 3);
	//	vid >> frame;
	//	if (frame.empty())
	//	{
	//		break;
	//	}
	//	else
	//	{
	//		cv::imshow("video", frame);
	//	}
	//			
	//	//if (cv::waitKey(30) >= 0) break;
	//}


	struct StructImage
	{
		int rows;
		int cols;
		size_t total;
	} strImg;

	ZeroMemory(&strImg, sizeof(strImg));
	strImg.rows = frame.rows;
	strImg.cols = frame.cols;
	strImg.total = frame.total();
	//memcpy(&st.row, &(img0.rows), sizeof img0.rows);
	//memcpy(&st.col, &(img0.cols), sizeof img0.cols);

	//char z[img0.rows];

	//testing: start
	//cv::Mat img1 = cv::Mat(strImg.rows, strImg.cols, CV_8UC3);
	////img1.data = (uchar *)img0.data;
	//char* imgBuff = (char*)malloc(strImg.total * 3);
	//ZeroMemory(imgBuff, strImg.total * 3);
	//memcpy(imgBuff, img0.data, strImg.total * 3);
	////imgBuff = (char*)img0.data;
	//img1.data = (uchar*)imgBuff;

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

	
	//int sendResult = send(sock, buf, img0.cols * img0.rows * 3 + 26, 0); //bufLen
	int sendStruct = send(sock, (char*)&strImg, sizeof strImg, 0); //bufLen
	//int sendImage  = send(sock, (const char*)img0.data, st.total * 3, 0);

	/*recv(sock, (char*)&st, sizeof st, 0);
	char *buff = (char*)malloc(st.total * 3);
	recv(sock, buff, st.total * 3, 0);*/


	if (sendStruct != SOCKET_ERROR)
	{
		std::cout << "Image structure sended with bytes : " << sendStruct << std::endl;
		int sendImage = send(sock, (const char*)frame.data, strImg.total * 3, 0);
		if (sendImage != SOCKET_ERROR)
		{
			std::cout << "Image data sended with bytes : " << sendStruct << std::endl;
		}

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