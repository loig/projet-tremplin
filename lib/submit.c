#include "submit.h"

// Calcul du hash d'un labyrinthe
void get_maze_hash(char* hash){

  if (lab.sizeX != 12 || lab.sizeY != 12) {
    strcpy(hash, "wrongLab");
    return;
  }

  int elements[9];
  int elem = 0;

  for (int pady = 0; pady < 12; pady += 4) {
    for (int padx = 0; padx < 12; padx += 4) {
      elements[elem] = 0;
      for (int y = pady; y < pady + 4; y++) {
        for (int x = padx; x < padx + 4; x++) {
          elements[elem] = 2 * elements[elem] + lab.map[y][x];
        }
      }
      elem++;
    }
  }

  sprintf(hash, "%d:%d:%d:%d:%d:%d:%d:%d:%d", elements[0], elements[1], elements[2], elements[3], elements[4], elements[5], elements[6], elements[7], elements[8]);

}

// Soumission d'un résultat
int submit(bool solvable) {

  if (parameters.contest) {

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
    char* currentHash = (char*)calloc(54, sizeof(char));
    get_maze_hash(currentHash);
    //printf("Hash: %s\n", currentHash);

    // Communication avec le serveur
    char* message = (char*)calloc(300, sizeof(char));
    sprintf(message, "%s:::%s:::%s\n", parameters.userName, currentHash, flag);

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
    int addr_ok = inet_aton(parameters.serverAdress, &serv_addr.sin_addr);
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

    // Lecture de la réponse du serveur
    memset(buffer,0,256);
    n = read(sockfd,buffer,255);
    if (n < 0) {
      fprintf(stderr,"ERROR reading from socket");
      exit(0);
    }

    if (n < 19) {
      fprintf(stderr,"WRONG message from server");
      exit(0);
    }

    close(sockfd);

    // Return code from the server
    int toReturn = buffer[0] - 48;

    // On attend un peu pour ne pas surcharger le serveur
    sleep(1);

    // On affiche le prochain labyrinthe à résoudre
    printf("Next maze: %s\n", &buffer[2]);

    return toReturn;

  } else {

    return 0;
  }

}
