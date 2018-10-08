#ifndef TABLERO_HPP
#define TABLERO_HPP

#include "casilla.hpp"

class Tablero {
private:
  Casilla tablero[100];


public:
  Tablero ();

  inline Casilla & getCasilla(int n){
    return tablero[n];
  }

  int getBombsTab();
  int getMarksTab();

  void resetTablero();
  void printTablero();

};

#endif
