#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "maze.h"
#include "player.h"

int submit(char* id, char* server, bool solvable);
