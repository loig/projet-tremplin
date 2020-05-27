#include "labs.h"

void lefthand_algorithm() {
  while(true) {
    if (is_move_forward_possible()) {
      move_forward();
    } else {
      turn_left();
    }
    print_terminal("test");
  }
}

void pledge_algorithm() {}

void personal_algorithm() {}
