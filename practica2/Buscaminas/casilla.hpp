#ifndef CASILLA_HPP
#define CASILLA_HPP

class Casilla {
private:
  int _n_bombs;
  bool _bomb;
  bool _mark;
  bool _uncovered;

public:
  /* Constructor */
  inline Casilla (int nBombas = 0, bool bomba = false, bool marca = false, bool descubierto = false){
    setNBombs(nBombas);
    setBomb(bomba);
    setMark(marca);
    setCover(descubierto);
  }

  /* Observadores */
  inline int getNBombs(){
    return _n_bombs;
  }

  inline bool hasBomb(){
    return _bomb;
  }

  inline bool hasMark(){
    return _mark;
  }

  inline bool isCovered(){
    return _uncovered;
  }

  /* Modificadores */

  inline void setNBombs(int nBombas){
    _n_bombs = nBombas;
  }

  inline void setBomb(bool bomba){
    _bomb = bomba;
  }

  inline void setMark(bool marca){
    _mark = marca;
  }

  inline void setCover(bool descubierto){
    _uncovered = descubierto;
  }

  /* Para imprimir casilla */
  void printCasilla();
};

#endif
