/* Fonction et constante de gestion du menu */

int onPlayerMainMenuClick(int x, int y);
int onPlayerLevelMenuClick(int x, int y, int & difficulty, int & museumID, int museumQty);

void displayMuseumName(SDL_Renderer * render, TTF_Font * font, SDL_Rect & canvas, SDL_Color color, char title[]);
void displayMuseumDifficulty(SDL_Renderer * render, TTF_Font * font, SDL_Rect & canvas, SDL_Color color, char difficulty[]);
