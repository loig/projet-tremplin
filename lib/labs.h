// labs provides basic utility functions to deal
// with a player in a labyrinth.
#include <stdbool.h>

// move_forward() moves the player of one step
// in its current looking direction. Initially
// the player looks up.
void move_forward();

// turn_left() changes the lookind direction of
// the player by rotating her of a quarter of a
// round to the left.
void turn_left();

// turn_right() changes the lookind direction of
// the player by rotating her of a quarter of a
// round to the right.
void turn_right();

// is_move_forward_possible() tells whether or not
// the player can move forward, that is, whether or
// not there is a wall in front of the player. It
// should always be used before the move_forward()
// function.
bool is_move_forward_possible();

// arrival_reached() tells whether or not the
// player is standing on the arrival of the
// labyrinth.
bool arrival_reached();

// is_solvable() tells whether or not there exists
// a path in the labyrinth from the starting position
// to the arrival.
bool is_solvable();

// write_coord_terminal() displays the coordinates of
// the current position of the player in the terminal.
void write_coord_terminal();

// print_terminal(s) displays the string s in the
// terminal.
void print_terminal(char* s);

// to be explained later
void submit(bool);
