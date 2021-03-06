#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "backend.h"

/******************************************************************************************/
/**************************************GENERAL_FUNCTIONS***********************************/
/******************************************************************************************/

// compare if 2 cells are the same
bool is_same_place(PointList* cell1, PointList* cell2) {
  return cell1->x == cell2->x && cell1->y == cell2->y;
}

// check if a list contains a cell
PointList* list_contains(PointList* cell, PointList* list) {
  PointList* s = list;
  while (s) {
    if (is_same_place(s, cell)) {
      return s;
    }
    s = s->next;
  }
  return NULL;
}

// deallocate list of elements. It checks for NULL so just call without worry
void clear_list (PointList* list) {
  if (list) {
    clear_list(list->next);
    free(list);
  }
}

// deallocate a board variable
void clear_board (Board* board) {
  if (board) {
    clear_list(board->snake_head); // just the head will be ok since tail points to the end of the same list
    clear_list(board->foods); // clear food list
    free(board); // free the board itself
  }
}

// return the length of snake
int snake_length (PointList* snake) {
  int counter = 0;
  while (snake) {
    counter++;
    snake = snake->next;
  }
  return counter;
}

// create a new cell with coordinates x and y. "next" and "previous" pointer set to NULL.
PointList* create_cell(int x, int y) {
  PointList* cell = malloc( sizeof(PointList) );
  cell->point = 0;
  cell->x = x;
  cell->y = y;
  cell->next = NULL;
  cell->previous = NULL;
  return cell;
}


/******************************************************************************************/
/****************************************MOVE_FUNCTIONS************************************/
/******************************************************************************************/

// move snake by creating new head and clearing the tail. Return SUCCESS if unhinged, FAILURE otherwise
enum Status move_snake(Board* board, enum Direction dir) {
  // Create a new beginning. Check boundaries.
  PointList* beginning = next_move(board, dir);
  if (beginning == NULL) {
    return FAILURE;
  }

  // Check for collisions with self
  if (list_contains(beginning, board->snake_head)) {
    free(beginning);
    return FAILURE;
  }

  // Attach the beginning to the rest of the snake
  board->snake_head->previous = beginning; // point back to the beginning
  beginning->next = board->snake_head;
  board->snake_head = beginning;
  
  // Check for food
  PointList* found_element = list_contains(beginning, board->foods); 
  if (found_element) { // if we found that snake indeed crosses food element
    board->score += found_element->point;
    remove_from_list(found_element, &board->foods);
    add_new_food(board);
    
    return SUCCESS;
  }

  board->snake_tail = board->snake_tail->previous;
  free(board->snake_tail->next);
  board->snake_tail->next = NULL;

  return SUCCESS;
}

/*
  This function takes in the board information and current snake direction
  and return the next head of snake.
 */
PointList* next_move(Board* board, enum Direction dir) {
  PointList* snake = board->snake_head;
  int new_x = snake->x;
  int new_y = snake->y;
  switch (dir) {
    case UP:
      new_y -= 1;
      if (new_y < board->ymin) new_y = board->ymax-1;
      break;
    case DOWN:
      new_y += 1;
      if (new_y > board->ymax-1) new_y = board->ymin;
      break;
    case LEFT:
      new_x -= 1;
      if (new_x < board->xmin) new_x = board->xmax-1;
      break;
    case RIGHT:
      new_x += 1;
      if (new_x > board->xmax-1) new_x = board->xmin;
      break;
  }
  return create_cell(new_x, new_y);
}


/******************************************************************************************/
/*****************************************INITIALIZERS*************************************/
/******************************************************************************************/

// initialize the snake for game
void create_snake(int x_snake, int y_snake, PointList** snake_head, PointList** snake_tail) {
  PointList* a = create_cell(x_snake, y_snake);
  PointList* b = create_cell(x_snake, y_snake-1);
  a->next = b; a->previous = NULL;
  b->next = NULL; b->previous = a;
  *snake_head = a;
  *snake_tail = b;
}

// initialize the board for the game
Board* create_board(PointList* snake_head, PointList* snake_tail, PointList* foods, int xmax, int ymax, int xmin, int ymin) {
  Board* board = malloc( sizeof(Board) );
  board->foods = foods;
  // head and tail to reduce the effort to iterate
  board->snake_head = snake_head;
  board->snake_tail = snake_tail;
  board->xmin = xmin;
  board->ymin = ymin;
  board->xmax = xmax;
  board->ymax = ymax;
  board->score = 0;
  return board;
}



/******************************************************************************************/
/****************************************FOOD_FUNCTIONS************************************/
/******************************************************************************************/

// create a random cell with coordinates x, y, next = NULL. xmax and ymax to set limits for rand function
PointList* create_random_cell(int xmax, int ymax, int xmin, int ymin) {
  return create_cell(xmin + (rand() % (xmax-xmin)), ymin + (rand() % (ymax-ymin)));
}

// add a new food element into board by adding a new element to the head of the food list in board info
void add_new_food(Board* board) {
  PointList* new_food;
  do {
    new_food = create_random_cell(board->xmax, board->ymax, board->xmin, board->ymin);
  } while(list_contains(new_food, board->foods) || list_contains(new_food, board->snake_head));
  new_food->point = (rand()%7)+1; // a random number of point from 1 to 7
  new_food->next = board->foods;
  new_food->previous = NULL;
  if (board->foods) { board->foods->previous = new_food; } // next element in line point back to current element
  board->foods = new_food;
}

// update the score value for each food element to randomize the gameplay
void score_update (PointList* foods) {
  PointList* iterator = foods;
  while (iterator) {
    iterator->point = (rand()%7)+1;
    iterator = iterator->next;
  }
}

// remove an element from list. If element is valid, remove and return true. Else return false
bool remove_from_list (PointList* element, PointList** list) { // lists are bidirectional
  if (element) {
    if (element == *list) {
      *list = (*list)->next;
    }
    if (element->previous) { // if previous element is not NULL
      element->previous->next = element->next;
    }
    if (element->next) { // if next element is not NULL
      element->next->previous = element->previous;
    }
    free(element);
    return true;
  }else {
    return false;
  }
}
