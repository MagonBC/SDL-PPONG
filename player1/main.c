/*

SDL-PPONG: An implementation of the Table Tennis game using C and SDL.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Copyright (C) 2010-2016 Issam Abdallah, Tunisia
Email:iabdallah@yandex.com
Web Site: http://issamabd.com
*/

#include <errno.h>
#include <string.h> /* strcpy() */
#include <getopt.h> /* to scan command options */
#include <unistd.h>
#include <pthread.h>

#include "display.h"
#include "moveball.h"


/*
 * Two header files generated by using rpcgen. 
 * They contain prototypes of remote functions.
 * 
 */

#include "player1_rpc.h"
#include "player2_rpc.h"

static int SDLlibs_init();
static void scan_options(int argc, char *argv[], char ** ip);
static void print_usage (char * program_name, 
                    FILE * stream, int exit_code);
extern int register_player1_service();

/* 
 * As we cannot pass them as parameters to remote functions,
 * we have to declare them as global variables! 
 * (see player1_rpc_server.c)
 * 
 */
SDL_TimerID timer;
PPong_Game game;

void *run_svc(void *arg) {
        svc_run();
}

/* main */
int main(int argc, char *argv[])
{
    
 SDL_Event event;

 int play = 1, p1 = 0, p2 = 0;
 int play_with_computer = 0;
 game.play = &play; // to not use malloc()!
 game.p1 = &p1;
 game.p2 = &p2;
 int paddle1_x,paddle2_x;
   
 char *ip = NULL;
 CLIENT * cl;
 enum clnt_stat result;
 int *clnt_res = (int*)malloc(sizeof (int));
 pthread_t  thread_1;

/* First, we scan command options (refer to GNU getopt.h API) */ 
  scan_options(argc, argv, &ip);
 
/* verify that player2 IP was scanned */
  if(!ip)
      print_usage(argv[0], stderr, EXIT_FAILURE);

/*
 * Associates prognum and versnum with the service dispatch procedure and 
 * call the portmap service to register it, so that the client (player2) can
 * call the remote functions defined in player1_rpc_server.c.
 *
 * NOTE: the rpcbind service must run first on the local host!
 */

  if(!register_player1_service())
    return EXIT_FAILURE;

/* SDL initialisation */
  if(!SDLlibs_init())
    exit(EXIT_FAILURE);

/* graphic Interface creation */
  game.window = SDL_CreateWindow("Player 1 ( P1 )",
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          window_WIDTH, window_HEIGHT,
                          0);

  game.renderer = SDL_CreateRenderer(game.window, -1, 0);

  if(!create_game_graphicItems(&game))
    exit(EXIT_FAILURE);

  set_graphicItems_positions(&game);

/* police */
  game.board.police = TTF_OpenFont("data/digital.ttf", 20);

/* sounds */
  Mix_OpenAudio(22050, AUDIO_U8, 1, 512);
  game.paddle_hit = Mix_LoadWAV("data/hit.wav");
  game.touchline_hit = Mix_LoadWAV("data/mur.wav");
  Mix_VolumeChunk(game.paddle_hit, VOLUME);
  Mix_VolumeChunk(game.touchline_hit, VOLUME);
	
/* Keyboard & Mouse */
  SDL_ShowCursor(0);

/* display the graphic interface for the first time */  
  display(&game);

/* Begin serving player2 requests */
  int ret = pthread_create(&thread_1, NULL, run_svc, NULL);

/* Create RPC client to connect to player2 */
  cl = clnt_create(ip, PLAYER2PROG, PLAYER2VERS, "tcp");
  if (cl == NULL)
    {
        /*
         * Couldn't establish connection with player1 for RPC calls: play with computer!
         * Print an error message anyway and cancel svc_run thread to not serve p2 requests!
         */

          clnt_pcreateerror(ip);
          printf("Playing against the computer...\n");
          pthread_cancel(thread_1);
          play_with_computer = 1;
    }

/* initial x coordinate of paddle1 */
  paddle1_x = game.table.paddle1.position.x;

/*
 * Every thing goes OK, Run the game and have fun :)
 */
  while(play)
  {
    /* poll for currently pending events, without blocking the loop! */
    if(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            /* quit the game */
            case SDL_QUIT: play = 0;
            break;

            case SDL_KEYDOWN:

	                switch(event.key.keysym.sym)
		        {
                         /* move paddle1 to the right by 1 pixel */
			 case SDLK_RIGHT :     paddle1_x += RSPEED;
		         break;

                         /* move paddle1 to the left by 1 pixel */
			 case SDLK_LEFT  :     paddle1_x -= RSPEED;
			 break;
             /* move paddle2 to the right by 1 pixel */
             case SDLK_v:    paddle2_x += RSPEED;
             break;

             /* move paddle2 to the left by 1 pixel */
             case SDLK_x :    paddle2_x -= RSPEED;
             break;

                         /* launch the ball: trigger the timer */
			 case SDLK_SPACE:

                             if(((game.table.ball.position.y +
                                 game.table.ball.surface->h)
                                == game.table.paddle1.position.y
                               ) ||
                        ((game.table.ball.position.y ==
                                                         (game.table.paddle2.position.y +
                                                         game.table.paddle2.surface->h)
                                                       )) && play_with_computer == 1)
                             {
                                /* First, launch the ball on player2 side */
                                if (play_with_computer == 0){
                                  result = launchballp2_1((void*)0, clnt_res, cl);
                                  if (result != 0)
                                  {
                                      /* the ball on player2 side was not launched */
                                     clnt_perror (cl, "call failed");
                                      continue;
                                  }else
                                  {
                                      SDL_RemoveTimer(timer);
                                      timer = SDL_AddTimer(BSPEED ,MoveBall, &game);
                                  }
                                }else
                                {
                                      SDL_RemoveTimer(timer);
                                      timer = SDL_AddTimer(BSPEED ,MoveBall, &game);
                                }
                             }

			 break;

			 case SDLK_q:	play = 0;

             break;

			 default:
			 break;
		      }

           break;
           default :
           break;
        }
    }

    /* update paddle1 x position */
    if( (game.table.paddle1.position.x + game.table.paddle1.surface->w)
            > (game.table.table.surface->w -5)
      )
        paddle1_x = game.table.table.surface->w -5 -game.table.paddle1.surface->w;

    if(game.table.paddle1.position.x < 3)
        paddle1_x = 3;
    
    game.table.paddle1.position.x= paddle1_x;

    if (play_with_computer == 0)
    {
      result = getpaddle2_1((int*)&game.table.paddle1.position.x, clnt_res, cl);
      if (result != 0)
      {
          // the ball on player2 side was not launched
          clnt_perror (cl, "call failed");
          continue;
      }else
      {
          game.table.paddle2.position.x= *clnt_res;
      }
    }else
    {
      // update paddle2 x position
      if( (game.table.paddle2.position.x + game.table.paddle2.surface->w)
              > (game.table.table.surface->w -5)
          )
          paddle2_x=game.table.table.surface->w -5 -game.table.paddle2.surface->w;

      if(game.table.paddle2.position.x < 3)
          paddle2_x = 3;

      game.table.paddle2.position.x = paddle2_x;
      }

      /* display the graphic items */
      display(&game);

     /* Cap frame rate !!!! */
     SDL_Delay(PAUSE_MS);
  }

/* destroys the RPC client's (player1) handle */
if (play_with_computer == 0)
 {
    pthread_cancel(thread_1);
    clnt_destroy (cl);
  }
  free(clnt_res);

/* remove timer and stop the ball */  
  SDL_RemoveTimer(timer);

/* free the video memory */
  free_video_memory(&game);

/* free police */  
  TTF_CloseFont(game.board.police);

/* close audio and free allocated sound chunks */  
  Mix_CloseAudio();
  Mix_FreeChunk(game.paddle_hit);
  Mix_FreeChunk(game.touchline_hit);
    
  Mix_Quit();
  TTF_Quit();
  SDL_Quit();

  return EXIT_SUCCESS; /* amused, No ? :) */
}

static int SDLlibs_init()
{
/* initialisation of SDL */
  if( SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)
    {
	fprintf(stderr, "SDL_Init Error : %s\n", SDL_GetError());
	return 0;
 	}

/* initialisation of SDL_ttf */
  if(TTF_Init() == -1)
    {
	fprintf(stderr, "TTF_Init Error : %s\n", TTF_GetError());
	return 0;
	}
/* initialisation of SDL_mixer */
  if(Mix_Init(0) == -1)
    {
	fprintf(stderr, "TTF_Init Error : %s\n", Mix_GetError());
	return 0;
	}

	return 1;
}

static void print_usage (char * program_name, FILE * stream, int exit_code)
{
    fprintf(stream, "Usage: %s [-i player2_address]\n\n", program_name);
    fprintf(stream, "  -h, --help\t\t Display this usage information.\n");
    fprintf(stream, "  -i, --ip\t\t IP address of player2's host.\n");
    exit(exit_code);
}

static void scan_options(int argc, char *argv[], char ** ip)
{    
  const char* const short_options = "hi:";
  const struct option long_options[] = 
   {
     {"help", 0, NULL, 'h'},  
     {"ip", 1, NULL, 'i'},
     {NULL, 0, NULL, 0}
   };

  int next_option;
 
 do{
    next_option = getopt_long(argc, argv, short_options, long_options, NULL);   
    switch(next_option)
    {
      case 'h': print_usage(argv[0], stdout, EXIT_SUCCESS);
      
      case 'i': 
                *ip = (char*)malloc (strlen(optarg)+1);
                strcpy(*ip,optarg );
      break;
      
      case '?': print_usage(argv[0], stderr, EXIT_FAILURE);
    
      case -1:
      break;
      default: abort();
    }
 } while(next_option != -1); 
    
}
