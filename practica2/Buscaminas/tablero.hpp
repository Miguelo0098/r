#ifndef TABLERO_HPP
#define TABLERO_HPP

#include "casilla.hpp"

class Tablero {
private:
  Casilla _tablero[10][10];


public:
  inline Tablero (){
    
  }

  inline Casilla & getCasilla(int n, int m){
    return _tablero[n][m];
  }

  int getBombsTab();
  int getMarksTabA();
  int getMarksTabB();

  void resetTablero();
  std::string printTablero(); // Envia el tablero como una cadena para el buffer

};

#endif
