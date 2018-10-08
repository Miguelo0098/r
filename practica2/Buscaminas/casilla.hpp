#ifndef CASILLA_HPP
#define CASILLA_HPP

class Casilla {
private:
  int n_bombs;
  bool bomb;
  bool mark;

public:
  /* Constructor */
  inline Casilla (int nBombas = 0, bool bomba = false, bool marca = false){
    setNBombs(nBombas);
    setBomb(bomba);
    setMark(marca);
  }

  /* Observadores */
  inline int getNBombs(){
    return n_bombs;
  }

  inline bool hasBomb(){
    return bomb;
  }

  inline bool hasMark(){
    return mark;
  }

  /* Modificadores */

  inline void setNBombs(int nBombas){
    n_bombs = nBombas;
  }

  inline void setBomb(bool bomba){
    bomb = bomba;
  }

  inline void setMark(bool marca){
    mark = marca;
  }

  /* Para imprimir casilla */
  void printCasilla();
};

#endif
