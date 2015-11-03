#include "MySocket.h"
#include <SDL2/SDL.h>
#include <cstdio>
#include "Interprete.h"
#include <iostream>
#include <unistd.h>
#include "Yaml.h"
#include "GameControllerSrc/mensaje.h"
#include "VistaSrc/Vista.h"
#include "ModeloSrc/Modelo.h"
#include "ModeloSrc/Juego.h"
#include <plog/Log.h>

using namespace std;

void enviarKeepAlive(MySocket* myClient, Interprete* interprete){

	msg_t messageToServer = interprete->getKeepAliveMsg();

	myClient->sendMessage(messageToServer);

}
void establecerLogin(MySocket* myClient, Interprete* interprete,string nombre){

	msg_t messageToServer = interprete->getLoginMsg(interprete->getNombreJugador());

	myClient->sendMessage(messageToServer);
}

void enviarAccion(MySocket* myClient, msg_t msg){

	myClient->sendMessage(msg);
}

void obtenerActualizacionesDelServer(MySocket* myClient, Interprete* interprete) {
	msg_t msgFromSrv;

	msgFromSrv = myClient->recieveMessage();
	while ((myClient->isConnected() == true) && (msgFromSrv.type != KEEPALIVE)){
		interprete->procesarMensajeDeServer(msgFromSrv);
		msgFromSrv = myClient->recieveMessage();
	}
}


int main(int argc, char *argv[])
{
	plog::init(plog::warning, "Log.txt");
	double tiempo_actual,tiempo_viejo=0;
	bool enviarAlive;
	msg_t msgToSrv;
	GameControllerCliente * gameController = new GameControllerCliente();
	Interprete interprete(gameController);
	bool reiniciar=true;

	MySocket myClient(PORTNUM);


	//lee el YAML antes de cargar el usuario y el modelo
	Yaml* reader = new Yaml("YAML/configuracionCliente2.yaml");
	Juego* juego = reader->readCliente();
	delete reader;
	if(!juego)return -1;//No se crea el jugadors
	gameController->insertarJuego(juego);



	myClient.connectToServer(gameController->ipJugador().c_str());
	establecerLogin(&myClient, &interprete,gameController->nombreJugador());

	//recibe parametros mapa y configuracion

	msg_t mensaje = myClient.recieveMessage();
	interprete.procesarMensajeDeServer(mensaje);

	mensaje = myClient.recieveMessage();
	interprete.procesarMensajeDeServer(mensaje);
	gameController->crearModelo();
	//Inicia vista
	Modelo *modelo = gameController->devolverModelo();
		Vista* vista=new Vista(modelo,gameController);
		vista->init();
		interprete.setVista(vista);

	mensaje = myClient.recieveMessage();
	while ((mensaje.type != FIN_INICIALIZACION) && (myClient.isConnected()) ) {
		mensaje = myClient.recieveMessage();
		interprete.procesarMensajeDeServer(mensaje);
	}
	LOG_WARNING << "Cargo datos del Server";

	vista->loadMedia();


	//comienza a jugar
	tiempo_viejo=SDL_GetTicks();
	bool fin;
	while (1)
	{

		if (myClient.isConnected() == false){
				LOG_WARNING << "Desconexcion del server";
				printf("desconexion del servidor \n");
				//vista->serverDisconnect();
				return 0;
		}
	   obtenerActualizacionesDelServer(&myClient, &interprete);
	   //printf("Mando a dibujar\n");
	   fin = vista->run();
	   //printf("Dibuja Ok \n");
	   if (fin){
		   msg_t quit = interprete.getQuit();
		   enviarAccion(&myClient,quit);
		   break;
	   }
	   //printf("No hay fin \n");
	   if (gameController->hayEventos()){
		   //printf("hay evento\n");
		   msg_t mensaje = gameController->sacarMensaje();
   		   enviarAccion(&myClient,mensaje);
	   }
	   else{
		   enviarKeepAlive(&myClient,&interprete);
	   }
	   //printf("Pasa los eventos \n");
	   usleep((40 - (tiempo_actual-tiempo_viejo))*1000);
	   tiempo_actual= SDL_GetTicks();
	   tiempo_viejo=tiempo_actual;
	}
	delete vista;
	delete gameController;
    return 0;
}


