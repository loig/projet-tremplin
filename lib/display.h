/**
 * \file display.h
 * \brief Display contain all the functions necessary to initialize and update
 * a labyrinth using Ncurses.
 * \author Albert.G, Simon.P, Alexis.G
 * \version 1.0
 * \date 17 février 2020
 *
 * library containing prototypes of methods necessary to generate a display of labyrinth
 *
 */

#ifndef _display_
#define _display_
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include "stack.h"
#include "maze.h"
#include "player.h"
#include "errors.h"

WINDOW *window, *mazeWindow, *legende, *tableauDeBord, *terminal;


/**
* \brief Display explanations on the textual help usage of the program
*/
void display_help();

/**
* \brief Display the textual help usage of the program
*/
void display_man();

/**
* \brief Display legend in order for explanations on the color code of cells
* \param xDebutLegende position x from where the legend begins
* \param yDebutLegende position y from where the legend begins
*/
void display_legende(int, int);

/**
* \brief Display the dashboard to see informations about program : file name, step count and time.
*/
void display_dashboard();

/**
* \brief write in program terminal like player position or if he reach the finish
* \param str it's the pointer on the string we want to display in the program terminal
*/
void write_terminal(char*);

/**
* \brief write in program terminal
*/
void write_coord_terminal();

/**
* \brief print in program terminal
*/
void print_terminal();

/**
* \brief print in program terminal the cursor
*/
void reprint_terminal_cursor();

/**
* \brief display the maze
*/
void display_maze();

/**
* \brief start display the initialization of maze
*/
void displayInitialization();

/**
* \brief get the size of the list that was displayed in the terminal
*/
int get_size_list_terminal();

#endif
