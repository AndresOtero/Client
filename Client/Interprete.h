/*
 * Interprete.h
 *
 *  Created on: 21/10/2015
 *      Author: ger
 */

#ifndef INTERPRETE_H_
#define INTERPRETE_H_
#include "GameControllerSrc/mensaje.h"
#include "GameControllerSrc/GameControllerCliente.h"
#include <string>
#include <string>
#include "VistaSrc/Vista.h"
#include <plog/Log.h>


using namespace std;

class Interprete {

public:
	Interprete(GameControllerCliente* gm) ;

	msg_t getKeepAliveMsg();

	msg_t getQuit();
	bool isQuit(msg_t quit);

	msg_t getLoginMsg(string nombre);

	void procesarMensajeDeServer(msg_t msg);

	string getNombreJugador();

	virtual ~Interprete();

	void setVista(Vista* vista) {this->vista = vista;}

	bool start;

private:
	GameControllerCliente* gameCtrl;
	Vista* vista;

	char* string_to_char_array(string str);

};

#endif /* INTERPRETE_H_ */
