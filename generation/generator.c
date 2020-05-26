#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void usage() {
  printf("Usage: generator labID\n\twith 0 <= labID <= 65535\n");
}

int main(int argc, char* argv[]) {

  if (argc < 2) {
    printf("Not enough arguments\n");
    usage();
    exit(1);
  }

  if (argc > 2) {
    printf("Too many arguments\n");
    usage();
    exit(2);
  }

  int isNum = 1;
  int len = 0;
  for (int j = 0; argv[1][j] != '\0' && isNum; j++) {
    isNum = isdigit(argv[1][j]);
    len++;
  }
  if (!isNum) {
    printf("The argument %s is not a number\n", argv[1]);
    usage();
    exit(3);
  }

  if (len > 5) {
    printf("The argument should be between %d and %d\n", 0, (1<<16) - 1);
    usage();
    exit(4);
  }

  unsigned int labNum = atoi(argv[1]);

  if (labNum>>16 != 0) {
    printf("The argument should be between %d and %d\n", 0, (1<<16) - 1);
    usage();
    exit(4);
  }

  for (int dec = 15; dec >= 0; dec--) {
    printf("%d", (labNum >> dec)&1);
    if (dec % 4 == 0) {printf("\n");}
  }

  return 0;

}
