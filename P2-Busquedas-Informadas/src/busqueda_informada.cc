/**
  * Universidad de La Laguna
  * Escuela Superior de Ingeniería y Tecnología
  * Grado en Ingeniería Informática
  * Inteligencia Artificial 2025-2026
  * 3º Año de Carrera
  * Practica 2: Busquedas informadas
  *
  * @author Alberto Del Castillo Díaz alu0101627137@ull.edu.es
  * @date 
  * @brief 
  * @version 1.0
  */

#include <vector>
#include <set>
#include <utility>
#include "busqueda_informada.h"

bool BusquedaInformada::BusquedaAStar() {
  InicializarMatrizCostes();
  std::vector<nodo> A{};  // Lista de nodos abiertos
  std::set<nodo> C{};     // Lista de nodos cerrados
  
  // Paso 1: Calcular f(n), g(n) y h(n) para el punto de entrada S
  std::pair<size_t, size_t> pos_actual = laberinto_->coordenadas_start();
  double g_cost = 0; 
  double h_cost = laberinto_->Heuristica(pos_actual.first, pos_actual.second);
  
  nodo S(pos_actual, g_cost, h_cost, {-1, -1});
  std::cerr << "Nodo inicial: " << S;
  matriz_costes_[pos_actual.first][pos_actual.second] = S;
  A.push_back(S);  // Insertar en la lista de nodos abiertos
  
  // Paso 2: Repetir mientras A no esté vacía
  while (!A.empty()) {
    // Paso 2(a): Seleccionar el nodo de menor coste f(n)
    auto it_mejor = A.begin();
    for (auto it = A.begin(); it != A.end(); ++it) {
      if (it->f_cost < it_mejor->f_cost) {
        it_mejor = it;
      }
    }
    nodo actual = *it_mejor;
    A.erase(it_mejor);  // Remover de A
    
    // Insertarlo en la lista de nodos cerrados C
    C.insert(actual);
    std::cerr << "Procesando nodo: " << actual;
    
    // Verificar si llegamos a la salida
    if (actual.posicion == laberinto_->coordenadas_exit()) {
      std::cout << "¡Camino encontrado!" << std::endl;
      return true;
    }
    
    // Paso 2(b): Para cada nodo vecino
    std::vector<std::pair<size_t, size_t>> vecinos = 
        laberinto_->GetVecinosCasilla(actual.posicion.first, actual.posicion.second);
    
    for (const auto& vecino_pos : vecinos) {
      size_t vecino_fila = vecino_pos.first;
      size_t vecino_col = vecino_pos.second;
      
      // Calcular costes para el vecino
      double move_cost = laberinto_->MoveCost(actual.posicion.first, actual.posicion.second, 
                                             vecino_fila, vecino_col);
      double g_cost_vecino = actual.g_cost + move_cost;
      double h_cost_vecino = laberinto_->Heuristica(vecino_fila, vecino_col);
      
      nodo vecino(vecino_pos, g_cost_vecino, h_cost_vecino, 
                 {static_cast<int>(actual.posicion.first), static_cast<int>(actual.posicion.second)});
      
      // Verificar si el nodo está en C (cerrados)
      bool en_C = (C.find(vecino) != C.end());
      
      // Verificar si el nodo está en A (abiertos)
      bool en_A = false;
      auto it_en_A = A.end();
      for (auto it = A.begin(); it != A.end(); ++it) {
        if (it->posicion == vecino_pos) {
          en_A = true;
          it_en_A = it;
          break;
        }
      }
      
      // Paso 2(b)i: Si el nodo no está ni en A ni en C
      if (!en_A && !en_C) {
        vecino.padre = {static_cast<int>(actual.posicion.first), static_cast<int>(actual.posicion.second)};
        matriz_costes_[vecino_fila][vecino_col] = vecino;
        A.push_back(vecino);  // Insertar en A
        std::cerr << "  Nuevo nodo añadido a A: " << vecino;
      }
      // Paso 2(b)ii: Si el nodo está en A
      else if (en_A) {
        // Verificar si encontramos un camino mejor (menor g_cost)
        if (g_cost_vecino < it_en_A->g_cost) {
          std::cerr << "  Actualizando nodo en A: g_cost mejorado de " 
                    << it_en_A->g_cost << " a " << g_cost_vecino << std::endl;
          
          // Actualizar coste g(n) y por lo tanto f(n)
          it_en_A->g_cost = g_cost_vecino;
          it_en_A->f_cost = g_cost_vecino + it_en_A->h_cost;
          it_en_A->padre = {static_cast<int>(actual.posicion.first), static_cast<int>(actual.posicion.second)};
          
          // Actualizar también en la matriz de costes
          matriz_costes_[vecino_fila][vecino_col] = *it_en_A;
        }
      }
      // Si el nodo está en C, no hacemos nada (ya fue procesado)
    }
  }
  
  // Paso 3: Si A está vacía y no se llegó a la salida
  std::cout << "No existe camino desde la entrada hasta la salida." << std::endl;
  return false;
}

void BusquedaInformada::InicializarMatrizCostes() {
  size_t filas = laberinto_->filas();
  size_t cols = laberinto_->columnas();
  
  matriz_costes_.clear();
  matriz_costes_.resize(filas, std::vector<nodo>(cols));
}
