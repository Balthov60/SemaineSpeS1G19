#include <iostream>

#include "config_sdl.h"

#include "Main.h"
#include "MapHandler.h"
#include "EditHandler.h"
using namespace std;

// Fonctions "non joueur"
void displayEditTab(SDL_Renderer *render, int tabEdit[MAP_BLOCK_X][MAP_BLOCK_Y], char editMapPath[])
{
	initEmptyMapFile(editMapPath, 2);
	getMapFromFile(editMapPath, tabEdit);

	displayMap(render, tabEdit);
	displayBoxLimits(render, tabEdit);
	displayEditMenu(render);

	return;
}
void displayBoxLimits(SDL_Renderer * render, int tab[MAP_BLOCK_X][MAP_BLOCK_Y])
{
	SDL_Rect box;
	box.w = BLOCK_SIZE_X;
	box.h = BLOCK_SIZE_Y;
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);

	for (int i = 0; i < MAP_BLOCK_Y; i++) {
		for (int j = 0; j < MAP_BLOCK_X; j++) {
			if (tab[j][i] == 0) {
				box.x = BORDER_SIZE + BLOCK_SIZE_X * j;
				box.y = BORDER_SIZE + BLOCK_SIZE_Y * i;
				SDL_RenderDrawRect(render, &box);
			}
		}
	}
	SDL_RenderPresent(render);
}
void displayEditMenu(SDL_Renderer * render)
{
	SDL_Rect item = {};
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 15);
	SDL_Texture* texture;

	displayEditTitle(render);

	// Panel

	item.w = BLOCK_SIZE_X * 2;
	item.h = BLOCK_SIZE_X * 2;
	item.y = PANEL_Y - item.w;

	addPanelBox(render, item, { 255, 174, 0 }, font, " Bloc Vide");
	addPanelBox(render, item, { 0, 0, 0 }, font, " Bloc Mur");
	addPanelBox(render, item, { 255, 0, 0 }, font, " Bloc Garde");
	addPanelBox(render, item, { 0, 255, 0 }, font, " Bloc Entrée/Sortie");
	addPanelBox(render, item, { 0, 0, 255 }, font, " Bloc Oeuvre");
	addPanelBox(render, item, { 0, 0, 0 }, font, " Bloc selectioné"); // Par défaut le mur // ATTENTION : A changer aussi dans le main en cas de modifications

	item.x = EDIT_BLOCKS_X + 5;
	item.y = SAVE_Y + 5;

	texture = loadText(render, "Appuyer sur 's' pour Sauvegarder la map.", { 0, 0, 0 }, font);
	SDL_QueryTexture(texture, NULL, NULL, &item.w, &item.h);
	SDL_RenderCopy(render, texture, NULL, &item);

	item.y += item.h + 5;

	texture = loadText(render, "Appuyer sur 'r' pour reinitialiser la map.", { 0, 0, 0 }, font);
	SDL_QueryTexture(texture, NULL, NULL, &item.w, &item.h);
	SDL_RenderCopy(render, texture, NULL, &item);

	SDL_RenderPresent(render);

	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
	TTF_Quit();
}
void displayEditTitle(SDL_Renderer * render) {
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);

	SDL_Rect item;
	SDL_Texture * texture;
	texture = loadText(render, "Musee 3", { 0, 0, 0 }, font);
	SDL_QueryTexture(texture, NULL, NULL, &item.w, &item.h);

	item.x = EDIT_BLOCKS_X + (EDIT_BLOCKS_WIDTH - item.w) / 2;
	item.y = TITLE_Y + (TITLE_HEIGHT - item.h) / 2;

	SDL_RenderCopy(render, texture, NULL, &item);
	SDL_RenderPresent(render);
	SDL_DestroyTexture(texture);

	TTF_CloseFont(font);
	TTF_Quit();
}
void addPanelBox(SDL_Renderer * render, SDL_Rect & item, SDL_Color boxColor, TTF_Font * font, char boxName[]) {
	SDL_Texture * texture;

	item.w = BLOCK_SIZE_X * 2;
	item.h = BLOCK_SIZE_X * 2;
	item.x = EDIT_BLOCKS_X + 5;
	item.y += item.w + 5;

	cout << "DEBUG : " << boxName << " " << item.w << " " << item.h << " " << item.x << " " << item.y << endl;
	SDL_SetRenderDrawColor(render, boxColor.r, boxColor.g, boxColor.b, 255);
	SDL_RenderFillRect(render, &item);

	item.x += item.w + 25;
	item.y += BLOCK_SIZE_Y / 2;
	texture = loadText(render, boxName, { 0, 0, 0 }, font);
	SDL_QueryTexture(texture, NULL, NULL, &item.w, &item.h);
	SDL_RenderCopy(render, texture, NULL, &item);

	item.y -= BLOCK_SIZE_Y / 2;
	SDL_DestroyTexture(texture);
}

// Fonctions "Joueur"
void onPlayerPanelClick(SDL_Renderer * render, int targetPlayerY, SDL_Color & currentEditColor) {
	SDL_Rect tile;

	switch (targetPlayerY) {
	case 0:
		currentEditColor = { 255, 174, 0, 255 };
		break;
	case 1:
		currentEditColor = { 0, 0, 0, 255 };
		break;
	case 2:
		currentEditColor = { 255, 0, 0, 255 };
		break;
	case 3:
		currentEditColor = { 0, 255, 0, 255 };
		break;
	case 4:
		currentEditColor = { 0, 0, 255, 255 };
		break;
	}

	tile.w = BLOCK_SIZE_X * 2;
	tile.h = BLOCK_SIZE_Y * 2;
	tile.x = EDIT_BLOCKS_X + 5;
	tile.y = 303; // ATTENTION : a Changer si le menu change

	SDL_SetRenderDrawColor(render, currentEditColor.r, currentEditColor.g, currentEditColor.b, currentEditColor.a);
	SDL_RenderFillRect(render, &tile);
	SDL_RenderPresent(render);
}
void onPlayerBuildMap(SDL_Renderer * render, int targetPlayerX, int targetPlayerY, SDL_Color currentEditColor, int tabEdit[MAP_BLOCK_X][MAP_BLOCK_Y]) {
	SDL_Rect tile;
	tile.w = BLOCK_SIZE_X;
	tile.h = BLOCK_SIZE_Y;
	tile.x = targetPlayerX * BLOCK_SIZE_X + BORDER_SIZE;
	tile.y = targetPlayerY * BLOCK_SIZE_Y + BORDER_SIZE;
 
	SDL_SetRenderDrawColor(render, currentEditColor.r, currentEditColor.g, currentEditColor.b, currentEditColor.a);
	SDL_RenderFillRect(render, &tile);

	if (currentEditColor.r == 0 && currentEditColor.g == 0 && currentEditColor.b == 0)
		tabEdit[targetPlayerX][targetPlayerY] = 1;
	else if (currentEditColor.r == 255 && currentEditColor.g == 174 && currentEditColor.b == 0) {
		tabEdit[targetPlayerX][targetPlayerY] = 0;
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		SDL_RenderDrawRect(render, &tile);
	}
	else if (currentEditColor.r == 255 && currentEditColor.g == 0 && currentEditColor.b == 0)
		tabEdit[targetPlayerX][targetPlayerY] = 2;
	else if (currentEditColor.r == 0 && currentEditColor.g == 255 && currentEditColor.b == 0)
		tabEdit[targetPlayerX][targetPlayerY] = 3;
	else if (currentEditColor.r == 0 && currentEditColor.g == 0 && currentEditColor.b == 255)
		tabEdit[targetPlayerX][targetPlayerY] = 4;

	SDL_RenderPresent(render);
}