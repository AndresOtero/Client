/*
 * GameControllerClientMock.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: gaston
 */

#include "GameControllerClient.h"

GameControllerClient::GameControllerClient() {

}

void GameControllerClient::actualizarJuego(string evento){
	printf("Actualizando modelo en cliente: %s",evento.c_str());
}



string GameControllerClient::obtenerSiguienteEvento(){
	//TODO GameController podria tener una cola de eventos donde se van agregando, y se saca el ultimo de ahi.
	string siguienteEvento = "Mover id:1 pos:(5,3)";

	return siguienteEvento;
}

bool GameControllerClient::hayNuevosEventos(){
	//TODO GameController podria tener una cola de eventos y checkear si hay nuevos eventos para mandar.
	return true;
}

GameControllerClient::~GameControllerClient() {

}

