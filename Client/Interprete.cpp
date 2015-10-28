/*
 * Interprete.cpp
 *
 *  Created on: 21/10/2015
 *      Author: ger
 */

#include "Interprete.h"

char* Interprete::string_to_char_array(string str){
	int str_size = str.size();
	char charArray[PARAM_STRING_LEN];
	for (int a = 0; a <= str_size; a++) {
		charArray[a] = str[a];
	}
	return charArray;
}

Interprete::Interprete() {
	this->gameCtrl = new GameControllerCliente();
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
	msg_t r;
	r.type = LOGIN;
	strcpy(r.paramNombre, string_to_char_array(nombre));
	return r;
}

void Interprete::procesarMensajeDeServer(msg_t msg){
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
		this->gameCtrl->agregarEntidad(msg.paramNombre, msg.paramDouble1, msg.paramDouble2, msg.paramInt1);
		break;
	case RECONNECT:
		this->gameCtrl->reconectar(msg.paramNombre);
		break;
	default:
		break;
	}
}

Interprete::~Interprete() {

}
