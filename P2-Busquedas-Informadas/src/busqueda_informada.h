/**
  * Universidad de La Laguna
  * Escuela Superior de Ingeniería y Tecnología
  * Grado en Ingeniería Informática
  * Inteligencia Artificial 2025-2026
  * 3º Año de Carrera
  * Practica 2: Busquedas informadas
  *
  * @author Alberto Del Castillo Díaz alu0101627137@ull.edu.es
  * @date 2025-10-23
  * @brief Definición de la clase BusquedaInformada para algoritmo A*
  * @version 1.0
  */

#ifndef BUSQUEDA_INFORMADA_H
#define BUSQUEDA_INFORMADA_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include "laberinto.h"

/**
 * @struct nodo
 * @brief Representa un nodo en el árbol de búsqueda A*
 * 
 * Estructura que almacena toda la información necesaria para un nodo
 * durante la ejecución del algoritmo A*:
 * - Posición en el laberinto
 * - Costes g(n), h(n) y f(n)
 * - Referencia al nodo padre para reconstrucción del camino
 * 
 * La función de evaluación es: f(n) = g(n) + h(n)
 * donde:
 * - g(n) = coste acumulado desde el inicio hasta n
 * - h(n) = estimación heurística del coste de n hasta la meta
 * - f(n) = estimación del coste total del camino pasando por n
 */
typedef struct nodo {
  std::pair<size_t, size_t> posicion{};
  double g_cost{}, h_cost{}, f_cost{};
  std::pair<int, int> padre{-1, -1};
  // Metodos
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
 * @class BusquedaInformada
 * @brief Implementa el algoritmo de búsqueda A* para laberintos
 * 
 * Esta clase proporciona dos modalidades de búsqueda:
 * 
 * 1. **A* Estático**: Búsqueda única en un laberinto sin cambios
 * 2. **A* Dinámico**: Búsqueda iterativa donde el entorno cambia después
 *                     de cada paso del agente
 */
class BusquedaInformada {
 public:
  BusquedaInformada() = default;
  BusquedaInformada(Laberinto& laberinto) : laberinto_{&laberinto} {}
  BusquedaInformada(const BusquedaInformada&) = delete;
  BusquedaInformada& operator=(const BusquedaInformada&) = delete;
  //Metodo de busqueda A* con manhattan
  bool BusquedaAStar(const std::pair<size_t, size_t>&);
  bool BusquedaAStarDinamica(const std::string&);
  //Metodo de busqueda A* con oricle
  bool BusquedaAStarH2(const std::pair<size_t, size_t>&);
  bool BusquedaAStarDinamicaH2(const std::string&);
  // Google style getters
  const std::vector<std::pair<size_t, size_t>>& camino_encontrado() const { 
    return camino_encontrado_; 
  }
  size_t nodos_generados() const { return nodos_generados_; }
  size_t nodos_inspeccionados() const { return nodos_inspeccionados_; }
  // Impresión
  void ImprimirLaberintoConCaminoYAgente(std::ostream&, const std::pair<size_t, size_t>&,
                                          const std::vector<std::pair<size_t, size_t>>& camino_historico = {}
                                        ) const;
  void GenerarReporteCompleto(const std::string&, const std::string&,
                              std::ostream& os = std::cout) const;

 private:
  Laberinto* laberinto_{};
  std::vector<std::pair<size_t, size_t>> camino_encontrado_{};
  size_t nodos_generados_{};
  size_t nodos_inspeccionados_{};
  std::vector<nodo> A_{};
  std::set<std::pair<size_t, size_t>> C_{};
  std::vector<std::vector<nodo>> matriz_costes_{};
  
  // Metodos auxiliares
  void InicializarMatrizCostes();
  std::vector<std::pair<size_t, size_t>> ReconstruirCamino(const std::pair<size_t, size_t>&);
  void ImprimirResumenFinal(std::ostream&, size_t, size_t, size_t, size_t, double,
                            const std::vector<std::pair<size_t, size_t>>&, 
                            bool) const;
  void ImprimirNodosGeneradosEInspeccionados(std::ostream&, const std::vector<nodo>&, const std::set<std::pair<size_t, size_t>>&) const;
};

#endif