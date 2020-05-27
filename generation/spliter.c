// Get the numbers associated to a 12x12 lab
// when splited as 9 4x4 labs
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int getPos(int num) {
  if (num < 4) {return 0;}
  if (num < 8) {return 1;}
  return 2;
}

int getVal(char c) {
  if (c == '0') {return 0;}
  return 1;
}

int main() {

  char line[13];

  int labs[3][3];
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      labs[i][j] = 0;
    }
  }

  for (int lineNum = 0; lineNum < 12; lineNum++) {
    ssize_t charRead = read(STDIN_FILENO, line, 13);
    if (charRead != 13) {
      printf("%d characters read\n", (int)charRead);
      exit(1);
    }
    for (int charNum = 0; charNum < 12; charNum++) {
      int i = getPos(lineNum);
      int j = getPos(charNum);
      labs[i][j] = 2 * labs[i][j] + getVal(line[charNum]);
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      printf("%d ", labs[i][j]);
    }
    printf("\n");
  }


  return 0;
}
