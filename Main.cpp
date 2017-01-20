#include <iostream>
#include <fstream>
#include "sqlite3.h"
#include "fonctionsBD.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include "config_sdl.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL2_rotozoom.h"

#include "MuseumService.h"
#include "Main.h"
#include "ArtHandler.h"
#include "EditHandler.h"
#include "MapHandler.h"
#include "ModHandler.h"
#include "MenuHandler.h"
#include "CharacterHandler.h"

using namespace std;

int main(int argn, char* argv[]) {

	// Initialisation SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "SDL ERROR" << endl;
		return 1;
	}

	TTF_Init();

	// Création de la fenetre
	SDL_Window* win = SDL_CreateWindow("Semaine Spe S1",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	// Création du rendu
	SDL_Renderer *render = SDL_CreateRenderer(
		win,
		-1,
		SDL_RENDERER_ACCELERATED);

	// Initialisation du mode de jeu, du choix de la carte et de l'id de l'oeuvre
	int mod = NULL;
	int museumID = 1;
	int difficulty = 1;
	int currentArtWorkId = NULL;
	int timer = 60 * 5 * 1000;
	int ArtWorkSteelQty = 0;

	char mapPath[20];
	char editMapPath[20] = ".\\data\\musee3.txt";
	char temp[5];
	int museumQty = getMuseumQty(".\\data\\bdMusees.sqlite");

	// Création des tableaux de carte;
	int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y];
	int tabEdit[MAP_BLOCK_X][MAP_BLOCK_Y];
	
	// Lancement du menu
	mod = launchMainMenuMod(render);

	// Creation des personnages
	SDL_Rect player;
	int targetPlayerX;
	int targetPlayerY;
	int playerSpeed = 5;

	// Creation gardes
	bool initGuards = false;
	int guardsQty = 0;
	SDL_Rect * Guards = NULL;
	int guardsSpeed = 2;

	// Lancement du jeu
	SDL_RenderPresent(render);
	bool isRunning = true;

	SDL_Rect editPaint;
	SDL_Event event;
	SDL_Color currentEditColor = { 0, 0, 0, 255 };
	SDL_Texture * point = loadImage(render, ".//data//point.png");

	while (isRunning)
	{
		if (mod == Game) {
			SDL_Delay(50); // Temps de refresh
			timer -= 50;
			displayTimer(render, timer);
			SDL_PollEvent(&event);

			if (timer <= 0) {
				mod = launchGameEndMod(render, -1, false);
			}
		}
		else
			SDL_WaitEvent(&event);

		switch (event.type)
		{

			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_MOUSEMOTION:
				if (mod == Game)
					SDL_GetMouseState(&targetPlayerX, &targetPlayerY);
				break;
			
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) { // Menu
					mod = launchMainMenuMod(render);
					break;
				}
				else if (event.key.keysym.sym == SDLK_r) { // reset Edit
					if (mod == Edit)
						launchEditMod(render, tabEdit, mod, editMapPath);
					break;
				}
				else if (event.key.keysym.sym == SDLK_s) { // export Edit
					if (mod == Edit) {
						exportMapIntoFile(editMapPath, tabEdit);
						mod = launchMainMenuMod(render);
					}
					break;
				}
				else if (event.key.keysym.sym == SDLK_SPACE) {
					if (mod == Game) {
						int x = player.x; int y = player.y; int artWorkId;
						getPlayerInGameLocation(x, y);
						if (getArtWorkUnderWatch(tabMap, x, y, artWorkId)) {
							ArtWorkSteelQty++;
							for (int i = 0; i < MAP_BLOCK_Y; i++) {
								for (int j = 0; j < MAP_BLOCK_X - 1; j++) {
									if (tabMap[j][i] == artWorkId) {
										tabMap[j][i] = 1;
										SDL_Rect replace;
										replace.w = BLOCK_SIZE_X;
										replace.h = BLOCK_SIZE_Y;
										replace.x = BORDER_SIZE + j * BLOCK_SIZE_X;
										replace.y = BORDER_SIZE + i * BLOCK_SIZE_Y;
										SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
										SDL_RenderFillRect(render, &replace);
										SDL_RenderPresent(render);
									}
								}
							}
						}
						if (tabMap[x + 1][y] == 3 || tabMap[x][y + 1] == 3 || tabMap[x - 1][y] == 3 || tabMap[x][y - 1] == 3) {
							cout << "DEBUG : OUT" << endl;
							mod = launchGameEndMod(render, ArtWorkSteelQty, true);
						}
					}
					break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (mod == MainMenu) {
					if (!event.button.button == SDL_BUTTON_LEFT)
						break;

					int click = onPlayerMainMenuClick(event.button.x, event.button.y);
					if (click == Edit)
						mod = launchEditMod(render, tabEdit, mod, editMapPath);
					else if (click == LevelMenu) {
						mod = launchLevelMenuMod(render, mod, museumQty);
						editPaint.x = WINDOW_WIDTH * 2 / 3 + BORDER_SIZE * 15 - 50;
						editPaint.w = editPaint.h = 25;
						editPaint.y = 25 * BORDER_SIZE;
					}
				}
				else if (mod == LevelMenu) {
					if (!event.button.button == SDL_BUTTON_LEFT)
						break;
					int click = onPlayerLevelMenuClick(event.button.x, event.button.y, difficulty, museumID, museumQty);
					if (click < 0 && click > -4) {
						SDL_SetRenderDrawColor(render, 255, 174, 0, 255);
						SDL_RenderFillRect(render, &editPaint);

						editPaint.x = WINDOW_WIDTH * 2 / 3 + BORDER_SIZE * 15 - 50;
						editPaint.w = editPaint.h = 25;
						editPaint.y = 10 * BORDER_SIZE + BORDER_SIZE * (abs(click) * 15);
						SDL_RenderCopy(render, point, NULL, &editPaint);
						SDL_RenderPresent(render);

						timer = 60 * 5 * 1000 / difficulty;
						if (difficulty == 1) {
							playerSpeed = 5;
							guardsSpeed = 2;
						}
						else if (difficulty == 2) {
							playerSpeed = 6;
							guardsSpeed = 2;
						}
						else if (difficulty == 3) {
							playerSpeed = 6;
							guardsSpeed = 4;
						}
						break;
					}
					else if (click == Game) {
						strcpy(mapPath, ".\\data\\musee");
						sprintf(temp, "%d", museumID);
						strcat(mapPath, temp);
						strcat(mapPath, ".txt");
						mod = launchPlayMod(render, tabMap, timer, mod, player, mapPath, initGuards, guardsQty, Guards, ArtWorkSteelQty);
						break;
					}
				}
				else if (mod == Edit)
				{
					if (!event.button.button == SDL_BUTTON_LEFT)
						break;

					targetPlayerX = event.button.x;
					targetPlayerY = event.button.y;
					getPlayerInGameLocation(targetPlayerX, targetPlayerY);
					if (targetPlayerX < -1) {
						break;
					}
					if (targetPlayerX == -1) { // Panel Click
						onPlayerPanelClick(render, targetPlayerY, currentEditColor);
						break;
					}

					onPlayerBuildMap(render, targetPlayerX, targetPlayerY, currentEditColor, tabEdit);
				}
				break;

			default:
				break;
		}
		if (mod == Game) {
			playerMove(render, player, targetPlayerX, targetPlayerY, tabMap, currentArtWorkId, museumID, playerSpeed);
			SDL_bool SDL_HasIntersection(const SDL_Rect * player, const SDL_Rect * Guard); //Gestion des collisions entre joueur et gardiens
			for (int i = 0; i < guardsQty; i++) {
				if (SDL_HasIntersection(&player, &Guards[i])) {
					mod = launchGameEndMod(render, 0, false);
					break;
				}
				if (ArtWorkSteelQty > 0 || !initGuards) {
					for (int i = 0; i < guardsQty; i++) {
						guardMove(render, Guards[i], player, tabMap, guardsSpeed);
					}
				}
				initGuards = true;
			}
		}
	}
	// destruction guardes
	delete[] Guards;

	// destruction SDL
	SDL_DestroyRenderer(render);

	SDL_DestroyWindow(win);
	TTF_Quit();

	SDL_Quit();
	return 0;
}