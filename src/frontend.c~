#include <stdlib.h>
#include <ncurses.h>
#include <ctype.h>
#include <string.h>

#include "backend.h"
#include "frontend.h"

// display a single block of element
void display_block (int x, int y, const chtype symbol, int attr_num) {
  attron(COLOR_PAIR(attr_num));
  mvaddch(y, x, symbol);
  attroff(COLOR_PAIR(attr_num));
}

// display entire list of elements
void display_foods (PointList* foods) {
  PointList* iterator = foods;
  while(iterator) {
    attron(COLOR_PAIR(iterator->point));
    // food element display might change later
    mvaddch(iterator->y, iterator->x, ACS_DIAMOND);
    attroff(COLOR_PAIR(iterator->point));
    iterator = iterator->next;
  }
}

// display snake, with a difference to its head
void display_snake (PointList* snake, enum Direction current_dir, int attr_num) {
  attron(COLOR_PAIR(attr_num));
  PointList* snake_head = snake;
  while(snake) {
    if (snake == snake_head) { // if element is the head
      attron(A_BOLD);
      switch (current_dir) { // display the head according to current position
      case RIGHT: mvaddch(snake->y, snake->x, ACS_RARROW); break;
      case LEFT: mvaddch(snake->y, snake->x, ACS_LARROW); break;
      case UP: mvaddch(snake->y, snake->x, ACS_UARROW); break;
      case DOWN: mvaddch(snake->y, snake->x, 'v'); break;
      }
      attroff(A_BOLD);
    }else {
      mvaddch(snake->y, snake->x, ACS_BLOCK);
    }
    snake = snake->next;
  }
  attroff(COLOR_PAIR(attr_num));
}

// draw border for the gameboard
void draw_border (int xmin, int ymin, int xmax, int ymax, int attr_num) {
  for (int x = xmin; x < xmax; x++) {
    display_block(x, ymin-1, ACS_HLINE, attr_num); // draw upper boundary
    display_block(x, ymax, ACS_HLINE, attr_num); // draw lower boundary
  }
  for (int y = ymin; y < ymax; y++) {
    display_block(xmax, y, ACS_VLINE, attr_num); // draw right boundary
    display_block(xmin-1, y, ACS_VLINE, attr_num); // draw left boundary
  }
}

// print a text out to the terminal
void print_text (char* string, int x, int y, int attr_num) {
  attron(COLOR_PAIR(attr_num));
  for (int i = 0; i < strlen(string); i++) mvaddch(y, x+i, string[i]);
  attroff(COLOR_PAIR(attr_num));
}

// initialize color for the terminal
void color_init () {
  start_color();
  
  // add colors to the interface
  init_color(COLOR_BLACK,0,0,0);
  init_color(COLOR_RED,1000,0,0);
  
  // define color pairs
  init_pair(0, COLOR_BLACK, 0);
  init_pair(1, COLOR_RED, 0);
  init_pair(2, COLOR_GREEN, 0);
  init_pair(3, COLOR_YELLOW, 0);
  init_pair(4, COLOR_BLUE, 0);
  init_pair(5, COLOR_MAGENTA, 0);
  init_pair(6, COLOR_CYAN, 0);
  init_pair(7, COLOR_WHITE, 0);
  wbkgd(stdscr,COLOR_PAIR(0));
}

// initialize the terminal
void terminal_init (void) {
  // start the cursors, keypad and terminal
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE); // make keys work
  curs_set(0); // hide cursor
  nodelay(stdscr,TRUE);
  timeout(100);
}

/*************************************************************************/
/***************************GAME_INTERFACE********************************/
/*************************************************************************/

// check user input when not in gameplay
void get_user_input (enum State *stage) {
  int ch = getch();
  if (ch) { // to be sure that pointer is not NULL
    switch (ch) {
    case KEY_UP: // up arrow
      if (*stage > MENU_1P && *stage <= MENU_HIGHSCORE) {
	(*stage)--;
      }else if (*stage == MENU_1P) {
	(*stage) = MENU_HIGHSCORE;
      }
      break;
    case KEY_DOWN: // down arrow
      if (*stage < MENU_HIGHSCORE) {
	(*stage)++; 
      }else if (*stage == MENU_HIGHSCORE){
	*stage = MENU_1P;
      }
      break;
    case 10: // enter key. ASCII code is 10
      if (*stage == MENU_1P) {
	*stage = GAME_1P;
      }else if (*stage == MENU_2P) {
	*stage = GAME_2P;
      }else if (*stage == MENU_GUIDE) {
	*stage = MANUAL;
      }else if (*stage == MENU_LICENSE) {
	*stage = LICENSE;
      }else if (*stage == MENU_HIGHSCORE) {
	*stage = HIGHSCORE;
      }
      break;
    case 'x': case 'X': // key x for exit
      if (*stage == VICTORY_P1 || *stage == VICTORY_P2 || *stage == GAMEOVER) {
	*stage = MENU_1P;
      }else if (*stage == MANUAL) {
	*stage = MENU_GUIDE;
      }else if (*stage == HIGHSCORE) {
	*stage = MENU_HIGHSCORE; 
      }else if (*stage == LICENSE) {
	*stage = MENU_LICENSE;
      }else if (*stage <= MENU_HIGHSCORE) { // any of menu states
	*stage = END_GAME;
      }
      break;
    }
  }
}

// the game start menu
void start_menu (enum State* stage) {
  clear();
  int xmax, ymax;
  getmaxyx(stdscr, ymax, xmax);
  int x_anchor = (xmax>>1)-15, y_anchor = (ymax>>2)-4;
  int color = (rand()%7)+1;
  print_text(" ##   #   #    #    #   #  ####\0", x_anchor, y_anchor, color);
  print_text("#  #  ##  #   # #   #  #   #   \0", x_anchor, y_anchor+1, color);
  print_text("#     ##  #  #   #  # #    #   \0", x_anchor, y_anchor+2, color);
  print_text(" #    # # #  #   #  ##     ####\0", x_anchor, y_anchor+3, color);
  print_text("  #   # # #  #####  ##     #   \0", x_anchor, y_anchor+4, color);
  print_text("   #  #  ##  #   #  # #    #   \0", x_anchor, y_anchor+5, color);
  print_text("#  #  #  ##  #   #  #  #   #   \0", x_anchor, y_anchor+6, color);
  print_text(" ##   #   #  #   #  #   #  ####\0", x_anchor, y_anchor+7, color);

  // start to write down the options that user can choose from
  print_text("1p single player\0", (xmax>>1)-7, (ymax*2/3)-(ymax/9)*2, COLOR_YELLOW);
  print_text("2p multiplayer\0", (xmax>>1)-6, (ymax*2/3)-(ymax/9), COLOR_RED);
  print_text("How to play?\0", (xmax>>1)-5, (ymax*2/3), COLOR_GREEN);
  print_text("License!\0", (xmax>>1)-3, (ymax*2/3)+(ymax/9), COLOR_MAGENTA);
  print_text("Hall of fame\0", (xmax>>1)-5, (ymax*2/3)+(ymax/9)*2, COLOR_CYAN);

  attron(COLOR_PAIR(COLOR_WHITE));
  if (*stage == MENU_1P)  {
    mvaddch((ymax*2/3)-(ymax/9)*2, (xmax>>1)-12, ACS_RARROW);
    mvaddch((ymax*2/3)-(ymax/9)*2, (xmax>>1)+12, ACS_LARROW);
  }else if (*stage == MENU_2P) {
    mvaddch((ymax*2/3)-(ymax/9), (xmax>>1)-12, ACS_RARROW);
    mvaddch((ymax*2/3)-(ymax/9), (xmax>>1)+12, ACS_LARROW);
  }else if (*stage == MENU_GUIDE) {
    mvaddch((ymax*2/3), (xmax>>1)-12, ACS_RARROW);
    mvaddch((ymax*2/3), (xmax>>1)+12, ACS_LARROW);
  }else if (*stage == MENU_LICENSE) {
    mvaddch((ymax*2/3)+(ymax/9), (xmax>>1)-12, ACS_RARROW);
    mvaddch((ymax*2/3)+(ymax/9), (xmax>>1)+12, ACS_LARROW);
  }else if (*stage == MENU_HIGHSCORE) {
    mvaddch((ymax*2/3)+(ymax/9)*2, (xmax>>1)-12, ACS_RARROW);
    mvaddch((ymax*2/3)+(ymax/9)*2, (xmax>>1)+12, ACS_LARROW);
  }
  attroff(COLOR_PAIR(COLOR_WHITE));

  // draw board border
  draw_border(1,  1,  xmax-1, ymax-1, COLOR_YELLOW);
  // quit instruction
  print_text("Press X to quit\0", xmax-17, ymax-2, COLOR_WHITE);
  // poll for user command
  get_user_input(stage);
}

// victory for one of the 2 players
void victory (enum State* stage, Board* board1, Board* board2) {
  clear();
  int xmax, ymax;
  getmaxyx(stdscr, ymax, xmax);
  int text_color = 0;
  char winner[15] = "Player x wins!\0";
  if (*stage == VICTORY_P1) {
    winner[7] = '1';
    text_color = COLOR_YELLOW;
  }else if (*stage == VICTORY_P2) {
    winner[7] = '2';
    text_color = COLOR_RED;
  }
  print_text(winner, (xmax>>1)-7, (ymax>>5), text_color);
  // find the effective score of the 2 players
  int snake_count_1 = snake_length(board1->snake_head), snake_count_2 = snake_length(board2->snake_head);
  float effective_score_p1 = (float)board1->score/(float)snake_count_1, effective_score_p2 = (float)board2->score/(float)snake_count_2;
  FILE* temp_file = fopen("temp.dat","w");
  if (temp_file) { // write formatted strings into the file
    fprintf(temp_file, "Efficiency: %5.3f\nEfficiency: %5.3f", effective_score_p1, effective_score_p2);
  }
  freopen("temp.dat","r",temp_file);
  // print score for player 1
  char buffer[20];
  int i = 0;
  if (temp_file)  fgets(buffer, 20, temp_file); // read back from temp file
  buffer[19] = '\0';
  print_text(buffer, (board1->xmin+board1->xmax)/2 - 9, (board1->ymin+board1->ymax)/2, COLOR_YELLOW);
  if (*stage == VICTORY_P1) {
    attron(A_BOLD);
    print_text("You won!",(board1->xmin+board1->xmax)/2 - 4, (board1->ymin+board1->ymax)/2 - 1, rand()%7+1);
    attroff(A_BOLD);
  }
  else print_text("You lost!",(board1->xmin+board1->xmax)/2 - 5, (board1->ymin+board1->ymax)/2 - 1, COLOR_YELLOW );
			      
  // print score for player 2
  if (temp_file) fgets(buffer, 20, temp_file); // read back from temp file
  buffer[19] = '\0';
  print_text(buffer, (board2->xmin+board2->xmax)/2 - 9, (board2->ymin+board2->ymax)/2, COLOR_RED);
  if (*stage == VICTORY_P2) {
    attron(A_BOLD);
    print_text("You won!",(board2->xmin+board2->xmax)/2 - 4, (board2->ymin+board2->ymax)/2 - 1, rand()%7+1);
    attroff(A_BOLD);
  }
  else print_text("You lost!",(board2->xmin+board2->xmax)/2 - 5, (board2->ymin+board2->ymax)/2 - 1, COLOR_RED);

  // draw board border
  draw_border(board1->xmin, board1->ymin, board1->xmax, board1->ymax, COLOR_YELLOW);
  draw_border(board2->xmin, board2->ymin, board2->xmax, board2->ymax, COLOR_RED);
  // quit instruction
  print_text("X: back to menu\0", xmax-17, ymax-1, COLOR_WHITE);
  get_user_input(stage); // update input
}

// game over in single player mode. highscore indicate player position on highscore board
void game_over (enum State* stage, Board* board, int highscore) {
  clear();
  int xmax, ymax;
  getmaxyx(stdscr, ymax, xmax);
  attron(A_BOLD);
  print_text("GAME OVER!", (xmax>>1)-5, ymax>>3, COLOR_RED);
  attroff(A_BOLD);
  char score_string[] = "Your score: xxxx\0";
  score_string[15] = '0'+(board->score)%10;
  score_string[14] = '0'+((board->score)%100)/10;
  score_string[13] = '0'+((board->score)%1000)/100;
  score_string[12] = '0'+(board->score)/1000;
  print_text(score_string, (xmax>>1)-8, ymax>>2, COLOR_YELLOW);
  // check of a player high score.
  if (highscore > 0) {
    attron(A_BOLD);
    print_text("!!!CONGRATULATIONS!!!\0", (xmax>>1)-10, (ymax>>1)-2, (rand()%7)+1);
    print_text("You have reached a new highscore\0", (xmax>>1)-16, (ymax>>1), (rand()%7)+1);
    attroff(A_BOLD);
  }
  // draw board border
  draw_border(1,  1,  xmax-1, ymax-1, COLOR_RED);
  // quit instruction
  print_text("X: back to menu\0", xmax-17, ymax-2, COLOR_WHITE);
  get_user_input(stage);
}

// display help menu
void help_menu (enum State* stage) {
  clear();
  int xmax, ymax;
  getmaxyx(stdscr, ymax, xmax);
  static int x_offset = 0;
  attron(A_BOLD);
  print_text("How to play?\0", (xmax>>1)-6, ymax>>3, COLOR_GREEN);
  attroff(A_BOLD);

  print_text("Controls:\0", (xmax>>2)-5, (ymax>>2), COLOR_CYAN);
  print_text("Player 1\0", (xmax>>1)-4, (ymax>>2), COLOR_YELLOW);
  
  print_text("UP\0", (xmax>>1)-1, (ymax>>2)+2, COLOR_YELLOW);
  print_text("LEFT\0", (xmax>>1)-7, (ymax>>2)+3, COLOR_YELLOW);
  print_text("RIGHT\0", (xmax>>1)+3, (ymax>>2)+3, COLOR_YELLOW);
  print_text("DOWN\0", (xmax>>1)-2, (ymax>>2)+4, COLOR_YELLOW);
  
  print_text("Player 2\0", (xmax*3/4)-4, (ymax>>2), COLOR_RED);
  print_text("W\0", (xmax*3/4), (ymax>>2)+2, COLOR_RED);
  print_text("A\0", (xmax*3/4)-2, (ymax>>2)+3, COLOR_RED);
  print_text("D\0", (xmax*3/4)+2, (ymax>>2)+3, COLOR_RED);
  print_text("S\0", (xmax*3/4), (ymax>>2)+4, COLOR_RED);

  // print instruction
  print_text("Eat food with higher values to obtain higher scores.", (xmax>>1)-28, (ymax>>1), COLOR_WHITE);
  print_text("Go head to head with another player and try to eat less, gain more!!", (xmax>>1)-35, (ymax>>1)+1, COLOR_WHITE);

  // print out food values
  print_text("Food values:\0", (xmax>>1)-32, (ymax*3/4)-2, COLOR_WHITE);
  char score_value[4] = "= x\0";
  int offset = 32;
  for (int i = 1; i < 8; i++) {
    attron(COLOR_PAIR(i));
    mvaddch((ymax*3/4), (xmax>>1)-offset, ACS_DIAMOND);
    attroff(COLOR_PAIR(i));
    score_value[2] = '0'+i;
    print_text(score_value, (xmax>>1)-offset+2, (ymax*3/4), i);
    offset -= 10;
  }
  
  // draw board border
  draw_border(1,  1,  xmax-1, ymax-1, COLOR_GREEN);
  // quit instruction
  print_text("X: back to menu\0", xmax-17, ymax-2, COLOR_WHITE);
  get_user_input(stage);
}

// license menu
void license (enum State* stage) {
  clear();
  int xmax, ymax;
  getmaxyx(stdscr, ymax, xmax);
  attron(A_BOLD);
  print_text("License", (xmax>>1)-3, ymax>>3, COLOR_MAGENTA);
  attroff(A_BOLD);  
  // printout the license text
  FILE* license_file = fopen("LICENSE", "r");
  char buffer[120];
  int y_anchor = (ymax>>1)-8;
  if (license_file) {
    while (fgets(buffer, 120, license_file)) {
        print_text(buffer,(xmax>>1)-40, y_anchor++, COLOR_WHITE);
    }
    fclose(license_file);
  }
  // draw board border
  draw_border(1,  1,  xmax-1, ymax-1, COLOR_MAGENTA);
  // quit instruction
  print_text("X: back to menu\0", xmax-17, ymax-2, COLOR_WHITE);
  get_user_input(stage);
}


// hall of fame or highscore
void hall_of_fame (enum State* stage) {
  clear();
  int xmax, ymax;
  getmaxyx(stdscr, ymax, xmax);
  attron(A_BOLD);
  print_text("Highscore", (xmax>>1)-5, ymax>>3, COLOR_CYAN);
  attroff(A_BOLD);
  // list of highscores
  int highscores[5] = {0, 0, 0, 0, 0};
  get_high_score(highscores);
  char score_string[5] = "xxxx\0";
  char indicators[5][5] = {"1st:\0", "2nd:\0", "3rd:\0", "4th:\0", "5th:\0"};
  
  for (int i = 0; i < 5; i++) {
    score_string[3] = '0'+(highscores[i])%10;
    score_string[2] = '0'+(highscores[i]%100)/10;
    score_string[1] = '0'+(highscores[i]%1000)/100;
    score_string[0] = '0'+(highscores[i])/1000;
    print_text(score_string, (xmax>>1), (ymax/3)+i*(ymax>>3), i+1);
    print_text(indicators[i], (xmax>>1)-5, (ymax/3)+i*(ymax>>3), i+1);
  }
  // draw board border
  draw_border(1,  1,  xmax-1, ymax-1, COLOR_CYAN);
  // quit instruction
  print_text("X: back to menu\0", xmax-17, ymax-2, COLOR_WHITE);
  get_user_input(stage);
}

/**************************************************************************/
/******************************HIGHSCORE***********************************/
/**************************************************************************/

// get the highscores from the data file
void get_high_score (int highscores[5]) {
  FILE* score_file = fopen(HIGHSCORE_FILE, "r");
  if (score_file) {
    char buffer[10];
    for (int i = 0; i < 5; i++) {
      fgets(buffer, sizeof(buffer), score_file);
      highscores[i] = atoi(buffer);
    }
    fclose(score_file);
  }
}

// write highscore into file
void write_high_score (int highscores[5]) {
  FILE* score_file = fopen(HIGHSCORE_FILE, "w");
  if (score_file) {
    fprintf(score_file, "%d\n%d\n%d\n%d\n%d", highscores[0], highscores[1], highscores[2], highscores[3], highscores[4]);
    fclose(score_file);
  }
}

// update highscore. Return the position that the player reached with their score
int update_highscore (Board* board) {
  int new_position = 0;
  int current_highscores[5] = {0,0,0,0,0};
  get_high_score(current_highscores); // get the highscores
  // copy the current highscores to updated array
  int updated_highscores[5];
  for (int i = 0; i < 5; i++) updated_highscores[i] = current_highscores[i]; 
  // find if the player manages to get in the highscore list
  for (int i = 1; i < 6; i++) {
    if (board->score > current_highscores[i-1]) {
      new_position = i;
      if (board) updated_highscores[i-1] = board->score; // make sure that board is not NULL
      for (int j = i; j < 5; j++) {
	updated_highscores[j] = current_highscores[j-1];
      }
      break;
    }
  }
  if (new_position) write_high_score(updated_highscores);
  return new_position;
}

/***************************************************************************/
/*****************************GAMEPLAY_ELEMENTS*****************************/
/***************************************************************************/

// check user for input during gameplay
void get_next_move (enum Direction* ctrl1, enum Direction* ctrl2, enum State* stage) {
  int ch = getch();
  if (ctrl2) { // check if ctrl2 is not NULL
    switch (ch) {
      // update control for player 2
    case 'a': case 'A':
      if (*ctrl2 != RIGHT && *ctrl2 != STOP) *ctrl2 = LEFT; break;
    case 'd': case 'D':
      if (*ctrl2 != LEFT && *ctrl2 != STOP) *ctrl2 = RIGHT; break;
    case 's': case 'S':
      if (*ctrl2 != UP && *ctrl2 != STOP) *ctrl2 = DOWN; break;
    case 'w': case 'W': 
      if (*ctrl2 != DOWN && *ctrl2 != STOP) *ctrl2 = UP; break;
    }
  }
  if (ctrl1) { // check if ctrl1 is not NULL
    switch(ch) {
      // update control for player 1
    case KEY_LEFT:
      if (*ctrl1 != RIGHT && *ctrl1 != STOP) *ctrl1 = LEFT; break;
    case KEY_RIGHT:
      if (*ctrl1 != LEFT  && *ctrl1 != STOP) *ctrl1 = RIGHT; break;
    case KEY_DOWN:
      if (*ctrl1 != UP && *ctrl1!= STOP) *ctrl1 = DOWN; break;
    case KEY_UP:
      if (*ctrl1 != DOWN && *ctrl1 != STOP) *ctrl1 = UP; break;
    }
  }
  static enum Direction saveP1, saveP2;
  // save the last direction before stopping
  if (ctrl1) saveP1 = (*ctrl1 != STOP) ? *ctrl1 : saveP1;
  if (ctrl2) saveP2 = (*ctrl2 != STOP) ? *ctrl2 : saveP2;
  switch (ch) {
  case 'p': case 'P':
    if (ctrl1) {
      if (*ctrl1 != STOP) *ctrl1 = STOP;
      else *ctrl1 = saveP1;
    }
    if (ctrl2) {
      if (*ctrl2 != STOP) *ctrl2 = STOP;
      else *ctrl2 = saveP2;
    }
    break;
  case 'x': case 'X':
    *stage = MENU_1P;
    break;
  }
}

// initilize the game. Can be called for multiplayer or singleplayer
Board* single_player_init (void) {
  // initialize the game board
  int xmax, ymax, xmin, ymin;
  getmaxyx(stdscr, ymax, xmax);
  // single player so the board takes up full screen
  xmin = (int)(xmax/7);
  ymin = (int)(ymax/7);
  xmax -= xmin;
  ymax -= ymin;

  // initilize the snake head and tail elements
  PointList *snake_head, *snake_tail;
  create_snake((xmax-xmin)/2+xmin,(ymax-ymin)/2+ymin, &snake_head, &snake_tail);
  
  Board* board = create_board(snake_head, snake_tail, NULL, xmax, ymax, xmin, ymin); 
  for (int i = 0; i < FOOD_MAX; i++) {
    add_new_food(board);
  }

  return board;
}

// continue the game for single player mode
void single_player_game (Board* board, int delay, enum Direction* dir, enum State *stage) {
  enum Status status = SUCCESS;
  get_next_move(dir,NULL,stage);

  if (*dir != STOP) {
    status = move_snake(board, *dir);
    
    clear();
    // print player index
    print_text("Player1\0", board->xmin, board->ymin-2, COLOR_YELLOW);

    // print score
    char score_string[12] = "Score: xxxx\0";
    score_string[10] = '0'+(board->score)%10;
    score_string[9] = '0'+((board->score)%100)/10;
    score_string[8] = '0'+((board->score)%1000)/100;
    score_string[7] = '0'+(board->score)/1000;  
    print_text(score_string,board->xmin,board->ymax+1, COLOR_YELLOW);

    // delay mechanism to randomize food value
    if (delay > 10) score_update(board->foods);
    
    // draw board border
    draw_border(board->xmin, board->ymin, board->xmax, board->ymax, COLOR_YELLOW);
    // display food elements
    display_foods(board->foods);
    // display the snake
    display_snake(board->snake_head , *dir, COLOR_YELLOW);
    // pause and quit instruction
    print_text("X: menu - P: pause\0", board->xmax-18, board->ymin-2, COLOR_WHITE);
  }else {
    print_text("GAME PAUSED\0", board->xmin+(board->xmax-board->xmin)/2-5, board->ymin+(board->ymax-board->ymin)/2, COLOR_YELLOW);
  }

  if (status == FAILURE) {
    *stage = GAMEOVER;
  }
}


// initialize the game to be multiplayer
Board** multiplayer_init (void) {
  // initialize the game board
  int xmax1, ymax1, xmin1, ymin1, xmin2, ymin2, xmax2, ymax2, xmax, ymax;
  getmaxyx(stdscr, ymax, xmax);
  // player 1 board will be on the left side
  xmin2 = xmax/14;
  ymin2 = ymax/12;
  xmax2 = xmax/2 - xmin2;
  ymax2 = ymax - ymin2;
  // player 2 board will be on the left side. y coords will be the same
  xmin1 = xmin2 + xmax/2;
  ymin1 = ymin2;
  xmax1 = xmax2 + xmax/2;
  ymax1 = ymax2;

  Board** player_boards = malloc (2*sizeof(Board*));
  PointList *snake_head, *snake_tail;
  create_snake((xmax1-xmin1)/2+xmin1, (ymax1-ymin1)/2+ymin1, &snake_head, &snake_tail);  
  player_boards[0] = create_board(snake_head, snake_tail, NULL, xmax1, ymax1, xmin1, ymin1);
  create_snake((xmax2-xmin2)/2+xmin2, (ymax2-ymin2)/2+ymin2, &snake_head, &snake_tail);  
  player_boards[1] = create_board(snake_head, snake_tail, NULL, xmax2, ymax2, xmin2, ymin2);
  for (int i = 0; i < FOOD_MAX; i++) {
    add_new_food(player_boards[0]);
    add_new_food(player_boards[1]);
  }

  return player_boards;
}

// continue the game for multiplayer mode
void multiplayer_game (Board* board1, Board* board2, int delay, enum Direction *dir1, enum Direction *dir2, enum State *stage) {
  enum Status status1 = SUCCESS, status2 = SUCCESS;
  
  get_next_move(dir1,dir2,stage); // player 1 control with UP DOWN LEFT RIGHT keys, player 2 with WSAD

  if (*dir1 != STOP && *dir2 != STOP) { // check if the control is not stop before updating
    status1 = move_snake(board1, *dir1);
    status2 = move_snake(board2, *dir2);
    
    clear();
    //print player index
    print_text("Player1\0", board1->xmin, board1->ymin-2, COLOR_YELLOW);
    print_text("Player1\0", board2->xmin, board2->ymin-2, COLOR_RED);
  
    // print score for player 1
    char score_string1[12] = "Score: xxxx\0";
    score_string1[10] = '0'+(board1->score)%10;
    score_string1[9] = '0'+((board1->score)%100)/10;
    score_string1[8] = '0'+((board1->score)%1000)/100;
    score_string1[7] = '0'+(board1->score)/1000;  
    print_text(score_string1, board1->xmin, board1->ymax+1, COLOR_YELLOW);

    // print score for player 2
    char score_string2[12] = "Score: xxxx\0";
    score_string2[10] = '0'+(board2->score)%10;
    score_string2[9] = '0'+((board2->score)%100)/10;
    score_string2[8] = '0'+((board2->score)%1000)/100;
    score_string2[7] = '0'+(board2->score)/1000;  
    print_text(score_string2, board2->xmin, board2->ymax+1, COLOR_RED);
  
    // delay mechanism to randomize food value
    if (delay > 10) {
      score_update(board1->foods);
      score_update(board2->foods);
    }

    // draw board border
    draw_border(board1->xmin, board1->ymin, board1->xmax, board1->ymax, COLOR_YELLOW);
    draw_border(board2->xmin, board2->ymin, board2->xmax, board2->ymax, COLOR_RED);
    // display food elements on board
    display_foods(board1->foods);
    display_foods(board2->foods);
    // display snake
    display_snake(board1->snake_head , *dir1, COLOR_YELLOW);
    display_snake(board2->snake_head , *dir2, COLOR_RED);
    // pause and quit instruction
    print_text("X: menu\0", board2->xmax-7, board2->ymin-2, COLOR_WHITE);
    print_text("P: pause\0", board1->xmax-8, board1->ymin-2, COLOR_WHITE);
  }else {
    print_text("GAME PAUSED\0", board1->xmin+(board1->xmax-board1->xmin)/2-5, board1->ymin+(board1->ymax-board1->ymin)/2, COLOR_YELLOW);
    print_text("GAME PAUSED\0", board2->xmin+(board2->xmax-board2->xmin)/2-5, board2->ymin+((board2->ymax-board2->ymin))/2, COLOR_RED);
  }

  // if someone fail, change game state
  if (status1 == FAILURE || status2 == FAILURE) {
    int snake_count_1 = snake_length(board1->snake_head), snake_count_2 = snake_length(board2->snake_head);
    float effective_score_p1 = (float)board1->score/(float)snake_count_1, effective_score_p2 = (float)board2->score/(float)snake_count_2;
    if (effective_score_p1 > effective_score_p2) {
      *stage = VICTORY_P1;
    }else if (effective_score_p1 < effective_score_p2) {
      *stage = VICTORY_P2;
    }else { // if effective score are the same then the person who ram himself loses
      if (status1 == FAILURE) *stage = VICTORY_P2;
      else *stage = VICTORY_P1;
    }
  }
}
