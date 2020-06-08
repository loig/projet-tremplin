/**
 *
 * \file player.h
 * \brief library player.
 * \author Albert.G, Simon.P, Alexis.G
 * \version 1.0
 * \date 17 février 2020
 *
 * library containing prototypes methods for the player.
 */

#ifndef _player_
#define _player_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "maze.h"
#include "display.h"
#include "player.h"
#include "errors.h"

/**
* \enum Orientation
* \brief Declaration of the different orientations available.
*/
typedef enum Orientation {NORTH, EAST, SOUTH, WEST} Orientation;


/**
 * \struct Player
 * \brief player describe the player situation in the labyrinthe.
 * coordinates contains the actual coordinates of the player.
 * orientation contains the actual orientation of the player, which can be between 0 to 3. (See Orientation struct above)
 * nb_steps contains the number of steps the player did since the begining of the algorithm.
 */
typedef struct Player{
    Coordonnes coordinates;
    Orientation orientation;
    int nb_steps;
    int nb_orientate_right;
    int nb_orientate_left;
} Player;

/**
* \brief global variable used by the function below. This use of a single variable allow
*        us to handle one player at time
*/
Player player;

__clock_t timeStart;

/**
* \brief Check if a forward move is possible.
* \return "true" if the movement is possible, "false" is the movement is impossible.
*/
bool is_move_forward_possible();

/**
* \brief Turn the player in a specific orientation.
* \param orientation orientation in which the player will turn.
*/
void orientate(Orientation);

/**
* \brief Move the player forward. Do not check if the movement is possible.
*/
void move_forward();

/**
* \brief Check if he arrival as been reached.
* \return "true" if the arrival is reached, "false" if the arrival is not reached.
*/
bool arrival_reached();

/**
* \brief Turn the player to his left. No forward move is done with this fucntion, it just change the orientation.
*/
void turn_left();

/**
* \brief Turn the player to his right. No forward move is done with this fucntion, it just change the orientation.
*/
void turn_right();

/**
* \brief initialize the player attribute. The coordinates are defined at the maze start.
*        The orientation is defined as NORTH.
*        nb_steps is defined as 0.
*/
void set_player();

#endif
