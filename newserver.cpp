#include <winsock2.h>  
#include <iostream>  
#pragma comment(lib, "ws2_32.lib")  

#include <sstream>
#include <string>
using namespace std;

#define BUFSIZE 64  
#define PORT 9999  

HINSTANCE hdll1;

typedef void(__stdcall *Dllfun1)();
Dllfun1 Run;

typedef void(__stdcall *Dllfun2)();
Dllfun1 getC;

typedef int(__stdcall *Dllfun3)();
Dllfun3 getTX;

typedef int(__stdcall *Dllfun4)();
Dllfun4 getTY;

typedef int(__stdcall *Dllfun5)();
Dllfun5 getTZ;

typedef int(__stdcall *Dllfun6)();
Dllfun6 getTXL;

typedef int(__stdcall *Dllfun7)();
Dllfun7 getTYL;

stringstream ss;

string str;
string str2;

DWORD WINAPI cam1(LPVOID lpparentet)
{
	Run();

	return 0;
}

int main()
{


	hdll1 = LoadLibrary(L"FINALDLL.dll");


	Run = (Dllfun1)GetProcAddress(hdll1, "run");
	getC = (Dllfun1)GetProcAddress(hdll1, "kalman_cal");
	getTX = (Dllfun3)GetProcAddress(hdll1, "getTX");
	getTY = (Dllfun4)GetProcAddress(hdll1, "getTY");
	getTZ = (Dllfun5)GetProcAddress(hdll1, "getTZ");
	getTXL = (Dllfun6)GetProcAddress(hdll1, "getTXL");
	getTYL = (Dllfun7)GetProcAddress(hdll1, "getTYL");
	CreateThread(NULL, 0, cam1, NULL, 0, NULL);


	WSADATA wsadata;
	SOCKET sServer;
	SOCKET sClient;
	int retVal;
	char buf[BUFSIZE];
	char sendBuf[BUFSIZE];//���ظ��ͻ��˵�����

	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
	{
		cout << "WSAStartupʧ��!" << endl;
		return -1;
	}

	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);                                /* ******************** 1 **************** */
	if (INVALID_SOCKET == sServer)
	{
		cout << "socket() ����!" << endl;
		WSACleanup();
		return -1;
	}

	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(PORT);
	addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	retVal = bind(sServer, (const struct sockaddr*)&addrServ, sizeof(addrServ));        /* ******************** 2 **************** */
	if (SOCKET_ERROR == retVal)
	{
		cout << "bind ����!" << endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}

	retVal = listen(sServer, 10);                                                        /* ******************** 3 **************** */
	if (SOCKET_ERROR == retVal)
	{
		cout << "listen ����!" << endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}

	cout << "TCP��������ʼ����" << PORT << "�˿�" << endl;
	SOCKADDR_IN addrClient;
	int addrClientLen = sizeof(addrClient);
	sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientLen);               /* ******************** 4 **************** */
	if (INVALID_SOCKET == sClient)
	{
		cout << "accept() ����!" << endl;
		WSACleanup();
		return -1;
	}

	while (true)
	{

		float TX = 0, TY=0, TZ = 0, TXL=0, TYL=0;

		TX = getTX();
		TY = (getTY())/3;
		TZ = getTZ();
		TXL = getTXL()-400;
		TYL = getTYL();
		//printf("%f  %f  %f \n", TX, TY, TZ);


		/*ZeroMemory(buf, BUFSIZE);
		retVal = recv(sClient, buf, BUFSIZE, 0);
		if (SOCKET_ERROR == retVal)
		{
		cout << "recv()����!" << endl;
		closesocket(sServer);
		closesocket(sClient);
		WSACleanup();
		return -1;
		}

		SYSTEMTIME time;
		GetLocalTime(&time);
		char datetime[30];
		sprintf(datetime, "%4d-%2d-%2d %2d:%2d:%2d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		cout << datetime << "��" << inet_ntoa(addrClient.sin_addr) << ":" << addrClient.sin_port << "��������������: " << buf << endl;

		if (strcmp(buf, "quit") == 0)
		{
		retVal = send(sClient, "quit", strlen("quit"), 0);
		break;
		}
		else
		{
		retVal = send(sClient, buf, strlen(buf), 0);
		if (SOCKET_ERROR == retVal)
		{
		cout << "send����!" << endl;
		closesocket(sServer);
		closesocket(sClient);
		WSACleanup();
		return -1;
		}
		}*/
		//���տͻ�������





		
		ss << TX;
		ss >> str;

		str2 += str + " ";
		str.clear();
		ss.clear();


		ss << TY;
		ss >> str;

		str2 += str + " ";
		str.clear();
		ss.clear();

		ss << TZ;
		ss >> str;

		str2 += str +" ";
		str.clear();
		ss.clear();


		ss << TXL;
		ss >> str;

		str2 += str + " ";
		str.clear();
		ss.clear();


		ss << TYL;
		ss >> str;

		str2 += str + " ";
		str.clear();
		ss.clear();

		ss << 200;
		ss >> str;

		str2 += str + " ";
		str.clear();
		ss.clear();

		cout <<"****" << endl;

		send(sClient, str2.c_str(), str2.length(), 0);

		cout << str2 << "****" << endl;

		str2.clear();


		Sleep(50);


		/*//�˳�
		closesocket(sServer); //�ر��׽���
		closesocket(sClient); //�ر��׽���
		WSACleanup(); //�ͷ��׽�����Դ;

		return 0;



		closesocket(sServer);
		closesocket(sClient);
		WSACleanup();

		return 0;*/

	}

}