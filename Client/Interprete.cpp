/*
 * Interprete.cpp
 *
 *  Created on: 21/10/2015
 *      Author: ger
 */

#include "Interprete.h"
#include <math.h>

char* Interprete::string_to_char_array(string str) {
	int str_size = str.size();
	char charArray[PARAM_STRING_LEN];
	for (int a = 0; a <= str_size; a++) {
		charArray[a] = str[a];
	}
	return charArray;
}

Interprete::Interprete(GameControllerCliente* gm) {
	this->gameCtrl = gm;
	this->start = false;
	this->vista = NULL;
}
msg_t Interprete::getKeepAliveMsg() {
	msg_t r;
	r.type = KEEPALIVE;
	return r;
}
msg_t Interprete::getQuit() {
	msg_t r;
	r.type = QUIT;
	return r;
}
bool Interprete::isQuit(msg_t quit) {
	return (quit.type == QUIT);
}
string Interprete::getNombreJugador() {
	return this->gameCtrl->nombreJugador();
}
msg_t Interprete::getLoginMsg(string nombre) {
	msg_t mensajeLogin;

	memcpy(mensajeLogin.paramTipo, string_to_char_array(this->gameCtrl->juego->escenario->jugador->raza), sizeof(mensajeLogin.paramTipo));

	memcpy(mensajeLogin.paramNombre, string_to_char_array(nombre), sizeof(mensajeLogin.paramNombre));
	return mensajeLogin;
}

void Interprete::procesarMensajeDeServer(msg_t msg) {
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

		if (this->gameCtrl->esNombre(msg.paramNombre)) {
			this->vista->setear_vista(msg.paramNombre);
		}
		break;
	case NUEVO_PERSONAJE:
		printf("NUEVO_PERSONAJE\n");
		p = this->gameCtrl->conectarCliente(msg.paramNombre, msg.paramTipo, msg.paramDouble1, msg.paramDouble2, msg.paramInt1);
		this->vista->crearPersonaje(string(msg.paramTipo), p);
		//printf("termino NUEVO PERSONAJE\n");
		break;

	case COMENZAR_PARTIDA:
		printf("COMENZAR_PARTIDA\n");

		this->start = true;

		break;
	case QUIT:
		printf("QUIT\n");

		this->gameCtrl->desconectar(msg.paramNombre);
		break;
	case CREAR_ENTIDAD:
		printf("CREAR_ENTIDAD\n");

		this->gameCtrl->agregarEntidad(msg.paramNombre, msg.paramDouble1, msg.paramDouble2, 0);
		//setear id de entidades
		this->gameCtrl->setId(msg.paramDouble1, msg.paramDouble2, msg.paramInt1);
		break;
	case CREAR_RECURSO:
		printf("CREAR_RECURSO\n");

		this->gameCtrl->agregarEntidad(msg.paramNombre, msg.paramDouble1, msg.paramDouble2, msg.paramInt1);
		break;
	case RECONNECT:
		printf("RECONNECT\n");

		this->gameCtrl->reconectar(msg.paramNombre);
		break;
	case PARAM_MAPA:
		printf("PARAM_MAPA\n");

		this->gameCtrl->juego->escenario->size_x = msg.paramDouble1;
		this->gameCtrl->juego->escenario->size_y = msg.paramDouble2;
		this->gameCtrl->setMapa(msg.paramDouble1,msg.paramDouble2);
		break;
	case CONFIGURACION:
		printf("CONFIGURACION\n");

		this->gameCtrl->setConfiguracion((int) msg.paramDouble1, (int) msg.paramDouble2);
		break;
	case DISCONNECT:
		printf("DISCONNECT\n");

		this->gameCtrl->desconectar(msg.paramNombre);
		break;
	case ACTUALIZACION_RECURSOS:
		printf("ACTUALIZACION_RECURSOS\n");

		this->gameCtrl->acutalizarRecursos(msg.paramNombre, msg.paramInt1, msg.paramDouble1, msg.paramDouble2);

		break;
	case SET_ID_RECURSO:
		//printf("SET_ID_RECURSO\n");
		printf("SET_ID_RECURSO\n");

		this->gameCtrl->setId(msg.paramDouble1, msg.paramDouble2, msg.paramInt1);
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

		this->gameCtrl->ataque(msg.paramInt1, msg.paramDouble1, msg.paramDouble2);
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

		this->gameCtrl->construir(msg.paramInt1, msg.paramDouble1, msg.paramDouble2);
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
		this->gameCtrl->agregarEntidad(msg.paramNombre, msg.paramDouble1, msg.paramDouble2, 0);
		//setear id de entidades
		this->gameCtrl->setId(msg.paramDouble1, msg.paramDouble2, msg.paramInt1);
		this->gameCtrl->finalizarConstruccion(msg.paramInt1);
		break;

	case CAMBIAR_PERSONAJE:
		printf("CAMBIAR_PERSONAJE\n");

		this->gameCtrl->cambiar_personaje(msg.paramInt1, msg.paramNombre, msg.paramTipo);
		break;

	case ELIMINAR_TODOS:
		printf("ELIMINAR_TODOS\n");

		this->gameCtrl->eliminarTodos(msg.paramTipo);

		break;

	case PIERDE:
		printf("PIERDE\n");

		this->gameCtrl->meFijoSiPerdi(msg.paramNombre);
		break;
	default:
		break;
	}
}

Interprete::~Interprete() {
	delete this->gameCtrl;
}
