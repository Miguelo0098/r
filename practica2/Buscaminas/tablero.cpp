#include "tablero.hpp"
#include <ctime>
#include <cstdlib>

int Tablero::getBombsTab(){
  int count = 0;
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (getCasilla(i, j).hasBomb() == true) {
        count++;
      }
    }
  }
  return count;
}

int Tablero::getMarksTabA(){
  int count = 0;
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (getCasilla(i, j).getMark() == 1) {
        count++;
      }
    }
  }
  return count;
}

int Tablero::getMarksTabB(){
  int count = 0;
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (getCasilla(i, j).getMark() == 2) {
        count++;
      }
    }
  }
  return count;
}

void Tablero::resetTablero(){
  srand(time(NULL));
  int rnumber = -1;
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      getCasilla(i, j).setBomb(false);
      getCasilla(i, j).setNBombs(0);
      getCasilla(i, j).setMark(0);
      getCasilla(i, j).setCover(false);
    }
  }

  while (getBombsTab() < 20) {
    rnumber = rand() % 100;
    getCasilla(rnumber/10, rnumber%10).setBomb(true);
  }
}
