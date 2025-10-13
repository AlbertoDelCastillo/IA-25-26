/**
  * Universidad de La Laguna
  * Escuela Superior de Ingeniería y Tecnología
  * Grado en Ingeniería Informática
  * Inteligencia Artificial 2025-2026
  * 3º Año de Carrera
  * Practica 1: Busquedas no informadas
  *
  * @author Alberto Del Castillo Díaz alu0101627137@ull.edu.es
  * @date Sep 01 2025
  * @brief Programa cliente con BFS
  * @version 1.0
  */

#include <iostream>
#include <fstream>
#include <string>
#include "funciones_cliente.h"
#include "grafo.h"

int main(int argc, char* argv[]) {
  if (!CheckCorrectsParameters(argc, argv)) {
    return 1;
  }
  // 1. Cargar grafo
  Grafo grafo(argv[1]);
  // 2. Obtener parámetros de búsqueda
  int origen, destino;
  std::cout << "Ingrese vértice origen (1-" << grafo.numero_nodos() << "): ";
  std::cin >> origen;
  std::cout << "Ingrese vértice destino (1-" << grafo.numero_nodos() << "): ";
  std::cin >> destino;   
  // Convertir a 0-based
  origen--;
  destino--;
  // 3. Menu de busqueda
  if (argv[2] != nullptr) {
    Menu(grafo, origen, destino, argv[2]);
  } else {
    Menu(grafo, origen, destino, "");
  }
  return 0;
}