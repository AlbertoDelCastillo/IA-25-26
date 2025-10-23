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
  // Eliminar copia y asignación para evitar problemas con el puntero
  BusquedaInformada(const BusquedaInformada&) = delete;
  BusquedaInformada& operator=(const BusquedaInformada&) = delete;
  //Metodo de busqueda A*
  bool BusquedaAStar(const std::pair<size_t, size_t>&);
  bool BusquedaAStarDinamica();
  // Getters
  const std::vector<std::pair<size_t, size_t>>& GetCamino() const { 
    return camino_encontrado_; 
  }
  size_t GetNodosGenerados() const { return nodos_generados_; }
  size_t GetNodosInspeccionados() const { return nodos_inspeccionados_; }
  // Impresión
  void ImprimirCamino(std::ostream& os = std::cout) const;
  void ImprimirEstadisticas(std::ostream& os = std::cout) const;
  void ImprimirLaberintoConCamino(std::ostream& os = std::cout) const;
  void ImprimirLaberintoConCaminoYAgente(
    std::ostream& os,
    const std::pair<size_t, size_t>& posicion_agente,
    const std::vector<std::pair<size_t, size_t>>& camino_historico = {}
  ) const;
  void GenerarReporteCompleto(const std::string& nombre_instancia, 
                           const std::string& nombre_heuristica,
                           std::ostream& os = std::cout) const;

 private:
  Laberinto* laberinto_{};
  std::vector<std::pair<size_t, size_t>> camino_encontrado_{};
  size_t nodos_generados_{};
  size_t nodos_inspeccionados_{};
  std::vector<std::vector<nodo>> matriz_costes_{};
  // Metodos auxiliares
  void InicializarMatrizCostes();
  std::vector<std::pair<size_t, size_t>> ReconstruirCamino(
    const std::pair<size_t, size_t>& meta
  );
  void ImprimirResumenFinal(std::ostream& os,
                           size_t iteraciones,
                           size_t pasos,
                           size_t nodos_gen,
                           size_t nodos_insp,
                           const std::vector<std::pair<size_t, size_t>>& camino,
                           bool exito) const;
};

#endif