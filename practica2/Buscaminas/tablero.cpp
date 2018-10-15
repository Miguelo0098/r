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

  /* Para las 64 bombas del centro */
  for (int i = 1; i < 9; i++) {
    for (int j = 1; j < 9; j++) {
      n_bombs = 0;
      for (int ip = -1; ip < 2; ip++) {
        for (int jp = -1; jp < 2; jp++) {
          if (getCasilla(i+ip, j+jp).hasBomb() == true) {
            n_bombs++;
          }
        }
      }
      getCasilla(i, j).setNBombs(n_bombs);
    }
  }

  /* Para los bordes de arriba e izquierda */
  int n_bombs2 = 0
  for (int i = 1; i < 10; i++) {
    n_bombs = 0;
    n_bombs2 = 0;
    for (int ip = -1; ip < 2; ip++) {
      for (int jp = 0; jp < 2; jp++) {
        if (getCasilla(i+ip, jp).hasBomb() == true) {
          n_bombs++;
        }
        if (getCasilla(jp, i+ip).hasBomb() == true) {
          n_bombs2++;
        }
      }
    }
    getCasilla(i, 0).setNBombs(n_bombs);
    getCasilla(0, i).setNBombs(n_bombs2);
  }

  /* Para los bordes de abajo y derecha */
  for (int i = 1; i < 10; i++) {
    n_bombs = 0;
    n_bombs2 = 0;
    for (int ip = -1; ip < 2; ip++) {
      for (int jp = 9; jp < 11; jp++) {
        if (getCasilla(i+ip, jp).hasBomb() == true) {
          n_bombs++;
        }
        if (getCasilla(jp, i+ip).hasBomb() == true) {
          n_bombs2++;
        }
      }
    }
    getCasilla(i, 10).setNBombs(n_bombs);
    getCasilla(10, i).setNBombs(n_bombs2);
  }

  /* Rellenamos las esquinas */
  n_bombs = 0;
  n_bombs2 = 0;
  int n_bombs3 = 0;
  int n_bombs4 = 0;

  for (i = 0; i < 2; i++) {
    for (j = 0; j < 2; j++) {
      if (getCasilla(i, j).hasBomb() == true) {
        n_bombs++;
      }
      if (getCasilla(10-i, j).hasBomb() == true) {
        n_bombs2++;
      }
      if (getCasilla(i, 10-j).hasBomb() == true) {
        n_bombs3++;
      }
      if (getCasilla(10-i, 10-j).hasBomb() == true) {
        n_bombs4++;
      }
    }
  }
  getCasilla(0, 0).setNBombs(n_bombs);
  getCasilla(10, 0).setNBombs(n_bombs2);
  getCasilla(0, 10).setNBombs(n_bombs3);
  getCasilla(10, 10).setNBombs(n_bombs4);

}
