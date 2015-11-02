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
string Interprete::getNombreJugador(){
	return this->gameCtrl->nombreJugador();
}
msg_t Interprete::getLoginMsg(string nombre){
	msg_t mensajeLogin;
	memcpy(mensajeLogin.paramNombre,string_to_char_array(nombre),sizeof(mensajeLogin.paramNombre));
	return mensajeLogin;
}

void Interprete::procesarMensajeDeServer(msg_t msg){
	//printf("type %d\n",msg.type);
	dibujo_t dibujo;
	switch (msg.type) {

	case KEEPALIVE:
		//no se hace nada
		break;

	case MOVER_PERSONAJE:

		this->gameCtrl->mover_personaje(msg.paramNombre, msg.paramDouble1, msg.paramDouble2);
		break;

	case LOGIN:
		//TODO SACAR EL HARCODEO
		//meto algo que se supone que no crea si ya esta hecho
		printf("\n Login\n");
		dibujo=this->vista->crearPersonaje("soldado");
		printf("Dibujo %d \n",dibujo);
		this->gameCtrl->conectarCliente(msg.paramNombre, "soldado", msg.paramDouble1, msg.paramDouble2,dibujo);
		this->vista->setear_referencia(msg.paramDouble1,msg.paramDouble2);
		break;
	case NUEVO_PERSONAJE:
		//TODO SACAR EL HARCODEO
		printf("crear Personaje\n");
		dibujo=this->vista->crearPersonaje("soldado");
		if(this->gameCtrl->esNombre(msg.paramNombre)){
			printf("Es el mismo \n");
			printf("Referencia x: %g",msg.paramDouble1);
			printf("Referencia y: %g",msg.paramDouble2);
			printf((this->vista!=NULL) ? "Vista Existe": "Vista No Existe");
			//this->vista->setear_referencia(msg.paramDouble1,msg.paramDouble2);
		}
		this->gameCtrl->conectarCliente(msg.paramNombre, "soldado", msg.paramDouble1, msg.paramDouble2,dibujo);
		printf("creado\n");
		break;

	case QUIT:
		printf("recibe QUIT\n");
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

		this->gameCtrl->juego->escenario->size_x = msg.paramDouble1;
		this->gameCtrl->juego->escenario->size_y = msg.paramDouble2;
		break;
	case CONFIGURACION:

		this->gameCtrl->setConfiguracion((int)msg.paramDouble1,(int)msg.paramDouble2);
		break;
	case DISCONNECT:

		printf("manda DISCONNECT");

		this->gameCtrl->desconectar(msg.paramNombre);
		break;
	case ACTUALIZACION_RECURSOS:
		if(this->gameCtrl->nombreJugador()==msg.paramNombre){
			this->gameCtrl->acutalizarRecursos(msg.paramInt1,msg.paramDouble1,msg.paramDouble2);
		}
		break;
	case FIN_INICIALIZACION:
		break;

	default:
		break;
	}
}

Interprete::~Interprete() {
	delete this->gameCtrl;
}
