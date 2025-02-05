#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define window_WIDTH 660
#define window_HEIGHT 700

#define RSPEED       10
#define BSPEED       10
#define VOLUME      100 	/* Volume 0..128 */
#define PAUSE_MS     16      /* 16ms => 60 FPS */

typedef struct GraphicItem
{
    SDL_Surface * surface;
    SDL_Rect position;
}PPong_GraphicItem;

typedef struct Table
{
    PPong_GraphicItem table;
    PPong_GraphicItem ball;
    PPong_GraphicItem paddle1, paddle2;
    PPong_GraphicItem net;
    PPong_GraphicItem topline, bottomline;
    PPong_GraphicItem rightline, leftline;
    SDL_Texture * texture;
}PPong_Table;

typedef struct Board
{
    PPong_GraphicItem board;
    PPong_GraphicItem title;
    PPong_GraphicItem time;
    PPong_GraphicItem score;
    TTF_Font *police;
    SDL_Texture * texture;
}PPong_Board;

typedef struct Game
{
    SDL_Window * window;
    SDL_Renderer * renderer;
    PPong_Table table;
    PPong_Board board;
    Mix_Chunk * paddle_hit;
    Mix_Chunk * touchline_hit;
    int * play, * p1, * p2;
}PPong_Game;

#endif // GAME_H
