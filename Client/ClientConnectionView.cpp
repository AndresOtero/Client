/*
 * ClientConnectionView.cpp
 *
 *  Created on: 14/11/2015
 *      Author: ger
 */

#include "ClientConnectionView.h"

#define TEXT_SPACING 20
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 400

ClientConnectionView::ClientConnectionView() {
	this->ip = "127.0.0.1";
	this->port = "51717";
	this->raza = "Elfo";
	this->username = "ger";
	this->currentInput = 1;

	this->font = NULL;
	this->renderer = NULL;
	this->window = NULL;
	this->shouldQuit = false;

	this->backgroundTexture = NULL;
	this->ipForm = NULL;
	this->ipRect = {0, 0, 0, 0};
	this->portForm = NULL;
	this->portRect = {0, 0, 0, 0};
	this->userForm = NULL;
	this->userRect = {0, 0, 0, 0};

	this->showingError = false;
	this->shouldStay = true;

}

bool ClientConnectionView::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}

	// Create window
	window = SDL_CreateWindow("Age of the Rings", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN);
	if (window == NULL) {
		SDL_Quit();
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	SDL_Surface *background = IMG_Load("img/extras/objectives.jpg");

	if (background == NULL) {
		printf("no carga imagen \n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);

	if (backgroundTexture == NULL) {
		printf("no carga imagen \n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	if (TTF_Init() == -1) {
		SDL_DestroyTexture(backgroundTexture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	font = TTF_OpenFont("font.ttf", 16);
	if (font == NULL) {
		TTF_Quit();
		SDL_DestroyTexture(backgroundTexture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}
	return true;
}

void ClientConnectionView::quit() {
	SDL_DestroyTexture(backgroundTexture);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	font = NULL;
	renderer = NULL;
	window = NULL;
}
bool ClientConnectionView::showForm() {
	if (!init()) {
		return false;
	}

	// Start capturing the keyboard events
	SDL_StartTextInput();

	// Initialize the form inputs
	ipForm = renderText(ip, &ipRect);
	portForm = renderText(port, &portRect);
	userForm = renderText(username, &userRect);
	// Set the views positioning
	SDL_Rect formTitleRect = { 0, TEXT_SPACING, 0, 0 };
	SDL_Texture* formTitle = renderText("Preparando partida...", &formTitleRect);
	formTitleRect.x = (WINDOW_WIDTH - formTitleRect.w) / 2;

	SDL_Rect ipLabelRect = { TEXT_SPACING, formTitleRect.y + formTitleRect.h + TEXT_SPACING, 0, 0 };
	SDL_Texture* ipLabel = renderText("IP servidor:", &ipLabelRect);
	ipRect.x = ipLabelRect.x + ipLabelRect.w + TEXT_SPACING;
	ipRect.y = ipLabelRect.y;

	SDL_Rect portLabelRect = { TEXT_SPACING, ipLabelRect.y + ipLabelRect.h + TEXT_SPACING, 0, 0 };
	SDL_Texture* portLabel = renderText("Puerto servidor:", &portLabelRect);
	portRect.x = portLabelRect.x + portLabelRect.w + TEXT_SPACING;
	portRect.y = portLabelRect.y;

	SDL_Rect userLabelRect = { TEXT_SPACING, portLabelRect.y + portLabelRect.h + TEXT_SPACING, 0, 0 };
	SDL_Texture* userLabel = renderText("Nombre de usuario:", &userLabelRect);
	userRect.x = userLabelRect.x + userLabelRect.w + TEXT_SPACING;
	userRect.y = userLabelRect.y;

	SDL_Rect formButtonElfosRect = { 0, userLabelRect.y + userLabelRect.h + TEXT_SPACING, 0, 0 };
	SDL_Texture* formButtonElfos = renderText("Elfos", &formButtonElfosRect);
	formButtonElfosRect.x = formButtonElfosRect.x + formButtonElfosRect.w + TEXT_SPACING;
	elfosButton = {
		formButtonElfosRect.x - TEXT_SPACING / 2,
		formButtonElfosRect.y - TEXT_SPACING / 2,
		formButtonElfosRect.w + TEXT_SPACING ,
		formButtonElfosRect.h + TEXT_SPACING
	};

	SDL_Rect formButtonHobbitRect =
			{ formButtonElfosRect.x + formButtonElfosRect.w, userLabelRect.y + userLabelRect.h + TEXT_SPACING, 0, 0 };
	SDL_Texture* formButtonHobbit = renderText("Hobbit", &formButtonHobbitRect);
	formButtonHobbitRect.x = formButtonHobbitRect.x + formButtonElfosRect.w + TEXT_SPACING;
	hobbitButton = {
		formButtonHobbitRect.x - TEXT_SPACING / 2,
		formButtonHobbitRect.y - TEXT_SPACING / 2,
		formButtonHobbitRect.w + TEXT_SPACING ,
		formButtonHobbitRect.h + TEXT_SPACING
	};

	SDL_Rect formButtonHumanosRect = { formButtonHobbitRect.x + formButtonHobbitRect.w, userLabelRect.y + userLabelRect.h + TEXT_SPACING, 0,
			0 };
	SDL_Texture* formButtonHumanos = renderText("Humanos", &formButtonHumanosRect);
	formButtonHumanosRect.x = formButtonHumanosRect.x + +formButtonElfosRect.w + TEXT_SPACING;
	humanosButton = {
		formButtonHumanosRect.x - TEXT_SPACING / 2,
		formButtonHumanosRect.y - TEXT_SPACING / 2,
		formButtonHumanosRect.w + TEXT_SPACING ,
		formButtonHumanosRect.h + TEXT_SPACING
	};

	SDL_Rect formButtonMordorRect = { formButtonHumanosRect.x + formButtonHumanosRect.w, userLabelRect.y + userLabelRect.h + TEXT_SPACING,
			0, 0 };
	SDL_Texture* formButtonMordor = renderText("Mordor", &formButtonMordorRect);
	formButtonMordorRect.x = formButtonMordorRect.x + formButtonElfosRect.w + TEXT_SPACING;
	mordorButton = {
		formButtonMordorRect.x - TEXT_SPACING / 2,
		formButtonMordorRect.y - TEXT_SPACING / 2,
		formButtonMordorRect.w + TEXT_SPACING,
		formButtonMordorRect.h + TEXT_SPACING
	};

	SDL_Rect formButtonRect = { 0, formButtonElfosRect.y + formButtonElfosRect.h + 2 * TEXT_SPACING, 0, 0 };
	SDL_Texture* formButton = renderText("Comenzar", &formButtonRect);
	formButtonRect.x = (WINDOW_WIDTH - formButtonRect.w) / 2;
	submitButton = {
		formButtonRect.x - TEXT_SPACING / 2,
		formButtonRect.y - TEXT_SPACING / 2,
		formButtonRect.w + TEXT_SPACING,
		formButtonRect.h + TEXT_SPACING
	};

	while (!shouldQuit) {
		// Update texts
		shouldStay = pullChanges();

		// Clear screen
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		// Draw
		SDL_RenderCopy(renderer, this->backgroundTexture, NULL, NULL);

		SDL_RenderCopy(renderer, formTitle, NULL, &formTitleRect);

		SDL_RenderCopy(renderer, ipLabel, NULL, &ipLabelRect);
		SDL_RenderCopy(renderer, ipForm, NULL, &ipRect);

		SDL_RenderCopy(renderer, portLabel, NULL, &portLabelRect);
		SDL_RenderCopy(renderer, portForm, NULL, &portRect);

		SDL_RenderCopy(renderer, userLabel, NULL, &userLabelRect);
		SDL_RenderCopy(renderer, userForm, NULL, &userRect);

		SDL_RenderCopy(renderer, formButtonElfos, NULL, &formButtonElfosRect);
		if (this->raza == "Elfo") {
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x00, 0x00);
			SDL_RenderDrawRect(renderer, &elfosButton);
		} else {
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(renderer, &elfosButton);
		}
		SDL_RenderCopy(renderer, formButtonHobbit, NULL, &formButtonHobbitRect);
		if (this->raza == "Hobbit") {
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x00, 0x00);
			SDL_RenderDrawRect(renderer, &hobbitButton);
		} else {
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(renderer, &hobbitButton);
		}
		SDL_RenderCopy(renderer, formButtonHumanos, NULL, &formButtonHumanosRect);
		if (this->raza == "Humano") {
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x00, 0x00);
			SDL_RenderDrawRect(renderer, &humanosButton);
		} else {
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(renderer, &humanosButton);
		}
		SDL_RenderCopy(renderer, formButtonMordor, NULL, &formButtonMordorRect);
		if (this->raza == "Mordor") {
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x00, 0x00);
			SDL_RenderDrawRect(renderer, &mordorButton);
		} else {
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(renderer, &mordorButton);
		}

		SDL_RenderCopy(renderer, formButton, NULL, &formButtonRect);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(renderer, &submitButton);

		// Update screen
		SDL_RenderPresent(renderer);

		SDL_Delay(50);
	}


	// Free memory
	freeTexture(ipForm);
	freeTexture(portForm);
	freeTexture(userForm);

	freeTexture(formTitle);
	freeTexture(formButton);

	freeTexture(ipLabel);
	freeTexture(portLabel);
	freeTexture(userLabel);
	// Stop capturing the keyboard events
	SDL_StopTextInput();

	quit();
	return shouldStay;
}

ClientConnectionView::~ClientConnectionView() {
}

bool ClientConnectionView::pullChanges() {
	bool updateString = false;
	SDL_Rect ipInput = { 0, ipRect.y, WINDOW_WIDTH, ipRect.h };
	SDL_Rect portInput = { 0, portRect.y, WINDOW_WIDTH, portRect.h };
	SDL_Rect userInput = { 0, userRect.y, WINDOW_WIDTH, userRect.h };
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {

		if (e.type == SDL_QUIT) {
			shouldQuit = true;
			return false;
		}
		if (!showingError) {
			// Detect what is clicking
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				// Get mouse position
				SDL_Point clickPoint = { 0, 0 };
				SDL_GetMouseState(&clickPoint.x, &clickPoint.y);

				if (pointInsideRect(clickPoint, submitButton)) {
					connectToServer();
				} else if (pointInsideRect(clickPoint, elfosButton)) {
					this->raza = "Elfo";
				} else if (pointInsideRect(clickPoint, hobbitButton)) {
					this->raza = "Hobbit";
				} else if (pointInsideRect(clickPoint, humanosButton)) {
					this->raza = "Humano";
				} else if (pointInsideRect(clickPoint, mordorButton)) {
					this->raza = "Mordor";
				} else if (pointInsideRect(clickPoint, ipInput)) {
					this->currentInput = 1;
				} else if (pointInsideRect(clickPoint, portInput)) {
					this->currentInput = 2;
				} else if (pointInsideRect(clickPoint, userInput)) {
					this->currentInput = 3;
				}
			}

			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_BACKSPACE && ip.length() > 0) {
					this->eraseCurrentInput();
					updateString = true;
				}
				// Normal form helpers
				if (e.key.keysym.sym == SDLK_TAB) {
					switch (this->currentInput) {
					case 1:
					case 2:
						this->currentInput++;
						break;
					default:
						this->currentInput = 1;
					}
				}
				if (e.key.keysym.sym == SDLK_RETURN) {
					this->connectToServer();
				}
			}

			if (e.type == SDL_TEXTINPUT) {
				this->addCurrentInput(e.text.text);
				updateString = true;
			}
		}
	}

	if (updateString) {
		this->redrawCurrentInput();
	}

	return true;
}

void ClientConnectionView::eraseCurrentInput() {
	switch (currentInput) {
	case 1:
		if (ip.length() > 0) {
			ip.erase(ip.length() - 1, 1);
		}
		break;
	case 2:
		if (port.length() > 0) {
			port.erase(port.length() - 1, 1);
		}
		break;
	case 3:
		if (username.length() > 0) {
			username.erase(username.length() - 1, 1);
		}
		break;
	default:
		break;
	}
}

void ClientConnectionView::addCurrentInput(char* newString) {
	switch (currentInput) {
	case 1:
		ip += newString;
		break;
	case 2:
		port += newString;
		break;
	case 3:
		username += newString;
		break;
	default:
		break;
	}
}

void ClientConnectionView::redrawCurrentInput() {
	switch (currentInput) {
	case 1:
		freeTexture(ipForm);
		ipForm = renderText(ip, &ipRect);
		break;
	case 2:
		freeTexture(portForm);
		portForm = renderText(port, &portRect);
		break;
	case 3:
		freeTexture(userForm);
		userForm = renderText(username, &userRect);
		break;
	default:
		break;
	}
}

void ClientConnectionView::connectToServer() {
	this->shouldQuit = true;
}

// Click helper
bool ClientConnectionView::pointInsideRect(SDL_Point point, SDL_Rect rect) {
	return (rect.x <= point.x && rect.x + rect.w >= point.x && rect.y <= point.y && rect.y + rect.h >= point.y);
}

// Texture text helpers
void ClientConnectionView::freeTexture(SDL_Texture* texture) {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
	}
}

SDL_Texture* ClientConnectionView::renderText(string text, SDL_Rect* textSize) {
	SDL_Color textColor = { 0, 0, 0 };
	SDL_Surface* surfaceText = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if (surfaceText == NULL) {
		// Error => sin surface
		return NULL;
	}
	textSize->w = surfaceText->w;
	textSize->h = surfaceText->h;

	SDL_Texture* renderedText = SDL_CreateTextureFromSurface(renderer, surfaceText);
	SDL_FreeSurface(surfaceText);
	if (renderedText == NULL) {
		// Error => sin texto
		textSize->w = 0;
		textSize->h = 0;
	}
	return renderedText;
}

string ClientConnectionView::getIp() {
	return this->ip;
}

string ClientConnectionView::getRaza() {
	return this->raza;
}

int ClientConnectionView::getPort() {
	return strtoimax(port.c_str(), NULL, 10);
}

string ClientConnectionView::getUsername() {
	return this->username;
}

// Error View
void ClientConnectionView::onServerError(string error) {
	this->shouldQuit = false;
	this->showingError = true;

	if (!init()) {
		// Some error
		return;
	}

	SDL_Rect errorRect = { 0, 0, 0, 0 };
	SDL_Texture* errorText = renderText(error, &errorRect);

	// Wait for the end
	pullChanges();

	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	// Draw
	SDL_RenderCopy(renderer, errorText, NULL, &errorRect);

	// Update screen
	SDL_RenderPresent(renderer);

	SDL_Delay(3000);

	freeTexture(errorText);

	quit();
}

