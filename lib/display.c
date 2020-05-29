/**
 *
 * \file display.c
 * \brief Display contain all the functions necessary to initialize and update
 * a labyrinth using Ncurses.
 * \author Albert.G, Simon.P, Alexis.G
 * \version 1.0
 * \date 17 février 2020
 *
 * library containing the methods necessary to generate a display of labyrinth
 */

#include "display.h"

#define CASE_VIDE 1
#define CASE_PLEINE 2
#define CASE_JOUEUR 3
#define CASE_ARRIVE 4
#define CASE_DEPART 5
#define CASE_NOIR 6

char* cleaner;
List listTerminal = {NULL, 0};
int sizeCleaner = 0;

void display_help() {
  printf("Use %s -h for complete help\n", parameters.execName);
}

void display_man() {
  printf("SYNOPSIS\n");
  printf("       %s [OPTION]... MAZE\n", parameters.execName);
  printf("       %s --g [OPTION]...\n", parameters.execName);
  printf("\n");
  printf("DESCRIPTION\n");
  printf("       Display MAZE or a randomly generated maze.\n");
  printf("\n");
  printf("       -a ALGNUM\n");
  printf("              choose the algorithm used to solve the maze. The value of ALGNUM indicates the algorithm to use: 0 for the left hand algorithm (default), 1 for the Pledge algorithm, 2 for a personal algorithm, 3 for comparing the three algorithms, and 9 for interactive keyboard solving.\n");
  printf("\n");
  printf("       --arrival X,Y\n");
  printf("              set the coordinates of the arrival to (X,Y).\n");
  printf("\n");
  printf("       --arrival-random\n");
  printf("              randomly set the coordinates of the arrival.\n");
  printf("\n");
  printf("       -d MODE\n");
  printf("              choose the display mode. The value MODE indicates the mode to use: 0 for full display of the maze (default), 1 for displaying only the height positions around the player, 2 for display only the position in front of the player, 3 for drawing the maze while the player progresses, 4 for displaying only what the player can see, 9 for displaying nothing. All modes but 9 require that the current terminal supports coloring.\n");
  printf("\n");
  printf("       --g     randomly generate the maze instead of reading it from a file.\n");
  printf("\n");
  printf("       -h     display this help.\n");
  printf("\n");
  printf("       --speed STEP\n");
  printf("              choose the time to spend between two moves of the player. The value STEP indicates the time to use: 0 for no time between moves, 1 for 10ms between moves, 2 for 100ms between moves, 3 for 25ms between moves (default), N for Nms between moves.\n");
  printf("\n");
  printf("       --start X,Y\n");
  printf("              set the coordinates of the start to (X,Y).\n");
  printf("\n");
  printf("       --start-random\n");
  printf("              randomly set the coordinates of the start.\n");
  printf("\n");
  printf("       --xsize N\n");
  printf("              choose the size of the maze in the x direction (this option can only be used with the -g option).\n");
  printf("\n");
  printf("       --ysize N\n");
  printf("              choose the size of the maze in the y direction (this option can only be used with the -g option).\n");
  printf("\n");
  printf("AUTHOR\n");
  printf("        Written by Albert Guihard, Alexis Guillotin, Simon Perrin, and Loïg Jezequel.\n");
  printf("\n");
  printf("REPORTING BUGS\n");
  printf("        loig.jezequel@univ-nantes.fr\n");
}

/**
* \brief get the size of the list that was displayed in the terminal
*/
int get_size_list_terminal(){
  return listTerminal.size;
}

/**
* \brief Display legend in order for explanations on the color code of cells
* \param xDebutLegende position x from where the legend begins
* \param yDebutLegende position y from where the legend begins
*/
void display_legende(int xDebutLegende, int yDebutLegende){
  attron(COLOR_PAIR(CASE_JOUEUR));
  mvhline(yDebutLegende+1, xDebutLegende+1, ' ', 4);

  mvhline(yDebutLegende+2, xDebutLegende+1, ' ', 4);
  attroff(COLOR_PAIR(CASE_JOUEUR));
  move(yDebutLegende+2, xDebutLegende+6);
  printw("Player box");

  attron(COLOR_PAIR(CASE_VIDE));
  mvhline(yDebutLegende+3, xDebutLegende+1, ' ', 4);

  mvhline(yDebutLegende+4, xDebutLegende+1, ' ', 4);
  attroff(COLOR_PAIR(CASE_VIDE));
  move(yDebutLegende+4, xDebutLegende+6);
  printw("Not a wall");

  attron(COLOR_PAIR(CASE_PLEINE));
  mvhline(yDebutLegende+5, xDebutLegende+1, ' ', 4);

  mvhline(yDebutLegende+6, xDebutLegende+1, ' ', 4);
  attroff(COLOR_PAIR(CASE_PLEINE));
  move(yDebutLegende+6, xDebutLegende+6);
  printw("Wall");

  attron(COLOR_PAIR(CASE_DEPART));
  mvhline(yDebutLegende+7, xDebutLegende+1, ' ', 4);

  mvhline(yDebutLegende+8, xDebutLegende+1, ' ', 4);
  attroff(COLOR_PAIR(CASE_DEPART));
  move(yDebutLegende+8, xDebutLegende+6);
  printw("Start box");

  attron(COLOR_PAIR(CASE_ARRIVE));
  mvhline(yDebutLegende+9, xDebutLegende+1, ' ', 4);

  mvhline(yDebutLegende+10, xDebutLegende+1, ' ', 4);
  attroff(COLOR_PAIR(CASE_ARRIVE));
  move(yDebutLegende+10, xDebutLegende+6);
  printw("Arrival box");
}

bool default_view_square(Coordonnes actual_square){
  int tmp = actual_square.x;
  tmp++;
  return true;
}

bool minimalist_view_square(Coordonnes actual_square){
  Coordonnes adjacent_square[8];
  generate_adjacent_square(adjacent_square, player.coordinates);

  return ((actual_square.x == adjacent_square[player.orientation*2+1].x
  && actual_square.y == adjacent_square[player.orientation*2+1].y)
  || (actual_square.x == player.coordinates.x
  && actual_square.y == player.coordinates.y));
}

bool eight_view_square(Coordonnes actual_square){
  return abs(actual_square.x-player.coordinates.x) < 2
  && abs(actual_square.y-player.coordinates.y) < 2;
}

bool footstep_view_square(Coordonnes actual_square){
  int tmp = actual_square.x;
  tmp++;

  if (actual_square.x < -1 || actual_square.y < -1) {
    return false;
  }
  if (actual_square.x >= lab.sizeX+1 || actual_square.y >= lab.sizeY+1) {
    return false;
  }

  static bool footprints_generated = false;
  static bool** footprints;

  if (!footprints_generated) {
    footprints = (bool**) malloc(sizeof(bool*)*lab.sizeY+2);
    for (int i = 0; i < lab.sizeY+2; i++) {
      footprints[i] = (bool*) malloc(sizeof(bool)*lab.sizeX+2);
    }
    for (int x = 0; x < lab.sizeX+2; x++) {
      for (int y = 0; y < lab.sizeY+2; y++) {
        footprints[y][x] = false;
      }
    }
    footprints_generated = true;
  }

  if (abs(actual_square.x-player.coordinates.x) < 2
  && abs(actual_square.y-player.coordinates.y) < 2) {

    footprints[actual_square.y+1][actual_square.x+1] = true;
  }

  return footprints[actual_square.y+1][actual_square.x+1];
}

bool bresenham(int x0, int y0, int x1, int y1) {
  // checks if there is a wall on the line from (x0, y0)
  // to (x1, y1), the line being defined according to
  // Bresenham algorithm

  // straight vertical line
  if (x0 == x1) {
    int x = x0;
    if (y0 < y1) {
      for (int y = y0; y <= y1; y++) {
        if (y != y0 && y != y1) {
          Coordonnes currentPointOnLine = {x, y};
          if (there_is_a_wall(currentPointOnLine, true)) return false;
        }
      }
      return true;
    } else {
      for (int y = y1; y <= y0; y++) {
        if (y != y0 && y != y1) {
          Coordonnes currentPointOnLine = {x, y};
          if (there_is_a_wall(currentPointOnLine, true)) return false;
        }
      }
      return true;
    }
  }

  // straight horizontal line
  if (y0 == y1) {
    int y = y0;
    if (x0 < x1) {
      for (int x = x0; x <= x1; x++) {
        if (x != x0 && x != x1) {
          Coordonnes currentPointOnLine = {x, y};
          if (there_is_a_wall(currentPointOnLine, true)) return false;
        }
      }
      return true;
    } else {
      for (int x = x1; x <= x0; x++) {
        if (x != x0 && x != x1) {
          Coordonnes currentPointOnLine = {x, y};
          if (there_is_a_wall(currentPointOnLine, true)) return false;
        }
      }
      return true;
    }
  }

  // lower right
  if (x1 > x0 && y1 > y0) {
    int deltaX = x1 - x0;
    int deltaY = y1 - y0;
    if (deltaX > deltaY) {
      float deltaError = (float)deltaY / (float)deltaX;
      float error = 0.0;
      int y = y0;
      for (int x = x0; x <= x1; x++) {
        if ((x != x0 || y != y0) && (x != x1 || y != y1)) {
          Coordonnes currentPointOnLine = {x, y};
          if (there_is_a_wall(currentPointOnLine, true)) return false;
        }
        error += deltaError;
        if (error >= 0.5) {
          y++;
          error -= 1.0;
        }
      }
      return true;
    } else {
      float deltaError = (float)deltaX / (float)deltaY;
      float error = 0.0;
      int x = x0;
      for (int y = y0; y <= y1; y++) {
        if ((x != x0 || y != y0) && (x != x1 || y != y1)) {
          Coordonnes currentPointOnLine = {x, y};
          if (there_is_a_wall(currentPointOnLine, true)) return false;
        }
        error += deltaError;
        if (error >= 0.5) {
          x++;
          error -= 1.0;
        }
      }
      return true;
    }
  }

  // lower left
  if (x1 < x0 && y1 > y0) {
    int deltaX = x0 - x1;
    int deltaY = y1 - y0;
    if (deltaX > deltaY) {
      float deltaError = (float)deltaY / (float)deltaX;
      float error = 0.0;
      int y = y0;
      for (int x = x1; x <= x0; x++) {
        if ((x != x0 || y != y0) && (x != x1 || y != y1)) {
          Coordonnes currentPointOnLine = {x, y};
          if (there_is_a_wall(currentPointOnLine, true)) return false;
        }
        error += deltaError;
        if (error >= 0.5) {
          y++;
          error -= 1.0;
        }
      }
      return true;
    } else {
      float deltaError = (float)deltaX / (float)deltaY;
      float error = 0.0;
      int x = x1;
      for (int y = y0; y <= y1; y++) {
        if ((x != x0 || y != y0) && (x != x1 || y != y1)) {
          Coordonnes currentPointOnLine = {x, y};
          if (there_is_a_wall(currentPointOnLine, true)) return false;
        }
        error += deltaError;
        if (error >= 0.5) {
          x++;
          error -= 1.0;
        }
      }
      return true;
    }
  }

  // upper right
  if (x1 > x0 && y1 < y0) {
    int deltaX = x1 - x0;
    int deltaY = y0 - y1;
    if (deltaX > deltaY) {
      float deltaError = (float)deltaY / (float)deltaX;
      float error = 0.0;
      int y = y1;
      for (int x = x0; x <= x1; x++) {
        if ((x != x0 || y != y0) && (x != x1 || y != y1)) {
          Coordonnes currentPointOnLine = {x, y};
          if (there_is_a_wall(currentPointOnLine, true)) return false;
        }
        error += deltaError;
        if (error >= 0.5) {
          y++;
          error -= 1.0;
        }
      }
      return true;
    } else {
      float deltaError = (float)deltaX / (float)deltaY;
      float error = 0.0;
      int x = x0;
      for (int y = y1; y <= y0; y++) {
        if ((x != x0 || y != y0) && (x != x1 || y != y1)) {
          Coordonnes currentPointOnLine = {x, y};
          if (there_is_a_wall(currentPointOnLine, true)) return false;
        }
        error += deltaError;
        if (error >= 0.5) {
          x++;
          error -= 1.0;
        }
      }
      return true;
    }
  }

  // upper left
  if (x1 < x0 && y1 < y0) {
    int deltaX = x0 - x1;
    int deltaY = y0 - y1;
    if (deltaX > deltaY) {
      float deltaError = (float)deltaY / (float)deltaX;
      float error = 0.0;
      int y = y1;
      for (int x = x1; x <= x0; x++) {
        if ((x != x0 || y != y0) && (x != x1 || y != y1)) {
          Coordonnes currentPointOnLine = {x, y};
          if (there_is_a_wall(currentPointOnLine, true)) return false;
        }
        error += deltaError;
        if (error >= 0.5) {
          y++;
          error -= 1.0;
        }
      }
      return true;
    } else {
      float deltaError = (float)deltaX / (float)deltaY;
      float error = 0.0;
      int x = x1;
      for (int y = y1; y <= y0; y++) {
        if ((x != x0 || y != y0) && (x != x1 || y != y1)) {
          Coordonnes currentPointOnLine = {x, y};
          if (there_is_a_wall(currentPointOnLine, true)) return false;
        }
        error += deltaError;
        if (error >= 0.5) {
          x++;
          error -= 1.0;
        }
      }
      return true;
    }
  }

  return false;
}

bool line_of_sight_view_square(Coordonnes actual_square){

  // always display the player
  if (player.coordinates.x == actual_square.x && player.coordinates.y == actual_square.y) {return true;}

    // directly visible
    if (bresenham(player.coordinates.x, player.coordinates.y, actual_square.x, actual_square.y)) {
      return true;
    }

    // close to a non-wall visible square
    if (actual_square.x > player.coordinates.x) {
      Coordonnes closer = {actual_square.x - 1, actual_square.y};
      if (!there_is_a_wall(closer, true)) {
        if (bresenham(player.coordinates.x, player.coordinates.y, actual_square.x - 1, actual_square.y)) {
          return true;
        }
      }
    }

    if (actual_square.x < player.coordinates.x) {
      Coordonnes closer = {actual_square.x + 1, actual_square.y};
      if (!there_is_a_wall(closer, true)) {
        if (bresenham(player.coordinates.x, player.coordinates.y, actual_square.x + 1, actual_square.y)) {
          return true;
        }
      }
    }

    if (actual_square.y > player.coordinates.y) {
      Coordonnes closer = {actual_square.x, actual_square.y - 1};
      if (!there_is_a_wall(closer, true)) {
        if (bresenham(player.coordinates.x, player.coordinates.y, actual_square.x, actual_square.y - 1)) {
          return true;
        }
      }
    }

    if (actual_square.y < player.coordinates.y) {
      Coordonnes closer = {actual_square.x, actual_square.y + 1};
      if (!there_is_a_wall(closer, true)) {
        if (bresenham(player.coordinates.x, player.coordinates.y, actual_square.x, actual_square.y + 1)) {
          return true;
        }
      }
    }

    return false;
}

/*
###############
#####
###
###
#

*/
/*
void draw3D(int x, int y) {

}
*/

/*
//taille du tableau : 28 "cubes" (deux espaces)
bool troisD_view() {

  switch(player.orientation) {

    case 0 :

      for (int x = 0; x < 15; x++) {
        Coordonnes possible_square = {player.coordinates.x -7 + x, player.coordinates.y - 4};
        if (there_is_a_wall(possible_square,true)) {
          draw3D(x,0);
        }
      }

      for (int x = 0; x < 5; x++) {
        Coordonnes possible_square = {player.coordinates.x -2 + x, player.coordinates.y - 3};
        if (there_is_a_wall(possible_square,true)) {
          draw3D(x,1);
        }
      }

      for (int x = 0; x < 3; x++) {
        Coordonnes possible_square = {player.coordinates.x -1 + x, player.coordinates.y - 2};
        if (there_is_a_wall(possible_square,true)) {
          draw3D(x,2);
        }
      }

      Coordonnes possible_square = {player.coordinates.x - 1, player.coordinates.y - 1};
      if (there_is_a_wall(possible_square,true)) {
        draw3D(7,3);
      }

      break;
  }

  return false;
}
*/

/**
* \brief display the maze
*/
void display_maze() {

  int placement_offset;
  int maze_offset;
  int square_size_x;
  int square_size_y;
  if (lab.sizeX>12 || lab.sizeY>12) {
    square_size_x = 2;
    square_size_y = 1;
    placement_offset = 13;
    maze_offset = 27;
  } else {
    square_size_x = 4;
    square_size_y = 2;
    placement_offset = 6;
    maze_offset = 13;
  }

  int marginAffichage = 3;
  int ligne = marginAffichage;
  int coll = marginAffichage;
  int start_x;
  int start_y;

  if(parameters.display || lab.sizeX>maze_offset){
    if(!parameters.display && player.coordinates.x - placement_offset < 0){
      start_x = -1;
    }else{
      start_x = player.coordinates.x - placement_offset;
      int offset;
      if(!parameters.display && (offset = lab.sizeX - start_x) <= maze_offset){
        start_x -= (maze_offset - offset);
      }
    }
  }else{
    start_x = (lab.sizeX - maze_offset - 1)/2;
  }

  if(parameters.display || lab.sizeY>maze_offset){
    if(!parameters.display && player.coordinates.y - placement_offset < 0){
      start_y = -1;
    }else{
      start_y = player.coordinates.y - placement_offset;
      int offset;
      if(!parameters.display && (offset = lab.sizeY - start_y) <= maze_offset){
        start_y -= (maze_offset - offset);
      }
    }
  }else{
    start_y = (lab.sizeY - maze_offset - 1)/2;
  }

  bool (*is_square_display_needed)(Coordonnes);

  switch (parameters.display) {
  case 0 :
    is_square_display_needed = default_view_square;
    break;
  case 1 :
    is_square_display_needed = eight_view_square;
    break;
  case 2 :
    is_square_display_needed = minimalist_view_square;
    break;
  case 3 :
    is_square_display_needed = footstep_view_square;
    break;
  case 4 :
    is_square_display_needed = line_of_sight_view_square;
    break;
    /*
  case 5 :
    troisD_view();
    break;
    */
  default:
    is_square_display_needed = default_view_square;
    break;
  }

  for (int i = start_y; i <= start_y+maze_offset; i++){
    for (int j = start_x; j <= start_x+maze_offset; j++){

      Coordonnes actual_square = {j,i};

/*
      if (parameters.display == 5) {
        troisD_view(actual_square);

      } else */
      if (!is_square_display_needed(actual_square)) {
        attron(COLOR_PAIR(CASE_NOIR));
        for (int height = 0; height < square_size_y; height++) {
          mvhline(ligne+height, coll, ' ', square_size_x);
        }
        attroff(COLOR_PAIR(CASE_VIDE));
      } else if (there_is_a_wall(actual_square,true)){
          if (is_outside_lab(actual_square)) {
            attron(COLOR_PAIR(CASE_NOIR));
            mvhline(ligne, coll, ' ', square_size_x);
            for (int height = 0; height < square_size_y; height++) {
              mvhline(ligne+height, coll, ' ', square_size_x);
            }
            attroff(COLOR_PAIR(CASE_NOIR));
          } else {
            attron(COLOR_PAIR(CASE_PLEINE));
            mvhline(ligne, coll, ' ', square_size_x);
            for (int height = 0; height < square_size_y; height++) {
              mvhline(ligne+height, coll, ' ', square_size_x);
            }
            attroff(COLOR_PAIR(CASE_PLEINE));
          }
      }else if(player.coordinates.x == j && player.coordinates.y == i){

        char* orientation_char = "^>v<";

        attron(COLOR_PAIR(CASE_JOUEUR));
        for (int height = 0; height < square_size_y; height++) {
          mvhline(ligne+height, coll, orientation_char[player.orientation], square_size_x);
        }
        attroff(COLOR_PAIR(CASE_JOUEUR));

      }else if(lab.arrival.x == j && lab.arrival.y == i) {

        attron(COLOR_PAIR(CASE_ARRIVE));
        for (int height = 0; height < square_size_y; height++) {
          mvhline(ligne+height, coll, ' ', square_size_x);
        }
        attroff(COLOR_PAIR(CASE_ARRIVE));

      }else if(lab.start.x == j && lab.start.y == i){
        // display cell arrival

        attron(COLOR_PAIR(CASE_DEPART));
        for (int height = 0; height < square_size_y; height++) {
          mvhline(ligne+height, coll, ' ', square_size_x);
        }
        attroff(COLOR_PAIR(CASE_DEPART));

      } else {
        attron(COLOR_PAIR(CASE_VIDE));
        for (int height = 0; height < square_size_y; height++) {
          mvhline(ligne+height, coll, ' ', square_size_x);
        }
        attroff(COLOR_PAIR(CASE_VIDE));
      }
      coll += square_size_x;
    }
    coll = marginAffichage;
    ligne += square_size_y;
  }
  refresh();
}

/**
* \brief print in program terminal the cursor
*/
void reprint_terminal_cursor(int cursor){
  int offSetY = 3;
  int offSetX = 61;
  int size = listTerminal.size;
  for (int i = LINES - 3; i >= offSetY; i--){
    mvprintw(i, offSetX, cleaner);
  }

  for (int i = cursor, j = LINES-3; i < size && j >= 3; i++, j--){
    mvprintw(j, offSetX, getListChar(&listTerminal, i));
  }
  refresh();
}

/**
* \brief print in program terminal
*/
void print_terminal(char* newStr){
  int offSetY = 3;
  int offSetX = 61;
  int size = listTerminal.size;

  if (0 == sizeCleaner){
    sizeCleaner = COLS - offSetX - 3;
    cleaner = (char*) malloc(sizeof(char) * sizeCleaner);
    for(int j = 0; j < sizeCleaner; ++j){
      cleaner[j] = ' ';
    }
  }

  if(sizeCleaner > (COLS-offSetX) - 3){
    addHeadList(&listTerminal, "Too long string");
  }else{
    addHeadList(&listTerminal, newStr);
  }

  for (int i = LINES - 3; i >= offSetY; i--){
    mvprintw(i, offSetX, cleaner);
  }
  for (int i = 0, j = LINES-3; i <= size && j >= 3; i++, j--){
    mvprintw(j, offSetX, getListChar(&listTerminal, i));
  }
  refresh();
}

/**
* \brief write in program terminal like player position or if he reach the finish
* \param str it's the pointer on the string we want to display in the program terminal
*/
void write_terminal(char* str){
  if(parameters.display != 9)
    print_terminal(str);
  else
    printf("%s\n", str);
}

/**
* \brief write in program terminal
*/
void write_coord_terminal(){
  char str[20] = {'\0'};
  int xPlayer = player.coordinates.x;
  int yPlayer = player.coordinates.y;
  sprintf(str, "x: %d|y: %d", xPlayer, yPlayer);
  if(parameters.display != 9)
    print_terminal(str);
  else
    printf("%s\n", str);

}

/**
* \brief Display the dashboard to see informations about program : file name, step count and time.
*/
void display_dashboard(){
  int yDebutTableauDeBord = 32;
  int xDebutTableauDeBord = 35;
  mvprintw(yDebutTableauDeBord + 2, xDebutTableauDeBord + 1, "File name :");
  int tailleString = 0;
  while (parameters.labName[tailleString] != '\0'){
    tailleString++;
  }
  if (tailleString > 23){
    for (int i = 0; i < 18; i++){
      mvprintw(yDebutTableauDeBord + 3, i + xDebutTableauDeBord + 1, "%c", parameters.labName[i]);
    }
    mvprintw(yDebutTableauDeBord + 3, xDebutTableauDeBord + 19, "...");
  }else{
    mvprintw(yDebutTableauDeBord + 3, xDebutTableauDeBord + 1, "%s", parameters.labName);
  }
  mvprintw(yDebutTableauDeBord + 5, xDebutTableauDeBord + 1, "Step count :");
  mvprintw(yDebutTableauDeBord + 6, xDebutTableauDeBord + 1, "%d", player.nb_steps);
  mvprintw(yDebutTableauDeBord + 8, xDebutTableauDeBord + 1, "Time (without delay) :");
  mvprintw(yDebutTableauDeBord + 9, xDebutTableauDeBord + 1, "%f6 s", (double) (clock()-timeStart)/CLOCKS_PER_SEC);
}

/**
* \brief start display the initialization of maze
*/
void displayInitialization () {
  srand(time(NULL));
  char processus_place[100];
  getcwd(processus_place, sizeof(processus_place));
  timeStart = clock();

  int largeurFenetreLab = ((12 + 2)*4)+2; //12 --> size lab
  int hauteurFenetreLab = (12+3)*2;
  if(parameters.display != 9){
    initscr();
    curs_set(0);
    if (LINES < 45 || COLS < 94){
      printw("The terminal window is not large enough to have a correct display.\nPlease enlarge it, please.\nCurrent size: %d lines, %d columns.\nMinimum size required: 45 lines, 94 columns.", LINES, COLS);
      getch();
      endwin();
      exit(EXIT_FAILURE);
    }

    /**Color initialization
     * CASE_VIDE: there is no wall, the box is available.
     * CASE_PLEINE: there is a wall, the box is unavailable.
     * PLAYER_BOX: the player is present on the box.
     */
    use_default_colors();
    start_color();
    init_pair(CASE_VIDE, COLOR_WHITE, COLOR_WHITE);
    init_pair(CASE_PLEINE, COLOR_BLUE, COLOR_BLUE);
    init_pair(CASE_JOUEUR, COLOR_BLACK, COLOR_RED);
    init_pair(CASE_ARRIVE, COLOR_GREEN, COLOR_GREEN);
    init_pair(CASE_DEPART, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(CASE_NOIR, COLOR_BLACK, COLOR_BLACK);

    window = subwin(stdscr, LINES, COLS, 0, 0);
    box(window, ACS_VLINE, ACS_HLINE);
    mvwprintw(window, 0, 1, "Project Tremplin | Screen size: %d * %d | %s/%s | %s", LINES, COLS, processus_place, parameters.labName, parameters.execName);

    mazeWindow = subwin(stdscr, hauteurFenetreLab, largeurFenetreLab, 2, 2);
    box(mazeWindow, ACS_VLINE, ACS_HLINE);
    mvwprintw(mazeWindow, 0, 1, "Maze");

    int xDebutTableauDeBord = 35;
    int yDebutTableauDeBord = 32;
    tableauDeBord = subwin(stdscr, LINES - yDebutTableauDeBord - 1, largeurFenetreLab - yDebutTableauDeBord - 1, yDebutTableauDeBord, xDebutTableauDeBord);
    box(tableauDeBord, ACS_VLINE, ACS_HLINE);
    mvwprintw(tableauDeBord, 0, 1, "Dashboard");

    int largeurLegende = largeurFenetreLab - 25;

    legende = subwin(stdscr, LINES - yDebutTableauDeBord - 1, largeurLegende, yDebutTableauDeBord, 2);
    box(legende, ACS_VLINE, ACS_HLINE);
    mvwprintw(legende, 0, 1, "Caption");

    terminal = subwin(stdscr, LINES - 3, COLS - largeurFenetreLab - 4, 2, 2 + largeurFenetreLab);
    box(terminal, ACS_VLINE, ACS_HLINE);
    mvwprintw(terminal, 0, 1, "Terminal");

    if(has_colors() == FALSE){
      printf("Error: Your terminal does not support color.\n");
      getch();
      endwin();
      exit(EXIT_FAILURE);
    }
    display_legende(2, yDebutTableauDeBord);
    display_dashboard();
    display_maze();
  }
}
