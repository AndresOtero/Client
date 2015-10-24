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
#include "mensaje.h"

using namespace std;

class Interprete {
public:
	Interprete();

	msg_t getKeepAliveMsg();

	msg_t getQuit();
	bool isQuit(msg_t quit);

	msg_t getLoginMsg();

	void notifyUpdate(msg_t msg);

	virtual ~Interprete();

private:
	GameControllerClient* gameCtrl;
};

#endif /* INTERPRETE_H_ */
