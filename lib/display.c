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

bool line_of_sight_view_square(Coordonnes actual_square){

  // always display the player
  if (player.coordinates.x == actual_square.x &&
      player.coordinates.y == actual_square.y)
    return true;

    int testX = 17;
    int testY = 3;

    if (actual_square.x == testX && actual_square.y == testY) {
      char buf[150];
      sprintf(buf, "Looking at square %d, %d", actual_square.x, actual_square.y);
      print_terminal(buf);
    }

    // Straight line from the player X increasing
    // and borders of this line
    if ((actual_square.y == player.coordinates.y
        || actual_square.y + 1 == player.coordinates.y
        || actual_square.y - 1 == player.coordinates.y)
        && actual_square.x > player.coordinates.x) {
      for (int x = player.coordinates.x + 1; x < actual_square.x; x++) {
        Coordonnes toLook = {x, player.coordinates.y};
        if (there_is_a_wall(toLook, true)) {
          return false;
        }
      }
      return true;
    }

    // Straight line from right of the player Y increasing
    // and borders of this line
    if ((actual_square.x == player.coordinates.x
        || actual_square.x + 1 == player.coordinates.x
        || actual_square.x - 1 == player.coordinates.x)
        && actual_square.y > player.coordinates.y) {
      for (int y = player.coordinates.y + 1; y < actual_square.y; y++) {
        Coordonnes toLook = {player.coordinates.x, y};
        if (there_is_a_wall(toLook, true)) {
          return false;
        }
      }
      return true;
    }

  // We try to go from the player to the actual_square in a line as straight
  // as possible and check that there is no wall on this line (if several lines
  // are equivalent it is sufficient that there is no wall on one of them to
  // consider that the player can see actual_square)
  int currentX = player.coordinates.x;
  int currentY = player.coordinates.y;
  if (currentX <= actual_square.x) {
    if (currentY <= actual_square.y) {
      currentX++;
      currentY++;
      int diffX = actual_square.x - currentX;
      int diffY = actual_square.y - currentY;
      int stepX = diffX + 1;
      int stepY = diffY + 1;
      if (diffY != 0) stepX = diffX/diffY + 1;
      if (diffX != 0) stepY = diffY/diffX + 1;
      if (diffX == diffY) {
        stepX = 1;
        stepY = 1;
      }
      int currentStepX = stepX;
      int currentStepY = stepY;
      while (diffX > 0 || diffY > 0) {

if (actual_square.x == testX && actual_square.y == testY) {
      char buf[150];
      sprintf(buf, "     Must go through square %d, %d (diffX: %d, diffY: %d, currentStepX: %d, currentStepY: %d)", currentX, currentY, diffX, diffY, currentStepX, currentStepY);
      print_terminal(buf);
    }

        Coordonnes toLook = {currentX, currentY};
        if (there_is_a_wall(toLook, true)) {
          return false;
        }
        if (currentStepX > currentStepY) {
          currentX++;
          currentStepX--;
          diffX--;
        } else if (currentStepX < currentStepY) {
          currentY++;
          currentStepY--;
          diffY--;
        } else {
          //Coordonnes toLooka = {currentX + 1, currentY};
          //Coordonnes toLookb = {currentX, currentY + 1};

          if (actual_square.x == testX && actual_square.y == testY) {
                char buf[150];
                sprintf(buf, "     Must go through square %d, %d or square %d, %d", currentX+1, currentY, currentX, currentY + 1);
                print_terminal(buf);
              }

          /*if (there_is_a_wall(toLooka, true) && there_is_a_wall(toLookb, true)) {
            return false;
          }*/
          if (stepX > stepY) {
            currentY++;
            currentStepY--;
            diffY--;
          } else {
            currentX++;
            currentStepX--;
            diffX--;
          }
        }
        if (currentStepX == 0 && diffX > 0) currentStepX = stepX;
        if (currentStepY == 0 && diffY > 0) currentStepY = stepY;
      }
      return true;
    } /*else {
      int diffX = actual_square.x - currentX;
      int diffY = currentY - actual_square.y;
      while (diffX != 0 || diffY != 0) {
        Coordonnes toLook = {currentX, currentY};
        if (there_is_a_wall(toLook, true)) {
          return false;
        }
        if (diffX == diffY) {
          Coordonnes toLooka = {currentX + 1, currentY};
          Coordonnes toLookb = {currentX, currentY - 1};
          if (there_is_a_wall(toLooka, true) && there_is_a_wall(toLookb, true)) {
            return false;
          }
          currentX++;
          currentY--;
        } else if (diffX < diffY) {
          currentY--;
        } else {
          currentX++;
        }
        diffX = actual_square.x - currentX;
        diffY = currentY - actual_square.y;
      }
      return true;
    }*/
  } /*else {
    if (currentY <= actual_square.y) {
      int diffX = currentX - actual_square.x;
      int diffY = actual_square.y - currentY;
      while (diffX != 0 || diffY != 0) {
        Coordonnes toLook = {currentX, currentY};
        if (there_is_a_wall(toLook, true)) {
          return false;
        }
        if (diffX == diffY) {
          Coordonnes toLooka = {currentX - 1, currentY};
          Coordonnes toLookb = {currentX, currentY + 1};
          if (there_is_a_wall(toLooka, true) && there_is_a_wall(toLookb, true)) {
            return false;
          }
          currentX--;
          currentY++;
        } else if (diffX < diffY) {
          currentY++;
        } else {
          currentX--;
        }
        diffX = currentX - actual_square.x;
        diffY = actual_square.y - currentY;
      }
      return true;
    } else {
      int diffX = currentX - actual_square.x;
      int diffY = currentY - actual_square.y;
      while (diffX != 0 || diffY != 0) {
        Coordonnes toLook = {currentX, currentY};
        if (there_is_a_wall(toLook, true)) {
          return false;
        }
        if (diffX == diffY) {
          Coordonnes toLooka = {currentX - 1, currentY};
          Coordonnes toLookb = {currentX, currentY - 1};
          if (there_is_a_wall(toLooka, true) && there_is_a_wall(toLookb, true)) {
            return false;
          }
          currentX--;
          currentY--;
        } else if (diffX < diffY) {
          currentY--;
        } else {
          currentX--;
        }
        diffX = currentX - actual_square.x;
        diffY = currentY - actual_square.y;
      }
      return true;
    }
  }*/


return false;
}

/*
###############
#####
###
###
#

*/
void draw3D(int x, int y) {

}

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

/**
* \brief display the maze
*/
void display_maze() {

  int placement_offset;
  int maze_offset;
  int square_size_x;
  int square_size_y;
  if (lab.sizeX>27 || lab.sizeY>27) {
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
    start_x = -1;
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
    start_y = -1;
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
  case 5 :
    troisD_view();
    break;
  }

  for (int i = start_y; i <= start_y+maze_offset; i++){
    for (int j = start_x; j <= start_x+maze_offset; j++){

      Coordonnes actual_square = {j,i};

      if (parameters.display == 5) {
        troisD_view(actual_square);

      } else if (!is_square_display_needed(actual_square)) {
        attron(COLOR_PAIR(CASE_NOIR));
        for (int height = 0; height < square_size_y; height++) {
          mvhline(ligne+height, coll, ' ', square_size_x);
        }
        attroff(COLOR_PAIR(CASE_VIDE));
      } else if (there_is_a_wall(actual_square,true)){
        attron(COLOR_PAIR(CASE_PLEINE));
        mvhline(ligne, coll, ' ', square_size_x);
        for (int height = 0; height < square_size_y; height++) {
          mvhline(ligne+height, coll, ' ', square_size_x);
        }
        attroff(COLOR_PAIR(CASE_PLEINE));

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
  for (int i = 0, j = LINES-3; i < size && j >= 3; i++, j--){
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
      exit(1);
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
      printf("Your terminal does not support color.\n");
      getch();
      endwin();
      exit(1);
    }
    display_legende(2, yDebutTableauDeBord);
    display_dashboard();
    display_maze();
  }
}
