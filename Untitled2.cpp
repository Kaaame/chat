#include <cstdio>
#include <cstdlib>
#include <conio.h> //dla getch
//#include <string>
//included win_32.a
//to dziala bardziej jako serwer

#include <winsock2.h>

#define BUFFER_LENGTH 1024
#define PORT 27015

//using std::string;

int main()
{
	WSADATA wsaData;
	int iRes = WSAStartup(MAKEWORD(2,2), &wsaData);
	u_long iMode = 1;
	
	
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
	if(iRes != NO_ERROR)
	{
		printf("ioctlsocket error.\n");
		WSACleanup();
		return 1;
	}
	
	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY; //sin_addr struktura zawierajaca s_addr, s_addr moze byc kilka, kilka adresow, niepotrzebne dla adresu loopback
	//service.sin_addr.s_addr = inet_addr("79.135.184.170"); //
	service.sin_port = htons(27015);
	if(bind(mainSocket,(SOCKADDR *) &service,sizeof(service)) == SOCKET_ERROR)
	{
		printf("couldnt bind address to socket.\n");
		closesocket(mainSocket);
		return 1;
	}
	if (listen(mainSocket,1) == SOCKET_ERROR) printf("couldnt listen on socket");
	SOCKET acceptSocket = SOCKET_ERROR;// = SOCKET_ERROR;
	printf("waiting for someone to connect.\n");
	while(acceptSocket == SOCKET_ERROR) //petla do zaakcepotwania polaczenia
	{
		acceptSocket = accept(mainSocket,NULL,NULL);
	}
	
	printf("Client connected, say hello.\n");
	//closesocket(mainSocket); //some experimental stuff here nigga, if mainSocket = acceptSocket, then can i work on acceptSocket?
	mainSocket = acceptSocket;
	//bytesRecv = recv(mainSocket, recvbuf, bufflen, 0);
	printf("Press esc to leave.\n");
	//while (bytesRecv!=0);
	printf("write something.\n");
	while(1)
	{
		gets(sendbuf);//,BUFFER_LENGTH,stdin); DLA PLIKU DALNIE, fgets mozna uzywac, pierdolil
		bytesSent = send(mainSocket, sendbuf, bufflen, 0);//drugi gosciu musi odbierac
		if(bytesSent == WSAEMSGSIZE) 
		{
			printf("message too long, not sent.\n");
		}			
		
		bytesRecv = recv(mainSocket, recvbuf, bufflen, 0);
		printf("Client: %s.\n", recvbuf);
		
		if (bytesRecv == 0)
		{
			printf("gracefully closed, client exited");
			WSACleanup();
			return 1;
		}
		//bytesRecv = SOCKET_ERROR?
		memset(&recvbuf,'\0',BUFFER_LENGTH);
		memset(&sendbuf,'\0',BUFFER_LENGTH);
	}
	
	WSACleanup(); // end
	return 0;
}
