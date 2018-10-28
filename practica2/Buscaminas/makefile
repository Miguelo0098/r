#
OBJECTS = tablero.o casilla.o

#
CPPFLAGS = -c -g -Wall -lrt -ansi -O2 -std=c++11

INCLUDES = casilla.cpp tablero.cpp

# Macros predefinidas
#
# $@: nombre del objetivo
# $^: todas las dependencias
# $<: primera dependencia
#

objetivo: server

# Primer objetivo
casilla.o: casilla.cpp casilla.hpp
			@echo "Compilando " $<
			@g++ $(CPPFLAGS) $<

#
tablero.o: tablero.cpp tablero.hpp
			@echo "Compilando " $<
			@g++ $(CPPFLAGS) $<

#
cliente: cliente.cpp
		@echo "Compilando " $<
		@g++ $(CPPFLAGS) $<

#
server: server.cpp casilla.o tablero.o
		@echo "Compilando " $<
		@g++ $(CPPFLAGS) $<

# Borrado
.PHONY: clean
clean:
	@echo "Borrando los ficheros superfluos"
	@rm -f $(OBJECTS) *~
