#ifndef DISPLAY_H 
#define DISPLAY_H

#include "game.h"

#define BITBIXEL     32
#define GREEN ((SDL_Color){0,255,0})

int create_game_graphicItems(PPong_Game * game);
void set_graphicItems_positions(PPong_Game * game);

int display(PPong_Game * game);

void free_video_memory( PPong_Game * game);

#endif // DISPLAY_H

