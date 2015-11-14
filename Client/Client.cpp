#include <string>

#include "ClientConnectionView.h"
#include "Game.h"
#include "MySocket.h"

using namespace std;

int main(int argc, char *argv[]) {
	Game game;
	ClientConnectionView clientConnectionView;

	clientConnectionView.showForm();

	int portNumber= clientConnectionView.getPort();
	string IPserver = clientConnectionView.getIp();
	string userName = clientConnectionView.getUsername();

	MySocket myClient(portNumber);

	myClient.connectToServer(IPserver.c_str());

	if (myClient.isConnected()) {

		if (!game.init(&myClient, userName)) return -1;
		game.jugar();
	}

	return 0;
}

