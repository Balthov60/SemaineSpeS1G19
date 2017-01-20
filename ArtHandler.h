/* Fonctions d'affichage des oeuvres */

const int INBLOCK_BORDER = 5;
const int TOOLS_BLOCKS_WIDTH = TOOLS_WIDTH - 2 * INBLOCK_BORDER;
const int POS_X = WINDOW_WIDTH - TOOLS_WIDTH - BORDER_SIZE;
const int POS_Y = BORDER_SIZE;
const int TOOLS_BLOCKS_X = POS_X + INBLOCK_BORDER;

const int TIMER_HEIGHT = (TOOLS_HEIGHT - 4 * INBLOCK_BORDER) / 10;
const int TIMER_Y = POS_Y + INBLOCK_BORDER;
const int ARTWORK_HEIGHT = (TOOLS_HEIGHT - 4 * INBLOCK_BORDER) / 10 * 6;
const int ARTWORK_Y = POS_Y + 2 * INBLOCK_BORDER + TIMER_HEIGHT;
const int DESCRIPTION_HEIGHT = (TOOLS_HEIGHT - 4 * INBLOCK_BORDER) / 10 * 3;
const int DESCRIPTION_Y = POS_Y + 3 * INBLOCK_BORDER + TIMER_HEIGHT + ARTWORK_HEIGHT;

void displayBasicToolsComponents(SDL_Renderer * render);
void displayHelpText(SDL_Renderer * render);

void displayTimer(SDL_Renderer * render, int currentTime);
void displayArtWork(SDL_Renderer * render, int artWorkID, int museumID);
void displayArtImage(SDL_Renderer * render, char fileName[]);
void displayArtTitle(SDL_Renderer * render, char artTitle[], SDL_Rect & canvas);
void displayArtDescription(SDL_Renderer * render, char artDescription[], SDL_Rect & canvas);