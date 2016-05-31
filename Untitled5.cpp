#include <cstdio>
#include <cstdlib>
#include <conio.h> //dla getch
//included win_32.a
//to dziala bardziej jako klient

#include <winsock2.h>
#define BUFFER_LENGTH 1024
#define PORT 27015

using namespace std;

int main()
{
	WSADATA wsaData;
	int iRes = WSAStartup(MAKEWORD(2,2), &wsaData);
	u_long iMode = 1;
	fd_set readset;
	struct timeval tv;
	
	
	int bytesSent, bytesRecv;
	char sendbuf[BUFFER_LENGTH];
	char recvbuf[BUFFER_LENGTH];
	
	memset(&recvbuf,'\0',BUFFER_LENGTH);
	memset(&sendbuf,'\0',BUFFER_LENGTH);
	
	int bufflen = BUFFER_LENGTH;
	if (iRes != 0)
	{
		printf("Couldnt initialize WSAStartup.\n");
		return 1;
	}
	SOCKET mainSocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
	if (mainSocket == INVALID_SOCKET)
	{
		printf("error while creating socket, err: %s.\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	
	iRes = ioctlsocket(mainSocket, FIONBIO, &iMode);
	if (iRes != NO_ERROR)
	{
		printf("error on ioctlsocket");
		WSACleanup();
		return 1;
	}
	
	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1"); //sin_addr struktura zawierajaca s_addr, s_addr moze byc kilka, kilka adresow, niepotrzebne dla adresu loopback
	service.sin_port = htons(27015);
	//z non blocking trzeba dac select, co by sprawdzic mozliwosc polaczenia
		
	iRes = connect(mainSocket,(SOCKADDR *) &service, sizeof(service)); // TO JEST KURNA WAZNE
	FD_ZERO(&readset); // cos nowego
	FD_SET(mainSocket, &readset);
	tv.tv_sec = 10;
	tv.tv_usec = 500000;
	
	iRes = select(mainSocket + 1, &readset, NULL, NULL, &tv);
	
	
	
	
	
	/*if (iRes == SOCKET_ERROR)
	{
		printf("couldnt connect.\n");
		getch();
		WSACleanup();
		return 1;
	}
	*/
	if(iRes == -1 || iRes == 0)
	{
		printf("error connecting");
		WSACleanup();
		return 1;
	}
	else
	{
		printf("all gut.\n");
	}
	printf("You got connected, say hello.\n");
	//mainSocket = acceptSocket
	printf("Press esc to leave.\n");
	//bytesSent = send(mainSocket, sendbuf, bufflen, 0);
	//while (getch()!= '\x1B')
	while(1)
	{
		bytesRecv = recv(mainSocket, recvbuf, bufflen, 0);
		printf("Server: %s.\n", recvbuf);
		if (bytesRecv == 0 || bytesRecv == WSAECONNRESET)
		{
			printf("gracefully closed.\n");
			WSACleanup;
			printf("session ended.\n");
			getch();
			return 1;
		}
		gets(sendbuf);		
		bytesSent = send(mainSocket, sendbuf, bufflen, 0);
		if (bytesSent == WSAEMSGSIZE)
		{
			printf("message too long, not sent.\n");
		}
		memset(&recvbuf,'\0',BUFFER_LENGTH);
		memset(&sendbuf,'\0',BUFFER_LENGTH);
	}
	printf("session ended.\n");
	WSACleanup(); // end
	getch();
	return 0;
}
