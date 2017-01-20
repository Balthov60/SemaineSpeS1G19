#include <fstream>
#include <iostream>

#include "config_sdl.h"

#include "Main.h"
#include "MapHandler.h"

using namespace std;

void getMapFromFile(char map_path[], int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y]) 
{
	ifstream file(map_path, ios::in);
	char temp[100];

	if (!file) {
		cout << "Probleme d'ouverture \n";
		return;
	}

	do {
		file.getline(temp, 100);
	} while (strcmp(temp, "Map :") != 0);

	while (!file.eof()) 
	{
		for (int i = 0; i < MAP_BLOCK_Y; i++) {
			for (int j = 0; j < MAP_BLOCK_X - 1; j++) {
				file.getline(temp, 5, ' ');
				tabMap[j][i] = atoi(temp);
			}
			file.getline(temp, 100, '\n');
			tabMap[MAP_BLOCK_X - 1][i] = atoi(temp);
		}
	}
	file.close();

	cout << "DEBUG MAP Matrice : " << endl;
	for (int i = 0; i < MAP_BLOCK_Y; i++) {
		for (int j = 0; j < MAP_BLOCK_X; j++)
			cout << tabMap[j][i];
		cout << endl;
	}
}

void initEmptyMapFile(char map_path[], int guardsQty)
{
	ofstream mapFile(map_path, ios::out);

	mapFile << "V 0.5 0.5" << endl;

	mapFile << "G " << guardsQty << endl;
	for (int i = 1; i <= guardsQty; i++)
		mapFile << "G " << i << ".5 " << i << ".5" << endl;

	mapFile << "Map :" << endl;
	for (int i = 0; i < MAP_BLOCK_Y - 1; i++) {
		for (int j = 0; j < MAP_BLOCK_X - 1; j++) {
			if (i == 0 || j == 0 || i == MAP_BLOCK_Y - 1 || j == MAP_BLOCK_X - 1)
				mapFile << "1 ";
			else
				mapFile << "0 ";
		}
		mapFile << "1";
		mapFile << endl;
	}
	for (int j = 0; j < MAP_BLOCK_X - 1; j++)
		mapFile << "1 ";
	mapFile << "1";

	mapFile.close();
}

void exportMapIntoFile(char map_path[], int tabEdit[MAP_BLOCK_X][MAP_BLOCK_Y]) {
	ofstream mapFile(map_path, ios::out);

	for (int i = 0; i < MAP_BLOCK_Y; i++) {
		for (int j = 0; j < MAP_BLOCK_X; j++) {
			if (tabEdit[j][i] == 3) {
				if (j > 0 && i + 1 > 0 && tabEdit[j][i + 1] == 0)
					mapFile << "V " << j << " " << i + 1 << endl;
				else if (j > 0 && i - 1 > 0 && tabEdit[j][i - 1] == 0)
					mapFile << "V " << j << " " << i - 1 << endl;
				else if (j + 1 > 0 && i > 0 && tabEdit[j + 1][i] == 0)
					mapFile << "V " << j + 1 << " " << i << endl;
				else if (j - 1 > 0 && i > 0 && tabEdit[j - 1][i] == 0)
					mapFile << "V " << j - 1 << " " << i << endl;
				break;
			}
		}
	}

	for (int i = 0; i < MAP_BLOCK_Y - 1; i++) {
		for (int j = 0; j < MAP_BLOCK_X - 1; j++) {
			if (tabEdit[j][i] == 2)
				mapFile << "G " << j << ".5 " << i << ".5" << endl;
		}
	}

	mapFile << "Map :" << endl;
	int countArtWork = 0;
	for (int i = 0; i < MAP_BLOCK_Y - 1; i++) {
		for (int j = 0; j < MAP_BLOCK_X - 1; j++) {
			if (tabEdit[j][i] == 4) {
				if (countArtWork < 6) {
					tabEdit[j][i] = countArtWork + 4;
					countArtWork++;
				}
				else
					mapFile << "0 ";
			}
			mapFile << tabEdit[j][i] << " ";
		}
		if (tabEdit[MAP_BLOCK_X - 1][i] == 4) {
			if (countArtWork < 6) {
				tabEdit[MAP_BLOCK_X - 1][i] = countArtWork + 4;
				countArtWork++;
			}
			else
				mapFile << "0 ";
		}
		mapFile << tabEdit[MAP_BLOCK_X - 1][i] << " ";
		mapFile << endl;
	}
	for (int j = 0; j < MAP_BLOCK_X - 1; j++) {
		if (tabEdit[j][MAP_BLOCK_Y - 1] == 4) {
			if (countArtWork < 6) {
				tabEdit[j][MAP_BLOCK_Y - 1] = countArtWork + 4;
				countArtWork++;
			}
			else
				mapFile << "0 ";
		}
		mapFile << tabEdit[j][MAP_BLOCK_Y - 1] << " ";
	}
	mapFile << tabEdit[MAP_BLOCK_X - 1][MAP_BLOCK_Y - 1];

	mapFile.close();
}

void displayMap(SDL_Renderer *render, int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y]) 
{
	SDL_Rect block;
	block.w = BLOCK_SIZE_X;
	block.h = BLOCK_SIZE_Y;

	for (int i = 0; i < MAP_BLOCK_Y; i++) {
		for (int j = 0; j < MAP_BLOCK_X; j++) {
			if (tabMap[j][i] == 1)
				SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
			else if (tabMap[j][i] == 0 || tabMap[j][i] == 2) {
				SDL_SetRenderDrawColor(render, 255, 174, 0, 255);
			}
			else if (tabMap[j][i] == 3)
				SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
			else
				SDL_SetRenderDrawColor(render, 0, 0, 255, 255);

			block.x = BORDER_SIZE + BLOCK_SIZE_X * j;
			block.y = BORDER_SIZE + BLOCK_SIZE_Y * i;

			SDL_RenderFillRect(render, &block);
		}
	}
	SDL_RenderPresent(render);
}

void getPlayerSpawnPoint(SDL_Rect & player, char mapFile[]) {
	ifstream file(mapFile, ios::in);
	char temp[100];

	if (!file) {
		cout << "Probleme d'ouverture \n";
		return;
	}

	file.getline(temp, 5, ' ');

	file.getline(temp, 5, ' ');
	player.x = (atoi(temp) + 0.5) * BLOCK_SIZE_X;

	file.getline(temp, 5, ' ');
	player.y = (atoi(temp) + 0.5) * BLOCK_SIZE_Y;

	file.close();
}