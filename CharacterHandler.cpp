#include <iostream>

#include "config_sdl.h"

#include "Main.h"
#include "CharacterHandler.h"
#include "ArtHandler.h"
#include "EditHandler.h"

// Joueur
void playerMove(SDL_Renderer * render, SDL_Rect & player, int targetPlayerX, int targetPlayerY, int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y], int & currentArtWorkId, int museumID, int playerSpeed)
{
	int reelPlayerX = player.x + CHARACTER_SIZE / 2;
	int reelPlayerY = player.y + CHARACTER_SIZE / 2;
	int distanceX = 0;
	int distanceY = 0;
	float vecteurX = 0;
	float vecteurY = 0;
	float factor;

	// Supression du visiteur à l'ancienen position
	SDL_SetRenderDrawColor(render, 255, 174, 0, 255);
	SDL_RenderFillRect(render, &player);

	// Calcule de la nouvelle position
	if (reelPlayerX != targetPlayerX || reelPlayerY != targetPlayerY) {
		distanceX = targetPlayerX - reelPlayerX;
		distanceY = targetPlayerY - reelPlayerY;
		if (distanceX == 0) {
			vecteurY = (distanceY / abs(distanceY));
			player.y += playerSpeed * vecteurY;
		}
		else if (distanceY == 0) {
			vecteurX = (distanceX / abs(distanceX));
			player.x += playerSpeed * vecteurX;
		}
		else
		{
			factor = (float)distanceY / distanceX;
			if (factor < 1 && factor > -1) {
				vecteurX = (distanceX / abs(distanceX));
				vecteurY = vecteurX * factor;
			}
			else {
				vecteurY = (distanceY / abs(distanceY));
				vecteurX = vecteurY / factor;
			}
			player.x += playerSpeed * vecteurX;
			player.y += playerSpeed * vecteurY;
		}
	}

	// Bloque le joueur en direction de la gauche
	if (tabMap[(player.x - BORDER_SIZE) / BLOCK_SIZE_X][(player.y - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		|| tabMap[((player.x) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		player.y += playerSpeed;
		player.x += playerSpeed;
		player.y -= playerSpeed;
	}

	// Bloque le joueur en direction de la droite
	if (tabMap[((player.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		|| tabMap[((player.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		player.y -= playerSpeed;
		player.x -= playerSpeed;
		player.y += playerSpeed;
	}

	// Bloque le joueur en direction du haut
	if (tabMap[(player.x - BORDER_SIZE) / BLOCK_SIZE_X][(player.y - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		|| tabMap[((player.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		player.y += playerSpeed;
		player.x -= playerSpeed;
		player.x += playerSpeed;
	}

	// Bloque le joueur en direction du bas
	if (tabMap[((player.x) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		|| tabMap[((player.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		player.x += playerSpeed;
		player.y -= playerSpeed;
		player.x -= playerSpeed;
	}

	// Bloque coin haut-gauche
	if (tabMap[((player.x) - BORDER_SIZE) / BLOCK_SIZE_X][(player.y - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((player.x) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((player.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		player.y += playerSpeed;
		player.x += playerSpeed;
	}

	// Bloque coin haut-droit
	if (tabMap[((player.x) - BORDER_SIZE) / BLOCK_SIZE_X][(player.y - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((player.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((player.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		player.y += playerSpeed;
		player.x -= playerSpeed;
	}

	// Bloque coin bas-gauche
	if (tabMap[((player.x) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((player.x) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((player.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		player.y -= playerSpeed;
		player.x += playerSpeed;
	}

	// Bloque coin bas-droit
	if (tabMap[((player.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((player.x) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0
		&& tabMap[((player.x + CHARACTER_SIZE) - BORDER_SIZE) / BLOCK_SIZE_X][((player.y) - BORDER_SIZE) / BLOCK_SIZE_Y] > 0) {
		player.y -= playerSpeed;
		player.x -= playerSpeed;
	}
	else {
		SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
		SDL_RenderFillRect(render, &player);
		SDL_RenderPresent(render);
	}

	int artWorkId = NULL;
	getPlayerInGameLocation(reelPlayerX, reelPlayerY);
	if (getArtWorkUnderWatch(tabMap, reelPlayerX, reelPlayerY, artWorkId)) {
		if (artWorkId != currentArtWorkId) {
			displayArtWork(render, artWorkId, museumID);
			currentArtWorkId = artWorkId;
		}
	}
	else {
		if (currentArtWorkId != NULL) {
			displayBasicToolsComponents(render);
			displayHelpText(render);
			currentArtWorkId = NULL;
		}
	}
}
bool getArtWorkUnderWatch(int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y], int x, int y, int & artWorkId) {
	if (x + 1 < MAP_BLOCK_X && tabMap[x + 1][y] > 3) {
		artWorkId = tabMap[x + 1][y];
		return true;
	}
	else if (y + 1 < MAP_BLOCK_Y && tabMap[x][y + 1] > 3) {
		artWorkId = tabMap[x][y + 1];
		return true;
	}
	else if (x - 1 >= 0 && tabMap[x - 1][y] > 3) {
		artWorkId = tabMap[x - 1][y];
		return true;
	}
	else if (y - 1 >= 0 && tabMap[x][y - 1] > 3) {
		artWorkId = tabMap[x][y - 1];
		return true;
	}

	return false;
}
void getPlayerInGameLocation(int & x, int & y) {
	if (x > BORDER_SIZE && x < BORDER_SIZE + MAP_WIDTH && y > BORDER_SIZE && y < BORDER_SIZE + MAP_HEIGHT) {
		x = (x - BORDER_SIZE) / BLOCK_SIZE_X;
		y = (y - BORDER_SIZE) / BLOCK_SIZE_Y;
		return;
	}

	if (x > EDIT_BLOCKS_X + BORDER_SIZE && x < EDIT_BLOCKS_X + BLOCK_SIZE_X * 2 + BORDER_SIZE && y > PANEL_Y + BORDER_SIZE && y < PANEL_Y + (BLOCK_SIZE_X * 2 + 5) * 5 + BORDER_SIZE) {
		x = -1;
		y = (y - PANEL_Y - BORDER_SIZE) / (BLOCK_SIZE_X * 2 + 5);
		return;
	}

	x = NULL;
}

