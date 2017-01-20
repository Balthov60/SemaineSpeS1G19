#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

SDL_Texture *loadImage(SDL_Renderer* rendu,char *name, int transparency=200, int r=-1, int g=-1, int b=-1);
SDL_Texture *loadText(SDL_Renderer* rendu,char *text, SDL_Color coul, TTF_Font* font);