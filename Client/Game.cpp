/*
 * Game.cpp
 *
 *  Created on: 14/11/2015
 *      Author: ger
 */

#include "Game.h"

namespace std {

Game::Game() {
	tiempo_actual= 0;
	tiempo_viejo = 0;
	gameController = new GameControllerCliente();
	interprete = new Interprete(gameController);
	myClient = NULL;
	vista = NULL;
}
bool Game::init(MySocket* socket, string userName, string raza){

	myClient = socket;

	Yaml* reader = new Yaml("YAML/configuracionCliente.yaml");
	Juego* juego = reader->readCliente();
	delete reader;

	juego->escenario->jugador->nombre = userName;
	juego->escenario->jugador->raza = raza;

	if (!juego){
		error = " No se pudo cargar los datos del juego (revise Log.txt)";
		return false;
	}

	gameController->insertarJuego(juego);

	gameController->crearModelo();

	vista = new Vista(gameController->devolverModelo(), gameController);
	vista->init();
	interprete->setVista(vista);

	if (!establecerLogin()) return false;

	recibirParametrosConfiguracionYMapa();

	recibirInicializacionMapa();

	vista->loadMedia();

	return true;
}

void Game::jugar(){
	tiempo_viejo = SDL_GetTicks();
	bool fin;
	while (1) {

		if (myClient->isConnected() == false) {
			return ;
		}
		obtenerActualizacionesDelServer();

		fin = vista->run();

		if (fin) {
			msg_t quit = interprete->getQuit();
			enviarAccion(quit);
			break;
		}
		if (gameController->hayEventos()) {

			msg_t mensaje = gameController->sacarMensaje();
			enviarAccion(mensaje);
		} else {

			enviarKeepAlive();
		}
		usleep((40 - (tiempo_actual - tiempo_viejo)) * 1000);
		tiempo_actual = SDL_GetTicks();
		tiempo_viejo = tiempo_actual;
	}
}

void Game::recibirInicializacionMapa(){
	msg_t mensaje = myClient->recieveMessage();
	while ((mensaje.type != FIN_INICIALIZACION) && (myClient->isConnected())) {
		mensaje = myClient->recieveMessage();
		interprete->procesarMensajeDeServer(mensaje);
	}
}

void Game::recibirParametrosConfiguracionYMapa(){
	msg_t mensaje = myClient->recieveMessage();
	interprete->procesarMensajeDeServer(mensaje);

	mensaje = myClient->recieveMessage();
	interprete->procesarMensajeDeServer(mensaje);

}
void Game::enviarKeepAlive() {

	msg_t messageToServer = interprete->getKeepAliveMsg();

	myClient->sendMessage(messageToServer);

}
bool Game::establecerLogin() {

	msg_t messageToServer = interprete->getLoginMsg(interprete->getNombreJugador());

	myClient->sendMessage(messageToServer);

	msg_t msgFromSrv = myClient->recieveMessage();

	if (msgFromSrv.type == ERROR_NOMBRE_TOMADO) {
		error = " El nombre de usuario ya fue tomado por otro jugador";
		return false;
	} else {
		return true;
	}
}

void Game::enviarAccion(msg_t msg) {

	myClient->sendMessage(msg);
}

void Game::obtenerActualizacionesDelServer() {
	msg_t msgFromSrv;

	msgFromSrv = myClient->recieveMessage();
	while ((myClient->isConnected() == true) && (msgFromSrv.type != KEEPALIVE)) {
		interprete->procesarMensajeDeServer(msgFromSrv);
		msgFromSrv = myClient->recieveMessage();
	}
}
string Game::getStringError(){
	return this->error;
}
Game::~Game() {
	if (interprete) delete interprete;
	if (vista) delete vista;
}
}
