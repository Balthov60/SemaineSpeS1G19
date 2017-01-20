#include <iostream>

#include "config_sdl.h"

#include "Main.h"
#include "ModHandler.h"
#include "MenuHandler.h"

using namespace std;

int onPlayerMainMenuClick(int x, int y)
{
	if (x < WINDOW_WIDTH / 4 && x > 0 && y > WINDOW_HEIGHT / 3 * 2 && y < WINDOW_HEIGHT) {
		cout << "DEBUG : Launch Edit" << endl;
		return Edit;
	}
	else if (x > WINDOW_WIDTH / 4 * 3 && x < WINDOW_WIDTH && y > WINDOW_HEIGHT / 3 * 2 && y < WINDOW_HEIGHT) {
		cout << "DEBUG : Launch Level Menu" << endl;
		return LevelMenu;
	}

}
int onPlayerLevelMenuClick(int x, int y, int & difficulty, int & museumID, int museumQty) {
	if (x > WINDOW_WIDTH * 2 / 3 + BORDER_SIZE * 15 && x < WINDOW_WIDTH && y > BORDER_SIZE * 25 && y < BORDER_SIZE * 60) {
		difficulty = (y - BORDER_SIZE * 25) / (BORDER_SIZE * 40 / 3) + 1;
		cout << "DEBUG : difficulty " << difficulty << endl;
		return -difficulty;
	}

	if (x > BORDER_SIZE * 8 && x < WINDOW_WIDTH  / 2 && y > 20 && y < 20 + 60 * museumQty) {
		museumID = (y - 20) / 55 + 1;
		return Game;
	}

	return LevelMenu;
}

void displayMuseumName(SDL_Renderer * render, TTF_Font * font, SDL_Rect & canvas, SDL_Color color, char title[]) 
{
	canvas.y += canvas.h + BORDER_SIZE;
	SDL_Texture* texture = loadText(render, title, color, font);

	SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);
	SDL_RenderCopy(render, texture, NULL, &canvas);
	
	SDL_DestroyTexture(texture);
	SDL_RenderPresent(render);
}
void displayMuseumDifficulty(SDL_Renderer * render, TTF_Font * font, SDL_Rect & canvas, SDL_Color color, char difficulty[]) {
	canvas.y += 15 * BORDER_SIZE;
	SDL_Texture * texture = loadText(render, difficulty, color, font);

	SDL_QueryTexture(texture, NULL, NULL, &canvas.w, &canvas.h);
	SDL_RenderCopy(render, texture, NULL, &canvas);

	SDL_DestroyTexture(texture);
	SDL_RenderPresent(render);
}