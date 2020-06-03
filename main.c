/**
 *
 * \file main.c
 * \brief main file witch can be excecute to run the program
 * \author Albert.G, Simon.P, Alexis.G
 * \version 1.0
 * \date 17 février 2020
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "lib/maze.h"
#include "lib/mazeResolution.h"
#include "lib/player.h"
#include "lib/display.h"
#include <unistd.h> //getopt_long
#include <getopt.h>
#include <ctype.h> //isdigit
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>

#define T true
#define F false
#define CASE_VIDE 1
#define CASE_PLEINE 2
#define CASE_JOUEUR 3
#define CASE_ARRIVE 4
#define CASE_DEPART 5
#define CASE_NOIR 6
#define PORT 8080

char int_to_hexa(int to_convert){
  switch (to_convert){
  case 10:
    return 'A';
  case 11:
    return 'B';
  case 12:
    return 'C';
  case 13:
    return 'D';
  case 14:
    return 'E';
  case 15:
    return 'F';
  default:
    return '0' + to_convert;
  }
}

int hexa_to_int(char to_convert){
  switch (to_convert){
  case 'A':
    return 10;
  case 'B':
    return 11;
  case 'C':
    return 12;
  case 'D':
    return 13;
  case 'E':
    return 14;
  case 'F':
    return 15;
  default:
    return to_convert - '0';
  }
}

void get_maze_hash(char hash[100]){
  hash[0] = '.';
  int char_count = 0;

  if(lab.sizeX == 12 && lab.sizeY == 12){
    for(int padx = 0; padx < 12; padx += 4){
      for(int pady = 0; pady < 12; pady += 4){
        for(int i = padx; i < padx+4; i++){
          int to_convert = 0;
          int to_pow = 3;
          for(int j = pady; j < pady+4; j++){
            to_convert += (lab.map[i][j] ? 1 : 0)*pow(2, to_pow);
            to_pow--;
          }
          hash[char_count] = int_to_hexa(to_convert);
          char_count++;
        }
      }
    }
  }
  hash[char_count] = '\n';
}

void submit(bool solvable){
  endwin();
  const char* json_model = "{\"student\": \"%s\", \"solve\": %d, \"count\": %d, \"status\": %d, \"orientateR\": %d, \"orientateL\": %d, \"time\": %f}\n";
  int sock = 0;
  struct sockaddr_in serv_addr;

  char message[1024];
  char buffer[1024] = {0};
  int flag = 1;

  char hash[100];
  get_maze_hash(hash);

  if(!solvable){
    if(is_solvable()){
      flag = 1;
    }else{
      flag = 0;
    }
  }else{
    if(arrival_reached()){
      flag = 0;
    }else{
      flag = 2;
    }
  }
  snprintf(message,  1023, json_model, getenv("USER"), 0, player.nb_steps, flag, player.nb_orientate_right, player.nb_orientate_left, (float) (clock()-timeStart)/CLOCKS_PER_SEC);
  printf("%f\n", (float) (clock()-timeStart)/CLOCKS_PER_SEC);
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
    printf("\nInvalid address / Address not supported \n");
    return;
  }

  if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
    printf("\nConnection Failed \n");
    return;
  }

  send(sock, message, strlen(message), 0);
  if(flag == 0){
    recv(sock, buffer, sizeof(buffer)-1, 0);
    if(strcmp("next", buffer) == 0){
      send(sock, hash, strlen(hash), 0);
      memset(buffer, 0, sizeof(buffer));
      recv(sock, buffer, sizeof(buffer)-1, 0);
    }
  }else{
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer)-1, 0);
  }
  strcpy(hash, buffer);


  switch (flag){
  case 0:
    puts("Fragments du prochain labyrinthe: ");
    break;
  case 1:
    puts("Vous avez signalé que le labyrinthe n'etait pas solvable, or il l'est.\nFragments du prochain labyrinthe: (c'est le meme que vous deviez faire precedemment)");
    break;
  case 2:
    puts("Vous avez signalé que vous aviez fini de résoudre le labyrinthe, or c'est faux.\nFragments du prochain labyrinthe: (c'est le meme que vous deviez faire precedemment)");
    break;
  }
  for(int frag = 0; frag < 9; frag++){
    printf("%d", hexa_to_int(hash[0]));
    for(int i = 1; i < (int)(strlen(hash)-1)/9; i++){
      printf("/%d", hexa_to_int(hash[i + (frag*4)]));
    }
    puts("");
  }

  close(sock);
  sleep(1);
}

/**
* \brief analyse all the option gived with the execution, and set the parameter struct with these
* \param argc number of parameter
* \param argv array of parameters, the parameter 0 is the name of the executable
*/
void analyseOption(int argc, char *argv[]) {

  static struct option long_options[] = {
                                        {"speed",    required_argument, 0,  't' }, //change speed
                                        {"contest",    no_argument, 0,  'u' }, //mode competition
                                        {"start-random",    no_argument, 0,  'w' }, //start set
                                        {"arrival-random",    no_argument, 0,  'x' }, //arrival set
                                        {"start",    required_argument, 0,  'y' }, //start set
                                        {"arrival",    required_argument, 0,  'z' }, //arrival set
                                        {"xsize", required_argument, 0, 'i'}, //size in x for gen
                                        {"ysize", required_argument, 0, 'j'}, //size in y for gen
                                        {"g", no_argument, 0, 'g'}, //size in y for gen
                                        {0,         0,                 0,  0 }
  };

  parameters.labName = ""; //name of the maze
  parameters.execName = argv[0]; //executable name
  parameters.display = 0; //display
  parameters.resolution = 0; //resolution
  parameters.generation = 0; //generation
  parameters.genSizeX = 12; // width of the generated maze
  parameters.genSizeY = 12; // height of the generated maze
  parameters.verbose = 0; //verbose
  parameters.speed = 0; //speed
  parameters.contest = false; //contest mode
  parameters.start.x = -1; //start
  parameters.start.y = -1; //start
  parameters.arrival.x = -1; //arrivalY
  parameters.arrival.y = -1; //arrivalY
  parameters.randomStart = false; //random start mode
  parameters.randomArrival = false; //random arrival mode

  int c;
  int index_table;
  while (1) {

    c = getopt_long(argc, argv, "a:d:h",long_options, &index_table);
    char * end;
    int val;

    if (c == -1) {
      if (optind < argc)
        parameters.labName = argv[optind];
      else
        parameters.labName = "";
      optind = 0;
      break;
    }

    switch (c) {
    case 'd'://display mode
    val = (int)strtol(optarg,&end,10);
    if (strcmp(end,"")) {
      printf("Incorrect syntax\n");
      exit(0);
    }
    parameters.display = val;
      break;

      case 'a'://display mode
      val = (int)strtol(optarg,&end,10);
      if (strcmp(end,"")) {
        printf("Incorrect syntax\n");
        exit(0);
      }
      parameters.resolution = val;
        break;

    case 'v'://verbose mode
      val = (int)strtol(optarg,&end,10);
      if (strcmp(end,"")) {
        printf("Incorrect syntax\n");
        exit(0);
      }
      parameters.verbose = val;
      break;

    case 'g'://generation
    /*
      val = (int)strtol(optarg,&end,10);
      if (strcmp(end,"")) {
        printf("Incorrect syntax\n");
        exit(0);
      }*/
      parameters.generation = 1;
      break;

      case 'i': //generation X size
      val = (int)strtol(optarg,&end,10);
      if (strcmp(end,"")) {
        printf("Incorrect syntax\n");
        exit(0);
      }
      parameters.genSizeX = val;
      break;

      case 'j': //generation Y size
      val = (int)strtol(optarg,&end,10);
      if (strcmp(end,"")) {
        printf("Incorrect syntax\n");
        exit(0);
      }
      parameters.genSizeY = val;
      break;

    case 'h'://help
      display_man();
      exit(0);
      break;

    case 't'://change speed
      val = (int)strtol(optarg,&end,10);
      if (strcmp(end,"")) {
        printf("Incorrect syntax\n");
        exit(0);
      }
      switch (val) {
      case 0:
        parameters.speed = 0;
        break;
      case 1:
        parameters.speed = 10000;
        break;
      case 2:
        parameters.speed = 100000;
        break;
      case 3:
        parameters.speed = 25000;
        break;
      default:
        parameters.speed = val * 1000;
        break;
      }
      break;

    case 'u'://contest
    parameters.display = 9;
    parameters.speed = 0;
    parameters.contest = true;
    break;

    case 'w'://start-random
      parameters.randomStart = true;
      break;

    case 'x'://arrival-random
      parameters.randomArrival = true;
      break;

    case 'y'://start
      //recovery of the first value
      val = (int)strtol(optarg,&end,10);
      if (end[0] != ',') {
        printf("Incorrect syntax\n");
        exit(0);
      }
      parameters.start.x = val;
      //recovery of the second value
      val = (int)strtol(&(end[1]),&end,10);
      if (end[0] != '\0') {
        printf("Incorrect syntax\n");
        exit(0);
      }
      parameters.start.y = val;
      break;

    case 'z'://arrival
    //recovery of the first value
    val = (int)strtol(optarg,&end,10);
    if (end[0] != ',') {
      printf("Incorrect syntax\n");
      exit(0);
    }
    parameters.arrival.x = val;
    //recovery of the second value
    val = (int)strtol(&(end[1]),&end,10);
    if (end[0] != '\0') {
      printf("Incorrect syntax\n");
      exit(0);
    }
    parameters.arrival.y = val;
      break;
    }
  }
}

/**
* \brief allow the user to solve the maze with the keybord
* \param window windows is the window where the modification will be made
*/
void keyboard_solve(WINDOW* window){
  while(!arrival_reached()){
    int c = wgetch(window);
    switch(c){
    case KEY_UP:
      if(is_move_forward_possible())
        move_forward();
      break;
    case KEY_LEFT:
      turn_left();
      break;
    case KEY_RIGHT:
      turn_right();
      break;
    case 'q':
      return;
      break;
    }
      write_coord_terminal();
  }
}

/**
* \brief allow the user to scroll the pseudo-terminal
* \param window windows is the window where the modification will be made
*/
void historic(WINDOW* window) {
  if (parameters.display != 9 && parameters.resolution != 9){
    int cmp = 0;
    bool choice = false;
    int max = get_size_list_terminal()-1;
    keypad(window, TRUE);
    while(!choice){
      int c = wgetch(window);
      switch(c){
      case KEY_UP:
        cmp++;
        if(cmp > max)
          cmp--;
        reprint_terminal_cursor(cmp);
        break;
      case KEY_DOWN:
        cmp--;
        if(cmp < 0)
          cmp = 0;
        reprint_terminal_cursor(cmp);
        break;
      case 'q':
        choice = true;
        break;
      default:
        break;
      }
    }
  }
}


void comparaison(){
  //Left_hand algorithm
  write_terminal("Starting left hand algorithm");
  double time_left = 0;
  timeStart = clock();
  lefthand_algorithm();
  bool reached_left = arrival_reached();
  int steps_left = player.nb_steps;
  int rotateLeft_left = player.nb_orientate_left;
  int rotateRight_left = player.nb_orientate_right;
  time_left = (double) (clock()-timeStart)/CLOCKS_PER_SEC;

  //Pledge algorithm
  write_terminal("Starting Pledge algorithm");
  double time_pledge = 0;
  maze_initialization();
  set_player();
  displayInitialization();
  timeStart = clock();
  pledge_algorithm();
  bool reached_pledge = arrival_reached();
  int steps_pledge = player.nb_steps;
  int rotateLeft_pledge = player.nb_orientate_left;
  int rotateRight_pledge = player.nb_orientate_right;
  time_pledge = (double) (clock()-timeStart)/CLOCKS_PER_SEC;

  //Personnal algorithm
  write_terminal("Starting personnal algorithm");
  double time_perso = 0;
  maze_initialization();
  set_player();
  displayInitialization();
  timeStart = clock();
  personal_algorithm();
  bool reached_perso = arrival_reached();
  int steps_perso = player.nb_steps;
  int rotateLeft_perso = player.nb_orientate_left;
  int rotateRight_perso = player.nb_orientate_right;
  time_perso = (double) (clock()-timeStart)/CLOCKS_PER_SEC;

  //display results
  char str_left[100];
  char str_pledge[100];
  char str_perso[100];
  if (reached_left) {
    sprintf(str_left, "Left hand algorithm: %fs, %d steps, %d left tuns, %d right turns", time_left, steps_left, rotateLeft_left, rotateRight_left);
  } else {
    sprintf(str_left, "Left hand algorithm: arrival not reached");
  }
  if (reached_pledge) {
    sprintf(str_pledge, "Pledge algorithm: %fs, %d steps, %d left tuns, %d right turns",time_pledge, steps_pledge, rotateLeft_pledge, rotateRight_pledge);
  } else {
    sprintf(str_pledge, "Pledge algorithm: arrival not reached");
  }
  if (reached_perso) {
    sprintf(str_perso, "Personnal algorithm: %fs, %d steps, %d left tuns, %d right turns",time_perso, steps_perso, rotateLeft_perso, rotateRight_perso);
  } else {
    sprintf(str_perso, "Personnal algorithm: arrival not reached");
  }

  write_terminal(str_left);
  write_terminal(str_pledge);
  write_terminal(str_perso);
}

/**
* \brief use the option in the parameter struct for select
*/
void resolution() {
  switch (parameters.resolution){
  case 0:
    lefthand_algorithm();
    break;
  case 1:
    pledge_algorithm();
    break;
  case 2:
    personal_algorithm();
    break;
  case 3:
    comparaison();
    break;
  case 9:
    keypad(window, TRUE);
    keyboard_solve(window);
    break;
  default:
    lefthand_algorithm();
    break;
  }
}

/**
* \brief initialize the maze, the player, the display, and run the resolution
* initialize the maze
* initialize the player
* initialize the display
* run the resolution of the maze
* active the scroll of the historic
* close the display
*/
int main(int argc, char *argv[]) {
  srand(time(NULL));

  analyseOption(argc,argv);

  maze_initialization();

  set_player();

  displayInitialization();

  resolution();

  historic(window);

  endwin();
  }
