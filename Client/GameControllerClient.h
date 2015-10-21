/*
 * GameControllerClientMock.h
 *
 *  Created on: Oct 21, 2015
 *      Author: gaston
 */

#ifndef GAMECONTROLLERCLIENT_H_
#define GAMECONTROLLERCLIENT_H_
#include <string>
#include <cstdio>

using namespace std;

class GameControllerClient {

public:
	GameControllerClient();

	void actualizarJuego(string evento);

	string obtenerSiguienteEvento();

	bool hayNuevosEventos();

	virtual ~GameControllerClient();
};

#endif /* GAMECONTROLLERCLIENT_H_ */
