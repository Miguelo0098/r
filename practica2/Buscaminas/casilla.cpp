#include "casilla.hpp"
#include <iostream>

void Casilla::printCasilla(){
  if(this->isCovered()){ //Esta cubierta
    if(!this->getMark()) //No tiene ninguna bandera
      printf("-");
    else{
      if(this->getMark() == 1){ //Bandera de jugador A
        printf("A");
      }else if(this->getMark() == 2){ //Bandera de jugador B
          printf("B");
        }
      }
  }
  else{ // Esta descubierta
    if(this->hasBomb()) // Tiene bomba
      printf("*");
    else{ //No tiene bomba
      printf("%d", this->getNBombs());
    }
  }
}
