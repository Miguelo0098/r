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
  /* Vacía el tablero */
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      getCasilla(i, j).setBomb(false);
      getCasilla(i, j).setNBombs(0);
      getCasilla(i, j).setMark(0);
      getCasilla(i, j).setCover(false);
    }
  }
  /* Rellena el tablero con 20 bombas */
  while (getBombsTab() < 20) {
    rnumber = rand() % 100;
    getCasilla(rnumber/10, rnumber%10).setBomb(true);
  }

  /* Contamos el número de bombas alrededor de cada casilla */
  int n_bombs = 0;
  for (int i = 1; i < 9; i++) {
    for (int j = 1; j < 9; j++) {
      n_bombs = 0;
      for (int ip = -1; ip < 2; ip++) {
        for (int jp = -1; jp < 2; jp++) {
          /* code */
        }
      }
    }
  }
}
