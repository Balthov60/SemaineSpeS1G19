/* Fonctions et constance de déplacement du joueur */

const int CHARACTER_SIZE = 10; // Dimension du personnagess

// Joueur
void playerMove(SDL_Renderer * render, SDL_Rect & player, int targetPlayerX, int targetPlayerY, int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y], int & currentArtWorkId, int museumID, int playerSpeed);
bool getArtWorkUnderWatch(int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y], int x, int y, int & artWorkId);
void getPlayerInGameLocation(int & x, int & y);

// Guardes
void guardMove(SDL_Renderer * render, SDL_Rect & guard, SDL_Rect player, int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y], int guardsSpeed);
int getGuardsQty(char mapPath[]);
void getGuardsSpawnPoint(SDL_Rect & guard, int index, char mapPath[]);
