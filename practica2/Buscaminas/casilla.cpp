#include "casilla.hpp"
#include <iostream>

void Casilla::printCasilla()const{
  if(isCovered()){ //Esta cubierta
    if(!getMark()) //No tiene ninguna bandera
      printf("-");
    else{
      if(getMark() == 1){ //Bandera de jugador A
        printf("A");
      }else{
        if(getMark() == 2){ //Bandera de jugador B
          printf("B");
        }else //Bandera de ambos
          printf("A|B");
      }
    }
  }else{ // Esta descubierta
    if(hasBomb()) // Tiene bomba
      printf("*");
    else{ //No tiene bomba
      printf(getNBombs());
    }
  }
}
