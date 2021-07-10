#include <cstdlib>
#include <iostream>
#include <winsock.h>
#include <windows.h>
#include <string>
#define CON_PORT 2014
#define REC_PORT 2015
using namespace std;

bool bConnectTo (const char* pszAaddress, int nNoPort);
bool bListenOnPort (int nNonPort);
void CloseConnection(SOCKET sock);
DWORD WINAPI Connect (LPVOID Param);
DWORD WINAPI Receive (LPVOID PARAM);

SOCKET sConn;
SOCKET sRecv;
WSADATA wsa1;
WSADATA wsa2;

int main (int argc, char *argv[])
{
	bool bStatus;
	DWORD id;
	bStatus = bListenOnPort (REC_PORT);
	if (bStatus == false)
	{
		cout << endl << "listening pada port " << REC_PORT << "gagal!!!" << endl;
	}
	else
	{
		cout << endl << "LIstening pada porting " << REC_PORT << "DONE!!" << endl;
	}
	CreateThread (NULL, 0, &Connect, NULL, 0, &id);
	CreateThread (NULL, 0, &Receive, NULL, 0, &id);
	
	while (true)
	{
		char szInput [1024] = { '\0'};
		SOCKET sSend;
		cout << "Send 2> ";
		cin.getline(szInput, 1024);
		sSend = send (sConn,szInput,1024,0);
		if (sSend == INVALID_SOCKET)
		{
		cout << "(failed sent)" << endl;	
		}
		else
		{
			cout << "Sending okay" << endl;
		}
	}
	system ("PAUSE");
	return EXIT_SUCCESS;
}
	DWORD WINAPI Receive (LPVOID Param)
{
	SOCKET sAcc;
	sAcc = accept (sRecv,NULL,NULL);
	if (sAcc == INVALID_SOCKET)
	{
		cout << endl << "connection lost!!" << endl;
	}
	else
	{
		cout << endl << "connected!!" << endl << "send> ";
	}
	while (true)
	{
		Sleep (1000);
		char szMessage[1024] = {'\0'};
		SOCKET sRcv;
		
		sRcv = recv(sAcc, szMessage, 1024, 0);
		
		if(sRcv == INVALID_SOCKET)
		{
			cout << endl << "failed data" << endl;
			break;
		}
		else
		{
			cout << endl << "send or data send :" << szMessage << endl << "send>";
		}
	}
}
DWORD WINAPI Connect (LPVOID Param)
{
	bool bKondisi;
	while (true)
	{
		Sleep (1000);
		bKondisi = bConnectTo("192.168.0.15", 2015);
		if (bKondisi == true)
		{
			cout << endl << "connected" << endl << "send> ";
			break;
		}
	}
	return 0;
}
bool bConnectTo (const char* pszAddress, int nNoPort)
{
	int error = WSAStartup (0X0202, &wsa1);
	if (error)
	{
		return false;
	}
	if (wsa1.wVersion !=0x0202)
	{
		WSACleanup ();
		return false;
	}
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons (nNoPort);
	addr.sin_addr.s_addr = inet_addr (pszAddress);
	sConn = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sConn == INVALID_SOCKET)
	{
		return false;
	}
	if (connect (sConn, (SOCKADDR *) & addr, sizeof(addr)) == SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool bListenOnPort (int nNoPort)
{
	int error = WSAStartup (0x0202, &wsa2);
	if (error)
	{
		return false;
	}
	if(wsa2.wVersion !=0x0202)
	{
		WSACleanup();
		return false;
	}
	SOCKADDR_IN addr;
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nNoPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	sRecv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if(sRecv == INVALID_SOCKET)
	{
		return false;
	}
	if(bind(sRecv, (LPSOCKADDR) &addr, sizeof(addr)) == SOCKET_ERROR)
	{
		return false;
	}
	listen (sRecv,SOMAXCONN);
}
void CloseConnection(SOCKET sock)
{
	if (sock)
	{
		closesocket(sock);
	}
	WSACleanup();
}

