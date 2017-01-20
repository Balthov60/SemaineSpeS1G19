// Constantes d'affichage.

const int BORDER_SIZE = 5;
const int WINDOW_WIDTH = 1008 + BORDER_SIZE * 2;
const int WINDOW_HEIGHT = 504 + BORDER_SIZE * 2;

const int MAP_WIDTH = (WINDOW_WIDTH - BORDER_SIZE * 2) / 3 * 2;
const int MAP_HEIGHT = WINDOW_HEIGHT - BORDER_SIZE * 2;
const int TOOLS_WIDTH = (WINDOW_WIDTH - BORDER_SIZE * 2) / 3 - BORDER_SIZE;
const int TOOLS_HEIGHT = WINDOW_HEIGHT - BORDER_SIZE * 2;

const int MAP_BLOCK_X = 32;
const int MAP_BLOCK_Y = 24;
const int BLOCK_SIZE_X = MAP_WIDTH / MAP_BLOCK_X;
const int BLOCK_SIZE_Y = MAP_HEIGHT / MAP_BLOCK_Y;