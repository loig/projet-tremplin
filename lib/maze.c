/**
 *
 * \file maze.c
 * \brief library maze.
 * \author Albert.G, Simon.P, Alexis.G
 * \date 17 fevrier 2020
 *
 * library containing methods necessary to generate a playable labyrinth
 */

#include "maze.h"
#include "display.h"

#define CASE_VIDE 1
#define CASE_PLEINE 2
#define CASE_JOUEUR 3
#define CASE_ARRIVE 4
#define CASE_DEPART 5
#define CASE_NOIR 6

Coordonnes start;
Coordonnes end;
int speed = 25000;

/**
 * \brief function to know the size of a map
*/
void size_map_lab() {

  FILE* fichier = NULL;
  fichier = fopen(parameters.labName, "r");

  if (fichier == NULL) {
    printf("With the options you used, you must specify a fodaly file\n");
    display_help();
    exit(1);
  }

  int caractereActuel = 0;
  int colonX = 0;
  int tmpx = 0;
  int nbMaxColon = 0;
  int nbMaxLine = 0;
  int x = 0;
  int y = 0;

  do {
    caractereActuel = fgetc(fichier); // read character

    if (caractereActuel != '0' && caractereActuel != '1' && caractereActuel != '\n' && caractereActuel != EOF) {
      printf("Bad character in the maze definition\n");
      exit(EXIT_FAILURE);
    }

    //si le caractére actuel est une valeur de case
    if (caractereActuel == '0' || caractereActuel == '1') {
      x++;
    }

    //if the current character is a line break
    if (caractereActuel == '\n' && x==0) {
      colonX += tmpx;  //we make a line break with the sector
      if (y != nbMaxLine) {
        printf("y = %d nbMaxLine = %d\n", y,nbMaxLine);
        printf("Wrong number of lines in this block\n");
        exit(EXIT_FAILURE);
      }
      nbMaxColon=0;
      y=0;

      //if the current character is a line break
    } else if (caractereActuel == '\n') {
      y++;
      if (y > nbMaxLine) {
        nbMaxLine=y;
      }
      if (!nbMaxColon) {
        nbMaxColon=x;
      }
      tmpx=x;
      if (x != nbMaxColon) {
        printf("Wrong column number in this block\n");
        exit(EXIT_FAILURE);
      }
      x=0; //Line break
    }

  } while (caractereActuel != EOF);

  lab.sizeX = colonX+tmpx;
  lab.sizeY = nbMaxLine;

  fclose(fichier);
}

/**
 * \brief Function for reading a text file from a labyrinth
 * and create the cartsize_map_labe of the labyrinth in a matrix of characters.
*/
void load_map_lab() {

  lab.map = (bool**) malloc(sizeof(bool*)*lab.sizeY);
  for (int i = 0; i < lab.sizeY; i++) {
    lab.map[i] = (bool*) malloc(sizeof(bool)*lab.sizeX);
  }

  FILE* fichier = NULL;
  fichier = fopen(parameters.labName, "r");


  if (fichier == NULL) {
    printf("With the options you used, you must specify a fodaly file\n");
    display_help();
    exit(1);
  }

  int caractereActuel = 0;
  int colonX = 0;
  int tmpx = 0;
  int x = 0;
  int y = 0;

  do {
    caractereActuel = fgetc(fichier); // read character

    //if the current character is a cell value
    if (caractereActuel == '0' || caractereActuel == '1') {
      lab.map[y][colonX+x] = caractereActuel - '0';
      x++;
    }

    //if the current character is a line break (we change block line)
    if (caractereActuel == '\n' && x==0) {
      colonX += tmpx;  //we make a line break with the sector
      y=0;
    } else if (caractereActuel == '\n') {
      y++; //line break
      tmpx=x;
      x=0; //line break
    }
  } while (caractereActuel != EOF);
  fclose(fichier);
}

/**
* \brief check if there is a wall on the given coordinates
* \param coordinates, coordinates the coordinates to check
* \param take_bound, take_bound if set at "true", take bound as a wall
* \return bool, "true" if there is a wall (or a bound, in fuction of the given parameters) "false" if there is no wall
*/
bool there_is_a_wall(Coordonnes coordinates, bool take_bound) {

  if (take_bound) {
    return coordinates.x < 0
    || coordinates.y < 0
    || lab.sizeX <= coordinates.x
    || lab.sizeY <= coordinates.y
    || lab.map[coordinates.y][coordinates.x];
  }

  else {
    return coordinates.x >= 0
    && coordinates.y >= 0
    && lab.sizeX > coordinates.x
    && lab.sizeY > coordinates.y
    && lab.map[coordinates.y][coordinates.x];
  }
}

//--------------------------
//maze generation
//--------------------------

Coordonnes* generate_adjacent_square(Coordonnes adjacentSquare[8], Coordonnes actualSquare) {

  Coordonnes top_left = {actualSquare.x-1,actualSquare.y-1};
  adjacentSquare[0] = top_left;
  Coordonnes top = {actualSquare.x,actualSquare.y-1};
  adjacentSquare[1] = top;
  Coordonnes top_right = {actualSquare.x+1,actualSquare.y-1};
  adjacentSquare[2] = top_right;
  Coordonnes right = {actualSquare.x+1,actualSquare.y};
  adjacentSquare[3] = right;
  Coordonnes bottom_right = {actualSquare.x+1,actualSquare.y+1};
  adjacentSquare[4] = bottom_right;
  Coordonnes bottom = {actualSquare.x,actualSquare.y+1};
  adjacentSquare[5] = bottom;
  Coordonnes bottom_left = {actualSquare.x-1,actualSquare.y+1};
  adjacentSquare[6] = bottom_left;
  Coordonnes left = {actualSquare.x-1,actualSquare.y};
  adjacentSquare[7] = left;

  return adjacentSquare;
}

/**
* \brief use to know how many cell available around a cell.
* \param coordinates the current coordinate
* \param array the list of available cell around the coordinates
* \return numbers of available cell. (those who are not a wall)
*/
int coordinates_near_coordiantes(Coordonnes coordinates, Coordonnes array[]){
  int cmp = 0;
  Coordonnes tmp = {coordinates.x - 1, coordinates.y};
  if (!there_is_a_wall(tmp,true)){
    array[cmp] = tmp;
    cmp++;
  }
  tmp.x = coordinates.x+1;
  if (!there_is_a_wall(tmp,true)) {
    array[cmp] = tmp;
    cmp++;
  }
  tmp.x = coordinates.x;
  tmp.y = coordinates.y-1;
  if (!there_is_a_wall(tmp,true)){
    array[cmp] = tmp;
    cmp++;
  }
  tmp.y = coordinates.y+1;
  if (!there_is_a_wall(tmp,true)){
    array[cmp] = tmp;
    cmp++;
  }
  return cmp;
}

void white_maze() {
  for (int x = 0; x < lab.sizeX; x++) {
    for (int y = 0; y < lab.sizeY; y++) {
      lab.map[y][x] = false;
    }
  }
}

void roots_set_by_probability(SetQueue* setqueue) {
  int nb_root = 0;

  int limit = (lab.sizeX + lab.sizeY);
  for (int i = 0; i < limit; i++) {
    if (rand() % limit > i)
      nb_root++;
  }

  for (int i = 0; i < nb_root; i++) {
    bool valide = false;
    Coordonnes racine;
    while (!valide) {
      valide = true;
      if (rand () % 100 > 10) {
        if (rand () % 100 > 50) racine.x = 0;
        else racine.x = lab.sizeX-1;
      } else racine.x = rand() % lab.sizeX;

      if (rand () % 100 > 10) {
        if (rand () % 100 > 50) racine.y = 0;
        else racine.y = lab.sizeY-1;
      } else racine.y = rand() % lab.sizeY;

      Coordonnes adjacentSquare[8];
      generate_adjacent_square(adjacentSquare, racine);

      if (there_is_a_wall(racine,true))
        valide = false;

      int nbWall = 0;
      for (int j = 0; j < 8; j++) {

        if (there_is_a_wall(adjacentSquare[j],false)) {

          nbWall++;
          if (nbWall > 0)
            valide = false;
        }
      }

    }
    setqueue_add_coor(setqueue,racine);
    lab.map[racine.y][racine.x] = 1;
  }

}

void roots_set_by_side(SetQueue* setqueue) {
  Coordonnes racine;

  racine.x = lab.sizeX/2;
  racine.y = lab.sizeY/2;
  setqueue_add_coor(setqueue,racine);
  lab.map[racine.y][racine.x] = 1;

  for (int i = rand()%5; i >= 0;i--) {
    racine.x = 0;
    racine.y = rand() % lab.sizeY;
    setqueue_add_coor(setqueue,racine);
    lab.map[racine.y][racine.x] = 1;
  }

  for (int i = rand()%7; i >= 0;i--) {
    racine.x = rand() % lab.sizeX;
    racine.y = 0;
    setqueue_add_coor(setqueue,racine);
    lab.map[racine.y][racine.x] = 1;
  }

  for (int i = rand()%5; i >= 0;i--) {
    racine.x = lab.sizeX-1;
    racine.y = rand() % lab.sizeY;
    setqueue_add_coor(setqueue,racine);
    lab.map[racine.y][racine.x] = 1;
  }

  for (int i = rand()%5; i >= 0;i--) {
    racine.x = rand() % lab.sizeX;
    racine.y = lab.sizeY-1;
    setqueue_add_coor(setqueue,racine);
    lab.map[racine.y][racine.x] = 1;
  }
}

void grow_maze(SetQueue* setqueue) {
  Coordonnes actualSquare;
  if (!setqueue_is_empty(setqueue))
    actualSquare = setqueue_pop(setqueue);

  while (!setqueue_is_empty(setqueue)) {

    bool valide = true;

    actualSquare = setqueue_pop(setqueue);
    for (int i = (rand() % 2)*1; i >= 0; i--) {
      setqueue_add_coor(setqueue,actualSquare);
      actualSquare = setqueue_pop(setqueue);
    }

    Coordonnes adjacentSquare[8];
    generate_adjacent_square(adjacentSquare, actualSquare);

    int nbWall = 0;
    for (int i = 0; i < 8; i++) {
      if (there_is_a_wall(adjacentSquare[i],true)) {
        nbWall++;
        if (nbWall > 3)
          valide = false;
      }
    }
    if (valide && nbWall > 1) {
      valide = false;
      for (int i = 0; i < 7; i += 2) {
        for (int j = 0, ConsecutiveWall = 0; j < 3; j++) {
          if (there_is_a_wall(adjacentSquare[(j+i)%8],true)) {
            ConsecutiveWall++;
          }
          if (ConsecutiveWall == nbWall) {
            valide = true;
          }
        }
      }
    }

    if (valide) {
      lab.map[actualSquare.y][actualSquare.x] = 1;
      for (int i = 1; i < 8; i += 2) {
        if (!there_is_a_wall(adjacentSquare[i],true)) {
        setqueue_add_coor(setqueue,adjacentSquare[i]);
        }
      }
    }
  }
  setqueue_free(setqueue);
}

void root_maze_generation() {
  SetQueue* setqueue = init_setqueue();

  roots_set_by_probability(setqueue);
  grow_maze(setqueue);

}

void generate_map_lab(int mode) {
  lab.map = (bool**) malloc(sizeof(bool*)*lab.sizeY);
  for (int i = 0; i < lab.sizeY; i++) {
    lab.map[i] = (bool*) malloc(sizeof(bool)*lab.sizeX);
  }

  switch (mode) {
  case 1 :
    white_maze();
    root_maze_generation();
    break;
  case 2 :
    rand_gen();


  }

  return;
}

/**
 * \brief method to generate random maze
*/
void rand_gen(){
  for (int i = 0; i < lab.sizeY; i++)
  {
    for (int j = 0; j < lab.sizeX; j++)
    {
      lab.map[i][j] = false;
    }

  }
  int randX;
  int randY;
  for (int i = 0; i < (lab.sizeX * lab.sizeY)/2.8; i++)
  {
    randY = rand() % lab.sizeY;
    randX = rand() % lab.sizeX;
    if (lab.map[randY][randX] == false) lab.map[randY][randX] = true;
  }
}


/**
* \brief set the start on the specific coordinates on the map.
*        If set on a wall or an arrival, exit the program.
* \param start, the entry coordinates to set.
*/
void set_start_lab(Coordonnes start) {
  lab.start = start;
  lab.map[start.x][start.y] = 0;
}

/**
* \brief set the arrival on the specific coordinates on the map.
*        If set on a wall or a start, exit the program.
* \param arrival, the exit coordinates to set.
*/
void set_arrival_lab(Coordonnes arrival) {
  lab.arrival = arrival;
}

/**
* \brief check if the maze is solvent
* \return "true" is the maze is solvent, "false" if the maze insolvent
*/
bool is_solvable() {
  Coordonnes actual_coordinates;
  SetQueue* setqueue = init_setqueue();
  SetQueue* coordinates_pass = init_setqueue();
  setqueue_add_coor(setqueue, lab.start);
  Coordonnes near_coordinates[4];
  int count_near_coor = 0;
  char found;
  do{
    found = 0;
    actual_coordinates = setqueue_pop(setqueue);
    if(actual_coordinates.x != -1 && actual_coordinates.y != -1){
      count_near_coor = coordinates_near_coordiantes(actual_coordinates, near_coordinates);
      for (int i = 0; i < count_near_coor; i++){
        if (near_coordinates[i].x == lab.arrival.x && near_coordinates[i].y == lab.arrival.y){
          found = 1;
        }
      }
      if (!found){
        for (int i = 0; i < count_near_coor; i++){
          if (!setqueue_contains(coordinates_pass, near_coordinates[i]))
            setqueue_add_coor(setqueue, near_coordinates[i]);
        }
      }
      setqueue_add_coor(coordinates_pass, actual_coordinates);
    }
  } while (!(setqueue_is_empty(setqueue) || found));
  setqueue_free(setqueue);
  //setqueue_free(coordinates_pass); this function create display's problems of coordinates
  return found;
}

/**
* \brief set the start on random square of the map. Avoid wall and arrival.
*/
void random_start(){
  Coordonnes square;
  square.x = rand() % lab.sizeX;
  square.y = rand() % lab.sizeY;
  set_start_lab(square);
}

/**
* \brief set the arrival on random square of the map. Avoid wall and start.
*/
void random_arrival(){
  Coordonnes square;
  do {
    square.x = rand() % lab.sizeX;
    square.y = rand() % lab.sizeY;
  } while (square.x != start.x && square.y != start.y);
  set_arrival_lab(square);
}

void cooldown(int temps){
  speed = temps;
}

/**
* \brief set the maze with the parameters declared above.
*/
void maze_initialization() {



  //Load labyrinth
  if (parameters.generation && parameters.contest == false) {
    lab.sizeX = parameters.genSizeX;
    lab.sizeY = parameters.genSizeY;
    generate_map_lab(parameters.generation);
  } else {
    //calculate the map size
    size_map_lab();
    //load the map
    load_map_lab();
  }

  //set the default start
  Coordonnes start = {0,0};
  set_start_lab(start);

  //set the default arrival
  Coordonnes arrival = {lab.sizeX-1,lab.sizeY-1};
  set_arrival_lab(arrival);

  if (parameters.randomStart) {
    random_start();
  }

  if (parameters.randomArrival) {
    random_arrival();
  }

  if (parameters.start.x > 0 && parameters.start.y > 0) {
    set_start_lab(parameters.start);
  }

  if (parameters.arrival.x > 0 && parameters.arrival.y > 0) {
    set_arrival_lab(parameters.arrival);
  }

  if (parameters.contest == true)
  {
    if (parameters.resolution == 9){
      printf("You can't choose keyboard resolution with the contest mode\n");
      exit(EXIT_FAILURE);
    }else{
    parameters.speed = 0;
    parameters.display = 9;
    parameters.resolution = 0;
    set_arrival_lab(arrival);
    set_start_lab(start);
    parameters.execName = "first_lab.fodaly";
    }
  }

  lab.map[start.y][start.x] = 0;
  lab.map[arrival.y][arrival.x] = 0;
}

/**
 * \brief display parameters we can use when we launch the program
*/
void displayParameters() {
  printf("maze name : %s\n", parameters.labName);
  printf("executable name : %s\n", parameters.execName);
  printf("display mode : %d\n", parameters.display);
  printf("resolution mode : %d\n", parameters.resolution);
  printf("generation mode : %d\n", parameters.generation);
  printf("verbose mode : %d\n", parameters.verbose);
  printf("speed : %d\n", parameters.speed);
  printf("contest mode : %d\n", parameters.contest);
  printf("start : %d,%d\n", parameters.start.x,parameters.start.y);
  printf("arrival : %d,%d\n", parameters.arrival.x,parameters.arrival.y);
  printf("random start mode : %d\n", parameters.randomStart);
  printf("random arrival mode : %d\n", parameters.randomArrival);
}
