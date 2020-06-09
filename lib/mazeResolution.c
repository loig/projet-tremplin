/**
 * \file mazeResolution.c
 * \brief library maze solving.
 * \author Albert.G, Simon.P, Alexis.G
 *
 * library of different maze solving algorithms
 *
 */
#include <stdlib.h>
#include <unistd.h>
//#include "mazeResolution.h"
#include "labs.h"

void lefthand_algorithm() {
  int orientation = 0;
  if (0 || is_solvable()){
    while (!arrival_reached()){
      orientation = (orientation + 3) % 4;
      turn_left();
      if(is_move_forward_possible()){
        move_forward();
      }else{
        orientation = (orientation + 1) % 4;
        turn_right();
        if(is_move_forward_possible()){
          move_forward();
        }else{
          orientation = (orientation + 1) % 4;
          turn_right();
        }
      }
      write_coord_terminal();
    }
    write_terminal("Arrival reached !!!");
    submit(true);
  }else{
    write_terminal("No solution was found");
    submit(false);
  }
}


void pledge_algorithm() {
  write_coord_terminal();
  while (true) {
    turn_left();
    turn_left();
    move_forward();
    move_forward();
    move_forward();
    write_coord_terminal();
  }
}

void personal_algorithm() {
  write_coord_terminal();
  while (true) {
    turn_left();
    turn_left();
    move_forward();
    move_forward();
    write_coord_terminal();
  }
}
