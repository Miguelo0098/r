#ifndef TABLERO_HPP
#define TABLERO_HPP

#include "casilla.hpp"

class Tablero {
private:
  Casilla _tablero[10][10];
  int _jugadorA;
  int _jugadorB;
  int _turno;


public:
  inline Tablero (){
    resetTablero();
  }

  inline Casilla & getCasilla(int n, int m){
    return _tablero[n][m];
  }

  inline int getJugadorA(){
    return _jugadorA;
  }

  inline int getJugadorB(){
    return _jugadorB;
  }

  inline int getTurno(){
      return _turno;
  }

  inline void setJugadorA(int jugadorA){
    _jugadorA = jugadorA;
  }

  inline void setJugadorB(int jugadorB){
    _jugadorB = jugadorB;
  }

  inline void setTurno(int turno){
      _turno = turno;
  }

  int getBombsTab();
  int getMarksTabA();
  int getMarksTabB();

  void resetTablero();
  std::string printTablero(); // Envia el tablero como una cadena para el buffer

};

#endif
