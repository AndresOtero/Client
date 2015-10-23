/*
 * Interprete.h
 *
 *  Created on: 21/10/2015
 *      Author: ger
 */

#ifndef INTERPRETE_H_
#define INTERPRETE_H_

#include <string>
#include <string>
#include "GameControllerClient.h"

using namespace std;

class Interprete {
public:
	Interprete();
	string getQuit();
	bool isQuit(string& quit);
	string getKeepAliveMsg();
	string getLoginMsg();
	string getReceivedMsg();

	string getActualizarRecursosMsg(int oro, int madera,int piedra);

	void notifyUpdate(string& msg);
	bool isFinalUpdates(string& msg);

	virtual ~Interprete();

private:
	GameControllerClient* gameCtrl;
};

#endif /* INTERPRETE_H_ */
