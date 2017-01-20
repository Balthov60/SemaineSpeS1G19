/* Fonction et constante d'édition de la map */

const int INBLOCK_BORDER_EDIT = 5;
const int EDIT_BLOCKS_WIDTH = TOOLS_WIDTH - 2 * INBLOCK_BORDER_EDIT;
const int EDIT_POS_X = WINDOW_WIDTH - TOOLS_WIDTH - BORDER_SIZE;
const int EDIT_POS_Y = BORDER_SIZE;
const int EDIT_BLOCKS_X = EDIT_POS_X + INBLOCK_BORDER_EDIT;

const int TITLE_HEIGHT = (TOOLS_HEIGHT - 4 * INBLOCK_BORDER_EDIT) / 10;
const int TITLE_Y = EDIT_POS_Y + INBLOCK_BORDER_EDIT;
const int PANEL_HEIGHT = (TOOLS_HEIGHT - 4 * INBLOCK_BORDER_EDIT) / 10 * 6;
const int PANEL_Y = EDIT_POS_Y + 2 * INBLOCK_BORDER_EDIT + TITLE_HEIGHT;
const int SAVE_HEIGHT = (TOOLS_HEIGHT - 4 * INBLOCK_BORDER_EDIT) / 10 * 3;
const int SAVE_Y = EDIT_POS_Y + 3 * INBLOCK_BORDER_EDIT + TITLE_HEIGHT + PANEL_HEIGHT;

// Fonctions "non joueur".
void displayEditTab(SDL_Renderer *render, int tabEdit[MAP_BLOCK_X][MAP_BLOCK_Y], char editMapPath[]);
void displayBoxLimits(SDL_Renderer * render, int tab[MAP_BLOCK_X][MAP_BLOCK_Y]);
void displayEditMenu(SDL_Renderer * render);
void addPanelBox(SDL_Renderer * render, SDL_Rect & item, SDL_Color boxColor, TTF_Font * font, char boxName[]);
void displayEditTitle(SDL_Renderer * render);

// Fonctions "joueur".
void onPlayerPanelClick(SDL_Renderer * render, int targetPlayerY, SDL_Color & currentEditColor);
void onPlayerBuildMap(SDL_Renderer * render, int targetPlayerX, int targetPlayerY, SDL_Color currentEditColor, int tabEdit[MAP_BLOCK_X][MAP_BLOCK_Y]);