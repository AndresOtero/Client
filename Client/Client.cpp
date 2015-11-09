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
bool establecerLogin(MySocket* myClient, Interprete* interprete,string nombre){

	msg_t messageToServer = interprete->getLoginMsg(interprete->getNombreJugador());

	myClient->sendMessage(messageToServer);

	msg_t msgFromSrv = myClient->recieveMessage();

	if (msgFromSrv.type == ERROR_NOMBRE_TOMADO){
		printf("Usuario con nombre de usuario ya tomado \n");
		return false;
	}else{
		printf("Usuario conectado con el nombre %s \n", interprete->getNombreJugador().c_str());
		return true;
	}
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
	//plog::init(plog::warning, "Log.txt");
	double tiempo_actual,tiempo_viejo=0;
	bool enviarAlive;
	msg_t msgToSrv;
	GameControllerCliente * gameController = new GameControllerCliente();
	Interprete interprete(gameController);
	bool reiniciar=true;

	MySocket myClient(PORTNUM);


	//lee el YAML antes de cargar el usuario y el modelo
	printf("Comienza a leer archivo\n");
	Yaml* reader = new Yaml("YAML/configuracionCliente2.yaml");
	Juego* juego = reader->readCliente();
	printf("Lee archivo\n");
	delete reader;
	if(!juego)return -1;//No se crea el jugadors
	gameController->insertarJuego(juego);

	printf("Inserta juego\n");
	myClient.connectToServer(gameController->ipJugador().c_str());
	if(myClient.isConnected()){
		printf("Usuario conectado \n");
		bool nombreDeUsuarioDisponible = establecerLogin(&myClient, &interprete,
				gameController->nombreJugador());

		if (nombreDeUsuarioDisponible) {
			//recibe parametros mapa y configuracion
			msg_t mensaje = myClient.recieveMessage();
			interprete.procesarMensajeDeServer(mensaje);

			mensaje = myClient.recieveMessage();
			interprete.procesarMensajeDeServer(mensaje);
			gameController->crearModelo();
			//Inicia vista
			Modelo *modelo = gameController->devolverModelo();
			Vista* vista = new Vista(modelo, gameController);
			vista->init();
			interprete.setVista(vista);


			mensaje = myClient.recieveMessage();
			while ((mensaje.type != FIN_INICIALIZACION) && (myClient.isConnected())) {
				mensaje = myClient.recieveMessage();
				interprete.procesarMensajeDeServer(mensaje);
			}

			vista->loadMedia();

			//comienza a jugar
			tiempo_viejo = SDL_GetTicks();
			bool fin;
			while (1) {

				if (myClient.isConnected() == false) {
					//LOG_WARNING << "Desconexcion del server";

					return 0;
				}
				obtenerActualizacionesDelServer(&myClient, &interprete);
				fin = vista->run();
				if (fin) {
					msg_t quit = interprete.getQuit();
					enviarAccion(&myClient, quit);
					break;
				}
				if (gameController->hayEventos()) {
					msg_t mensaje = gameController->sacarMensaje();
					enviarAccion(&myClient, mensaje);
				} else {
					enviarKeepAlive(&myClient, &interprete);
				}
				usleep((40 - (tiempo_actual - tiempo_viejo)) * 1000);
				tiempo_actual = SDL_GetTicks();
				tiempo_viejo = tiempo_actual;
			}
			delete vista;
		}}
	delete gameController;

	return 0;
}

