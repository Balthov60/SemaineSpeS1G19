#include <iostream>
#include <fstream>

#include "config_sdl.h"

#include "Main.h"
#include "CharacterHandler.h"
#include "ArtHandler.h"
#include "EditHandler.h"

using namespace std;

// Gardes
void guardMove(SDL_Renderer * render, SDL_Rect & guard, SDL_Rect player, int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y], int guardsSpeed) {

	SDL_SetRenderDrawColor(render, 255, 174, 0, 255);
	SDL_RenderFillRect(render, &guard);

	if (player.x > guard.x) {
		guard.x = guard.x + guardsSpeed;
	}

	if (player.x < guard.x) {
		guard.x = guard.x - guardsSpeed;
	}

	if (player.y > guard.y) {
		guard.y = guard.y + guardsSpeed;
	}

	if (player.y < guard.y) {
		guard.y = guard.y - guardsSpeed;
	}

	// Bloque le gardien en direction de la gauche
	if (tabMap[(guard.x - BORDER_SIZE) / BLOCK_SIZE_X][(guard.y - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		|| tabMap[((guard.x) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y+CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		guard.x += guardsSpeed;
		guard.y += guardsSpeed;
		guard.y -= guardsSpeed;
	}

	// Bloque le gardien en direction de la droite
	if (tabMap[((guard.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		|| tabMap[((guard.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		guard.x -= guardsSpeed;
		guard.y += guardsSpeed;
		guard.y -= guardsSpeed;
	}

	// Bloque le gardien en direction du haut
	if (tabMap[(guard.x - BORDER_SIZE) / BLOCK_SIZE_X][(guard.y - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		|| tabMap[((guard.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		guard.x += guardsSpeed;
		guard.x -= guardsSpeed;
		guard.y += guardsSpeed;
	}

	// Bloque le gardien en direction du bas 
	if (tabMap[((guard.x) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		|| tabMap[((guard.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		guard.x += guardsSpeed;
		guard.x -= guardsSpeed;
		guard.y -= guardsSpeed;
	}

	// Bloque coin haut-gauche
	if (tabMap[((guard.x) - BORDER_SIZE) / BLOCK_SIZE_X][(guard.y - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((guard.x) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((guard.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		guard.x += guardsSpeed;
		guard.y += guardsSpeed;
	}

	// Bloque coin haut-droit
	if (tabMap[((guard.x) - BORDER_SIZE) / BLOCK_SIZE_X][(guard.y - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((guard.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((guard.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		guard.x -= guardsSpeed;
		guard.y += guardsSpeed;
	}

	// Bloque coin bas-gauche
	if (tabMap[((guard.x) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((guard.x) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((guard.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		guard.x += guardsSpeed;
		guard.y -= guardsSpeed;
	}

	// Bloque coin bas-droit
	if (tabMap[((guard.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((guard.x) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((guard.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((guard.y) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		guard.x -= guardsSpeed;
		guard.y -= guardsSpeed;
	}
		// Dessine la nouvelle position du garde
		SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
		SDL_RenderFillRect(render, &guard);
		SDL_RenderPresent(render);
}

int getGuardsQty(char mapPath[]) {
	int guardsQty = 0;
	char temp[100];
	ifstream file(mapPath, ios::in);

	if (!file) {
		cout << "Probleme d'ouverture \n";
		return 0;
	}

	while (!file.eof()) {
		file.getline(temp, 100);
		if (temp[0] == 'G')
			guardsQty++;
	}

	file.close();

	return guardsQty;
}

void getGuardsSpawnPoint(SDL_Rect & guard, int index, char mapPath[]) {
	int guardsQty = 0;
	ifstream file(mapPath, ios::in);
	char temp[100];

	if (!file) {
		cout << "Probleme d'ouverture \n";
		return;
	}

	while (!file.eof()) {
		file.getline(temp, 100, ' ');
		if (temp[0] == 'G') {
			guardsQty++;
			if (guardsQty == index) {
				file.getline(temp, 5, ' ');
				guard.x = (atoi(temp) + 0.5) * BLOCK_SIZE_X;
				file.getline(temp, 5, ' ');
				guard.y = (atoi(temp) + 0.5) * BLOCK_SIZE_Y;
				file.close(); 
				return;
			}
		}
		else {
			file.getline(temp, 100);
		}
	} 

	file.close();
}




