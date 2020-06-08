/**
 * \file maze.h
 * \brief library maze.
 * \author Albert.G, Simon.P, Alexis.G
 * \date 17 fevrier 2020
 *
 * \brief maze contains all the functions necessary to initialize a maze.
 */

#ifndef _maze_
#define _maze_
#include <stdbool.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "queue.h"
#include "errors.h"
#include "display.h"

/**
 * \struct maze
 * \brief maze describe the maze.
 *
 * start contains the entry coordinates in the maze.
 *
 * arrival contains the exit coordinates in the maze.
 *
 * sizeX contains the size of the x axis of the maze.
 *
 * sizeY contains the size of the y axis of the maze.
 *
 * map contains a boolean matrix with a size of sizeX * sizeY,
 * which describe each square of the maze.
 * "true" mean a wall, "false" mean an empty square.
 *
 */
typedef struct maze{
    Coordonnes start;
    Coordonnes arrival;
    int sizeX;
    int sizeY;
    bool** map;
} maze;

/**
 * \struct Parameters
 * \brief Parameters describe all the parameters selected by the user for the maze.
 *
 * labName is the file name of the maze.
 *
 * execName is the file name of the executable
 *
 * display contains the display mode of the maze.
 *
 * generation contains the generation mode of the maze.
 * genSizeX/genSizeY contains the X/Y size of the maze to be generated
 *
 * verbose contains the verbose mode of the maze.
 *
 * speed contains the speed of the resolution.
 *
 * contest is a boolean that indicate if the contest is activated.
 * "true" for activated, "false" for desactivated
 *
 * start contains the entry coordinates selected by the user.
 *
 * arrival contains the exit coordinates selected by the user.
 *
 * randomStart is a boolean that indicate if the random start is activated.
 * "true" for activated, "false" for desactivated
 *
 * randomArrival is a boolean that indicate if the random arrival is activated.
 * "true" for activated, "false" for desactivated
 */
typedef struct Parameters {
  char* labName;
  char* execName;
  char* userName;
  char* serverAdress;
  int display;
  int resolution;
  int generation;
  int genSizeX;
  int genSizeY;
  int verbose;
  int speed;
  bool contest;
  Coordonnes start;
  Coordonnes arrival;
  bool randomStart;
  bool randomArrival;
}Parameters;

maze lab;
Parameters parameters;

/**
 * \brief function to know the size of a map
*/
void size_map_lab();

/**
 * \brief Function for reading a text file from a labyrinth
 * and create the cartsize_map_labe of the labyrinth in a matrix of characters.
*/
void load_map_lab();

/**
* \brief check if the maze is solvent
* \return "true" is the maze is solvent, "false" if the maze insolvent
*/
bool is_solvable();

/**
* \brief set the start on random square of the map. Avoid wall and arrival.
*/
void random_start();

/**
* \brief set the arrival on random square of the map. Avoid wall and start.
*/
void random_arrival();

/**
* \brief set the start on the specific coordinates on the map.
*        If set on a wall or an arrival, exit the program.
* \param start, the entry coordinates to set.
*/
void set_start_lab(Coordonnes start);

/**
* \brief set the arrival on the specific coordinates on the map.
*        If set on a wall or a start, exit the program.
* \param arrival, the exit coordinates to set.
*/
void set_arrival_lab(Coordonnes arrival);

/**
* \brief set the maze with the parameters declared above.
*/
void maze_initialization();

/**
 * \brief display parameters we can use when we launch the program
*/
void displayParameters();


/**
* \brief check if there is a wall on the given coordinates
* \param coordinates, coordinates the coordinates to check
* \param take_bound, take_bound if set at "true", take bound as a wall
* \return bool, "true" if there is a wall (or a bound, in fuction of the given parameters) "false" if there is no wall
*/
bool there_is_a_wall(Coordonnes coordinates, bool take_bound);

bool is_outside_lab(Coordonnes coordinates);

/**
* \brief fill an array with the adjacents squares of a specific square
* \param adjacentSquare, adjacentSquare the array to fill
* \param actualSquare, actualSquare the square to take in account
* \return Coordonnes*, an array wich contains all adjacents square for the top-left to the left,
*         in clockwise order.
*/
Coordonnes* generate_adjacent_square(Coordonnes[8], Coordonnes);

/**
 * \brief method to generate random maze
*/
void rand_gen();

#endif
