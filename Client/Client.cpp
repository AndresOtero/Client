#include "MySocket.h"
#include <SDL2/SDL.h>
#include <cstdio>
#include "Interprete.h"
#include <iostream>
#include <unistd.h>
#include "Yaml.h"
#include "VistaSrc/Vista.h"
#include "ModeloSrc/Modelo.h"
#include "ModeloSrc/Juego.h"
using namespace std;

void enviarKeepAlive(MySocket* myClient, Interprete* interprete){

	msg_t messageToServer = interprete->getKeepAliveMsg();

	myClient->sendMessage(messageToServer);

}
void establecerLogin(MySocket* myClient, Interprete* interprete){

	msg_t messageToServer = interprete->getLoginMsg();

	myClient->sendMessage(messageToServer);
}

void enviarAccion(MySocket* myClient, msg_t msg){

	myClient->sendMessage(msg);
}

void obtenerActualizacionesDelServer(MySocket* myClient, Interprete* interprete) {

	msg_t msgFromSrv = myClient->recieveMessage();

	if (myClient->isConnected() == true){
		cout << "Server: Recive actualizacion de tipo: " << msgFromSrv.type << "\n";

		interprete->notifyUpdate(msgFromSrv);
	}
}


int main(int argc, char *argv[])
{
	double tiempo_actual,tiempo_viejo=0;
	bool enviarAlive;
	msg_t msgToSrv;

	Interprete interprete;
	bool reiniciar=true;
	while (reiniciar){
			Yaml* reader=new Yaml("YAML/configuracionCliente.yaml");
			Juego* juego = reader->readCliente();
			delete reader;
			juego->agregarEntidad("barraca",0,0);
			juego->setEscenario("Orleans",100,100);
			juego->setConfiguracion(200,1);
			Modelo* modelo=new Modelo(juego);
			Vista* vista=new Vista(modelo);
			vista->init();
			vista->loadMedia();
			reiniciar = vista->run();
			delete modelo;
			delete vista;
	}
	MySocket myClient(PORTNUM);
	myClient.connectToServer("127.0.0.1");

	establecerLogin(&myClient, &interprete);

	tiempo_viejo=SDL_GetTicks();

	while (1)
	{
		enviarAlive = true; //poner en false si mando otra cosa

		if (myClient.isConnected() == false){
				printf("desconexion del servidor \n");
				//myClient.reconnectToServer(); TE DEJO ACA PARA QUE VEAS COMO RECONECTAR ANTE UN MENSAJE DEL USUARIO
		}
	   obtenerActualizacionesDelServer(&myClient, &interprete);
	   //aca va toda la corrida del juego, puede devolver un string con el evento a notificar

	   if (enviarAlive ){
		   enviarKeepAlive(&myClient,&interprete);
	   }else{
		   enviarAccion(&myClient, msgToSrv);
	   }

	   usleep((40 - (tiempo_actual-tiempo_viejo))*1000);
	   tiempo_actual= SDL_GetTicks();
	   tiempo_viejo=tiempo_actual;
	}
    return 0;
}


