// Handling fatal errors
#include "errors.h"

void handleFatalError(char* error, bool running, bool displayHelp) {
  if (running && parameters.display != 9) {
    write_terminal(error);
    write_terminal("Press q to quit.");
    keypad(window, TRUE);
    bool done = false;
    while(!done){
      int c = wgetch(window);
      switch(c){
      case 'q':
        done = true;
        break;
      default:
        break;
      }
    }
    endwin();
  } else {
    printf("%s\n", error);
    if (displayHelp) {
      printf("Use %s -h for complete help\n", parameters.execName);
    }
  }
  exit(EXIT_FAILURE);
}
