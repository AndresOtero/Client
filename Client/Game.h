/*
 * Game.h
 *
 *  Created on: 14/11/2015
 *      Author: ger
 */

#ifndef GAME_H_
#define GAME_H_

#include <GameControllerSrc/mensaje.h>
#include "GameControllerSrc/GameControllerCliente.h"
#include "Interprete.h"
#include <string>
#include "MySocket.h"
#include "Yaml.h"

namespace std {

class Game {
public:
	Game();
	void jugar();
	string getStringError();
	bool init(MySocket* myClient, string userName, string raza);
	virtual ~Game();

private:
	Vista* vista;
	MySocket* myClient;
	GameControllerCliente * gameController;
	Interprete* interprete;
	string error;

	double tiempo_actual, tiempo_viejo;
	void obtenerActualizacionesDelServer();
	void enviarAccion( msg_t msg);
	bool establecerLogin();
	void enviarKeepAlive();
	void recibirInicializacionMapa();
	void recibirParametrosConfiguracionYMapa();
};

}

#endif /* GAME_H_ */
