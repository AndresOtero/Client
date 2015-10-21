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
string Interprete:: getKeepAliveMsg(){
	return string("ALIVE");
}
string Interprete:: getReceivedMsg(){
	return string("OK");
}
string Interprete:: getQuit(){
	return string("QUIT");
}
bool Interprete::isQuit(string& quit){
	return (quit.compare("QUIT") == 0);
}
string Interprete::getLoginMsg(){
	string result = string("LOGIN ");
	string name = string("name");//ir a buscar esto al game controller
	result += name;

	return result;
}

string Interprete:: getActualizarRecursosMsg(int oro, int madera,int piedra){
	char buffer[50];

	sprintf(buffer,"RECURSOS %10d %10d %10d",oro,madera,piedra);

	string result = string(buffer);

	return result;
}

bool  Interprete::isFinalUpdates(string& msg){

	printf("%s",msg.c_str());

	if(msg.compare(string("ALIVE")) == 0){
		return true;
	}
	return false;
}

void Interprete:: notifyUpdate(string& msg){
//aca hacer la decodificacion de msgs y ejecutar el comando del gameCtrl;
}

Interprete::~Interprete() {

}

