#include "tablero.hpp"

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
