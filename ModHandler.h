/* Fonctions de changement de modes */

; enum  Modes { Game, Edit, MainMenu, LevelMenu, Pause, Fin };

// Lancement des différents modes
int launchPlayMod(SDL_Renderer * render, int tabMap[MAP_BLOCK_X][MAP_BLOCK_Y], int timer, int mod, SDL_Rect & player, char mapPath[], bool & initGuards, int & guardsQty, SDL_Rect *& guards, int & ArtWorkSteelQty);
int launchEditMod(SDL_Renderer * render, int tabEdit[MAP_BLOCK_X][MAP_BLOCK_Y], int mod, char editMapPath[]);
int launchMainMenuMod(SDL_Renderer * render);
int launchLevelMenuMod(SDL_Renderer * render, int mod, int & museumQty);
int launchGameEndMod(SDL_Renderer * render, int ArtWorkSteelQty, bool win);

// Affichage du fond commun
void initBasicsWindowComponents(SDL_Renderer * render);