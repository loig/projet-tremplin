/**
 *
 * \file player.c
 * \brief library player.
 * \author Albert.G, Simon.P, Alexis.G
 * \version 1.0
 * \date 17 fevrier 2020
 *
 * library containing methods for the player.
 */

#include "player.h"

extern void display_dashboard();
extern void display_maze();

/**
* \brief initialize the player attribute. The coordinates are defined at the maze start.
*        The orientation is defined as NORTH.
*        nb_steps is defined as 0.
*/
void set_player() {
  player.coordinates.x = lab.start.x;
  player.coordinates.y = lab.start.y;
  player.orientation = NORTH;
}

/**
* \brief Check if a forward move is possible.
* \return "true" if the movement is possible, "false" is the movement is impossible.
*/
bool is_move_forward_possible(){
  if (player.orientation == NORTH) {
    if (player.coordinates.y-1 < 0 || lab.map[player.coordinates.y-1][player.coordinates.x]) {
      return false;
    }
  }
  if (player.orientation == EAST) {
    if (player.coordinates.x+1 > lab.sizeX-1 || lab.map[player.coordinates.y][player.coordinates.x+1]) {
      return false;
    }
  }
  if (player.orientation == SOUTH) {
    if (player.coordinates.y+1 > lab.sizeY-1 || lab.map[player.coordinates.y+1][player.coordinates.x]) {
      return false;
    }
  }
  if (player.orientation == WEST) {
    if (player.coordinates.x-1 < 0 || lab.map[player.coordinates.y][player.coordinates.x-1]) {
      return false;
    }
  }
  return true;
}

/**
* \brief Move the player forward. Do not check if the movement is possible.
*/
void move_forward(){
  usleep(parameters.speed);
  if (is_move_forward_possible()) {
    if (player.orientation == NORTH) {
      player.coordinates.y--;
    }
    if (player.orientation == EAST) {
      player.coordinates.x++;
    }
    if (player.orientation == SOUTH) {
      player.coordinates.y++;
    }
    if (player.orientation == WEST) {
      player.coordinates.x--;
    }
    //player.course[player.coordinates.y][player.coordinates.x] = 1;
    player.nb_steps++;
    if(parameters.display != 9){
      display_maze();
      display_dashboard();
    }
    return;
  }
  endwin();
  printf("Moving to %d %d unauthorized\n",player.coordinates.y, player.coordinates.x);
  exit(1);
}

/**
* \brief Check if he arrival as been reached.
* \return "true" if the arrival is reached, "false" if the arrival is not reached.
*/
bool arrival_reached(){
  return (player.coordinates.x == lab.arrival.x && player.coordinates.y == lab.arrival.y);
}

/**
* \brief Turn the player in a specific orientation.
* \param orientation orientation in which the player will turn.
*/
void orientate(Orientation orientation){
  player.orientation = orientation;
  if(parameters.display != 9){
    display_maze();
    display_dashboard();
  }
}

/**
* \brief Turn the player to his left. No forward move is done with this fucntion, it just change the orientation.
*/
void turn_left(){
  player.nb_orientate_left++;
  orientate((player.orientation + 3) % 4);
}

/**
* \brief Turn the player to his right. No forward move is done with this fucntion, it just change the orientation.
*/
void turn_right(){
  player.nb_orientate_right++;
  orientate((++player.orientation) % 4);
}
