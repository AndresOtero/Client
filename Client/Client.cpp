#include "MySocket.h"
#include <SDL2/SDL.h>
#include <cstdio>
#include "Interprete.h"
#include "ModeloSrc/Modelo.h"
#include "VistaSrc/Vista.h"
#include "ModeloSrc/Configuracion.h"
#include "VistaSrc/CambioDeCoordendas.h"
#include "GameControllerSrc/GameControllerCliente.h"
#include "Yaml.h"

using namespace std;

void enviarKeepAlive(MySocket* myClient, Interprete* interprete){

	string messageToServer = interprete->getKeepAliveMsg();

	myClient->sendMessage(messageToServer);

}
void establecerLogin(MySocket* myClient, Interprete* interprete){

	string messageFromServer = "";

	string messageToServer = interprete->getLoginMsg();

	myClient->sendMessage(messageToServer);
}

void enviarAccion(MySocket* myClient, string & msg){

	myClient->sendMessage(msg);
}

void obtenerActualizacionesDelServer(MySocket* myClient, Interprete* interprete) {

	string msgFromSrv = myClient->recieveMessage();

	if (myClient->isConnected() == true){
		cout <<  msgFromSrv.c_str() << "\n";
		printf("Cliente - Recive Update: %s \n", msgFromSrv.c_str());

		/*string alive = "ALIVE";
		if (msgFromSrv.c_str() == alive.c_str()){
			printf("le pega al alive");
		}else{
			printf("no le pega al alive");
		}*/
		interprete->notifyUpdate(msgFromSrv);
	}
}

int main(int argc, char *argv[]) {
	/**Main del juego**/
	bool reiniciar = true;

	while (reiniciar) {
		Yaml* reader = new Yaml();
		Juego* juego = reader->read();
		delete reader;
		Modelo* modelo = new Modelo(juego);
		Vista* vista = new Vista(modelo);
		vista->init();
		vista->loadMedia();
		reiniciar = vista->run();
		delete modelo;
		delete vista;
	}
	/**Fin del main del juego**/
	double tiempo_actual, tiempo_viejo = 0;
	string msgToSrv, msgFromSrv;
	string vacio = "";
	Interprete interprete;

	MySocket myClient(PORTNUM);  // create the client socket
	myClient.connectToServer("127.0.0.1");

	establecerLogin(&myClient, &interprete);

	tiempo_viejo=SDL_GetTicks();

	while (1)
	{

		if (myClient.isConnected() == false){
				printf("desconexion del servidor \n");
				//myClient.reconnectToServer(); TE DEJO ACA PARA QUE VEAS COMO RECONECTAR ANTE UN MENSAJE DEL USUARIO
		}
	   obtenerActualizacionesDelServer(&myClient, &interprete);
	   //aca va toda la corrida del juego, puede devolver un string con el evento a notificar

	   msgToSrv = "";
	   msgToSrv = interprete.getActualizarRecursosMsg(0,0,1);

	   if (msgToSrv == vacio ){
		   enviarKeepAlive(&myClient,&interprete);
	   }else{
		   enviarAccion(&myClient, msgToSrv);
	   }

	   usleep((40 - (tiempo_actual-tiempo_viejo))*1000);
	   tiempo_actual= SDL_GetTicks();
	   tiempo_viejo=tiempo_actual;
	}
    return 0;
}


