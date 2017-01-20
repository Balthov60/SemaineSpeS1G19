#include <iostream>
#include <string>

#include "config_sdl.h"

#include "Main.h"
#include "ModHandler.h"
#include "ArtHandler.h"
#include "EditHandler.h"
#include "CharacterHandler.h"
#include "MapHandler.h"
#include "MenuHandler.h"
#include "MuseumService.h"

; using namespace std;

int launchPlayMod(SDL_Renderer * render, int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y], int timer, int mod, SDL_Rect & player, char mapPath[], bool & initGuards, int & guardsQty, SDL_Rect *& guards, int & ArtWorkSteelQty)
{
	// Initialisation des éléments du rendu principal
	initBasicsWindowComponents(render);
	displayBasicToolsComponents(render);
	displayTimer(render, timer);
	displayHelpText(render);

	// Affichage Personnages :
	if (mod != Pause || Game) {
		getMapFromFile(mapPath, tabMap);
		getPlayerSpawnPoint(player, mapPath);
		player.w = player.h = CHARACTER_SIZE;
		SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
		SDL_RenderFillRect(render, &player);
		SDL_RenderPresent(render);
	}

	// Affichage de la carte
	displayMap(render, tabMap);

	// Initialisation des gardes

	guardsQty = getGuardsQty(mapPath);
	delete[] guards;
	guards = new SDL_Rect[guardsQty];
	for (int i = 0; i < guardsQty; i++) {
		guards[i].h = guards[i].w = CHARACTER_SIZE;
		getGuardsSpawnPoint(guards[i], i + 1, mapPath);
	}

	initGuards = false;
	ArtWorkSteelQty = 0;

	return Game;
}
int launchEditMod(SDL_Renderer * render, int tabEdit[MAP_BLOCK_X][MAP_BLOCK_Y], int mod, char editMapPath[])
{
	if (mod != Edit) {
		// Suppression du plateau jeu
		initBasicsWindowComponents(render);
		displayBasicToolsComponents(render);
	}

	// Affichage des options de création de niveau
	displayEditTab(render, tabEdit, editMapPath);

	return Edit;
}
int launchMainMenuMod(SDL_Renderer * render) {
	SDL_Texture * texture;
	SDL_Rect MenuIm;

	texture = loadImage(render, ".\\data\\ImageMenu.png");
	MenuIm.x = 0;
	MenuIm.y = 0;
	MenuIm.w = WINDOW_WIDTH;
	MenuIm.h = WINDOW_HEIGHT;

	SDL_RenderCopy(render, texture, NULL, &MenuIm);
	SDL_RenderPresent(render);
	SDL_DestroyTexture(texture);

	return MainMenu;
}
int launchLevelMenuMod(SDL_Renderer * render, int mod, int & museumQty) {
	TTF_Init();

	SDL_Rect canvas;
	SDL_Texture * texture;
	SDL_Color color = { 0, 0, 0, 255 };
	TTF_Font* font = TTF_OpenFont(".\\data\\font\\DIGIFIT.TTF", 25);
	museumQty = getMuseumQty(".\\data\\bdMusees.sqlite");

	canvas.x = canvas.y = 0;
	canvas.w = WINDOW_WIDTH;
	canvas.h = WINDOW_HEIGHT;

	SDL_SetRenderDrawColor(render, 255, 174, 0, 255);
	SDL_RenderFillRect(render, &canvas);

	// Musee
	canvas.x = BORDER_SIZE * 8;
	canvas.w = WINDOW_WIDTH*(1 / 2);
	canvas.h = 15;
	canvas.y = 15;

	for (int i = 1; i <= museumQty && i; i++)
		displayMuseumName(render, font, canvas, color, getMuseumName(i, ".\\data\\bdMusees.sqlite"));

	// Difficulté
	canvas.x = WINDOW_WIDTH * 2 / 3 + BORDER_SIZE * 8;
	canvas.y = BORDER_SIZE * 6;
	canvas.w = WINDOW_WIDTH*(2 / 3);
	canvas.h = 15;

	texture = loadText(render, "DIFFICULTY", color, font);
	SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);
	SDL_RenderCopy(render, texture, NULL, &canvas);
	SDL_RenderPresent(render);

	if (mod != LevelMenu) {
		texture = loadImage(render, ".//data//point.png");
		canvas.x = WINDOW_WIDTH * 2 / 3 + BORDER_SIZE * 15 - 50;
		canvas.w = canvas.h = 25;
		canvas.y = 25 * BORDER_SIZE;
		SDL_RenderCopy(render, texture, NULL, &canvas);
	}

	canvas.x = WINDOW_WIDTH * 2 / 3 + BORDER_SIZE * 15;
	canvas.y = BORDER_SIZE * 10;
	canvas.w = WINDOW_WIDTH*(2 / 3);
	canvas.h = 15;

	displayMuseumDifficulty(render, font, canvas, color, "Easy");
	displayMuseumDifficulty(render, font, canvas, color, "Medium");
	displayMuseumDifficulty(render, font, canvas, color, "Hard");

	// double droite de séparation
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	canvas.x = WINDOW_WIDTH * 1 / 2;
	canvas.y = WINDOW_HEIGHT * 1 / 4;
	canvas.h = WINDOW_HEIGHT / 2;
	canvas.w = 5;
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderFillRect(render, &canvas);
	SDL_RenderPresent(render);

	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	canvas.x = WINDOW_WIDTH * 1 / 2 + 10;
	canvas.y = WINDOW_HEIGHT * 1 / 4;
	canvas.h = WINDOW_HEIGHT / 2 - 20;
	canvas.w = 5;
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderFillRect(render, &canvas);
	SDL_RenderPresent(render);

	// Souligner DIFFICULTY

	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	canvas.x = WINDOW_WIDTH * 2 / 3 + BORDER_SIZE * 8;
	canvas.y = BORDER_SIZE * 6 + 40;
	canvas.h = 5;
	canvas.w = 140;
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderFillRect(render, &canvas);
	SDL_RenderPresent(render);

	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
	TTF_Quit();
	return LevelMenu;
}

void initBasicsWindowComponents(SDL_Renderer * render) {
	SDL_Rect rectangle; // Création module de dessin

						// Creation de la bordure
	rectangle.x = 0;
	rectangle.y = 0;
	rectangle.w = WINDOW_WIDTH;
	rectangle.h = WINDOW_HEIGHT;

	SDL_SetRenderDrawColor(render, 0, 0, 0, 256);
	SDL_RenderFillRect(render, &rectangle);

	rectangle.x = BORDER_SIZE;
	rectangle.y = BORDER_SIZE;
	rectangle.w = WINDOW_WIDTH - BORDER_SIZE * 2;
	rectangle.h = WINDOW_HEIGHT - BORDER_SIZE * 2;

	SDL_SetRenderDrawColor(render, 255, 174, 0, 255);
	SDL_RenderFillRect(render, &rectangle);

	// Creation separation
	rectangle.x = BORDER_SIZE + MAP_WIDTH;
	rectangle.y = BORDER_SIZE;
	rectangle.w = BORDER_SIZE;
	rectangle.h = MAP_HEIGHT;

	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderFillRect(render, &rectangle);

	SDL_RenderPresent(render);
}

int launchGameEndMod(SDL_Renderer * render, int ArtWorkSteelQty, bool win) {
	SDL_Rect canvas;
	SDL_Texture * texture;
	SDL_Color color = { 0, 0, 0, 255 };
	TTF_Font* font = TTF_OpenFont(".\\data\\font\\DIGIFIT.TTF", 75);

	// Fond
	canvas.x = canvas.y = 0;
	canvas.w = WINDOW_WIDTH;
	canvas.h = WINDOW_HEIGHT;

	SDL_SetRenderDrawColor(render, 255, 174, 0, 255);
	SDL_RenderFillRect(render, &canvas);

	// GAME OVER
	texture = loadText(render, "GAME OVER", color, font);
	SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);

	canvas.x = (WINDOW_WIDTH - canvas.w) / 2;
	canvas.y = (WINDOW_HEIGHT / 2 - canvas.h) / 2;

	SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);
	SDL_RenderCopy(render, texture, NULL, &canvas);
	SDL_RenderPresent(render);

	font = TTF_OpenFont(".\\data\\font\\BAUHS93.TTF", 40);
	if (ArtWorkSteelQty < 0) {
		texture = loadText(render, "Le temps est ecoulé ...", color, font);
		SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);

		canvas.x = (WINDOW_WIDTH - canvas.w) / 2;
		canvas.y = (WINDOW_HEIGHT / 2 - canvas.h) / 2 + WINDOW_HEIGHT / 2;

		SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);
		SDL_RenderCopy(render, texture, NULL, &canvas);
		SDL_RenderPresent(render);
	}
	else if (!win) {
		texture = loadText(render, "Les gardes vous ont eu !!!", color, font);
		SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);

		canvas.x = (WINDOW_WIDTH - canvas.w) / 2;
		canvas.y = (WINDOW_HEIGHT / 2 - canvas.h) / 2 + WINDOW_HEIGHT / 2;

		SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);
		SDL_RenderCopy(render, texture, NULL, &canvas);
		SDL_RenderPresent(render);
	}
	else if (win && ArtWorkSteelQty == 0) {
		texture = loadText(render, "Perdu, vous n'avez rien volé ..", color, font);
		SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);

		canvas.x = (WINDOW_WIDTH - canvas.w) / 2;
		canvas.y = (WINDOW_HEIGHT / 2 - canvas.h) / 2 + WINDOW_HEIGHT / 2;

		SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);
		SDL_RenderCopy(render, texture, NULL, &canvas);
		SDL_RenderPresent(render);
	}
	else {
		char temp[100] = "Vous avez volé : ";
		string qty = "";
		qty += to_string(ArtWorkSteelQty);
		strcat(temp, qty.c_str());
		strcat(temp, " Oeuvre(s) d'art.");

		texture = loadText(render, temp, color, font);
		SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);

		canvas.x = (WINDOW_WIDTH - canvas.w) / 2;
		canvas.y = (WINDOW_HEIGHT / 2 - canvas.h) / 2 + WINDOW_HEIGHT / 2;

		SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);
		SDL_RenderCopy(render, texture, NULL, &canvas);
		SDL_RenderPresent(render);
	}

	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
	TTF_Quit();
	return Fin;
}