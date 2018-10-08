#ifndef CASILLA_HPP
#define CASILLA_HPP

class Casilla {
private:
  int _n_bombs;
  bool _bomb;
  int _mark;
  bool _uncovered;

public:
  /* Constructor */
  inline Casilla (int n_bombs = 0, bool bomb = false, int mark = 0, bool uncovered = false){
    setNBombs(n_bombs);
    setBomb(bomb);
    setMark(mark);
    setCover(uncovered);
  }

  /* Observadores */
  inline int getNBombs(){
    return _n_bombs;
  }

  inline bool hasBomb(){
    return _bomb;
  }

  inline int getMark(){
    return _mark;
  }

  inline bool isCovered(){
    return _uncovered;
  }

  /* Modificadores */

  inline void setNBombs(int n_bombs){
    _n_bombs = n_bombs;
  }

  inline void setBomb(bool bomb){
    _bomb = bomb;
  }

  inline void setMark(int mark){
    _mark = mark;
  }

  inline void setCover(bool uncovered){
    _uncovered = uncovered;
  }

  /* Para imprimir casilla */
  void printCasilla();
};

#endif
