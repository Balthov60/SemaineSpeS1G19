/* fonctions et variables global de la carte */

void getMapFromFile(char map_path[], int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y]);
void initEmptyMapFile(char map_path[], int guardsQty);
void exportMapIntoFile(char map_path[], int tabEdit[MAP_BLOCK_X][MAP_BLOCK_Y]); 
void displayMap(SDL_Renderer *render, int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y]);

void getPlayerSpawnPoint(SDL_Rect & player, char mapFile[]);
