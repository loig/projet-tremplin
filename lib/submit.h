#ifndef _submit_
#define _submit_

#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "maze.h"
#include "player.h"
#include "errors.h"

int submit(bool solvable);

#endif
