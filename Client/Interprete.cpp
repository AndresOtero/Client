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

	memcpy(mensajeLogin.paramTipo,string_to_char_array(this->gameCtrl->juego->escenario->protagonista->objetoMapa->nombre), sizeof(mensajeLogin.paramTipo));

	memcpy(mensajeLogin.paramNombre,string_to_char_array(nombre),sizeof(mensajeLogin.paramNombre));
	return mensajeLogin;
}

void Interprete::procesarMensajeDeServer(msg_t msg){
	dibujo_t dibujo;
	Personaje* p;
	switch (msg.type) {

	case KEEPALIVE:
		break;

	case MOVER_PERSONAJE:
		printf("MOVER_PERSONAJE\n");
		this->gameCtrl->mover_personaje(msg.paramInt1, msg.paramDouble1, msg.paramDouble2);
		break;

	case LOGIN:

		printf("LOGIN\n");
		//TODO SACAR EL HARCODEO
		//meto algo que se supone que no crea si ya esta hecho
		 p=this->gameCtrl->conectarCliente(msg.paramNombre, msg.paramTipo, msg.paramDouble1, msg.paramDouble2,dibujo,msg.paramInt1);
		this->vista->crearPersonaje(string(msg.paramTipo),p);
		if (this->gameCtrl->esNombre(msg.paramNombre)) {
			this->vista->setear_referencia(msg.paramDouble1, msg.paramDouble2);
		}
		break;
	case NUEVO_PERSONAJE:
		printf("NUEVO PERSONAJE\n");
		//TODO SACAR EL HARCODEO
		p=this->gameCtrl->conectarCliente(msg.paramNombre, msg.paramTipo, msg.paramDouble1, msg.paramDouble2,dibujo,msg.paramInt1);
		this->vista->crearPersonaje(string(msg.paramTipo),p);
		printf("termino NUEVO PERSONAJE\n");
		break;

	case QUIT:
		printf("QUIT\n");

		this->gameCtrl->desconectar(msg.paramNombre);
		break;
	case CREAR_ENTIDAD:
		printf("CREAR_ENTIDAD\n");

		this->gameCtrl->agregarEntidad(msg.paramNombre, msg.paramDouble1, msg.paramDouble2, 0);
		//setear id de entidades
		this->gameCtrl->setId(msg.paramDouble1,msg.paramDouble2,msg.paramInt1);
		break;
	case CREAR_RECURSO:
		printf("CREAR_RECURSO\n");

		this->gameCtrl->agregarEntidad(msg.paramNombre, msg.paramDouble1,msg.paramDouble2,msg.paramInt1);
		break;
	case RECONNECT:
		printf("RECONNECT\n");

		this->gameCtrl->reconectar(msg.paramNombre);
		break;
	case PARAM_MAPA:
		printf("PARAM_MAPA\n");

		this->gameCtrl->juego->escenario->size_x = msg.paramDouble1;
		this->gameCtrl->juego->escenario->size_y = msg.paramDouble2;
		break;
	case CONFIGURACION:
		printf("CONFIGURACION\n");

		this->gameCtrl->setConfiguracion((int)msg.paramDouble1,(int)msg.paramDouble2);
		break;
	case DISCONNECT:
		printf("DISCONNECT\n");

		this->gameCtrl->desconectar(msg.paramNombre);
		break;
	case ACTUALIZACION_RECURSOS:
		printf("ACTUALIZACION_RECURSOS\n");
		this->gameCtrl->acutalizarRecursos(msg.paramNombre,msg.paramInt1,msg.paramDouble1,msg.paramDouble2);

		break;
	case SET_ID_RECURSO:
		printf("SET_ID_RECURSO\n");

		this->gameCtrl->setId(msg.paramDouble1,msg.paramDouble2,msg.paramInt1);
		break;
	case ELIMINAR_ENTIDAD:
		printf("ELIMINAR_ENTIDAD\n");

		this->gameCtrl->eliminarEntidad(msg.paramInt1);
		break;
	case FIN_INICIALIZACION:
		printf("FIN_INICIALIZACION\n");

		break;
	case ATACAR:
		printf("ATACAR\n");

		this->gameCtrl->ataque(msg.paramInt1,msg.paramDouble1,msg.paramDouble2);
		break;
	case ELIMINAR_PERSONAJE:
		printf("ELIMINAR_PERSONAJE\n");

		this->gameCtrl->eliminar_personaje(msg.paramInt1);
		break;
	case ELIMINAR:
		printf("ELIMINAR\n");

		this->gameCtrl->eliminar(msg.paramInt1);
		break;
	case CONSTRUIR:
		printf("CONSTRUIR\n");

		this->gameCtrl->construir(msg.paramInt1,msg.paramDouble1,msg.paramDouble2);
		break;
	case EMPEZAR_ACCION:
		printf("EMPEZAR_ACCION\n");

		this->gameCtrl->empezarAccion(msg.paramInt1);
		break;
	case TERMINAR_ACCION:
		printf("TERMINAR_ACCION\n");

		this->gameCtrl->terminarAccion(msg.paramInt1);
		break;
	case CREAR_ENTIDAD_CONSTRUIDA:
		printf("CREAR_ENTIDAD_CONSTRUIDA\n");

		this->gameCtrl->agregarEntidad(msg.paramNombre, msg.paramDouble1,
				msg.paramDouble2, 0);
		//setear id de entidades
		this->gameCtrl->setId(msg.paramDouble1, msg.paramDouble2,
				msg.paramInt1);
		this->gameCtrl->finalizarConstruccion(msg.paramInt1);
		break;

	default:
		break;
	}
}

Interprete::~Interprete() {
	delete this->gameCtrl;
}
