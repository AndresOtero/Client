#include <string>

#include "ClientConnectionView.h"
#include "Game.h"
#include "MySocket.h"

using namespace std;

int main(int argc, char *argv[]) {
	Game game;
	bool reiniciar = true;

	while (reiniciar) {

		ClientConnectionView clientConnectionView;

		reiniciar = clientConnectionView.showForm();
		if (reiniciar) {

			int portNumber= clientConnectionView.getPort();
			string IPserver = clientConnectionView.getIp();
			string userName = clientConnectionView.getUsername();
			string raza = clientConnectionView.getRaza();

			MySocket myClient(portNumber);

			myClient.connectToServer(IPserver.c_str());

			if (myClient.isConnected()) {
				if (game.init(&myClient, userName, raza)) {
					game.jugar();
					reiniciar = false;
				}else{
					//clientConnectionView.onServerError(game.getStringError());
				}
			} else {
				clientConnectionView.onServerError(" No se pudo conectar al servidor");
			}
		}
	}
	return 0;
}

