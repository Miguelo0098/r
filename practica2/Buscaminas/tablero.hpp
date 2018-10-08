#ifndef TABLERO_HPP
#define TABLERO_HPP

#include "casilla.hpp"

class Tablero {
private:
  Casilla _tablero[10][10];


public:
  Tablero ();

  inline Casilla & getCasilla(int n, int m){
    return tablero[n][m];
  }

  int getBombsTab();
  int getMarksTab();

  void resetTablero();
  void printTablero();

};

#endif
