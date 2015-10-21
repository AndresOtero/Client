#include "MySocket.h"
#include <SDL2/SDL.h>
#include <cstdio>
#include "Interprete.h"
#include <iostream>
#include <unistd.h>  // problem solved! it compiles!
using namespace std;

/*void enviarKeepAlive(MySocket* myClient, Interprete* interprete){

	string messageFromServer = "";

	string messageToServer = interprete->getKeepAliveMsg();

	myClient->sendMessage(messageToServer);

	myClient->recieveMessage(messageFromServer);
}*/

void establecerLogin(MySocket* myClient, Interprete* interprete){
	string messageFromServer = "";

	string messageToServer = interprete->getLoginMsg();

	myClient->sendMessage(messageToServer);

	myClient->recieveMessage(messageFromServer); //ok message

	printf("%s \n", messageFromServer.c_str());
}

void enviarAccion(MySocket* myClient, string & msg){
	string messageFromServer = "";

	myClient->sendMessage(msg);

	myClient->recieveMessage(messageFromServer); //ok message
}

void enviarConfirmReceived(MySocket* myClient, Interprete* interprete){
	string msgToSrv = interprete->getReceivedMsg();
	myClient->sendMessage(msgToSrv);
}

void obtenerActualizacionesDelServer(MySocket* myClient, Interprete* interprete) {
	string msgFromSrv = "";

	myClient->recieveMessage(msgFromSrv);

	if (myClient->isConnected() == true){

		enviarConfirmReceived(myClient,interprete);

		while ( !interprete->isFinalUpdates(msgFromSrv)){

			printf("confirmo \n");
			string msgFromSrv = "";
			myClient->recieveMessage(msgFromSrv);
			if (myClient->isConnected() == true){
				enviarConfirmReceived(myClient,interprete);

				interprete->notifyUpdate(msgFromSrv);
			}
		}

	}
}


int main(int argc, char *argv[])
{
	double tiempo_actual,tiempo_viejo=0;
	string msgToSrv, msgFromSrv;
	Interprete interprete;

	MySocket myClient(PORTNUM);  // create the client socket
	myClient.connectToServer("127.0.0.1");

	establecerLogin(&myClient, &interprete);

	tiempo_viejo=SDL_GetTicks();

	while (1)
	{
		if (myClient.isConnected() == false){
			   	printf("perdida de conexion\n");
		}

	   obtenerActualizacionesDelServer(&myClient, &interprete);

	   //aca va toda la corrida del juego, puede devolver un string con el evento a notificar

	   msgToSrv = interprete.getActualizarRecursosMsg(0,0,1);

	   enviarAccion(&myClient, msgToSrv);

	   usleep((40 - (tiempo_actual-tiempo_viejo))*1000);
	   tiempo_actual= SDL_GetTicks();
	   tiempo_viejo=tiempo_actual;
	}
    return 0;
}


