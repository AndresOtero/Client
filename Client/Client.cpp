#include "MySocket.h"
#include "ClientConnectionView.h"
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

void enviarKeepAlive(MySocket* myClient, Interprete* interprete) {

	msg_t messageToServer = interprete->getKeepAliveMsg();

	myClient->sendMessage(messageToServer);

}
bool establecerLogin(MySocket* myClient, Interprete* interprete, string nombre) {

	msg_t messageToServer = interprete->getLoginMsg(interprete->getNombreJugador());

	myClient->sendMessage(messageToServer);

	msg_t msgFromSrv = myClient->recieveMessage();

	if (msgFromSrv.type == ERROR_NOMBRE_TOMADO) {
		return false;
	} else {
		return true;
	}
}

void enviarAccion(MySocket* myClient, msg_t msg) {

	myClient->sendMessage(msg);
}

void obtenerActualizacionesDelServer(MySocket* myClient, Interprete* interprete) {
	msg_t msgFromSrv;

	msgFromSrv = myClient->recieveMessage();
	while ((myClient->isConnected() == true) && (msgFromSrv.type != KEEPALIVE)) {
		interprete->procesarMensajeDeServer(msgFromSrv);
		msgFromSrv = myClient->recieveMessage();
	}
}

int main(int argc, char *argv[]) {

	ClientConnectionView clientConnectionView;

	clientConnectionView.showForm();

	int portNumber= clientConnectionView.getPort();
	string IPserver = clientConnectionView.getIp();
	string userName = clientConnectionView.getUsername();

	double tiempo_actual, tiempo_viejo = 0;
	GameControllerCliente * gameController = new GameControllerCliente();
	Interprete interprete(gameController);

	MySocket myClient(portNumber);

	Yaml* reader = new Yaml("YAML/configuracionCliente.yaml");
	Juego* juego = reader->readCliente();
	delete reader;

	if (!juego)
		return -1;

	gameController->insertarJuego(juego);

	myClient.connectToServer(IPserver.c_str());

	if (myClient.isConnected()) {

		bool nombreDeUsuarioDisponible = establecerLogin(&myClient, &interprete, gameController->nombreJugador());

		if (nombreDeUsuarioDisponible) {
			//recibe parametros mapa y configuracion
			msg_t mensaje = myClient.recieveMessage();
			interprete.procesarMensajeDeServer(mensaje);

			mensaje = myClient.recieveMessage();
			interprete.procesarMensajeDeServer(mensaje);

			gameController->crearModelo();

			//Inicia vista
			printf("Inicio vista\n");
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
		}
	}
	delete gameController;

	return 0;
}

