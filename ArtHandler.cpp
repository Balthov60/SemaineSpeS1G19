#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_Image.h>
#include "config_sdl.h"

#include "Main.h"
#include "ArtHandler.h"
#include "MuseumService.h"

using namespace std;

void displayBasicToolsComponents(SDL_Renderer * render) {
	SDL_Rect rectangle; // Création module de dessin

	// Paramètre généraux des compartiments
	rectangle.x = TOOLS_BLOCKS_X;
	rectangle.w = TOOLS_BLOCKS_WIDTH;

	// Creation du compteur
	rectangle.y = TIMER_Y;
	rectangle.h = TIMER_HEIGHT;

	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderFillRect(render, &rectangle);

	// Creation du cadre
	rectangle.y = ARTWORK_Y;
	rectangle.h = ARTWORK_HEIGHT;

	SDL_RenderFillRect(render, &rectangle);

	// Description
	rectangle.y = DESCRIPTION_Y;
	rectangle.h = DESCRIPTION_HEIGHT;

	SDL_RenderFillRect(render, &rectangle);

	SDL_RenderPresent(render);
}

// currentTile en ms
void displayTimer(SDL_Renderer * render, int currentTime) { 
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);
	SDL_Texture* timer;
	SDL_Rect canvas = {};

	currentTime /= 1000;

	string time = "0";
	char timeCString[6];
	
	time += to_string(currentTime / 60);
	time += ":";
	if (currentTime % 60 < 10) {
		time += "0"; 
		time += to_string(currentTime % 60);
	}
	else {
		time += to_string(currentTime % 60);
		if (time.length() < 5)
			time += "0";
	}
		
	strncpy_s(timeCString, time.c_str(), 100);
	timer = loadText(render, timeCString, { 0, 0, 0, 255 }, font);
	SDL_QueryTexture(timer, NULL, NULL, &canvas.w, &canvas.h);

	canvas.x = TOOLS_BLOCKS_X + (TOOLS_BLOCKS_WIDTH - canvas.w) / 2;
	canvas.y = TIMER_Y + (TIMER_HEIGHT - canvas.h) / 2;
	
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderFillRect(render, &canvas);

	SDL_RenderCopy(render, timer, NULL, &canvas);
	SDL_RenderPresent(render);

	SDL_DestroyTexture(timer);
	TTF_CloseFont(font);
}

void displayArtWork(SDL_Renderer * render, int artWorkID, int museumID) {

	displayBasicToolsComponents(render);

	SDL_Rect canvas;
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);
	char *** artWorkInformations = NULL;

	getMuseumArtWorkIdForCurrentMuseum(artWorkID, museumID, ".\\data\\bdMusees.sqlite");
	getMuseumArtWorkByID(artWorkInformations, artWorkID, ".\\data\\bdMusees.sqlite");

	displayArtImage(render, artWorkInformations[0][3]);

	// Titre et Description
	SDL_Color color = { 0, 0, 0 };
	int descriptionSize = strlen(artWorkInformations[0][2]);
	int index = 0;
	int lineStartIndex = 0;
	int lastSpaceIndex = 0;
	string line = "";
	char lineCString[100];

	// Titre
	SDL_Texture* text = loadText(render, artWorkInformations[0][1], color, font);
	
	canvas.x = TOOLS_BLOCKS_X + 5;
	canvas.y = DESCRIPTION_Y + 5;
	SDL_QueryTexture(text, NULL, NULL, &canvas.w, &canvas.h);
	SDL_RenderCopy(render, text, NULL, &canvas);

	// Description
	canvas.y += canvas.h;

	while (index < descriptionSize) {
		do {
			line += artWorkInformations[0][2][index];
			strncpy_s(lineCString, line.c_str(), 100);

			text = loadText(render, lineCString, color, font);
			SDL_QueryTexture(text, NULL, NULL, &canvas.w, &canvas.h);

			if (artWorkInformations[0][2][index] == ' ')
				lastSpaceIndex = index;

			canvas.w *= 0.75;
			canvas.h *= 0.75;
			index++;
		} while (canvas.w < (TOOLS_BLOCKS_WIDTH - 10) && index < descriptionSize);

		if (index - 1 != lastSpaceIndex && index < descriptionSize) {
			line = "";

			for (int i = lineStartIndex; i < lastSpaceIndex; i++) {
				line += artWorkInformations[0][2][i];
			}

			strncpy_s(lineCString, line.c_str(), 100);
			text = loadText(render, lineCString, color, font);
			SDL_QueryTexture(text, NULL, NULL, &canvas.w, &canvas.h);

			canvas.w *= 0.75;
			canvas.h *= 0.75;

			index = lastSpaceIndex + 1;
		}
		SDL_RenderCopy(render, text, NULL, &canvas);
		canvas.y += canvas.h;

		lineStartIndex = index;
		line = "";
	}
	SDL_DestroyTexture(text);
	SDL_RenderPresent(render);
	TTF_CloseFont(font);
}

void displayArtImage(SDL_Renderer * render, char fileName[]) {
	SDL_Rect canvas;
	SDL_Texture * image;
	canvas.x = TOOLS_BLOCKS_X;
	canvas.y = ARTWORK_Y;

	// Get Image
	char imagePathCString[100];
	string imagePath = ".\\";
	imagePath += fileName;
	strncpy_s(imagePathCString, imagePath.c_str(), 100);

	image = loadImage(render, imagePathCString);
	SDL_QueryTexture(image, NULL, NULL, &canvas.w, &canvas.h);

	// Redimensionner l'image
	while (canvas.w >= TOOLS_BLOCKS_WIDTH || canvas.h >= ARTWORK_HEIGHT) {
		canvas.w *= 0.95;
		canvas.h *= 0.95;
	}

	// Centrer l'image
	canvas.x += (TOOLS_BLOCKS_WIDTH - canvas.w) / 2;
	canvas.y += (ARTWORK_HEIGHT - canvas.h) / 2;

	SDL_RenderCopy(render, image, NULL, &canvas);

	SDL_RenderPresent(render);
	SDL_DestroyTexture(image);
}

void displayArtTitle(SDL_Renderer * render, char artTitle[], SDL_Rect & canvas) {
	SDL_Color color = { 255, 255, 255 };
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);
	SDL_Texture* texture = loadText(render, artTitle, color, font);

	canvas.x = TOOLS_BLOCKS_X + 5;
	canvas.y = DESCRIPTION_Y + 5;
	SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);
	SDL_RenderCopy(render, texture, NULL, &canvas);

	SDL_RenderPresent(render);

	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
	TTF_Quit();
}

void displayArtDescription(SDL_Renderer * render, char artDescription[], SDL_Rect & canvas) {
	SDL_Color color = { 255, 255, 255 };
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);
	SDL_Texture* texture;

	int descriptionSize = strlen(artDescription);
	
	int index = 0;
	int lineStartIndex = 0;
	int lastSpaceIndex = 0;
	
	string line = "";
	char lineCString[100];

	canvas.y += canvas.h;

	while (index < descriptionSize) {
		do {
			line += artDescription[index];
			strncpy_s(lineCString, line.c_str(), 100);
			texture = loadText(render, lineCString, color, font);
			SDL_QueryTexture(texture, NULL, NULL, &*&canvas.w, &*&canvas.h);

			if (artDescription[index] == ' ')
				lastSpaceIndex = index;

			canvas.w *= 0.75;
			canvas.h *= 0.75;
			index++;
		} 
		while (canvas.w < (TOOLS_BLOCKS_WIDTH - 10) && index < descriptionSize);

		if (index - 1 != lastSpaceIndex && index < descriptionSize) {
			line = "";

			for (int i = lineStartIndex; i < lastSpaceIndex; i++) {
				line += artDescription[i];
			}

			strncpy_s(lineCString, line.c_str(), 100);
			texture = loadText(render, lineCString, color, font);
			SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);

			canvas.w *= 0.75;
			canvas.h *= 0.75;

			index = lastSpaceIndex + 1;
		}
		SDL_RenderCopy(render, texture, NULL, &canvas);
		canvas.y += canvas.h;

		lineStartIndex = index;
		line = "";
	}

	SDL_RenderPresent(render);

	TTF_CloseFont(font);
	TTF_Quit();
}

void displayHelpText(SDL_Renderer * render) {
	TTF_Init();
	SDL_Rect item;
	SDL_Texture * texture;
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 20);

	item.x = TOOLS_BLOCKS_X + 5;
	item.y = DESCRIPTION_Y + 5;

	texture = loadText(render, "Appuyer sur 'espace' pour voler.", { 0, 0, 0 }, font);
	SDL_QueryTexture(texture, NULL, NULL, &item.w, &item.h);
	SDL_RenderCopy(render, texture, NULL, &item);

	item.y += item.h + 5;

	texture = loadText(render, "Appuyer sur 'espace' pour sortir.", { 0, 0, 0 }, font);
	SDL_QueryTexture(texture, NULL, NULL, &item.w, &item.h);
	SDL_RenderCopy(render, texture, NULL, &item);

	item.y += item.h + 5;

	texture = loadText(render, "Appuyer sur 'echap' pour quitter.", { 0, 0, 0 }, font);
	SDL_QueryTexture(texture, NULL, NULL, &item.w, &item.h);
	SDL_RenderCopy(render, texture, NULL, &item);

	SDL_RenderPresent(render);

	TTF_CloseFont(font);
	TTF_Quit();
}