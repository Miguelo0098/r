#include "casilla.hpp"
#include <iostream>

std::string Casilla::printCasilla(){
    std::string retval;
  if(this->isCovered()){ //Esta cubierta
    if(!this->getMark()) //No tiene ninguna bandera
      retval = "-";
    else{
      if(this->getMark() == 1){ //Bandera de jugador A
        retval = "A";
      }else if(this->getMark() == 2){ //Bandera de jugador B
          retval = "B";
        }
      }
  }
  else{ // Esta descubierta
    if(this->hasBomb()) // Tiene bomba
      retval = "*";
    else{ //No tiene bomba
      retval = std::to_string(this->getNBombs());
    }
  }
  return retval;
}
