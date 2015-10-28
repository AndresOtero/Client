/*
 * Interprete.cpp
 *
 *  Created on: 21/10/2015
 *      Author: ger
 */

#include "Interprete.h"
#include <math.h>


char* Interprete::string_to_char_array(string str){
	int str_size = str.size();
	char charArray[PARAM_STRING_LEN];
	for (int a = 0; a <= str_size; a++) {
		charArray[a] = str[a];
	}
	return charArray;
}

Interprete::Interprete(GameControllerCliente* gm) {
	this->gameCtrl = gm;
}
msg_t Interprete:: getKeepAliveMsg(){
	msg_t r;
	r.type = KEEPALIVE;
	return r;
}
msg_t Interprete:: getQuit(){
	msg_t r;
	r.type = QUIT;
	return r;
}
bool Interprete::isQuit(msg_t quit){
	return (quit.type == QUIT);
}
msg_t Interprete::getLoginMsg(string nombre){
	msg_t mensajeLogin;
	memcpy(mensajeLogin.paramNombre,string_to_char_array(nombre),sizeof(mensajeLogin.paramNombre));
	return mensajeLogin;
}

void Interprete::procesarMensajeDeServer(msg_t msg){
	printf("type %d\n",msg.type);
	switch (msg.type) {

	case KEEPALIVE:
		//no se hace nada
		break;

	case MOVER_PERSONAJE:
		this->gameCtrl->mover_personaje(msg.paramNombre, msg.paramDouble1, msg.paramDouble2);
		break;

	case QUIT:
		this->gameCtrl->desconectar(msg.paramNombre);
		break;
	case CREAR_ENTIDAD:
		this->gameCtrl->agregarEntidad(msg.paramNombre, msg.paramDouble1, msg.paramDouble2, 0);
		break;
	case CREAR_RECURSO:
		this->gameCtrl->agregarEntidad(msg.paramNombre, msg.paramDouble1,msg.paramDouble2,msg.paramInt1);
		break;
	case RECONNECT:
		this->gameCtrl->reconectar(msg.paramNombre);
		break;
	case PARAM_MAPA:
		printf("llega\n");
		this->gameCtrl->juego->escenario->size_x = msg.paramDouble1;
		this->gameCtrl->juego->escenario->size_y = msg.paramDouble2;
		break;
	case CONFIGURACION:
		this->gameCtrl->setConfiguracion((int)msg.paramDouble1,(int)msg.paramDouble2);
		break;
	case DISCONNECT:
		this->gameCtrl->desconectar(msg.paramNombre);
		break;
	default:
		break;
	}
}

Interprete::~Interprete() {
	delete this->gameCtrl;
}
