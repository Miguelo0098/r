#include "casilla.hpp"
#include <iostream>

void Casilla::printCasilla()const{
  if(this->isCovered()){ //Esta cubierta
    if(!this->getMark()) //No tiene ninguna bandera
      printf("-");
    else{
      if(this->getMark() == 1){ //Bandera de jugador A
        printf("A");
      }else{
        if(this->getMark() == 2){ //Bandera de jugador B
          printf("B");
        }else //Bandera de ambos
          printf("A|B");
      }
    }
  }else{ // Esta descubierta
    if(this->hasBomb()) // Tiene bomba
      printf("*");
    else{ //No tiene bomba
      printf(this->getNBombs());
    }
  }
}
