#include "MySocket.h"
#include <SDL2/SDL.h>
#include <cstdio>
#include "Interprete.h"
#include <iostream>
#include <unistd.h>
#include "Yaml.h"
#include "VistaSrc/Vista.h"
#include "ModeloSrc/Modelo.h"
#include "ModeloSrc/Juego.h"
using namespace std;

void enviarKeepAlive(MySocket* myClient, Interprete* interprete){

	msg_t messageToServer = interprete->getKeepAliveMsg();

	myClient->sendMessage(messageToServer);

}
void establecerLogin(MySocket* myClient, Interprete* interprete){

	msg_t messageToServer = interprete->getLoginMsg();

	myClient->sendMessage(messageToServer);
}

void enviarAccion(MySocket* myClient, msg_t msg){

	myClient->sendMessage(msg);
}

void obtenerActualizacionesDelServer(MySocket* myClient, Interprete* interprete) {

	msg_t msgFromSrv = myClient->recieveMessage();

	if (myClient->isConnected() == true){
		cout << "Server: Recive actualizacion de tipo: " << msgFromSrv.type << "\n";

		interprete->notifyUpdate(msgFromSrv);
	}
}


int main(int argc, char *argv[])
{
	double tiempo_actual,tiempo_viejo=0;
	bool enviarAlive;
	msg_t msgToSrv;
	GameControllerCliente * gameController = new GameControllerCliente();
	Interprete interprete;
	bool reiniciar=true;
<<<<<<< HEAD
=======
	while (reiniciar){
			Yaml* reader=new Yaml("YAML/configuracion.yaml");
			Juego* juego = reader->read();
			delete reader;
			Modelo* modelo=new Modelo(juego);
			//modelo->agregarEntidad("barraca",0,0);
>>>>>>> refs/remotes/origin/master

	MySocket myClient(PORTNUM);
	myClient.connectToServer("127.0.0.1");

	establecerLogin(&myClient, &interprete);

	//lee el YAML antes de cargar el usuario y el modelo
	Yaml* reader = new Yaml("YAML/configuracion.yaml");
	Juego* juego = reader->read();
	delete reader;
	gameController->insertarJuego(juego);

	//se lo deberia pasar el server
	juego->setEscenario("Orlean",100,100);
	juego->setConfiguracion(100,1);

	//una vez que pudo ingresar deberia cargar el juego
	//while (true){
	 //msg_t mensaje = myClient.recieveMessage();
	 //if (mensaje.type==finCarga){
	 //		break;
	 //}
	 //interpretar mensaje
	//}

	gameController->crearModelo();
	Modelo *modelo = gameController->devolverModelo();

	//Inicia vista
	Vista* vista=new Vista(modelo,gameController);
	vista->init();
	vista->loadMedia();

	//comienza a jugar
	tiempo_viejo=SDL_GetTicks();
	bool fin;
	while (1)
	{
		enviarAlive = true; //poner en false si mando otra cosa

		if (myClient.isConnected() == false){
				printf("desconexion del servidor \n");
				//myClient.reconnectToServer(); TE DEJO ACA PARA QUE VEAS COMO RECONECTAR ANTE UN MENSAJE DEL USUARIO
		}
	   obtenerActualizacionesDelServer(&myClient, &interprete);
	   //aca va toda la corrida del juego, puede devolver un string con el evento a notificar

	   fin = vista->run(enviarAlive);

	   if (fin){
		   msg_t quit = interprete.getQuit();
		   myClient.sendMessage(quit);
		   break;
	   }

	   //si el juego no mando nada
	   if (enviarAlive )
		   enviarKeepAlive(&myClient,&interprete);

	   usleep((40 - (tiempo_actual-tiempo_viejo))*1000);
	   tiempo_actual= SDL_GetTicks();
	   tiempo_viejo=tiempo_actual;
	}

	delete modelo;
	delete vista;
	delete gameController;
    return 0;
}


