#include "submit.h"

// Soumission d'un résultat
int submit(bool solvable) {

  char* id = "test-test";
  char* server = "127.0.0.1";

  // Vérification que le résultat est correct
  //   - si le labyrinthe est soluble, il faut que la position courante soit à l'arrivée et solvable à vrai
  //   - sinon il faut que solvable soit à faux
  char* flag = (char*)calloc(3, sizeof(char));
  int hasSol = is_solvable();
  if (hasSol) {
    if (player.coordinates.x == lab.arrival.x && player.coordinates.y == lab.arrival.y) {
      flag = "ok";
    } else {
      flag = "ko";
    }
  } else {
    if (solvable) {
      flag = "ko";
    } else {
      flag = "ok";
    }
  }

  // Calcul du hash courant
  char* currentHash = "test";

  // Communication avec le serveur
  char* message = (char*)calloc(300, sizeof(char));
  sprintf(message, "%s:::%s:::%s\n", id, currentHash, flag);

  int sockfd, portno, n;
  struct sockaddr_in serv_addr;

  char buffer[256];
  portno = 8192;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    fprintf(stderr,"ERROR opening socket");
    exit(0);
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  int addr_ok = inet_aton(server, &serv_addr.sin_addr);
  if (!addr_ok) {
    fprintf(stderr,"ERROR, invalid address\n");
    exit(0);
  }
  serv_addr.sin_port = htons(portno);

  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
    fprintf(stderr,"ERROR connecting");
    exit(0);
  }

  n = write(sockfd,message,strlen(message));
  if (n < 0) {
    fprintf(stderr,"ERROR writing to socket");
    exit(0);
  }

  memset(buffer,0,256);
  n = read(sockfd,buffer,255);
  if (n < 0) {
    fprintf(stderr,"ERROR reading from socket");
    exit(0);
  }

  close(sockfd);

  int toReturn = 0;
  switch (buffer[0]) {
  default:
    toReturn = -1;
  }

  // On attend un peu pour ne pas surcharger le serveur

  sleep(1);

  // On affiche le prochain labyrinthe à résoudre

  return toReturn;

}
