#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "backend.h"
#include "frontend.h"


int main() {
  terminal_init();
  color_init();
  // initial condition of the game
  enum State game_stage = MENU_1P;

  // 1p variables
  Board* sp_board;
  enum Direction p_dir;
  
  // 2p variables
  Board** p_boards; 
  enum Direction p1_dir,  p2_dir;

  // indicate that new highscore is reached
  int highscore;
  int delay;
  
  while(game_stage != END_GAME) {
    highscore = 0;
    delay = 0;
    sp_board = NULL;
    p_boards = NULL;
    
    // menu display
    while (game_stage <= MENU_HIGHSCORE) {
      start_menu(&game_stage);
    }

    // display help menu
    while (game_stage == MANUAL) {
      help_menu(&game_stage);
    }

    // display highscore hall of fame
    while (game_stage == HIGHSCORE) {
      hall_of_fame(&game_stage);
    }

    // display the license agreement
    while (game_stage == LICENSE) {
      license(&game_stage);
    }
    
    // handle the game for single player
    if (game_stage == GAME_1P) {
      sp_board = single_player_init();
      p_dir = RIGHT;
    }
    while (game_stage == GAME_1P) { // enter single player game
      delay++;
      single_player_game(sp_board, delay, &p_dir, &game_stage);
      if (delay > 10) delay = 0; // reset delay
    }
    
    // handle the game for 2 players
    if (game_stage == GAME_2P) { // initialize
        p_boards = multiplayer_init();
        p1_dir = RIGHT; p2_dir = RIGHT;
    }
    while (game_stage == GAME_2P) { // enter 2 player game
      delay++;
      multiplayer_game(p_boards[0], p_boards[1], delay, &p1_dir, &p2_dir, &game_stage);
      if (delay > 10) delay = 0; // reset delay
    }

    // handle a game over in single player mode
    if (game_stage == GAMEOVER) {
      highscore = update_highscore(sp_board);
    }
    while (game_stage == GAMEOVER) {
      game_over(&game_stage, sp_board, highscore);
    }
    // handle a victory condition for 2 players mode
    while (game_stage == VICTORY_P1 || game_stage == VICTORY_P2) {
      victory(&game_stage, p_boards[0], p_boards[1]);
    }

    // clear player boards
    if (p_boards) { // check if p_board is not NULL
      clear_board(p_boards[0]);
      clear_board(p_boards[1]);
      free(p_boards);
    }
    clear_board(sp_board);
  }

  endwin();

  return 0;
}

