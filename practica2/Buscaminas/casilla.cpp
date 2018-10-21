#include "casilla.hpp"
#include <iostream>

string Casilla::printCasilla(){
  if(this->isCovered()){ //Esta cubierta
    if(!this->getMark()) //No tiene ninguna bandera
      return "-";
    else{
      if(this->getMark() == 1){ //Bandera de jugador A
        return "A";
      }else if(this->getMark() == 2){ //Bandera de jugador B
          return "B";
        }
      }
  }
  else{ // Esta descubierta
    if(this->hasBomb()) // Tiene bomba
      return "*";
    else{ //No tiene bomba
      return std::to_string(this->getNBombs());
    }
  }
}
