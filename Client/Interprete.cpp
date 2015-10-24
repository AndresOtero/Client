/*
 * Interprete.cpp
 *
 *  Created on: 21/10/2015
 *      Author: ger
 */

#include "Interprete.h"

Interprete::Interprete() {
	this->gameCtrl = new GameControllerClient();
}
msg_t Interprete:: getKeepAliveMsg(){
	msg_t r = {KEEPALIVE,"",0,0};
	return r;
}
msg_t Interprete:: getQuit(){
	msg_t r = {QUIT,"",0,0};
	return r;
}
bool Interprete::isQuit(msg_t quit){
	return (quit.type == QUIT);
}
msg_t Interprete::getLoginMsg(){
	msg_t r = {LOGIN,"TROYANO",0,0};
	return r;
}

void Interprete:: notifyUpdate(msg_t msg){
//aca hacer la decodificacion de msgs y ejecutar el comando del gameCtrl;
}
Interprete::~Interprete() {

}

