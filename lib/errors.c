// Handling fatal errors
#include "errors.h"

void handleFatalError(char* error, bool running, bool displayHelp) {
  if (running && parameters.display != 9) {
    int len = strlen(error);
    char* errorForTerminal = calloc(len + 2, sizeof(char));
    sprintf(errorForTerminal, "%s\n", error);
    write_terminal(errorForTerminal);
    write_terminal("Press q to quit.\n");
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
