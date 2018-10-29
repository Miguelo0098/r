#
CPPFLAGS = -c -g -lrt -ansi -O2 -std=c++11

# Macros predefinidas
#
# $@: nombre del objetivo
# $^: todas las dependencias
# $<: primera dependencia
#

objetivo: buscaminas

# Primer objetivo
#
buscaminas: casilla.cpp tablero.cpp server.cpp
		@echo "Compilando  cliente"
		@g++ cliente.cpp -o cliente
		@echo "Compilando  server"
		@g++ $^ -o server
