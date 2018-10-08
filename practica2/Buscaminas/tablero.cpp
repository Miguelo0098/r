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
