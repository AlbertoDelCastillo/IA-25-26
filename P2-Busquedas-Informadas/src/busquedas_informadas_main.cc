/**
  * Universidad de La Laguna
  * Escuela Superior de Ingeniería y Tecnología
  * Grado en Ingeniería Informática
  * Inteligencia Artificial 2025-2026
  * 3º Año de Carrera
  * Practica 2: Busquedas informadas
  *
  * @author Alberto Del Castillo Díaz alu0101627137@ull.edu.es
  * @date Oct 11 2025
  * @brief Programa cliente 
  * @version 1.0
  */

#include <iostream>
#include <fstream>
#include <string>
#include "funciones_cliente.h"
#include "laberinto.h"

int main(int argc, char* argv[]) {
  if (!CheckCorrectsParameters(argc, argv)) {
    return 1;
  }
  PrintProgramPorpouse();
  Laberinto laberinto{argv[1]};
  std::string nombre_output{""};
  if (argc == 3) {
    nombre_output = {argv[2]};
  }
  Menu(laberinto, nombre_output);
  return 0;
}