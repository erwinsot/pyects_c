#include <stdio.h>
#include<sys/types.h>
#include <winsock2.h>
#include <string.h>
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <windows.h>
#include <unistd.h>
#include <stdlib.h>

char SendBuff[512],RecvBuff[512];

int main(int argc, char *argv[]){

  WSADATA wsaData;
  SOCKET conn_socket,comm_socket;
  SOCKET comunicacion;
  struct sockaddr_in server;
  struct sockaddr_in client;
  struct hostent *hp;
  int resp,stsize;
  
  //Inicializamos la DLL de sockets
  resp=WSAStartup(MAKEWORD(1,0),&wsaData);
  if(resp){
    printf("Error al inicializar socket\n");
    getchar();return resp;
  }
  
  //Obtenemos la IP que usará nuestro servidor... 
  // en este caso localhost indica nuestra propia máquina...
  hp=(struct hostent *)gethostbyname("localhost");

  if(!hp){
    printf("No se ha encontrado servidor...\n");
    getchar();WSACleanup();return WSAGetLastError();
  }

  // Creamos el socket...
  conn_socket=socket(AF_INET,SOCK_STREAM, 0);
  if(conn_socket==INVALID_SOCKET) {
    printf("Error al crear socket\n");
    getchar();WSACleanup();return WSAGetLastError();
  }
  
  memset(&server, 0, sizeof(server)) ;
  memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
  server.sin_family = hp->h_addrtype;
  server.sin_port = htons(6000);

  // Asociamos ip y puerto al socket
  resp=bind(conn_socket, (struct sockaddr *)&server, sizeof(server));
  if(resp==SOCKET_ERROR){
    printf("Error al asociar puerto e ip al socket\n");
    closesocket(conn_socket);WSACleanup();
    getchar();return WSAGetLastError();
  }

  if(listen(conn_socket, 1)==SOCKET_ERROR){
    printf("Error al habilitar conexiones entrantes\n");
    closesocket(conn_socket);WSACleanup();
    getchar();return WSAGetLastError();
  }
              
  // Aceptamos conexiones entrantes
  printf("Esperando conexiones entrantes... \n");
  stsize=sizeof(struct sockaddr);
  comm_socket=accept(conn_socket,(struct sockaddr *)&client,&stsize);
  if(comm_socket==INVALID_SOCKET){
    printf("Error al aceptar conexión entrante\n");
    closesocket(conn_socket);WSACleanup();
    getchar();return WSAGetLastError();
  }
  printf("Conexión entrante desde: %s\n", inet_ntoa(client.sin_addr));
              
  // Como no vamos a aceptar más conexiones cerramos el socket escucha
  closesocket(conn_socket);
  char *buf;
  buf=(char *)malloc(100*sizeof(char));
  getcwd(buf,100);
  printf("\n %s \n",buf);
  char senbuff2[512];
  //buf2=(char *)malloc(1000*sizeof(char));
  printf("datos de entrada\n");
  scanf("%s",&senbuff2);
  //strcry(buf,&buf2);
  //printf("es %s\n",&buf2);
  //send (comm_socket, buf2, sizeof(buf2), 0);
  strcpy(SendBuff,senbuff2);
  //Enviamos y recibimos datos...
  printf("Enviando Mensaje... \n");
  send (comm_socket, SendBuff, sizeof(SendBuff), 0);
  printf("Datos enviados: %s \n", SendBuff);
  
  printf("Recibiendo Mensaje... \n");
  recv (comm_socket, RecvBuff, sizeof(RecvBuff), 0);
  printf("Datos recibidos: %s \n", RecvBuff);
  
  getchar();            

  // Cerramos el socket de la comunicacion
  closesocket(comm_socket);
  
  // Cerramos liberia winsock
  WSACleanup();
  return (EXIT_SUCCESS);
} 