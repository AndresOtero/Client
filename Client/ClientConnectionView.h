/*
 * ClientConnectionView.h
 *
 *  Created on: 14/11/2015
 *      Author: ger
 */

#ifndef CLIENTCONNECTIONVIEW_H_
#define CLIENTCONNECTIONVIEW_H_


#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using namespace std;
class ClientConnectionView {
public:
	ClientConnectionView();

	string getIp();
	int getPort();
	string getUsername();
	string getRaza();

	bool showForm();
	void onServerError(string error);

	virtual ~ClientConnectionView();

private:
	string ip;
	string port;
	string username;
	string raza;

	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;

	SDL_Texture *backgroundTexture;

	SDL_Texture* ipForm;
	SDL_Rect ipRect;

	SDL_Texture* portForm;
	SDL_Rect portRect;

	SDL_Texture* userForm;
	SDL_Rect userRect;

	SDL_Rect elfosButton;
	SDL_Rect hobbitButton;
	SDL_Rect humanosButton;
	SDL_Rect mordorButton;

	SDL_Rect submitButton;

	int currentInput;
	bool shouldQuit;
	bool shouldStay ;
	bool showingError;

	bool init();
	void quit();
	bool pullChanges();
	void connectToServer();

	void eraseCurrentInput();
	void addCurrentInput(char* newString);
	void redrawCurrentInput();
	// Click helper
	bool pointInsideRect(SDL_Point point, SDL_Rect rect);
	// Texture text helpers
	SDL_Texture* renderText(string text, SDL_Rect* textSize);
	void freeTexture(SDL_Texture* texture);
};

#endif /* CLIENTCONNECTIONVIEW_H_ */
