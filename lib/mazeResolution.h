/**
 * \file mazeResolution.h
 * \brief resolution maze library
 * \author Albert.G, Simon.P, Alexis.G
 * mazeResolution contain all resolution algorithms. These algorithms are written by students.
 */
#ifndef _mazeResolution_
#define _mazeResolution_

/**
 * left hand algorithm   
*/
void lefthand_algorithm();

/**
 * pledge algorithm   
*/
void pledge_algorithm();

/**
 * personal algorithm   
*/
void personal_algorithm();


/**
 * all functions students are allow tu use
*/
bool is_solvable();
bool arrival_reached();
void turn_left();
void turn_right();
bool is_move_forward_possible();
void move_forward();
void write_coord_terminal();
void print_terminal();

void submit(bool);


#endif
