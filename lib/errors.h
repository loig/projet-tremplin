#ifndef _errors_
#define _errors_

#include <ncurses.h>
#include <string.h>
#include "maze.h"
#include "display.h"

void handleFatalError(char* error, bool running, bool displayHelp);

#endif
