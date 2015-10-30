#include "MySocket.h"
#include <SDL2/SDL.h>
#include <cstdio>
#include "Interprete.h"
#include <iostream>
#include <unistd.h>
#include "Yaml.h"
#include "GameControllerSrc/mensaje.h"
#include "VistaSrc/Vista.h"
#include "ModeloSrc/Modelo.h"
#include "ModeloSrc/Juego.h"
using namespace std;

void enviarKeepAlive(MySocket* myClient, Interprete* interprete){

	msg_t messageToServer = interprete->getKeepAliveMsg();

	myClient->sendMessage(messageToServer);

}
void establecerLogin(MySocket* myClient, Interprete* interprete,string nombre){

	msg_t messageToServer = interprete->getLoginMsg(interprete->getNombreJugador());

	myClient->sendMessage(messageToServer);
}

void enviarAccion(MySocket* myClient, msg_t msg){

	myClient->sendMessage(msg);
}

void obtenerActualizacionesDelServer(MySocket* myClient, Interprete* interprete) {
	msg_t msgFromSrv;

	msgFromSrv = myClient->recieveMessage();
	while ((myClient->isConnected() == true) && (msgFromSrv.type != KEEPALIVE)){
		interprete->procesarMensajeDeServer(msgFromSrv);
		msgFromSrv = myClient->recieveMessage();
	}
}


int main(int argc, char *argv[])
{
	double tiempo_actual,tiempo_viejo=0;
	bool enviarAlive;
	msg_t msgToSrv;
	GameControllerCliente * gameController = new GameControllerCliente();
	Interprete interprete(gameController);
	bool reiniciar=true;

	MySocket myClient(PORTNUM);


	//lee el YAML antes de cargar el usuario y el modelo
	Yaml* reader = new Yaml("YAML/configuracionCliente.yaml");
	Juego* juego = reader->readCliente();
	delete reader;
	printf("Lee");
	if(!juego)return -1;//No se crea el jugador
	printf(" Lo hace bien\n");
	gameController->insertarJuego(juego);


	//se lo deberia pasar el server
	//juego->setEscenario("Orlean",100,100);
	//juego->setConfiguracion(100,1);

	//una vez que pudo ingresar deberia cargar el juego
	//interpretar mensaje
	//}



	myClient.connectToServer(gameController->ipJugador().c_str());
	establecerLogin(&myClient, &interprete,gameController->nombreJugador());

	//recibe parametros mapa y configuracion

	msg_t mensaje = myClient.recieveMessage();
	interprete.procesarMensajeDeServer(mensaje);

	mensaje = myClient.recieveMessage();
	interprete.procesarMensajeDeServer(mensaje);

	gameController->crearModelo();
	mensaje = myClient.recieveMessage();
	double referencia_x,referencia_y;
	printf("llego hasta aca");
	while ((mensaje.type != FIN_INICIALIZACION) && (myClient.isConnected()) ) {
		if(mensaje.type==LOGIN){
			referencia_x=mensaje.paramDouble1;
			referencia_y=mensaje.paramDouble2;
		}
		mensaje = myClient.recieveMessage();
		interprete.procesarMensajeDeServer(mensaje);
		//printf("%d\n", mensaje.type);
	}
	Modelo *modelo = gameController->devolverModelo();

	//Inicia vista
	Vista* vista=new Vista(modelo,gameController);
	vista->init();
	vista->loadMedia();
	vista->setear_referencia(referencia_x,referencia_y);
	//comienza a jugar
	tiempo_viejo=SDL_GetTicks();
	bool fin;
	while (1)
	{
		enviarAlive = true; //poner en false si mando otra cosa

		if (myClient.isConnected() == false){
				printf("desconexion del servidor \n");
				usleep(1000);
				break;
				//myClient.reconnectToServer(); TE DEJO ACA PARA QUE VEAS COMO RECONECTAR ANTE UN MENSAJE DEL USUARIO
		}
	   obtenerActualizacionesDelServer(&myClient, &interprete);

	   fin = vista->run();

	   if (fin){
		   msg_t quit = interprete.getQuit();
		   enviarAccion(&myClient,quit);
		   break;
	   }
	   if (gameController->hayEventos()){
		   msg_t mensaje = gameController->sacarMensaje();
   		   enviarAccion(&myClient,mensaje);
	   }
	   else{
		   enviarKeepAlive(&myClient,&interprete);
	   }
	   usleep((40 - (tiempo_actual-tiempo_viejo))*1000);
	   tiempo_actual= SDL_GetTicks();
	   tiempo_viejo=tiempo_actual;
	}
	delete modelo;
	delete vista;
	delete gameController;
    return 0;
}


