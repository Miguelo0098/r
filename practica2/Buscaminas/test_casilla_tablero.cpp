#include "tablero.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>

using namespace std;

int main(int argc, char const *argv[]) {
  Tablero tablero;
  tablero.resetTablero();
  printf("%s\n", tablero.printTablero().c_str());
  return 0;
}
