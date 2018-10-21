#ifndef CASILLA_HPP
#define CASILLA_HPP

#include <string>

class Casilla {
private:
  int _n_bombs;
  bool _bomb;
  int _mark;
  bool _uncovered;

public:
  /* Constructor */
  inline Casilla (int n_bombs = 0, bool bomb = false, int mark = 0, bool uncovered = false){
    this->setNBombs(n_bombs);
    this->setBomb(bomb);
    this->setMark(mark);
    this->setCover(uncovered);
  }

  /* Observadores */
  inline int getNBombs(){
    return this->_n_bombs;
  }

  inline bool hasBomb(){
    return this->_bomb;
  }

  inline int getMark(){
    return this->_mark;
  }

  inline bool isCovered(){
    return this->_uncovered;
  }

  /* Modificadores */

  inline void setNBombs(int n_bombs){
    this->_n_bombs = n_bombs;
  }

  inline void setBomb(bool bomb){
    this->_bomb = bomb;
  }

  inline void setMark(int mark){
    this->_mark = mark;
  }

  inline void setCover(bool uncovered){
    this->_uncovered = uncovered;
  }

  /* Para imprimir casilla */
  string printCasilla();
};

#endif
