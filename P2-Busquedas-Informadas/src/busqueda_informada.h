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

#ifndef BUSQUEDA_INFORMADA_H
#define BUSQUEDA_INFORMADA_H

#include <iostream>
#include <fstream>
#include <set>
#include <queue>
#include <utility>
#include "laberinto.h"

typedef struct nodo {
  std::pair<size_t, size_t> posicion{};
  double g_cost{}, h_cost{}, f_cost{};
  std::pair<int, int> padre{-1, -1};
  nodo() = default;
  nodo(std::pair<size_t, size_t> pos, double g, double h, std::pair<int, int> p) 
  : posicion{pos}, g_cost{g}, h_cost{h}, f_cost{g + h}, padre(p) {}
  
  // Operador < necesario para std::set
  bool operator<(const nodo& other) const {
    if (posicion.first != other.posicion.first) {
      return posicion.first < other.posicion.first;
    }
    return posicion.second < other.posicion.second;
  }
  
  friend std::ostream& operator<<(std::ostream& os, const nodo& n) {
    os << "pos=(" << n.posicion.first << "," << n.posicion.second << ") "
       << "g=" << n.g_cost << " h=" << n.h_cost << " f=" << n.f_cost << "\n";
    return os;
  }
} nodo;

/**
 * 
 * 
 */
class BusquedaInformada {
 public:
  BusquedaInformada() = default;
  BusquedaInformada(Laberinto& laberinto) : laberinto_{&laberinto} {}
  //Metodo de busqueda A*
  bool BusquedaAStar();
  
 private:
  Laberinto* laberinto_{};
  std::vector<std::pair<size_t, size_t>> camino_encontrado_{};
  size_t nodo_generados_{};
  size_t nodo_inspeccionados_{};
  std::vector<std::vector<nodo>> matriz_costes_{};
  // Metodos auziliares
  void InicializarMatrizCostes();

};

#endif